# 改進實作檢查清單

根據程式碼分析報告，以下是建議的改進項目檢查清單。

---

## 🔴 緊急修正（立即處理）

### 作業三：記憶體洩漏修正

- [ ] **修正 CircularList 解構子**
  ```cpp
  // 檔案：homework3/src/main.cpp
  // 行數：約 419
  ~CircularList() { clear(); }  // 加入 clear() 呼叫
  ```

- [ ] **修正 `_narrow()` 方法**
  ```cpp
  // 檔案：homework3/src/main.cpp
  // 行數：約 633-649
  // 將 delete 和賦值操作移出迴圈
  ```

- [ ] **移除所有混淆定義**
  ```cpp
  // 檔案：homework3/src/main.cpp
  // 行數：約 17-19
  // 刪除：
  // #define eIse if
  // #define ιf else
  ```

### 驗證方式
```bash
# 使用 Valgrind 檢測記憶體洩漏
g++ -g homework3/src/main.cpp -o hw3
valgrind --leak-check=full ./hw3 < test_input.txt
```

---

## 🟡 重要改進（逐步實作）

### 作業一：Ackermann 函數

- [ ] **加入輸入驗證**
  ```cpp
  // 檔案：homework1/src/problem1/main.cpp
  int A_nr(int m, int n) {
      if (m < 0 || n < 0) {
          throw std::invalid_argument("Parameters must be non-negative");
      }
      if (m > 4 || (m == 4 && n > 1)) {
          throw std::overflow_error("Result would overflow");
      }
      // ... 原有程式碼
  }
  ```

- [ ] **使用動態陣列**
  ```cpp
  // 將 int array[MAX_STACK_NUM]; 改為
  std::vector<int> stack;
  stack.reserve(10000);
  ```

### 作業一：冪集合

- [ ] **簡化 Node 類別記憶體管理**
  ```cpp
  // 考慮使用 std::variant 或 std::unique_ptr
  union _data {
      std::unique_ptr<TheSet> set;
      std::unique_ptr<string> value;
  } data;
  ```

- [ ] **優化 getPowerSet 實作**
  ```cpp
  // 改用迭代替代遞迴
  // 使用位移運算替代 pow()
  ```

### 作業二：多項式運算

- [ ] **簡化輸入解析器**
  ```cpp
  // 考慮使用正規表達式
  #include <regex>
  std::regex term_regex(R"([+-]?\s*\d*\.?\d*\s*x?(\^[+-]?\d+)?)");
  ```

- [ ] **改善排序方法**
  ```cpp
  void _sort() {
      if (this->is_sorted || terms <= 1) return;
      std::sort(termArray, termArray + terms, 
          [](const Term& a, const Term& b) { return a.exp < b.exp; });
      this->is_sorted = true;
      _narrow();
      _dump();
  }
  ```

- [ ] **加入常數正確性**
  ```cpp
  bool iszero() const;  // 改為 const
  size_t length() const { return terms; }  // 新增
  ```

### 作業三：多項式運算

- [ ] **優化乘法實作**
  ```cpp
  // 降低時間複雜度從 O(n²m) 到 O(nm)
  Polynomial operator*(const Polynomial& that) {
      // 使用 += 運算子實作
  }
  ```

---

## 🟢 持續優化（長期改善）

### 現代 C++ 特性

- [ ] **使用 auto 關鍵字**
  ```cpp
  // 原本：
  CircularList<Term>::iterator itr = nodes->begin();
  // 改為：
  auto itr = nodes->begin();
  ```

- [ ] **使用範圍 for 迴圈**
  ```cpp
  // 原本：
  for (size_t i = 0; i < nodes->size(); ++i) {
      // 使用 nodes[i]
  }
  // 改為：
  for (auto& term : *nodes) {
      // 直接使用 term
  }
  ```

- [ ] **使用智能指標**
  ```cpp
  // 所有原始指標改為：
  std::unique_ptr<T>  // 獨佔所有權
  std::shared_ptr<T>  // 共享所有權
  ```

- [ ] **加入移動語意**
  ```cpp
  // 移動建構子
  Polynomial(Polynomial&& other) noexcept;
  // 移動賦值
  Polynomial& operator=(Polynomial&& other) noexcept;
  ```

### 錯誤處理

