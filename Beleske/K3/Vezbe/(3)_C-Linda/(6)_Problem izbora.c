// Postoje tri osobe od kojih treba izabrati jednu. Svaka od tih osoba poseduje novcic koji ima dve strane. Izbor osobe se odigrava
// tako sto svaka osoba nezavisno baca svoj novcic. Ukoliko postoji osoba kojoj je novcic pao na drugu stranu u odnosu na preostale
// dve osobe, onda se ta osoba bira. Ukoliko sve tri osobe imaju isto postavljen novcic postupak se ponavlja sve dok se ne odabere
// jedna osoba.
const int NUMBER_OF_PLAYERS = 3;

void initialize() {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        eval(player(i));
    }
}

void player(int id) {
    int myCoin, leftPersonCoin, rightPersonCoin;
    int iterationNumber = 0;
    bool end = false;
    while (!end) {
        myCoin = random(0, 2);
        out("result", id, iterationNumber, myCoin);
        out("result", id, iterationNumber, myCoin);
        in("result", (id + 1) % NUMBER_OF_PLAYERS, iterationNumber, ?rightPersonCoin);
        in("result", (id + 2) % NUMBER_OF_PLAYERS, iterationNumber, ?leftPersonCoin);
        end = (myCoin != leftPersonCoin) || (myCoin != rightPersonCoin) || (leftPersonCoin != rightPersonCoin);
        if ((myCoin != leftPersonCoin) && (myCoin != rightPersonCoin)) {
            out("winner", id);
        }
        iterationNumber++;
    }
}