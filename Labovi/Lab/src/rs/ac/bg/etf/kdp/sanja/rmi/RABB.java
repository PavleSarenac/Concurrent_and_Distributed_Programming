package rs.ac.bg.etf.kdp.sanja.rmi;

import java.rmi.Remote;
import java.rmi.RemoteException;

// Za svaki objekat kojem zelimo da pristupamo udaljeno pomocu RMI, moramo da 
// napravimo po jedan ovakav interfejs preko kog ce im se pristupati.

// Ukoliko metodi zelimo da udaljeno pristupamo, ona mora da baca 
// RemoteException, inace nece raditi udaljeni pristup.

// Ukoliko nekoj metodi unutar interfejsa koji prosiruje interfejs Remote zelimo da 
// onemogucimo udaljeni pristup, ona samo ne treba da baca RemoteException.

// Svaki objekat koji se salje izmedju procesa kod RMI mora da implementira interfejs Serializable. Ovo je logicno zato sto je to bilo
// obavezno i kod soketa, a RMI svakako koristi sokete, samo implicitno jer nismo mi odgovorni za rukovanje tim soketima nego se to
// automatski dogadja u pozadini.

public interface RABB<T> extends Remote {
	
	public void put(T item) throws RemoteException;

	public T get(int id) throws RemoteException;
	
}
