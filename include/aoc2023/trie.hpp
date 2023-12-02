
class Trie
{
    static constexpr size_t ALPHABET_SIZE = 26;

    class Node
    {
        Node *children[ALPHABET_SIZE];
        bool IsTerminal;
        char value;
    };
};