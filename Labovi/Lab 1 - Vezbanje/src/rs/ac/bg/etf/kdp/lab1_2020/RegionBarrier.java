package rs.ac.bg.etf.kdp.lab1_2020;

public class RegionBarrier implements Barrier {

	private static class Barrier {
		public int numOfWaitingThreads = 0;
		public boolean barrierPassed = false;
	}
	
	private final int totalNumOfThreads;
	private final Barrier barrier;
	
	public RegionBarrier(int totalNumOfThreads) {
		this.totalNumOfThreads = totalNumOfThreads;
		barrier = new Barrier();
	}

	@Override
	public void arrived() {
		synchronized (barrier) {
			if (++barrier.numOfWaitingThreads == totalNumOfThreads) {
				barrier.barrierPassed = true;
				barrier.notifyAll();
			} else {
				while (!barrier.barrierPassed) {
					try {
						barrier.wait();
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
			}
		}
	}

	@Override
	public boolean await(long timeout) {
		synchronized (barrier) {
			while (!barrier.barrierPassed) {
				long timeBefore = System.currentTimeMillis();
				try {
					barrier.wait(timeout);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
				long timeAfter = System.currentTimeMillis();
				if (timeout != 0 && (timeAfter - timeBefore) >= timeout) {
					break;
				}
			}
			return barrier.barrierPassed;
		}
	}
	
}
