// Resenje 1 - vaspitacica koja je htela da ode kuci, ali nije mogla pa ceka taj trenutak ne vodi racuna o dekrementiranju
// brojaca numOfTeachers i numOfWaitingTeachers vec se to radi u metodama takeChildrenHome i teacherComesToWork u kojima
// se signalizira cekajucim vaspitacicama da mogu da idu kuci.

int numOfChildren = 0;
int numOfTeachers = 0;
int numOfWaitingTeachers = 0;
const int NUM_OF_KIDS_PER_TEACHER = 3;

sem mutex = 1;
sem teacherLeave = 0;

int bringChildren(int num) {
    wait(mutex);
    if (numOfChildren + num <= numOfTeachers * NUM_OF_KIDS_PER_TEACHER) {
        numOfChildren += num;
        signal(mutex);
        return 1;  // Ima mesta u vrticu za jos num dece.
    } else {
        signal(mutex);  // Nema mesta u vrticu za jos num dece.
        return 0;
    }
}

void takeChildrenHome(int num) {
    wait(mutex);
    if (num <= numOfChildren) {
        numOfChildren -= num;
    }

    // Provera da li nakon odvodjenja num dece mozemo da pustimo neku vaspitacicu kuci (ukoliko ima vaspitacica koje cekaju to).
    while (numOfWaitingTeachers > 0 && numOfChildren <= (numOfTeachers - 1) * NUM_OF_KIDS_PER_TEACHER) {
        numOfWaitingTeachers--;
        numOfTeachers--;
        signal(teacherLeave);
    }

    signal(mutex);
}

void teacherComesToWork() {
    wait(mutex);
    // Ako ima neka vaspitacica koja ceka da ode kuci, ova nova je samo zameni i pusti da ide, pa je u tom slucaju broj vaspitacica u vrticu nepromenjen.
    if (numOfWaitingTeachers > 0) {
        numOfWaitingTeachers--;
        signal(teacherLeave);
    } else {
        numOfTeachers++;
    }
    signal(mutex);
}

void teacherGoesHome() {
    wait(mutex);
    if (numOfChildren <= (numOfTeachers - 1) * NUM_OF_KIDS_PER_TEACHER) {
        numOfTeachers--;
        signal(mutex);
    } else {
        numOfWaitingTeachers++;
        signal(mutex);
        wait(teacherLeave);
    }
}


// Resenje 2 - u ovom resenju je drugacije samo to sto ovde upravo vaspitacica koja odlazi iz vrtica vodi racuna o
// dekrementiranju brojaca numOfTeachers i numOfWaitingTeachers umesto metoda takeChildrenHome i teacherGoesHome u kojima
// se signalizira vaspitacici koja ceka da ode kuci. Zato nam je ovde potreban semafor confirm preko kog vaspitacica javlja
// ovim metodama da je dekrmentirala brojace i zaista otisla iz vrtica. Da nemamo ovaj semafor, program uopste ne bi radio
// jer bi moglo npr. da se desi da se signalizira nekoj vaspitacici viska jer ona prosla jos uvek nije dekrementirala
// brojace i onda ce ova nova vaspitacica umesto da se zablokira na semaforu teacherLeave proci i otici kuci zajedno sa
// drugom vaspitacicom - ishod je onda da nemamo dovoljno vaspitacica za nasu decu u vrticu.

int numOfChildren = 0;
int numOfTeachers = 0;
int numOfWaitingTeachers = 0;
const int NUM_OF_KIDS_PER_TEACHER = 3;

sem mutex = 1;
sem confirm = 0;
sem teacherLeave = 0;

int bringChildren(int num) {
    wait(mutex);
    if (numOfChildren + num <= numOfTeachers * NUM_OF_KIDS_PER_TEACHER) {
        numOfChildren += num;
        signal(mutex);
        return 1;  // Ima mesta u vrticu za jos num dece.
    } else {
        signal(mutex);  // Nema mesta u vrticu za jos num dece.
        return 0;
    }
}

void takeChildrenHome(int num) {
    wait(mutex);
    if (num <= numOfChildren) {
        numOfChildren -= num;
    }

    // Provera da li nakon odvodjenja num dece mozemo da pustimo neku vaspitacicu kuci (ukoliko ima vaspitacica koje cekaju to).
    while (numOfWaitingTeachers > 0 && numOfChildren <= (numOfTeachers - 1) * NUM_OF_KIDS_PER_TEACHER) {
        signal(teacherLeave);
        wait(confirm);
    }

    signal(mutex);
}

void teacherComesToWork() {
    wait(mutex);
    numOfTeachers++;
    // Ako ima neka vaspitacica koja ceka da ode kuci, ova nova je samo zameni i pusti da ide, pa je u tom slucaju broj vaspitacica u vrticu nepromenjen.
    if (numOfWaitingTeachers > 0) {
        signal(teacherLeave);
        wait(confirm);
    }
    signal(mutex);
}

void teacherGoesHome() {
    wait(mutex);
    if (numOfChildren <= (numOfTeachers - 1) * NUM_OF_KIDS_PER_TEACHER) {
        numOfTeachers--;
        signal(mutex);
    } else {
        numOfWaitingTeachers++;
        signal(mutex);
        wait(teacherLeave);
        numOfWaitingTeachers--;
        numOfTeachers--;
        signal(confirm);
    }
}


