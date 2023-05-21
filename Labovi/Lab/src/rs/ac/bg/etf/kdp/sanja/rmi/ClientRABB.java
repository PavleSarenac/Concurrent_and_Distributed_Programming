package rs.ac.bg.etf.kdp.sanja.rmi;

import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

import rs.ac.bg.etf.kdp.sanja.viseelementniBafer.ABB;

public class ClientRABB<T> implements ABB<T> {
	
	private RABB<T> stub;

	@SuppressWarnings("unchecked")
	public ClientRABB(String serverHost, int serverPort) {
		
		// SecurityManager nam je potreban da bismo mogli u java.policy fajlu da sklonimo sve sigurnosne zastite kako bismo mogli
		// da pokrenemo nasu distribuiranu aplikaciju.
		if (System.getSecurityManager() == null) {
			System.setSecurityManager(new SecurityManager());
		}
		
		try {
			// U objektu registry se nalaze udaljene reference servera, tj. objekti koji su na serveru, ali im mogu pristupiti
			// klijenti udaljeno, sa svojih racunara upravo tako sto ih dohvate iz tog registra.
			Registry registry = LocateRegistry.getRegistry(serverHost, serverPort);
			stub = (RABB<T>) registry.lookup("/buffer");
			
		} catch (RemoteException | NotBoundException e) {
			e.printStackTrace();
		}
		
	}

	// Metode put i get se zavrsavaju kada se zavrse metode stub.put i stub.get na serveru.
	// Razmena poruka preko soketa se opet dogadja izmedju klijenta i servera, medjutim to sve radi RMI u pozadini sakriveno od nas - 
	// mi to sve iniciramo jednostavnim pozivima stub.put ili stub.get metoda udaljenog objekta stub koji je na serveru.
	
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
