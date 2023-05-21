package lab2;

public class Consumer {

	public static void main(String[] args) throws Exception {
		String host = args[0];
		int port = Integer.parseInt(args[1]);
		AB ab = new RMIAB();

		if (!ab.init(host, port))
			return;

		for (int i = 2; i < args.length; i++) {
			String name = args[i];

			Goods goods = ab.getGoods(name);

			int size = goods.getNumLines();
			for (int j = 0; j < size; j++) {
				System.out.println(goods.readLine());
				Thread.sleep(1000 + (int) (Math.random() * 734));
			}
		}
		ab.close();
	}
}
