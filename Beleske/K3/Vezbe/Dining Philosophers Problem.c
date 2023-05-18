// Resenje 1 - svaka viljuska je svoj proces i svaka viljuska vodi racuna o sebi, pa nam ne treba proces koordinator.
// Svaka viljuska cita iz 2 postanska sanduceta.
// Mrtvu blokadu cemo izbeci tako sto ce svi filozofi prvo traziti levu pa desnu viljusku, dok ce poslednji filozof jedini
// traziti prvo desnu pa levu. Na taj nacin smo sprecili kruzno blokiranje procesa.

const int N = 5;

typedef struct msg {
    int senderId;
} msg;

mbx philosophers[N];
void Philosopher(int id) {
    int firstFork, secondFork;
    msg message, responseMessage;
    bool status;

    if (id == N - 1) {
        firstFork = 0;
        secondFork = id;
    } else {
        firstFork = id;
        secondFork = id + 1;
    }

    while (true) {
        think();

        // Filozof trazi viljuske.
        message.senderId = id;
        mbx_put(message, getFork[firstFork]);
        mbx_get(responseMessage, philosophers[id], INF, status);
        mbx_put(message, getFork[secondFork]);
        mbx_get(responseMessage, philosophers[id], INF, status);

        eat();

        // Filozof javlja da je gotov sa jelom.
        mbx_put(message, returnFork[firstFork]);
        mbx_put(message, returnFork[secondFork]);
    }
}

// Za svaku viljusku cemo imati po dva sanduceta - jedno u koje ce stizati poruke od filozofa koji zele da dobiju tu viljusku, i
// drugo u koje ce stizati poruke od filozofa koji zele da vrate tu viljusku. Ovo smo uradili da bismo mogli u funkciji Fork
// da razlikujemo ta dva tipa poruka.
mbx getFork[N], returnFork[N];
void Fork(int id) {
    msg message;
    bool status;

    while (true) {
        // Cekamo da filozof zatrazi viljusku.
        mbx_get(message, getFork[id], INF, status);
        // Javljamo filozofu da moze da koristi viljusku.
        mbx_put(message, philosophers[message.senderId]);
        // Cekamo da filozof vrati viljusku.
        mbx_get(message, returnFork[id], INF, status);
    }
}


// Resenje 2 - jedna viljuska moze da cita iz samo jednog postanskog sanduceta.

const int N = 5;

typedef struct msg {
    int senderId;
    char* operation;
} msg;

mbx philosophers[N];
void Philosopher(int id) {
    int firstFork, secondFork;
    msg message, responseMessage;
    bool status;

    if (id == N - 1) {
        firstFork = 0;
        secondFork = id;
    } else {
        firstFork = id;
        secondFork = id + 1;
    }

    while (true) {
        think();

        // Filozof trazi viljuske.
        message.senderId = id;
        message.operation = "startEating";
        mbx_put(message, getFork[firstFork]);
        mbx_get(responseMessage, philosophers[id], INF, status);
        mbx_put(message, getFork[secondFork]);
        mbx_get(responseMessage, philosophers[id], INF, status);

        eat();

        // Filozof javlja da je gotov sa jelom.
        message.senderId = id;
        message.operation = "endEating";
        mbx_put(message, returnFork[firstFork]);
        mbx_put(message, returnFork[secondFork]);
    }
}

// Na neku viljusku moze da ceka maksimalno jedan filozof - logicno, samo sused od filozofa koji koristi tu viljusku, i to sa
// strane na kojoj je viljuska.

mbx forks[N];
void Fork(int id) {
    msg message;
    bool status;
    msg philosopherWaiting = NULL;
    while (true) {
        if (philosopherWaiting == NULL) {
            // U ovu granu ulazimo ako nema filozofa koji ceka ovu viljusku od kad je prosli filozof nju koristio.
            // Cekamo da neki filozof zatrazi viljusku.
            mbx_get(message, getFork[id], INF, status);
        } else {
            // U ovu granu ulazimo ako ima filozofa koji ceka ovu viljusku od kad je prosli filozof nju koristio, pa hocemo
            // njemu da je damo na koriscenje.
            message = philosopherWaiting;
            philosopherWaiting = NULL;
        }
        
        // Javljamo filozofu da moze da koristi viljusku.
        mbx_put(message, philosophers[message.senderId]);
        // Ukoliko je message.operation == "startEating" ovde, to znaci da je neki filozof zatrazio ovu viljusku dok
        // nju koristi neki drugi filozof. Ukoliko je message.operation == "endEating", to samo znaci da je filozof
        // koji je koristio ovu viljusku javio da je on vise ne koristi.
        mbx_get(message, returnFork[id], INF, status);  // tip ove poruke moze biti i startEating i endEating
        if (message.operation == "startEating") {
            philosopherWaiting = message.senderId;
            // Cekamo da filozof koji je koristio ovu viljusku nju i vrati. 
            mbx_get(message, returnFork[id], INF, status);  // tip ove poruke je sigurno endEating
        }
    }
}