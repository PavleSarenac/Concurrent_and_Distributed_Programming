package rs.ac.bg.etf.kdp.sanja.guiSwingWorker;

import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.concurrent.ExecutionException;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JTextArea;
import javax.swing.SwingWorker;

import rs.ac.bg.etf.kdp.sanja.viseelementniBafer.ABB;
import rs.ac.bg.etf.kdp.sanja.viseelementniBafer.Message;

@SuppressWarnings("serial")
public class Get extends JFrame {

	JButton jb;
	JTextArea jta;

	public Get(ABB<Message<String>> buffer, int id) {
		super("Get");
		jb = new JButton("Get");
		jta = new JTextArea();

		jta.setEditable(false);

		jb.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				// Ovo odradjuje gui nit.
				jb.setEnabled(false);

				// Klasa SwingWorker nam sluzi da nam omoguci koriscenje njegovog pool-a niti
				// koje ce da rade posao
				// u pozadini dok se izvrsava gui nit regularno.
				// Mana SwingWorker-a je sto ima samo 10 niti na raspolaganju.
				SwingWorker<Message<String>, Void> sw = new SwingWorker<Message<String>, Void>() {

					// Ovu metodu ce izvrsavati pozadinske niti klase SwingWorker.
					@Override
					protected Message<String> doInBackground() throws Exception {
						Message<String> item = buffer.get(id);
						return item;
					}

					// Ovu metodu ce SwingWorker dati GUI niti da izvrsi.
					@Override
					protected void done() {
						super.done();
						try {
							// Pozivom get() se dobija povratna vrednost metode doInBackground.
							jta.setText(get().getBody());
						} catch (InterruptedException | ExecutionException e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
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
