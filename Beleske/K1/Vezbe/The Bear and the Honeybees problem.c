/*
i ovde cemo koristiti tehniku predaje stafetne palice (kada poslednja pcela ubaci porciju u kosnicu, ona budi medveda a blokira sebe i ostale
pcele, i isto tako kada medved pojede hranu, on budi pcele a sebe blokira) i raspodeljenih binarnih semafora
*/

const int N = ...;  // broj pcela
const int H = ...;  // kapacitet kosnice (broj porcija koje mogu da stanu)

int food = 0;  // trenutan broj porcija u kosnici

sem bear = 0;
sem bee = 1;  // ova dva semafora predstavljaju raspodeljeni binarni semafor (samo jedan moze da ima vrednost 1 u jednom trenutku)

void bear() {
	while (1) {
		wait(bear);
		eat();
		food = 0;
		signal(bee);
	}
}

void bee() {
	while (1) {
		wait(bee);
		food++;
		if (food == H) {
			signal(bear);
		} else {
			signal(bee);
		}
	}
}