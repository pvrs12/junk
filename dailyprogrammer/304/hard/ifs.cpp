#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <random>

int RES = 10000;

int WIDTH = 1000;
int HEIGHT = 1000;

bool USE_COLOR = false;

struct color {
	int r,g,b;
	
	color():
		r(0), g(0), b(0)
	{}

	color(int _r, int _g, int _b):
		r(_r), g(_g), b(_b)
	{}
};

struct equ {
	double a,b,c,d,e,f;
	color col;

	double apply_x(double x, double y){
		return a * x + b * y + e;
	}

	double apply_y(double x, double y){
		return c * x + d * y + f;
	}

	std::string to_str(){
		std::stringstream ss;
		ss << a << " " << b << " " << c << " " << d << " " << e << " " << f;
		return ss.str();
	}
};

std::vector<color> COLOR_LIST = {
	color(255,0,0),
	color(255,255,0),
	color(0,255,0),
	color(0,255,255),
	color(0,0,255),
	color(255,0,255)
};

double scale(double x, double in_min, double in_max, double out_min, double out_max){
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void write_pbm(const std::vector<std::vector<color>>& img, std::ostream& os){
	os << "P3\n";
	os << img.size() << " " << img[0].size() << "\n";
	os << "255\n";
	for(std::vector<color> v: img){
		for(color& c:v){
			os << c.r << " " << c.g << " " << c.b<< "	";
		}
		os << "\n";
	}
}

int main(int argc, char* argv[]){

	if(argc > 1){
		RES = atoi(argv[1]);
		if(argc > 3){
			WIDTH = atoi(argv[2]);
			HEIGHT = atoi(argv[3]);
			if(argc > 4){
				USE_COLOR = true;
			}
		}
	}

	std::vector<equ> equs;
	std::vector<double> weights;

	std::vector<std::pair<color, std::pair<double, double>>> points;

	while(true){
		equ e;
		double p;
		std::cin >> e.a >> e.b >> e.c >> e.d >> e.e >> e.f >> p;
		if( !e.a && !e.b && !e.c && !e.d && !e.e && !e.f && !p){
			break;
		}
		e.col = COLOR_LIST[equs.size() % COLOR_LIST.size()];
		equs.push_back(e);
		weights.push_back(p);
	}
	//generate random numbers with weights
	std::discrete_distribution<> dist(weights.begin(), weights.end());
	std::random_device rd;
	std::mt19937 gen(rd());

	//initial X and Y
	double x = 1, y = 1;
	double min_x= 100, min_y = 100, max_x = -100, max_y = -100;
	for(int i = 0;i < RES; ++i){
		int n = dist(gen);
		equ e = equs[n];
		//std::cout<<x<<" "<<y<<std::endl;
		if(x < min_x){
			min_x = x;
		}
		if(y < min_y){
			min_y = y;
		}
		if(x > max_x){
			max_x = x;
		}
		if(y > max_y){
			max_y = y;
		}
		points.push_back(std::make_pair(e.col, std::make_pair(x, y)));

		x = e.apply_x(x,y);
		y = e.apply_y(x,y);
	}
	std::vector<std::vector<color>> image(WIDTH + 1, std::vector<color>(HEIGHT + 1, color(255, 255, 255)));

	for(std::pair<color,std::pair<double, double>>& pp: points){
		std::pair<double,double> p = pp.second;
		int image_x = scale(p.first - min_x, 0, max_x - min_x, 0, WIDTH);
		int image_y = scale(p.second - min_y, 0, max_y - min_y, 0, HEIGHT);
		//std::cout<<image_x<<" "<<image_y<<std::endl;

		if(USE_COLOR){
			image[image_x][image_y] = pp.first;
		} else {
			image[image_x][image_y] = color(0,0,0);
		}
	}
	write_pbm(image, std::cout);
}
