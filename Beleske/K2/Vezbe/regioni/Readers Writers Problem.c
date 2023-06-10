// Resenje 1
// Problem kod ovog resenja je izgladnjivanje pisaca - moguce je da dolazi neogranicen broj citalaca i da tako pisac nikad ne dodje na red.

typedef struct RW {
    int r = 0;
    int w = 0;
} RW;

RW rw;

void reader() {
    while (true) {
        region(rw) {
            await(w == 0);
            r++;
        }

        read();

        region(rw) {
            r--;
        }
    }
}

void writer() {
    while (true) {
        region(rw) {
            await(r == 0 && w == 0);
            w++;
        }

        write();

        region(rw) {
            w--;
        }
    }
}

// Resenje 2
// Problem kod ovog resenja je deadlock - moguce je da dolazi neogranicen broj citalaca i da tako pisac nikad ne dodje na red.
// Ako citaoci citaju, i u nekom trenutku dodje pisac, to ce biti odmah zabelezeno inkrementiranjem promenljive w i 
// tako sprecavamo onda nove citaoce da dolaze jer sledeci treba da bude pisac. Medjutim, problem nastaje ako dodje vise pisaca
// jer onda nijedan nece moci da pise jer ce promenljiva w imati vrednost vecu od 1. Tako dolazi do deadlock-a.

typedef struct RW {
    int r = 0;
    int w = 0;
} RW;

RW rw;

void reader() {
    while (true) {
        region(rw) {
            await(w == 0);
            r++;
        }

        read();

        region(rw) {
            r--;
        }
    }
}

void writer() {
    while (true) {
        region(rw) {
            w++;
            await(r == 0 && w == 1);  // ako npr. w postane 4, onda ne mogu da napreduju ni citaoci ni pisci => deadlock
        }

        write();

        region(rw) {
            w--;
        }
    }
}


// Resenje 3
// U ovom resenju je resen problem izgladnjivanja pisaca (sprecavanjem dolaska novih citalaca kada je u medjuvremenu dosao pisac), kao i deadlock-a (sprecavanjem da se vise pisaca najavi - samo jedan moze i onda ce taj pisac sigurno doci na red kad citaoci koji su dosli pre njega zavrse).
// Potencijalni nedostatak resenja je sto nije garantovan FIFO redosled.

typedef struct RW {
    int r = 0;
    int w = 0;
} RW;

RW rw;

void reader() {
    while (true) {
        region(rw) {
            await(w == 0);
            r++;
        }

        read();

        region(rw) {
            r--;
        }
    }
}

void writer() {
    while (true) {
        region(rw) {
            await(w == 0);  // ovako smo se osigurali da ne moze vise pisaca istovremeno da pise
            w++;  // ovako se belezi da je dosao pisac i onda ce se naredni citaoci blokirati - spreceno izgladnjivanje pisaca
            await(r == 0);  // sprecen deadlock jer ce pisac koji je dosao sigurno doci na red kada zavrse citaoci koji su
        }                   // dosli pre njega

        write();

        region(rw) {
            w--;
        }
    }
}
