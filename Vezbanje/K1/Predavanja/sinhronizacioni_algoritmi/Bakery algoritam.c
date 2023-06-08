// Bakery algoritam - coarse-grain
const int N = ...;  // broj procesa

int myTicket[N] = {-1};  // ovde cemo cuvati myTicket za svaki proces

void process(int id) {  // id = 0..N-1
    while (true) {
        <myTicket[id] = max(myTicket) + 1;>
        for (int i = 0; i < N; i++) {
            if (i != id) {
                <await(myTicket[i] == -1 || myTicket[i] > myTicket[id]);>
            }
        }
        // critical section
    }
}



// Bakery algoritam - fine-grain

// Vrlo je vazno da se proces najavi pre racunanja max(myTicket) (mislim na liniju koda myTicket[id] = 0;) jer ce onda u slucaju da 
// vise procesa dobije isti myTicket dole u while petlji da se medju tim procesima blokiraju svi sem onog koji ima najmanji id.

// Kada se procesi ne bi najavljivali ovako pre racunanja max(myTicket) (dakle kada bismo uklonili liniju koda 
// myTicket[id] = 0;) bila bi moguca situacija da npr. 2 procesa (P1 i P2) izracunaju istu vrednost (max(myTicket) + 1) i 
// recimo da je samo proces P2 upisao tu vrednost u svoj myTicket[id]. To bi znacilo da za proces P1 i dalje vazi
// myTicket[id] == -1. Neka proces P2 nastavi dalje izvrsavanje, i onda ce zbog prvog uslova while petlje (myTicket[i] != -1)
// on da se propusti dalje i da udje u kriticnu sekciju iako ima veci identifikator od procesa P1 (2 > 1) jer za proces P1
// i dalje vazi myTicket[id] == -1. Sada moze da se dogodi da proces P1 dobije procesor, upise u myTicket[id] tek sada sta je
// izracunao i onda, posto ima isti myTicket[id] kao proces P2, ali manji identifikator (1 < 2), onda ce se i proces P1 pustiti
// u kriticnu sekciju i dosli smo u situaciju da se dva procesa istovremeno nalaze u kriticnoj sekciji. 

// Zato je obavezno da se svaki proces pre racunanja max(myTicket) najavi tako sto ce u svoj myTicket[id] upisati 0, pa ce se
// onda u situacijama kao sto je ova gore opisana sigurno blokirati svi procesi sem onog koji ima najmanji identifikator i
// samo ce taj proces uci u kriticnu sekciju.

const int N = ...;  // broj procesa

int myTicket[N] = {-1};  // ovde cemo cuvati myTicket za svaki proces

void process(int id) {  // id = 0..N-1
    while (true) {
        myTicket[id] = 0;
        myTicket[id] = max(myTicket) + 1;
        for (int i = 0; i < N; i++) {
            if (i != id) {
                while ((myTicket[i] != -1) &&
                       ((myTicket[i] < myTicket[id]) || (myTicket[i] == myTicket[id] && i < id))) skip();
            }
        }
        // critical section
        myTicket[id] = -1;
        // noncritical section
    }
}