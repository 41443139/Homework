#include <iostream>
#include <string>
using namespace std;
void nfu(char S[], int n, int index, string current) {
if (index == n) {
cout << "(" << current << ")" << endl;
 return;
}
nfu(S, n, index + 1, current);
nfu(S, n, index + 1, current + S[index]);
}
int main() {
char S[] = { 'a', 'b', 'c' };
int n = 3;
cout << "nfu(S):" << endl;
nfu(S, n, 0, "");
return 0;
}
