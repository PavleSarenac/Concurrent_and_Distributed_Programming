// Resenje 1 - nema izgladnjivanja, ali se moze desiti da masovno naizmenicno ulazi zena pa muskarac.

const int N = ...; // Kapacitet toaleta.

typedef struct WC {
    int cntM = 0;
    int cntW = 0;
    int waitingM = 0;
    int waitingW = 0;
    // turn = 0 (niko nema prednost); turn = 1 (muskarci imaju prednost); turn = 2 (zene imaju prednost);
    int turn = 0;
} WC;

WC wc;

void women() {
    // Ulazak u wc.
    region(wc) {
        waitingW++;
        await(cntM == 0 && turn != 1 && cntW < N);
        waitingW--;
        cntW++;
        if (waitingM > 0) {
            turn = 1;
        }
    }

    useToilet();

    // Izlazak iz wc-a.
    region(wc) {
        cntW--;
        if (cntW == 0 && waitingM == 0) {
            turn = 0;
        }
    }
}

void men() {
    // Ulazak u wc.
    region(wc) {
        waitingM++;
        await(cntW == 0 && turn != 2 && cntM < N);
        waitingM--;
        cntM++;
        if (waitingW > 0) {
            turn = 2;
        }
    }

    useToilet();

    // Izlazak iz wc-a.
    region(wc) {
        cntM--;
        if (cntM == 0 && waitingW == 0) {
            turn = 0;
        }
    }
}


// Resenje 2 - obezbedjen FIFO redosled i samim tim je spreceno i izgladnjivanje.

const int N = ...; // Kapacitet toaleta.

typedef struct WC {
    int cntM = 0;
    int cntW = 0;
    int ticket = 0;
    int next = 0;
} WC;

WC wc;

void women() {
    int myTicket;
    // Ulazak u wc.
    region(wc) {
        myTicket = ticket++;
        await(cntM == 0 && cntW < N && myTicket == next);
        cntW++;
        next++;
    }

    useToilet();

    // Izlazak iz wc-a.
    region(wc) {
        cntW--;
    }
}

void men() {
    int myTicket;
    // Ulazak u wc.
    region(wc) {
        myTicket = ticket++;
        await(cntW == 0 && cntM < N && myTicket == next);
        cntM++;
        next++;
    }

    useToilet();

    // Izlazak iz wc-a.
    region(wc) {
        cntM--;
    }
}
