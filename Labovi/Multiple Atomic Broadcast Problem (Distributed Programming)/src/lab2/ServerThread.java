package lab2;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;

public class ServerThread extends Thread {
	private final Socket s;
	private final ObjectInputStream ois;
	private final ObjectOutputStream oos;
	private final IAtomicBroadcast ab;
	private int id = -1;

	public ServerThread(Socket s, IAtomicBroadcast ab) throws IOException {
		this.s = s;
		ois = new ObjectInputStream(s.getInputStream());
		oos = new ObjectOutputStream(s.getOutputStream());
		this.ab = ab;
	}

	@Override
	public void run() {
		try {
			boolean running = true;
			while (running) {
				String command = ois.readUTF();
				System.out.println("Received command: " + command);
				String name;
				GoodsImpl goods;
				switch (command.toLowerCase()) {
					case "put":
						name = ois.readUTF();
						goods = (GoodsImpl) ois.readObject();
						ab.putGoods(name, goods);
						oos.writeObject("ACK");
						break;
					case "get":
						// Not used by the socket side (Producer class).
						name = ois.readUTF();
						if (id == -1) {
							id = ab.getId();
						}
						if (id == -1) {
							oos.writeObject(null);
						} else {
							goods = ab.getGoods(name, id);
							oos.writeObject(goods);
						}
						break;
					case "end":
						System.out.println("Ending socket connection.");
						running = false;
						break;
					default:
						System.err.println("Invalid command: " + command);
						break;
				}
			}
		} catch (IOException | ClassNotFoundException e) {
			e.printStackTrace();
		} finally {
			try {
				ois.close();
				oos.close();
				s.close();
			} catch (IOException e1) {
				e1.printStackTrace();
			}
		}
	}
}
