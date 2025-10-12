#include <iostream>
#include <cmath>
using namespace std;

class Polynomial
{
public:
    Polynomial(int length = 0) : capacity(length)
    {
        termArray = new Term[length];
    };
    // Construct the polynomial p(x) = 0.
    // Copy constructor
    Polynomial(const Polynomial &that) : Polynomial(that.capacity)
    {
        for (int i = 0; i < capacity; ++i)
        {
            this->termArray[i] = that.termArray[i];
        }
    };

    // destructor
    ~Polynomial()
    {
        delete[] termArray;
    };

    Polynomial Add(Polynomial poly)
    {
        int new_capacity = (this->capacity > poly.capacity ? this->capacity : poly.capacity);
        Polynomial result(new_capacity);
        for (int index = 0; index < new_capacity; ++index)
        {
            result.termArray[index].exp = index;
            if (this->capacity >= new_capacity) {} else {}
        }
    };
    // Return the sum of the polynomials *this and poly.

    Polynomial Mult(Polynomial poly) {};
    // Return the product of the polynomials *this and poly.

    float Eval(float f)
    {
        float temp = 0;
        for (int i = 0; i < capacity; ++i)
        {
            temp += termArray[i].coef * pow(f, termArray[i].exp);
        }
        return temp;
    };
    // Evaluate the polynomial *this at f and return the result.

    // copy operators
    Polynomial &operator=(Polynomial &that) {};

private:
    Term *termArray; // array of nonzero terms
    int capacity;    // size of termArray
    int terms;       // number of nonzero terms
};

class Term
{
    friend Polynomial;

public:
    Term() : coef(0.0f), exp(0) {};
    // Term operator=(const Term &that) {};

private:
    float coef; // coefficient
    int exp;    // exponent
};

int main()
{
    return 0;
}