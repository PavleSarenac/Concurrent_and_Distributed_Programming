// Resenje 1 - jedan proces moze da cita iz vise razlicitih postanskih sanducica - proces koordinator ce imati dva sanduceta.
// Da bismo povecali konkurentnost, bice nam potreban proces koordinator.
// U slucaju da je citao odredjeni broj citalaca, njihove poruke kojima javljaju da su gotovi ce biti u sanducetu endCoordinator 
// i nece biti procitane, a samim tim ni brojac citalaca nece biti azuriran sve dok ne dodje neki pisac koji zeli da pise.
// Analogno je i za poruku od pisca kojom javlja da je gotov. Ovo zapravo i nije problem, program ce raditi ispravno, samo 
// brojaci pisaca i citalaca nece biti uvek azurni, tj. nece predstavljati trenutnu situaciju - ali to nije ni vazno.

const int R = ...;  // broj citalaca
const int W = ...;  // broj pisaca

typedef struct msg {
    int senderId;
    char* operation;
} msg;

// Svaki citalac ima svoje postansko sanduce.
mbx readers[R];
void reader(int readerId) {
    msg message;
    bool status;

    while (true) {
        message.senderId = readerId;
        message.operation = "startRead";
        mbx_put(message, startCoordinator);
        mbx_get(message, readers[readerId], INF, status);

        read();

        // Ne moramo u ovoj poruci da naglasavamo da je u pitanju javljanje zavrsetka citanja jer ce proces koordinator
        // znati na osnovu brojaca citalaca i pisaca ko mu je javio da je zavrsio - citalac ili pisac.
        mbx_put(message, endCoordinator);
    }
}

// Svaki pisac ima svoje postansko sanduce.
mbx writers[W];
void writer(int writerId) {
    msg message;
    bool status;

    while (true) {
        message.senderId = readerId;
        message.operation = "startWrite";
        mbx_put(message, startCoordinator);
        mbx_get(message, readers[readerId], INF, status);
        
        write();

        // Ne moramo u ovoj poruci da naglasavamo da je u pitanju javljanje zavrsetka pisanja jer ce proces koordinator
        // znati na osnovu brojaca citalaca i pisaca ko mu je javio da je zavrsio - pisac ili citalac.
        mbx_put(message, endCoordinator);
    }
}

mbx startCoordinator, endCoordinator;
void coordinator() {
    msg messageStart, messageEnd;
    msg emptyMessage;
    bool status;
    int numOfReaders = 0;
    int numOfWriters = 0;

    while (true) {
        mbx_get(messageStart, startCoordinator, INF, status);
        if (messageStart.operation == "startRead") {
            // Ako hoce citalac da cita a neki pisac pise, citalac mora da saceka da pisac zavrsi.
            if (numOfWriters > 0) {
                mbx_get(messageEnd, endCoordinator, INF, status);
                numOfWriters--;
            }
            // Citalac moze da cita, pa mu javljamo da pocne.
            numOfReaders++;
            mbx_put(emptyMessage, readers[messageStart.senderId]);
        } else if (messageStart.operation == "startWrite") {
            // Ako pisac hoce da pise, a vec postoji neki pisac koji pise, ovaj novi mora da saceka da prosli zavrsi.
            if (numOfWriters > 0) {
                mbx_get(messageEnd, endCoordinator, INF, status);
                numOfWriters--;
            }
            // Ako pisac hoce da pise, a vec ima citalaca koji citaju, on mora da saceka da svi oni zavrse.
            while (numOfReaders > 0) {
                mbx_get(messageEnd, endCoordinator, INF, status);
                numOfReaders--;
            }
            // Pisac moze da pise, pa mu javljamo da pocne.
            numOfWriters++;
            mbx_put(emptyMessage, writers[messageStart.senderId]);
        }
    }
}


// Resenje 2 - jedan proces moze da cita iz vise razlicitih postanskih sanducica - proces koordinator ce imati dva sanduceta.
// U ovom resenju je samo drugaciji sitan detalj - necemo brojati pisca koji cita pomocu brojaca, vec cemo odmah da
// ga sacekamo da zavrsi. Takodje cemo isprazniti uvek poruke o zavrsetku od citalaca, da bi nam brojac citalaca uvek bio azuran
// da bi predstavljao trenutno stanje.

const int R = ...;  // broj citalaca
const int W = ...;  // broj pisaca

typedef struct msg {
    int senderId;
    char* operation;
} msg;

// Svaki citalac ima svoje postansko sanduce.
mbx readers[R];
void reader(int readerId) {
    msg message;
    bool status;

    while (true) {
        message.senderId = readerId;
        message.operation = "startRead";
        mbx_put(message, startCoordinator);
        mbx_get(message, readers[readerId], INF, status);

        read();

        // Ne moramo u ovoj poruci da naglasavamo da je u pitanju javljanje zavrsetka citanja jer ce proces koordinator
        // znati na osnovu brojaca citalaca i pisaca ko mu je javio da je zavrsio - citalac ili pisac.
        mbx_put(message, endCoordinator);
    }
}

