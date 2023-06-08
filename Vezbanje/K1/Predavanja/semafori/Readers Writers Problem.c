// Readers Writers problem - resenje 1 (jednostavan kritican region - u jednom trenutku ili jedan citalac ili jedan pisac)
Semaphore resource = Semaphore(1);

void reader(int id) {
    while (true) {
        resource.wait();
        read();
        resource.signal();
    }
}

void writer(int id) {
    while (true) {
        resource.wait();
        write();
        resource.signal();
    }
}



// Readers/Writers problem - resenje 2 (moze vise citalaca da cita istovremeno, ali je moguce izgladnjivanje pisaca)
// Vrlo je vazno da pozivi mutex.signal() budu tamo gde jesu - da smo npr. stavili mutex.signal() iznad provere
// if (numOfReaders == 1), bilo bi moguce da iako se blokira prvi citalac jer neko pise, udju naredni citaoci, sto ne sme
// da se dogodi.
int numOfReaders = 0;

Semaphore resource = Semaphore(1);
Semaphore mutex = Semaphore(1);

void reader(int id) {
    while (true) {
        mutex.wait();
        numOfReaders++;
        if (numOfReaders == 1) {
            resource.wait();
        }
        mutex.signal();  
        read();
        mutex.wait();
        numOfReaders--;
        if (numOfReaders == 0) {
            resource.signal();
        }
        mutex.signal();
    }
}

void writer(int id) {
    while (true) {
        resource.wait();
        write();
        resource.signal();
    }
}



// Readers/Writers problem - resenje 3 (moze vise citalaca da cita istovremeno i reseno izgladnjivanje pisaca)
// Ovo resenje obezbedjuje da pisci nisu izgladnjivani samo ako semafor entry postuje FIFO, u suprotnom nema garancije da nece doci
// do izgladnjivanja pisaca.
int numOfReaders = 0;

Semaphore resource = Semaphore(1);
Semaphore mutex = Semaphore(1);
Semaphore entry = Semaphore(1);

void reader(int id) {
    while (true) {
        entry.wait();
        mutex.wait();
        numOfReaders++;
        if (numOfReaders == 1) {
            resource.wait();
        }
        mutex.signal();
        entry.signal();
        read();
        mutex.wait();
        numOfReaders--;
        if (numOfReaders == 0) {
            resource.signal();
        }
        mutex.signal();
    }
}

void writer(int id) {
    while (true) {
        entry.wait();
        resource.wait();
        entry.signal();
        write();
        resource.signal();
    }
}



// Readers/Writers - resenje 4 (coarse-grain; moze vise citalaca istovremeno i nema izgladnjivanja pisaca)
int numOfReaders = numOfWriters = 0;
Queue queue;

void reader(int id) {
    while (true) {
        <await(numOfWriters == 0 && queue.size() == 0); numOfReaders++;>
        read();
        <numOfReaders--;>
    }
}

void writer(int id) {
    while (true) {
        <await(numOfReaders == 0 && numOfWriters == 0 && queue.size() == 0); numOfWriters++;>
        write();
        <numOfWriters--;>
    }
}



// Readers/Writers - resenje 4 (fine-grain; moze vise citalaca istovremeno i nema izgladnjivanja pisaca - reseno pomocu prosledjivanja
// stafete i raspodeljenih binarnih semafora)
int numOfReaders = numOfWriters = 0;
Queue queue;

Semaphore mutex = Semaphore(1);
Semaphore read = Semaphore(0);
Semaphore write = Semaphore(0);

void reader(int id) {
    while (true) {
        mutex.wait();
        if (numOfWriters > 0 || queue.size() > 0) {
            queue.insert("reader");
            mutex.signal();
            read.wait();
        }
        numOfReaders++;
        SIGNAL();
        read();
        mutex.wait();
        numOfReaders--;
        SIGNAL();
    }
}

void writer(int id) {
    while (true) {
        mutex.wait();
        if (numOfReaders > 0 || numOfWriters > 0 || queue.size() > 0) {
            queue.insert("writer");
            mutex.signal();
            write.wait();
        }
        numOfWriters++;
        SIGNAL();
        write();
        mutex.wait();
        numOfWriters--;
        SIGNAL();
    }
}

void SIGNAL() {
    if (numOfWriters == 0 && queue.size() > 0 && queue.get(0) == "reader") {
        queue.remove(0);
        read.signal();
    } else if (numOfReaders == 0 && numOfWriters == 0 && queue.size() > 0 && queue.get(0) == "writer") {
        queue.remove(0);
        write.signal();
    } else {
        mutex.signal();
    }
}