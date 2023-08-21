// Resenje 1 - dva razlicita postanska sanduceta roler kostera (jedno na koje putnici javljaju da zele da pocnu voznju, a drugo
// na koje javljaju da su napustili roler koster)

const int N = ...;  // ukupan broj ljudi u luna parku
const int K = ...;  // broj ljudi koji mogu da stane u roler koster
mbx passengers[N];
// Napravili smo dva odvojena postanska sanduceta za roler koster da bi na jedno putnici slali poruke kada zele da pocnu voznju,
// a na drugo kada zele da jave da su izasli iz roler kostera. To smo uradili jer kada bi se te poruke slale na isto
// sanduce, nas roler koster bi mogao da pogresno protumaci neke poruke - npr. moze neki putnik da posalje poruku da zeli da
// pocne voznju, a nas roler koster se i dalje vrti u petlji K puta jer ceka poruke od svih putnika iz tekuce voznje da mu jave
// da su izasli. U tom slucaju on bi pogresno protumacio tu poruku kao da mu je taj putnik javio da je izasao iz roler kostera,
// a zapravo je to bio novi putnik koji je hteo da zapocne voznju.
mbx rollerCoasterStart, rollerCoasterEnd;

// Moramo u sklopu poruke da posaljemo svoj id ukoliko zelimo da primalac moze da zna od koga je dobio poruku. To je slucaj zato
// sto se kod postanskih sanducica radi neimenovano slanje poruka, pa da nemamo ovo polje strukture primalac ne bi ni mogao da
// zna od koga je dobio poruku.
typedef struct msg {
    int senderId;
} msg;

void passenger(int passengerId) {
    msg message;
    bool status;
    while (true) {
        // Setamo okolo po luna parku.
        walkAround();
        // Javljamo roler kosteru da zelimo da se vozimo.
        message.senderId = passengerId;
        mbx_put(message, rollerCoasterStart);
        // Cekamo odgovor od roler kostera da mozemo da pocnemo sa voznjom.
        mbx_get(message, passengers[passengerId], INF, status);
        // Vozimo se na roler kosteru.
        passengerRide();
        // Cekamo da nam roler koster javi da je voznja gotova.
        mbx_get(message, passengers[passengerId], INF, status);
        // Javljamo roler kosteru da smo uspesno napustili roler koster.
        mbx_put(message, rollerCoasterEnd);
    }
}

// Nas roler koster ce da bude ujedno i proces koordinator - nema potrebe za dodatnim procesom koji bi bio koordinator.
void rollerCoaster() {
    // Moramo da pamtimo koji putnici su na trenutnoj voznji da bismo znali kome sve treba javiti kada se voznja zavrsi.
    int ridingPassengers[K];
    msg message;
    bool status;
    while (true) {
        // Cekamo da se skupi K putnika da bi voznja mogla da pocne.
        for (int i = 0; i < K; i++) {
            mbx_get(message, rollerCoasterStart, INF, status);
            ridingPassengers[i] = message.senderId;
        }
        // Javljamo svim putnicima da mogu da pocnu sa voznjom na roler kosteru.
        for (int i = 0; i < K; i++) {
            mbx_put(ridingPassengers[i], passengers[ridingPassengers[i]]);
        }
        // Voznja na roler kosteru.
        rollerCoasterRide();
        // Obavestavamo sve putnike da je voznja gotova.
        for (int i = 0; i < K; i++) {
            mbx_put(ridingPassengers[i], passengers[ridingPassengers[i]]);
        }
        // Cekamo da nam svih K putnika iz ove voznje javi da je uspesno napustilo roler koster.
        for (int i = 0; i < K; i++) {
            mbx_get(message, rollerCoasterEnd, INF, status);
        }
    }
}

// Resenje 2 - svaki proces sme da cita poruke iz samo jednog sanduceta, tako da ne sme roler koster da ima dva sanduceta, vec 
// samo jedno. U ovom slucaju procesi onda vise lice na aktivne monitore (jer aktivni monitori imaju samo jedan ulazni kanal i
// jedan tok kontrole).

