package rs.ac.bg.etf.kdp.sanja2;

public class Test {

	public static void main(String[] args) {
		int B = 5;  // kapacitet bafera
		int N = 3;  // broj potrosaca (consumer-a)
		
		AtomicBroadcastBuffer<Integer> buffer = new SemaphoreAtomicBroadcastBuffer<>(B, N);
		
		Producer p1  = new Producer("P1", buffer);
		p1.start();
		Producer p2 = new Producer("P2", buffer);
		p2.start();
		
		for (int i = 0; i < N; i++) {
			Consumer c = new Consumer("C" + (i + 1), i, buffer);
			c.start();
		}
		
	}
	
}
