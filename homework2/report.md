# 41343131

作業二

## 解題說明

題目分別要求實現 `Polynomial`多項式類，以及C++方法來輸入及輸出多項式。並多載<<和>>運算子。

先觀察，可以注意到題目提供的多項是類資料結構有一個 `termArray` ，類型為 `Term` 的指標，還配有一個 `capacity` 用於存放 `termArray` 的大小，至於 `terms` 暫時不確定用途。題目描述(圖片)包含多項式的結構，以及建構子、`Add`、`Mult`及`Eval`方法。

在方法 `Add` 及 `Mult` 中，回傳類型為 `Polynomial` ，考量到有私有指標資料 `termArray` 因此應該需要複製建構子及=運算子多載進行指標資料複製。

在<<及>>運算子的實現中，需要決定字串輸入輸出格式，以及相對應分析算法。

一開始計畫每個多項式皆從 $x^0$ 也就是常數項開始，儘管其係數為0，犧牲部分空間以簡化算法。

原先打算做的方案退回，因為索引即指數在特定條件下造成嚴重浪費，故思考新方案(稀疏)。

稀疏方案加法沒甚麼問題，打算使用雙指針進行掃描(定義 `termArray` 皆為昇冪排列)相加放入暫存器(大小為 $n+m$ ，其中 $n$ 為多項式本身長度， $m$ 為加多項式之長度)。隨後從第一個開始分別針對this&that掃描，當相等直接相加；若其一較大則只處理小的，小的指針+1；若相加為零則最終暫存器長度-1(定義一開始與 $n+m$ 相等)。執行完畢後創建新的暫存器，過濾所有為零項。

相較於加法，乘法需要考慮中間結果項的合併與排序。直接相乘會產生 $m*n$ 個項，效率應列入考量。暫時列出以下三種方案:

1. 直接相乘定儲存於暫存器(大小為 $m*n$ )，隨後經排序後收縮成最終結果。
2. 針對key:value，先抓取最小與最大指數項，創建陣列最大-最小。依序暴力相乘 $n*m$ 將結果加入到結果陣列。
3. 直接相乘定儲存於暫存器(大小為 $m*n$ )，透過n個指針從開頭開始依序歷經累加後放入最終結果。

上述方案各有優缺

| 方案 | 時間複雜度 | 優點 | 缺點 |
| - | - | - | - |
| 方案一 | $O(m*n\log(mn))$ | 實現簡單 | 複雜度較高 |
| 方案二 | $O(m*n)$ | 實現簡單 | 有範圍限制且空間可能浪費 |
| 方案三 | $O(n^2*m)$ | 多線操作 | 僅透過n指針操作效率不足 |

因此我決定結合上述方案並改善方案三，透過二元堆積(最小堆)進行操作使複雜度降為 $O(n*m\log(n))$。

二元堆積法，透過一個陣列定義其為一樹狀結構使每當n項則左右項index分別為 $2n+1$ 及 $2n+2$

## 程式實作

