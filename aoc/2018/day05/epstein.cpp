#include <chrono>
#include <cstdlib>
#include <cstdio>

#ifndef C_STYLE
#include <stack>

using std::stack;
#endif
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::nanoseconds;

#include "epstein.h"

#ifdef X_INLINE
inline bool reactive( char a, char b ) {
#else
bool reactive(char a, char b) {
#endif
 return a - b == 32 || a - b == -32;
}

int main() {
 auto begin = high_resolution_clock::now();

#ifdef C_STYLE
 char s[11678];
 size_t stack_len = 0;
#else
 stack<char> s;
#endif 

 for( int i = 0; i < read; ++i ) {
#ifdef C_STYLE
   if( reactive( s[stack_len-1], line[i] ) ) {
     stack_len --;
   } else {
     s[stack_len++] = line[i];
   }
#else
   if( reactive( s.top(), line[i] ) ) {
     s.pop();
   } else {
     s.push( line[i] );
   }
#endif
 }

 auto end = high_resolution_clock::now();
#ifdef C_STYLE
 printf( "%lu\n", stack_len);
#else
 printf( "%lu\n", s.size());
#endif

 printf( "%f\n", duration_cast<nanoseconds>( end - begin ).count() / 1000.0 / 1000.0 );
}
