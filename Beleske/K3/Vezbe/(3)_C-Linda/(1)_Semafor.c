void initializeSemaphore(String semaphoreName, unsigned int value) {
    for (int i = 0; i < value; i++) {
        out(semaphoreName);
    }
}

void signal(String semaphoreName) {
    out(semaphoreName);
}

void wait(String semaphoreName) {
    in(semaphoreName);
}