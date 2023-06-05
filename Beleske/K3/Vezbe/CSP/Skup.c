// Treba predstaviti skup od najvise 100 celih brojeva kao proces S koji prihvata dva tipa instrukcija od pozivajuceg procesa X:
// 1) S!insert(n) - ubacuje ceo broj n u skup
// 2) S!has(n); ... ; S?b; - gde je b == true ako se n nalazi u skupu, a u suprotnom je b == false
// U pocetku je skup prazan.

[S::set || X::user]

set::
[
    int setContent(0..99);  // sadrzaj skupa
    int size = 0;  // skup je na pocetku prazan
    int n;
    *[
        X?has(n) -> [
            SEARCH_SET;  // ova funkcija pretrazuje skup i gleda da li u njemu postoji element n
            X!(i < size);  // ako je i < size to znaci da se pretraga zaustavila pre kraja skupa u funkciji SEARCH, tj.
        ]                  // element n je nadjen u skupu; u suprotnom, element n nije nadjen u skupu

        []

        X?insert(n) -> [
            SEARCH_SET;  // ova funkcija pretrazuje skup i gleda da li u njemu postoji element n
            [
                i < size -> [  // ako element n vec postoji u skupu, necemo da ga umecemo
                    SKIP;
                ]

                []

                i == size and size < 100 -> [  // ako element n ne postoji u skupu i ima jos mesta u skupu, dodajemo element n
                    setContent(size) = n;  // ubacivanje elementa u skup na odgovarajucu poziciju size
                    size++;
                ]
            ]
        ]
    ]
]

user:: 
*[
    int n = Math.random() * 1000;  // generisanje nasumicnog broja od 0 do 999
    bool doesHaveN;
    S!insert(n);
    S!has(n);
    S?doesHaveN;
]

SEARCH_SET:
[
    int i = 0;
    *[
        i < size and setContent(i) != n -> [
            i++;
        ]
    ]
]


// Prosirenje inicijalnog resenja: obezbediti brzi metod za skeniranje svih elemenata skupa, bez menjanja vrednosti

[S::set || X::user]

set::
[
    int setContent(0..99);  // sadrzaj skupa
    int size = 0;  // skup je na pocetku prazan
    int n;
    *[
        X?scan() -> [
        int i = 0;
        *[
            i < size -> [
                X!next(setContent(i));
                i++;
            ]
        ]
        X!noneLeft();
        ]
    ]
]

// iterator za skup
user::
*[
    S!scan();  // ovako se skup postavlja u skenirajuci rezim
    bool more = true;
    int x;
    *[
        more == true, S?next(x) -> [
            // obrada dohvacenog x
        ]

        []

        more == true, S?noneLeft() -> [
            more = false;
        ]
    ]
]


// Prosirenje inicijalnog resenja: isti je problem kao inicijalni sa operacijama has(n) i insert(n), i isto imamo skup sa 
// kapacitetom 100, samo je razlika sto sada to treba resiti pomocu niza procesa od kojih ce svaki sadrzati najvise jedan broj.
// Ukoliko proces ne sadrzi nijedan broj, on treba da na svaki upit has(n) da odgovori sa false.

// Proces se nalazi u pocetnom stanju kada nema sadrzaj. Po prvom ubacivanju elementa u taj proces, on menja stanje u stanje u kom
// sada prima poruke od prethodnog procesa i po potrebi salje podatak sledecem procesu u nizu.

// Pozivajuci proces je proces S(0) koji, kada hoce da posalje podatak u skup, salje ga ka procesu S(1), a kada ispituje da li je
// podatak u skupu radi sledece: S(1)!has(n); ... ; [(i: 1..100)S(i)?b -> skip]
// Zbog efikasnosti pretrage, skup treba da bude sortiran.

[S(i:1..100)::set]

set::
[
    int n = ...;
    *[
        // Ako ovde nas proces S(i) primi signal has(n) od procesa S(i-1), znamo da se sigurno nije izvrsio insert u proces S(i),
        // sto znaci da proces S(i) ne sadrzi nijedan broj i zato on odmah odgovara procesu S(0) sa false - tako je receno u 
        // tekstu da treba.
        S(i-1)?has(n) -> [
            S(0)!false;
        ]

        []
        
        // Kada proces S(i) primi prvu poruku insert, upise se vrednost u promenljivu n i od tog trenutka proces S(i) sadrzi taj
        // broj. Nadalje, svaki signal has(m) ili insert(m) ce proces S(i) cekati unutar ove iterativne komande.
        // Na ovo se mislilo u tekstu kada se govorilo da proces menja stanje kada prvi put primi element - sada je proces S(i)
        // u stanju u kom prima poruke od prethodnog procesa i po potrebi salje podatak sledecem procesu u nizu.
        S(i-1)?insert(n) -> *[
            int m;
            // Ovo je odradjeno ovako jer je skup sortiran rastuce. Zato, ako je m <= n, to znaci da je broj n od procesa S(i)
            // sigurno ili jednak m ili veci od m upravo zbog te uredjenosti skupa. Ukoliko je m > n, onda treba da se prosledi
            // m sledecem procesu u nizu dok se ne naidje na neko n za koje ce vaziti m <= n. Takvo n jedino moze potencijalno
            // da bude jednako m.
            S(i-1)?has(m) -> [
                m <= n -> [
                    S(0)!(m == n);
                ]

                []

                m > n -> [
                    S(i+1)!has(m);
                ]
            ]

            []

            S(i-1)?insert(m) -> [
                // Ovo radimo da bismo ocuvali rastuce uredjen skup pri umetanju novog elementa.
                m < n -> [
                    S(i+1)!insert(n);
                    n = m;
                ]

                []

                // Ako je element m vec u skupu necemo da ga umecemo.
                m == n -> [
                    SKIP;
                ]

                []

                // Ovo radimo da bismo ocuvali rastuce uredjen skup pri umetanju novog elementa.
                m > n -> [
                    S(i+1)!insert(m);
                ]
            ]
        ]
    ] 
]