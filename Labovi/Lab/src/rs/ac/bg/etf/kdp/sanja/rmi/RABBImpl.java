package rs.ac.bg.etf.kdp.sanja.rmi;

import java.rmi.RemoteException;

import rs.ac.bg.etf.kdp.sanja.viseelementniBafer.ABB;
import rs.ac.bg.etf.kdp.sanja.viseelementniBafer.SemaphoreABB;

// Objektu ove klase (RABBImpl<T>) ce biti moguc udaljen pristup zato sto ova klasa implementira interfejs RABB<T> koji prosiruje 
// interfejs Remote. Takodje, moci ce udaljeno da se pozivaju metode put i get objekta ove klase (RABBImpl<T>) zato sto one bacaju
// izuzetak RemoteException.

// Ova klasa ce interno koristiti bafer realizovan pomocu semafora (objekat klase SemaphoreABB<T>), ali to klijenta niti interesuje, niti
// ce on to znati. Takodje, klijent kada bude pozivao put i get metode objekta klase ClientRABB<T>, on ce to raditi najnormalnije kao
// sto poziva i bilo koju drugu metodu nekog objekta koji je na njegovom racunaru - uopste nece videti da se u pozadini zapravo dovhata
// udaljeni objekat klase RABBImpl<T> sa nekog potpuno drugog racunara i pozivaju njegove metode. Za klijenta je ceo RMI transparentan,
// sto je i poenta RMI.

public class RABBImpl<T> implements RABB<T> {
	
	private ABB<T> buffer;
	
	public RABBImpl(int capacity, int numberOfConsumers) {
		buffer = new SemaphoreABB<>(capacity, numberOfConsumers);
	}

	@Override
	public void put(T item) throws RemoteException {
		buffer.put(item);
	}

	@Override
	public T get(int id) throws RemoteException {
		return buffer.get(id);
	}

}
