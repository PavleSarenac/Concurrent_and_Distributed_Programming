// i ovde cemo koristiti tehniku predaje stafetne palice (kada roditelj ptica napuni posudu, budi pticicu i blokira se, a onda kada pticice pojedu
// sve crvice, onda poslednja pticica budi roditelja, a zatim se naredne pticice blokiraju dok se opet ne napuni posuda)

const int F = ...;  // kapacitet posude (u nju moze da stane F crvica)
const int N = ...;  // broj pticica (beba)

int numOfWorms = 0;

sem babyMutex = 0;
sem parent = 1;  // semafori babyMutex i parent predstavljaju raspodeljeni binarni semafor (ne mogu oba istovremeno da imaju vrednost 1)
sem parentMutex = 1;  // ovaj semafor obezbedjuje da ne moze da se dogodi da obe ptice roditelji love istovremeno (jedna za bebe, druga za sebe)

void babyBird() {
	while (1) {
		wait(babyMutex);
		numOfWorms--;
		if (numOfWorms == 0) {
			signal(parent);
		}
		else {
			signal(babyMutex);
		}
		eatWorm();
	}
}

void parentHuntForHimself() {
	wait(parentMutex);
	hunt();
	signal(parentMutex);
}

void parentHuntForBabies() {
	wait(parent);
	wait(parentMutex);
	hunt();
	numOfWorms = F;
	signal(parentMutex);
	signal(babyMutex);
}

void parentBird() {
	while (1) {
		...
		parentHuntForBabies();
		...
		parentHuntForHimself();
	}
}