const int N = ...;  // ukupan broj ljudi u luna parku
const int K = ...;  // broj ljudi koji mogu da stane u roler koster
mbx passengers[N];
mbx rollerCoaster;

// Moramo u sklopu poruke da posaljemo svoj id ukoliko zelimo da primalac moze da zna od koga je dobio poruku. To je slucaj zato
// sto se kod postanskih sanducica radi neimenovano slanje poruka, pa da nemamo ovo polje strukture primalac ne bi ni mogao da
// zna od koga je dobio poruku.
typedef struct msg {
    int senderId;
    char* operation;
} msg;

void passenger(int passengerId) {
    msg message;
    bool status;
    while (true) {
        // Setamo okolo po luna parku.
        walkAround();
        // Javljamo roler kosteru da zelimo da se vozimo.
        message.senderId = passengerId;
        message.operation = "start";
        mbx_put(message, rollerCoaster);
        // Cekamo odgovor od roler kostera da mozemo da pocnemo sa voznjom.
        mbx_get(message, passengers[passengerId], INF, status);
        // Vozimo se na roler kosteru.
        passengerRide();
        // Cekamo da nam roler koster javi da je voznja gotova.
        mbx_get(message, passengers[passengerId], INF, status);
        // Javljamo roler kosteru da smo uspesno napustili roler koster.
        message.senderId = passengerId;
        message.operation = "end";
        mbx_put(message, rollerCoaster);
    }
}

// Nas roler koster ce da bude ujedno i proces koordinator - nema potrebe za dodatnim procesom koji bi bio koordinator.
void rollerCoaster() {
    // Moramo da pamtimo koji putnici su na trenutnoj voznji da bismo znali kome sve treba javiti kada se voznja zavrsi.
    int ridingPassengers[K];
    // U ovom redu cemo cuvati poruke od onih putnika koji su zatrazili da se voze dok jos nisu svi putnici iz prethodne
    // voznje napustili roler koster. Nakon sto se isprazni roler koster, oni ce imati prednost pri pustanju narednih
    // putnika u roler koster u odnosu na nove putnike koji budu zatrazili voznju.
    Queue<msg> buffer;
    msg message;
    bool status;
    while (true) {
        // Cekamo da se skupi K putnika da bi voznja mogla da pocne.
        // Ovde znamo da su sigurno sve poruke tipa message.operation = "start" jer je roler koster na pocetku iteracije petlje
        // sigurno prazan i nemoguce je da mu iko posalje poruku tipa message.operation = "end" jer nikoga nema u roler kosteru.
        for (int i = 0; i < K; i++) {
            if (buffer.empty()) {
                mbx_get(message, rollerCoaster, INF, status);
            } else {
                message = buffer.delete();
            }
            ridingPassengers[i] = message.senderId;
        }
        // Javljamo svim putnicima da mogu da pocnu sa voznjom na roler kosteru.
        for (int i = 0; i < K; i++) {
            mbx_put(ridingPassengers[i], passengers[ridingPassengers[i]]);
        }
        // Voznja na roler kosteru.
        rollerCoasterRide();
        // Obavestavamo sve putnike da je voznja gotova.
        for (int i = 0; i < K; i++) {
            mbx_put(ridingPassengers[i], passengers[ridingPassengers[i]]);
        }
        // Cekamo da nam svih K putnika iz ove voznje javi da je uspesno napustilo roler koster.
        // Ovde je moguce da primimo poruke od putnika tipa message.operation = "end" jer ima ljudi u roler kosteru i oni
        // upravo to treba da jave pri izlasku, ali mozemo primiti i poruke tipa message.operation = "start" od ljudi koji
        // zele da zapocnu voznju na roler kosteru. Nas trenutno interesuju samo ljudi koji izlaze i ne mozemo da pustamo nikoga
        // da udje dok svi ne izadju.
        for (int i = 0; i < K;) {
            mbx_get(message, rollerCoaster, INF, status);
            if (message.operation == "end") {
                i++;
            } else {
                buffer.insert(message);
            }
        }
    }
}