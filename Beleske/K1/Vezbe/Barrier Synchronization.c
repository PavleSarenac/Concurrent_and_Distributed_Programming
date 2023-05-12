/*
* Sinhronizaciona barijera omogucava nitima da na njoj sacekaju dok tacno N niti ne dostigne 
   odredjenu tacku u izvrsavanju, pre nego sto bilo koja od tih niti ne nastavi sa svojim izvrsavanjem.
* Necemo implementirati kao na predavanjima sa procesom koordinatorom, vec ce procesi sami da se sinhronizuju.
* Napravicemo reupotrebljivu barijeru.
*/

// Resenje sa 3 semafora

const int N = ...;  // broj niti (>1)

int cnt = 0;

sem mutex = 1;
sem b = 0;
sem ok = 0;

void barrier() {
	wait(mutex);
	cnt++;
	if (cnt == N) {
		cnt = 0;
		for (int i = 0; i < N; i++) {
			signal(b);
			wait(ok);
		}
		signal(mutex);
	} else {
		signal(mutex);
		wait(b);
		signal(ok);
	}
}

/*
- Resenje u kom svaki proces ima svoj semafor
	* mana ovog resenja je sto ima mnogo semafora - dodatni rezijski troskovi
	* takodje ovo resenje nije bas najzgodnije za situacije kada ne zelimo da se svih N procesa blokira na barijeri, nego npr. 
	samo podskup od M procesa jer bismo tada morali da pratimo kojih M procesa je doslo do barijere da bismo znali koje treba
	da budimo (posto u tom slucaju ne treba svih N procesa da se bude, vec samo M, gde je M < N)
*/

const int N = ...;  // broj niti (>1)

int cnt = 0;

sem mutex = 1;
sem b[N] = {0};

void barrier(int processID) {
	wait(mutex);
	cnt++;
	if (cnt == N) {
		cnt = 0;
		for (int i = 0; i < N; i++) {
			if (i != processID) {
				signal(b[i]);
			}
		}
		signal(mutex);
	} else {
		signal(mutex);
		wait(b[processID]);
	}
}

/*
- Resenje u kojem se koristi barijera sa dvoja vrata
	* najelegantnije resenje - simetricno, i koriste se samo dva semafora
	* ovo je situacija kao npr. kada ulazimo u banku pa imaju glavna vrata, pa mali prostor za cekanje, i onda druga vrata
		- logika je onda da nije dozvoljen ulaz kroz druga vrata sve dok se u prostoru za cekanje ne nakupi N ljudi (procesa),
		a kada se nakupi, otvaraju se druga vrata i zatvaraju glavna vrata sve dok se ne isprazni prostor za cekanje i tako u krug
*/
		
const int N = ...;  // broj procesa

int cnt = 0;

sem barrier1 = 1;
sem barrier2 = 0;

void barrier() {
	wait(barrier1);
	cnt++;
	if (cnt == N) {
		signal(barrier2);
	} else {
		signal(barrier1);
	}
	
	wait(barrier2);
	cnt--;
	if (cnt == 0) {
		signal(barrier1);
	} else {
		signal(barrier2);
	}
}