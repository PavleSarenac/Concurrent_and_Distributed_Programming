package lab2;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;

public class Server {

	public static void main(String[] args) throws Exception {
		// May require the -Djava.security.manager=allow JVM argument.
		if (System.getSecurityManager() == null) {
			System.setSecurityManager(new SecurityManager());
		}
		int port = Integer.parseInt(args[0]);
		int capacity = Integer.parseInt(args[1]);
		int consumers = Integer.parseInt(args[2]);
		IAtomicBroadcast ab = new AtomicBroadcast(capacity, consumers);
		try (ServerSocket ss = new ServerSocket(port)) {
			Registry registry = LocateRegistry.createRegistry(port + 1);
			IAtomicBroadcast stub = (IAtomicBroadcast) UnicastRemoteObject.exportObject(ab, 0);
			registry.rebind("/ab", stub);
			System.out.println("Registry started.");
			while (true) {
				try {
					Socket s = ss.accept();
					System.out.println("Accepted socket.");
					new ServerThread(s, ab).start();
				} catch (IOException e) {
				}
			}
		}
	}

}
