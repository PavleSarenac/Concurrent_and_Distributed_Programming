void reader() {
    int myTicket, next, numberOfReaders;
    while (true) {
        in("ticket", ?myTicket);
        next = myTicket + 1;
        out("ticket", next);
        in("canProceed", myTicket);
        in("numberOfReaders", ?numberOfReaders);
        out("numberOfReaders", numberOfReaders + 1);
        out("canProceed", next);
        read();
        in("numberOfReaders", ?numberOfReaders);
        out("numberOfReaders", numberOfReaders - 1);
    }
}

void writer() {
    int myTicket, next;
    while (true) {
        in("ticket", ?myTicket);
        next = myTicket + 1;
        out("ticket", next);
        in("canProceed", myTicket);
        rd("numberOfReaders", 0);
        write();
        out("canProceed", next);
    }
}