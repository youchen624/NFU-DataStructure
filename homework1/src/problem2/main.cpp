#include <iostream>
#include <cmath>
using namespace std;

class TheSet
{
public:
    enum class Type
    {
        Number = 0,
        Set = 1
    };

    class Node
    {
    public:
        Node() : Node(Type::Set) {};
        Node(int num) : _type(Type::Number)
        {
            data.number = num;
        };
        Node(Type type) : _type(type)
        {
            if (type == Type::Set)
            {
                data.set == nullptr;
            }
            else
            {
                data.number = 0;
            }
        };
        Type _type;
        union _data
        {
            TheSet *set;
            int number;
        } data;

        friend ostream &operator<<(ostream &output, const Node &node);
    };

    Node *nodes = nullptr;
    int length;

    TheSet() : TheSet(nullptr, 0) {};
    TheSet(int *array, int size) : length(size)
    {
        if (size <= 0 || array == nullptr)
        {
            return;
        }
        nodes = new Node[size];
    };
    ~TheSet()
    {
        delete[] nodes;
    };

    friend ostream &operator<<(ostream &output, const TheSet &theSet);
    TheSet getPowerSet()
    {
        auto recursive = [&]() -> void {};
        return *this;
    };
};
ostream &operator<<(ostream &output, const TheSet::Node &node)
{
    if (node._type == TheSet::Type::Number)
    {
        output << node.data.number;
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
        output << " ( )";
        return output;
    }
    output << "( " << theSet.nodes[0];
    for (int i = 1; i < theSet.length; ++i)
    {
        output << ", " << theSet.nodes[i];
    }
    output << " )";
    return output;
};

int main()
{
    TheSet set;
    cout << set << endl;
    return 0;
}