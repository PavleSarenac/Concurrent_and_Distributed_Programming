// Proizvodjac/potrosac sinhronizacija (deljeni jednoelementni bafer za kopiranje niza)
const int N = ...;

int buffer = NULL;
int producerCnt = consumerCnt = 0;

void producer() {
    int arr[N];
    while (true) {
        <await(producerCnt == consumerCnt);>
        buffer = arr[producerCnt++];
    }
}

void consumer() {
    int arrCopy[N];
    while (true) {
        <await(producerCnt == consumerCnt + 1);>
        arrCopy[consumerCnt++] = buffer;
    }
}



// Kriticna sekcija za dva procesa
bool in1 = in2 = false;

void process(int id = 1) {
    while (true) {
        <await(in2 == false); in1 = true;>
        // critical section
        in1 = false;
        // noncritical section
    }
}

void process(int id = 2) {
    while (true) {
        <await(in1 == false); in2 = true;>
        // critical section
        in2 = false;
        // noncritical section
    }
}



// Kriticna sekcija za vise procesa (coarse-grain)
bool lock = false;

void process(int id) {
    while (true) {
        <await(lock == false); lock = true;>
        // critical section
        lock = false;
        // noncritical section
    }
}



// Kriticna sekcija za vise procesa (fine-grain sa Test-and-Set)
bool lock = false;

void process(int id) {
    while (true) {
        while (lock == true) skip();
        while (TS(lock) == true) {
            while (lock == true) skip();
        }
        // critical section
        lock = false;
        // noncritical section
    }
}
