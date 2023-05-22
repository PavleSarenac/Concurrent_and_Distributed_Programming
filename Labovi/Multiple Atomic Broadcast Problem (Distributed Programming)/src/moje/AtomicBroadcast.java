package moje;

import java.util.HashMap;

public class AtomicBroadcast {
	
	private int bufferCapacity;
	private int numOfConsumers;
	private HashMap<String, GoodsBuffer> buffers;
	
	private class GoodsBuffer {
		Goods buffer[];
		int writeIndex = 0;
		int size = 0;
		
		public void addToBuffer(Goods goods) {
			buffer[writeIndex] = goods;
			writeIndex = (writeIndex + 1) % bufferCapacity;
			size++;
		}
		
		public GoodsBuffer() {
			buffer = new Goods[bufferCapacity];
		}
	}

	public AtomicBroadcast(int bufferCapacity, int numOfConsumers) {
		this.bufferCapacity = bufferCapacity;
		this.numOfConsumers = numOfConsumers;
		buffers = new HashMap<>();
	}
	
	public synchronized void putGoods(String name, Goods goods) {
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
		
		goodsBuffer.addToBuffer(goods);
		notifyAll();
	}
	
	public synchronized Goods getGoods(String name) {
		return null;
	}
	
}
