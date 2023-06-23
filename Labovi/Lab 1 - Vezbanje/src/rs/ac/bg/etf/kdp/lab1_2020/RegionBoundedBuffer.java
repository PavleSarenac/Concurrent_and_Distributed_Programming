package rs.ac.bg.etf.kdp.lab1_2020;

import java.util.ArrayList;
import java.util.List;

public class RegionBoundedBuffer<T> implements BoundedBuffer<T> {
	
	private final int capacity;
	private final List<T> buffer;
	
	public RegionBoundedBuffer(int capacity) {
		this.capacity = capacity;
		buffer = new ArrayList<T>(capacity);
	}

	@Override
	public void put(T data) {
		synchronized (buffer) {
			while (buffer.size() == capacity) {
				try {
					buffer.wait();
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
			buffer.add(data);
			buffer.notifyAll();
		}
	}

	@Override
	public T get() {
		synchronized (buffer) {
			while (buffer.size() == 0) {
				try {
					buffer.wait();
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
			T data = buffer.remove(0);
			buffer.notifyAll();
			return data;
		}
	}

}
