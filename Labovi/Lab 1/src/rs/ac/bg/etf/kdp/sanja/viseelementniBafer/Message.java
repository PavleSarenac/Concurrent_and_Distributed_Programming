package rs.ac.bg.etf.kdp.sanja.viseelementniBafer;

// Postansko sanduce.
public interface Message<T> {

	public void setId(long id);

	public long getId();

	public void setBody(T body);

	public T getBody();

}
