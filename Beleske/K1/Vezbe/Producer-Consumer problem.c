const int BUFF_SIZE = ...;

int buffer[BUFF_SIZE] = {};
int front = 0, rear = 0;

sem mutexP = 1;
sem mutexC = 1;
sem empty = BUFF_SIZE;
sem full = 0;

void producer() {
	while (1) {
		int item = produce();
		wait(empty);
		wait(mutexP);
		buffer[rear] = item; rear = (rear + 1) % BUFF_SIZE;
		signal(mutexP);
		signal(full);
	}
}
void consumer() {
	while (1) {
		wait(full);
		wait(mutexC);
		int item = buffer[front]; front = (front + 1) % BUFF_SIZE;
		signal(mutexC);
		signal(empty);
		consume(item);
	}
}