// Resenje 3 - u ovom resenju ukoliko nema mesta da roditelj ostavi decu u vrticu, roditelj se blokira i ceka sve dok se ne
// oslobodi mesto za njegovu decu. Nije idealno resenje zbog koriscenja entry semafora ciju implementaciju ne znamo, tj. da
// li postuje FIFO ili ne. Bolje bi resenje bilo sa privatnim semaforima jer bismo tada mogli da garantujemo fer redosled budjenja
// procesa (FIFO).

// Uz pomoc entry semafora smo se osigurali da uvek maksimalno jedan roditelj/vaspitacica moze da bude blokiran na semaforu
// parentLeave/teacherLeave u jednom trenutku. To zelimo jer ako bismo dopustili da npr. vise roditelja moze da se zablokira
// na semaforu parentLeave, broj dece koja cekaju da udju u vrtic bi se sa svakim roditeljem povecavao i mi zapravo ne bismo
// pustali nijedno dete sve dok ne budu sva ta deca mogla da udju u vrtic, sto nema smisla jer to onda omogucava da se dogodi
// situacija da npr. ima mesta u vrticu za jos troje dece, ali posto ukupno petoro dece ceka da udje mi necemo pustiti ovo troje
// iako oni sasvim komotno mogu da udju. Zato je potrebno da obezbedimo da maksimalno jedan roditelj moze da se zablokira na ovom
// semaforu, i tek kada on ostavi svoju decu, naredni roditelj moze da pokusa da dovede svoju.

// Semafor entry takodje sluzi da obezbedi da ne moze da se dogodi da dok jos uvek roditelj ceka na semaforu parentLeave da ode i 
// ostavi svoju decu, neka vaspitacica ode iz vrtica. To ne zelimo da dozvolimo jer nije logicno - ako roditelj mora da se 
// blokira na semaforu parentLeave, to znaci da trenutno u vrticu nema dovoljno vaspitacica da bi on mogao svoju decu da ostavi.
// Zato nema smisla da dopustimo da dok on ceka da postane dovoljan broj vaspitacica, dakle da dodje jos neka na posao, mi 
// dopustimo nekoj vaspitacici koja je trenutno u vrticu da ode (sve vaspitacice koje su trenutno u vrticu moraju da ostanu
// i cekaju da dodje jos vaspitacica da bi roditelj mogao da ostavi svoju decu). Analogno, nema smisla da pustamo da roditelj
// dovede svoju decu dok neka vaspitacica ceka da ode iz vrtica (jer ona upravo ne moze da ode jer i dalje ima previse dece u
// vrticu, pa ceka da dovoljan broj dece ode iz vrtica da bi mogla i ona kuci - zato nema logike da dopustamo za to vreme
// da dodje jos dece u vrtic).

// Pretpostavicemo da entry semafor postuje FIFO i da nema izgladnjivanja niti roditelja koji zele da ostave decu niti 
// vaspitacica koje zele da odu kuci, vec da se redom izvrsava svaki zahtev kako je i pristigao.

int numOfChildren = 0;
int numOfTeachers = 0;
int numOfWaitingChildren = 0;
int numOfWaitingTeachers = 0;
const int NUM_OF_KIDS_PER_TEACHER = 3;

sem mutex = 1;
sem entry = 1;
sem teacherLeave = 0;
sem parentLeave = 0;

int bringChildren(int num) {
    wait(entry);
    wait(mutex);
    if (numOfChildren + num > numOfTeachers * NUM_OF_KIDS_PER_TEACHER) {
        numOfWaitingChildren += num;
        signal(mutex);
        wait(parentLeave);
        numOfWaitingChildren = 0;
    }
    numOfChildren += num;
    signal(mutex);
    signal(entry);
}

void takeChildrenHome(int num) {
    wait(mutex);
    numOfChildren -= num;
    if (numOfWaitingChildren > 0 && numOfChildren + numOfWaitingChildren <= numOfTeachers * NUM_OF_KIDS_PER_TEACHER) {
        signal(parentLeave);
    } else if (numOfWaitingTeachers > 0 && numOfChildren <= (numOfTeachers - 1) * NUM_OF_KIDS_PER_TEACHER) {
        signal(teacherLeave);
    } else {
        signal(mutex);
    }
}

void teacherComesToWork() {
    wait(mutex);
    numOfTeachers++;
    if (numOfWaitingChildren > 0 && numOfChildren + numOfWaitingChildren <= numOfTeachers * NUM_OF_KIDS_PER_TEACHER) {
        signal(parentLeave);
    } else if (numOfWaitingTeachers > 0 && numOfChildren <= (numOfTeachers - 1) * NUM_OF_KIDS_PER_TEACHER) {
        signal(teacherLeave);
    } else {
        signal(mutex);
    }
}

void teacherGoesHome() {
    wait(entry);
    wait(mutex);
    if (numOfChildren > (numOfTeachers - 1) * NUM_OF_KIDS_PER_TEACHER) {
        numOfWaitingTeachers++;
        signal(mutex);
        wait(teacherLeave);
        numOfWaitingTeachers--;
    }
    numOfTeachers--;
    signal(mutex);
    signal(entry);
}