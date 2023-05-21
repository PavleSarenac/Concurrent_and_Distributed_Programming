package rs.ac.bg.etf.kdp.sanja.guiSwingWorker;

import rs.ac.bg.etf.kdp.sanja.rmi.ClientRABB;
import rs.ac.bg.etf.kdp.sanja.soketi.SocketABB;
import rs.ac.bg.etf.kdp.sanja.viseelementniBafer.ABB;
import rs.ac.bg.etf.kdp.sanja.viseelementniBafer.Message;
import rs.ac.bg.etf.kdp.sanja.viseelementniBafer.SemaphoreABB;

@SuppressWarnings("unused")
public class GuiTest {

	public static void main(String[] args) {
		int m = 2;  // broj proizvodjaca (producer-a)
		int n = 3;  // broj potrosaca (consumer-a)
		int bufferCapacity = 5;
		
		// Bafer za lab 1 (konkurentno programiranje)
		//ABB<Message<String>> buffer = new SemaphoreABB<>(bufferCapacity, n);
		
		// Bafer za lab 2 (distribuirano programiranje - soketi)
		//ABB<Message<String>> buffer = new SocketABB<>("localhost", 4001);
		
		// Bafer za lab2 (distribuirano programiranje - RMI)
		ABB<Message<String>> buffer = new ClientRABB<>("localhost", 4001);
		
		Put p[] = new Put[m];
		Get c[] = new Get[n];
		
		for (int i = 0; i < p.length; i++) {
			p[i] = new Put(buffer);
		}
		
		for (int i = 0; i < c.length; i++) {
			c[i] = new Get(buffer, i);
		}
	}
	
}
