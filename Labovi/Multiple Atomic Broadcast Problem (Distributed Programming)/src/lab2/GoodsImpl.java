package lab2;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class GoodsImpl implements Goods {
	
	private String fileName = null;
	private List<String> body = null;
	private int index = 0;

	public GoodsImpl(String name) {
		this.fileName = name;
	}
	
	public GoodsImpl(Goods goods) {
		setName(goods.getName());
		setBody(goods.getBody());
	}
	
	@Override
	public String getName() {
		return fileName;
	}

	@Override
	public void setName(String name) {
		this.fileName = name;
	}

	@Override
	public String[] getBody() {
		if (body == null) {
			return null;
		}
		return (String[]) body.toArray();
	}

	@Override
	public void setBody(String[] body) {
		this.body = Arrays.asList(body);
		index = 0;
	}

	@Override
	public String readLine() {
		if (body == null || index == body.size()) {
			return null;
		}
		return body.get(index++);
	}

	@Override
	public void printLine(String body) {
		if (this.body == null) {
			this.body = new ArrayList<>();
		}
		this.body.add(body);
	}

	@Override
	public int getNumLines() {
		if (body == null) {
			return -1;
		}
		return body.size();
	}

	@Override
	public void save(String name) {
		if (body == null) {
			return;
		}
		try (BufferedWriter bw = new BufferedWriter(new FileWriter(name))) {
			for (String line : body) {
				bw.append(line);
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	@Override
	public void load(String name) {
		try (BufferedReader br = new BufferedReader(new FileReader(name))) {
			String line;
			body = new ArrayList<>();
			index = 0;
			while ((line = br.readLine()) != null) {
				body.add(line);
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

}
