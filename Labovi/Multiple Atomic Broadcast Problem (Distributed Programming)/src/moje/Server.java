package moje;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class Server {

	public static void main(String[] args) {
		int serverPort = Integer.parseInt(args[0]);
		int bufferCapacity = Integer.parseInt(args[1]);
		int numOfConsumers = Integer.parseInt(args[2]);
		AtomicBroadcast buffer = new AtomicBroadcast(bufferCapacity, numOfConsumers);
		try (ServerSocket serverSocket = new ServerSocket(serverPort)) {
			System.out.println("Server.java: Server started at port " + serverPort + ".");
			while (true) {
				Socket clientSocket = serverSocket.accept();
				new RequestHandler(clientSocket, buffer).start();
				System.out.println("Server.java: New RequestHandler created.");
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
}
