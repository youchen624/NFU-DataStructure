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
    ChainNode* _search(size_t index_ = 0)
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
        ChainIterator(ChainNode* ptr_ = nullptr) : _ptr(ptr_) {};
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
            this->_ptr = this->_ptr->_link;
            return *this;
        };
        // ChainIterator& operator--() const {};
        ChainIterator& operator++(int) {
            ChainIterator t = *this;
            this->_ptr = this->_ptr->_link;
            return t;
        };

        bool operator!=(const ChainIterator& that) const {
            return (this->_ptr != that._ptr);
        };
        bool operator==(const ChainIterator& that) const {
            return !operator!=(that);
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

    // returns the index of the first value-matches index of item from the head; returns the number of size when not found
    size_t index_of(const T& value_) const {
        if (!_size) return _size;
        ChainNode* p = this->_node_head;
        for (size_t i = 0; i < _size; ++i) {
            if (p->_data == value_) return i;
            else if (p->_link) p =  p->_link;
        }
        return _size;
    };
    // returns the index of the first value-matches index of item from the tail; returns the number of size when not found
    size_t index_of_opposite(const T& value_) {
        if (!_size) return _size;
        ChainNode* p = this->_node_head;
        ChainNode** array = new ChainNode*[this->_size];    // these can be replaced by 2-way chain if it exists
        for (size_t i = 0; i < _size; ++i) {
            array[i] = p;
            if (p->_link) p = p->_link;
        }
        for (size_t i = 0; i < _size; ++i) {
            if (array[_size - i - 1]->_data == value_) {
                delete[] array;
                return _size - i - 1;
            };
            // else if (array->_link) array  p->_link;
        }
        delete[] array;
        return _size;
    };

    // return the top(first index) of iterator
    iterator begin() {
        if (empty()) return iterator(nullptr);
        return iterator(this->_search(0));
    };
    // return nullptr of iterator //~~// return the end(last index) of iterator~~
    iterator end() {
        return iterator(nullptr);
        // return iterator(this->_search(this->_size - 1));
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
    // append an item at the front
    size_t push_front(const T& item_) {
        if (this->empty()) return this->push_back(item_);
        else {
            _node_head = new ChainNode(item_, _node_head);
        }
        return ++_size;
    };

    // return the top(first index) of item, and then remove that
    T pop_front() {
        if (this->empty()) throw "Chain is empty.";
        T t = _node_head->_data;
        ChainNode* t_ptr = _node_head->_link;
        delete _node_head;
        _node_head = t_ptr;
        --_size;
        return t;
    };
    // return the end(last index) of item, and then remove that
    T pop_back() {
        if (_size <= 1) return pop_front();
        T t = _node_last->_data;
        delete _node_last;
        _node_last = _search(_size - 2);
        _node_last->_link = nullptr;
        --_size;
        return t;
    };

    // insert, insert an item into the target index, and cause 1 offset for all behind's, then returns whether item that be inserted is the last item
    bool insert(size_t index_, const T& item_) {
        if (index_ > _size) throw "Out of range";
        ChainNode* ptr;
        if (!index_) {
            ptr = _node_head = new ChainNode(item_, _node_head);
        } else {
            ChainNode* pptr = _search(index_ - 1);
            ptr = pptr->_link = new ChainNode(item_, pptr->_link);
        }
        ++_size;
        return (ptr == _node_last);
    };

    // delete, then returns whether deleted anything
    bool remove(size_t index_) {
        if (index_ > _size) return false; // delete nothing ~~//throw "Out of range";~~
        ChainNode* del = nullptr;
        if (!index_) {
            del = _node_head;
            _node_head = _node_head->_link;
        } else {
            ChainNode* pptr = _search(index_ - 1);
            del = pptr->_link;
            pptr->_link = pptr->_link->_link;   // bypassing
        }
        --_size;
        delete del;
        return true;
    };

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
        _size = 0;
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

    void debug() {
        cout
            << "debug--->" << endl
            << "size=" << size() << endl
            << "head=" << _node_head << endl
            << "last=" << _node_last << endl
            << "<---debug"
        << endl;
    };
    
private:
    size_t _size;
    ChainNode* _node_head;
    ChainNode* _node_last;
};

// Chain #END



// CircularList #BEGIN
// w12Sa 251129

template <class T>
class CircularList {
protected:
    class Node {
    friend class CircularList;
    private:
        T data;
        Node* next;
        Node* prev;
    public:
        // void* operator new(size_t size) {};
        // void operator delete(Node*, size_t) {};
        Node(const T& data_ = {}, Node* next_ = nullptr, Node* prev_ = nullptr) : data(data_), next(next_), prev(prev_) {};
        ~Node() {};
    };

    Node* _search(size_t index_ = 0) {
        if (!_size) throw "List is empty.";
        if (index_ >= _size) throw "Out of range.";

        Node* ptr = _node_head;
        if (index_ > _size % 2) {
            // rev
            for (size_t i = _size - 1;true; --i) {
                if (index_ == i)
                    return ptr->prev;
                else
                    ptr = ptr->prev;
            }
        } else {
            // pos
            for (size_t i = 0;true; ++i) {
                if (index_ == i)
                    return ptr;
                else
                    ptr = ptr->next;
            }
        }
    };

    /*
public:
    class CircularListIterator {
    public:
        CircularListIterator(Node* ptr_) : ptr(ptr_) {};
        ~CircularListIterator() {};

        T& operator*() const {
            return this->ptr->data;
        };
        T* operator->() const {
            return &(this->ptr->data);
        };

        CircularListIterator& operator++() {
            this->ptr = this->ptr->next;
            return *this;
        };
        CircularListIterator& operator++(int) {
            CircularListIterator t = *this;
            this->ptr = this->ptr->next;
            return t;
        };
        CircularListIterator& operator--() {
            this->ptr = this->ptr->prev;
            return *this;
        };
        CircularListIterator& operator--(int) {
            CircularListIterator t = *this;
            this->ptr = this->ptr->prev;
            return t;
        };

        bool operator!=(const CircularListIterator& that) const {
            return (this->ptr != that.ptr);
        };
        bool operator==(const CircularListIterator& that) const {
            return !operator!=(that);
        };
    private:
        Node* ptr;
    };
    using iterator = CircularListIterator;
    */

    // Node* _node_require(T& _item, Node* next_, Node* prev_) {};
    // void _node_release(Node* ptr_) {};

public:
    CircularList() : _size(0) {};
    ~CircularList() {};

    // returns whether the List empty
    bool empty() const {
        return !_size;
    };
    // returns current size of the List
    size_t size() const {
        return this->_size;
    };

    // returns the index of the first value-matches index of item from the head; returns the number of size when not found
    size_t index_of(const T& value_) const {
        Node* ptr = _node_head;
        for (size_t i = 0; i < _size; ++i) {
            if (ptr->data == value_) return i;
            else ptr = ptr->next;
        }
        return this->_size;
    };
    // returns the index of the first value-matches index of item from the tail; returns the number of size when not found
    size_t index_of_opposite(const T& value_) const {
        Node* ptr = _node_head->prev;
        for (size_t i = 0; i < _size; ++i) {
            if (ptr->data == value_) return _size - i - 1;
            else ptr = ptr->prev;
        }
        return this->_size;
    };

    /*
    iterator begin() {
        return iterator(_node_head);
    };
    iterator end() {
        return iterator(nullptr);
    };
    */

    // append an item at the tail
    size_t push_back(const T& item_) {
        if (empty()) {
            _node_head = new Node(item_);
            _node_head->prev
                = _node_head->next
                = _node_head;
        } else {
            // !! WARN !! below#3 , ordering problem if you change !! WARN !!
            // #BEGIN
            _node_head->prev
                = _node_head->prev->next
                = new Node(item_, _node_head, _node_head->prev);
            // #END
            // !! WARN !! ^^^#3 , ordering problem if you change !! WARN !!
        }
        return ++_size;
    };
    // append an item at the front
    size_t push_front(const T& item_) {
        if (empty()) {
            return this->push_back(item_);
        } else {
            // !! WARN !! below#4 , ordering problem if you change !! WARN !!
            // #BEGIN
            _node_head
                = _node_head->prev
                = _node_head->prev->next
                = new Node(item_, _node_head, _node_head->prev);
            // #END
            // !! WARN !! ^^^#4 , ordering problem if you change !! WARN !!
        }
        return ++_size;
    };

    // return the head item, and then remove that from the List
    T pop_front() {
        if (empty()) throw "List is empty.";
        T t = _node_head->data;
        if (_size == 1) {
            delete _node_head;
            _node_head = nullptr;
            --_size;
            return t;
        }
        Node* ptr = _node_head->next;
        _node_head->next->prev = _node_head->prev;
        _node_head->prev->next = _node_head->next;
        delete _node_head;
        _node_head = ptr;
        --_size;
        return t;
    };
    // return the tail item, and then remove that from the List
    T pop_back() {
        if (empty()) throw "List is empty.";
        T t = _node_head->prev->data;
        if (_size == 1) return pop_front();
        Node* dptr = _node_head->prev;
        dptr->prev->next = _node_head;
        _node_head->prev = dptr->prev;
        delete dptr;
        --_size;
        return t;
    };

    // sort
    // void sort() {};

    // insert, insert an item into the target index, and cause 1 offset for all behind's, then returns whether item that be inserted is the last item of the List
    bool insert(size_t index_, const T& item_) {
        if (index_ > _size) throw "Out of range";
        else if (index_ == _size) return push_back(item_);
        else if (!index_) return push_front(item_); // || empty()
        Node* pptr = _search(index_ - 1);
        pptr->next
            = pptr->next->prev
            = new Node(item_, pptr->next, pptr);
        ++_size;
        return false;
    };

    // delete, dump an item, and cause -1 offset for all behind's, then returns the item which be dumped
    T remove(size_t index_) {
        if (index_ > _size) throw "Out of range"; // return false;
        else if (index_ == _size) return pop_back();
        else if (!index_) return pop_front(); // || empty()
        Node* dptr = _search(index_ );
        T t = dptr->data;
        dptr->prev->next = dptr->next;
        dptr->next->prev = dptr->prev;
        --_size;
        return t;
    };

    // delete all items from List
    void clear() {
        for (
            Node* dptr;
            _size; --_size
        ) {
            dptr = _node_head;
            _node_head = dptr->next;
            delete dptr;
            // dptr->next->prev = _node_head;
        }
        _node_head->prev = _node_head;
    };

    // works as normal Array
    T& operator[](size_t index_) {
        return this->_search(index_)->data;
    };

    /*
    // Mystery func (s)
    // makes the circular list change the rotation direction
    void inverse() {};
    // extracts all items of odd index from the List, and they will not keep in the origin List; count from "index_"; work like "star_even()" when index_ +- 1
    CircularList star_odd(size_t index_) {};
    //
    CircularList connect(CircularList& that) {};
    */

    void debug() {};
protected:
    size_t _size;
    Node* _node_head;
    // Node* _node_last;
};

// CircularList #END


// Polynomial #BEGIN
// w14Fa1 251212

class Polynomial
{
    friend istream &operator>>(istream &is, Polynomial &x);
    friend ostream &operator<<(ostream &is, Polynomial &x);

protected:
    typedef struct Term {
        int coef;
        int exp;
    } Term;
    void _narrow() {};
    void _dump() {
        CircularList<Term>* new_nodes = new CircularList<Term>();
        for (size_t i; i < nodes->size(); ++i) {
            if ((*nodes)[i].coef) new_nodes->push_back((*nodes)[i]);
        }
        delete nodes;
        nodes = new_nodes;
    };

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

private:
    CircularList<Term>* nodes;
};
istream &operator>>(istream &is, Polynomial &x) {};
ostream &operator<<(ostream &is, Polynomial &x) {};


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

// Polynomial #END



/*
int main()
{
    Chain<int> c;
    for (int i = 0; i < 20; ++i) {
        c.push_back(i);
    }
    cout << "c.size=" << c.size() << endl;
    for (Chain<int>::ChainIterator i = c.begin(); i != c.end(); ++i) {
        int v = *i;
        cout << v << " ";
    } cout << endl;
    cout << "c.index_of(5)=" << c.index_of(5) << endl;
    cout << "c.index_of_opposite(6)=" << c.index_of_opposite(6) << endl;

    cout << "c[7]=9=" << c[7] << endl;
    c[7] = 9;
    for (Chain<int>::ChainIterator i = c.begin(); i != c.end(); ++i) {
        int v = *i;
        cout << v << " ";
    } cout << endl;

    cout << "c.insert(8, 999)=>" << endl;
    c.insert(8, 999);
    for (Chain<int>::ChainIterator i = c.begin(); i != c.end(); ++i) {
        int v = *i;
        cout << v << " ";
    } cout << endl;

    cout << "c.remove(8)=>" << endl;
    c.remove(8);
    for (Chain<int>::ChainIterator i = c.begin(); i != c.end(); ++i) {
        int v = *i;
        cout << v << " ";
    } cout << endl;

    // c.debug();

    cout << "c.clear()=>" << endl;
    c.clear();
    for (Chain<int>::ChainIterator i = c.begin(); i != c.end(); ++i) {
        int v = *i;
        cout << v << " ";
    } cout << endl;

    // c.debug();
    cout << "c.push_front([20~1])=>" << endl;
    for (int i = 20; i > 0; --i) {
        c.push_front(i);
    }
    // c.debug();
    for (Chain<int>::ChainIterator i = c.begin(); i != c.end(); ++i) {
        int v = *i;
        cout << v << " ";
    } cout << endl;

    int back = c.pop_back();
    cout << "c.pop_back()=" << back << endl << "c= ";
    for (Chain<int>::ChainIterator i = c.begin(); i != c.end(); ++i) {
        int v = *i;
        cout << v << " ";
    } cout << endl;

    int front = c.pop_front();
    cout << "c.pop_front()=" << front << endl << "c= ";
    for (Chain<int>::ChainIterator i = c.begin(); i != c.end(); ++i) {
        int v = *i;
        cout << v << " ";
    } cout << endl;

    cout << "c.clear()" << endl;
    c.clear();
    for(int i = 0; i < 10; ++i) {
        c.push_back(2);
    }
    cout << "c.push_back([2 * 10])=>" << endl;
    for (Chain<int>::ChainIterator i = c.begin(); i != c.end(); ++i) {
        int v = *i;
        cout << v << " ";
    } cout << endl;
    cout << "c.index_of(2)=" << c.index_of(2) << endl;
    cout << "c.index_of_opposite(2)=" << c.index_of_opposite(2) << endl;
    cout << "c.index_of(200)=" << c.index_of(200) << endl;
    cout << "c.index_of_opposite(200)=" << c.index_of_opposite(200) << endl;
    return 0;
}
*/


int main()
{
    CircularList<int> c;
    for (int i = 0; i < 20; ++i) {
        c.push_back(i);
    }
    cout << "c.size=" << c.size() << endl;
    for (size_t i =0; i < c.size(); ++i) {
        int v = c[i];
        cout << v << " ";
    } cout << endl;
    cout << "c.index_of(5)=" << c.index_of(5) << endl;
    cout << "c.index_of_opposite(6)=" << c.index_of_opposite(6) << endl;

    cout << "c[7]=9=" << c[7] << endl;
    c[7] = 9;
    for (size_t i =0; i < c.size(); ++i) {
        int v = c[i];
        cout << v << " ";
    } cout << endl;

    cout << "c.insert(8, 999)=>" << endl;
    c.insert(8, 999);
    for (size_t i =0; i < c.size(); ++i) {
        int v = c[i];
        cout << v << " ";
    } cout << endl;

    cout << "c.remove(8)=>" << endl;
    c.remove(8);
    for (size_t i =0; i < c.size(); ++i) {
        int v = c[i];
        cout << v << " ";
    } cout << endl;

    // c.debug();

    cout << "c.clear()=>" << endl;
    c.clear();
    for (size_t i =0; i < c.size(); ++i) {
        int v = c[i];
        cout << v << " ";
    } cout << endl;

    // c.debug();
    cout << "c.push_front([20~1])=>" << endl;
    for (int i = 20; i > 0; --i) {
        c.push_front(i);
    }
    // c.debug();
    for (size_t i =0; i < c.size(); ++i) {
        int v = c[i];
        cout << v << " ";
    } cout << endl;

    int back = c.pop_back();
    cout << "c.pop_back()=" << back << endl << "c= ";
    for (size_t i =0; i < c.size(); ++i) {
        int v = c[i];
        cout << v << " ";
    } cout << endl;

    int front = c.pop_front();
    cout << "c.pop_front()=" << front << endl << "c= ";
    for (size_t i =0; i < c.size(); ++i) {
        int v = c[i];
        cout << v << " ";
    } cout << endl;

    cout << "c.clear()" << endl;
    c.clear();
    for(int i = 0; i < 10; ++i) {
        c.push_back(2);
    }
    cout << "c.push_back([2 * 10])=>" << endl;
    for (size_t i =0; i < c.size(); ++i) {
        int v = c[i];
        cout << v << " ";
    } cout << endl;
    cout << "c.index_of(2)=" << c.index_of(2) << endl;
    cout << "c.index_of_opposite(2)=" << c.index_of_opposite(2) << endl;
    cout << "c.index_of(200)=" << c.index_of(200) << endl;
    cout << "c.index_of_opposite(200)=" << c.index_of_opposite(200) << endl;
    return 0;
}