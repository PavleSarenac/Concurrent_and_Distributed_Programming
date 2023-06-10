// Resenje 1
// Dobro je sto je u ovom resenju moguce da dva filozofa istovremeno jedu.
// Problem sa ovim resenjem je to sto je moguce izgladnjivanje filozofa - ne garantujemo da ce svi jesti u nekom trenutku
// jer je moguce da jedan te isti filozof konstantno dolazi opet da jede i da zbog njega susedni filozofi nikad ne jedu.

int availableforks[5] = {2, 2, 2, 2, 2};  // za svakog filozofa brojimo koliko on ima dostupnih viljuski

void philosopher(int i) {  // i = 0..4 predstavlja indeks jednog filozofa
    int leftPhil = (i - 1) % 5;
    int rightPhil = (i + 1) % 5;
    while (true) {
        think();
        
        region(availableforks) {
            await(availableforks[i] == 2);
            availableforks[leftPhil]--;
            availableforks[rightPhil]--;
        }

        eat();

        region(availableforks) {
            availableforks[leftPhil]++;
            availableforks[rightPhil]++;
        }
    }
}

// Resenje 2 - ovo je sustinski isto kao Resenje 1, samo sto ovde vodimo evidenciju da li filozof razmislja umesto broja dostupnih viljuski. Osim toga, ima istu prednost i manu kao Resenje 1.

int thinking[5] = {true, true, true, true, true};

void philosopher(int i) {
    int leftPhil = (i - 1) % 5;
    int rightPhil = (i + 1) % 5;
    while (true) {
        think();

        region(thinking) {
            await(thinking[leftPhil] && thinking[rightPhil]);
            thinking[i] = false;
        }

        eat();

        region(thinking) {
            thinking[i] = true;
        }
    }
}

// Resenje 3
// Vec se moze naslutiti na prvi pogled da je ovo resenje lose jer imamo region unutar regiona - to vodi ka deadlock-u.
// Npr. ako svi filozofi uzmu istovremenu svoju levu viljusku, niko nece moci da uzme desnu viljusku, dakle niko nece moci da jede i svi ce se zablokirati - deadlock.
// Zakljucak: nije dobra praksa stavljati region unutar regiona.

typedef struct Fork {} Fork;
Fork fork0, fork1, fork2, fork3, fork4;

Fork forks[5] = {fork0, fork1, fork2, fork3, fork4};

void philosopher(int i) {
    Fork leftFork = forks[i];
    Fork rightFork = forks[(i + 1) % 5];    
    while (true) {
        think();

        region(leftFork) {
            region(rightFork) {
                eat();
            }
        }
    }
}
