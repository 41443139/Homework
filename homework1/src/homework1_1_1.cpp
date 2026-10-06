#include <iostream>//遞迴
using namespace std;
int nfu(int m, int n) {
if (m == 0) {
return n + 1;
}
if (n == 0) {
return nfu(m - 1, 1);
}
return nfu(m - 1, nfu(m, n - 1));
}

int main() {
int m, n;
cout << "輸入 m 和 n: ";
cin >> m >> n;
cout << "結果: " << nfu(m, n) << endl
return 0;
}
