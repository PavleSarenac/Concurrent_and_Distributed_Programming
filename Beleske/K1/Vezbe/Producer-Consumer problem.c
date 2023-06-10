const int CAPACITY = ...;

int buffer[CAPACITY];
int writeIndex = readIndex = 0;

Semaphore empty = Semaphore(CAPACITY);
Semaphore full = Semaphore(0);
Semaphore mutexP = Semaphore(1);
Semaphore mutexC = Semaphore(1);

void producer(int id) {
    int newItem;
    while (true) {
        newItem = produce();
        empty.wait();
        mutexP.wait();
        buffer[writeIndex] = newItem;
        writeIndex = (writeIndex + 1) % CAPACITY;
        mutexP.signal();
        full.signal();
    }
}

void consumer(int id) {
    int newItem;
    while (true) {
        full.wait();
        mutexC.wait();
        newItem = buffer[readIndex];
        readIndex = (readIndex + 1) % CAPACITY;
        mutexC.signal();
        empty.signal();
        consume(newItem);
    }
}