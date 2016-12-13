#include <iostream>
#include <vector>
#include <string>

using namespace std;

typedef vector<vector<int> > grid;

void draw_grid(const grid& g){
	for(int i=0;i<g.size();++i){
		for(int j=0;j<g[0].size();++j){
			cout<<g[i][j] % 5 + 1<<" ";
		}
		cout<<endl;
	}
}

int main(){
		grid g(3,vector<int>(3,0));
		g[1][1]=1;
		char button;
		string trash;
		draw_grid(g);
		while(true){
			cin>>button;
			getline(cin,trash);
			switch(button){
				case('a'):
					g[0][0]++;
					g[0][1]++;
					g[1][0]++;
					g[1][1]++;
					break;
				case('b'):
					g[0][1]++;
					g[0][2]++;
					g[1][1]++;
					g[1][2]++;
					break;
				case('c'):
					g[1][0]++;
					g[1][1]++;
					g[2][0]++;
					g[2][1]++;
					break;
				case('d'):
					g[1][1]++;
					g[1][2]++;
					g[2][1]++;
					g[2][2]++;
					break;
			}
			for(int i=0;i<g.size();++i){
				for(int j=0;j<g.size();++j){
					g[i][j]++;
				}
			}

			draw_grid(g);
		}

}
