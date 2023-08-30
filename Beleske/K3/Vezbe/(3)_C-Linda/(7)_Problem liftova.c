// Putnik poziva lift sa proizvoljnog sprata. Kada lift stigne na neki sprat svi putnici koji su izrazili zelju da sidju na tom spratu
// obavezno izadju. Nakon izlaska putnika svi putnici koji su cekali na ulazak udju u lift i kazu na koji sprat zele da predju.
// Tek kada se svi izjasne lift ide dalje. Nije potrebno optimizovati put lifta i putnika.

// Za svaki sprat postoji po jedan mutex - ("mutexFloorNumber", brojSprata) da ne bi moglo da se dogodi da neko npr. pozove lift
// da dodje do 5. sprata da bi on tu usao u lift, a da isto tako neko klikne 5. sprat u liftu da bi izasao na 5. spratu i da onda
// u prostoru torki imamo dve iste torke sto bi onda dovelo sistem u nekonzistentno stanje.
const int NUMBER_OF_FLOORS = ...;
const int NUMBER_OF_PASSENGERS = ...;

void initialize() {
    for (int i = 0; i < NUMBER_OF_FLOORS; i++) {
        out("numberOfPeopleLeftInElevator", i, 0);
        out("numberOfPeopleLeftOutsideOfElevator", i, 0);
        out("mutexFloorNumber", i);
    }
    eval(elevator());
    for (int i = 0; i < NUMBER_OF_PASSENGERS; i++) {
        int enteringFloor = random(0, NUMBER_OF_FLOORS);
        int leavingFloor = random(0, NUMBER_OF_FLOORS);
        eval(passenger(i, enteringFloor, leavingFloor));
    }
}

void elevator() {
    int nextFloorNumber;
    while (true) {
        nextFloorNumber = getNextFloorNumber();
        peopleLeavingElevator(nextFloorNumber);
        peopleEnteringElevator(nextFloorNumber);
    }
}

void passenger(int id, int enteringFloor, int leavingFloor) {
    int numberOfPeopleLeftInElevator, numberOfPeopleLeftOutsideOfElevator;
    if (enteringFloor != leavingFloor) {
        // Putnik se prijavljuje da ceka lift i ako je prvi na spratu enteringFloor koji je dosao, treba i da pozove lift ukoliko
        // ga nije niko vec pozvao da dodje do sprata enteringFloor.
        in("numberOfPeopleLeftOutsideOfElevator", enteringFloor, ?numberOfPeopleLeftOutsideOfElevator);
        in("mutexFloorNumber", enteringFloor);
        if (numberOfPeopleLeftOutsideOfElevator == 0 && !rdp("floorNumber", enteringFloor)) out("floorNumber", enteringFloor);
        out("mutexFloorNumber", enteringFloor);
        out("numberOfPeopleLeftOutsideOfElevator", enteringFloor, numberOfPeopleLeftOutsideOfElevator + 1);

        // Putnik ulazi u lift i klikce u liftu sprat na koji zeli da ide (leavingFloor) pod uslovom da je lift prazan i da
        // lift vec nije pozvan da ode na bas taj sprat.
        in("enterElevator", enteringFloor);
        in("numberOfPeopleLeftOutsideOfElevator", enteringFloor, ?numberOfPeopleLeftOutsideOfElevator);
        in("numberOfPeopleLeftInElevator", leavingFloor, ?numberOfPeopleLeftInElevator);
        in("mutexFloorNumber", leavingFloor);
        if (numberOfPeopleLeftInElevator == 0 && !rdp("floorNumber", leavingFloor)) out("floorNumber", leavingFloor);
        out("mutexFloorNumber", leavingFloor);
        out("numberOfPeopleLeftOutsideOfElevator", enteringFloor, numberOfPeopleLeftOutsideOfElevator - 1);
        out("numberOfPeopleLeftInElevator", leavingFloor, numberOfPeopleLeftInElevator + 1);
        out("enterElevator", enteringFloor);

        // Putnik izlazi iz lifta.
        in("leaveElevator", leavingFloor);
        in("numberOfPeopleLeftInElevator", leavingFloor, ?numberOfPeopleLeftInElevator);
        out("numberOfPeopleLeftInElevator", leavingFloor, numberOfPeopleLeftInElevator - 1);
        out("leaveElevator", leavingFloor);
    }
}

int getNextFloorNumber() {
    int floorNumber;
    in("floorNumber", ?floorNumber);
    return floorNumber;
}

void peopleLeavingElevator(int nextFloorNumber) {
    out("leaveElevator", nextFloorNumber);  // Signaliziramo da ljudi treba da napuste lift.
    in("numberOfPeopleLeftInElevator", nextFloorNumber, 0);  // Cekamo da svi napuste lift.
    in("leaveElevator", nextFloorNumber);  // Sklanjamo signalizaciju za napustanje lifta.
    out("numberOfPeopleLeftInElevator", nextFloorNumber, 0);  // Vracamo brojac ljudi u liftu za ovaj sprat u prostor torki.
}

void peopleEnteringElevator(int nextFloorNumber) {
    out("enterElevator", nextFloorNumber);  // Signaliziramo da ljudi treba da udju u lift.
    in("numberOfPeopleLeftOutsideOfElevator", nextFloorNumber, 0);  // Cekamo da svi udju u lift.
    in("enterElevator", nextFloorNumber);  // Sklanjamo signalizaciju za ulazak u lift.
    out("numberOfPeopleLeftOutsideOfElevator", nextFloorNumber, 0);  // Vracamo brojac ljudi van lifta za ovaj sprat u prostor torki.
}