package lab2;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;

public class NetAB implements AB {

	private Socket s;
	private ObjectOutputStream oos;
	private ObjectInputStream ois;

	@Override
	public boolean init(String host, int port) {
		try {
			s = new Socket(host, port);
			oos = new ObjectOutputStream(s.getOutputStream());
			ois = new ObjectInputStream(s.getInputStream());
			return true;
		} catch (IOException e) {
			return false;
		}
	}

	@Override
	public boolean close() {
		try {
			oos.writeUTF("end");
			oos.flush();
			oos.close();
			ois.close();
			s.close();
			return true;
		} catch (IOException e) {
			return false;
		}
	}

	@Override
	public void putGoods(String name, Goods goods) {
		GoodsImpl sendGoods = null;
		if (goods instanceof GoodsImpl) {
			// If the client has passed a GoodsImpl object, it is
			// serializable so we only need to cast it.
			sendGoods = (GoodsImpl) goods;
		} else {
			// Otherwise, wrap our Goods object in a serializable
			// class so we can send it over an ObjectOutputStream.
			sendGoods = new GoodsImpl(goods);
		}
		try {
			// Request
			oos.writeUTF("put");
			oos.writeUTF(name);
			oos.writeObject(sendGoods);
			// Response
			ois.readObject();
		} catch (IOException | ClassNotFoundException e) {
			e.printStackTrace();
		}
	}

	@Override
	public Goods getGoods(String name) {
		GoodsImpl goods = null;
		try {
			// Request
			oos.writeUTF("get");
			oos.writeUTF(name);
			// Response
			goods = (GoodsImpl) ois.readObject();
		} catch (IOException | ClassNotFoundException e) {
			e.printStackTrace();
		}
		return goods;
	}

}
