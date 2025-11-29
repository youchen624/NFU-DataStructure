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
        friend class Chain;
        friend class ChainIterator;
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
        ChainNode(const T& data_ = {}, ChainNode* link_ = nullptr) : _link(link_), _data(data_) {};
        ~ChainNode() {};
    };

protected:
    ChainNode *_search(size_t index_ = 0)
    {
        if (_size <= 0)
            throw "Chain-List is empty.";
        if (index_ >= _size)
            throw "Out of range.";
        if (index_ == _size - 1)
            return this->_node_last;

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
        ChainIterator(ChainNode* ptr_) : _ptr(ptr_) {};
        ~ChainIterator() {};

        // get the raw value ref like a normal pointer
        T& operator*() const {
            return this->_ptr->_data;
        };
        // using as a pointer
        T* operator->() const {
            return &(this->_ptr->_data);
        };

        ChainIterator& operator++() {
            this->_ptr = this->_ptr->link;
            return *this;
        };
        // ChainIterator& operator--() const {};
        ChainIterator& operator++(int) {
            ChainIterator t = *this;
            this->_ptr = this->_ptr->_link;
            return t;
        };

        bool operator==(const ChainIterator& that) const {
            return (this->_ptr == that._ptr);
        };
        bool operator!=(const ChainIterator& that) const {
            return (this->_ptr != that._ptr);
        };

        private:
        ChainNode* _ptr;
    };
    using iterator = ChainIterator;

    Chain() : _size(0) {};
    ~Chain() {
        ChainNode* t = _node_head;
        for (size_t i = 0; i < _size; ++i) {
            t = t->_link;
            delete _node_head;
            _node_head = t;
        }
    };

    // returns whether the Chain empty (no items)
    bool empty() const
    {
        return !_size;
    };
    // returns current size of the Chain
    size_t size() const
    {
        return _size;
    };

    // returns the index of the first value-matches index of item from head; returns the number of size when not found
    size_t index_of(const T& value_) const {
        if (!_size) return false;
        ChainNode* p = this->_node_head;
        for (size_t i = 0; i < _size; ++i) {
            if (p->_data == value_) return true;
            else if (p->_link) p =  p->_link;
        }
        return false;
    };
    // returns the index of the first value-matches index of item from tail; returns the number of size when not found
    size_t index_of_opposite(const T& value_) {
        if (!_size) return false;
        ChainNode* p = this->_node_head;
        ChainNode** array = new ChainNode*[this->_size];    // these can be replaced by 2-way chain if it exists
        for (size_t i = 0; i < _size; ++i) {
            array[i] = p;
            if (p->_link) p = p->_link;
        }
        for (size_t i = _size - 1; i >= 0; --i) {
            if (array[i]->_data == value_) {
                delete[] array;
                return true;
            };
            // else if (array->_link) array  p->_link;
        }
        delete[] array;
        return false;
    };

    // return the top(first index) of iterator
    iterator begin() {
        return iterator(this->_search(0));
    };
    // return the end(last index) of iterator
    iterator end() {
        return iterator(this->_search(this->_size - 1));
    };

    // append an item at the tail
    size_t push_back(const T& item_) {
        if (this->empty()) {
            _node_head = _node_last = new ChainNode(item_);
        } else {
            _node_last->_link = new ChainNode(item_);
            _node_last = _node_last->_link;
        }
        return ++_size;
    };
    // append an item at the font
    size_t push_font(const T& item_) {
        if (this->empty()) return this->push_back(item_);
        else {
            _node_head = new ChainNode(item_, _node_head);
        }
        return ++_size;
    };

    // return the top(first index) of item, and then remove that
    T pop_front(size_t index_) {
        if (this->empty()) throw "Chain is empty.";
        T t = _node_head->_data;
        ChainNode* t_ptr = _node_head;
        delete _node_head;
        _node_head = t_ptr;
        --_size;
        return t;
    };
    // return the end(last index) of item, and then remove that
    T pop_back(size_t index_) {
        if (_size <= 1) return pop_front(index_);
        T t = _node_last->_data;
        delete _node_last;
        _node_last = _search(_size - 2);
        --_size;
        return;
    };

    // insert, insert an item into the target index, and cause 1 offset for all behind's, then returns whether item that be inserted is the last item
    bool insert(size_t index_, const T& item_) {};
    // delete, then returns whether deleted anything
    bool remove(size_t index_) {};

    // delete all items, then returns whether deleted anything
    bool clear() {
        if (this->empty()) return false;
        ChainNode* ptr = _node_head;
        for (size_t i = 0; i < _size; ++i) {
            ptr = ptr->_link;
            delete _node_head;
            _node_head = ptr;
        }
        _node_head = _node_last = nullptr;
        return true;
    };

    T& operator[](size_t index_) {
        return this->_search(index_)->_data;
    };

    /* // mystery func (s)
    // undo for one step, this is an once function, should do any operation before calling, or always fail; returns whether undo successful
    bool undo() {};
    // random all items position
    void random() {};
    */
    
private:
    size_t _size;
    ChainNode* _node_head;
    ChainNode* _node_last;
};

// Chain #END

/*
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
*/

int main()
{
    Chain<int> c;
    for (int i = 0; i < 20; ++i) {
        c.push_back(i);
    }
    cout << "c.size=" << c.size() << endl;
    for (size_t i = 0; i < c.size(); ++i) {
        cout << c[i] << " ";
    } cout << endl;
    return 0;
}