```cpp
// !! ATTENTION !! must using C++14 or above
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <string>
#include <cmath>

// #include <limits.h>
#define INT_MAX 2147483647
// float 允許誤差
#define EPS 1e-6f
// exponent
const wchar_t EXP_N[] = L"⁰¹²³⁴⁵⁶⁷⁸⁹";

using namespace std;

// swap
/*
template <typename T>
void swap(T &a, T &b)
{
    T c = a;
    a = b;
    b = c;
}
*/

// std::bit_ceil    // #include <bit>
unsigned int __bit_ceil(unsigned int n)
{
    if (!n)
        return 1;
    --n;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    // n |= n >> 32;
    ++n;
    return n;
};

// std::isdigit     // #include <ctype.h>
bool isdigit(char c)
{
    return (c >= '0' && c <= '9');
}

// is_zero  // fixed float bug by using a range limiting
bool iszero(float f)
{
    return (fabs(f) < EPS);
}

// float equal
bool float_equal(float a, float b)
{
    return fabs(a - b) < EPS;
}

// float(double) to string pro version  // (#by @ChatGPT)
string ftos_pro(double f, int precision = 2)
{
    std::ostringstream oss;
    if (fabs(f - round(f)) < EPS)
    {
        oss << (long long)round(f);
    }
    else
    {
        oss.setf(ios::fixed);
        oss.precision(precision);
        oss << f;
    }

    return oss.str();
}

/*
template <typename T>
class ArrayElf {
public:
    ArrayElf(T *ptr = nullptr, size_t size_ = 0) : array(ptr), size(size_) {};
    ~ArrayElf() {
        delete[] array;
    };
    T *array;
    size_t size;
};
class StringArray {
public:
    static StringArray* string_split(const string &string) {};
    StringArray() {};
    ~StringArray() {};
private:
    string* _data;
    size_t _size;
};
*/

class Polynomial;
class Term
{
    friend Polynomial;
    friend struct term_isExpLess;
    friend bool term_isExpLess(const Term &a, const Term &b);
    // friend ostream &operator<<(const ostream &output, const Term term);
    friend istream &operator>>(istream &input, Polynomial &poly);

    // #DEBUG
    friend ostream &operator<<(ostream &output, const Polynomial poly);

public:
    Term(float coef_ = 0.0f, int exp_ = 0) : coef(coef_), exp(exp_) {};
    Term operator=(const Term &that) {
        this->coef = that.coef;
        this->exp = that.exp;
        return *this;
    };

    // only add the coef, set exp as that
    void c_add(const Term &that)
    {
        this->exp = that.exp;
        this->coef += that.coef;
    };
    // void e_add(const Term& that) {
    //     this->exp += that.exp;
    //     this->coef = that.coef;
    // };

    string to_string(bool with_posSign = false) const
    {
        // cout << "term.coef" << coef << ", " << "term.exp" << exp << endl; // DEBUG
        string str = "";
        if (iszero())
            return "0";
        // if (this->coef < 0)
        //     str += '-';
        // else if (with_posSign)
        if (this->coef > 0 || float_equal(this->coef, 0))
        {
            if (with_posSign)
                str += '+';
        }
        else
        {
            str += '-';
        }
        if ((!float_equal(fabs(this->coef), 1)) || !this->exp)
            str += ftos_pro(fabs(this->coef));
        if (!this->exp)
            return str;
        str += 'x';
        if (this->exp != 1)
        {
            str += '^';
            str += std::to_string(this->exp);
        }
        if (str == "")
            return "0";
        // str += " ";
        return str;
    };

    int get_exp() const
    {
        return this->exp;
    };
    float get_coef() const
    {
        return this->coef;
    };
    bool iszero() const
    {
        return ::iszero(this->coef);
    };

    Term operator*(const Term &that)
    {
        return Term(this->coef * that.coef, this->exp + that.exp);
    };

private:
    float coef; // coefficient
    int exp;    // exponent
};
// Term exp less than CMP-function
// struct term_isExpLess
// {
//     bool operator()(const Term &a, const Term &b)
//     {
//         return (a.exp < b.exp);
//     };
// };
bool term_isExpLess(const Term &a, const Term &b)
{
    return (a.exp < b.exp);
};

// CMP should be a compare-function bool(T a, T b)
template <typename T>
class Heap
{
public:
    using CMP = bool (*)(const T &, const T &);
    Heap(size_t capacity_, CMP comp = nullptr) : _size(0), _capacity(__bit_ceil(capacity_)), _comparator(comp)
    {
        heapArray = new T[_capacity];
        if (!_comparator)
        {
            _comparator = [](const T &a, const T &b)
            { return a.get_exp() < b.get_exp(); };
        }
    };
    ~Heap()
    {
        delete[] heapArray;
    };

    // build the Heap with array
    void buildHeap(T *array_, size_t size_)
    {
        if (this->_capacity < size_)
        {
            _resize(size_);
        }
        this->_size = size_;
        copy(array_, array_ + size_, this->heapArray);
        for (int i = (size_ / 2) - 1; i >= 0; --i)
        {
            _heapify_down(i);
        }
    };

    bool isEmpty()
    {
        return !_size;
    };
    const T &top() const
    {
        return heapArray[0];
    };

    // get an obj back, AND NOT replace a new
    T extract()
    {
        if (!_size)
        {
            throw std::out_of_range("Heap is empty.");
        }
        T old_value = heapArray[0];
        heapArray[0] = heapArray[--_size];
        if (_size > 1)
        {
            _heapify_down(0);
        }
        return old_value;
    };

    // get an obj back, AND replace a new
    T replace(const T &value)
    {
        if (!_size)
        {
            throw std::out_of_range("Heap is empty.");
        }
        T old_value = heapArray[0];
        heapArray[0] = value;
        if (_size > 1)
        {
            _heapify_down(0);
        }
        return old_value;
    };

private:
    void _resize(size_t capacity_)
    {
        // #TODO have to fix when "reducing capacity"

        size_t new_capacity = __bit_ceil(capacity_);
        T *temp = heapArray;
        heapArray = new T[new_capacity];
        copy(temp, temp + _size, heapArray);
        delete[] temp;
        _capacity = new_capacity;
    };
    void _heapify_down(size_t index = 0)
    {
        size_t left = i_left(index);
        size_t right = i_right(index);
        size_t smallest = index; // pre

        // cmp left chil-node
        if ((left < _size) && _comparator(heapArray[left], heapArray[smallest]))
        {
            smallest = left;
        }

        // cmp right chil-node
        if ((right < _size) && _comparator(heapArray[right], heapArray[smallest]))
        {
            smallest = right;
        }

        // cont
        if (smallest != index)
        {
            swap(heapArray[index], heapArray[smallest]);
            _heapify_down(smallest);
        }
    };
    size_t i_parent(size_t i) { return (i - 1) / 2; };
    size_t i_left(size_t i) { return 2 * i + 1; };
    size_t i_right(size_t i) { return 2 * i + 2; };

    T *heapArray;
    size_t _size;
    size_t _capacity;
    CMP _comparator;
};

// #TODO add a operator&&()
//
class Polynomial
{
    friend istream &operator>>(istream &input, Polynomial &poly);
    friend ostream &operator<<(ostream &output, Polynomial poly);

public:
    // Construct the polynomial p(x) = 0.
    Polynomial(int length = 1) : capacity(__bit_ceil(length)), terms(0)
    {
        this->is_sorted = true;
        termArray = new Term[capacity];
    };

    // Copy constructor
    Polynomial(const Polynomial &that) : Polynomial(that.capacity)
    {
        this->terms = that.terms;
        this->is_sorted = that.is_sorted;
        for (int i = 0; i < that.terms; ++i)
        {
            this->termArray[i] = that.termArray[i];
        }
    };

    // Destructor
    ~Polynomial()
    {
        delete[] termArray;
    };

    // Return the sum of the polynomials *this and poly.
    Polynomial Add(const Polynomial that) const
    {
        Polynomial poly_this(*this), poly_that(that);
        poly_this._sort();
        poly_that._sort();
        int temp_capacity = poly_this.capacity + poly_that.capacity;
        if (!temp_capacity)
            return Polynomial();
        int this_ti = 0, that_ti = 0, main_ti = 0;
        Polynomial temp_poly(temp_capacity);
        bool avail_this = (this_ti < poly_this.terms);
        bool avail_that = (that_ti < poly_that.terms);
        for (
            ; avail_this || avail_that;
            avail_this = (this_ti < poly_this.terms),
            avail_that = (that_ti < poly_that.terms))
        {
            int this_exp = avail_this ? poly_this.termArray[this_ti].exp : INT_MAX;
            int that_exp = avail_that ? poly_that.termArray[that_ti].exp : INT_MAX;
            if (this_exp == that_exp)
            {
                const double temp_coef = poly_this.termArray[this_ti].coef + poly_that.termArray[that_ti].coef;
                ++this_ti;
                ++that_ti;
                if (float_equal(temp_coef, 0.0))
                    continue;
                temp_poly.termArray[main_ti].coef = temp_coef;
                temp_poly.termArray[main_ti].exp = this_exp;
            }
            else if (this_exp < that_exp)
            {
                if (poly_this.termArray[this_ti].iszero())
                {
                    ++this_ti;
                    continue;
                }
                temp_poly.termArray[main_ti].coef = poly_this.termArray[this_ti].coef;
                temp_poly.termArray[main_ti].exp = this_exp;
                ++this_ti;
            }
            else
            {
                if (poly_that.termArray[that_ti].iszero())
                {
                    ++that_ti;
                    continue;
                }
                temp_poly.termArray[main_ti].coef = poly_that.termArray[that_ti].coef;
                temp_poly.termArray[main_ti].exp = that_exp;
                ++that_ti;
            }
            temp_poly.terms = ++main_ti;
        }
        if (temp_capacity == main_ti)
            return temp_poly;
        Polynomial final_poly(main_ti);
        final_poly.terms = main_ti;
        copy(temp_poly.termArray, temp_poly.termArray + main_ti, final_poly.termArray);
        return final_poly;
    };

    // Return the product of the polynomials *this and poly.
    Polynomial Mult(const Polynomial that) const
    {
        Polynomial poly_this(*this), poly_that(that);
        if (poly_this.iszero() || poly_that.iszero())
            return Polynomial();
        if ((poly_this.terms == 1) && (poly_that.terms == 1))
        {
            Polynomial poly;
            poly.term_append(poly_this.termArray[0] * poly_that.termArray[0]);
            return poly;
        }
        poly_this._sort();
        poly_that._sort();

        Polynomial *poly_cheaper = &poly_this, *poly_greater = &poly_that;
        if (poly_cheaper->terms > poly_greater->terms)
            swap(poly_cheaper, poly_greater);
        int sml_terms = poly_cheaper->terms;

        Polynomial poly_temp(sml_terms + poly_greater->terms); // min

        // initial a Heap (prepare)
        Heap<Term> heap(sml_terms, term_isExpLess);
        Term *terray = new Term[sml_terms]; // very temp using
        for (size_t i = 0; i < sml_terms; ++i)
        {
            terray[i] = poly_cheaper->termArray[i] * poly_greater->termArray[0];
        }
        heap.buildHeap(terray, sml_terms);
        delete[] terray;

        // start multiing
        size_t c_i = -1, g_i = 1;
        Term temp; // where is the first term?? #TODO
        do
        {
            Term t_term;
            do
            {
                // add
                t_term.c_add(temp);
                // 抓
                if (!(++c_i < sml_terms))
                {
                    c_i = 0;
                    if (!(g_i < poly_greater->terms))
                    {
                        c_i = sml_terms;
                        // g_i
                    }
                    else
                        ++g_i;
                };
                // if (!(g_i < poly_greater->terms))
                // cout << "\nc_i: " << c_i << "\ng_i: " << g_i << endl;
                temp = ((g_i < poly_greater->terms) || (c_i < sml_terms))
                           ? heap.replace(poly_cheaper->termArray[c_i] * poly_greater->termArray[g_i])
                           : heap.extract();
                // cout << "\nc_i: " << c_i << endl;
            } while (t_term.exp == temp.exp);
            if (!t_term.iszero())
                poly_temp.term_append(t_term);
        } while (!heap.isEmpty()); // where is the last term?? #TODO (when Empty, it still at "temp")
        poly_temp.term_append(temp);

        // debug
        // cout <<"debug:\n" << poly_temp << endl;
        // #was-bug bc an unknown reason; but it auto disappeared....??

        return poly_temp;
    };

    // Evaluate the polynomial *this at f and return the result.
    float Eval(float f) const
            // oh yeah these was an evil
    {
        float temp = 0;
        for (int i = 0; i < terms; ++i)
        {
            // if (float_equal(termArray[i].coef, 0.0)) continue;
            temp += termArray[i].coef * pow(f, termArray[i].exp);
        }
        return temp;
    };
    bool iszero()
    {
        _sort();
        return (!terms || ((terms <= 1) && termArray[0].iszero()));
    };
    /*
    bool iszero() const
    // shit method
    {
        Polynomial poly_this(*this);
        return poly_this.iszero();
        / *  // cause bug (when 2x-2x)
        for (int i = 0; i < terms; ++i) {
            if (!termArray[i].iszero())
            return false;
            else
            continue;
        }
        return true;
        * /
    };
    */

    // add a term into the poly
    Polynomial &add(float coef, int exp)
    {
        return add(Term(coef, exp));
    };
    // add a term into the poly #TODO
    Polynomial &add(const Term &term)
    {
        // find the correct index by "_find"
        // if the index == terms
        //      do append and return
        // else do
        // cut to "2 paragraphs"
        // new mem
        // paste 1st paragraph
        // paste adding or merge with the last term of 1st paragraph
        // paste 2nd paragraph
        return *this;
    };

    // append a term at the end
    /*
        !#WARNING!! this method is different with add
        this method WILL "**append** a new term at the end"
        AND WILL NOT do any "sort" or "merging equal exp terms"
    */
    Polynomial &term_append(float coef, int exp)
    {
        return term_append(Term(coef, exp));
    };
    // append a term at the end
    /*
        !#WARNING!! this method is different with add
        this method WILL "**append** a new term at the end"
        AND WILL NOT do any "sort" or "merging equal exp terms"
    */
    Polynomial &term_append(const Term &term)
    {
        if ((terms > 0) && !(termArray[terms - 1].exp < term.exp))
            this->is_sorted = false;
        if (capacity == terms)
            _upgrade_capacity();
        termArray[terms] = term;
        ++terms;
        // this->debug();
        return *this;
    };

    //
    // Polynomial &term_set(int index, float coef, int exp) {};

    // operators
    Polynomial operator=(const Polynomial &that)
    {
        if (this->capacity < that.terms)
        {
            _upgrade_capacity(__bit_ceil(that.capacity / this->capacity));
        }
        this->terms = that.terms;
        this->is_sorted = that.is_sorted;
        for (int i = 0; i < terms; ++i)
        {
            this->termArray[i] = that.termArray[i];
        }
        return *this;
    };
    Polynomial operator+(const Polynomial &that) const
    {
        return this->Add(that);
    };
    Polynomial operator*(const Polynomial &that) const {
        Polynomial temp = that;
        return this->Mult(that);
    };

    float operator()(const float f) const {
        return Eval(f);
    };

    void debug()
    {
        cout << "-------- -------- --------" << endl;
        cout << "terms: " << this->terms << endl;
        cout << "capacity: " << this->capacity << endl;
        // cout << "terms: " << this->terms << endl;
        for (int i = 0; i < terms; ++i)
        {
            cout << termArray[i].to_string() << " ";
        }
        cout << endl;
        cout << "-------- -------- --------" << endl;
    };

private:
    void _upgrade_capacity(int multiplier = 2)
    {
        Term *temp_ptr = termArray;
        int tc = capacity;
        capacity *= multiplier;
        termArray = new Term[capacity];
        copy(temp_ptr, temp_ptr + tc, termArray);
        delete[] temp_ptr;
    };

    void _sort()
    {
        if (this->is_sorted)
            return;
        this->is_sorted = true;
        if (terms <= 1)
            return;
        // here is why have to use C++14
        auto q_sort = [&](auto &self, int left, int right) mutable -> void
        {
            if (left >= right)
                return;

            int pivot = termArray[left].exp;
            int i = left - 1;
            int j = right + 1;

            while (true)
            {
                do
                {
                    ++i;
                } while (termArray[i].exp < pivot);
                do
                {
                    --j;
                } while (termArray[j].exp > pivot);

                if (i >= j)
                    break;

                swap(termArray[i], termArray[j]);
            }
            self(self, left, j);
            self(self, j + 1, right);
        };
        q_sort(q_sort, 0, terms - 1);

        // narrow
        Term *temp = new Term[capacity];
        int new_terms = 0;
        for (int i = 0, j = 0; i < terms; i = j, ++new_terms)
        {
            temp[new_terms] = termArray[i];
            for (j = i + 1; (j < terms) && (termArray[i].exp == termArray[j].exp); ++j)
            {
                temp[new_terms].coef += termArray[j].coef;
                // ++new_terms;
            }
        }
        delete[] termArray;
        termArray = temp;
        terms = new_terms;

        // dump 0^0
        _dump();
    };

    void _dump(double coef = 0)
    {
        if (terms <= 0 || capacity <= 0)
            return;
        Term *temp = new Term[capacity];
        int new_terms = 0;
        for (int i = 0; i < terms; ++i)
        {
            if (!termArray[i].iszero()) // float_equal(termArray[i].coef, coef))) // 0?
            {
                temp[new_terms] = termArray[i];
                ++new_terms;
            }
        }
        delete[] termArray;
        termArray = temp;
        terms = new_terms;
    };

    // #TODO let "_find" finding the index which nearest but equals or greater than goal
    // binary search; # return index, -1 if not exists
    int _find(int exp)
    {
        int min_site = 0, max_site = terms - 1, mid;
        if (exp < termArray[min_site].exp || termArray[max_site].exp < exp)
            return -1;
        while (min_site <= max_site)
        {
            mid = (min_site + max_site) / 2;
            int mid_exp = termArray[mid].exp;
            if (exp > mid_exp)
            {
                min_site = mid + 1;
            }
            else if (exp < mid_exp)
            {
                max_site = mid - 1;
            }
            else
            {
                return mid;
            }
        }
        return -1;
    };

    Term *termArray; // array of nonzero terms
    bool is_sorted;
    int capacity; // size of termArray
    int terms;    // number of nonzero terms
};

// ostream &operator<<(ostream &output, const Term &term) {
//     string str = "";
// };
// #TODO make "2x^4+(2x^4+1)" supports
istream &operator>>(istream &input, Polynomial &poly)
{
    string coef_str = "0", exp_str = "0";
    int c = input.peek();
    // cout << "debug:" << (char)c << endl; // DEBUG
    for (
        int sign = 1,
            has_coef = 0;

        (c != EOF) && (c != '\n');

        c = input.peek(),
            sign = 0,
            has_coef = 0,
            coef_str = "0",
            exp_str = "0")
    {
        if (
            // unacceptable char
            c != ' ' &&
            c != '+' &&
            c != '-' &&
            !isdigit(c) &&
            c != '.' &&
            c != 'x')
        {
            input.setstate(std::ios::failbit);
            return input;
        }

        Term term;

        // remove space
        while (c == ' ')
        {
            input.get();
            c = input.peek();
        }

        // sign
        // [+-..][0~9..][x][^][0~9..]
        // #TODO must to make "+++" / "----"/ "+-+-" unacceptable
        while ((c == '+') || (c == '-'))
        {
            if (c == '-')
                (sign *= -1) || (sign = -1);
            else
                sign || (sign = 1);
            input.get();
            c = input.peek();

            // remove space
            while (c == ' ')
            {
                input.get();
                c = input.peek();
            }
        }

        // if no sign
        if (!sign)
        {
            input.setstate(std::ios::failbit);
            return input;
        }

        // coefficient
        // integer part
        while (isdigit(c))
        {
            has_coef = 1;
            coef_str += c;
            input.get();
            c = input.peek();
        }
        // decimals part
        if (c == '.')
        {
            has_coef = 1;
            coef_str += '.';
            input.get();
            c = input.peek();
            if (coef_str == "0." && !isdigit(c))
            {
                input.setstate(std::ios::failbit);
                return input;
            }
            while (isdigit(c))
            {
                coef_str += c;
                input.get();
                c = input.peek();
            }
        }
        if (has_coef)
            term.coef = std::stof(coef_str) * sign;
        else if (c == 'x')
            term.coef = sign;
        else
        {
            input.setstate(std::ios::failbit);
            return input;
        } // term.coef = 0;

        // x & its exponent
        if (c == 'x')
        {
            // if (float_equal(term.coef, 0))
            //     term.coef = sign;
            input.get();
            c = input.peek();

            // remove space
            while (c == ' ')
            {
                input.get();
                c = input.peek();
            }

            if (c == '^') // exp >1
            {
                // remove space
                while (c == ' ')
                {
                    input.get();
                    c = input.peek();
                }

                input.get();
                c = input.peek();
                sign = 1;
                while ((c == '+') || (c == '-')) // sign
                {
                    if (c == '-')
                        sign *= -1;
                    input.get();
                    c = input.peek();
                }

                // remove space
                while (c == ' ')
                {
                    input.get();
                    c = input.peek();
                }

                if (!isdigit(c))
                {
                    input.setstate(std::ios::failbit);
                    return input;
                }
                while (isdigit(c)) // exp getting
                {
                    exp_str += c;
                    input.get();
                    c = input.peek();
                }
                if (!term.exp)
                    term.exp = std::stoi(exp_str) * sign;
            }
            else // exp = 1
            {
                term.exp = 1;
            };
        }
        else
        {
            term.exp = 0;
        }

        // set
        poly.term_append(term);
        // cout << "debug" << coef_str << ", " << exp_str << "; " << term.to_string() << endl; // DEBUG
    };
    input.get(); // for "\n"
    poly._sort();
    return input;
};
ostream &operator<<(ostream &output, Polynomial poly)
{
    poly._sort();
    if (poly.terms <= 0)
    {
        output << "0"; // << endl;
        return output;
    }
    for (int i = 0; i < poly.terms; ++i)
    {
        // cout << "debug: " << poly.termArray[poly.terms - i - 1].coef << ", " << poly.termArray[poly.terms - i - 1].exp << endl;
        output << poly.termArray[poly.terms - i - 1].to_string(i);
        if (i < poly.terms - 1)
            output << " ";
    }
    // output << endl;
    return output;
};

int main()
{
    /*  // input test
    do {
        Polynomial poly;
        cout << "> ";
        if (!(cin >> poly))
        {
            if (cin.eof())
            break;
            cout << "Unacceptable input." << endl;
            cin.clear();
            std::cin.ignore(INT_MAX, '\n');
            continue;
        }
        cout << poly;
    } while(0);
    */

    // v for test
    while (true)
    {
        Polynomial poly1, poly2, poly3;
        float f = 0.0;
        try
        {
            cout << "Enter the first poly:\n> ";
            if (!(cin >> poly1))
                throw -1;
            cout << "Enter the second poly:\n> ";
            if (!(cin >> poly2))
                throw -1;
        }
        catch (const int error)
        {
            if (cin.eof())
                break;
            cout << "Unacceptable input." << endl;
            cin.clear();
            std::cin.ignore(INT_MAX, '\n');
            continue;
        };
        poly3 = poly1.Mult(poly2);
        cout << "(" << poly1 << ") + (" << poly2 << ") = \n"
             << poly1.Add(poly2) << endl;
        cout << "(" << poly1 << ") x (" << poly2 << ") = \n"
             << poly3 << endl;
        cout << "Enter the x:\n> ";
        cin >> f;
        cout << poly3 << "(x=" << f << ") = "<< poly3(f) << endl;
            cin.clear();
            std::cin.ignore(INT_MAX, '\n');
        // poly.debug();
    }
    return 0;
}
```

