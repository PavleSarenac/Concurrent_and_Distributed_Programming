package rs.ac.bg.etf.kdp.sanja.soketi;

import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;

import rs.ac.bg.etf.kdp.sanja.viseelementniBafer.AtomicBroadcastBuffer;

public class SocketAtomicBroadcastBuffer<T> implements AtomicBroadcastBuffer<T> {

	private String serverHost;
	private int serverPort;

	public SocketAtomicBroadcastBuffer(String serverHost, int serverPort) {
		super();
		this.serverHost = serverHost;
		this.serverPort = serverPort;
	}
	
	// Svi objekti koji se salju kao poruka moraju da implementiraju interfejs Serializable.

	// Za slanje poruka, najbolje je da koristimo writeObject jer ta metoda garantuje da poruka nece biti baferisana i odlozena
	// da se posalje kasnije, vec da ce biti poslata odmah. Kada bismo koristili neku drugu metodu koja ce potencijalno da
	// baferuje objekte da bi ih kasnije poslala, moguce je da se dogodi da se nakon baferovanja zatvori konekcija sa serverom
	// i onda da se objekat nikada ni ne posalje.
	
	// Kada koristimo writeObject metodu, ne moramo da pozivamo metodu flush() koja zasigurno salje
	// objekat, jer ce on svakako sigurno biti poslat nakon izvrsenja writeObject metode.
	
	// Sve ove objekte kreiramo unutar zagrada nakon try jer na taj nacin ne moramo mi da brinemo o njihovom zatvaranju,
	// tj. dealokaciji, vec ce to automatski da se odradi u pozadini nakon zavrsetka ove metode.
	
	// Takodje, vrlo je vazno da se prvo kreira objekat outputStream, pa tek onda objekat inputStream jer
	// objekat inputStream uvek gleda da se nakaci na neki drugi outputStream (ne ovaj nas ovde, vec od servera npr.)
	// a ako nema na koji, onda se nit blokira - tako potencijalno moze da nastane mrtva blokada. Da bismo bili sigurni
	// da ovo ne moze da se desi, najbolja praksa je da uvek kreiramo prvo outputStream, pa tek onda inputStream.
	
	@Override
	public void put(T item) {
		try (Socket clientSocket = new Socket(serverHost, serverPort);
				ObjectOutputStream outputStream = new ObjectOutputStream(clientSocket.getOutputStream());
				ObjectInputStream inputStream = new ObjectInputStream(clientSocket.getInputStream());) {

			// Sinhrona komunikacija izmedju klijenta i servera.
			outputStream.writeObject("put");  // Klijent prvo mora da javi serveru koju operaciju on treba da izvrsi.
			String serverResponse = (String) inputStream.readObject();  // Klijent ceka odgovor od servera da vidi da li je uspesno primio poruku.
			if ("Operation received.".equals(serverResponse)) {  // Server je javio da je uspesno primio poruku.
				outputStream.writeObject(item);  // Klijent salje objekat serveru.
				inputStream.readObject();  // Klijent ceka potvrdu od servera da je uspesno izvrsena trazena operacija.
			} else {
				System.err.println(serverResponse);  // Server je javio da nije uspesno primio poruku.
			}
			
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	@SuppressWarnings("unchecked")
	@Override
	public T get(int id) {
		try (Socket clientSocket = new Socket(serverHost, serverPort);
				ObjectOutputStream outputStream = new ObjectOutputStream(clientSocket.getOutputStream());
				ObjectInputStream inputStream = new ObjectInputStream(clientSocket.getInputStream());) {
			
			// Sinhrona komunikacija izmedju klijenta i servera.
			outputStream.writeObject("get");  // Klijent prvo mora da javi serveru koju operaciju on treba da izvrsi.
			String serverResponse = (String) inputStream.readObject();  // Klijent ceka odgovor od servera da vidi da li je uspesno primio poruku.
			if ("Operation received.".equals(serverResponse)) {  // Server je javio da je uspesno primio poruku.
				outputStream.writeInt(id);  // Saljemo serveru id niti koja je pozvala get jer je to potreban argument za dohvatanje iz bafera.
				outputStream.flush();  // Ovo radimo da bi se prost tip id sigurno poslao serveru - to nam nije garantovano metodom writeInt kao sto jeste kod metode writeObject.
				T item = (T) inputStream.readObject();  // Klijent ceka da primi objekat od servera.
				outputStream.writeObject("Client has received server's object.");  // Klijent javlja serveru da je uspesno primio objekat.
				return item;
			} else {
				System.err.println(serverResponse);  // Server je javio da nije uspesno primio poruku.
			}
			
			
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}

}
