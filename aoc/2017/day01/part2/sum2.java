import java.util.*;

public class sum2 {
	public static void main(String[] args){
		Scanner in = new Scanner(System.in);
		
		String s = in.next(); 
		int total = 0;
		for(int i=0; i<s.length(); ++i){
			int j = s.charAt(i) - '0';
			int k = s.charAt((i + s.length() / 2) % s.length()) - '0';
			if(j == k){
				total += j;
			}
		}
		System.out.println(total);

		in.close();
	}
}
