#include <iostream>
#include <string>
using namespace std;
void nfu(string S, int n, int index, string current) {
	if (index == n) {
cout << "(" << current << ")" << endl;
return;
}
nfu(S, n, index + 1, current);
nfu(S, n, index + 1, current + S[index]);
}
int main() {
int n;
cout << "請輸入集合有幾個元素 (n): ";
cin >> n;
string S = "";
cout << "請輸入 " << n << " 個字元 : ";
for (int i = 0; i < n; i++) {
char ch;
cin >> ch;
S += ch;
}
cout << "nfu(S) 的所有子集合結果：" << endl;
nfu(S, n, 0, "");
return 0;
}

