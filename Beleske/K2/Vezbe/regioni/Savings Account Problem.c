const int N = ...;  // Broj racuna u banci.

typedef struct Account {
    int value = 0;
    int ticket = 0;
    int next = 0;
} Account;

Account accounts[N];

// Receno u tekstu da treba obezbediti FIFO redosled za isplate novca sa racuna.
void withdraw(int accNum, int amount) {
    int myTicket;
    region(accounts[accNum]) {
        myTicket = ticket++;
        await(myTicket == next && value >= amount);
        value -= amount;
        next++;
    }
}

void payment(int accNum, int amount) {
    region(accounts[accNum]) {
        value += amount;
    }
}

void checkBalance(int accNum) {
    int balance;
    region (accounts[accNum]) {
        balance = value;
    }
    return balance;
}