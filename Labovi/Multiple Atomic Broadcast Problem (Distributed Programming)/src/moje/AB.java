package moje;

public interface AB {
	/**
	 * Inicijalizacija.
	 * 
	 * @param host
	 * @param port
	 * @return true ako je uspesno, inace false
	 */
	public boolean init(String host, int port);

	/**
	 * Regularan zavrsetak programa.
	 * 
	 * @return true ako je uspesno, inace false
	 */
	public boolean close();

	public void putGoods(String name, Goods goods);

	public Goods getGoods(String name);

}
