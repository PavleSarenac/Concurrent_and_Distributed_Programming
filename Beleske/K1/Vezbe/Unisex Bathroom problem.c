/*
* u jednom trenutku u wc-u mogu da budu ili samo zene ili samo muskarci
* izgladnjivanje izbegnuto zahvaljujuci semaforu entry koji ogranicava dolazak novih ljudi, a ne sprecava odlazak starih
	- ista logika kao kod Readers/Writers problema sa predavanja
* ovo je prakticno slicno kao Readers/Writers problem, samo sto se ovde moze reci da nemamo citaoce i pisce, vec dve
razlicite vrste citalaca koje ne mogu istovremeno da citaju (a moze da se desi da vise citalaca iste vrste istovremeno cita)
* toalet ima ogranicen kapacitet, pa zato moramo da imamo i semafor ticket koji ne dozvoljava da udje vise ljudi u toalet
nego sto je kapacitet
*/

// Pretpostavka je da je entry semafor FIFO, inace ovo resenje ne radi.
const int BATHROOM_CAPACITY = ...;  // BATHROOM_CAPACITY > 1

int numOfMen = numOfWomen = 0;

Semaphore empty = Semaphore(BATHROOM_CAPACITY);
Semaphore toilet = Semaphore(1);
Semaphore mutexMen = Semaphore(1);
Semaphore mutexWomen = Semaphore(1);
Semaphore entry = Semaphore(1);

void man(int id) {
    while (true) {
        entry.wait();
        mutexMen.wait();
        numOfMen++;
        if (numOfMen == 1) {
            toilet.wait();
        }
        mutexMen.signal();

        entry.signal();
        empty.wait();
        useToilet();
        empty.signal();

        mutexMen.wait();
        numOfMen--;
        if (numOfMen == 0) {
            toilet.signal();
        }
        mutexMen.signal();
    }
}

void woman(int id) {
    while (true) {
        entry.wait();
        mutexWomen.wait();
        numOfWomen++;
        if (numOfWomen == 1) {
            toilet.wait();
        }
        mutexWomen.signal();

        entry.signal();
        empty.wait();
        useToilet();
        empty.signal();

        mutexWomen.wait();
        numOfWomen--;
        if (numOfWomen == 0) {
            toilet.signal();
        }
        mutexWomen.signal();
    }
}



// Resenje preko stafetne palice i raspodeljenih binarnih semafora - nema izgladnjivanja muskaraca ili zena, ali FIFO nije garantovan
// ukoliko semafori men i women nisu FIFO. Ukoliko jesu, onda je garantovan i FIFO.
const int BATHROOM_CAPACITY = ...;  // BATHROOM_CAPACITY > 1

int numOfMen = numOfWomen = 0;
Queue<char*> queue;

Semaphore mutex = Semaphore(1);
Semaphore men = Semaphore(0);
Semaphore women = Semaphore(0);

void man(int id) {
    while (true) {
        mutex.wait();
        if (numOfWomen > 0 || numOfMen >= BATHROOM_CAPACITY) {
            queue.insert("man");
            mutex.signal();
            men.wait();
        }
        numOfMen++;
        SIGNAL();
        useToilet();
        mutex.wait();
        numOfMen--;
        SIGNAL();
    }
}

void woman(int id) {
    while (true) {
        mutex.wait();
        if (numOfMen > 0 || numOfWomen >= BATHROOM_CAPACITY) {
            queue.insert("woman");
            mutex.signal();
            women.wait();
        }
        numOfWomen--;
        SIGNAL();
        useToilet();
        mutex.wait();
        numOfWomen--;
        SIGNAL();
    }
}

void SIGNAL() {
    if (queue.size() > 0 && queue.get(0) == "man" && numOfWomen == 0 && numOfMen < BATHROOM_CAPACITY) {
        queue.remove(0);
        men.signal();
    } else if (queue.size() > 0 && queue.get(0) == "woman" && numOfMen == 0 && numOfWomen < BATHROOM_CAPACITY) {
        queue.remove();
        women.signal();
    } else {
        mutex.signal();
    }
}



// Resenje sa prosledjivanjem palice i privatnim semaforima - nema izgladnjivanja i garantovan je FIFO uvek.
// Mana ovog resenja je to sto moramo da znamo unapred koliko muskaraca i koliko zena ce koristiti nas toalet, kao i to sto se
// koristi veliki broj semafora.
const int BATHROOM_CAPACITY = ...;  // BATHROOM_CAPACITY > 1
const int N = ...;  // ukupan broj muskaraca koji ce koristiti toalet
const int M = ...;  // ukupan broj zena koji ce koristiti toalet

typedef struct ProcessInfo {
    char* type;
    int id;
    ProcessInfo(char* t, int i) {
        type = t;
        id = i;
    }
} ProcessInfo;

int numOfMen = numOfWomen = 0;
Queue<ProcessInfo> queue;

Semaphore mutex = Semaphore(1);
Semaphore men[N] = Semaphore(0);
Semaphore women[M] = Semaphore(0);

void man(int id) {
    while (true) {
        mutex.wait();
        if (numOfWomen > 0 || numOfMen >= BATHROOM_CAPACITY) {
            ProcessInfo process = ProcessInfo("man", id);
            queue.insert(process);
            mutex.signal();
            men[id].wait();
        }
        numOfMen++;
        SIGNAL();
        useToilet();
        mutex.wait();
        numOfMen--;
        SIGNAL();
    }
}

void woman(int id) {
    while (true) {
        mutex.wait();
        if (numOfMen > 0 || numOfWomen >= BATHROOM_CAPACITY) {
            ProcessInfo process = ProcessInfo("woman", id);
            queue.insert(process);
            mutex.signal();
            women[id].wait();
        }
        numOfWomen--;
        SIGNAL();
        useToilet();
        mutex.wait();
        numOfWomen--;
        SIGNAL();
    }
}

void SIGNAL() {
    if (queue.size() > 0 && queue.get(0)->type == "man" && numOfWomen == 0 && numOfMen < BATHROOM_CAPACITY) {
        men[queue.remove(0)->id].signal();
    } else if (queue.size() > 0 && queue.get(0)->type == "woman" && numOfMen == 0 && numOfWomen < BATHROOM_CAPACITY) {
        women[queue.remove(0)->id].signal();
    } else {
        mutex.signal();
    }
}