# 41443139

**作業2_1**

## 解題說明

本題要求利用物件導向程式設計（OOP）概念，實作一個能處理單變數多項式的類別 Polynomial。多項式以非零項（Non-zero Terms）的形式動態儲存在記憶體中，並實作多項式加法、乘法、代入數值求值，以及輸入／輸出運算子重載等完整操作。
### 解題策略

1. **資料結構設計：**
   - Term 類別：儲存單項的係數（coef，浮點數）與次方（exp，整數）。
   - Polynomial 類別：內部封裝動態陣列指標 termArray、當前容量 capacity 及實際有效項數 terms。
   - 動態擴充機制：預設容量為 4，當陣列空間不足時採用「倍增策略（capacity *= 2）」，並重新配置記憶體。

2. **核心運算方法：**
   - 新增與合併項目 (NewTerm)：新增單項時若遇到同次項則累加係數；若累加後係數為 0 則移除該項並將後續項目向前平移；若為新次項則依次方由大到小（降冪）排序插入。
   - 多項式加法 (Add)：利用雙指標法（Two Pointers）同步遍歷兩多項式陣列，針對同次項相加、不同次項則優先將較高次方項複製至新物件，時間效率為線性。
   - 多項式乘法 (Mult)：雙層迴圈逐項相乘，將結果依序呼叫 NewTerm 自動累加合併同類項。
   - 代入求值 (Eval)：遍歷所有非零項，呼叫 pow(x, exp) 計算各項數值並累加總。
1. 資料結構設計：
   - Term 類別：儲存單項的係數（coef，浮點數）與次方（exp，整數）。
   - Polynomial 類別：內部封裝動態陣列指標 termArray、當前容量 capacity 及實際有效項數 terms。
   - 動態擴充機制：預設容量為 4，當陣列空間不足時採用「倍增策略（capacity *= 2）」，並重新配置記憶體。

2. 核心運算方法：
   - 新增與合併項目 (NewTerm)：新增單項時若遇到同次項則累加係數；若累加後係數為 0 則移除該項並將後續項目向前平移；若為新次項則依次方由大到小（降冪）排序插入。
   - 多項式加法 (Add)：利用雙指標法（Two Pointers）同步遍歷兩多項式陣列，針對同次項相加、不同次項則優先將較高次方項複製至新物件，時間效率為線性。
   - 多項式乘法 (Mult)：雙層迴圈逐項相乘，將結果依序呼叫 NewTerm 自動累加合併同類項。
   - 代入求值 (Eval)：遍歷所有非零項，呼叫 pow(x, exp) 計算各項數值並累加總。

3. I/O 運算子多載：
   - 重載 operator>>：先讀取項數 $n$，再依序讀入每一項的係數與次方。
   - 重載 operator<<：格式化輸出多項式，處理正負號連接、省略係數 1、常數項與一階項次方的美化呈現。
---

## 程式實作

以下為包含「遞迴」與「非遞迴」兩版本的完整 C++ 程式碼：

