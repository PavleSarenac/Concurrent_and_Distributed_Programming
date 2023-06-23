package rs.ac.bg.etf.kdp.lab1_2020;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

public class Test {

	private static final int BUFFER_CAPACITY = 10000;
	
	public static void main(String[] args) {
		long programStartTime = System.currentTimeMillis();
		
		final int numberOfConsumers = Integer.parseInt(args[0]);
		final int progressUpdateFrequency = Integer.parseInt(args[1]);
		final String fileName = args[2];
		final long printingFrequency = Long.parseLong(args[3]);
	
		BoundedBuffer<String> linesFromFile = new RegionBoundedBuffer<>(BUFFER_CAPACITY);
		Barrier consumerBarrier = new MonitorBarrier(numberOfConsumers);
		ConcurrentMap<Integer, Integer> progress = new ConcurrentHashMap<>();
		BoundedBuffer<Result> results = new SemaphoreBoundedBuffer<>(numberOfConsumers + 1);
		ConcurrentMap<Integer, Boolean> selectedConsumers = new ConcurrentHashMap<>();
		Barrier selectedBarrier = new RegionBarrier(1);
		Barrier printerBarrier = new SemaphoreBarrier(1);
		BoundedBuffer<Film> sending = new MonitorBoundedBuffer<>(BUFFER_CAPACITY);
		List<Film> globalFilmsWithMaxDirectors = new ArrayList<>();
		
		Producer producer = new Producer(fileName, linesFromFile);
		producer.start();
		
		for (int i = 0; i < numberOfConsumers; i++) {
			Consumer consumer = new Consumer(i, progressUpdateFrequency, linesFromFile, progress, results,
											 selectedConsumers, selectedBarrier, consumerBarrier, sending);
			consumer.start();
		}
		
		Combiner combiner = new Combiner(consumerBarrier, results, selectedConsumers, selectedBarrier, sending,
										 globalFilmsWithMaxDirectors, printerBarrier);
		combiner.start();
		
		Printer printer = new Printer(printingFrequency, progress, printerBarrier, globalFilmsWithMaxDirectors);
		printer.start();
		
		try {
			printer.join();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		long programEndTime = System.currentTimeMillis();
		System.out.println("Execution time: " + (programEndTime - programStartTime) + " miliseconds.");
	}
	
}
