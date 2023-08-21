// Resenje 1 - filozofi i viljuske su procesi.

[Philosopher(i:0..4)::philosopher || Fork(i:0..4)::fork || Room::room]

philosopher::
[
    int leftFork = i;
    int rightFork = (i + 1) % 5;
    *[
        true -> [
            THINK;

            Room!wantingToEat();
            Fork(leftFork)!takingFork();
            Fork(rightFork)!takingFork();

            EAT;

            Fork(leftFork)!returningFork();
            Fork(rightFork)!returningFork();
            Room!bothForksReturned();
        ]
    ]
]

// Ovde je implicitno obezbedjeno kada neki filozof uzme viljusku da se onda ulazi u dublju granu u kojoj se ceka da taj isti
// filozof i vrati tu viljusku. Na taj nacin se ne obradjuju novi zahtevi za uzimanjem te viljuske sve dok ovaj filozof ne
// vrati viljusku. Tako implicitno blokiramo procese koji su zatrazili ovu istu viljusku dok je ona zauzeta tako sto oni 
// cekaju da proces viljuska primi njihovu poruku.

// Nedeterministicki je odredjeno koji filozof ce dobiti viljusku ukoliko je njih vise zatrazi istovremeno - samo znamo da ce
// je dobiti tacno jedan filozof, to je obezbedjeno u CSP.
fork::
[
    int leftPhilosopher = (i - 1) % 5;
    int rightPhilosopher = i;
    *[
        Philosopher(leftPhilosopher)?takingFork() -> [
            Philosopher(leftPhilosopher)?returningFork();
        ]

        []

        Philosopher(rightPhilosopher)?takingFork() -> [
            Philosopher(rightPhilosopher)?returningFork();
        ]
    ]
]

// Room simulira semafor koji ogranicava filozofe tako da istovremeno maksimalno 4 filozofa moze da zatrazi viljuske - na
// taj nacin izbegavamo deadlock (mrtvu blokadu) - ovo je ono resenje sa predmeta OS2.
room::
[
    int semaphoreValue = 4;
    *[
        semaphoreValue > 0, Philosopher(i)?wantingToEat() -> [
            semaphoreValue--;
        ]
        
        []

        Philosopher(i)?bothForksReturned() -> [
            semaphoreValue++;
        ]
    ]
]