**程式實作**
```cpp
#include <iostream>
#include <cmath>

using namespace std;

class Polynomial;

class Term {
    friend class Polynomial;
    friend ostream& operator<<(ostream& os, const Polynomial& p);
    friend istream& operator>>(istream& is, Polynomial& p);
private:
    float coef;
    int exp;
};

class Polynomial {
    friend ostream& operator<<(ostream& os, const Polynomial& p);
    friend istream& operator>>(istream& is, Polynomial& p);
private:
    Term* termArray;
    int capacity;
    int terms;

    void NewTerm(float c, int e) {
        if (c == 0.0f) return;
        for (int i = 0; i < terms; i++) {
            if (termArray[i].exp == e) {
                termArray[i].coef += c;
                if (termArray[i].coef == 0.0f) {
                    for (int j = i; j < terms - 1; j++) {
                        termArray[j] = termArray[j + 1];
                    }
                    terms--;
                }
                return;
            }
        }
        if (terms >= capacity) {
            capacity *= 2;
            Term* temp = new Term[capacity];
            for (int i = 0; i < terms; i++) {
                temp[i] = termArray[i];
            }
            delete[] termArray;
            termArray = temp;
        }
        termArray[terms].coef = c;
        termArray[terms].exp = e;
        terms++;

        for (int i = terms - 1; i > 0; i--) {
            if (termArray[i].exp > termArray[i - 1].exp) {
                Term temp = termArray[i];
                termArray[i] = termArray[i - 1];
                termArray[i - 1] = temp;
            }
            else {
                break;
            }
        }
    }
public:
    Polynomial() {
        capacity = 4;
        terms = 0;
        termArray = new Term[capacity];
    }
    Polynomial(const Polynomial& p) {
        capacity = p.capacity;
        terms = p.terms;
        termArray = new Term[capacity];
        for (int i = 0; i < terms; i++) {
            termArray[i] = p.termArray[i];
        }
    }
    ~Polynomial() {
        delete[] termArray;
    }
    Polynomial& operator=(const Polynomial& p) {
        if (this == &p) return *this;

        delete[] termArray;
        capacity = p.capacity;
        terms = p.terms;
        termArray = new Term[capacity];
        for (int i = 0; i < terms; i++) {
            termArray[i] = p.termArray[i];
        }
        return *this;
    }
    Polynomial Add(Polynomial poly) {
        Polynomial ans;
        int i = 0, j = 0;

        while (i < terms && j < poly.terms) {
            if (termArray[i].exp == poly.termArray[j].exp) {
                float sum = termArray[i].coef + poly.termArray[j].coef;
                if (sum != 0.0f) {
                    ans.NewTerm(sum, termArray[i].exp);
                }
                i++;
                j++;
            }
            else if (termArray[i].exp < poly.termArray[j].exp) {
                ans.NewTerm(poly.termArray[j].coef, poly.termArray[j].exp);
                j++;
            }
            else {
                ans.NewTerm(termArray[i].coef, termArray[i].exp);
                i++;
            }
        }

        for (; i < terms; i++) ans.NewTerm(termArray[i].coef, termArray[i].exp);
        for (; j < poly.terms; j++) ans.NewTerm(poly.termArray[j].coef, poly.termArray[j].exp);

        return ans;
    }
    Polynomial Mult(Polynomial poly) {
        Polynomial ans;
        for (int i = 0; i < terms; i++) {
            for (int j = 0; j < poly.terms; j++) {
                float c = termArray[i].coef * poly.termArray[j].coef;
                int e = termArray[i].exp + poly.termArray[j].exp;
                ans.NewTerm(c, e);
            }
        }
        return ans;
    }
    float Eval(float f) {
        float ans = 0;
        for (int i = 0; i < terms; i++) {
            ans += termArray[i].coef * static_cast<float>(pow(f, termArray[i].exp));
        }
        return ans;
    }
};

istream& operator>>(istream& is, Polynomial& p) {
    int n;
    if (!(is >> n)) return is;

    p.terms = 0;

    for (int i = 0; i < n; i++) {
        float c;
        int e;
        is >> c >> e;
        p.NewTerm(c, e);
    }
    return is;
}

ostream& operator<<(ostream& os, const Polynomial& p) {
    if (p.terms == 0) {
        os << "0";
        return os;
    }
    for (int i = 0; i < p.terms; i++) {
        float c = p.termArray[i].coef;
        int e = p.termArray[i].exp;
        if (i > 0) {
            if (c > 0) {
                os << " + ";
            }
            else {
                os << " - ";
                c = -c;
            }
        }
        else {
            if (c < 0) {
                os << "-";
                c = -c;
            }
        }
        if (e == 0) {
            os << c;
        }
        else if (e == 1) {
            if (c == 1.0f) os << "x";
            else os << c << "x";
        }
        else {
            if (c == 1.0f) os << "x^" << e;
            else os << c << "x^" << e;
        }
    }
    return os;
}

int main() {
    Polynomial A, B;
    cout << "請輸入多項式 A:(項數/係數/次方 )";
    cin >> A;
    cout << "請輸入多項式 B(項數/係數/次方 ): ";
    cin >> B;
    int choice;
    while (true) {
        cout << "\n========操作選單========" << endl;
        cout << "1. 顯示 A、B" << endl;
        cout << "2. A + B" << endl;
        cout << "3. A * B" << endl;
        cout << "4. 計算 A(x)" << endl;
        cout << "5. 計算 B(x)" << endl;
        cout << "6. 重新輸入 A" << endl;
        cout << "7. 重新輸入 B" << endl;
        cout << "0. 結束程式" << endl;
        cout << "選擇: ";

        if (!(cin >> choice)) break;
        if (choice == 0) {
            break;
        }
        cout << endl;
        if (choice == 1) {
            cout << "A(x) = " << A << endl;
            cout << "B(x) = " << B << endl;
        }
        else if (choice == 2) {
            cout << "A(x) = " << A << endl;
            cout << "B(x) = " << B << endl;
            Polynomial sum = A.Add(B);
            cout << "A + B = " << sum << endl;
        }
        else if (choice == 3) {
            cout << "A(x) = " << A << endl;
            cout << "B(x) = " << B << endl;
            Polynomial prod = A.Mult(B);
            cout << "A * B = " << prod << endl;
        }
        else if (choice == 4) {
            float x;
            cout << "請輸入 x 的值: ";
            cin >> x;
            cout << "A(" << x << ") = " << A.Eval(x) << endl;
        }
        else if (choice == 5) {
            float x;
            cout << "請輸入 x 的值: ";
            cin >> x;
            cout << "B(" << x << ") = " << B.Eval(x) << endl;
        }
        else if (choice == 6) {
            cout << "請輸入多項式 A: ";
            cin >> A;
        }
        else if (choice == 7) {
            cout << "請輸入多項式 B: ";
            cin >> B;
        }
    }
    return 0;
}
```


