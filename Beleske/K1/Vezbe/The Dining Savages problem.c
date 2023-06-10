/*
* koristicemo tehniku predaje stafetne palice (kada se kazan isprazni ljudozder predaje palicu kuvaru, tj. njega budi a sebe blokira, i isto tako
kada kuvar napuni kazan budi ljudozdera a sebe blokira) i raspodeljene binarne semafore
* predaja stafetne palice znaci da neki proces direktno predaje ekskluzivno pravo pristupa kriticnoj sekciji drugom procesu	
* raspodeljeni binarni semafori su oni za koje vazi da od svih njih u jednom trenutku maksimalno jedan semafor moze imati vrednost 1
*/

const int M = ...;  // broj porcija koji moze da stane u kazan

int pot = 0;  // trenutan broj porcija u kazanu

Semaphore mutex = Semaphore(1);  // ova tri semafora predstavljaju raspodeljeni binarni semafor
Semaphore semSavage = Semaphore(0);
Semaphore semCook = Semaphore(0);

void savage(int id) {
    while (true) {
        mutex.wait();
        if (pot == 0) {
            semCook.signal();
            semSavage.wait();
        }
        pot--;
        mutex.signal();
        eat();
    }
}

void cook() {
    while (true) {
        semCook.wait();
        prepareFood();
        pot = M;
        semSavage.signal();
    }
}