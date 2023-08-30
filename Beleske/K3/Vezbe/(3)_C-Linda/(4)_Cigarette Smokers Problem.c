void agent() {
    while (true) {
        int i = random(0, 3);
        switch(i) {
            case 0:
                out("smokerMatches");
                out("paper");
                out("tobacco");
                break;
            case 1:
                out("smokerTobacco");
                out("paper");
                out("matches");
                break;
            case 2:
                out("smokerPaper");
                out("tobacco");
                out("matches");
                break;
        }
        in("doneSmoking");
    }
}

void smokerMatches() {
    while (true) {
        in("smokerMatches");
        in("paper");
        in("tobacco");
        smoke();
        out("doneSmoking");
    }
}

void smokerTobacco() {
    while (true) {
        in("smokerTobacco");
        in("paper");
        in("matches");
        smoke();
        out("doneSmoking");
    }
}

void smokerPaper() {
    while (true) {
        in("smokerPaper");
        in("tobacco");
        in("matches");
        smoke();
        out("doneSmoking");
    }
}