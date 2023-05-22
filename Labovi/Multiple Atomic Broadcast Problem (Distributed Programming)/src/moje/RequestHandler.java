package moje;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;

public class RequestHandler extends Thread {
	
	Socket clientSocket = null;
	ObjectOutputStream outputStream = null;
	ObjectInputStream inputStream = null;
	IAtomicBroadcast buffer = null;
	private int threadId = -1;

	public RequestHandler(Socket clientSocket, IAtomicBroadcast buffer) {
		this.clientSocket = clientSocket;
		try {
			outputStream = new ObjectOutputStream(clientSocket.getOutputStream());
			inputStream = new ObjectInputStream(clientSocket.getInputStream());
			this.buffer = buffer;
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	@Override
	public void run() {
		try {
			
			boolean connectionOpen = true;
			while (connectionOpen) {
				String operation = (String) inputStream.readObject();
				System.out.println("RequestHandler.java: Operation: " + operation + ".");
				switch (operation) {
					case "put":
						String putName = (String) inputStream.readObject();
						System.out.println("RequestHandler.java: Buffer type: " + putName + ".");
						GoodsImpl putGoods = (GoodsImpl) inputStream.readObject();
						buffer.putGoods(putName, putGoods);
						outputStream.writeObject("ACK");
						break;
					case "get":
						String getName = (String) inputStream.readObject();
						System.out.println("RequestHandler.java: Buffer type: " + getName + ".");
						if (threadId == -1) {
							threadId = buffer.generateThreadId();
						}
						if (threadId == -1) {
							outputStream.writeObject(null);
						} else {
							GoodsImpl getGoods = (GoodsImpl) buffer.getGoods(getName, threadId);
							outputStream.writeObject(getGoods);
						}
						inputStream.readObject();
						break;
					case "end":
						connectionOpen = false;
						break;
					default:
						connectionOpen = false;
						break;
				}
				
			}
			
		} catch (ClassNotFoundException | IOException e) {
			e.printStackTrace();
		} finally {
			try {
				outputStream.close();
				inputStream.close();
				clientSocket.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
}
