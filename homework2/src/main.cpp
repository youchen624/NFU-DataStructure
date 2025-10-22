#include <iostream>
#include <algorithm>
#include <string>
#include <cmath>

// #include <limits.h>
#define INT_MAX 2147483647

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
unsigned int _bit_ceil(unsigned int n)
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

public:
    Term(float coef_ = 0.0f, int exp_ = 0) : coef(coef_), exp(exp_) {};
    // Term operator=(const Term &that) {};
    string to_string(bool with_posSign = false)
    {
        string str = "";
        if (!this->coef)
            return str;
        if (this->coef < 0)
            str += '-';
        else if (with_posSign)
            str += '+';
        str += std::to_string(this->coef);
        if (!this->exp)
            return str;
        str += 'x';
        if (this->exp != 1)
        {
            str += '^';
            str += std::to_string(this->exp);
        }
        return str;
    };

private:
    float coef; // coefficient
    int exp;    // exponent
};
// Term exp less than CMP-function
struct term_isExpLess
{
    bool operator()(const Term &a, const Term &b)
    {
        return (a.exp < b.exp);
    };
};

// CMP should be a compare-function bool(T a, T b)
template <typename T, typename CMP>
class Heap
{
public:
    Heap() {};
    ~Heap() {};
    T pop() {};
    T popush(T replace_) {};

private:
    void _heapify(size_t d_index = 0) {};
    size_t _size;
};

// #TODO add a operator&&()
class Polynomial
{
    friend istream &operator>>(istream &input, Polynomial &poly);
    friend ostream &operator<<(ostream &output, const Polynomial &poly);

public:
    // Construct the polynomial p(x) = 0.
    Polynomial(int length = 0) : capacity(_bit_ceil(length))
    {
        termArray = new Term[length];
    };

    // Copy constructor
    Polynomial(const Polynomial &that) : Polynomial(that.capacity)
    {
        this->terms = that.terms;
        for (int i = 0; i < terms; ++i)
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
    Polynomial Add(const Polynomial that)
    {
        int temp_capacity = this->capacity + that.capacity;
        if (!temp_capacity)
            return Polynomial();
        int this_ti = 0, that_ti = 0, main_ti = 0;
        Polynomial temp_poly(temp_capacity);
        bool avail_this = (this_ti < this->capacity);
        bool avail_that = (that_ti < that.capacity);
        for (
            ; avail_this || avail_that;
            avail_this = (this_ti < this->capacity),
            avail_that = (that_ti < that.capacity))
        {
            int this_exp = avail_this ? this->termArray[this_ti].exp : INT_MAX;
            int that_exp = avail_that ? that.termArray[that_ti].exp : INT_MAX;
            if (this_exp == that_exp)
            {
                const int temp_coef = this->termArray[this_ti].coef + that.termArray[that_ti].coef;
                ++this_ti;
                ++that_ti;
                if (!temp_coef)
                    continue;
                temp_poly.termArray[main_ti].coef = temp_coef;
                temp_poly.termArray[main_ti].exp = this_exp;
            }
            else if (this_exp < that_exp)
            {
                temp_poly.termArray[main_ti].coef = this->termArray[this_ti].coef;
                temp_poly.termArray[main_ti].exp = this_exp;
                ++this_ti;
            }
            else
            {
                temp_poly.termArray[main_ti].coef = that.termArray[that_ti].coef;
                temp_poly.termArray[main_ti].exp = that_exp;
                ++that_ti;
            }
            ++main_ti;
        }
        if (temp_capacity == main_ti)
            return temp_capacity;
        Polynomial final_poly(main_ti);
        copy(temp_poly.termArray, temp_poly.termArray + main_ti - 1, final_poly.termArray);
        return final_poly;
    };

    // Return the product of the polynomials *this and poly.
    Polynomial Mult(Polynomial that)
    {
        Polynomial poly;
        return poly;
    };

    // Evaluate the polynomial *this at f and return the result.
    float Eval(float f)
    {
        float temp = 0;
        for (int i = 0; i < terms; ++i)
        {
            temp += termArray[i].coef * pow(f, termArray[i].exp);
        }
        return temp;
    };

    // add a term
    Polynomial &term_add(float coef, int exp)
    {
        return term_add(Term(coef, exp));
    };
    Polynomial &term_add(Term term)
    {
        if (capacity == terms)
            _upgrade_capacity();
        termArray[terms - 1] = term;
        terms++;
        return *this;
    };

    //
    // Polynomial &term_set(int index, float coef, int exp) {};

    // copy operators
    // Polynomial &operator=(Polynomial &that) {};

private:
    void _upgrade_capacity()
    {
        Term *temp_ptr = termArray;
        int tc = capacity;
        capacity *= 2;
        termArray = new Term[capacity];
        copy(temp_ptr, temp_ptr + tc, termArray);
        delete[] temp_ptr;
    };

    void _sort()
    {
        if (terms <= 1)
            return;
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
            temp[i] = termArray[i];
            for (j = i + 1; (j < terms) && (termArray[i].exp == termArray[j].exp); ++j)
            {
                temp[i].coef += termArray[j].coef;
            }
        }
        delete[] termArray;
        termArray = temp;
        terms = new_terms;

        // dump 0^0
        _dump();
    };

