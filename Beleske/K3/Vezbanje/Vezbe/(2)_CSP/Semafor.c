sem::
[
    int s = ...;
    * [
        s > 0, P?wait() -> [
            s--;
        ]
        []
        P?signal() -> [
            s++;
        ]
    ]
]