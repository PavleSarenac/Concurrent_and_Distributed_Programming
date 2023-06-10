/*
- Jednoelementni bafer
	* sme vise potrosaca istovremeno da cita iz bafera, nikakav problem - to je dobra konkurentnost
	* potrebno je da svaki potrosac ima svoj semafor da bi se obezbedilo da svaki potrosac tacno jednom
	procita iz bafera (da ne bi bilo moguce da npr. jedan potrosac procita M puta iz bafera, a ostali nijednom)
*/

const int M = ...;  // broj potrosaca

int buffer;  
int consumerCnt = 0;  // brojac potrosaca koji su procitali iz bafera

Semaphore empty = Semaphore(1);
Semaphore full[M] = {Semaphore(0)};
Semaphore mutexC = Semaphore(1);

void producer() {
    int newItem;
    while (true) {
        newItem = produce();
        empty.wait();
        buffer = newItem;
        for (int i = 0; i < M; i++) {
            full[i].signal();
        }
    }
}

void consumer(int id) {
    int newItem;
    while (true) {
        full[id].wait();
        newItem = buffer;
        mutexC.wait();
        consumerCnt++;
        if (consumerCnt == M) {
            consumerCnt = 0;
            empty.signal();
        }
        mutexC.signal();
        consume(newItem);
    }
}

/*
- Viseelementni bafer
	* proizvodjac stavlja proizvode u bafer velicine CAPACITY, i svih M potrosaca treba da procita proizvode sa svih pozicija iz tog bafera, dakle svaka pozicija u baferu treba da bude procitana po M puta
*/

const int CAPACITY = ...;  // kapacitet bafera
const int M = ...;  // broj potrosaca

int buffer[CAPACITY];
int consumerCnt[CAPACITY] = {0};  // za svaku poziciju u baferu brojimo koliko potrosaca je procitalo odatle

Semaphore empty = Semaphore(CAPACITY);
Semaphore full[M] = {Semaphore(0)};
Semaphore mutexC[CAPACITY] = {Semaphore(1)};  // koristimo niz mutexa da bi mogli brojaci za razlicite pozicije da se menjaju 
                                              // istovremeno - bolja konkurentnost
void producer() {
    int writeIndex = 0;
    int newItem;
    while (true) {
        newItem = produce();
        empty.wait();
        buffer[writeIndex] = newItem;
        writeIndex = (writeIndex + 1) % CAPACITY;
        for (int i = 0; i < M; i++) {
            full[i].signal();
        }
    }
}

void consumer(int id) {
    int newItem;
    int readIndex = 0;
    while (true) {
        full[id].wait();
        newItem = buffer[readIndex];
        mutexC[readIndex].wait();
        consumerCnt[readIndex]++;
        if (consumerCnt[readIndex] == M) {
            consumerCnt[readIndex] = 0;
            empty.signal();
        }
        mutexC[readIndex].signal();
        readIndex = (readIndex + 1) % CAPACITY;
        consume(newItem);
    }
}