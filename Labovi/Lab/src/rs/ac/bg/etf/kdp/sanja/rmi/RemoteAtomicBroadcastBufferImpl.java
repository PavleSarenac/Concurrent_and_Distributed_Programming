package rs.ac.bg.etf.kdp.sanja.rmi;

import java.rmi.RemoteException;

import rs.ac.bg.etf.kdp.sanja.viseelementniBafer.AtomicBroadcastBuffer;
import rs.ac.bg.etf.kdp.sanja.viseelementniBafer.SemaphoreAtomicBroadcastBuffer;

public class RemoteAtomicBroadcastBufferImpl<T> implements RemoteAtomicBroadcastBuffer<T> {
	
	private AtomicBroadcastBuffer<T> buffer;
	
	public RemoteAtomicBroadcastBufferImpl(int capacity, int numberOfConsumers) {
		buffer = new SemaphoreAtomicBroadcastBuffer<>(capacity, numberOfConsumers);
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
