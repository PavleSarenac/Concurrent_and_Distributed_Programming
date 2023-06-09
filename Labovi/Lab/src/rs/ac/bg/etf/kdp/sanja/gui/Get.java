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

				// Moramo da pravimo novu nit za obradjivanje svakog klika jer gui ima samo
				// jednu nit i ako bismo nju zablokirali (sto upravo i radi buffer.get(id))
				// program bi nam se zaustavio - zato je potrebno koriscenje pozadinskih niti
				// kao sto je ova da bismo odradili blokirajucu operaciju kao sto je dohvatanje
				// iz bafera.
				Thread t = new Thread() {

					@Override
					public void run() {
						// Ovo ne radi gui nit, vec nasa nova nit koju smo napravili.
						Message<String> item = buffer.get(id);

						// Gui-u ne treba pristupati iz niti koja nije gui nit, pa na ovaj nacin
						// iz nase nove niti govorimo da gui nit treba da izvrsi ono sto je unutar ove
						// run metode naredni put kada dodje na red za izvrsavanje gui nit.
						SwingUtilities.invokeLater(new Runnable() {

							@Override
							public void run() {
								jta.setText(item.getBody());
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
