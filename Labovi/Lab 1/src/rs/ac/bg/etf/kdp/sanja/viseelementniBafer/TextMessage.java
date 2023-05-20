package rs.ac.bg.etf.kdp.sanja.viseelementniBafer;

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
