package rs.ac.bg.etf.kdp.sanja.viseelementniBafer;

import java.io.Serializable;

// Postansko sanduce.
// Ovaj interfejs prosiruje interfejs Serializable da bismo mogli da saljemo poruke u distribuiranom programiranju.
public interface Message<T> extends Serializable {

	public void setId(long id);

	public long getId();

	public void setBody(T body);

	public T getBody();

}
