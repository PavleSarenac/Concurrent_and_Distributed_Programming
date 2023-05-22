package nijeMoje;

import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

public class RMIAB implements AB {

	private IAtomicBroadcast ab;
	private int id = -1;

	@Override
	public boolean init(String host, int port) {
		// May require the -Djava.security.manager=allow JVM argument.
		if (System.getSecurityManager() == null) {
			System.setSecurityManager(new SecurityManager());
		}
		try {
			Registry registry = LocateRegistry.getRegistry(host, port);
			ab = (IAtomicBroadcast) registry.lookup("/ab");
			return true;
		} catch (RemoteException | NotBoundException e) {
			e.printStackTrace();
			return false;
		}
	}

	@Override
	public boolean close() {
		// Nothing to clean up.
		return true;
	}

	@Override
	public void putGoods(String name, Goods goods) {
		GoodsImpl putGoods;
		if (goods instanceof GoodsImpl) {
			putGoods = (GoodsImpl) goods;
		} else {
			putGoods = new GoodsImpl(goods);
		}
		try {
			ab.putGoods(name, putGoods);
		} catch (RemoteException e) {
			e.printStackTrace();
		}
	}

	@Override
	public Goods getGoods(String name) {
		if (id == -1) {
			try {
				id = ab.getId();
				if (id == -1) {
					throw new RuntimeException("Too many connected consumers.");
				}
			} catch (RemoteException e) {
				e.printStackTrace();
				return null;
			}
		}
		try {
			return ab.getGoods(name, id);
		} catch (RemoteException e) {
			e.printStackTrace();
			return null;
		}
	}

}
