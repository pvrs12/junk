#include <iostream>
#include <string>
#include <vector>

using namespace std;

void replace_str(string& s, string find, string rep){
	size_t pos = 0;
	while((pos=s.find(find,pos))!=string::npos){
		s.replace(pos,find.length(),rep);
		pos+=rep.length();
	}
}

void remove_nevers(string& s){
	replace_str(s,"qu","\x0001");
	replace_str(s,"tr","\x0002");
	replace_str(s,"br","\x0003");
	replace_str(s,"str","\x0004");
	replace_str(s,"st","\x0005");
	replace_str(s,"sl","\x0006");
	replace_str(s,"bl","\x0007");
	replace_str(s,"cr","\x0008");
	replace_str(s,"ph","\x0009");
	replace_str(s,"ch","\x000a");
}

void restore_nevers(string& s){
	replace_str(s,"\x0001","qu");
	replace_str(s,"\x0002","tr");
	replace_str(s,"\x0003","br");
	replace_str(s,"\x0004","str");
	replace_str(s,"\x0005","st");
	replace_str(s,"\x0006","sl");
	replace_str(s,"\x0007","bl");
	replace_str(s,"\x0008","cr");
	replace_str(s,"\x0009","ph");
	replace_str(s,"\x000a","ch");
}

int vowel_len = 6;
char vowels[] = {'a','e','i','o','u','y'};

bool is_vowel(char c){
	for(int j=0;j<vowel_len;++j){
		if(c==vowels[j]){
			return true;
		}
	}
	return false;
}

void split_vccv(string& s){
	vector<int> splits;
	if(s.size() < 4){
		return;
	}
	for(int i=0;i<s.size()-3;++i){
		if(is_vowel(s[i])){
			if(!is_vowel(s[i+1])){
				if(!is_vowel(s[i+2])){
					if(is_vowel(s[i+3])){
						//vccv
						//split vc-cv
						splits.push_back(i+2);
					}
				}
			}
		}
	}
	for(int i=0;i<splits.size();++i){
		s.insert(splits[i],"-");
	}
}

void split_vcv(string& s){
	vector<int> splits;
	if(s.size()<3){
		return;
	}
	for(int i=0;i<s.size()-2;++i){
		if(is_vowel(s[i])){
			if(!is_vowel(s[i+1])){
				if(is_vowel(s[i+2])){
					if(s[i+2] == s.size()-1){
						//if end of word
						if(s[i+2]!='e'){
							//.*vc(^e)v
							//split v-cv
							splits.push_back(i+1);
						}
					} else {
						//not end of word
						//split v-cv
						splits.push_back(i+1);
					}
				}
			}
		}
	}
	for(int i=0;i<splits.size();++i){
		s.insert(splits[i],"-");
	}
}

int main(){
	string s;
	while(true){
		cin>>s;
		if(s=="==="){
			break;
		}
	//	cout<<"with nevers\t"<<s<<endl;
		remove_nevers(s);
	//	cout<<"without nevers\t"<<s<<endl;
		split_vccv(s);
	//	cout<<"vccv split\t"<<s<<endl;
		split_vcv(s);
	//	cout<<"vcv split\t"<<s<<endl;
		restore_nevers(s);
		cout<<s<<endl;
	}
}
