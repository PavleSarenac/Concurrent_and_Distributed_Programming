package rs.ac.bg.etf.kdp.lab1_2020;

public interface BoundedBuffer<T> {
	
	public void put(T data);
	public T get();

}
