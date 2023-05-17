// Podrazumevamo da je u pitanju neusmeren graf.

// Resenje 1 - pocetni cvor ne zna nista o topologiji grafa, a svaki cvor ima informacije samo o svojim susedima. 
// Cvor ne moze da zna da li su njegovi susedi vec primili poruku - zato mora da je posalje svim svojim susedima da bismo bili
// sigurni da ce poruka doci do svakog cvora u grafu.

const int N = ...;  // broj cvorova u grafu

// Za svaki cvor u grafu imamo po jedno postansko sanduce.
mbx nodes[N];

// Ovaj cvor inicira sve tako sto prvi posalje poruku bilo kom od preostalih cvorova u grafu.
void initiatiorGraphNode(int nodeId) {
    msg message;
    bool status;
    int firstNodeId = Math.floor(Math.random() * N);

    mbx_put(message, nodes[firstNodeId]);
}

void regularGraphNode(int nodeId) {
    // Lista suseda za nas cvor - ukoliko je clan niza true, cvor sa tim nodeId je sused nasem cvoru, a u suprotnom nije.
    bool neighbors[N];
    msg message;
    bool status;
    int numOfNeighbors = 0;
    // Brojimo koliko nas cvor ima suseda i smestamo taj broj u numOfNeighbors.
    for (int j = 0; j < N; j++) {
        if (neighbors[j]) {
            numOfNeighbors++;
        }
    }

    // Cvor ceka da primi prvu poruku od jednog od svojih suseda.
    mbx_get(message, nodes[nodeId], INF, status);

    // Cvor prosledjuje poruku svim svojim susedima (pa cak i onom od kog je primio poruku, zbog uniformnosti).
    for (int j = 0; j < N; j++) {
        if (neighbors[j]) {
            mbx_put(message, nodes[j]);
        }
    }

    // Cvor ceka da primi poruku od preostalih suseda od kojih je nije primio. Ovo radimo zato sto ce sve ove poruke biti visak i
    // na ovaj nacin cistimo "djubre" iz postanskog sanduceta kako se naredni broadcast neke poruke ne bi zagubio zbog ovih
    // poruka koje su visak.
    for (int i = 0; i < numOfNeighbors - 1; i++) {
        mbx_get(message, nodes[nodeId], INF, status);
    }
}


// Resenje 2 - u ovom resenju pocetni cvor ima informaciju o kompletnoj topologiji grafa, tako da on moze da transformise graf u
// obuhvatno stablo (dakle da svi cvorovi budu povezani, ali da nema petlji - ima minimalan broj grana, tj. svaki cvor ima
// minimalan broj suseda tako da se dobije povezan aciklican graf). Zahvaljujuci ovome smo onda sigurni da ce poruka
// stici do svakog cvora u grafu ukoliko svaki cvor prosledi poruku svim svojim susedima u obuhvatnom stablu (osim onom od kog
// je dobio poruku jer ovde za tim nema potrebe, to bi bio visak poruka). U ovom resenju je onda skroz u redu da svaki cvor primi 
// poruku samo jednom jer nece biti uopste viskova poslatih poruka.

const int N = ...;  // broj cvorova u grafu

typedef struct msg {
    int senderId;
} msg;

// Za svaki cvor u grafu imamo po jedno postansko sanduce.
mbx nodes[N];

// Ovaj cvor inicira sve tako sto prvi posalje poruku bilo kom od preostalih cvorova u grafu.
void initiatiorGraphNode(int nodeId) {
    msg message;
    bool status;
    int firstNodeId = Math.floor(Math.random() * N);
    bool graph[N][N];
    message.spanningTree = createSpanningTree(graph);
    message.senderId = nodeId;

    mbx_put(message, nodes[firstNodeId]);
}

void regularGraphNode(int nodeId) {
    msg message;
    bool status;

    // Cvor ceka da primi poruku od jednog od svojih suseda.
    mbx_get(message, nodes[nodeId], INF, status);

    // Ovde pamtimo Id cvora koji nam je poslao poruku jer njemu ne zelimo da je saljemo nazad. Trebace nam za proveru uslova dole
    // kada budemo slali poruke nasim susedima kako bismo obezbedili da je ne saljemo onom koji je poslao nama.
    int nodeThatSentMeMessage = message.senderId;
    // Upisujemo u strukturu poruke nas nodeId kako bi cvorovi kojima prosledimo poruku znali da smo im je mi poslali, i kako oni
    // onda ne bi nama slali istu tu poruku nazad.
    message.senderId = nodeId;
    // Cvor prosledjuje poruku svim svojim susedima u obuhvatnom stablu (osim onom od kog je primio poruku jer nema potrebe za
    // tim - to bi bio visak poruka).
    for (int j = 0; j < N; j++) {
        if (message.spanningTree[nodeId][j] && j != nodeThatSentMeMessage) {
            mbx_put(message, nodes[j]);
        }
    }
}