/*
- Jednoelementni bafer
	* sme vise potrosaca istovremeno da cita iz bafera, nikakav problem - to je dobra konkurentnost
	* potrebno je da svaki potrosac ima svoj semafor da bi se obezbedilo da svaki potrosac tacno jednom
	procita iz bafera (da ne bi bilo moguce da npr. jedan potrosac procita N puta iz bafera, a ostali nijednom)
*/

const int N = ...;  // broj potrosaca (consumer-a)
const int B = 1;  // velicina bafera

int cnt = 0;  // brojac potrosaca koji su procitali iz bafera
int buffer[1] = {};

sem mutex = 1;
sem empty = B;
sem full[N] = {0};

void producer() {
	while (1) {
		int item = produce();
		wait(empty);
		buffer[0] = item;
		for (int i = 0; i < N; i++) {
			signal(full[i]);
		}
	}
}

void consumer(int id) {
	while (1) {
		wait(full[id]);
		int item = buffer[0];
		wait(mutex);
		cnt++;
		if (cnt == N) {
			cnt = 0;
			signal(empty);
		}
		signal(mutex);
		consume(item);
	}
}

/*
- Viseelementni bafer
	* proizvodjac stavlja proizvode u bafer velicine B, i svih N potrosaca treba da procita proizvode sa svih pozicija iz tog bafera, dakle svaka
	pozicija u baferu treba da bude procitana po N puta
*/

const int N = ...;  // broj potrosaca (consumer-a)
const int B = ...;  // velicina bafera
int cnt[B] = {0};  // niz brojaca za svaku poziciju u baferu - broji se za svaku poziciju u baferu koliko potrosaca je citalo sa te pozicije
int buffer[B] = {};

sem mutex[B] = {1};
sem empty = B;
sem full[N] = {0};

void producer() {
	int wi = 0;  // pozicija u baferu na koju se upisuje (writeIndex)
	while (1) {
		int item = produce();
		wait(empty);
		buffer[wi] = item;
		wi = (wi + 1) % B;
		for (int i = 0; i < N; i++) {
			signal(full[i]);
		}
	}
}

void consumer(int id) {
	int ri = 0;  // pozicija u baferu sa koje potrosac cita (readIndex)
	while (1) {
		wait(full[id]);
		int item = buffer[ri];
		wait(mutex[ri]);  // bolja konkurentnost sa nizom mutex-a jer je onda moguce razlicite pozicije u nizu brojaca menjati istovremeno
		cnt[ri]++;
		if (cnt[ri] == N) {  // svih N potrosaca su procitali proizvod sa pozicije ri iz bafera, pa proizvodjac moze da stavi nov proizvod na tu poziciju
			cnt[ri] = 0;
			signal(empty);
		}
		signal(mutex[ri]);
		consume(item);
		ri = (ri + 1) % B;
	}
}