# 41443139

**作業2_1**

## 解題說明

本題要求利用物件導向程式設計（OOP）概念，實作一個能處理單變數多項式的類別 Polynomial。多項式以非零項的形式動態儲存在記憶體中，並實作多項式加法、乘法、代入數值求值，以及輸入／輸出運算子重載等完整操作。
### 解題策略
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

以下為此次作業的完整 C++ 程式碼：

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
        if (c == 0.0f || e < 0) return;

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
            } else {
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
            } else if (termArray[i].exp < poly.termArray[j].exp) {
                ans.NewTerm(poly.termArray[j].coef, poly.termArray[j].exp);
                j++;
            } else {
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

        if (e < 0) {
            cout << "【警告】次方不可為負數 (" << e << ")，已忽略該項！" << endl;
            continue;
        }

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
            } else {
                os << " - ";
                c = -c;
            }
        } else {
            if (c < 0) {
                os << "-";
                c = -c;
            }
        }

        if (e == 0) {
            os << c;
        } else if (e == 1) {
            if (c == 1.0f) os << "x";
            else os << c << "x";
        } else {
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
        } else if (choice == 2) {
            cout << "A(x) = " << A << endl;
            cout << "B(x) = " << B << endl;
            Polynomial sum = A.Add(B);
            cout << "A + B = " << sum << endl;
        } else if (choice == 3) {
            cout << "A(x) = " << A << endl;
            cout << "B(x) = " << B << endl;
            Polynomial prod = A.Mult(B);
            cout << "A * B = " << prod << endl;
        } else if (choice == 4) {
            float x;
            cout << "請輸入 x 的值: ";
            cin >> x;
            cout << "A(" << x << ") = " << A.Eval(x) << endl;
        } else if (choice == 5) {
            float x;
            cout << "請輸入 x 的值: ";
            cin >> x;
            cout << "B(" << x << ") = " << B.Eval(x) << endl;
        } else if (choice == 6) {
            cout << "請輸入多項式 A: ";
            cin >> A;
        } else if (choice == 7) {
            cout << "請輸入多項式 B: ";
            cin >> B;
        }
    }

    return 0;
}
```



## 效能分析

### 時間複雜度

設多項式 $A$ 包含 $m$ 個非零項，多項式 $B$ 包含 $n$ 個非零項：

- **加法運算 (`Add`)：** 內部採用雙指標同步走訪兩陣列，迴圈次數最多為 $m + n$ 次。每次呼叫 `NewTerm` 插入時為常數時間（因維持降冪接續插入），故加法時間複雜度為 **$O(m + n)$**。
- **乘法運算 (`Mult`)：** 雙層迴圈枚舉所有項次組合，共產生 $m \times n$ 次項。每次相乘結果呼叫 `NewTerm` 時進行搜尋與合併，最壞時間複雜度為 **$O(m^2 \cdot n^2)$**（若每項均需平移插入）或平均 **$O(m \cdot n \log(m \cdot n))$**。
- **代入求值 (`Eval`)：** 遍歷陣列各項並計算乘冪，時間複雜度為 **$O(m)$**。

### 空間複雜度

- **動態儲存空間：** 僅儲存係數不為 0 的有效項，空間開銷與項數呈線性正比，空間複雜度為 **$O(m + n)$**。
- **相較於固定陣列：** 傳統以「最高次方（Degree）」開固定大小陣列需要 $O(\text{Degree})$ 的空間；本實作採用非零項動態配置，面對稀疏多項式（如 $x^{1000} + 1$）極度節省記憶體空間。

---

## 測試與驗證

### 測試案例

| 測試案例 | 多項式 $A(x)$ | 多項式 $B(x)$ | 測試功能 | 預期輸出 | 實際輸出 |
|:---|:---|:---|:---|:---|:---|
| **測試一** | $3x^2 + 2x + 1$ | $2x^2 - 2x + 5$ | 相加 ($A + B$) | $5x^2 + 6$ | `5x^2 + 6` |
| **測試二** | $x + 1$ | $x - 1$ | 相乘 ($A \times B$) | $x^2 - 1$ | `x^2 - 1` |
| **測試三** | $2x^3 - 5x + 3$ | （無須） | $A(x)$ 代入 $x = 2$ | $2(8) - 5(2) + 3 = 9$ | `9` |
| **測試四** | $4x^4 - 2x^2$ | $-4x^4 + 2x^2$ | 完全相消 | $0$ | `0` |

### 編譯與執行指令

```shell
$ g++ -std=c++17 -o polynomial polynomial.cpp
$ ./polynomial
請輸入多項式 A:(項數/係數/次方 )3 3 2 2 1 1 0
請輸入多項式 B(項數/係數/次方 ): 3 2 2 -2 1 5 0

========操作選單========
1. 顯示 A、B
2. A + B
3. A * B
4. 計算 A(x)
5. 計算 B(x)
6. 重新輸入 A
7. 重新輸入 B
0. 結束程式
選擇: 2

