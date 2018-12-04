import java.util.*;

public class tree {
		
	static class Node{
		String name;
		String child_string = "";
		int weight;
		boolean is_leaf;

		Node parent = null;
		ArrayList<Node> children;

		static Node parseNode(String line){
			String[] parts = line.split("->");

			String[] first_parts = parts[0].split(" ");
			String _name = first_parts[0].trim();
			int _weight = 0;
			try{
				_weight = Integer.parseInt(first_parts[1].substring(1, first_parts[1].length() - 1));
			}catch(RuntimeException e){
				System.out.println("Error! `"+line+"`");
				throw e;
			}

			boolean _is_leaf = parts.length == 1;

			Node n;
			if(_is_leaf){
				n = new Node(_name, _weight, _is_leaf);
			} else {
				n = new Node(_name, _weight, parts[1]);
			}
			return n;
		}

		Node(String name, int weight, boolean is_leaf) {
			this.name = name;
			this.weight = weight;

			this.is_leaf = is_leaf;
			if(!is_leaf) {
				children = new ArrayList<>();
			} else {
				children = null;
			}
		}

		Node(String name, int weight, String child_string) {
			this(name, weight, false);

			this.child_string = child_string;
		}

		int addChildren(ArrayList<Node> nodes){
			int childrenAdded = 0;

			String[] children_s = child_string.split(",");
			for(String child_s: children_s){
				child_s = child_s.trim();

				Node child = null;
				for(Node n: nodes){
					if(n.name.equals(child_s)){
						addChild(n);
						child = n;
						childrenAdded++;
						break;
					}
				}
				if(child == null) {
					throw new RuntimeException("Can't find the node `"+child_s+"`");
				}
			}
			return childrenAdded;
		}

		private void addChild(Node n){
			if(is_leaf){
				throw new RuntimeException("Can't add child to a leaf!");
			}
			if(n.parent != null){
				throw new RuntimeException("Can't re-parent a node!");
			}
			
			n.parent = this;
			children.add(n);
		}

		@Override
		public boolean equals(Object b){
			if(!(b instanceof Node)){
				return false;
			}

			return name == ((Node)b).name;
		}

		@Override
		public String toString(){
			StringBuilder s = new StringBuilder();
			s.append("\"").append(name).append("\"").append(" (").append(weight).append(")");
			if(!is_leaf){
				s.append(" -> ");
				for(Node n: children){
					s.append("\"").append(n.name).append("\", ");
				}
			}
			return s.toString();
		}
	}

	public static void main(String[] args){
		Scanner in = new Scanner(System.in);
		
		ArrayList<Node> nodes = new ArrayList<>();
		while(in.hasNextLine()){
			String line = in.nextLine();
			if(line.trim().length() < 1){
				continue;
			}

			Node n = Node.parseNode(line);
			nodes.add(n);
		}
		for(Node n: nodes){
			System.out.println(n);
		}

		System.out.println("=-=-=-=-=-=-=-=-=");

		for(Node n:nodes){
			if(!n.is_leaf){
				n.addChildren(nodes);
			}
		}
		for(Node n: nodes){
			if(n.parent == null){
				System.out.println(n);
			}
		}
	}
}
