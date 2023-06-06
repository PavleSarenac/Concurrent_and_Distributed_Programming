// Ticket algoritam - coarse-grain
int ticket = 0;
int next = 0;

void process(int id) {
    int myTicket;
    while (true) {
        <myTicket = ticket++;>
        <await(myTicket == next);>
        // critical section
        next++;
        // noncritical section
    }
}



// Ticket algoritam - fine-grain
int ticket = 0;
int next = 0;

void process(int id) { 
    int myTicket;
    while (true) {
        myTicket = FA(ticket, 1);
        while (myTicket != next) skip();
        // critical section
        next++;
        // noncritical section
    }
}