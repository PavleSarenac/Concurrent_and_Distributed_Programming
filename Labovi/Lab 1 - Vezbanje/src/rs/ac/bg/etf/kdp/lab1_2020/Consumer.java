package rs.ac.bg.etf.kdp.lab1_2020;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ConcurrentMap;

public class Consumer extends Thread {

	private final int threadId;
	private final int progressUpdateFrequency;
	private final BoundedBuffer<String> linesFromFile;
	private final ConcurrentMap<Integer, Integer> progress;
	private final BoundedBuffer<Result> results;
	private final ConcurrentMap<Integer, Boolean> selectedConsumers;
	private final Barrier selectedBarrier;
	private final Barrier consumerBarrier;
	private final BoundedBuffer<Film> sending;
	
	public Consumer(int threadId, int progressUpdateFrequency, BoundedBuffer<String> linesFromFile, 
					ConcurrentMap<Integer, Integer> progress, BoundedBuffer<Result> results, 
					ConcurrentMap<Integer, Boolean> selectedConsumers, Barrier selectedBarrier, Barrier consumerBarrier, 
					BoundedBuffer<Film> sending) {
		super("Consumer" + threadId);
		this.threadId = threadId;
		this.progressUpdateFrequency = progressUpdateFrequency;
		this.linesFromFile = linesFromFile;
		this.progress = progress;
		this.results = results;
		this.selectedConsumers = selectedConsumers;
		this.selectedBarrier = selectedBarrier;
		this.consumerBarrier = consumerBarrier;
		this.sending = sending;
	}
	
	@Override
	public void run() {
		String lineFromFile;
		List<Film> localFilmsWithMaxDirectors = new ArrayList<>();
		int localMaxDirectors = 0;
		int numOfProcessedFilms = 0;
		while ((lineFromFile = linesFromFile.get()) != null) {
			Film currentFilm = new Film(lineFromFile);
			int currentDirectors = currentFilm.directors == null ? 0 : currentFilm.directors.length;
			if (currentDirectors > localMaxDirectors) {
				localMaxDirectors = currentDirectors;
				localFilmsWithMaxDirectors.clear();
			}
			if (currentDirectors == localMaxDirectors) {
				localFilmsWithMaxDirectors.add(currentFilm);
			}
			if (++numOfProcessedFilms % progressUpdateFrequency == 0) {
				progress.put(threadId, numOfProcessedFilms);
			}
		}
		linesFromFile.put(null);
		progress.put(threadId, numOfProcessedFilms);
		results.put(new Result(threadId, localMaxDirectors));
		consumerBarrier.arrived();
		selectedBarrier.await(0);
		if (selectedConsumers.getOrDefault(threadId, false)) {
			for (Film film : localFilmsWithMaxDirectors) {
				sending.put(film);
			}
			sending.put(null);
		}
	}
	
}
