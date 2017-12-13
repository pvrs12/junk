import java.util.*;

public class instruction {
    public static void main(String[] args){
        Scanner in = new Scanner(System.in);

        HashMap<String, Integer> mem = new HashMap<>();

        while(in.hasNextLine()){
            String line = in.nextLine();
            if(line.length() < 1){
                continue;
            }

            String[] split = line.split(" if ");

            String[] statements = split[0].trim().split(" ");
            String reg = statements[0];
            String instruction = statements[1];
            int value = Integer.parseInt(statements[2]);
            if(!mem.containsKey(reg)){
                mem.put(reg, 0);
            }

            String[] condition = split[1].trim().split(" ");
            String c_reg = condition[0];
            String conditional = condition[1];
            int c_value = Integer.parseInt(condition[2]);
            if(!mem.containsKey(c_reg)){
                mem.put(c_reg, 0);
            }

            boolean run_command = false;
            int c_r = mem.get(c_reg);
            //System.out.printf("'%s'=='%d'\n", c_reg, c_r);
            //System.out.printf("test='%d'\n", c_value);
            switch(conditional){
                case "==":
                    run_command = c_r == c_value;
                    break;
                case "!=":
                    run_command = c_r != c_value;
                    break;
                case "<=":
                    run_command = c_r <= c_value;
                    break;
                case ">=":
                    run_command = c_r >= c_value;
                    break;
                case ">" :
                    run_command = c_r >  c_value;
                    break;
                case "<" :
                    run_command = c_r <  c_value;
                    break;
                default:
                    throw new RuntimeException("bad conditional");
            }
            if(run_command){
                //System.out.println("Running: c_r = " + c_r + "\t" + line);

                int r = mem.get(reg);
                if(instruction.equals("inc")){
                    r += value;
                } else if(instruction.equals("dec")){
                    r -= value;
                } else {
                    throw new RuntimeException("bad operation");
                }
                //System.out.printf("Writing '%d' to '%s'\n", r, reg);
                mem.put(reg, r);
            }
        }
        int max = Integer.MIN_VALUE;
        for (HashMap.Entry<String, Integer> data : mem.entrySet()) {
            if(data.getValue() > max){
                max = data.getValue();
            }
        }
        System.out.println(max);
        in.close();
    }
}