## 效能分析

| 操作 | 時間複雜度(已排序) | 時間複雜度(未排序) |
| - | - | - |
| `_sort()` | $O(1)$ | $O(n log n)$ (最壞 $O(n²)$ ) |
| `Add()` | $O(m + n)$ | $O(m log m + n log n)$ |
| `Mult()` | $O(mn log(min(m, n)))$ | $O( m log m + n log n + mn log(min(m, n)))$ |
| `>>` | - | $O(n + n log n)$ |
| `<<` | $O(n)$ | $O(n + n log n)$ |

設計上除了輸入以外多項式皆會保持"已排序"狀態，這樣能夠在運算解省效能；若需排序則僅需要額外 $O(m log m + n log n)$ 。

雖然在多個地方呼叫`_sort()`方法，但該方法透過標記的方式避免再已經整理的狀況下再次整理。

| 操作 | 空間複雜度 |
| - | - |
| `_sort()` | $O(log k)$ (最壞 $O(k)$ ) |
| `Add()` | $O(1)$ |
| `Mult()` | $O(min(m, n))$ |
| `>>` | $O(n)$ |
| `<<` | $O(1)$ |

## 測試與驗證

### 基礎預期輸入

| 測試編號 | 測試輸入 | 預期輸出 | 實際輸出 |
| - | - | - | - |
| 1 | `x` | `x` | `x` |
| 2 | `-x` | `-x` | `-x` |
| 3 | `1` | `1` | `1` |
| 4 | `-1` | `-1` | `-1` |
| 5 | `3x` | `3x` | `3x` |
| 6 | `-3x` | `-3x` | `-3x` |
| 7 | `3.5x` | `3.50x` | `3.50x` |
| 8 | `x^2` | `x^2` | `x^2` |
| 9 | `-x^2` | `-x^2` | `-x^2` |
| 10 | `1x^2+3x+5x-2x` | `x^2 +6x` | `x^2 +6x` |
| 11 | `1x^2+3x+5x-x+1` | `x^2 +7x +1` | `x^2 +7x +1` |
| 12 | `x^0` | `1` | `1` |
| 13 | `-x^0` | `-1` | `-1` |
| 14 | `x^-2` | `x^-2` | `x^-2` |
| 15 | `.5x + 1` | `0.50x +1` | `0.50x +1` |
| 16 | `5.x + x` | `6x` | `6x` |
| 17 | `+x` | `x` | `x` |
| 18 | `+3x^2-+5x` | `3x^2 -5x` | `3x^2 -5x` |
| 19 | `3.0000000001x` | `3x` | `3x` |
| 20 | `0003x` | `3x` | `3x` |
| 21 | `x^0003` | `x^3` | `x^3` |
| 22 | `-.5x` | `-0.50x` | `-0.50x` |
| 23 | `--3x` | `3x` | `3x` |
| 24 | `3+++x` | `x +3` | `x +3` |
| 25 | `1x+1x+1x+1x` | `4x` | `4x` |
| 26 | `3x^4 - 2x^4 + x^4` | `2x^4` | `2x^4` |
| 27 | `3x^2+5-5x^2+10x-x` | `-2x^2 +9x +5` | `-2x^2 +9x +5` |
| 28 | `1000000x^100 - 999999x^100` | `x^100` | `x^100` |
| 29 | `0x^5 + 0x^3 + 0x + 0` | `0` | `0` |
| 30 | `-0.000001x + 0.000001x` | `0` | `0` |

