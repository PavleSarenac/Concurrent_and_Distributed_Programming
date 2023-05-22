package moje;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;

public class NetAB implements AB {
	
	Socket clientSocket = null;
	ObjectOutputStream outputStream = null;
	ObjectInputStream inputStream = null;

	@Override
	public boolean init(String host, int port) {
		try {
			clientSocket = new Socket(host, port);
			outputStream = new ObjectOutputStream(clientSocket.getOutputStream());
			inputStream = new ObjectInputStream(clientSocket.getInputStream());
			return true;
		} catch (Exception e) {
			return false;
		}
	}

	@Override
	public boolean close() {
		try {
			outputStream.writeObject("end");
			outputStream.close();
			inputStream.close();
			clientSocket.close();
			return true;
		} catch (IOException e) {
			return false;
		}
	}

	@Override
	public void putGoods(String name, Goods goods) {
		GoodsImpl checkedGoods;
		if (goods instanceof GoodsImpl) {
			checkedGoods = (GoodsImpl) goods;
		} else {
			checkedGoods = new GoodsImpl(goods);
		}
		try {
			outputStream.writeObject("put");
			outputStream.writeObject(name);
			outputStream.writeObject(checkedGoods);
			inputStream.readObject();
			
		} catch (IOException | ClassNotFoundException e) {
			e.printStackTrace();
		}
	}

	@Override
	public Goods getGoods(String name) {
		GoodsImpl goods = null;
		
		try {
			outputStream.writeObject("get");
			outputStream.writeObject(name);
			goods = (GoodsImpl) inputStream.readObject();
			outputStream.writeObject("ACK");
		} catch (IOException | ClassNotFoundException e) {
			e.printStackTrace();
		}
		
		return goods;
	}

}
