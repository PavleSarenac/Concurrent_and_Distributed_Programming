// Resenje 1
const int NUMBER_OF_READERS = ...;
const int NUMBER_OF_WRITERS = ...;

typedef struct Message {
    int senderId;
    char* operation;
} Message;

mbx start, end;
mbx readers[NUMBER_OF_READERS], writers[NUMBER_OF_WRITERS];

void reader(int readerId) {
    Message message;
    bool status;
    while (true) {
        message.senderId = readerId;
        message.operation = "startRead";
        mbx_put(message, start);
        mbx_get(message, readers[readerId], INF, status);
        read();
        message.senderId = readerId;
        message.operation = "endRead";
        mbx_put(message, end);
    }
}

void writer(int writerId) {
    Message message;
    bool status;
    while (true) {
        message.senderId = writerId;
        message.operation = "startWrite";
        mbx_put(message, start);
        mbx_get(message, writers[writerId], INF, status);
        write();
        message.senderId = writerId;
        message.operation = "endWrite";
        mbx_put(message, end);
    }
}

void sharedFile() {
    bool status;
    Message startMessage, endMessage;
    Message emptyMessage;
    int numberOfReaders = 0, numberOfWriters = 0;
    while (true) {
        mbx_get(startMessage, start, INF, status);
        if (startMessage.operation == "startRead") {
            if (numberOfWriters > 0) {
                mbx_get(endMessage, end, INF, status);
                numberOfWriters--;
            }
            numberOfReaders++;
            mbx_put(emptyMessage, readers[startMessage.senderId]);
        } else if (startMessage.operation == "startWrite") {
            if (numberOfWriters > 0) {
                mbx_get(endMessage, end, INF, status);
                numberOfWriters--;
            }
            while (numberOfReaders > 0) {
                mbx_get(endMessage, end, INF, status);
                numberOfReaders--;
            }
            numberOfWriters++;
            mbx_put(emptyMessage, writers[startMessage.senderId]);
        }
    }
}


// Resenje 2
const int NUMBER_OF_READERS = ...;
const int NUMBER_OF_WRITERS = ...;

typedef struct Message {
    int senderId;
    char* operation;
} Message;

mbx start, end;
mbx readers[NUMBER_OF_READERS], writers[NUMBER_OF_WRITERS];

void reader(int readerId) {
    Message message;
    bool status;
    while (true) {
        message.senderId = readerId;
        message.operation = "startRead";
        mbx_put(message, start);
        mbx_get(message, readers[readerId], INF, status);
        read();
        message.senderId = readerId;
        message.operation = "endRead";
        mbx_put(message, end);
    }
}

void writer(int writerId) {
    Message message;
    bool status;
    while (true) {
        message.senderId = writerId;
        message.operation = "startWrite";
        mbx_put(message, start);
        mbx_get(message, writers[writerId], INF, status);
        write();
        message.senderId = writerId;
        message.operation = "endWrite";
        mbx_put(message, end);
    }
}

void sharedFile() {
    bool status;
    Message startMessage, endMessage;
    Message emptyMessage;
    int numberOfReaders = 0;
    while (true) {
        mbx_get(startMessage, start, INF, status);
        if (startMessage.operation == "startRead") {
            numberOfReaders++;
            mbx_put(emptyMessage, readers[startMessage.senderId]);
        } else if (startMessage.operation == "startWrite") {
            while (numberOfReaders > 0) {
                mbx_get(endMessage, end, INF, status);
                numberOfReaders--;
            }
            mbx_put(emptyMessage, writers[startMessage.senderId]);
            mbx_get(endMessage, end, INF, status);
        }
        mbx_get(endMessage, end, 0, status);
        while (status) {
            numberOfReaders--;
            mbx_get(endMessage, end, 0, status);
        }
    }
}


// Resenje 3
const int NUMBER_OF_READERS = ...;
const int NUMBER_OF_WRITERS = ...;

typedef struct Message {
    int senderId;
    char* operation;
} Message;

mbx file;
mbx readers[NUMBER_OF_READERS], writers[NUMBER_OF_WRITERS];

void reader(int readerId) {
    Message message;
    bool status;
    while (true) {
        message.senderId = readerId;
        message.operation = "startRead";
        mbx_put(message, start);
        mbx_get(message, readers[readerId], INF, status);
        read();
        message.senderId = readerId;
        message.operation = "endRead";
        mbx_put(message, end);
    }
}

void writer(int writerId) {
    Message message;
    bool status;
    while (true) {
        message.senderId = writerId;
        message.operation = "startWrite";
        mbx_put(message, start);
        mbx_get(message, writers[writerId], INF, status);
        write();
        message.senderId = writerId;
        message.operation = "endWrite";
        mbx_put(message, end);
    }
}

void sharedFile() {
    bool status;
    Message message;
    Message emptyMessage;
    int numberOfReaders = 0, numberOfWriters = 0;
    Queue<Message> queue;
    while (true) {
        if (queue.size() > 0 && 
            (queue.peek().operation == "startRead" && numberOfWriters == 0) ||
            (queue.peek().operation == "startWrite" && numberOfReaders + numberOfWriters == 0)) {
                message = queue.pop();
            }
        else {
            mbx_get(message, file, INF, status);
        }
        switch (message.operation) {
            case "startRead":
                if (numberOfWriters > 0) {
                    queue.push(message);
                } else {
                    numberOfReaders++;
                    mbx_put(emptyMessage, readers[message.senderId]);
                }
                break;
            case "startWrite":
                if (numberOfReaders > 0 || numberOfWriters > 0) {
                    queue.push(message);
                } else {
                    numberOfWriters++;
                    mbx_put(emptyMessage, writers[message.senderId]);
                }
                break;
            case "endRead":
                numberOfReaders--;
                break;
            case "endWrite":
                numberOfWriters--;
                break;
        }
    }
}