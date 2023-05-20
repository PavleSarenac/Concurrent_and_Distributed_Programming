package rs.ac.bg.etf.kdp.sanja.viseelementniBafer;

import java.util.concurrent.Semaphore;

public class SemaphoreAtomicBroadcastBuffer<T> implements AtomicBroadcastBuffer<T> {

	// Broj potrosaca (consumer-a).
	private int N;
	// Velicina bafera.
	private int B;
	// Niz brojaca za svaku poziciju u baferu - broji se za svaku poziciju u baferu
	// koliko potrosaca je citalo sa te pozicije.
	private int cnt[];
	private T buffer[];
	private int writeIndex, readIndex[];

	// Bolja konkurentnost sa nizom mutex-a (za svakog consumer-a po jedan) jer je
	// onda moguce razlicite pozicije u
	// nizu brojaca menjati istovremeno.
	private Semaphore mutex[];
	// Ovaj mutexP smo dodali za slucaj da imamo vise proizvodjaca.
	private Semaphore mutexP;
	private Semaphore empty;
	private Semaphore full[];

	@SuppressWarnings("unchecked")
	public SemaphoreAtomicBroadcastBuffer(int capacity, int n) {
		this.N = n;
		this.B = capacity;
		this.cnt = new int[B];
		this.buffer = (T[]) new Object[B];
		this.writeIndex = 0;
		this.readIndex = new int[N];

		this.mutex = new Semaphore[B];
		for (int i = 0; i < B; i++) {
			cnt[i] = 0;
			mutex[i] = new Semaphore(1);
		}
		this.mutexP = new Semaphore(1);
		this.empty = new Semaphore(B);
		this.full = new Semaphore[N];
		for (int i = 0; i < N; i++) {
			readIndex[i] = 0;
			full[i] = new Semaphore(0);
		}
	}

	@Override
	public void put(T item) {
		empty.acquireUninterruptibly();
		
		mutexP.acquireUninterruptibly();
		buffer[writeIndex] = item;
		writeIndex = (writeIndex + 1) % B;
		mutexP.release();
		
		for (int i = 0; i < full.length; i++) {
			full[i].release();
		}
	}

	@Override
	public T get(int id) {
		if (id < 0 || id >= N) {
			return null;
		}
		
		full[id].acquireUninterruptibly();
		T item = buffer[readIndex[id]];
		mutex[readIndex[id]].acquireUninterruptibly();
		cnt[readIndex[id]]++;
		if (cnt[readIndex[id]] == N) {
			cnt[readIndex[id]] = 0;
			empty.release();
		}
		mutex[readIndex[id]].release();
		readIndex[id] = (readIndex[id] + 1) % B;
		
		return item;
	}

}
