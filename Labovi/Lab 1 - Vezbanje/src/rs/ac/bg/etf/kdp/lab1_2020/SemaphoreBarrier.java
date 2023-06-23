package rs.ac.bg.etf.kdp.lab1_2020;

import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;

public class SemaphoreBarrier implements Barrier {
	
	private final int totalNumOfThreads;
	private int numOfWaitingThreads = 0;
	private boolean barrierPassed = false;
	private int numOfAwaitingThreads = 0;
	
	private final Semaphore firstDoor = new Semaphore(1);
	private final Semaphore secondDoor = new Semaphore(0);
	private final Semaphore mutex = new Semaphore(1);
	private final Semaphore awaiter = new Semaphore(0);
	
	public SemaphoreBarrier(int totalNumOfThreads) {
		this.totalNumOfThreads = totalNumOfThreads;
	}

	@Override
	public void arrived() {
		firstDoor.acquireUninterruptibly();
		if (++numOfWaitingThreads == totalNumOfThreads) {
			secondDoor.release();
			mutex.acquireUninterruptibly();
			awaiter.release(numOfAwaitingThreads);
			barrierPassed = true;
			mutex.release();
			
		} else {
			firstDoor.release();
		}
		
		secondDoor.acquireUninterruptibly();
		if (--numOfWaitingThreads == 0) {
			firstDoor.release();
		} else {
			secondDoor.release();
		}
	}

	@Override
	public boolean await(long timeout) {
		if (barrierPassed) {
			return true;
		}
		mutex.acquireUninterruptibly();
		numOfAwaitingThreads++;
		mutex.release();
		if (timeout == 0) {
			awaiter.acquireUninterruptibly();
		} else {
			try {
				awaiter.tryAcquire(timeout, TimeUnit.MILLISECONDS);
			} catch (InterruptedException e) {
				e.printStackTrace();
				return false;
			}
		}
		mutex.acquireUninterruptibly();
		numOfAwaitingThreads--;
		mutex.release();
		return barrierPassed;
	}

}
