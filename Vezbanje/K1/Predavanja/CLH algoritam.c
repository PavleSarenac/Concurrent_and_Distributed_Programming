// CLH algoritam - coarse-grain

// na cvoru prev tekuci proces ceka za ulazak u kriticnu sekciju, dok ce na 
// cvoru newNode cekati naredni proces koji pokusa da udje u kriticnu sekciju
	
typedef struct Node {
    bool flag;
    Node(bool f) {
        flag = f;
    }
} Node;

Node tail = Node(true);

void process(int id) {
    Node prev, newNode;
    while (true) {
        newNode = Node(false);
        <prev = tail; tail = newNode;>
        <await(prev->flag == true);>
        // critical section
        newNode->flag = true;  // pustamo naredni proces po FIFO redosledu da udje u kriticnu sekciju
        // noncritical section
    }
}



// CLH algoritam - fine-grain

// na cvoru prev tekuci proces ceka za ulazak u kriticnu sekciju, dok ce na 
// cvoru newNode cekati naredni proces koji pokusa da udje u kriticnu sekciju
	
typedef struct Node {
    bool flag;
    Node(bool f) {
        flag = f;
    }
} Node;

Node tail = Node(true);

void process(int id) {
    Node prev, newNode;
    while (true) {
        newNode = Node(false);
        prev = GS(tail, newNode);
        while (prev->flag == false) skip();
        // critical section
        newNode->flag = true;  // pustamo naredni proces po FIFO redosledu da udje u kriticnu sekciju
        // noncritical section
    }
}