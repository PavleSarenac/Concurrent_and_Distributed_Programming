package rs.ac.bg.etf.kdp.sanja.viseelementniBafer;

// Ovde nikako ne smemo da upisujemo atribut koji predstavlja serijski broj klase koja implementira interfejs Serializable
// jer bi nam to napravilo problem pri slanju takve poruke u distribuiranom programiranju - ako se na nasem racunaru izgenerise
// jedan serijski broj, a na racunaru kom saljemo poruku drugi, onda se takva poruka nece uspesno primiti jer ce racunar primalac
// uporediti primljenu poruku sa poslatom sa racunara posiljaoca i uvideti da te klase nisu iste zbog tog atributa, pa ce odbaciti
// poruku.
@SuppressWarnings("serial")
public class TextMessage implements Message<String> {

	private long id;
	private String body;

	@Override
	public void setId(long id) {
		this.id = id;
	}

	@Override
	public long getId() {
		return id;
	}

	@Override
	public void setBody(String body) {
		this.body = body;
	}

	@Override
	public String getBody() {
		return body;
	}

}
