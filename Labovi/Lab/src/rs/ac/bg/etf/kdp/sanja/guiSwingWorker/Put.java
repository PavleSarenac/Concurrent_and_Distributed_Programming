package rs.ac.bg.etf.kdp.sanja.guiSwingWorker;

import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JTextArea;
import javax.swing.SwingWorker;

import rs.ac.bg.etf.kdp.sanja.viseelementniBafer.ABB;
import rs.ac.bg.etf.kdp.sanja.viseelementniBafer.Message;
import rs.ac.bg.etf.kdp.sanja.viseelementniBafer.TextMessage;

@SuppressWarnings("serial")
public class Put extends JFrame {

	JButton jb;
	JTextArea jta;

	public Put(ABB<Message<String>> buffer) {
		super("Put");
		jb = new JButton("Put");
		jta = new JTextArea();

		jb.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				// Ovo odradjuje gui nit.
				String text = jta.getText();
				jta.setEditable(false);
				jb.setEnabled(false);

				// Klasa SwingWorker nam sluzi da nam omoguci koriscenje njegovog pool-a niti
				// koje ce da rade posao
				// u pozadini dok se izvrsava gui nit regularno.
				// Mana SwingWorker-a je sto ima samo 10 niti na raspolaganju.
				SwingWorker<Void, Void> sw = new SwingWorker<Void, Void>() {

					// Ovu metodu ce izvrsavati pozadinske niti klase SwingWorker.
					@Override
					protected Void doInBackground() throws Exception {
						Message<String> msg = new TextMessage();
						msg.setBody(text);
						buffer.put(msg);
						return null;
					}

					// Ovu metodu ce SwingWorker dati GUI niti da izvrsi.
					@Override
					protected void done() {
						super.done();
						jta.setText("");
						jta.setEditable(true);
						jb.setEnabled(true);
					}

				};

				sw.execute();

			}
		});

		this.setVisible(true);
		this.setDefaultCloseOperation(EXIT_ON_CLOSE);
		this.setLayout(new GridLayout(2, 1));
		this.setBounds(600, 200, 300, 300);
		this.add(jb);
		this.add(jta);
	}

}
