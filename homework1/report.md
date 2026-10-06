# 41443139

作業一

## 解題說明

本題要求實作阿克曼函數（Ackermann's function），計算 $A(m, n)$ 之值。為了深入探討演算法機制，本作業分別採用**遞迴（Recursive）**與**非遞迴（Non-recursive）**兩種方式進行實作與比較。

### 阿克曼函數數學定義

阿克曼函數定義如下：

$$A(m, n) =  \begin{cases}  n + 1 & \text{if } m = 0 \\  A(m - 1, 1) & \text{if } n = 0 \\  A(m - 1, A(m, n - 1)) & \text{otherwise}  \end{cases}$$

### 解題策略

1. **遞迴版本：**
   - 直接將數學定義轉化為條件判斷式。
   - 透過系統隱式呼叫堆疊（Implicit Call Stack）自動處理雙重遞迴 $A(m - 1, A(m, n - 1))$ 的運算順序。
2. **非遞迴版本：**
   - 使用一維陣列 `nfu` 與指標 `top` 自訂顯式堆疊（Explicit Stack）模擬遞迴過程。
   - 透過 `while (top >= 0)` 迴圈處理狀態轉移：
     - 當 $cur\_m = 0$ 時：代表達到基底條件，將 $n$ 加 1（`n = n + 1`）。
     - 當 $n = 0$ 時：對應 $A(m-1, 1)$，更新堆疊頂端為 $cur\_m - 1$，並設 $n = 1$。
     - 其餘情況：對應 $A(m-1, A(m, n-1))$，依序將外層任務 $cur\_m - 1$ 與內層任務 $cur\_m$ 推入堆疊，並將 $n$ 減 1（`n = n - 1`）。

---

## 程式實作

以下為包含「遞迴」與「非遞迴」兩版本的完整 C++ 程式碼：

```cpp
#include <iostream>
using namespace std;

// 1. 遞迴版本
int ackermann_recursive(int m, int n) {
    if (m == 0)
        return n + 1;
    else if (n == 0)
        return ackermann_recursive(m - 1, 1);
    else
        return ackermann_recursive(m - 1, ackermann_recursive(m, n - 1));
}

// 2. 非遞迴版本 (使用自訂 Stack)
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
    cout << "計算結果 (遞迴版):   " << ackermann_recursive(m, n) << endl;
    cout << "計算結果 (非遞迴版): " << data(m, n) << endl;
    return 0;
}
