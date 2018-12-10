#include <chrono>
#include <cstdio>
#include <cstdlib>

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::nanoseconds;
int main() {
  auto begin = high_resolution_clock::now();
  char before;
  char after;
  const int V = 26;
  uint8_t adj[V][10] = {{0}};

  int degree[V] = {0};
  while( true ) {
    if( scanf( "Step %c must be finished before step %c can begin.\n", &before, &after ) == EOF ) {
      break;
    }
    adj[before - 'A'][adj[before - 'A'][0] + 1] = after - 'A';
    ++adj[before - 'A'][0];
    ++degree[after - 'A'];
  }

  bool visited[V] = {false};
  int i;
  int cnt = 0;

  while( cnt != V ) {
    for( i = 0; i < V; i++ ) {
      if( degree[i] == 0 && !visited[i] ) {
        printf( "%c", i + 'A' );
        visited[i] = true;
        ++cnt;
        break;
      }
    }
    for( int j = 0; j < adj[i][0]; ++j ) {
      int v = adj[i][j + 1];
      --degree[v];
    }
  }

  printf( "\n" );
  auto end = high_resolution_clock::now();
  printf( "%f\n", duration_cast<nanoseconds>( end - begin ).count() / 1000.0 / 1000.0 );
}
