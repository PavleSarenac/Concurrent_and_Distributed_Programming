// Semafor implementiran u CSP
// Procesi se implicitno blokiraju kada pozovu wait ukoliko nije ispunjen uslov s > 0

sem::
[
    int s = ...;
    *[
        s > 0, P?wait() -> [
            s--;
        ]
        []
        P?signal() -> [
            s++;
        ]
    ]
]