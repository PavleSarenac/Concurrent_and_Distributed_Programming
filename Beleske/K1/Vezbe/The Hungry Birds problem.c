// i ovde cemo koristiti tehniku predaje stafetne palice (kada roditelj ptica napuni posudu, budi pticicu i blokira se, a onda kada pticice pojedu
// sve crvice, onda poslednja pticica budi roditelja, a zatim se naredne pticice blokiraju dok se opet ne napuni posuda)

int N = ...;  // broj ptica dece
int F = ...;  // broj crva koji moze da stane u zajednicku posudu

int pot = 0;  // broj crva u zajednickoj posudi

Semaphore parent = Semaphore(1);  // semafori parent i mutexChild predstavljaju raspodeljeni binarni semafor
Semaphore mutexChild = Semaphore(0);
Semaphore mutexParent = Semaphore(1);  // ovaj semafor obezbedjuje da ne mogu obe ptice roditelji da odu u lov istovremeno

void birdChild(int id) {  // id = 0..N-1
    while (true) {
        mutexChild.wait();
        pot--;
        if (pot == 0) {
            parent.signal();
        } else {
            mutexChild.signal();
        }
        eatAndSleep();
    }
}

void birdParent(int id) {  // id = 0..1
    while (true) {
        parentHuntForChildren(id);
        parentHuntForHimself(id);
    }
}

void parentHuntForChildren(int id) {  // id = 0..1
    parent.wait();
    mutexParent.wait();
    hunt();
    pot = F;
    mutexParent.signal();
    mutexChild.signal();
}

void parentHuntForHimself(int id) {  // id = 0..1
    mutexParent.wait();
    hunt();
    mutexParent.signal();
}