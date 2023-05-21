package rs.ac.bg.etf.kdp.sanja.rmi;

import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

import rs.ac.bg.etf.kdp.sanja.viseelementniBafer.AtomicBroadcastBuffer;

public class ClientRemoteAtomicBroadcastBuffer<T> implements AtomicBroadcastBuffer<T> {
	
	private RemoteAtomicBroadcastBuffer<T> stub;

	@SuppressWarnings("unchecked")
	public ClientRemoteAtomicBroadcastBuffer(String serverHost, int serverPort) {
		
		if (System.getSecurityManager() == null) {
			System.setSecurityManager(new SecurityManager());
		}
		
		try {
			Registry registry = LocateRegistry.getRegistry(serverHost, serverPort);
			stub = (RemoteAtomicBroadcastBuffer<T>) registry.lookup("/buffer");
			
		} catch (RemoteException | NotBoundException e) {
			e.printStackTrace();
		}
		
	}

	@Override
	public void put(T item) {
		try {
			stub.put(item);
		} catch (RemoteException e) {
			e.printStackTrace();
		}
	}

	@Override
	public T get(int id) {
		T item = null;
		
		try {
			item = stub.get(id);
		} catch (RemoteException e) {
			e.printStackTrace();
		}
		
		return item;
	}

}
