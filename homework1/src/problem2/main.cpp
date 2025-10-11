#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
using namespace std;

#define THE_SET_EMPTY_SET "∅"
#define THE_SET_LEFT_SYMBOL "{"
#define THE_SET_RIGHT_SYMBOL "}"
#define THE_SET_SEPARATOR ", "
#define BITS_IN_SIZE_T 64

/*
[Ref](https://blog.csdn.net/github_38148039/article/details/109598368)
[Ref](https://tomsworkspace.github.io/2021/02/27/GCC%E8%87%AA%E5%B8%A6%E7%9A%84%E4%B8%80%E4%BA%9Bbuiltin%E5%86%85%E5%BB%BA%E5%87%BD%E6%95%B0/)

__builtin_popcountl(u)
std::popcount(u)
* @code
* ```cpp
unsigned long __popcount (unsigned long u)
{
    u = (u & 0x5555555555555555ULL) + ((u >> 1) & 0x5555555555555555ULL);
    u = (u & 0x3333333333333333ULL) + ((u >> 2) & 0x3333333333333333ULL);
    u = (u & 0x0F0F0F0F0F0F0F0FULL) + ((u >> 4) & 0x0F0F0F0F0F0F0F0FULL);
    u = (u & 0x00FF00FF00FF00FFULL) + ((u >> 8) & 0x00FF00FF00FF00FFULL);
    u = (u & 0x0000FFFF0000FFFFULL) + ((u >> 16) & 0x0000FFFF0000FFFFULL);
    u = (u & 0x00000000FFFFFFFFULL) + ((u >> 32) & 0x00000000FFFFFFFFULL);
    return u;
}
* ```
*/
size_t __popcount(size_t n)
{
    // kernigha
    size_t count = 0;
    while (n > 0)
    {
        n &= (n - 1);
        count++;
    }
    return count;
}

/*
[Ref](https://tomsworkspace.github.io/2021/02/27/GCC%E8%87%AA%E5%B8%A6%E7%9A%84%E4%B8%80%E4%BA%9Bbuiltin%E5%86%85%E5%BB%BA%E5%87%BD%E6%95%B0/)
__builtin_ctzl(x)
std::countr_zero(x)
*/
size_t _ctzl(size_t x) {
    if (x == 0) {
        // return sizeof(size_t) * CHAR_BIT;
        return BITS_IN_SIZE_T;
    }
    unsigned long long u = (unsigned long long)x;
    size_t r = 63;
    u &= -u;
    if (u & 0x00000000FFFFFFFFULL) r -= 32;
    if (u & 0x0000FFFF0000FFFFULL) r -= 16;
    if (u & 0x00FF00FF00FF00FFULL) r -= 8;
    if (u & 0x0F0F0F0F0F0F0F0FULL) r -= 4;
    if (u & 0x3333333333333333ULL) r -= 2;
    if (u & 0x5555555555555555ULL) r -= 1;

    return r;
}

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
                data.value = nullptr;
            }
        };
        Node(const string value) : _type(Type::Data)
        {
            data.value = new string(value);
        };
        Node(const Node &node)
        {
            this->_type = node._type;
            if (_type == Type::Data)
            {
                this->data.value = new string(*node.data.value);
            }
            else if (_type == Type::Set)
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

        Node &reset()
        {
            clean_delete();
            return *this;
        };
        string to_string()
        {
            stringstream ss;
            ss << *this;
            return ss.str();
        };

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
    TheSet &reset(const size_t index)
    {
        nodes[index].reset();
        return *this;
    };
    TheSet &set(const size_t index, const string &value)
    {
        nodes[index] = value;
        return *this;
    };
    TheSet &set(const size_t index, const TheSet &set)
    {
        nodes[index] = set;
        return *this;
    };
    TheSet &set(const size_t index, const Node &node)
    {
        nodes[index] = node;
        return *this;
    };
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
    TheSet getPowerSet()
    {
        const size_t powerSet_length = round(pow(2, _length));
        TheSet result_set = TheSet(powerSet_length);
        _get_power_set(result_set, powerSet_length);
        // recursive function
        // void recursive = [&](size_t num) -> void {
        //     for() {}
        //     recursive(num + 1);
        // };
        // recursive(0);
        return result_set;
    };

    string to_string()
    {
        stringstream ss;
        ss << *this;
        return ss.str();
    };

    const size_t length()
    {
        return _length;
    };
    friend ostream &operator<<(ostream &output, const TheSet &theSet);

private:
    Node *nodes = nullptr;
    size_t _length;

    void _get_power_set(TheSet &result, size_t total, size_t num = 1)
    {
        // how many 1(2) inside num
        // size_t size = __builtin_popcountl(num);
        size_t size = __popcount(num);
        size_t t = num;
        TheSet set(size);
        for (size_t i = 0; i < size; ++i)
        {
            // int index = __builtin_ctzl(t);
            int index = _ctzl(t);
            t ^= (1UL << index);
            set.set(i, this->nodes[index]);
        }
        result.set(num, set);
        ++num;
        if (num < total)
            _get_power_set(result, total, num);
    };
};
ostream &operator<<(ostream &output, const TheSet::Node &node)
{
    if (node._type == TheSet::Type::Data)
    {
        if (node.data.value != nullptr)
            output << *node.data.value;
    }
    else if (node._type == TheSet::Type::Set)
    {
        if (node.data.set != nullptr)
            output << *node.data.set;
        else
            output << THE_SET_EMPTY_SET;
    }
    return output;
};
ostream &operator<<(ostream &output, const TheSet &theSet)
{
    if (theSet.nodes == nullptr)
    {
        output << THE_SET_EMPTY_SET;
        return output;
    }
    output << THE_SET_LEFT_SYMBOL << theSet.nodes[0];
    for (size_t i = 1; i < theSet._length; ++i)
    {
        output << THE_SET_SEPARATOR << theSet.nodes[i];
    }
    output << THE_SET_RIGHT_SYMBOL;
    return output;
};

int main()
{
    /*
    // const int array[10] = {0};
    const string array[3] = {"a", "b", "c"};
    TheSet set;
    TheSet array_set(array, 3);
    TheSet copy = array_set;
    cout << set << endl;
    cout << copy << endl;
    cout << array_set << endl;
    cout << array_set.getPowerSet() << endl;
    */

    cout << "Input the size of the set:" << endl;
    int size;
    cin >> size;
    string *the_datas = new string[size];
    for (int i = 0; i < size; ++i)
    {
        cin >> the_datas[i];
    }
    TheSet input_set(the_datas, size);
    const TheSet powerset = input_set.getPowerSet();
    cout << powerset << endl;
    delete the_datas;
    return 0;
}