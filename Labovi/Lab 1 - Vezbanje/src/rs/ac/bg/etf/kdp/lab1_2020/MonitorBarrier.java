package rs.ac.bg.etf.kdp.lab1_2020;

public class MonitorBarrier implements Barrier {

	private final int totalNumOfThreads;
	private int numOfWaitingThreads = 0;
	private boolean barrierPassed = false;
	
	public MonitorBarrier(int totalNumOfThreads) {
		this.totalNumOfThreads = totalNumOfThreads;
	}
	
	@Override
	public synchronized void arrived() {
		if (++numOfWaitingThreads == totalNumOfThreads) {
			barrierPassed = true;
			notifyAll();
		} else {
			while (!barrierPassed) {
				try {
					wait();
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		}
	}

	@Override
	public synchronized boolean await(long timeout) {
		while (!barrierPassed) {
			long timeBefore = System.currentTimeMillis();
			try {
				wait(timeout);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			long timeAfter = System.currentTimeMillis();
			if (timeout != 0 && timeAfter - timeBefore >= timeout) {
				break;
			}
		}
		return barrierPassed;
	}

}
