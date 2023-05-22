package moje;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class GoodsImpl implements Goods, Serializable {
	
	private String fileName = null;
	private List<String> body = null;
	private int readIndex = 0;
	
	public GoodsImpl(String fileName) {
		this.fileName = fileName;
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
	public void setName(String fileName) {
		this.fileName = fileName;
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
	}

	@Override
	public String readLine() {
		if (this.body == null) {
			return null;
		}
		return this.body.get(readIndex++);
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
		try (BufferedWriter bw = new BufferedWriter(new FileWriter(name, true))) {
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
			readIndex = 0;
			while ((line = br.readLine()) != null) {
				body.add(line);
			}
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

}
