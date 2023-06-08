// Tie-Breaker (Petersonov) algoritam za 2 procesa - coarse-grain
int in1 = in2 = false;
int last = 1;

void process(int id = 1) {
    while (true) {
        in1 = true;
        last = 1;
        <await(in2 == false || last == 2);>
        // critical section
        in1 = false;
        // noncritical section
    }
}

void process(int id = 2) {
    while (true) {
        in2 = true;
        last = 2;
        <await(in1 == false || last == 1);>
        // critical section
        in2 = false;
        // noncritical section
    }
}



// Tie-Breaker (Petersonov) algoritam za 2 procesa - fine-grain
int in1 = in2 = false;
int last = 1;

void process(int id = 1) {
    while (true) {
        in1 = true;
        last = 1;
        while (in2 == true && last == 1) skip();
        // critical section
        in1 = false;
        // noncritical section
    }
}

void process(int id = 2) {
    while (true) {
        in2 = true;
        last = 2;
        while (in1 == true && last == 2) skip();
        // critical section
        in2 = false;
        // noncritical section
    }
}



// Tie-Breaker (Petersonov) algoritam za N procesa - fine-grain
const int N = ...;  // broj procesa, pa samim tim i broj stanja koje cemo imati

int in[N] = {-1};  // za svaki proces pamtimo broj stanja u kom se on trenutno nalazi
int last[N] = {-1};  // za svako stanje pamtimo id procesa koji je poslednji dosao u njega

void process(int id) {  // id = 0..N-1
    while (true) {
        for (int i = 0; i < N; i++) {
            in[id] = i;
            last[i] = id;
            for (int j = 0; j < N; j++) {
                if (j != id) {
                    while (in[id] <= in[j] && last[in[id]] == id) skip();
                }
            }
        }
        // critical section
        in[id] = -1;
        // noncritical section
    }
}