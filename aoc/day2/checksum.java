import java.util.Scanner;

public class checksum {
	public static void main(String[] args){
		Scanner in = new Scanner(System.in);

		int total = 0;
		while(in.hasNextLine()){
			String line = in.nextLine();
			String[] nums = line.split("\t");
			int max = -1; 
			int min = 5000;
			for(String s: nums){
				if(s.trim().length() < 1){
					continue;
				}
				int i = Integer.parseInt(s);
				if(i < min){
					min = i;
				}
				if(i > max){
					max = i;
				}
			}
			if(max > -1 && min < 5000){
				total += max - min;
			}
		}
		System.out.println(total);
	}
}
