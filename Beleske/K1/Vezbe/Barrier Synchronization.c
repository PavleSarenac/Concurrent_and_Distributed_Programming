/*
* Sinhronizaciona barijera omogucava nitima da na njoj sacekaju dok tacno N niti ne dostigne 
   odredjenu tacku u izvrsavanju, pre nego sto bilo koja od tih niti ne nastavi sa svojim izvrsavanjem.
* Necemo implementirati kao na predavanjima sa procesom koordinatorom, vec ce procesi sami da se sinhronizuju.

* Napravicemo reupotrebljivu barijeru - zato nam je neophodan semafor escaped da budemo sigurni da su svi procesi koji su bili
* blokirani na barijeri zaista nju i napustili Da nemamo ovaj semafor, moglo bi da se dogodi da procesi koji su sada bili u barijeri
* je uopste ne napuste jer nisu dobili procesor, a da dodju novi procesi na barijeru koji ce je onda proci umesto da se blokiraju, pri
* cemu ce dodatno procesi koji su morali da napuste barijeru malopre ostati blokirani u njoj.
*/

// Resenje sa 3 semafora

const int N = ...;  // broj niti, veci od 1

int threadCnt = 0;

Semaphore mutex = Semaphore(1);
Semaphore semBarrier = Semaphore(0);
Semaphore escaped = Semaphore(0); 

void barrier() {
    mutex.wait();
    threadCnt++;
    if (threadCnt == N) {
        threadCnt = 0;
        for (int i = 0; i < N; i++) {
            semBarrier.signal();
            escaped.wait();
        }
        mutex.signal();
    } else {
        mutex.signal();
        semBarrier.wait();
        escaped.signal();
    }
}



/*
- Resenje u kom svaki proces ima svoj semafor
	* mana ovog resenja je sto ima mnogo semafora - dodatni rezijski troskovi
	* takodje ovo resenje nije bas najzgodnije za situacije kada ne zelimo da se svih N procesa blokira na barijeri, nego npr. 
	samo podskup od M procesa jer bismo tada morali da pratimo kojih M procesa je doslo do barijere da bismo znali koje treba
	da budimo (posto u tom slucaju ne treba svih N procesa da se bude, vec samo M, gde je M < N)
*/

const int N = ...;  // broj niti, veci od 1

int threadCnt = 0;

Semaphore mutex = Semaphore(1);
Semaphore barrier[N] = {Semaphore(0)};

void barrier(int id) {  // id = 0..N-1
    mutex.wait();
    threadCnt++;
    if (threadCnt == N) {
        threadCnt = 0;
        for (int i = 0; i < N; i++) {
            if (i != id) {
                barrier[i].signal();
            }
        }
        mutex.signal();
    } else {
        mutex.signal();
        barrier[id].wait();
    }
}



/*
- Resenje u kojem se koristi barijera sa dvoja vrata
	* najelegantnije resenje - simetricno, i koriste se samo dva semafora
	* ovo je situacija kao npr. kada ulazimo u banku pa imaju glavna vrata, pa mali prostor za cekanje, i onda druga vrata
		- logika je onda da nije dozvoljen ulaz kroz druga vrata sve dok se u prostoru za cekanje ne nakupi N ljudi (procesa),
		a kada se nakupi, otvaraju se druga vrata i zatvaraju glavna vrata sve dok se ne isprazni prostor za cekanje i tako u krug
*/
		
const int N = ...;  // broj niti

int threadCnt = 0;

Semaphore firstDoor = Semaphore(1);
Semaphore secondDoor = Semaphore(0);

void barrier() {
    firstDoor.wait();
    threadCnt++;
    if (threadCnt == N) {
        secondDoor.signal();
    } else {
        firstDoor.signal();
    }

    secondDoor.wait();
    threadCnt--;
    if (threadCnt == 0) {
        firstDoor.signal();
    } else {
        secondDoor.signal();
    }
}