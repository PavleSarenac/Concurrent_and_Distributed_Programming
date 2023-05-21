package rs.ac.bg.etf.kdp.sanja.rmi;

import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;

import rs.ac.bg.etf.kdp.sanja.viseelementniBafer.Message;

public class Server {

	@SuppressWarnings("unchecked")
	public static void main(String[] args) {
		
		int bufferCapacity = 5;
		int numberOfConsumers = 3;
		
		if (System.getSecurityManager() == null) {
			System.setSecurityManager(new SecurityManager());
		}

		RemoteAtomicBroadcastBuffer<Message<String>> buffer = new RemoteAtomicBroadcastBufferImpl<>(bufferCapacity, numberOfConsumers);
		// Za broj porta stavljamo 0 da bi se on dinamicki dodelio i da mi ne bismo brinuli o tome (nece biti zaista broj porta 0, vec
		// neki drugi koji dodeli operativni sistem).
		try {
			RemoteAtomicBroadcastBuffer<Message<String>> stub = (RemoteAtomicBroadcastBuffer<Message<String>>) UnicastRemoteObject.exportObject(buffer, 0);
			Registry registry = LocateRegistry.createRegistry(4001);
			registry.rebind("/buffer", stub);
			
			System.out.println("RMI server started...");
		} catch (RemoteException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}