package rs.ac.bg.etf.kdp.sanja.jednoelementniBafer;

import java.util.concurrent.Semaphore;
import java.util.concurrent.atomic.AtomicInteger;

public class Test {

	public static void main(String[] args) {
		int N = 3; // broj potrosaca (consumer-a)
		int B = 1; // velicina bafera

		AtomicInteger cnt = new AtomicInteger(0); // brojac potrosaca koji su procitali iz bafera
		AtomicInteger buffer = new AtomicInteger(0);

		Semaphore mutex = new Semaphore(1);
		Semaphore empty = new Semaphore(B);
		Semaphore full[] = new Semaphore[N];
		for (int i = 0; i < N; i++) {
			full[i] = new Semaphore(0);
		}

		Producer p = new Producer("P1", N, buffer, empty, full);
		p.start();

		for (int i = 0; i < N; i++) {
			Consumer c = new Consumer("C" + (i + 1), N, cnt, buffer, mutex, empty, full[i]);
			c.start();
		}
	}

}
