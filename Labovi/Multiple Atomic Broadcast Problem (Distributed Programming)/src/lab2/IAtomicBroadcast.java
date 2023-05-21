package lab2;

import java.rmi.Remote;
import java.rmi.RemoteException;

// Required for the AtomicBroadcast object to be transmitted over RMI.
public interface IAtomicBroadcast extends Remote {
	public void putGoods(String name, GoodsImpl goods) throws RemoteException;

	public GoodsImpl getGoods(String name, int id) throws RemoteException;

	public int getId() throws RemoteException;
}
