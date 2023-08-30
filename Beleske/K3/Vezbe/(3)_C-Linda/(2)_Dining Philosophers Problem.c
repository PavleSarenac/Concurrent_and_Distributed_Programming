// stavljanjem 4 roomTicket u funkciji initialize() smo se osigurali da ne moze da se dogodi da svih 5 filozofa istovremeno zatraze
// viljuske i da tako dodje do deadlock-a

const int NUMBER_OF_PHILOSOPHERS = 5;

void initialize() {
    for (int i = 0; i < NUMBER_OF_PHILOSOPHERS; i++) {
        out("fork", i);
        eval(philosopher(i));
        if (i < NUMBER_OF_PHILOSOPHERS - 1) out("roomTicket");
    }
}

void philospher(int i) {
    while (true) {
        think();
        in("roomTicket");
        in("fork", i);
        in("fork", (i + 1) % NUMBER_OF_PHILOSOPHERS);
        eat();
        out("fork", i);
        out("fork", (i + 1) % NUMBER_OF_PHILOSOPHERS);
        out("roomTicket");
    }
}