// Dining philosophers - logika
void philosopher(int id) {  // id = 0..4
    while (true) {
        think();
        acquireForks();
        eat();
        releaseForks();
    }
}



// Dining philosophers - resenje 1 (deadlock moguc)
Semaphore forks[5] = {Semaphore(1)};
void philosopher(int id) {  // id = 0..4
    while (true) {
        think();
        forks[id].wait();
        forks[(id + 1) % 5].wait();
        eat();
        forks[id].signal();
        forks[(id + 1) % 5].signal();
    }
}



// Dining philosophers - resenje 2 (deadlock ne moze da se dogodi)
Semaphore forks[5] = {Semaphore(1)};
void philosopher(int id) {  // id = 0..3
    while (true) {
        think();
        forks[id].wait();
        forks[id + 1].wait();
        eat();
        forks[id].signal();
        forks[id + 1].signal();
    }
}

void philosopher(int id) {  // id = 4
    while (true) {
        think();
        forks[0].wait();
        forks[4].wait();
        eat();
        forks[0].signal();
        forks[4].signal();
    }
}



// Dining philosophers - resenje 3 (deadlock ne moze da se dogodi i konkurentnost je maksimalna)
Semaphore forks[5] = {Semaphore(1)};
void philosopher(int id) {  // id = 0, 2, 4
    while (true) {
        think();
        forks[(id + 1) % 5].wait();
        forks[id].wait();
        eat();
        forks[(id + 1) % 5].signal()
        forks[id].signal();
    }
}

void philosopher(int id) {  // id = 1, 3
    while (true) {
        think();
        forks[id].wait();
        forks[id + 1].wait();
        eat();
        forks[id].signal();
        forks[id + 1].signal();
    }
}