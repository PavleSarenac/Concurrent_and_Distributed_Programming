// jedan server, vise klijenata - koristi se ticket algoritam za obezbedjivanje FIFO
const int NUMBER_OF_CLIENTS = ...;

typedef struct Request {
    ...
} Request;

typedef struct Response {
    ...
} Response;

void initialize() {
    out("ticket", 1);
    eval(server());
    for (int i = 0; i < NUMBER_OF_CLIENTS; i++) {
        eval(client());
    }
}

void server() {
    int ticket = 1;
    Request request;
    while (true) {
        in("request", ticket, ?request);
        Response response = ...;
        out("response", ticket++, response);
    }
}

void client() {
    int myTicket, next;
    in("ticket", ?myTicket);
    next = myTicket + 1;
    out("ticket", next);
    Request request = ...;
    Response response;
    out("request", myTicket, request);
    in("response", myTicket, ?response);
}


// vise servera, vise klijenata, ima i proces sumator koji periodicno trazi da mu serveri jave broj obradjenih zahteva
const int NUMBER_OF_CLIENTS = ...;
const int NUMBER_OF_SERVERS = ...;

typedef struct Request {
    ...
} Request;

typedef struct Response {
    ...
} Response;

void initialize() {
    out("numberOfActiveServers", 0);
    out("clientTicket", 1);
    out("serverTicket", 1);
    out("notSumming");
    for (int i = 0; i < NUMBER_OF_CLIENTS; i++) {
        eval(client());
    }
    for (int i = 0; i < NUMBER_OF_SERVERS; i++) {
        eval(server());
    }
}

void server() {
    int myServerTicket, oldNumberOfActiveServers, numberOfHandledRequests = 0;
    Request request;
    in("notSumming");
    in("numberOfActiveServers", ?oldNumberOfActiveServers);
    out("numberOfActiveServers", oldNumberOfActiveServers + 1);
    out("notSumming");
    while (true) {
        if (rdp("summing")) {
            out("numberOfHandledRequests", numberOfHandledRequests);
            rd("notSumming");
        } else {
            in("serverTicket", ?myServerTicket);
            out("serverTicket", myServerTicket + 1);
            in("request", myServerTicket, ?request);
            Response response = ...;
            out("response", myServerTicket, response);
            numberOfHandledRequests++;
        }
    }
}

void client() {
    int myClientTicket, next;
    in("clientTicket", ?myClientTicket);
    next = myClientTicket + 1;
    out("clientTicket", next);
    Request request = ...;
    Response response;
    out("request", myClientTicket, request);
    in("response", myClientTicket, ?response);
}

void sumator() {
    int numberOfActiveServers, currentNumberOfHandledRequests, totalNumberOfHandledRequests = 0;
    in("notSumming");
    out("summing");
    rd("numberOfActiveServers", ?numberOfActiveServers);
    for (int i 0; i < numberOfActiveServers; i++) {
        in("numberOfHandledRequests", ?currentNumberOfHandledRequests);
        totalNumberOfHandledRequests += currentNumberOfHandledRequests;
    }
    in("summing");
    out("notSumming");
}