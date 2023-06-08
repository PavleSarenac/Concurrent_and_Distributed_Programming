// Producers and consumers (N proizvodjaca i N potrosaca) - raspodeljeni binarni semafori
int data;

Semaphore empty = Semaphore(1);
Semaphore full = Semaphore(0);

void producer(int id) {
    while (true) {
        empty.wait();
        data = produce();
        full.signal();
    }
}

void consumer(int id) {
    while (true) {
        full.wait();
        consume(data);
        empty.signal();
    }
}



// Bounded buffer (1 proizvodjac i 1 potrosac)
const int CAPACITY = ...;

int buffer[CAPACITY];
int readIndex = writeIndex = 0;

Semaphore empty = Semaphore(CAPACITY);
Semaphore full = Semaphore(0);

void producer() {
    while (true) {
        empty.wait();
        buffer[writeIndex] = produce();
        writeIndex = (writeIndex + 1) % CAPACITY;
        full.signal();
    }
}

void consumer() {
    while (true) {
        full.wait();
        consume(buffer[readIndex]);
        readIndex = (readIndex + 1) % CAPACITY;
        empty.signal();
    }
}



// Bounded buffer (M proizvodjaca i N potrosaca)
const int CAPACITY = ...;

int buffer[CAPACITY];
int writeIndex = readIndex = 0;

Semaphore empty = Semaphore(CAPACITY);
Semaphore full = Semaphore(0);
Semaphore mutexP = Semaphore(1), mutexC = Semaphore(1);

void producer(int id) {
    while (true) {
        empty.wait();
        mutexP.wait();
        buffer[writeIndex] = produce();
        writeIndex = (writeIndex + 1) % CAPACITY;
        mutexP.signal();
        full.signal();
    }
}

void consumer(int id) {
    while (true) {
        full.wait();
        mutexC.wait();
        consume(buffer[readIndex]);
        readIndex = (readIndex + 1) % CAPACITY;
        mutexC.signal();
        empty.signal();
    }
}