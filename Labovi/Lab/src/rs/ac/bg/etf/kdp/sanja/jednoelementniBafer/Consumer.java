package rs.ac.bg.etf.kdp.sanja.jednoelementniBafer;

import java.util.concurrent.Semaphore;
import java.util.concurrent.atomic.AtomicInteger;

public class Consumer extends Thread {

	private int n;
	private AtomicInteger cnt, buffer;
	private Semaphore mutex, empty, full;

	public Consumer(String name, int n, AtomicInteger cnt, AtomicInteger buffer, Semaphore mutex, Semaphore empty,
			Semaphore full) {
		super(name);
		this.n = n;
		this.cnt = cnt;
		this.buffer = buffer;
		this.mutex = mutex;
		this.empty = empty;
		this.full = full;
	}

	@Override
	public void run() {
		super.run();
		while (true) {
			full.acquireUninterruptibly();
			int item = buffer.get();
			System.out.println(String.format("%s took %d", this.getName(), item));
			cnt.getAndIncrement(); // ovo ne mora da bude unutar mutex-a jer je cnt objekat klase AtomicInteger gde
									// su sve operacije garantovano atomicne
			mutex.acquireUninterruptibly();
			if (cnt.get() == n) {
				cnt.set(0);
				empty.release();
			}
			mutex.release();
			consume(item);
		}
	}

	private void consume(int item) {
		try {
			sleep((int) (Math.random() * 1500));
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		System.out.println(String.format("%s consumed %d", this.getName(), item));
	}

}
