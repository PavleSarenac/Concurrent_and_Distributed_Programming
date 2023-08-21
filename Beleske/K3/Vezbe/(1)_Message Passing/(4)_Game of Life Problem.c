// Potrebno je obezbediti da kada neki cvor racuna svoje stanje za narednu generaciju gleda stanja svojih suseda iz prosle
// generacije. Takodje, nijedan cvor ne moze da predje u trecu generaciju dok svi cvorovi nisu presli iz prve u drugu
// generaciju. Zato je dovoljno da u jednom trenutku cuvamo stanja cvorova iz samo dve generacije (tekuce i naredne) kako bismo 
// mogli da obavljamo prelaz iz tekuce generacije u narednu.

// Da ne bismo konstantno vrsili kopiranja nove generacije u proslu pri prelasku iz tekuce generacije u sledecu (u sustini da
// ne bismo radili: oldGeneration = newGeneration; newGeneration = Generation();), cuvacemo podatke o generacijama u dva niza - 
// jedan ce sluziti da u njemu cuvamo podatke o parnim generacijama, a drugi za podatke o neparnim generacijama. Na taj nacin
// nikada ne radimo kopiranje podataka, vec samo jednostavno overwrite-ovanje.

const int NUMBER_OF_GENERATIONS = ...;
const int N = ...;

typedef struct msg {
    bool nodeAlive;  // nodeAlive govori da li je celija koja je poslala poruku ziva ili mrtva
    int i;
    int j;
    int generation;  // generation govori u kojoj generaciji je celija koja je poslala poruku
} msg;

// Pravimo matricu postanskih sanducica jer nam treba po N sanducica za obe generacije cvorova.
mbx box[N][N];

void node(int i, int j) {
    bool nodeAlive, status;
    msg neighbors[2][8];  // ova matrica nam sluzi da cuvamo pristigle poruke od nasih suseda iz obe generacije
    int num[2];  // ovaj niz sluzi da brojimo koliko poruka je primio cvor iz parne (num[0]) ili neparne (num[1]) generacije
    msg message;

    num[0] = 0; num[1] = 0;
    for (int currGen = 0; currGen < NUMBER_OF_GENERATIONS; k++) {
        message.nodeAlive = nodeAlive;
        message.i = i;
        message.j = j;
        message.generation = currGen;

        // Tekuci cvor salje poruku svim svojim susedima iz obe generacije (zato se vrtimo u duploj petlji).
        for (int p = xStart(i); p <= xEnd(i); p++) {
            for (int q = yStart(j); q <= yEnd(j); q++) {
                if (p != i || q != j) {
                    mbx_put(message, box[p][q]);
                }
            }
        }

        // Tekuci cvor prima poruke od svih svojih suseda. U ovoj petlji se vrtimo sve dok ne dobijemo poruke od svih svojih
        // suseda iz tekuce generacije (zato se proverava vrednost num[currGen % 2]) jer nam je vazno da imamo informacije
        // o svim susedima iz nase generacije kako bismo mogli onda dole da pozovemo funkciju calculateState da bismo izracunali
        // nase stanje u narednoj generaciji i da predjemo u tu generaciju.
        
        // Medju porukama pristiglim u ovoj petlji naravno mogu da stignu i poruke od suseda koji su presli vec u narednu
        // generaciju, ali one nam nisu u ovoj iteraciji vazne, primicemo ih naravno, ali ovde nam je cilj da primimo sve
        // poruke suseda iz tekuce generacije.
        while (num[currGen % 2] < numOfNeighbors(i, j)) {
            mbx_get(message, box[i][j], INF, status);
            neighbors[message.generation % 2][num[message.generation % 2]] = message;
            num[message.generation % 2]++;
        }

        // Resetujemo brojac poruka pristiglih od suseda tekuceg cvora iz tekuce generacije.
        num[currGen % 2] = 0;
        // Tekuci cvor racuna svoje stanje u narednoj generaciji i prelazi u nju.
        calculateState(i, j, k, neighbors, nodeAlive);
    }
}