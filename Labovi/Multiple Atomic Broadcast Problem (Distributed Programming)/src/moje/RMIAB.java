package moje;

import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

public class RMIAB implements AB {
	
	private IAtomicBroadcast buffer;
	private int threadId = -1;

	@Override
	public boolean init(String host, int port) {
		if (System.getSecurityManager() == null) {
			System.setSecurityManager(new SecurityManager());
		}
		try {
			Registry registry = LocateRegistry.getRegistry(port);
			buffer = (IAtomicBroadcast) registry.lookup("/buffer");
			return true;
		} catch (RemoteException | NotBoundException e) {
			e.printStackTrace();
			return false;
		}
		
	}

	@Override
	public boolean close() {
		return true;
	}

	@Override
	public void putGoods(String name, Goods goods) {
		GoodsImpl checkedGoods;
		if (goods instanceof GoodsImpl) {
			checkedGoods = (GoodsImpl) goods;
		} else {
			checkedGoods = new GoodsImpl(goods);
		}
		try {
			buffer.putGoods(name, checkedGoods);
		} catch (RemoteException e) {
			e.printStackTrace();
		}
	}

	@Override
	public Goods getGoods(String name) {
		GoodsImpl goods = null;
		
		if (threadId == -1) {
			try {
				threadId = buffer.generateThreadId();
			} catch (RemoteException e) {
				e.printStackTrace();
			}
			if (threadId == -1) {
				System.err.println("RMIAB.java: Number of possible consumers is exceeded.");
				return goods;
			} 
		}
		
		try {
			goods = buffer.getGoods(name, threadId);
		} catch (RemoteException e) {
			e.printStackTrace();
		}
		
		return goods;
	}

}
