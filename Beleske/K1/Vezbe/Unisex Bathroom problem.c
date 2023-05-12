/*
* u jednom trenutku u wc-u mogu da budu ili samo zene ili samo muskarci
* izgladnjivanje izbegnuto zahvaljujuci semaforu entry koji ogranicava dolazak novih ljudi, a ne sprecava odlazak starih
	- ista logika kao kod Readers/Writers problema sa predavanja
* ovo je prakticno slicno kao Readers/Writers problem, samo sto se ovde moze reci da nemamo citaoce i pisce, vec dve
razlicite vrste citalaca koje ne mogu istovremeno da citaju (a moze da se desi da vise citalaca iste vrste istovremeno cita)
* toalet ima ogranicen kapacitet, pa zato moramo da imamo i semafor ticket koji ne dozvoljava da udje vise ljudi u toalet
nego sto je kapacitet
*/

const int BATHROOM_CAPACITY = ...;

int womenCount = 0;
int menCount = 0;

sem toilet = 1;
sem entry = 1;
sem ticket = BATHROOM_CAPACITY;
sem mutexWomen = 1;
sem mutexMen = 1;

void woman() {
	while (1) {
		wait(entry);
		
		wait(mutexWomen);
		womenCount++;
		if (womenCount == 1) wait(toilet);
		signal(mutexWomen);
		
		signal(entry);
		wait(ticket);
		useToilet();
		signal(ticket);
		
		wait(mutexWomen);
		womenCount--;
		if (womenCount == 0) signal(toilet);
		signal(mutexWomen);
	}
}

void man() {
	while (1) {
		wait(entry);
		
		wait(mutexMen);
		menCount++;
		if (menCount == 1) wait(toilet);
		signal(mutexMen);
		
		signal(entry);
		wait(ticket);
		useToilet();
		signal(ticket);
		
		wait(mutexMen);
		menCount--;
		if (menCount == 0) signal(toilet);
		signal(mutexMen);
	}
}