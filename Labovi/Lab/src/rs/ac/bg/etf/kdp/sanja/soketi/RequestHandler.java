package rs.ac.bg.etf.kdp.sanja.soketi;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;

import rs.ac.bg.etf.kdp.sanja.viseelementniBafer.AtomicBroadcastBuffer;
import rs.ac.bg.etf.kdp.sanja.viseelementniBafer.Message;

public class RequestHandler extends Thread {

	private Socket clientSocket;
	private AtomicBroadcastBuffer<Message<String>> buffer;

	public RequestHandler(Socket clientSocket, AtomicBroadcastBuffer<Message<String>> buffer) {
		this.clientSocket = clientSocket;
		this.buffer = buffer;
	}

	@SuppressWarnings("unchecked")
	@Override
	public void run() {
		super.run();
		// Ovde unutar zagrada nakon try pravimo i novu referencu na nas clientSocket da
		// bismo obezbedili da se clientSocket
		// automatski zatvori nakon zavrsetka metode run.
		try (Socket client = this.clientSocket;
				ObjectOutputStream outputStream = new ObjectOutputStream(client.getOutputStream());
				ObjectInputStream inputStream = new ObjectInputStream(client.getInputStream());) {
			
			// Ovo je broj porta klijenta kojeg mu operativni sistem implicitno dodeljuje za komunikaciju sa serverom.
			System.out.println("Client port: " + client.getPort());
			// Ovo je broj porta na kom server osluskuje zahteve i na koji se klijent povezao.
			System.out.println("Client local port: " + client.getLocalPort());
			// Ovo je IP adresa klijenta.
			System.out.println("Client address: " + client.getInetAddress());
			// Ovo je IP adresa servera.
			System.out.println("Client local address: " + client.getLocalAddress());

			// Vazno je napomenuti da sve sto se salje izmedju procesa u distribuiranoj aplikaciji se salje po vrednosti,
			// sto je i logicno - nema smisla slati po referenci kada su u pitanju odvojeni adresni prostori za svaki proces.
			// Zato treba paziti da nikada ne poredimo primljene objekte po referenci - npr. kada bismo uradili operation == "put"
			// to ne bi bilo true iako je primljeni objekat zaista bio "put", jer == poredi po referenci. Ovo bi trebalo zato
			// da se radi ili kao "put".equals(operation) ili kao sto smo uradili pomocu switch-a, jer je to onda poredjenje po
			// vrednosti.
			String operation = (String) inputStream.readObject();  // Klijent nam javlja koju operaciju zeli da izvrsimo.
			switch (operation) {
				case "put":
					outputStream.writeObject("Operation received.");  // Javljamo klijentu da je operacija uspesno primljena.
					Message<String> putMessage = (Message<String>) inputStream.readObject();  // Klijent nam salje objekat potreban za izvrsenje trazene operacije.
					buffer.put(putMessage);
					outputStream.writeObject("Operation successfully executed.");  // Javljamo klijentu da je operacija uspesno izvrsena.
					break;
				case "get":
					outputStream.writeObject("Operation received.");  // Javljamo klijentu da je operacija uspesno primljena.
					int threadId = inputStream.readInt();
					Message<String> getMessage = buffer.get(threadId);
					outputStream.writeObject(getMessage);  // Saljemo klijentu trazeni objekat iz bafera.
					inputStream.readObject();  // Server ceka da klijent javi da je primio trazeni objekat iz bafera.
					break;
				default:
					String errorMessage = String.format("*** Operation %s is not supported.", operation);
					System.err.println(errorMessage);
					outputStream.writeObject(errorMessage);
					break;
			}
		} catch (IOException | ClassNotFoundException e) {
			e.printStackTrace();
		}
	}

}
