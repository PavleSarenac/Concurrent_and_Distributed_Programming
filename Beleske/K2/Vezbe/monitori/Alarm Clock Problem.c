// Koristicemo monitor sa Signal and Wait disciplinom.

// Upravo zato sto je Signal and Wait disciplina, vazno je da u metodi tick ne signaliziramo u petlji, vec da stoji if umesto
// while jer kada bismo u petlji budili sve procese koje treba, jedan poziv metode tick bi potencijalno mnogo dugo trajao jer
// se kod Signal and Wait discipline monitora proces koji signalizira blokira i odlazi na entry queue, a onaj sto se budi dobija
// odmah pravo pristupa monitoru. Kada stavimo if, proces koji signalizira nece ici na entry queue jer je u tom slucaju
// signaliziranje poslednja instrukcija metode i tada imamo tu optimizaciju.

// Zato je mnogo bolje da u metodi tick budimo jedan proces i onda da u metodi sleep taj probudjeni proces probudi neki drugi
// ukoliko treba, pa ce i taj drugi probuditi neki treci ako treba itd. - rasporedili smo signalizaciju tako da se ne dogodi
// da monitor bude predugo blokiran.

// Zakljucak: kod monitora sa Signal and Wait disciplinom treba izbegavati budjenje procesa u petlji jer onda moze da se dogodi
// da nam monitor bude predugo blokiran.


monitor AlarmClock {
    int now = 0;
    cond check;

    void sleep(int n) {
        int wake_up_time = now + n;
        if (wake_up_time > now) {
            check.wait(wake_up_time);
        }
        if (check.queue() && check.min_rank() <= now) {
            check.signal();
        }
    }

    void tick() {
        now++;
        if (check.queue() && check.min_rank() <= now) {
            check.signal();
        }
    }
}
