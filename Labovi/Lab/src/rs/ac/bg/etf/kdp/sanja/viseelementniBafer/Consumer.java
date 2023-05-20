package rs.ac.bg.etf.kdp.sanja.viseelementniBafer;

public class Consumer extends Thread {

	private int id;
	AtomicBroadcastBuffer<Integer> buffer;

	public Consumer(String name, int id, AtomicBroadcastBuffer<Integer> buffer) {
		super(name);
		this.id = id;
		this.buffer = buffer;
	}

	@Override
	public void run() {
		super.run();
		while (true) {
			int item = buffer.get(id);
			System.out.println(getName() + " - " + item);
			try {
				sleep((int) (Math.random() * 2000));
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	
}
