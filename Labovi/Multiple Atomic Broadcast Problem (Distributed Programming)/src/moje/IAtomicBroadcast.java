package moje;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface IAtomicBroadcast extends Remote {

	public void putGoods(String name, GoodsImpl putGoods) throws RemoteException;
	
	public GoodsImpl getGoods(String name, int threadId) throws RemoteException;
	
	public int generateThreadId() throws RemoteException;
	
}
