// Signal and Wait disciplina.
// Izgladnjivanje spreceno tako sto citaoci kad zavrse gledaju da probude pisca, a pisac kad zavrsi prvo gleda da probudi citaoca.

// Posto je u pitanju Signal and Wait disciplina, uslovi ne moraju da se proveravaju u petlji, vec je dovoljno sa if-om jer je
// kod ove discipline garantovano da ce probudjeni proces odmah dobiti ekskluzivno pravo pristupa monitoru, tako da ne moze nista
// od uslova da se promeni od trenutka kada je proces probudjen do trenutka kada bi ih on opet proverio (kada bismo ostavili
// petlju). Zato nema potrebe ponovo ispitivati uslov nakon budjenja, jer ako je probudjen, sigurni smo da treba da dobije
// pristup monitoru.

// Pisac ne budi sve citaoce u endWrite metodi nego samo jednog, a onda u okviru startRead metode citaoca se bude dodatni citaoci
// ukoliko ih ima u redu cekanja. Ovako je implementirano jer kada bi pisac budio sve blokirane citaoce u metodi endWrite (to bi
// mogao da uradi jedino u petlji jer signalAll kod SW discipline ne postoji), to bi posle svakog poziva signal pisca prebacilo u
// entry queue, pa bi performanse programa bile znatno losije - zato je bolje da citaoci bude preostale citaoce u svojoj metodi
// startRead.

monitor RW_SW {
    int cntR = 0;
    int cntW = 0;
    cond rq, wq;

    void startRead() {
        if (cntW > 0 || wq.queue()) {
            rq.wait();
        }
        cntR++;
        if (rq.queue()) {
            rq.signal();  // Posto je ovo poslednja instrukcija ove metode monitora, proces koji je izvrsi, tj. probudi drugi  
        }                 // proces i napusti monitor nece ici u entry queue jer je svoj posao u monitoru zavrsio.
    }

    void endRead() {
        cntR--;
        if (cntR == 0 && wq.queue()) {
            wq.signal();
        }
    }

    void startWrite() {
        if (cntR > 0 || cntW > 0) {
            wq.wait();
        }
        cntW++;
    }

    void endWrite() {
        cntW--;
        if (rq.queue()) {
            rq.signal();  // Ako ima vise blokiranih citalaca ovo ce probuditi samo jednog od njih pa zato oni onda moraju da se
        } else if (wq.queue()) {  // bude dalje sami (prvi budi drugog, drugi treceg itd.).
            wq.signal();
        }
    }

}

// Prikaz koriscenja monitora za sinhronizaciju.

void reader() {
    while (true) {
        RW_SW.startRead()
        read();
        RW_SW.endRead();
    }
}

void writer() {
    while (true) {
        RW_SW.startWrite();
        write();
        RW_SW.endWrite();
    }
}


// Signal and Continue disciplina. Obezbedjen FIFO redosled.

// Koristicemo wait sa prioritetom - wait(priority) da bismo obezbedili FIFO redosled. Zbog toga nam nisu potrebna dva odvojena
// reda za citaoce i pisce, vec ih sve mozemo smestati u jedan posto cemo na osnovu prioriteta znati uvek koga sledeceg budimo.

// U metodi startRead iako je u pitanju Signal and Continue disciplina ne moramo da proveravamo uslov u while petlji, jer kada
// tu citalac bude probudjen on ce sigurno smeti da cita ko god da ga je probudio (postoje dve opcije: 1) probudio 
// ga je citalac, pa sme da cita jer sme vise citalaca da cita istovremeno; 2) probudio ga je pisac, pa opet sme da 
// cita jer sigurno niko u tom trenutku ne pise jer je dozvoljen samo jedan pisac u jednom trenutku;).

// U metodi startWrite iako je u pitanju Signal and Continue disciplina ne moramo da proveravamo uslov u while petlji, jer smo
// se osigurali da kada tu pisac bude probudjen on sigurno sme da pise (to smo obezbedili tako sto citaoci imaju parne ticket-e, 
// a pisci neparne i onda mozemo da proverimo na kraju metode startRead pri pozivanju signal da ga pozivamo samo u slucaju da je
// naredna osoba na redu citalac, tako da se pisci nece buditi bespotrebno). Ostala dva mesta na kojima se poziva signal su
// krajevi endRead i endWrite metoda, tako da u tim slucajevima ce takodje probudjeni pisac sigurno smeti da pise.

monitor RW_SC_FIFO {
    int cntR = 0;
    int ticket = 0;
    int next = 0;
    cond q;

    void startRead() {
        int myTicket = 2 * (ticket++);
        if (myTicket / 2 != next) {
            q.wait(myTicket);
        }
        cntR++;
        next++;
        if (q.queue() && q.min_rank() % 2 == 0) {
            q.signal();
        }
    }

    void endRead() {
        cntR--;
        if (cntR == 0 && q.queue()) {
            q.signal();
        }
    }

    void startWrite() {
        int myTicket = 2 * (ticket++) + 1;
        if (cntR > 0 || myTicket / 2 != next) {
            q.wait(myTicket);
        }
    }

    void endWrite() {
        next++;
        if (q.queue()) {
            q.signal();
        }
    }
}

// Prikaz koriscenja monitora za sinhronizaciju.

void reader() {
    while (true) {
        RW_SC_FIFO.startRead()
        read();
        RW_SC_FIFO.endRead();
    }
}

void writer() {
    while (true) {
        RW_SC_FIFO.startWrite();
        write();
        RW_SC_FIFO.endWrite();
    }
}