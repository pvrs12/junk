#include <iostream>
#include <vector>
#include <string>
#include <cmath>

#define DIAGONALS 0

struct pixel {
	int r,g,b;
	pixel(int red,int green,int blue):
		r(red),g(green),b(blue)
	{}

	pixel():
		r(0),g(0),b(0)
	{}
};

class BadCommandException{};

typedef std::vector<std::vector<pixel> > image ;
//#define image std::vector<std::vector<pixel> >

void print_image(image& img){
	//header
	std::cout<<"P3"<<std::endl;
	//size
	std::cout<<img.size()<<" "<<img[0].size()<<std::endl;
	//max color
	std::cout<<255<<std::endl;

	//pixel data
	for(unsigned i=0;i<img.size();++i){
		for(unsigned j=0;j<img[0].size();++j){
			std::cout<<img[i][j].r<<" "<<img[i][j].g<<" "<<img[i][j].b<<"\t";
		}
		std::cout<<"\n";
	}
	std::cout<<std::flush;
}

bool is_empty(const pixel& p){
	return (p.r==0 && p.g==0 && p.b==0);
}

void draw_pixel(pixel& color, image& img, int r, int c, bool quiet=false){
#ifdef DEBUG
	if(!quiet){
		std::cerr<<"drawing pixel @ ["<<r<<","<<c<<"]"<<std::endl;
	}
#endif
	img[r][c]=color;
}

void draw_line(pixel& color, image& img, int r0,int c0,int r1,int c1, bool quiet=false){
			int startrow,startcol;
			int endrow,endcol;
			startcol=std::min(c0,c1);
			endcol=std::max(c0,c1);
			if(startcol==c0){
				startrow=r0;
				endrow = r1;
			} else {
				startrow=r1;
				endrow=r0;
			}

#ifdef DEBUG
			if(!quiet){
				std::cerr<<"drawing line from ["<<startrow<<","<<startcol<<"] to ["<<endrow<<","<<endcol<<"]";
			}
#endif

			//vertical
			if(startcol==endcol){
#ifdef DEBUG
				if(!quiet){
					std::cerr<<"vertical line"<<std::endl;
				}
#endif
				for(int row=std::min(startrow,endrow);row<std::max(startrow,endrow);++row){
					draw_pixel(color,img,row,startcol,true);
				}
			} else {
				double slope = ((double)endrow-startrow)/((double)endcol-startcol);
#ifdef DEBUG
				if(!quiet){
					std::cerr<<"slope="<<slope<<std::endl;
				}
#endif
				for(int col = startcol;col<endcol;++col){
					int row = (slope*col+startrow);
					if(row >= (int)img.size()){
						continue;
					}
					if(col >= (int)img[0].size()){
						continue;
					}
					draw_pixel(color,img,row,col,true);
				}
			}
}

void draw_rect(pixel& color, image& img,int r,int c, int width, int height){
#ifdef DEBUG
	std::cerr<<"drawing rect @ ["<<r<<","<<c<<"] width="<<width<<" height="<<height<<std::endl;
#endif

	for(int i=r;i<r+height;++i){
		draw_line(color,img,i,c,i,c+width,true);
	}
}

void fill(pixel& color, image& img, int r, int c){
	int rows=(int)img.size();
	int cols=(int)img[0].size();
	
	draw_pixel(color,img,r,c);

	if(r>0){
#if DIAGONALS==1
		if(c>0){
			if(is_empty(img[r-1][c-1])){
				fill(color,img,r-1,c-1);
			}
		}
		if(c<cols-1){
			if(is_empty(img[r-1][c+1])){
				fill(color,img,r-1,c+1);
			}
		}
#endif
		if(is_empty(img[r-1][c])){
			fill(color,img,r-1,c);
		}
	}
	if(r<rows-1){
#if DIAGONALS==1
		if(c>0){
			if(is_empty(img[r+1][c-1])){
				fill(color,img,r+1,c-1);
			}
		}
		if(c<cols-1){
			if(is_empty(img[r+1][c+1])){
				fill(color,img,r+1,c+1);
			}
		}
#endif
		if(is_empty(img[r+1][c])){
			fill(color,img,r+1,c);
		}
	}
	if(c>0){
		if(is_empty(img[r][c-1])){
			fill(color,img,r,c-1);
		}
	}
	if(c<cols-1){
		if(is_empty(img[r][c+1])){
			fill(color,img,r,c+1);
		}
	}
}

void grey(image& img){
	int rows=img.size();
	int cols=img[0].size();
	for(int i=0;i<rows;++i){
		for(int j=0;j<cols;++j){
			pixel p = img[i][j];
			int avg = (p.r+p.g+p.b)/3;
			img[i][j].r=avg;
			img[i][j].g=avg;
			img[i][j].b=avg;
		}
	}
}

void edge(image& img){
	int rows=(int)img.size();
	int cols=(int)img[0].size();
	grey(img);

	int horz,vert;
	int a=0,b=0,c0=0,d=0,e=0,f=0,g=0,h=0;
	for(int r=0;r<rows;++r){
		for(int c=0;c<cols;++c){
			if(r>0){
				if(c>0){
					//a
					a=img[r-1][c-1].r;
				}
				if(c<cols-1){
					//c
					c0=img[r-1][c+1].r;
				}
				//b
				b=img[r-1][c].r;
			}
			if(r<rows-1){
				if(c>0){
					//f
					f=img[r+1][c-1].r;
				}
				if(c<cols-1){
					//h
					h=img[r+1][c+1].r;
				}
				//g
				g=img[r+1][c].r;
			}
			if(c>0){
				//d
				d=img[r][c-1].r;
			}
			if(c<cols-1){
				//e
				e=img[r][c+1].r;
			}
			horz=(c+2*e+h)-(a+2*d+f);
			vert=(f+2*g+h)-(a+2*b+c0);

			int color = (int)sqrt((horz*horz)+(vert*vert));
			img[r][c].r=color;
			img[r][c].g=color;
			img[r][c].b=color;
		}
	}
}

int main(){
	int rows,cols;
	std::cin>>rows>>cols;
	image img = std::vector<std::vector<pixel> >(rows,std::vector<pixel>(cols));

	std::string cmd;
	while(std::cin>>cmd){
		if(cmd==""
			|| cmd[0]=='#'){
			//comment or blank line
			std::getline(std::cin,cmd);
			continue;
		}
		if(cmd=="edge"){
			edge(img);
			std::getline(std::cin,cmd);
			continue;
		} else if(cmd=="grey"){
			grey(img);
			std::getline(std::cin,cmd);
			continue;
		}

		pixel p;
		std::cin>>p.r>>p.g>>p.b;

		if(cmd=="point"){
			int row,col;
			std::cin>>row>>col;
			
			draw_pixel(p,img,row,col);
		} else if(cmd=="line"){
			int row0,col0,row1,col1;
			std::cin>>row0>>col0>>row1>>col1;
			draw_line(p,img,row0,col0,row1,col1);
		
		} else if(cmd=="rect"){
			int row,col,width,height;
			std::cin>>row>>col>>width>>height;
			draw_rect(p,img,row,col,width,height);

		} else if(cmd=="fill"){
			int r,c;
			std::cin>>r>>c;
			fill(p,img,r,c);
		}
		
		else {
			throw BadCommandException();
		}
	}

	print_image(img);
}
