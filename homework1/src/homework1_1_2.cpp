#include <iostream>
using namespace std;
int data(int m, int n) {
    int nfu[100000]; 
    int top = -1; 
    top++;
    nfu[top] = m;
    while (top >= 0) {
    int cur_m = nfu[top];
        top--;
        if (cur_m == 0) {
            n = n + 1;
        } 
        else if (n == 0) {
            top++;
            nfu[top] = cur_m - 1; 
            n = 1;
        } 
        else {
            top++;
            nfu[top] = cur_m - 1;
            top++;
            nfu[top] = cur_m;  
            n = n - 1;
        }
    }
    return n;
}
int main() {
    int m, n;
    cout << "請輸入 m 和 n: ";
    cin >> m >> n;
    cout << "計算結果 (非遞迴): " << data(m, n) << endl;
    return 0;
}
