/*
* koristicemo tehniku predaje stafetne palice (kada se kazan isprazni ljudozder predaje palicu kuvaru, tj. njega budi a sebe blokira, i isto tako
kada kuvar napuni kazan budi ljudozdera a sebe blokira) i raspodeljene binarne semafore
* predaja stafetne palice znaci da neki proces direktno predaje ekskluzivno pravo pristupa kriticnoj sekciji drugom procesu	
* raspodeljeni binarni semafori su oni za koje vazi da od svih njih u jednom trenutku maksimalno jedan semafor moze imati vrednost 1
*/

const int M = ...;  // broj porcija koje mogu da stanu u kazan (>0)

int food = ...;  // trenutan broj porcija u kazanu (>=0)

sem mutex = 1;
sem cook = 0;
sem savage = 0;  // ova tri semafora predstavljaju jedan raspodeljeni binarni semafor

void cook() {
	while (1) {
		wait(cook);
		prepareFood();
		food = M;
		signal(savage);
	}
}

void savage() {
	while (1) {
		wait(mutex);
		if (food == 0) {
			signal(cook);
			wait(savage);
		}
		food--;
		signal(mutex);
		eat();
	}
}