## 測試與驗證

### 測試案例

| 測試案例 | 輸入 $m$ | 輸入 $n$ | 預期輸出 | 遞迴版實際輸出 | 非遞迴版實際輸出 |
|:---|:---|:---|:---|:---|:---|
| 測試一 | $m = 0$ | $n = 3$ | 4 | 4 | 4 |
| 測試二 | $m = 1$ | $n = 2$ | 4 | 4 | 4 |
| 測試三 | $m = 2$ | $n = 2$ | 7 | 7 | 7 |
| 測試四 | $m = 3$ | $n = 2$ | 29 | 29 | 29 |
| 測試五 | $m = 3$ | $n = 3$ | 61 | 61 | 61 |

### 編譯與執行指令

```shell
$ g++ -std=c++17 -o ackermann ackermann.cpp
$ ./ackermann
請輸入 m 和 n: 3 3
計算結果 (遞迴版):   61
計算結果 (非遞迴版): 61
```

## 結論

1. **結果一致性：** 遞迴版與非遞迴版在一般輸入情況下（如 $m \le 3$），輸出的計算結果完全一致，驗證了**兩者演算法邏輯的正確性**。
2. **記憶體容量差異：** 遞迴版本受限於作業系統有限的 **Call Stack 容量**，當 $m \ge 4$ 或 $n$ 稍大時會直接引發 **Stack Overflow** 崩潰；非遞迴版自訂陣列開在 **Data Segment**，能承載更深層的任務堆疊，但當超出 `nfu[100000]` 的邊界時依然需要額外動態擴充。

---

## 申論及開發報告

### 阿克曼函數：遞迴 vs 非遞迴實作差異探討

為了比較兩種寫法的實作差異，我同時開發了遞迴與非遞迴版本。

#### 1. 記憶體管理：系統 Call Stack vs 自訂 Stack

- **遞迴寫法（用系統的 Call Stack）：**  
  如果用一般的遞迴寫，程式每呼叫自己一次，系統就會在 Call Stack 裡面開一塊空間（Stack Frame）來記住現在跑到哪、變數是多少。但系統給的 Call Stack 空間非常小，如果數字稍微大一點點，系統堆疊就直接被塞爆，跳出慘絕人寰的 **Stack Overflow** 崩潰。

