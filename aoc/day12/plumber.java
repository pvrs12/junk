import java.util.*;

public class plumber {

    static HashSet<Integer> visited = new HashSet<>();

    public static int get_connected(HashMap<Integer, ArrayList<Integer>> dataset, int start){
        if(visited.contains(start)){
            return 0;
        }
        visited.add(start);

        int total = 1;
        for(int i : dataset.get(start)){
            total += get_connected(dataset, i);
        }
        return total;
    }

    public static void main(String[] args){
        Scanner in = new Scanner(System.in);

        HashMap<Integer, ArrayList<Integer>> dataset = new HashMap<>();

        while(in.hasNextLine()){
            String line = in.nextLine();
            if(line.length() < 1){
                continue;
            }
            
            String[] parts = line.split("<->");
            int n = Integer.parseInt(parts[0].trim());
            String[] connections = parts[1].split(",");

            ArrayList<Integer> conns = new ArrayList<>();
            for(String s :connections){
                conns.add(Integer.parseInt(s.trim()));
            }
            dataset.put(n, conns);
        }

        //start at 0, DFS and count unique entries
        //System.out.println(get_connected(dataset, 0));

        int count = 0;
        for(HashMap.Entry<Integer, ArrayList<Integer>> kv: dataset.entrySet()){
            if(!visited.contains(kv.getKey())){
                System.out.println(kv.getKey() + "\t" + get_connected(dataset, kv.getKey()));
                count ++;
            }
        }
        System.out.println(count);

        in.close();
    }
}