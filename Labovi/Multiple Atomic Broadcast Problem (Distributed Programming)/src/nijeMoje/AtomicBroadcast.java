package nijeMoje;

import java.rmi.RemoteException;
import java.util.HashMap;
import java.util.Map;

public class AtomicBroadcast implements IAtomicBroadcast {
	private final int capacity;
	private final int consumers;

	private class GoodsResource {
		GoodsImpl[] data;
		int[] consumerIndex;
		int[] consumerCount;
		int producerIndex = 0;
		int size = 0;

		GoodsResource() {
			data = new GoodsImpl[capacity];
			consumerIndex = new int[consumers];
			consumerCount = new int[capacity];
		}
	}

	private final Map<String, GoodsResource> resources = new HashMap<>();
	private int lastConsumerId = 0;

	public AtomicBroadcast(int capacity, int consumers) {
		this.capacity = capacity;
		this.consumers = consumers;
	}

	public synchronized void putGoods(String name, GoodsImpl goods) throws RemoteException {
		GoodsResource res = resources.get(name);
		if (res == null) {
			res = new GoodsResource();
			resources.put(name, res);
		}
		while (res.size == capacity) {
			try {
				wait();
			} catch (InterruptedException e) {
			}
		}
		res.data[res.producerIndex] = goods;
		res.consumerCount[res.producerIndex] = consumers;
		res.producerIndex = (res.producerIndex + 1) % capacity;
		++res.size;
		notifyAll();
	}

	public synchronized GoodsImpl getGoods(String name, int id) throws RemoteException {
		GoodsResource res = resources.get(name);
		if (res == null) {
			res = new GoodsResource();
			resources.put(name, res);
		}
		while (res.size == 0) {
			try {
				wait();
			} catch (InterruptedException e) {
			}
		}
		GoodsImpl ret = res.data[res.consumerIndex[id]];
		if (--res.consumerCount[res.consumerIndex[id]] == 0) {
			--res.size;
		}
		res.consumerIndex[id] = (res.consumerIndex[id] + 1) % capacity;
		notifyAll();
		return ret;
	}

	public synchronized int getId() throws RemoteException {
		if (lastConsumerId == consumers) {
			return -1;
		}
		return lastConsumerId++;
	}

}
