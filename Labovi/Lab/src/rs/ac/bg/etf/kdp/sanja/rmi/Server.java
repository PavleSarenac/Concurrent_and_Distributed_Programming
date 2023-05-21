package rs.ac.bg.etf.kdp.sanja.rmi;

import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;

import rs.ac.bg.etf.kdp.sanja.viseelementniBafer.Message;

public class Server {

	@SuppressWarnings("unchecked")
	public static void main(String[] args) {
		
		int bufferCapacity = 5;
		int numberOfConsumers = 3;
		
		// SecurityManager nam je potreban da bismo mogli u java.policy fajlu da sklonimo sve sigurnosne zastite kako bismo mogli
		// da pokrenemo nasu distribuiranu aplikaciju.
		if (System.getSecurityManager() == null) {
			System.setSecurityManager(new SecurityManager());
		}

		// Objektu klase RABBImpl<T> je moguc udaljeni pristup jer on implementira odgovarajuci interfejs RABB<T> - zato cemo ga koristiti.
		RABB<Message<String>> buffer = new RABBImpl<>(bufferCapacity, numberOfConsumers);
		
		try {
			// Pozivom metode UnicastRemoteObject.exportObject(obj, 0) pravimo od prosledjenog objekta obj udaljeni objekat kome ce onda
			// moci da se pristupa sa drugih racunara. Drugi parametar je broj porta preko kog ce moci da mu se pristupa.
			
			// Za broj porta stavljamo 0 da bi se on dinamicki dodelio i da mi ne bismo brinuli o tome (nece biti zaista broj porta 0, vec 
			// neki drugi koji dodeli operativni sistem). Najbolje je tako uraditi jer je za svaki udaljeni objekat potreban po jedan port
			// preko kog moze da mu se pristupi, pa da ne napravimo neku gresku kada ima vise udaljenih objekata - da budemo sigurni da svaki
			// ima svoj port.
			
			// Za svaki zahtev za dohvatanjem ovog objekta ce RMI sistem u pozadini iskoristiti jednu nit iz svog pool-a spremnih niti
			// u okviru koje ce se izvrsavati sve operacije koje klijent pozove nad nasim udaljenim objektom. Na taj nacin ce klijent
			// imati privid kao da se sve to izvrsava na njegovom racunaru, a zapravo sve sto on radi se izvrsava u okviru jedne niti
			// na potpuno drugom racunaru, tj. serveru.
			RABB<Message<String>> stub = (RABB<Message<String>>) UnicastRemoteObject.exportObject(buffer, 0);
			
			// Na ovaj nacin omogucavamo da klijenti mogu preko porta 4001 da pristupe registru udaljenih referenci - server osluskuje
			// zahteve za pristup udaljenim referencama na portu 4001 od sada.
			
			// Rekla je na vezbama da nekada ume da pravi probleme ovakav nacin kreiranja registra i da je najsigurnije napraviti ga u
			// komandnoj liniji jednostavnom komandom (rmiregistry 4001). U tom slucaju bismo dohvatili registar pozivom
			// Registry registry = LocateRegistry.getRegistry("localhost", 4001); i onda bismo dalje radili nad tim objektom bind/rebind.
			Registry registry = LocateRegistry.createRegistry(4001);
			
			// Na ovaj nacin dodajemo nas udaljeni objekat u registar udaljenih referenci i dajemo mu naziv "buffer" po kom ce klijenti
			// moci da ga pronadju u ovom registru i dohvate. Svaki naziv udaljene reference mora poceti sa "/".
			
			// Umesto rebind metode mogla je i bind metoda da se pozove. Razlika izmedju ove dve je to sto bind metoda baca gresku ukoliko
			// vec u registru postoji udaljena referenca nazvana "buffer", dok rebind nece baciti gresku vec ce postojecu udaljenu referencu
			// izbaciti iz registra i zameniti je novom.
			registry.rebind("/buffer", stub);
			
			System.out.println("RMI server started...");
		} catch (RemoteException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}