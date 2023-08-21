// Filterski proces - mailbox
const int BUFFER_CAPACITY = ...;

Channel inputChannel(char character);
Channel outputChannel(char[BUFFER_CAPACITY] line);

void char_to_line() {
    char line[BUFFER_CAPACITY];
    int i = 0;
    while (true) {
        receive inputChannel(line[i]);
        while (line[i] != CR && i < BUFFER_CAPACITY - 1) {
            i++;
            receive inputChannel(line[i]);
        }
        line[i] = EOL;
        send outputChannel(line);
        i = 0;
    }
}


// Filterska mreza za sortiranje sa binarnim stablom
Channel inputChannel1(int value1);
Channel inputChannel2(int value2);
Channel outputChannel(int smallerValue);

void merge() {
    int value1, value2;
    receive inputChannel1(value1);
    receive inputChannel2(value2);
    while (value1 != EOS && value2 != EOS) {
        if (value1 < value2) {
            send outputChannel(value1);
            receive inputChannel1(value1);
        } else {
            send outputChannel(value2);
            receive inputChannel2(value2);
        }
        if (value1 == EOS) {
            while (value2 != EOS) {
                send outputChannel(value2);
                receive inputChannel2(value2);
            }
        }
        if (value2 == EOS) {
            while (value1 != EOS) {
                send outputChannel(value1);
                receive inputChannel1(value1);
            }
        }
    }
    send outputChannel(EOS);
}


// K3 - 2023 Jun (1. zadatak) - trazenje minimuma niza od 11 vrednosti
// Procesi ce biti organizovani hijerarhijski sledecim redosledom: 6, 3, 1, 1 => imacemo ukupno 11 procesa
Channel inputChannel1(int value1);
Channel inputChannel2(int valu2);
Channel outputChannel(int smallerValue);

void process() {
    int value1, value2;
    receive inputChannel1(value1);
    receive inputChannel2(value2);
    if (value1 == EOS) {
        send outputChannel(value2);
    } else if (value2 == EOS) {
        send outputChannel(value1);
    } else {
        if (value1 < value2) {
            send outputChannel(value1);
        } else {
            send outputChannel(value2);
        }
    }
}