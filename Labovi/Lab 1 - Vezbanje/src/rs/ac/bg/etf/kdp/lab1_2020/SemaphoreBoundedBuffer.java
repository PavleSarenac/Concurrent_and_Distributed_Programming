package rs.ac.bg.etf.kdp.lab1_2020;

import java.util.concurrent.Semaphore;

public class SemaphoreBoundedBuffer<T> implements BoundedBuffer<T> {
	
	private final T buffer[];
	private int readIndex;
	private int writeIndex;
	private int capacity;
	
	private final Semaphore mutexProducer;
	private final Semaphore mutexConsumer;
	private final Semaphore empty;
	private final Semaphore full;
	
	@SuppressWarnings("unchecked")
	public SemaphoreBoundedBuffer(int capacity) {
		readIndex = writeIndex = 0;
		buffer = (T[]) new Object[capacity];
		this.capacity = capacity;
		
		mutexProducer = new Semaphore(1);
		mutexConsumer = new Semaphore(1);
		empty = new Semaphore(capacity);
		full = new Semaphore(0);
	}

	@Override
	public void put(T data) {
		empty.acquireUninterruptibly();
		
		mutexProducer.acquireUninterruptibly();
		buffer[writeIndex] = data;
		writeIndex = (writeIndex + 1) % capacity;
		mutexProducer.release();
		
		full.release();
	}

	@Override
	public T get() {
		full.acquireUninterruptibly();
		
		mutexConsumer.acquireUninterruptibly();
		T data = buffer[readIndex];
		readIndex = (readIndex + 1) % capacity;
		mutexConsumer.release();
		
		empty.release();
		return data;
	}

}
