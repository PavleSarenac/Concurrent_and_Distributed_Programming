typedef struct Point {
    int x = 0;
    int y = 0;
    bool full = true;
} Point;

Point point;

void makePoints() {
    for (int i = 1; i <= n; i++) {
        region(point) {
            await(full == false);
            x = i;
            y = i * i;
            full = true;
        }
    }
}

void printPoints() {
    for (int i = 0; i <= n; i++) {
        region(point) {
            await(full == true);
            print(x);
            print(y);
            full = false;
        }
    }
}