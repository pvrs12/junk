#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>
#include <ctime>

#include <map>
#include <set>
#include <vector>

int midsally(std::stringstream& ss);
int getNumber(std::stringstream& ss);

std::map<std::string, int> month_lookup;
std::map<int,int> month_days;
std::set<std::string> relative_keywords;
std::map<std::string,time_t> constants;

time_t nowsecs;
tm* now;

enum weekday {
	Sunday,Monday,Tuesday,Wednesday,Thursday,Friday
};

void findreplace(std::string& str, const std::string finds, const std::string repls){
	size_t pos = 0;
	while((pos=str.find(finds,pos))!=std::string::npos){
		str.replace(pos,finds.length(),repls);
		pos+=repls.length();
	}
}

void setup(){
	//set nowsecs = 1450112881 for accurate dates
	time(&nowsecs);
	now = localtime(&nowsecs);

	month_lookup["january"]=1;
	month_lookup["february"]=2;
	month_lookup["march"]=3;
	month_lookup["april"]=4;
	month_lookup["may"]=5;
	month_lookup["june"]=6;
	month_lookup["july"]=7;
	month_lookup["august"]=8;
	month_lookup["september"]=9;
	month_lookup["october"]=10;
	month_lookup["november"]=11;
	month_lookup["december"]=12;

	month_lookup["jan"]=1;
	month_lookup["feb"]=2;
	month_lookup["mar"]=3;
	month_lookup["apr"]=4;
	month_lookup["may"]=5;
	month_lookup["jun"]=6;
	month_lookup["jul"]=7;
	month_lookup["aug"]=8;
	month_lookup["sep"]=9;
	month_lookup["oct"]=10;
	month_lookup["nov"]=11;
	month_lookup["dec"]=12;

	month_days[1]=31;
	month_days[2]=28;
	month_days[3]=31;
	month_days[4]=30;
	month_days[5]=31;
	month_days[6]=30;
	month_days[7]=31;
	month_days[8]=31;
	month_days[9]=30;
	month_days[10]=31;
	month_days[11]=30;
	month_days[12]=31;

	relative_keywords.insert("ago");
	relative_keywords.insert("from");
	relative_keywords.insert("last");
	relative_keywords.insert("next");

	now->tm_yday++;
	constants["tomorrow"]=mktime(now);
	now->tm_yday-=2;
	constants["yesterday"]=mktime(now);
	now->tm_yday++;
	constants["today"]=mktime(now);
}

std::string tolower(std::stringstream& ss){
	std::string s = ss.str();
	for(unsigned i=0;i<s.size();++i){
		s[i] = tolower(s[i]);
	}
	return s;
}

std::string replace_constants(std::stringstream& ss){
	std::string s;
	std::stringstream ret;
	while(ss>>s){
		for(auto itr = constants.begin();itr!=constants.end();++itr){
			std::stringstream r;
			r<<itr->second;
			findreplace(s,itr->first, r.str());
		}
		ret<<s<<" ";
	}
	return ret.str();
}

void sally(std::stringstream& rs){
	std::stringstream lower(tolower(rs));
	std::stringstream ss(replace_constants(lower));
	printf("sally date\t\"%s\"\n",ss.str().c_str());	
	//if this takes too long
	//printf("the date is %04d-%02d-%02d\n",now->tm_year+1900,now->tm_mon,now->tm_mday);
	//first see if date is relative
	//look for key words
	int relativeKeywordLoc = -1;
	std::vector<std::string> words;
	std::string word;
	
	while(ss>>word){
		if(relative_keywords.find(word)!=relative_keywords.end() && relativeKeywordLoc == -1){
			relativeKeywordLoc = words.size();
		}
		words.push_back(word);
	}
	if(relativeKeywordLoc != -1){
		printf("\tkeyword found, it is likely %s\n",words[relativeKeywordLoc].c_str());
	} else {
		//printf("\tno keyword found, date may be absolute\n");
		std::stringstream absolute;
		for(unsigned i=0;i<words.size();++i){
			absolute<<words[i];
		}
		if(words.size()<2){
			//single word
			//probably a constant time
			long time;
			std::cout<<ss.str()<<std::endl;
			ss>>time;
			std::cout<<time<<std::endl;

			tm* date = localtime(&time);
			printf("%04d-%02d-%02d\n",date->tm_year+1900,date->tm_mon,date->tm_mday);
			return;
		} else {
			//2 words
			int year = now->tm_year+1900;
			int month = midsally(absolute);
			int day = getNumber(absolute);
			printf("%04d-%02d-%02d\n",year,month,day);
			return;
		}
	}
}

int midsally(std::stringstream& ss){
	char c;
	std::stringstream mid;
	while(ss.get(c)){
		c=tolower(c);
		if(!isalpha(c)){
			break;
		} else {
			mid<<c;
		}
	}
	auto itr = month_lookup.find(mid.str());
	if(itr == month_lookup.end()){
		//put all the characters back
		ss.putback(c);
		std::string mid_string = mid.str();
		for(int i = mid_string.size()-1;i>=0;--i){
			ss.putback(mid_string[i]);
		}
		return -1;
	} else {
		return itr->second;
	}
}

int getNumber(std::stringstream& ss){
	bool first = true;
	int number=0;
	char c;
	while(ss.get(c)){
		if(!isdigit(c)){
			if(first){
				ss.putback(c);
				return -1;
			}
			break;	
		} else {
			number*=10;
			number+=c-'0';
		}
		first=false;
	}
	return number;
}

int main(){
	setup();
	while(true){
		int year,month,day;
		std::string line_input;
		if(!std::getline(std::cin,line_input)){
			break;
		}
		std::stringstream line(line_input);
		int firstNum = getNumber(line);
		if(firstNum == -1){
			sally(line);
			continue;
		}
		int secondNum = getNumber(line);
		if(secondNum==-1){
			secondNum = midsally(line);
		}
		if(secondNum==-1){
			//put back onto line_input
			std::stringstream back_buf;
			back_buf<<firstNum<<" ";
			std::string back_string=back_buf.str();
			for(int i = back_string.size()-1;i>=0;--i){
				line.putback(back_string[i]);
			}
			sally(line);
			continue;
		}
		int thirdNum = getNumber(line);

		
		if(firstNum > 12){
			year = firstNum;
			month = secondNum;
			day = thirdNum;
		} else {
			month = firstNum;
			day = secondNum;
			year = thirdNum;
			//if 2 digit year
			if(year < 1000){
				year+=2000;
			}
		}
		if(year==0 || month==0 || day==0){
			break;
		}
		printf("%4d-%02d-%02d\n",year,month,day);
		if(!std::cin || std::cin.eof()){
			break;
		}
	}
}
