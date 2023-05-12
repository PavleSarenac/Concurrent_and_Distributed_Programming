// Srz problema: na mostu se ne mogu naci istovremeno automobil sa severa i automobil sa juga, ali moze vise automobila iz istog smera

// Resenje 1 - ima problem izgladnjivanja, jer je moguce da konstantno dolaze 
// automobili iz istog smera i tako onda nikad ne bi dosli na red automobili iz suprotnog smera

typedef struct Bridge {
    int northCnt = 0;  // brojac automobila koji se trenutno nalaze na mostu a dolaze sa severa
    int southCnt = 0;  // brojac automobila koji se trenutno nalaze na mostu a dolaze sa juga
} Bridge;

Bridge bridge;

void north() {
    // Automobil ceka da predje most.
    region(bridge) {
        await(southCnt == 0);
        northCnt++;
    }

    // Automobil prelazi most.
    crossBridge();

    // Automobil odlazi sa mosta.
    region(bridge) {
        northCnt--;
    }
}

void south() {
    // Automobil ceka da predje most.
    region(bridge) {
        await(northCnt == 0);
        southCnt++;
    }

    // Automobil prelazi most.
    crossBridge();

    // Automobil odlazi sa mosta.
    region(bridge) {
        southCnt--;
    }
}


// Resenje 2 - resen problem izgladnjivanja tako sto se menja dozvoljeni smer dolaska automobila na most ukoliko se dogodi da
// most predje 10 automobila iz istog smera, a da je za to vreme cekao jedan ili vise automobila iz suprotnog smera

typedef struct Bridge {
    // Ove promenljive broje koliko automobila se trenutno nalazi na mostu (za oba smera).
    int northCnt = 0;
    int southCnt = 0;
    // Ove promenljive broje koliko automobila trenutno ceka da predje most (za oba smera).
    int waitNorthCnt = 0;
    int waitSouthCnt = 0;
    // Ove promenljive broje koliko automobila je preslo most (za oba smera).
    // Ovaj brojac za jedan smer se uvecava samo ako u trenutku prelaska mosta automobila iz
    // tog smera ima automobila koji cekaju da predju most iz suprotnog smera.
    int crossNorthCnt = 0;
    int crossSouthCnt = 0;
} Bridge;

Bridge bridge;

void north() {
    // Automobil ceka da predje most.
    region(bridge) {
        waitNorthCnt++;
        await(southCnt == 0 && crossNorthCnt < 10);
        waitNorthCnt--;

        northCnt++;
        if (waitSouthCnt > 0) {
            crossNorthCnt++;
        }
    }

    // Automobil prelazi most.
    crossBridge();

    // Automobil odlazi sa mosta.
    region(bridge) {
        northCnt--;
        if (northCnt == 0) {
            crossSouthCnt = 0;
        }
    }
}

void south() {
    // Automobil ceka da predje most.
    region(bridge) {
        waitSouthCnt++;
        await(northCnt == 0 && crossSouthCnt < 10);
        waitSouthCnt--;

        southCnt++;
        if (waitNorthCnt > 0) {
            crossSouthCnt++;
        }
    }

    // Automobil prelazi most.
    crossBridge();

    // Automobil odlazi sa mosta.
    region(bridge) {
        southCnt--;
        if (southCnt == 0) {
            crossNorthCnt = 0;
        }
    }
}