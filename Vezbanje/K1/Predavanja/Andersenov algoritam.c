// Andersenov algoritam - coarse-grain
const int N = ...;  // broj procesa

int slot = 0;  // promenljiva slot ima istu ulogu kao promenljiva ticket u Ticket algoritmu
bool flag[N] = {false};  // svaki proces ima svoju bool promenljivu koja mu govori da li moze da udje u kriticnu sekciju
flag[1] = true;

void process(int id) {  // id = 0..N-1
    int mySlot;
    while (true) {
        <mySlot = (slot + 1) % N; slot++>
        <await(flag[mySlot] == true);>
        // critical section
        flag[mySlot] = false;
        flag[(mySlot + 1) % N] = true;
        // noncritical section
    }
}



// Andersenov algoritam - fine-grain
const int N = ...;  // broj procesa

int slot = 0;  // promenljiva slot ima istu ulogu kao promenljiva ticket u Ticket algoritmu
int flag[N] = {false};  // svaki proces ima svoju bool promenljivu koja mu govori da li moze da udje u kriticnu sekciju
flag[1] = true;

void process(int id) {  // id = 0..N-1
    int mySlot;
    while (true) {
        mySlot = (FA(slot, 1) + 1) % N;
        while (flag[mySlot] == false) skip();
        // critical section
        flag[mySlot] = false;
        flag[(mySlot + 1) % N] = true;
        // noncritical section
    }
}