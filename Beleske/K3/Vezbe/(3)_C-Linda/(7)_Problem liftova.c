// Putnik poziva lift sa proizvoljnog sprata. Kada lift stigne na neki sprat svi putnici koji su izrazili zelju da sidju na tom spratu
// obavezno izadju. Nakon izlaska putnika svi putnici koji su cekali na ulazak udju u lift i kazu na koji sprat zele da predju.
// Tek kada se svi izjasne lift ide dalje. Nije potrebno optimizovati put lifta i putnika.

// U funkciji passenger linije koda inp("floorNumber", enteringFloor); i inp("floorNumber", leavingFloor); su vrlo vazne jer je
// moguce da se dogodi da istovremeno neko 2 putnika pozovu lift npr. jedan da dodje do sprata 5 da bi usao, a drugi da
// ode do sprata 5 da bi izasao i onda bismo u prostoru torki imali duplikat iste torke i zato cistimo te duplikate pomocu inp.
// Kada ne bismo to radili, program bi vrlo lako mogao da zavrsi u nekonzistentnom stanju.
const int NUMBER_OF_FLOORS = ...;
const int NUMBER_OF_PASSENGERS = ...;

void initialize() {
    for (int i = 0; i < NUMBER_OF_FLOORS; i++) {
        out("numberOfPeopleLeftInElevator", i, 0);
        out("numberOfPeopleLeftOutsideOfElevator", i, 0);
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
        if (numberOfPeopleLeftOutsideOfElevator == 0 && !rdp("floorNumber", enteringFloor)) out("floorNumber", enteringFloor);
        out("numberOfPeopleLeftOutsideOfElevator", enteringFloor, numberOfPeopleLeftOutsideOfElevator + 1);

        // Putnik ulazi u lift i klikce u liftu sprat na koji zeli da ide (leavingFloor) pod uslovom da je lift prazan i da
        // lift vec nije pozvan da ode na bas taj sprat.
        in("enterElevator", enteringFloor);
        in("numberOfPeopleLeftOutsideOfElevator", enteringFloor, ?numberOfPeopleLeftOutsideOfElevator);
        in("numberOfPeopleLeftInElevator", leavingFloor, ?numberOfPeopleLeftInElevator);
        if (numberOfPeopleLeftInElevator == 0 && !rdp("floorNumber", leavingFloor)) out("floorNumber", leavingFloor);
        inp("floorNumber", enteringFloor);
        out("numberOfPeopleLeftOutsideOfElevator", enteringFloor, numberOfPeopleLeftOutsideOfElevator - 1);
        out("numberOfPeopleLeftInElevator", leavingFloor, numberOfPeopleLeftInElevator + 1);
        out("enterElevator", enteringFloor);

        // Putnik izlazi iz lifta.
        in("leaveElevator", leavingFloor);
        inp("floorNumber", leavingFloor);
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