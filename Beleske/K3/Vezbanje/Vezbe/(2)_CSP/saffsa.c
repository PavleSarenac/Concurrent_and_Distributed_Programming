[West::west || East::east || X::method]

method::
[
    char c;
    * [
        East?c -> [
        West!c;
        ]
    ]
]

method::
[
    * [
        char c;
        East?c -> [
            c != "*" -> [
                West!c;
            ]
            []
            c == "*" -> [
                East?c -> [
                    c != "*" -> [
                        West!"*";
                        West!c;
                    ]
                    []
                    c == "*" -> [
                        West!"^";
                    ]
                ]
            ]
        ]
    ]
]