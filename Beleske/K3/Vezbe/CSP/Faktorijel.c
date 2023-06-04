// Faktorijel izracunat rekurzivnom metodom, do odredjene granice.

// Redom se prvo propagiraju vrednosti od n do LIMIT od procesa Factorial(0) do procesa Factorial(LIMIT), i onda se rezultati
// propagiraju nazad od procesa Factorial(LIMIT) do procesa Factiorial(0), da bi na kraju procesa Factorial(0) zaista i dobio
// kao rezultat vrednost faktorijela od broja n.

[Factorial(0)::userNode || (i:1..LIMIT)Factorial(i)::regularNode]

// ova metoda je odgovorna za zapocinjanje lanca izracunavanja faktorijela
userNode::
[
    int n = ...;
    int nFactorial;  // u ovu promenljivu cemo smestiti izracunat faktorijel
    Factorial(1)!n;  // ovde saljemo prvom cvoru u lancu broj n jer zelimo da izracunamo faktorijel od n
    Factorial(1)?nFactioral;
]

// ova metoda definise racunanje faktorijela u svakom cvoru u lancu izracunavanja
regularNode::
[
    *[
        int n;
        Factorial(i-1)?n -> [
            n == 0 -> [
                Factorial(i-1)!1;
            ]

            []

            n > 0 -> [
                Factorial(i+1)!n-1;  // dajemo narednom cvoru u lancu da racuna faktorijel rekurzivno od n-1
                int r;
                Factorial(i+1)?r;  // rezultat koji nam vrati taj naredni cvor je zapravo faktorijel od n-1 i to smestamo u r
                Factorial(i-1)!n*r;
            ]
        ]
    ]
]