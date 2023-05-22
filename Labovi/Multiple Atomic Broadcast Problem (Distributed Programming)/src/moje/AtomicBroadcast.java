package moje;

import java.rmi.RemoteException;
import java.util.HashMap;

public class AtomicBroadcast implements IAtomicBroadcast {
	
	private int bufferCapacity;
	private int numOfConsumers;
	private HashMap<String, GoodsBuffer> buffers;
	private int lastConsumerId = 0;
	
	private class GoodsBuffer {
		GoodsImpl buffer[];
		int writeIndex = 0;
		int readIndex[];
		int consumerCount[];
		int size = 0;
		
		public void addToBuffer(GoodsImpl goods) {
			buffer[writeIndex] = goods;
			writeIndex = (writeIndex + 1) % bufferCapacity;
			size++;
		}
		
		public GoodsImpl getFromBuffer(int threadId) {
			GoodsImpl goods = buffer[readIndex[threadId]];
			if (--consumerCount[readIndex[threadId]] == 0) {
				size--;
			}
			readIndex[threadId] = (readIndex[threadId] + 1) % bufferCapacity;
			return goods;
		}
		
		public GoodsBuffer() {
			buffer = new GoodsImpl[bufferCapacity];
			readIndex = new int[numOfConsumers];
			consumerCount = new int[bufferCapacity];
		}
	}

	public AtomicBroadcast(int bufferCapacity, int numOfConsumers) {
		this.bufferCapacity = bufferCapacity;
		this.numOfConsumers = numOfConsumers;
		buffers = new HashMap<>();
	}
	
	public synchronized void putGoods(String name, GoodsImpl putGoods) throws RemoteException {
		GoodsBuffer goodsBuffer;
		if (buffers.get(name) == null) {
			goodsBuffer = new GoodsBuffer();
			buffers.put(name, goodsBuffer);
		} else {
			goodsBuffer = buffers.get(name);
		}
		
		while (goodsBuffer.size == bufferCapacity) {
			try {
				wait();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		
		goodsBuffer.addToBuffer(putGoods);
		notifyAll();
	}
	
	public synchronized GoodsImpl getGoods(String name, int threadId) throws RemoteException {
		GoodsBuffer goodsBuffer = buffers.get(name);
		if (goodsBuffer == null) {
			goodsBuffer = new GoodsBuffer();
			buffers.put(name, goodsBuffer);
		}
		while (goodsBuffer.size == 0) {
			try {
				wait();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		GoodsImpl goods = goodsBuffer.getFromBuffer(threadId);
		notifyAll();
		return goods;
	}

	public synchronized int generateThreadId() throws RemoteException {
		if (lastConsumerId == numOfConsumers) {
			return -1;
		}
		return lastConsumerId++;
	}
	
}
