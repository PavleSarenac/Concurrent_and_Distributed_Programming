package moje;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;

public class Server {

	public static void main(String[] args) {
		int serverPort = Integer.parseInt(args[0]);
		int bufferCapacity = Integer.parseInt(args[1]);
		int numOfConsumers = Integer.parseInt(args[2]);
		IAtomicBroadcast buffer = new AtomicBroadcast(bufferCapacity, numOfConsumers);
		if (System.getSecurityManager() == null) {
			System.setSecurityManager(new SecurityManager());
		}
		try {
			IAtomicBroadcast stub = (IAtomicBroadcast) UnicastRemoteObject.exportObject(buffer, 0);
			Registry registry = LocateRegistry.createRegistry(serverPort + 1);
			registry.rebind("/buffer", stub);
			System.out.println("Server.java: Buffer is in the registry and ready for RMI.");
		} catch (RemoteException e1) {
			e1.printStackTrace();
		}
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
