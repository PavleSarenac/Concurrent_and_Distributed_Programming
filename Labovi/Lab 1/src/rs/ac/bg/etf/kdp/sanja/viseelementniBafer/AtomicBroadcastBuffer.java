package rs.ac.bg.etf.kdp.sanja.viseelementniBafer;

public interface AtomicBroadcastBuffer<T> {

	public void put(T item);
	
	public T get(int id);
	
}
