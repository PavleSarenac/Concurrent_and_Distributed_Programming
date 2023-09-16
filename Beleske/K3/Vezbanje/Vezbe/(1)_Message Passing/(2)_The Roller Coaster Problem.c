// Resenje 1
const int NUMBER_OF_PASSENGERS = ...;
const int ROLLER_COASTER_CAPACITY = ...;

typedef struct Message {
    int senderId;
} Message;

mbx passengers[NUMBER_OF_PASSENGERS];
mbx rollerCoasterStart, rollerCoasterEnd;

void passenger(int passengerId) {
    Message message;
    bool status;
    while (true) {
        message.senderId = passengerId;
        mbx_put(message, rollerCoasterStart);
        mbx_get(message, passengers[passengerId], INF, status);
        ride();
        mbx_get(message, passengers[passengerId], INF, status);
        mbx_put(message, rollerCoasterEnd);
    }
}

void rollerCoaster() {
    Message message;
    bool status;
    int passengerIds[ROLLER_COASTER_CAPACITY];
    while (true) {
        for (int i = 0; i < ROLLER_COASTER_CAPACITY; i++) {
            mbx_get(message, rollerCoasterStart, INF, status);
            passengerIds[i] = message.senderId;
        }
        for (int i = 0; i < ROLLER_COASTER_CAPACITY; i++) {
            mbx_put(message, passengers[passengerIds[i]]);
        }
        ride();
        for (int i = 0; i < ROLLER_COASTER_CAPACITY; i++) {
            mbx_put(message, passengers[passengerIds[i]]);
        }
        for (int i = 0; i < ROLLER_COASTER_CAPACITY; i++) {
            mbx_get(message, rollerCoasterEnd, INF, status);
        }
    }
}


// Resenje 2
const int NUMBER_OF_PASSENGERS = ...;
const int ROLLER_COASTER_CAPACITY = ...;

typedef struct Message {
    int senderId;
    char* operation;
} Message;

mbx passengers[NUMBER_OF_PASSENGERS];
mbx rollerCoaster;

void passenger(int passengerId) {
    Message message;
    bool status;
    while (true) {
        message.senderId = passengerId;
        message.operation = "startRide";
        mbx_put(message, rollerCoaster);
        mbx_get(message, passengerIds[passengerId], INF, status);
        ride();
        mbx_get(message, passengerIds[passengerId], INF, status);
        message.senderId = passengerId;
        message.operation = "endRide";
        mbx_put(message, rollerCoaster);
    }
}

void rollerCoaster() {
    Message message;
    bool status;
    int passengerIds[ROLLER_COASTER_CAPACITY];
    Queue<int> queue;
    while (true) {
        for (int i = 0; i < ROLLER_COASTER_CAPACITY; i++) {
            if (queue.notEmpty()) {
                passengerIds[i] = queue.pop();
            } else {
                mbx_get(message, rollerCoaster, INF, status);
                passengerIds[i] = message.senderId;
            }
        }
        for (int i = 0; i < ROLLER_COASTER_CAPACITY; i++) {
            mbx_put(message, passengers[passengerIds[i]]);
        }
        ride();
        for (int i = 0; i < ROLLER_COASTER_CAPACITY; i++) {
            mbx_put(message, passengers[passengerIds[i]]);
        }
        for (int i = 0; i < ROLLER_COASTER_CAPACITY;) {
            mbx_get(message, rollerCoaster, INF, status);
            if (message.operation == "endRide") {
                i++;
            } else {
                queue.push(message.senderId);
            }
        }
    }
}