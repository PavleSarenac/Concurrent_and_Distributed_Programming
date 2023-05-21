package rs.ac.bg.etf.kdp.sanja.rmi;

import java.rmi.Remote;
import java.rmi.RemoteException;

// Ukoliko metodi zelimo da udaljeno pristupamo, ona mora da baca RemoteException, inace nece raditi udaljeni pristup.
// Ukoliko nekoj metodi unutar interfejsa koji prosiruje interfejs Remote zelimo da onemogucimo udaljeni pristup, ona samo ne treba da baca
// RemoteException.

public interface RemoteAtomicBroadcastBuffer<T> extends Remote {
	
	public void put(T item) throws RemoteException;

	public T get(int id) throws RemoteException;
	
}
