package rs.ac.bg.etf.kdp.sanja.gui;

import rs.ac.bg.etf.kdp.sanja.viseelementniBafer.AtomicBroadcastBuffer;
import rs.ac.bg.etf.kdp.sanja.viseelementniBafer.Message;
import rs.ac.bg.etf.kdp.sanja.viseelementniBafer.SemaphoreAtomicBroadcastBuffer;

public class GuiTest {

	public static void main(String[] args) {
		int m = 2;  // broj proizvodjaca (producer-a)
		int n = 3;  // broj potrosaca (consumer-a)
		int bufferCapacity = 5;
		
		AtomicBroadcastBuffer<Message<String>> buffer = new SemaphoreAtomicBroadcastBuffer<>(bufferCapacity, n);
		
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
