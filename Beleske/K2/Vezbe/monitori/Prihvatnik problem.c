// Koristicemo monitor sa Signal and Wait disciplinom.

// Sustina je da metoda put stavlja dva elementa, a metoda get uzima samo jedan.

// Posto je u pitanju monitor sa Signal and Wait disciplinom, pre drugog poziva signal u metodi put potrebno je proveriti i
// da li je i dalje size > 0 jer nakon prvog poziva signal proces napusta monitor i ide u entry queue i ne znamo kada ce opet
// dobiti monitor, tako da je moguce da se u medjuvremenu promenila vrednost size ako je neko dosao i pozvao get metodu.

// Signaliziranje smo opet rasporedili tako da svaki proces signalizira po jednom, u clear_buffer metodi se pokrece to lancano
// signaliziranje i onda svaki probudjeni proces na kraju metode put po potrebi budi naredni proces - tako nam monitor nece
// uopste biti predugo blokiran. Losiji pristup bi bio da smo u while petlji u metodi clear_buffer budili sve procese jer
// nakon svakog poziva signal proces koji signalizira ide u entry queue i ne znamo kada ce se opet vratiti u monitor i tako u
// krug.

monitor Buffer {
    const int N = ...;  // N >= 2
    T buffer[N];
    int front = 0;
    int rear = 0;
    int size = 0;
    cond empty;
    cond full;

    void put(T item1, T item2) {
        if (size >= N - 1) {
            empty.wait();
        }
        buffer[rear] = item1;
        tail = (tail + 1) % N;
        buffer[rear] = item2;
        tail = (tail + 1) % N;
        size += 2;
        if (full.queue()) {
            full.signal();
        }
        if (full.queue() && size > 0) {
            full.signal();
        }
        if (empty.queue() && size <= N - 2) {
            empty.signal();
        }
    }

    T get() {
        if (size == 0) {
            full.wait();
        }
        T item = buffer[head];
        head = (head + 1) % N;
        size--;
        if (empty.queue() && size <= N - 2) {
            empty.signal();
        }
        return item;
    }

    void clear_buffer() {
        front = 0;
        rear = 0;
        size = 0;
        if (empty.queue()) {
            empty.signal();
        }
    }
}