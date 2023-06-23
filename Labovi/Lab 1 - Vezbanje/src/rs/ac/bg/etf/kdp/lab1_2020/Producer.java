package rs.ac.bg.etf.kdp.lab1_2020;

import java.io.BufferedReader;
import java.io.FileReader;

public class Producer extends Thread {
	private final String fileName;
	private final BoundedBuffer<String> linesFromFile;
	
	public Producer(String fileName, BoundedBuffer<String> linesFromFile) {
		super("Producer");
		this.fileName = fileName;
		this.linesFromFile = linesFromFile;
	}

	@Override
	public void run() {
		try (BufferedReader reader = new BufferedReader(new FileReader(fileName));) {
			String line = reader.readLine();
			while ((line = reader.readLine()) != null) {
				linesFromFile.put(line);
			}
			linesFromFile.put(null);
		} catch (Exception e) {
			
		}
	}
	
}
