package rs.ac.bg.etf.kdp.lab1_2020;

import java.util.List;
import java.util.TreeSet;
import java.util.concurrent.ConcurrentMap;

public class Printer extends Thread {
	
	private final static int NUMBER_OF_MILISECONDS_IN_ONE_SECOND = 1000;

	private final long printingFrequency;
	private final ConcurrentMap<Integer, Integer> progress;
	private final Barrier printerBarrier;
	private final List<Film> globalFilmsWithMaxDirectors;
	
	public Printer(long printingFrequency, ConcurrentMap<Integer, Integer> progress, Barrier printerBarrier,
				   List<Film> globalFilmsWithMaxDirectors) {
		super("Printer");
		this.printingFrequency = printingFrequency;
		this.progress = progress;
		this.printerBarrier = printerBarrier;
		this.globalFilmsWithMaxDirectors = globalFilmsWithMaxDirectors;
	}
	
	@Override
	public void run() {
		while (true) {
			if (printerBarrier.await(printingFrequency * NUMBER_OF_MILISECONDS_IN_ONE_SECOND)) {
				break;
			}
			for (int consumerId : progress.keySet()) {
				System.out.println("Consumer " + consumerId + ": " + progress.get(consumerId));
			}
		}
		System.out.println("Films with maximum number of directors:");
		for (Film film : globalFilmsWithMaxDirectors) {
			System.out.println("Film id: " + film.tconst + "; Number of directors: " + film.directors.length + ";");
		}
	}
	
}
