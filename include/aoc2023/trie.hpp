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

        Node() : value(-1)
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
            size_t idx = c - 97;
            if (x->children[idx] == nullptr)
            {
                x->children[idx] = new Node();
            }
            x = x->children[idx];
        }
        x->value = val;
    }

    Node *search(char c) const
    {
        if ((c < 97) || (c > 122)) {
            return nullptr;
        }
        return root.children[c - 97];
    }

    void print()
    {
        std::queue<Node *> cue;

        cue.push(&root);

        while (!cue.empty())
        {
            Node *x = cue.front();
            cue.pop();
            for (const auto &node : x->children)
            {
                if (node)
                {
                    std::cout << node->value << " ";
                    cue.push(node);
                }
            }
            std::cout << std::endl;
        }
    }

private:
    Node root{};
};