/*
Objasnjenje torbe poslova (Bag of Tasks).
- Vazno je naglasiti da je sled dogadjaja uvek fiksan:
    1) proces radnik trazi zadatak od torbe 
    2) torba mu ga salje 
    3) proces radnik odradjuje zadatak
    4) proces radnik salje rezultat (u zavisnosti od varijante salje ili nazad torbi ili posebnom procesu kolektoru)

- Nikada nece torba poslati zadatak nekom procesu radniku a da taj proces radnik nije prethodno trazio zadatak od torbe.

- Postoje dve varijante:
    1) Varijanta bez kolektora. 
        Torba (Bag) zna sta treba da se odradi i ona treba da taj ceo posao izdeli na vise manjih poslova
        koji se obavljaju na isti nacin. Te manje poslove treba da obradjuju procesi radnici (Worker-i) - u ovom primeru to su procesi Node. Procesi radnici (Worker-i) traze zadatak od torbe (Bag), dobijaju ga, izvrsavaju ga, i onda salju rezultat nazad torbi.
    2) Varijanta sa kolektorom. 
        Malo je drugacija sinhronizacija izmedju ucesnika. I dalje procesi radnici (Worker-i) traze zadatke od torbe (Bag), dobijaju ga i izvrsavaju, medjutim ne salju rezultate nazad torbi, vec kolektoru koji sluzi upravo sa prikupljanje
        rezultata. Kada kolektor prikupi dovoljan broj rezultata, onda je ceo posao gotov. U ovom zadatku sa izracunavanjem integrala je tako, dakle kada bi se prikupili svi rezultati to bi znacilo da je integral izracunat i to je onda kraj. 
        
        U nekom drugom problemu, moguce je da se dogodi i da kolektor prikupi odredjen broj rezultata i da to nije onda kraj, vec da onda kolektor sacuva taj rezultat, ali javi torbi (Bag) da je potrebno izvrsiti novi posao i onda da torba (Bag) raspodeli procesima radnicima (Worker-ima) taj novi posao. Tako bi se izvrsavalo onoliko puta koliko poslova je proces kolektor odredio da je potrebno. 
*/


// Ovde je odradjena varijanta bez kolektora jer se rezultat cuva u promenljivoj F procesa Bag (torbe).

[Node(p:1..N)::node || Bag::bag]

node::
[
    double left, right, data;
    *[
        Bag!getTask();
        Bag?getData(left, right) -> [
            CALCULATE;
        ]
        Bag!putResult(data);
    ]
]

bag::
[
    double Xmin, Xmax, dx, x, F = 0, data;
    int N, i = 0;
    INIT;
    *[
        i < N, x < Xmax, Node(p)?getTask() -> [
            Node(p)!getData(x, x + dx);
            x += dx;
        ]

        []

        // Omoguceno je da ako neki procesi radnici brze rade i brze salju onda rezultate nazad, mi cemo te rezultate prihvatati
        // zahvaljujuci logici CSP koja dozvoljava istovremeno osluskivanje veceg broja poruka - nece nas onda kociti sporiji
        // procesi radnici. Ako su neki procesi radnici mnogo brzi od nekih drugih procesa radnika, moze se desiti da ovi drugi
        // ni ne stignu da zatraze da odrade posao, vec da ceo posao odrade brzi procesi radnici, sto je i u redu.
        i < N, Node(p)?putResult(data) -> [
            F += data;
            i++;
        ]
    ]
    STOP;
]