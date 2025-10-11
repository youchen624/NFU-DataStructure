#include <iostream>
#include <cmath>
using namespace std;

class Polynomial
{
public:
    Polynomial() : capacity(0) {};
    // Construct the polynomial p(x) = 0.

    Polynomial Add(Polynomial poly)
    {
        int new_capacity = (this->capacity > poly.capacity ? this->capacity : poly.capacity);
    };
    // Return the sum of the polynomials *this and poly.

    Polynomial Mult(Polynomial poly);
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

private:
    Term *termArray; // array of nonzero terms
    int capacity;    // size of termArray
    int terms;       // number of nonzero terms
};

class Term
{
    friend Polynomial;

private:
    float coef; // coefficient
    int exp;    // exponent
};

int main()
{
    return 0;
}