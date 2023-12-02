#include <stddef.h>
#include <string>
#include <queue>

class Trie
{
    static constexpr size_t ALPHABET_SIZE = 26;

public:
    struct Node
    {
        Node *children[ALPHABET_SIZE];
        int value;
        bool terminal;

        Node() : value(-1), terminal(false)
        {
            for (int i = 0; i < ALPHABET_SIZE; ++i)
            {
                children[i] = nullptr;
            }
        }
    };

    void insert(std::string key, int val)
    {
        Node *x = &root;
        for (const auto &c : key)
        {
            size_t idx = c - 'a';
            if (x->children[idx] == nullptr)
            {
                x->children[idx] = new Node();
            }
            x = x->children[idx];
        }
        x->value = val;
        x->terminal = true;
    }

    int search(const std::string &s) const
    {
        const Node *x = &root;
        for (const auto &c : s)
        {
            if ('a' <= c && c <= 'z' && x->children[c - 'a'])
            {
                x = x->children[c - 'a'];
                if (x->terminal)
                {
                    return x->value;
                }
            }
            else
            {
                break;
            }
        }
        return -1;
    }

private:
    Node root{};
};