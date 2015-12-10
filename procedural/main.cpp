#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <cstdlib>
#include <ctime>

const int WIDTH=200,HEIGHT=200;
const bool COLOR_ON=false;

typedef std::vector<std::vector<int>* > map_type;
#define water 0
#define land 1
#define treasure 2
#define start 3
#define path 4

enum color {
	black,red,green,brown,blue,magenta,cyan,gray
};

int dist(std::pair<int,int> s,std::pair<int,int> e){
	return (int)sqrt((s.first-e.first)*(s.first-e.first)+(s.second-e.second)*(s.second-e.second));
}

void set_color(color c){
	if(!COLOR_ON){
		return;
	}
	std::cout<<"\x1B[3"<<c<<"m";
}

void reset_color(){
	if(!COLOR_ON){
		return;
	}
	std::cout<<"\x1B[39m"<<std::flush;
}

void print_map(map_type& m){
	for(unsigned i=0;i<m.size();++i){
		for(unsigned j=0;j<m[i]->size();++j){
			int space = m[i]->at(j);
			if(space==land){
				set_color(brown);
			} else if(space==water){
				set_color(blue);
			} else if(space==treasure){
				set_color(red);
			} else if(space==path){
				set_color(magenta);
			} else if(space==start){
				set_color(red);
			}
			std::cout<<space<<' ';
		}
		std::cout<<std::endl;
	}
}

int fill_adjacent(map_type& m,int x, int y, int symbol){
	int coverage = 0;
	if(x>0){
		if(y>0){
			m[x-1]->at(y-1)=symbol;
			++coverage;
		}
		if(y<(int)m[x-1]->size()-1){
			m[x-1]->at(y+1)=symbol;
			++coverage;
		}
		m[x-1]->at(y)=symbol;
			++coverage;
	}
	if(x<(int)m.size()-1){
		if(y>0){
			m[x+1]->at(y-1)=symbol;
			++coverage;
		}
		if(y<(int)m[x+1]->size()-1){
			m[x+1]->at(y+1)=symbol;
			++coverage;
		}
		m[x+1]->at(y)=symbol;
			++coverage;
	}
	if(y>0){
		m[x]->at(y-1)=symbol;
			++coverage;
	}
	if(y<(int)m[x]->size()-1){
		m[x]->at(y+1)=symbol;
			++coverage;
	}
	return coverage;
}

bool is_valid(map_type& m, int x, int y){
	return x<(int)m.size() && x > 0 && y < (int)m[0]->size() && y > 0;
}

int gen_land(map_type& m, int depth=0,int start_x=-1,int start_y=-1,int max_depth=5){
	if(depth > max_depth){
		return 0;
	}
	if(start_x==-1 || start_y==-1){
		start_x = rand() % WIDTH;
		start_y = rand() % HEIGHT;
	}

	//choose a start at random
	m[start_x]->at(start_y)=land;
	//fill all squares around this one
	int coverage = fill_adjacent(m,start_x,start_y,land);
	//adjacent and recur down them
	int xchange=-2*WIDTH;
	int ychange=-2*HEIGHT;
	while(!is_valid(m,start_x+xchange,start_y+ychange)){
		xchange=rand()%3-1;
		ychange=rand()%3-1;
	}
	coverage += gen_land(m,depth+1,start_x+xchange, start_y+ychange,max_depth+rand()%3-1);
	xchange=-2*WIDTH;
	ychange=-2*HEIGHT;
	while(!is_valid(m,start_x+xchange,start_y+ychange)){
		xchange=rand()%3-1;
		ychange=rand()%3-1;
	}
	coverage += gen_land(m,depth+1,start_x+xchange, start_y+ychange,max_depth+rand()%3-1);
	return coverage;
}

std::pair<int,int> place_treasure(map_type& m){
	int x=rand()%WIDTH,y=rand()%HEIGHT;
	while(m[x]->at(y)!=land){
		x=rand()%WIDTH,y=rand()%HEIGHT;
	}
	m[x]->at(y)=treasure;
	return std::make_pair(x,y);
}

std::pair<int,int> place_start(map_type& m,std::pair<int,int> end_pos){
	int x=rand()%WIDTH,y=rand()%HEIGHT;
	std::pair<int,int> pos = std::make_pair(x,y);
	while(m[x]->at(y)!=land && (dist(pos,end_pos)>WIDTH/2 || dist(pos,end_pos)<5)){
		x=rand()%WIDTH,y=rand()%HEIGHT;
	}
	m[x]->at(y)=start;
	return std::make_pair(x,y);
}

std::vector<std::pair<int,int>> route;

void draw_path(map_type& m, int rootx, int rooty, int destx, int desty, int start_dist){
	if(rootx==destx && rooty==desty){
		return;
	}
	//pick a random direction, move that way
	int xchange=-2*WIDTH;
	int ychange=-2*HEIGHT;
	while(!is_valid(m,rootx+xchange,rooty+ychange) 
				|| start_dist < dist(std::make_pair(rootx+xchange,rooty+ychange),std::make_pair(destx,desty)) ){
		xchange=rand()%3-1;
		ychange=rand()%3-1;
	}

	draw_path(m,rootx+xchange,rooty+ychange,destx,desty,start_dist);
	route.push_back(std::make_pair(rootx+xchange,rooty+ychange));
}


int main(){
	srand(time(0));

	map_type map(WIDTH);
	for(unsigned i=0;i<map.size();++i){
		map[i]= new std::vector<int>(HEIGHT,0);
	}
	
	gen_land(map);
	gen_land(map);
	gen_land(map);

/*	int coverage =0;
	while(coverage < WIDTH*HEIGHT/2){
		coverage += gen_land(map);
	}
*/
	std::pair<int,int> end_pos = place_treasure(map);
	std::pair<int,int> start_pos = place_start(map,end_pos);
	unsigned d = dist(start_pos,end_pos);
	draw_path(map,start_pos.first,start_pos.second,end_pos.first,end_pos.second, d);

	for(unsigned i=1;i<route.size();++i){
		map[route[i].first]->at(route[i].second)=path;
	}
	map[start_pos.first]->at(start_pos.second)=start;

	print_map(map);
	reset_color();

	for(unsigned i = 0;i<map.size();++i){
		delete(map[i]);
	}
}
