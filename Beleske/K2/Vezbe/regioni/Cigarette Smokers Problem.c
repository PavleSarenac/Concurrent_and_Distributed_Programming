// Napomena: funkcije kao sto je ovde smoke() npr., za koje ne znamo koliko traju nikada ne treba stavljati unutar kriticne sekcije jer to moze znatno pogorsati performanse naseg programa - sto je duze jedan proces u kriticnoj sekciji, to duze niko drugi ne moze da napreduje.


typedef struct Smokers {
    // Svaki atribut oznacava da li je konkretan predmet trenutno na stolu.
    bool papir = false;
    bool duvan = false;
    bool sibice = false;
    // Ovaj atribut govori da li agent moze da spusti naredna dva predmeta na sto.
    bool next = false;
} Smokers;

Smokers smokers;

void agent() {
    while (true) {
        int i = random(0, 3);
        region(smokers) {
            switch(i) {
                case 0:
                    papir = true;
                    duvan = true;
                    break;
                case 1:
                    papir = true;
                    sibice = true;
                    break;
                case 2:
                    duvan = true;
                    sibice = true;
                    break;
            }
        }

        await(next);
        next = false;
    }
}

// Ovaj pusac ima neogranicenu zalihu papira i njemu trebaju duvan i sibice.
void smokerPapir() {
    while (true) {
        region(smokers) {
            await(duvan && sibice);
            duvan = false;
            sibice = false;
        }

        smoke();

        region(smokers) {
            next = true;
        }
    }
}

// Ovaj pusac ima neogranicenu zalihu duvana i njemu trebaju papir i sibice.
void smokerDuvan() {
    while (true) {
        region(smokers) {
            await(papir && sibice);
            papir = false;
            sibice = false;
        }

        smoke();

        region(smokers) {
            next = true;
        }
    }
}

// Ovaj pusac ima neogranicenu zalihu sibica i njemu trebaju papir i duvan.
void smokerSibice() {
    while (true) {
        region(smokers) {
            await(papir && duvan);
            papir = false;
            duvan = false;
        }

        smoke();

        region(smokers) {
            next = true;
        }
    }
}