A(x) = 3x^2 + 2x + 1
B(x) = 2x^2 - 2x + 5
A + B = 5x^2 + 6
```
## 結論

1. **功能完整且運算正確：** 程式能正確完成多項式的輸入解析、降冪整理、同類項合併以及運算子重載輸出，加法與乘法皆能準確消去係數為 0 的項目。
2. **動態配置與稀疏矩陣優勢：** 採用動態陣列搭配 `Term` 儲存非零項，不僅避開了陣列長度受限於次方大小的缺陷，也大幅降低了稀疏多項式的記憶體負擔。

---

## 申論及開發報告

### 多項式物件導向實作差異與踩坑心得

這次用 C++ 類別來刻多項式運算，真的比以前那種直接開大陣列的陽春寫法聰明太多了！不僅不用白白浪費一堆記憶體，程式碼管理起來也清爽很多。

#### 1. 記憶體管理：到底誰才是空間管理大師？

- **指數索引陣列（Array of Degree）：**  
  以前剛學的時候超偷懶，直接宣告一個超大陣列（比如 `int arr[10001]`），陣列格子號碼當次方、裡面塞係數。看起來很直覺對吧？但只要遇到像 $f(x) = 100x^{10000} + 1$ 這種只有兩項的「稀疏多項式」，中間將近一萬格全部都是 0，直接把記憶體白白浪費掉，超級冤枉。
- **動態項數陣列（Array of Terms + Dynamic Resizing）：**  
  這次我們聰明多了，用指標 `Term* termArray` 搭配動態記憶體配置（`new` / `delete[]`），只記錄真正有數字的非零項！而且容量滿了就自動翻倍（容量乘 2），空間不夠才長大，既不會浪費半點記憶體，也不用怕次方太大塞不下，彈性直接拉滿！

#### 2. 深層複製（Deep Copy）：沒寫好直接原地崩潰！

這題最危險的大坑就是指標！因為類別裡面有自己用 `new` 抓出來的動態記憶體，如果偷懶沒寫 **Copy Constructor** 和 **Assignment Operator (`operator=`)**，編譯器預設的淺層複製（Shallow Copy）就只是傻傻把指標位址抄一份過去。  
結果就是兩個物件同時抓著同一塊記憶體，等到程式結束解構子（Destructor）開始清垃圾時，同一個門牌被砸了兩次，直接跳出超級兇殘的 **Double Free Bug** 讓程式當場暴斃。乖乖把深層複製寫完整，自己複製一份全新的陣列，程式才能穩如泰山。

#### 3. 輸出格式化：細節魔鬼藏在印出來的樣子裡

數學上看起來很簡單的式子，要叫電腦漂亮印出來簡直是惡夢，一大堆邊界條件只要漏掉一個畫面就超尷尬：

- **正負號不要打架：** 第一項要是負的就印 `-`，但後面的項次要是正的就要接 ` + `，負的要接 ` - `，絕對不能印出 `+ -3x` 這種醜東西。
- **係數是 1 超尷尬：** 次方大於 0 的時候，如果係數剛好是 1，一定要把 1 藏起來印 `x^2`，印出 `1x^2` 看起來就很菜。
- **次方是 0 跟 1：** 一次方千萬不能印出 `^1`，常數項（次方是 0）更是連 `x` 都不該出現，只能乖乖留數字。
- **算到最後變空氣：** 要是兩坨式子相減剛好全部消光光，或者是空的多項式，一定要記得單獨印個 `0`，不然整個終端機螢幕一片空白，還以為程式死當了。

- **程式碼實作差異：**
  - **未降冪版本（Append Only）：**  
    新增項目時，僅檢查是否有同次項；若無同次項則直接將項目加到陣列末尾（`terms++`）。
    ```cpp
    termArray[terms].coef = c;
    termArray[terms].exp = e;
    terms++;
    ```
  - **有降冪版本（Insertion Sort）：**  
    新項目放入末端後，即刻使用插入排序法由後向前比對次方，若發現前方次項較小則進行交換，直到維持嚴格降冪排列。
    ```cpp
    termArray[terms].coef = c;
    termArray[terms].exp = e;
    terms++;

    for (int i = terms - 1; i > 0; i--) {
        if (termArray[i].exp > termArray[i - 1].exp) {
            Term temp = termArray[i];
            termArray[i] = termArray[i - 1];
            termArray[i - 1] = temp;
        } else {
            break;
        }
    }
    ```

- **執行結果與影響比較：**

| 比較項目 | 未降冪版本（未排序） | 有降冪版本（插入排序） |
|:---|:---|:---|
| **測試輸入** | 3 項：`1 0`（常數項）、`3 2`（二次項）、`2 1`（一次項） | 3 項：`1 0`、`3 2`、`2 1` |
| **輸出結果** | `1 + 3x^2 + 2x` | `3x^2 + 2x + 1` |
| **對加法 (`Add`) 影響** | **運算錯誤**。雙指標演算法預設兩多項式皆為降冪，未排序會導致同次項無法正確對齊相加。 | **運算正確**。雙指標以 $O(m+n)$ 線性平順走訪並正確合併同次項。 |
| **閱讀直觀度** | 不符合數學常規書寫習慣 | 嚴格依降冪排列 |

#### 5. 心得總結
一開始我其實沒想那麼多，只想著把多項式的數字存進去就好，結果隨便亂輸幾次才發現，印出來的次方東倒西歪完全沒降冪，甚至連寫好的加法都直接算錯。後來加了一小段插入排序把它修好。另外就是指標跟動態記憶體真的要很小心。雖然中間除錯找了一陣子，但看到最後所有加法、乘法跟美化輸出都跑順了，真的很有成就感！
