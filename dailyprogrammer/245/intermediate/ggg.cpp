#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

#include <bitset>
#include <map>
#include <queue>

struct tree {
	tree* left;
	tree* right;

	int freq;
	char c;
};

class Compare {
public:
	bool operator() (tree* a, tree* b){
		return a->freq > b->freq;
	}
};

void findReplace(std::string& str, const std::string& oldStr, const std::string& newStr){
  size_t pos = 0;
  while((pos = str.find(oldStr, pos)) != std::string::npos){
     str.replace(pos, oldStr.length(), newStr);
     pos += newStr.length();
  }
}

std::string encode(std::string& plain,std::map<char,std::string> conv){
	for(auto itr = conv.begin();itr!=conv.end();++itr){
		std::cout<<itr->first<<" "<<itr->second<<" ";
	}
	std::cout<<std::endl;

	std::stringstream ss;
	for(unsigned i=0;i<plain.size();++i){
		if(!isalpha(plain[i])){
			ss<<plain[i];
		} else {
			if(conv.find(plain[i])==conv.end()){
				std::cout<<"something went wrong"<<std::endl;
			}
			else{
				ss<<conv[plain[i]];
			}
		}
	}
	return ss.str();
}

std::string decode(std::string& encoded, std::map<std::string,char> conv){
	std::stringstream ss;
	int len=0;
	for(unsigned i=0;i<encoded.size()-len;++i){
		if(!isalpha(encoded[i])){
			ss<<encoded[i];
		} else {
			std::string lookup = encoded.substr(i,len+1);
			if(conv.find(lookup)==conv.end()){
				//not found
				len++;
				i--;
				if(encoded.size()-len < 0){
					std::cout<<"something went wrong"<<std::endl;
				}
			} else {
				i+=len;
				len=0;
				ss<<conv[lookup];
			}
		}
	}
	return ss.str();
}

std::map<std::string,char> make_map(std::string& m){
	std::map<std::string,char> conv;
	std::stringstream ss(m);
	char c;
	std::string s;
	while(ss>>c>>s){
		conv[s]=c;
	}
	return conv;
}

void in_order(tree* root, size_t depth=0,size_t path=0){
	if(root->left != nullptr){
		in_order(root->left,depth+1,(path|0)<<1);
	}
	std::cout<<std::string(depth,' ')<<path<<"\t"<<root->c<<std::endl;
	if(root->right != nullptr){
		in_order(root->right,depth+1,(path|1)<<1);
	}
}

void build_huffman_map(std::map<char,std::string>& c, tree* root, int path=0){
	if(root->left != nullptr){
		std::cout<<"going left"<<std::endl;
		build_huffman_map(c,root->left,(path | 0) << 1);
	} 
	if(root->c != '\0'){
		findReplace(p,"0","g");
		findReplace(p,"1","G");
		c[root->c] = p;
	}
	if(root->right !=nullptr){
		std::cout<<"going right"<<std::endl;
		build_huffman_map(c,root->right, (path | 1) << 1);
	}
}

std::map<char,std::string> gen_huffman_encoding(std::string& plain){
	std::map<char,int> prob;
	//caclulate probability for each character
	for(unsigned i=0;i<plain.size();++i){
		if(!isalpha(plain[i])){
			continue;
		}
		if(prob.find(plain[i])==prob.end()){
			prob[plain[i]]=1;
		} else {
			prob[plain[i]]++;
		}
	}
	
	std::priority_queue<tree*,std::vector<tree*>,Compare> pque;
	for(auto itr=prob.begin();itr!=prob.end();++itr){
		tree* t = new tree;

		t->left=nullptr;
		t->right=nullptr;
		t->c = itr->first;
		t->freq = itr->second;

		pque.push(t);
	}

	while(pque.size()>1){
		tree* l1 = pque.top();
		pque.pop();
		tree* l2=pque.top();
		pque.pop();

		tree* internal = new tree;
		internal -> left = l2;
		internal -> right = l1;
		internal -> c = '\0';
		internal -> freq = l1->freq + l2->freq;

		pque.push(internal);
	}
	
	std::map<char,std::string> ret;
	//in_order(pque.top());
	build_huffman_map(ret,pque.top());
	return ret;	
}

int main(){
	std::string input1,input2;
	std::getline(std::cin,input1);
	if(std::getline(std::cin,input2)){
		std::cout<<"decoding"<<std::endl;
		//decode
		std::map<std::string,char> m = make_map(input1);
		std::cout<<decode(input2,m)<<std::endl;
	} else {
		std::cout<<"encoding"<<std::endl;
		//encode
		//std::map<char, std::string> m = gen_simple_encoding(input1);
		std::map<char,std::string> m = gen_huffman_encoding(input1);
		std::cout<<encode(input1,m)<<std::endl;
	}
}