### 非預期輸入

| 測試編號 | 測試輸入 | 預期輸出 | 實際輸出 |
| - | - | - | - |
| 1 | `.` | error | error |
| 2 | `..5x` | error | error |
| 3 | `5..3x` | error | error |
| 4 | `x^` | error | error |
| 5 | `x^x` | error | error |
| 6 | `3x5` | error | error |
| 7 | `3 3x` | error | error |
| 8 | `x x` | error | error |
| 9 | `+` | error | error |
| 10 | `-` | error | error |
| 11 | `+-` | error | error |
| 12 | `x^-` | error | error |
| 13 | `x^+` | error | error |
| 14 | `.x` | error | error |
| 15 | `x.` | error | error |
| 16 | `5x.` | error | error |
| 17 | `3x^2.0` | error | error |
| 18 | `x^2.3` | error | error |
| 19 | `三x^二` | error | error |
| 20 | `x^999999999999999999999` | error | error |

### 數組測試

| 測試編號 | 測試輸入1 | 測試輸入2 | 相加預期輸出 | 相乘預期輸出 | 相加實際輸出 | 相乘實際輸出 |
| - | - | - | - | - | - | - |
| 1 | `x` | `x` | `2x` | `x^2` | `2x` | `x^2` |
| 2 | `3x` | `5x` | `8x` | `15x^2` | `8x` | `15x^2` |
| 3 | `x^2` | `x^3` | `x^3 +x^2` | `x^5` | `x^3 +x^2` | `x^5` |
| 4 | `2x^2` | `-3x^2` | `-x^2` | `-6x^4` | `-x^2` | `-6x^4` |
| 5 | `3x + 2` | `x + 4` | `4x +6` | `3x^2 +14x +8` | `4x +6` | `3x^2 +14x +8` |
| 6 | `x^2 + x` | `x + 1` | `x^2 +2x +1` | `x^3 +2x^2 +x` | `x^2 +2x +1` | `x^3 +2x^2 +x` |
| 7 | `2x^2 + 5x` | `x^2 - 3x` | `3x^2 +2x` | `2x^4 -x^3 -15x^2` | `3x^2 +2x` | `2x^4 -x^3 -15x^2` |
| 8 | `x^3 + 2x` | `x^2 - x` | `x^3 +x^2 +x` | `x^5 -x^4 +2x^3 -2x^2` | `x^3 +x^2 +x` | `x^5 -x^4 +2x^3 -2x^2` |
| 9 | `-x^2 + 4x - 1` | `x^2 - x + 3` | `3x +2` | `-x^4 +5x^3 -14x^2 +13x -3` | `3x +2` | `-x^4 +5x^3 -14x^2 +13x -3` |
| 10 | `3x^3 - x` | `2x^2 + 1` | `3x^3 +2x^2 -x +1` | `6x^5 +x^3 -x` | `3x^3 +2x^2 -x +1` | `6x^5 +x^3 -x` |

