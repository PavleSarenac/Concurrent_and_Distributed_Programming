// Koristicemo monitor sa Signal and Wait disciplinom.

// Posto je u pitanju Signal and Wait disciplina, mozemo uslove size == N i size == 0 da ispitujemo pomocu if-a umesto while-a
// zato sto tu probudjeni proces odmah dobija monitor i uslov nema ni kad da se izmeni od trenutka njegovog budjenja do trenutka
// kada je on dobio monitor.

// Kod Signal and Continue discipline bismo morali pomocu while-a jer je tu moguce da se uslov izmeni dok probudjeni proces dobije
// monitor jer tu probudjeni proces ide prvo na entry queue a onaj sto budi nastavlja sa koriscenjem monitora.

// Dobro je da pre signaliziranja proverimo da li ima uopste zablokiranih procesa na toj uslovnoj promenljivoj nad kojom zelimo da
// pozovemo signal - to je slucaj zato sto signalizacija iziskuje odredjene rezijske troskove i ne treba ih uzalud traziti.

monitor ProducerConsumer {
    const int N = ...;
    T buffer[N];
    int front = 0;
    int rear = 0;
    int size = 0;
    cond empty;
    cond full;

    void produce(T item) {
        if (size == N) {
            empty.wait();
        }
        buffer[rear] = item;
        tail = (tail + 1) % N;
        size++;
        if (full.queue()) {
            full.signal();
        }
    }

    T consume() {
        if (size == 0) {
            full.wait();
        }
        T item = buffer[head];
        head = (head + 1) % N;
        size--;
        if (empty.queue()) {
            empty.signal();
        }
        return item;
    }
}
