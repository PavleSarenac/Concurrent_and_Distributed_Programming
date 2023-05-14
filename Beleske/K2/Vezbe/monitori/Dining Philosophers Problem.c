// Resenje 1 - Koristicemo monitor sa Signal and Wait disciplinom.

// Problem kod ovog resenja: ako bi dva filozofa koja su jedan naspram drugog naizmenicno jeli, onaj izmedju njih nikad ne bi
// dosao na red - izgladnjivanje odredjenih filozofa je moguce.

monitor DiningPhilosophers {
    const int N = 5;  // broj filozofa
    int state[N] = {0};  // 0 - filozof razmislja, 1 - filozof jede
    cond canEat[N];

    void pickUp(int id) {
        if (state[(id - 1) % N] == 1 || state[(id + 1) % N] == 1) {
            canEat[id].wait();
        }
        state[id] = 1;
    }

    void putDown(int id) {
        state[id] = 0;
        if (state[(id + 2) % N] == 0 && canEat[(id + 1) % N].queue()) {
            canEat[(id + 1) % N].signal();
        }
        if (state[(id - 2) % N] == 0 && canEat[(id - 1) % N].queue()) {
            canEat[(id - 1) % N].signal();
        }
    }
}

// Primer kako bi se u sekvencijalnom kodu koristio nas monitor za sinhronizaciju.
void philosopher(int id) {
    while (true) {
        think();
        DiningPhilosophers.pickUp(id);
        eat();
        DiningPhilosophers.putDown(id);
    }
}

// Resenje 2 - U ovom resenju cemo obezbediti i FIFO poredak. Takodje cemo koristiti uslovnu promenljivu koja ima prioritetni
// red cekanja.

monitor DiningPhilosophersFIFO {
    const int N = 5;  // broj filozofa
    int state[N] = {0};  // 0 - filozof razmislja, 1 - filozof jede
    int ticket = 0;
    cond canEat;

    void pickUp(int id) {
        // Ovako racunamo ticket da bismo posle uvek mogli da znamo o kom je filozofu rec tako sto izracunamo myTicket % N i tako
        // dobijemo id filozofa.
        int myTicket = N * ticket++ + id;
        if (canEat.queue() || state[(id - 1) % N] == 1 || state[(id + 1) % N] == 1) {
            canEat.wait(myTicket);
        }
        state[id] = 1;
    }

    void putDown(int id) {
        state[id] = 0;
        // Ova petlja ce se izvrsiti uvek max dva puta jer imamo ukupno 5 filozofa, dakle max 2 mogu istovremeno da jedu.
        while (canEat.queue()) {
            int waitingPhilId = canEat.min_rank() % N;
            if (state[(waitingPhilId + 1) % N] == 0 && state[(waitingPhilId - 1) % N] == 0) {
                canEat.signal();
            } else {
                break;
            }
        }
    }
}