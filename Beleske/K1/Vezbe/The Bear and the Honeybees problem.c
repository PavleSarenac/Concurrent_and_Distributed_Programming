/*
i ovde cemo koristiti tehniku predaje stafetne palice (kada poslednja pcela ubaci porciju u kosnicu, ona budi medveda a blokira sebe i ostale
pcele, i isto tako kada medved pojede hranu, on budi pcele a sebe blokira) i raspodeljenih binarnih semafora
*/

const int N = ...;  // broj pcela
const int H = ...; // broj porcija meda koji moze da stane u kosnicu

int kosnica = 0;  // trenutan broj porcija meda u kosnici

Semaphore mutex = Semaphore(1);  // ova dva semafora predstavljaju raspodeljeni binarni semafor
Semaphore bear = Semaphore(0);

void honeybee(int id) {  // id = 0..N-1
    while (true) {
        collectHoney();
        mutex.wait();
        kosnica++;
        if (kosnica == H) {
            bear.signal();
        } else {
            mutex.signal();
        }
    }
}

void bear() {
    while (true) {
        bear.wait();
        eat();
        kosnica = 0;
        mutex.signal();
    }
}