package rs.ac.bg.etf.kdp.sanja.viseelementniBafer;

public class Producer extends Thread {

	ABB<Integer> buffer;

	public Producer(String name, ABB<Integer> buffer) {
		super(name);
		this.buffer = buffer;
	}

	@Override
	public void run() {
		super.run();
		while (true) {
			int item = (int) (Math.random() * 100) + 1;
			buffer.put(item);
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