### 更複雜的數組

| 測試編號 | 測試輸入1 | 測試輸入2 | 相加預期輸出 | 相乘預期輸出 | 相加實際輸出 | 相乘實際輸出 |
| - | - | - | - | - | - | - |
| 1 | `-x^4 +4x^3 -3x^2 +2x +6` | `3x^4 -2x^3 +x^2 +5x -7` | `2x^4 +2x^3 -2x^2 +7x -1` | `-3x^8 +14x^7 -18x^6 +11x^5 +38x^4 -53x^3 +37x^2 +16x -42` | `2x^4 +2x^3 -2x^2 +7x -1` | `-3x^8 +14x^7 -18x^6 +11x^5 +38x^4 -53x^3 +37x^2 +16x -42` |
| 2 | `2x^5 - x^4 + 3x^3 - 4x + 1` | `x^4 + x^3 - 2x^2 + 5x - 3` | `2x^5 +4x^3 -2x^2 +x -2` | `2x^9 +x^8 -2x^7 +15x^6 -21x^5 +15x^4 -22x^2 +17x -3` | `2x^5 +4x^3 -2x^2 +x -2` | `2x^9 +x^8 -2x^7 +15x^6 -21x^5 +15x^4 -22x^2 +17x -3` |
| 3 | `3x^5 -2x^3 +8x -1` | `x^3 +4x^2 -5x +7` | `3x^5 -x^3 +4x^2 +3x +6` | `3x^8 +12x^7 -17x^6 +13x^5 +18x^4 +17x^3 -44x^2 +61x -7` | `3x^5 -x^3 +4x^2 +3x +6` | `3x^8 +12x^7 -17x^6 +13x^5 +18x^4 +17x^3 -44x^2 +61x -7` |
| 4 | `-2x^4 +5x^2 +x -9` | `x^4 -x^3 +3x +6` | `-x^4 -x^3 +5x^2 +4x -3` | `-2x^8 +2x^7 +5x^6 -10x^5 -22x^4 +24x^3 +33x^2 -21x -54` | `-x^4 -x^3 +5x^2 +4x -3` | `-2x^8 +2x^7 +5x^6 -10x^5 -22x^4 +24x^3 +33x^2 -21x -54` |
| 5 | `x^6 -4x^4 +2x^2 -10` | `x^5 +3x^3 -x` | `x^6 +x^5 -4x^4 +3x^3 +2x^2 -x -10` | `x^11 -x^9 -11x^7 -32x^3 +10x` | `x^6 +x^5 -4x^4 +3x^3 +2x^2 -x -10` | `x^11 -x^9 -11x^7 -32x^3 +10x` |

