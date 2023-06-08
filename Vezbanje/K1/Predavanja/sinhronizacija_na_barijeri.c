// Sinhronizacija na barijeri sa procesom koordinatorom
const int N = ...;  // broj procesa

int arrived[N] = {false};
int go[N] = {false};

void worker(int id) {  // id = 0..N-1
    while (true) {
        // some work done here...
        arrived[id] = true;
        <await(go[id] == true);>  // prvi proces moze prodje ovaj uslov tek nakon sto su svi procesi stigli na barijeru
        go[id] = false;
    }
}

void coordinator() {
    while (true) {
        for (int i = 0; i < N; i++) {
            <await(arrived[i] == true);>
            arrived[i] = false;
        }
        for (int i = 0; i < N; i++) {
            go[id] = true;
        }
    }
}