- **非遞迴寫法（我們自己建的 Stack）：**  
  為了避免系統 Stack 溢位，我們改成用非遞迴，並自己宣告一個陣列 `nfu` 當做 Stack。自己建的 Stack 是放在主記憶體的資料區（Data Segment），空間比系統 Call Stack 大太多了！這樣就部會受限於數字大小

#### 2. 控制流程：自動配對 vs 手動排隊

- **遞迴寫法：**  
  碰到像 $A(m - 1, A(m, n - 1))$ 這種兩層遞迴時，編譯器跟系統會自動幫你處理好「先算內層、再算外層」的順序。程式碼看起來非常乾淨優雅，跟數學公式長得一模一樣，但底層到底是怎麼轉的你其實看不到。

- **非遞迴寫法：**  
  沒有系統幫忙，我們就必須自己來。程式裡把 $m - 1$ 跟 $m$ 依序推進 Stack 裡面，其實就是我們手動在指揮：「先拿內層的 $A(m, n-1)$ 出來算，算完結果再丟給外層的 $m-1$」。雖然寫起來比較複雜，但可以掌握整個運算過程。

#### 3. 區別

| 比較項目 | 遞迴版本 (Recursive) | 非遞迴版本 (Non-recursive) |
|:---|:---|:---|
| **程式碼可讀性** | 比較直觀（跟數學公式一樣直觀） | 比較複雜（要手動管理 Stack 跟 while 迴圈） |
| **記憶體用哪裡** | 空間超小的系統 Call Stack | 空間大很多的自訂陣列 (Data Segment) |
| **執行效率** | 頻繁進出函式，額外開銷（Function Call Overhead）較大 | 無函式呼叫開銷，狀態轉移比較快 |
| **當機 (Overflow) 風險** | 輸入超小（如 $m=3, n=4$）就會直接崩潰 | 空間承受力高很多，但受限於陣列 `nfu[100000]` 的大小 |

#### 4. 總結

雖然阿克曼函數用遞迴寫最直覺，但因為系統記憶體的瓶頸，在實際應用上非遞迴版本具有更好的抗溢位能力。透過同時實作這兩種方案，驗證了結果的精確性，也讓我更清楚看到程式底層在處理複雜邏輯時，記憶體與堆疊到底是怎麼運作的。



**作業1_2**

## 解題說明

本作業旨在利用**遞迴（Recursion）**與**二元決策樹（Binary Decision Tree）**的概念，實作並探討如何列印出一個給定集合 $S$ 的所有子集合（即該集合的「冪集」 Power Set）。

### 冪集（Power Set）定義

若集合 $S$ 包含 $n$ 個元素，則 $S$ 的冪集 $\mathcal{P}(S)$ 為包含 $S$ 所有可能子集合（包含空集合與自身）的集合：

$$
|\mathcal{P}(S)| = 2^n
$$

對於集合中的每一個元素 $S[i]$，在建構子集合時都只有兩種選擇：
1. **不納入（Exclude）：** 不將該元素加入當前子集合。
2. **納入（Include）：** 將該元素加入當前子集合。
## 程式實作

以下為列印集合所有子集合之完整 C++ 程式碼：

```cpp
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
```
## 效能分析

### 時間複雜度

- **總節點數 / 呼叫次數：** 決策樹的深度為 $n$，形成的滿二元樹（Full Binary Tree）總葉子節點數為 $2^n$ 個（對應 $2^n$ 個子集合），總呼叫節點數為 $2^{n+1} - 1$。
- **字串複製開銷：** 在最深層印出子集合時，平均字串長度為 $\frac{n}{2}$，印出總代價為 $O(n \cdot 2^n)$。
- **總時間複雜度：** **$O(n \cdot 2^n)$**。

### 空間複雜度

