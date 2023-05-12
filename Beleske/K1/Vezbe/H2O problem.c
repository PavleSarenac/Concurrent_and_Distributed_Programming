// Prvo resenje (sa vezbi)

int o = 0;
int h = 0;

sem mutex = 1;
sem oSem = 0;
sem hSem = 0;
sem oOk = 0;
sem hOk = 0;

void oxygen(int id) {  // id postoji da bi se znalo koji atomi tacno cine molekul vode (funkcija bond pravi molekul)
	wait(mutex);
	if (h >= 2) {
		h -= 2;
		signal(hSem);
		wait(hOk);
		signal(hSem);
		wait(hOk);
		bond(id);
		signal(mutex);
	} else {
		o++;
		signal(mutex);
		wait(oSem);
		bond(id);
		signal(oOk);
	}
}

void hydrogen(int id) {
	wait(mutex);
	if (h > 0 && o > 0) {
		h--;
		o--;
		signal(oSem);
		wait(oOk);
		signal(hSem);
		wait(hOk);
		bond(id);
		signal(mutex);
	} else {
		h++;
		signal(mutex);
		wait(hSem);
		bond(id);
		signal(hOk);
	}
}

/*
- Drugo resenje
	* u ovom resenju se koristi princip barijere sa dvoja vrata
*/

typedef struct Molecule {  // da bi se znalo tacno o kojim atomima vodonika i kiseonika se radi
	int ids[3];
};

int count = 0;
Molecule mol SharedVariable;

sem hydroSem = 2;
sem oxySem = 1;
sem door1 = 1;
sem door2 = 0;

void oxygen(int id) {
	wait(oxySem);
	Molecule molecule;
	barrier(id, molecule);
	bond(molecule);
}

void hydrogen(int id) {
	wait(hydroSem);
	Molecule molecule;
	barrier(id, molecule);
	bond(molecule);
}

void barrier(int id, Molecule& molecule) {
	wait(door1);
	molSharedVariable.ids[count] = id;
	count++;
	if (count == 3) {
		signal(door2);
	} else {
		signal(door1);
	}
	
	wait(door2);
	molecule = molSharedVariable;
	count--;
	if (count == 0) {
		signal(oxySem);
		signal(hydroSem);
		signal(hydroSem);
		signal(door1);
	} else {
		signal(door2);
	}
}

/*
- Trece resenje
	* u ovom resenju atom kiseonika je kao proces koordinator
*/
	
int count = 0;

sem oxySem = 0;
sem oxyMutex = 1;
sem hydroSem = 0;
sem hydroSem2 = 0;
sem hydroMutex = 1;

void oxygen(int id) {
	wait(oxyMutex);
	signal(hydroSem);
	signal(hydroSem);
	wait(oxySem);
	bond(id);
	signal(oxyMutex);
}

void hydrogen(int id) {
	wait(hydroSem);
	wait(hydroMutex);
	count++;
	if (count == 2) {
		signal(oxySem);
		signal(hydroSem2);
		signal(hydroSem2);
		count = 0;
	}
	signal(hydroMutex);
	wait(hydroSem2);
	bond(id);
}