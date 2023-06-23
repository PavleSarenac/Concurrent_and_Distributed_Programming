package rs.ac.bg.etf.kdp.lab1_2020;

public interface Barrier {
	
	public void arrived();
	public boolean await(long timeout);
	
}
