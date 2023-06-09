package rs.ac.bg.etf.kdp.sanja.soketi;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

import rs.ac.bg.etf.kdp.sanja.viseelementniBafer.ABB;
import rs.ac.bg.etf.kdp.sanja.viseelementniBafer.Message;
import rs.ac.bg.etf.kdp.sanja.viseelementniBafer.SemaphoreABB;

public class Server {

	public static void main(String[] args) {

		int serverPort = Integer.parseInt(args[0]);
		int bufferCapacity = 5;
		int numberOfPossibleClients = 3;
		ABB<Message<String>> buffer = new SemaphoreABB<>(bufferCapacity, numberOfPossibleClients);

		try (ServerSocket serverSocket = new ServerSocket(serverPort)) {
			System.out.println("Server started...");

			while (true) {
				Socket clientSocket = serverSocket.accept();
				// Hocemo da umesto glavne serverske niti, zahtev od klijenta obradjuje posebna
				// nova nit koja je zapravo objekat klase RequestHandler.
				new RequestHandler(clientSocket, buffer).start();
			}
		} catch (IOException e) {
			e.printStackTrace();
		}

	}

}
