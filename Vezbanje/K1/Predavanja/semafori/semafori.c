// Shortest Job Next Allocation (SJN) - coarse-grain
bool free = true;

void request(int time, int id) {
    <await(free == true); free = false;>
}

void release() {
    <free = true;>
}



// Shortest Job Next Allocation (SJN) - fine-grain, ali bazicno
bool free = true;

Semaphore entry = Semaphore(1);

void request(int time, int id) {
    entry.wait();
    if (free == false) {
        DELAY();
    }
    free = false;
    SIGNAL();
}

void release() {
    entry.wait();
    free = true;
    SIGNAL();
}



// Shortest Job Next Allocation (SJN) - fine-grain (sa privatnim semaforima)
const int N = ...;  // broj procesa

typedef struct ProcessInfo {
    int time;
    int id;
    ProcessInfo(int t, int i) {
        time = t;
        id = i;
    }
} ProcessInfo;

bool free = true;
PriorityQueue<ProcessInfo> priorityQueue;  // na pocetku ovog reda ce uvek biti proces sa najmanjom vrednoscu time

Semaphore entry = Semaphore(1);
Semaphore privateSemaphores[N] = {Semaphore(0)};

void request(int time, int id) {
    ProcessInfo process = ProcessInfo(time, id);  // ovo nije deljena promenljiva, pa je u redu da ne bude zasticena semaforom
    entry.wait();
    if (free == false) {
        priorityQueue.insert(process);
        entry.signal();
        privateSemaphores[id].wait();
    }
    free = false;
    entry.signal();
}

void release() {
    entry.wait();
    free = true;
    if (priorityQueue.size() > 0) {
        int nextProcessId = priorityQueue.remove(0)->id;
        privateSemaphores[nextProcessId].signal();
    } else {
        entry.signal();
    }
}



// Generalizacija alokacije (uz pomoc prosledjivanja stafete i privatnih semafora)
const int M = ...;

typedef ProcessInfo {
    int time;
    int numOfResources;
    int id;
    ProcessInfo(int t, int num, int i) {
        time = t;
        numOfResources = num;
        id = i;
    }
} ProcessInfo;

int availableResources = M;
PriorityQueue<ProcessInfo> priorityQueue;

Semaphore entry = Semaphore(1);
Semaphore privateSemaphores[N] = {Semaphore(0)};

void request(int time, int numOfResources, int id) {
    ProcessInfo process = ProcessInfo(time, numOfResources, id);
    entry.wait();
    if (numOfResources > availableResources) {
        priorityQueue.insert(process);
        entry.signal();
        privateSemaphores[id].wait();
    }
    availableResources -= numOfResources;
    entry.signal();
}

void release(int numOfResources) {
    entry.wait();
    availableResources += numOfResources;
    if (priorityQueue.size() > 0) {
        int requestedResources = priorityQueue.get(0)->numOfResources;
        if (requestedResources <= availableResources) {
            privateSemaphores[priorityQueue.remove(0)->id].signal();
        } else {
            entry.signal();
        }
    } else {
        entry.signal();
    }
}