// Napomena: funkcije kao sto je ovde smoke() npr., za koje ne znamo koliko traju nikada ne treba stavljati unutar kriticne sekcije jer to moze znatno pogorsati performanse naseg programa - sto je duze jedan proces u kriticnoj sekciji, to duze niko drugi ne moze da napreduje.


typedef struct Smokers {
    // Svaki atribut oznacava da li je konkretan predmet trenutno na stolu.
    bool paper = false;
    bool tobacco = false;
    bool matches = false;
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
                    paper = true;
                    tobacco = true;
                    break;
                case 1:
                    paper = true;
                    matches = true;
                    break;
                case 2:
                    tobacco = true;
                    matches = true;
                    break;
            }
            await(next == true);
        }
        next = false;
    }
}

// Ovaj pusac ima neogranicenu zalihu papira i njemu trebaju duvan i sibice.
void smokerPaper() {
    while (true) {
        region(smokers) {
            await(tobacco && matches);
            tobacco = false;
            matches = false;
        }

        smoke();

        region(smokers) {
            next = true;
        }
    }
}

// Ovaj pusac ima neogranicenu zalihu duvana i njemu trebaju papir i sibice.
void smokerTobacco() {
    while (true) {
        region(smokers) {
            await(paper && matches);
            paper = false;
            matches = false;
        }

        smoke();

        region(smokers) {
            next = true;
        }
    }
}

// Ovaj pusac ima neogranicenu zalihu sibica i njemu trebaju papir i duvan.
void smokerMatches() {
    while (true) {
        region(smokers) {
            await(paper && tobacco);
            paper = false;
            tobacco = false;
        }

        smoke();

        region(smokers) {
            next = true;
        }
    }
}