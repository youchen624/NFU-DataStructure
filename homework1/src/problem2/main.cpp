#include <iostream>
#include <string>
#include <cmath>
using namespace std;

class TheSet
{
public:
    enum class Type
    {
        Data = 0,
        Set = 1
    };

    class Node
    {
    public:
        Node() : Node(Type::Set) {};
        Node(Type type) : _type(type)
        {
            if (type == Type::Set)
            {
                data.set = nullptr;
            }
            else
            {
                data.value = new string("0");
            }
        };
        Node(const string value) : _type(Type::Data)
        {
            data.value = new string(value);
        };
        Node(const Node &node)
        {
            this->_type = node._type;
            if (_type == Type::Set)
            {
                this->data.value = new string(*node.data.value);
            }
            else if (_type == Type::Data)
            {
                this->data.set = new TheSet(*node.data.set);
            }
        };
        ~Node()
        {
            if (_type == Type::Set)
            {
                delete data.set;
            }
            else if (_type == Type::Data)
            {
                delete data.value;
            }
        };
        Node &operator=(const string value)
        {
            clean_delete();
            _type = Type::Data;
            data.value = new string(value);
            return *this;
        };
        Node &operator=(const TheSet &set)
        {
            clean_delete();
            _type = Type::Set;
            data.set = new TheSet(set);
            return *this;
        };
        Node &operator=(const Node &that)
        {
            if (this == &that)
            {
                return *this;
            }
            clean_delete();
            this->_type = that._type;
            if ((_type == Type::Data) && (that.data.value != nullptr))
            {
                this->data.value = new string(*that.data.value);
            }
            else if ((_type == Type::Set) && (that.data.set != nullptr))
            {
                this->data.set = new TheSet(*that.data.set);
            }
            return *this;
        };

        Node &reset() {};

        Type _type;
        union _data
        {
            TheSet *set;
            string *value;
        } data;

        friend ostream &operator<<(ostream &output, const Node &node);

    private:
        void clean_delete()
        {
            if (_type == Type::Set && data.set)
            {
                delete data.set;
                data.set = nullptr;
            }
            else if (_type == Type::Data && data.value)
            {
                delete data.value;
                data.value = nullptr;
            }
        };
    };

    TheSet() : TheSet(nullptr, 0) {};
    TheSet(const size_t length) : _length(length)
    {
        if (length <= 0)
        {
            nodes = nullptr;
            return;
        }
        nodes = new Node[length];
    };
    TheSet(const string *array, size_t length) : _length(length)
    {
        if (length <= 0 || array == nullptr)
        {
            nodes = nullptr;
            _length = 0;
            return;
        }
        nodes = new Node[length];
        for (size_t i = 0; i < _length; ++i)
        {
            nodes[i] = array[i];
        }
    };
    TheSet(const TheSet &that)
    {
        this->_length = that._length;
        this->nodes = new Node[_length];
        for (size_t i = 0; i < _length; ++i)
        {
            nodes[i] = that.nodes[i];
        }
    };
    ~TheSet()
    {
        delete[] nodes;
    };
    TheSet &operator=(const TheSet &set)
    {
        if (this == &set)
            return *this;
        clear();
        _length = set._length;
        nodes = new Node[_length];
        for (size_t i = 0; i < _length; ++i)
        {
            nodes[i] = set.nodes[i];
        }
        return *this;
    };

    TheSet &clear()
    {
        delete[] nodes;
        _length = 0;
        nodes = nullptr;
        return *this;
    };
    TheSet &reset(const size_t index) {};
    TheSet &set(const size_t index, const string& value) {
        nodes[index] = value;
    };
    TheSet &set(const size_t index, const TheSet set) {};
    TheSet &change_length(const size_t length) //, const int mode)
    {
        if (_length == length)
            return *this;
        if (length)
        {
            Node *temp = new Node[length];
            for (size_t i = 0; (i < _length) && (i < length); ++i)
            {
                temp[i] = nodes[i];
            }
            delete[] nodes;
            nodes = temp;
            _length = length;
        }
        else
            clear();
        return *this;
    };
    TheSet &getPowerSet()
    {
        const size_t powerSet_length = round(pow(2, _length));
        TheSet *result_set = new TheSet(powerSet_length);
        // recursive function
        auto recursive = [&]() -> void {};
        return *result_set;
    };
    const size_t length()
    {
        return _length;
    };
    friend ostream &operator<<(ostream &output, const TheSet &theSet);

private:
    Node *nodes = nullptr;
    int _length;
};
ostream &operator<<(ostream &output, const TheSet::Node &node)
{
    if (node._type == TheSet::Type::Data)
    {
        output << node.data.value;
    }
    else if (node._type == TheSet::Type::Set)
    {
        output << node.data.set;
    }
    return output;
};
ostream &operator<<(ostream &output, const TheSet &theSet)
{
    if (theSet.nodes == nullptr)
    {
        output << "( )";
        return output;
    }
    output << "( " << theSet.nodes[0];
    for (size_t i = 1; i < theSet._length; ++i)
    {
        output << ", " << theSet.nodes[i];
    }
    output << " )";
    return output;
};

int main()
{
    // const int array[10] = {0};
    const string array[3] = {"a", "b", "c"};
    TheSet set;
    TheSet array_set(array, 10);
    TheSet copy = array_set;
    cout << set << endl;
    cout << copy << endl;
    cout << array_set << endl;
    cout << array_set.getPowerSet() << endl;
    return 0;
}