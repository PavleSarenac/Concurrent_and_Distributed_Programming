package rs.ac.bg.etf.kdp.lab1_2020;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ConcurrentMap;

public class Combiner extends Thread {
	
	private final Barrier consumerBarrier;
	private final BoundedBuffer<Result> results;
	private final ConcurrentMap<Integer, Boolean> selectedConsumers;
	private final Barrier selectedBarrier;
	private final BoundedBuffer<Film> sending;
	private final List<Film> globalFilmsWithMaxDirectors;
	private final Barrier printerBarrier;

	public Combiner(Barrier consumerBarrier, BoundedBuffer<Result> results, ConcurrentMap<Integer, Boolean> selectedConsumers,
					Barrier selectedBarrier, BoundedBuffer<Film> sending, List<Film> globalFilmsWithMaxDirectors,
					Barrier printerBarrier) {
		super("Combiner");
		this.consumerBarrier = consumerBarrier;
		this.results = results;
		this.selectedConsumers = selectedConsumers;
		this.selectedBarrier = selectedBarrier;
		this.sending = sending;
		this.globalFilmsWithMaxDirectors = globalFilmsWithMaxDirectors;
		this.printerBarrier = printerBarrier;
	}
	
	@Override
	public void run() {
		Result currentResult;
		List<Integer> consumersWithFilmsWithMaxDirectorsIds = new ArrayList<>();
		int globalMaxDirectors = 0;
		consumerBarrier.await(0);
		results.put(null);
		while ((currentResult = results.get()) != null) {
			int currentDirectors = currentResult.localMaxDirectors;
			if (currentDirectors > globalMaxDirectors) {
				globalMaxDirectors = currentDirectors;
				consumersWithFilmsWithMaxDirectorsIds.clear();
			}
			if (currentDirectors == globalMaxDirectors) {
				consumersWithFilmsWithMaxDirectorsIds.add(currentResult.threadId);
			}
		}
		int numberOfConsumersForSending = consumersWithFilmsWithMaxDirectorsIds.size();
		for (int consumerId : consumersWithFilmsWithMaxDirectorsIds) {
			selectedConsumers.put(consumerId, true);
		}
		selectedBarrier.arrived();
		for (int i = 0; i < numberOfConsumersForSending;) {
			Film currentFilm = sending.get();
			if (currentFilm == null) {
				i++;
			} else {
				globalFilmsWithMaxDirectors.add(currentFilm);
			}
		}
		printerBarrier.arrived();
	}
	
}
