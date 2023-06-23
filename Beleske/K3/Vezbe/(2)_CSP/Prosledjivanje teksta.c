[West::west || East::east || X::method]

// Resenje 1 - metoda samo treba da prosledjuje karaktere od procesa West procesu East
method::
*[
    char c;
    West?c -> East!c;
]

//Resenje 2 - metoda treba svake dve susedne "**" koje dobije od procesa West da zameni sa "^" i da to prosledi procesu East
method::
*[
    char c;
    West?c -> [
        c != "*" -> [
            East!c;
        ]
        []
        c == "*" -> [
            West?c -> [
                c != "*" -> [
                    East!"*";
                    East!c;
                ]
                []
                c == "*" -> [
                    East!"^";
                ]
            ]
        ]
    ]
]