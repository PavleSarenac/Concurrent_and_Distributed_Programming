package nijeMoje;

public interface Goods {
	/**
	 * Dohvata ime datoteke.
	 * 
	 * @return ime datoteke; null ako ne postoji
	 */
	public String getName();

	/**
	 * Postavlja ime datoteke.
	 * 
	 * @param name ime datoteke
	 */
	public void setName(String name);

	/**
	 * Dohvata telo objekta.
	 * 
	 * @return telo objekta; null ako ne postoji
	 */
	public String[] getBody();

	/**
	 * Postavlja telo objekta.
	 * 
	 * @param body telo objekta
	 */
	public void setBody(String[] body);

	/**
	 * Cita narednu liniju iz objekta.
	 * 
	 * @return prva naredna linija; null ako ne postoji
	 */
	public String readLine();

	/**
	 * Dodaje liniju kao poslednju u objektu.
	 * 
	 * @param body dodaje liniju na kraj
	 */
	public void printLine(String body);

	/**
	 * Dohvata broj linija koje se nalaze u objektu.
	 * 
	 * @return broj linija; -1 ako ne postoji
	 */
	public int getNumLines();

	/**
	 * Cuva telo (niz stringova) u tekstualnu datoteku.
	 * 
	 * @param name ime datoteke
	 */
	public void save(String name);

	/**
	 * Ucitava telo iz tekstualne datoteke.
	 * 
	 * @param name ime datoteke
	 */
	public void load(String name);
}
