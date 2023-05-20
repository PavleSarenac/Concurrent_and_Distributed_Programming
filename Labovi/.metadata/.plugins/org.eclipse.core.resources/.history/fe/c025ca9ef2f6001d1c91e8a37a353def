package rs.ac.bg.etf.kdp.sanja;

import java.util.concurrent.Semaphore;
import java.util.concurrent.atomic.AtomicInteger;

public class Producer extends Thread {

	private AtomicInteger buffer;
	private int n;
	private Semaphore empty, full[];

	public Producer(String name, int n, AtomicInteger buffer, Semaphore empty, Semaphore full[]) {
		super(name);
		this.buffer = buffer;
		this.empty = empty;
		this.full = full;
		this.n = n;
	}

	@Override
	public void run() {
		super.run();
		while (true) {
			int item = produce();
			empty.acquireUninterruptibly();
			buffer.set(item);
			System.out.println(this.getName() + " produced " + item);
			for (int i = 0; i < n; i++) {
				full[i].release();
			}
		}
	}

	private int produce() {
		try {
			sleep((int) (Math.random() * 2000));
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		return (int) (Math.random() * 100) + 1;
	}

}
