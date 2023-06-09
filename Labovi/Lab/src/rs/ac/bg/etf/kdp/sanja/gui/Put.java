package rs.ac.bg.etf.kdp.sanja.gui;

import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JTextArea;
import javax.swing.SwingUtilities;

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
				// Odmah na pocetku dohvatamo tekst iz jta da bismo bili sigurni da cemo pri
				// kliku na dugme zaista i poslati u bafer ono sto je u tom trenutku u jta
				// (necemo dole negde da to radimo jer bi onda mogao tekst u jta da se promeni u
				// medjuvremenu od klika na dugme do njegovog dohvatanja).
				String text = jta.getText();
				jta.setEditable(false);
				jb.setEnabled(false);

				// Moramo da pravimo novu nit za obradjivanje svakog klika jer gui ima samo
				// jednu nit i ako bismo nju zablokirali (sto upravo i radi buffer.get(id))
				// program bi nam se zaustavio - zato je potrebno koriscenje pozadinskih niti
				// kao sto je ova da bismo odradili blokirajucu operaciju kao sto je dohvatanje
				// iz bafera.
				Thread t = new Thread() {

					@Override
					public void run() {
						// Ovo ne radi gui nit, vec nasa nova nit koju smo napravili.
						Message<String> msg = new TextMessage();
						msg.setBody(text);
						buffer.put(msg);

						// Gui-u ne treba pristupati iz niti koja nije gui nit, pa na ovaj nacin
						// iz nase nove niti govorimo da gui nit treba da izvrsi ono sto je unutar ove
						// run metode naredni put kada dodje na red za izvrsavanje gui nit.
						SwingUtilities.invokeLater(new Runnable() {

							@Override
							public void run() {
								jta.setText("");
								jta.setEditable(true);
								jb.setEnabled(true);
							}

						});
					}

				};

				t.start();
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
