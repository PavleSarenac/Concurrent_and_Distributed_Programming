package rs.ac.bg.etf.kdp.lab1_2020;

public class Film {

	public final String tconst;
	public final String[] directors;
	public final String[] writers;
	
	public Film(String line) {
		String[] splitLine = line.split("\t");
		tconst = splitLine[0];
		directors = splitLine[1].equals("\\N") ? null : splitLine[1].split(",");
		writers = splitLine[2].equals("\\N") ? null : splitLine[2].split(",");
	}
	
}