### Eval測試

$x = 2$

| 測試編號 | 測試對象(多項式) | 預期結果 | 實際結果 |
| - | - | - | - |
| 1 | `-3x^8 +14x^7 -18x^6 +11x^5 +38x^4 -53x^3 +37x^2 +16x -42` | `546` | `546` |
| 2 | `3x^8 +12x^7 -17x^6 +13x^5 +18x^4 +17x^3 -44x^2 +61x -7` | `1995` | `1995` |
| 3 | `-2x^8 +2x^7 +5x^6 -10x^5 -22x^4 +24x^3 +33x^2 -21x -54` | `-380` | `-380` |
| 4 | `x^11 -x^9 -11x^7 -32x^3 +10x` | `-108` | `-108` |

## 效能量測

```shell
_meas_: 0
Enter the first poly:
> 1x^10 + 1x^8 + 1x^6 + 1x^4 + 1x^2
_meas_: 7
Enter the second poly:
> 1x^9 + 1x^7 + 1x^5 + 1x^3 + 1x^1
_meas_: 14
_meas_: 14
_meas_: 164
(x^10 +x^8 +x^6 +x^4 +x^2) + (x^9 +x^7 +x^5 +x^3 +x) = 
x^10 +x^9 +x^8 +x^7 +x^6 +x^5 +x^4 +x^3 +x^2 +x
_meas_: 209
(x^10 +x^8 +x^6 +x^4 +x^2) x (x^9 +x^7 +x^5 +x^3 +x) = 
x^19 +2x^17 +3x^15 +4x^13 +5x^11 +4x^9 +3x^7 +2x^5 +x^3
_meas_: 261
Enter the x:
> 0
x^19 +2x^17 +3x^15 +4x^13 +5x^11 +4x^9 +3x^7 +2x^5 +x^3(x=0) = 0
_meas_: 303
_meas_: 0
Enter the first poly:
> x + 1
_meas_: 3
Enter the second poly:
> x^4 + x^3 + x^2 + x + 1
_meas_: 10
_meas_: 10
_meas_: 69
(x +1) + (x^4 +x^3 +x^2 +x +1) = 
x^4 +x^3 +x^2 +2x +2
_meas_: 98
(x +1) x (x^4 +x^3 +x^2 +x +1) = 
x^5 +2x^4 +2x^3 +2x^2 +2x +1
_meas_: 111
_meas_: 0
Enter the first poly:
> x^3 + x^2 + x + 1
_meas_: 6
Enter the second poly:
> x^3 + x^2 + x + 1
_meas_: 12
_meas_: 12
_meas_: 99
(x^3 +x^2 +x +1) + (x^3 +x^2 +x +1) = 
2x^3 +2x^2 +2x +2
_meas_: 127
(x^3 +x^2 +x +1) x (x^3 +x^2 +x +1) = 
x^6 +2x^5 +3x^4 +4x^3 +3x^2 +2x +1
_meas_: 142
```

據觀察，符合預期分析。

## 心得討論

參見 [申論及開發報告](##申論及開發報告)

## 申論及開發報告

-# 部分內容請參見 [解題說明](##解題說明)

受標頭限制因此自行撰寫了一些方法進行輔助(例如: 堆積、`__bit_ceil`、`float_equal`...)，設計輸入的解析程式相當有趣，在這個部分還有升級的空間，例如可以允許括號"()"或是允許更複雜的多項式。

這次做題遇到問題大概是在處理乘法時明明已經思考完運算的原理且經過驗證，卻在實作上不斷寫出bug，導致錯誤(例如莫名其妙少一堆項或在僅輸入"+"時存作為1)。

這次作業中其實除了使用記憶體複製術也能使用指標實踐Node技術，雖然不確定重複且高頻的記憶體刪除/複寫是不是個好主意。

預期輸入會是`ax^b..`這種結構，其中\^以後可省略(x^1)、a/b帶符號、a可為小數(經度可於程式中設定)，詳細預期測試參見 [基礎預期輸入](###基礎預期輸入) ，目前我所設計的允許任意數量符號，且能夠以正確邏輯解析，但據我後來了解這似乎不太正確，不應允許這種語法，應拋出錯誤，這個問題應在新版本修正。