// Svaki pisac ima svoje postansko sanduce.
mbx writers[W];
void writer(int writerId) {
    msg message;
    bool status;

    while (true) {
        message.senderId = readerId;
        message.operation = "startWrite";
        mbx_put(message, startCoordinator);
        mbx_get(message, readers[readerId], INF, status);
        
        write();

        // Ne moramo u ovoj poruci da naglasavamo da je u pitanju javljanje zavrsetka pisanja jer ce proces koordinator
        // znati na osnovu brojaca citalaca i pisaca ko mu je javio da je zavrsio - pisac ili citalac.
        mbx_put(message, endCoordinator);
    }
}

mbx startCoordinator, endCoordinator;
void coordinator() {
    msg messageStart, messageEnd;
    msg emptyMessage;
    bool status;
    int numOfReaders = 0;

    while (true) {
        mbx_get(messageStart, startCoordinator, INF, status);
        if (messageStart.operation == "startRead") {
            // Citalac sigurno moze da cita jer pisca dole jos sacekamo da zavrsi odmah nakon sto mu javimo da pocne, tako da ne moze da se dogodi da udjemo u ovu granu, a da pisac pise.
            numOfReaders++;
            mbx_put(emptyMessage, readers[messageStart.senderId]);
        } else if (messageStart.operation == "startWrite") {
            // Ako pisac hoce da pise, a vec ima citalaca koji citaju, on mora da saceka da svi oni zavrse.
            while (numOfReaders > 0) {
                mbx_get(messageEnd, endCoordinator, INF, status);
                numOfReaders--;
            }
            // Pisac moze da pise, pa mu javljamo da pocne.
            mbx_put(emptyMessage, writers[messageStart.senderId]);
            // Cekamo da nam pisac javi da je zavrsio.
            mbx_get(messageEnd, endCoordinator, INF, status);
        }
        // Proveravamo ukoliko su neki citaoci vec zavrsili u medjuvremenu od kad smo im iznad javili da pocnu da citaju,
        // i onda cistimo sanduce u koje su oni poslali poruke da su gotovi kako bismo u brojacu numOfReaders cuvali zaista
        // trenutan broj citalaca.
        status = true;
        while (status) {
            mbx_get(messageEnd, endCoordinator, 0, status);
            if (status) {
                numOfReaders--;
            }
        }
    }
}


// Resenje 3 - jedan proces moze da cita iz samo jednog postanskog sanduceta. Ovo resenje ce zato vise liciti na aktivne monitore
// (posto oni imaju samo jedan ulazni kanal i jedan tok kontrole). Ovde cemo umesto cekanja da neko zavrsi sa radom da bi se
// neki zahtev izvrsio samo baferovati te zahteve i necemo se blokirati u tim situacijama u procesu koordinatoru.

const int R = ...;  // broj citalaca
const int W = ...;  // broj pisaca

typedef struct msg {
    int senderId;
    char* operation;
} msg;

// Svaki citalac ima svoje postansko sanduce.
mbx readers[R];
void Reader(int readerId) {
    msg message;
    bool status;

    while (true) {
        message.senderId = readerId;
        message.operation = "startRead";
        mbx_put(message, coordinator);
        mbx_get(message, readers[readerId], INF, status);

        read();

        message.senderId = readerId;
        message.operation = "endRead";
        mbx_put(message, coordinator);
    }
}

// Svaki pisac ima svoje postansko sanduce.
mbx writers[W];
void Writer(int writerId) {
    msg message;
    bool status;

    while (true) {
        message.senderId = readerId;
        message.operation = "startWrite";
        mbx_put(message, coordinator);
        mbx_get(message, readers[readerId], INF, status);
        
        write();

        message.senderId = readerId;
        message.operation = "endWrite";
        mbx_put(message, coordinator);
    }
}

mbx coordinator;
void Coordinator() {
    msg message;
    msg emptyMessage;
    bool status;
    int numOfReaders = 0;
    int numOfWriters = 0;
    Queue<msg> buffer;

    while (true) {
        if (buffer.size() > 0 && 
        ((buffer.get(0).operation == "startRead" && numOfWriters == 0) ||
        (buffer.get(0).operation == "startWrite" && numOfReaders + numOfWriters == 0))) {
            message = buffer.remove(0);
        } else {
            // Ako nema poruka u baferu ili ako ima ali naredni zahtev iz bafera ne moze trenutno da se izvrsi, cekamo da primimo
            // novu poruku od citaoca ili pisca.
            mbx_get(message, coordinator, INF, status);
        }
        switch (message.operation) {
            case "startRead":
                if (numOfWriters > 0) {
                    buffer.insert(message);
                } else {
                    numOfReaders++;
                    mbx_put(message, readers[message.senderId]);
                }
                break;
            case "startWrite":
                if (numOfWriters > 0 || numOfReaders > 0) {
                    buffer.insert(message);
                } else {
                    numOfWriters++;
                    mbx_put(message, writers[message.senderId]);
                }
                break;
            case "endRead":
                numOfReaders--;
                break;
            case "endWrite":
                numOfWriters--;
                break;
        }
    }
}