// Data je kvadratna matrica A reda 3. Na ulaz dolaze 3 niza podataka, od kojih svaki predstavlja po jedan element vektora IN.
// Tri niza podataka treba da se pojave na izlazu, od kojih svaki predstavlja po jedan element proizvoda INA. Posle pocetnog
// kasnjenja, rezultati treba da se proizvode istom brzinom kojom stize ulaz. Matrica A je fiksna.

[M(i:1..3, 0)::west || M(0, j:1..3)::north || M(i:1..3, 4)::east || M(4, j:1..3)::south || M(i:1..3, j:1..3)::center]

// Procesi west i south su korisnicki programi i njihova implementacija nas ne zanima ovde.
// west se odnosi na dodatnu kolonu levo od osnovne matrice, a south se odnosi na dodatan red ispod osnovne matrice

// north se odnosi na dodatan red iznad osnovne matrice - odatle cvorovi samo salju elementima prve vrste osnovne matrice nulu
north::
*[
    true -> [  
        M(1, j)!0;
    ]
]

// east se odnosi na dodatnu kolonu desno od osnovne matrice - ovaj proces prima elemente od trece kolone osnovne matrice samo
// da bi nase resenje radilo - uvek cvorovi osnovne matrice prosledjuju x na desno, pa da bi kod bio isti i za trecu kolonu
// osnovne matrice dodali smo ovu east kolonu koja samo prima x i nista vise ne radi
east::
*[
    double x;
    M(i, 3)?x -> [
        SKIP;
    ]
]

// center se odnosi na nasu osnovnu matricu
center::
*[
    double x;
    M(i, j-1)?x -> [
        M(i, j+1)!x;  // na desno se uvek prosledjuje x, sto predstavlja broj sa kojim treba da se pomnozi cvor matrice
        double sum;
        M(i-1, j)?sum;  // odozgo cvor dobija sumu koju treba da doda na proizvod svoje vrednosti i broja x
        M(i+1, j)!(A(i, j)*x + sum);  // na dole cvor prosledjuje finalnu sumu za tekuci cvor
    ]
]