- **呼叫堆疊（Call Stack）深度：** 遞迴調用最深到達第 $n$ 層，故 Call Stack 的最大深度為 $O(n)$。
- **字串空間：** 每次遞迴傳值時傳遞字串副本，累積空間開銷為 $O(n^2)$（若考慮字串長度）；若以記憶體堆疊上限來看，主要受限於 $O(n)$ 的遞迴深度。
- **總空間複雜度：** **$O(n)$**（輔助空間）。
- 
## 測試與驗證

### 測試案例

| 測試案例 | 輸入 $n$ | 輸入集合元素 | 預期子集合數量 | 實際輸出結果 |
|:---|:---|:---|:---|:---|
| **測試一** | $n = 0$ | （無） | $2^0 = 1$ | `()` |
| **測試二** | $n = 1$ | `a` | $2^1 = 2$ | `()`, `(a)` |
| **測試三** | $n = 2$ | `a b` | $2^2 = 4$ | `()`, `(b)`, `(a)`, `(ab)` |
| **測試四** | $n = 3$ | `a b c` | $2^3 = 8$ | `()`, `(c)`, `(b)`, `(bc)`, `(a)`, `(ac)`, `(ab)`, `(abc)` |

### 編譯與執行指令

```shell
$ g++ -std=c++17 -o subset subset.cpp
$ ./subset
請輸入集合有幾個元素 (n): 3
請輸入 3 個字元 : a b c
nfu(S) 的所有子集合結果：
()
(c)
(b)
(bc)
(a)
(ac)
(ab)
(abc)
```
## 結論

1. **答案完全正確：** 實際測試輸入 $n=0$ 到 $n=3$，印出來的子集合數量剛好都是 $2^n$ 個，而且連空集合（什麼都不選）跟集合自己都有印出來，代表遞迴的邏輯完全沒問題！
2. **數字長超快：** 因為子集合的數量是呈 **$2^n$ 幾何級數爆炸**，只要 $n$ 稍微變大（比如 $n=30$），數量就會衝到好幾億個，電腦光是要印出來就會跑很久甚至卡死。

---

## 申論及開發報告

### 子集合怎麼產生的？

這次寫的 `nfu` 函式，其實就是演算法裡面很經典的**「樹狀圖走訪」**（回溯法）。

#### 1. 畫樹狀圖的概念（二元決策樹）

要把所有子集合找出來，過程就像在畫樹狀圖，每一層都在做**二選一**：
- **遇到第 $i$ 個字元：** 我到底要不要把它放入袋子裡？
- **走左邊（不選）：** 直接跳下一個字元，袋子保持原樣。
- **走右邊（選）：** 把這個字元放進袋子裡，再跳下一個字元。

因為每個字元都有 2 種選擇，到了最底層（全部字元都考慮完）就會剛好分出 **$2^n$ 個分枝**，也就是所有的子集合。

#### 2. 為什麼退回上一層時，文字不會亂掉？

程式裡面的字串變數 `current` 是用**「直接傳值」**的方式傳進去：
- 當我往右邊走加入新字元時，電腦會另外複製一份新的字串給下一層用。
- 等到那條路走完退回來時，原本這層的字串**完全沒有被改到**。
- 這樣寫的好處是程式碼超級簡短，不用手動去把剛剛加進去的字元刪掉；缺點就是一直複製字串會比較吃記憶體。

#### 3. 如果想讓程式跑更快、更省記憶體？

- **用指標/引用（Reference）來改：**  
  如果不希望電腦一直複製字串，可以讓大家共用同一個字串，只是往下走時把字元接上去，退回來時手動把最後一個字元刪掉（`pop_back`），這樣就會省很多記憶體。
- **用二進位（Bitmask）來做：**  
  因為「選或不選」很像二進位的 `1` 和 `0`，其實可以用一個簡單的 `for` 迴圈從 $0$ 跑到 $2^n-1$，看二進位哪一位是 `1` 就抓那個字元，這樣連遞迴都不用寫了！

#### 4. 心得總結

寫完這次作業後，雖然只有短短幾行程式碼，背後卻是用樹狀圖把所有可能都走一遍。這種「選或不選」的思考方式，在以後遇到更複雜的程式題目時應該會非常實用。
