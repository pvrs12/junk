import java.util.Scanner;
public class checksum {
	public static void main(String[] args){
		Scanner in = new Scanner(System.in);
		int total = 0;
		while(in.hasNextLine()){
			String line = in.nextLine();
			String[] ints = line.split("\t");

			outer:for(int i=0; i<ints.length-1; ++i){
				int n = Integer.parseInt(ints[i]);
				for(int j=i+1; j<ints.length; ++j){
					int m = Integer.parseInt(ints[j]);
					if(n % m == 0){
						total += n / m;
						break outer;
					} else if(m % n == 0){
						total += m / n;
						break outer;
					}
				}
			}
		}
		System.out.println(total);
		in.close();
	}
}
