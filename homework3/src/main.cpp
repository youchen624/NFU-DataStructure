#include <iostream>
using namespace std;

class Polynomial
{
    friend istream &operator>>(istream &is, Polynomial &x);
    friend istream &operator>>(istream &is, Polynomial &x);

public:
    Polynomial() {};
    Polynomial(const Polynomial &a) {};
    ~Polynomial() {};

    const Polynomial &operator=(const Polynomial a) {};
    Polynomial operator+(const Polynomial b) {};
    Polynomial operator-(const Polynomial b) {};
    Polynomial operator*(const Polynomial b) {};

    float Evaluate(float x) const {};

    class Node
    {
    public:
        // Node() : link(nullptr) {};
        Node(int coef_ = 0, int exp_ = 0) : link(nullptr), coef(coef_), exp(exp_) {};
        Node(const Node &that) : Node(that.coef, that.exp) {};
        ~Node() {};

        Node operator=(const Node &that)
        {
            this->coef = that.coef;
            this->exp = that.exp;
            // dont care 'link' (Node*)
            return *this;
        };

        Node *get(size_t index)
        {
            if (!index)
                return this;
            else if (!link)
                throw "Out of Range.";
            else
                return this->link->get(index - 1);
        };
        Node *get_prv(size_t index)
        {
            if (!index)
                return nullptr;
            return Node::get(index - 1);
        };

    public:
        int coef;
        int exp;
        Node *link;
        // private:
    };

private:
    // Node* _node_get(size_t index) {};
    void _sort() {};

    size_t _node_append(Node node) {};
    void _node_insert(size_t index, int coef, int exp) {};
    void _node_insert(size_t index, Node node) {};

    // returns false when deletes nothing
    bool _node_delete(size_t index)
    {
        if (index > _sizes) return false;
    };
    bool _node_replace(size_t index, Node node = {})
    {
        try
        {
            if (index = _sizes)
            {
                // node.link = _nodes->link;
                // *_nodes = node;
                return _node_append(node);
            }
            else if (index > _sizes)
            {
                // out of range
                throw -1;
                // return false;
            }
            else
            {
                // Node *ptr = _nodes->get(index);
                // node.link = ptr->link;
                *(_nodes->get(index)) = node;
            }
            return true;
        }
        catch (int e)
        {
            return false;
        }
    };

    Node _node_pop() {};
    Node _node_shift() {};

private:
    // node list
    Node *_nodes;

    bool _is_sorted;
    size_t _sizes;
};

istream &operator>>(istream &is, Polynomial &x) {};
istream &operator>>(istream &is, Polynomial &x) {};

int main()
{
    return 0;
}