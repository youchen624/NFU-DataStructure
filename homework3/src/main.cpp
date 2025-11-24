#include <iostream>
using namespace std;

// Chain #BEGIN
// w11W 251119

template <class T>
class Chain
{
private:
    // template <class T>
    class ChainNode
    {
        // friend class Chain;
    private:
        T _data;
        ChainNode *_link;

    private:
        /*
        ChainNode* _search(size_t index_ = 0) {
            if (!index_) return this;
            if (this->_link) return this->_link->_search(index_ - 1);
            else throw "Out of range.";
        };
        */
        // protected:
    public:
    };

protected:
    ChainNode *_search(size_t index_ = 0)
    {
        if (_size <= 0)
            throw "Chain-List is empty.";
        if (index_ >= _size)
            throw "Out of range.";

        ChainNode *ptr = _node_head;
        for (size_t i = 0; i < index_; ++i)
        {
            if (!(ptr->_link))
                throw "Chain internal error (logic).";
            ptr = ptr->_link;
        }
        return ptr;
    };

public:
    class ChainIterator
    {
    public:
        ChainIterator(ChainNode* ptr) {};
        ~ChainIterator() {};
    private:
    };
    using iterator = ChainIterator;

    Chain() : _size(0) {};
    ~Chain() {};

    // returns whether the Chain empty (no items)
    bool is_empty() const
    {
        return !_size;
    };
    size_t size() const
    {
        return _size;
    };

    // returns the index of the first value-matches index of items from head; returns the number of size when not found
    size_t index_of(const T& value_) const {};
    // returns the index of the first value-matches index of items from tail; returns the number of size when not found
    size_t index_of_reverse(const T& value_) {};

    // return the top(first index) of item ref
    iterator begin() {};
    // return the end(last index) of item ref
    iterator end() {};

    // append an item at the tail
    size_t push_back(const T& item_) {};

    // return the top(first index) of item, and then remove that
    T pop_front(size_t index_) {};
    // return the end(last index) of item, and then remove that
    T pop_back(size_t index_) {};

    // insert, then returns whether item that be inserted is the last item
    bool insert(size_t index_, const T& item_) {};
    // delete, then returns whether deleted anything
    bool remove(size_t index_) {};

    // delete all items, then returns whether deleted anything
    bool clear() {};

private:
    size_t _size;
    ChainNode *_node_head;
};

// Chain #END

class Polynomial
{
    friend istream &operator>>(istream &is, Polynomial &x);
    friend istream &operator>>(istream &is, Polynomial &x);

public:
    Polynomial() {};
    Polynomial(const Polynomial &a) {};
    ~Polynomial() {
        // delete _node ss (link)
    };

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
        if (index >= _sizes)
            return false;
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