- [ ] **統一使用標準異常**
  ```cpp
  #include <stdexcept>
  throw std::invalid_argument("...");
  throw std::out_of_range("...");
  throw std::overflow_error("...");
  ```

- [ ] **加入異常安全保證**
  - 基本保證：不洩漏資源
  - 強保證：操作失敗時恢復原狀
  - 不拋出保證：標記為 noexcept

### 測試框架

- [ ] **引入 Google Test**
  ```cpp
  #include <gtest/gtest.h>
  
  TEST(AckermannTest, BasicCases) {
      EXPECT_EQ(A(0, 0), 1);
      EXPECT_EQ(A(1, 2), 4);
      EXPECT_EQ(A(2, 3), 9);
  }
  ```

- [ ] **建立測試案例**
  - 正常輸入測試
  - 邊界條件測試
  - 錯誤輸入測試
  - 效能測試

### 建置系統

- [ ] **建立 CMakeLists.txt**
  ```cmake
  cmake_minimum_required(VERSION 3.10)
  project(NFU-DataStructure)
  
  set(CMAKE_CXX_STANDARD 17)
  
  add_executable(hw1_p1 homework1/src/problem1/main.cpp)
  add_executable(hw1_p2 homework1/src/problem2/main.cpp)
  # ...
  ```

---

## 📊 進度追蹤

### 作業一
- [ ] 緊急修正 (0/0)
- [ ] 重要改進 (0/3)
- [ ] 持續優化 (0/4)

### 作業二  
- [ ] 緊急修正 (0/0)
- [ ] 重要改進 (0/3)
- [ ] 持續優化 (0/4)

### 作業三
- [ ] 緊急修正 (0/3) ⚠️
- [ ] 重要改進 (0/1)
- [ ] 持續優化 (0/4)

### 共通改善
- [ ] 現代 C++ (0/4)
- [ ] 錯誤處理 (0/3)
- [ ] 測試框架 (0/2)
- [ ] 建置系統 (0/1)

---

## 🎯 里程碑

### 第一週：緊急修正
- [ ] 修正所有記憶體洩漏
- [ ] 移除混淆定義
- [ ] 通過 Valgrind 檢查

### 第二週：基礎改善
- [ ] 加入輸入驗證
- [ ] 改用動態陣列
- [ ] 加入錯誤處理

### 第一個月：重要功能
- [ ] 簡化複雜程式碼
- [ ] 優化演算法效能
- [ ] 加入常數正確性

### 第二個月：現代化
- [ ] 使用智能指標
- [ ] 加入移動語意
- [ ] 引入測試框架

### 第三個月：完善專案
- [ ] 建立 CMake 建置系統
- [ ] 完整的單元測試
- [ ] 文檔更新

---

## 📝 驗證方式

### 記憶體檢查
```bash
# Valgrind
valgrind --leak-check=full --show-leak-kinds=all ./program

# Address Sanitizer
g++ -fsanitize=address -g program.cpp -o program
./program
```

### 編譯警告
```bash
# 開啟所有警告
g++ -Wall -Wextra -Wpedantic -std=c++17 program.cpp

# 更嚴格的檢查
g++ -Wall -Wextra -Werror -std=c++17 program.cpp
```

### 靜態分析
```bash
# Clang-Tidy
clang-tidy program.cpp -- -std=c++17

# Cppcheck
cppcheck --enable=all program.cpp
```

### 單元測試
```bash
# Google Test
./run_tests
```

---

## 📚 學習資源

### 記憶體管理
- [C++ Smart Pointers](https://en.cppreference.com/w/cpp/memory)
- [RAII](https://en.cppreference.com/w/cpp/language/raii)

### 現代 C++
- [Effective Modern C++](https://www.oreilly.com/library/view/effective-modern-c/9781491908419/)
- [C++17 Features](https://en.cppreference.com/w/cpp/17)

### 測試
- [Google Test Tutorial](https://google.github.io/googletest/)
- [Catch2 Tutorial](https://github.com/catchorg/Catch2)

### 工具
- [Valgrind Manual](https://valgrind.org/docs/manual/manual.html)
- [CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/)

---

**建議：** 從緊急修正開始，逐步完成每個項目。每完成一個里程碑，記得在此檢查清單上打勾！

**更新日期：** 2026-01-07