    void _dump(double coef = 0, int exp = 0)
    {
        Term *temp = new Term[capacity];
        int new_terms = 0;
        for (int i = 0; i < terms; ++i)
        {
            if ((termArray[i].exp != exp) || (termArray[i].coef != coef))
            {
                temp[new_terms] = termArray[i];
                ++new_terms;
            }
        }
        delete[] termArray;
        termArray = temp;
        terms = new_terms;
    };

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
    int capacity;    // size of termArray
    int terms;       // number of nonzero terms
};

// ostream &operator<<(ostream &output, const Term &term) {
//     string str = "";
// };
// #TODO make "2x^4+(2x^4+1)" supports
istream &operator>>(istream &input, Polynomial &poly)
{
    Term term;
    string coef_str = "0", exp_str = "0";
    int c = input.peek();
    for (
        int sign = 1;

        (c != EOF) && (c != '\n');

        c = input.peek(),
            sign = 1,
            coef_str = "0",
            exp_str = "0",
            term = Term())
    {
        // sign
        // [+-..][0~9..][x][^][0~9..]
        while ((c == '+') || (c == '-'))
        {
            if (c == '-')
                sign *= -1;
            input.get();
            c = input.peek();
        }

        // coefficient
        // integer part
        while (isdigit(c))
        {
            coef_str += c;
            input.get();
            c = input.peek();
        }
        // decimals part
        if (c == '.')
        {
            coef_str += '.';
            input.get();
            c = input.peek();
            while (isdigit(c))
            {
                coef_str += c;
                input.get();
                c = input.peek();
            }
        }
        term.coef = std::stof(coef_str) * sign;

        // x & its exponent
        if (c == 'x')
        {
            if (!term.coef)
                term.coef = sign;
            input.get();
            c = input.peek();
            if (c != '^') // exp =1
            {
                term.exp = 1;
            }
            else // exp > 1
            {
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
                while (isdigit(c)) // exp getting
                {
                    exp_str += c;
                    input.get();
                    c = input.peek();
                }
                term.exp = std::stoi(exp_str) * sign;
            };
        }
        else
        {
            term.exp = 0;
        }

        // set
        poly.term_add(term);

    };
    // isdigit('.');
    // isdigit('a');
    return input;
};
ostream &operator<<(ostream &output, const Polynomial &poly)
{
    for (int i = poly.terms - 1; i >= 0; ++i)
    {
        output << poly.termArray[i].to_string(i);
    }
    output << endl;
    return output;
};

int main()
{
    Polynomial poly;
    cin >> poly;
    cout << poly;
    return 0;
}