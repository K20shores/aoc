#pragma once

#include <stddef.h>
#include <string>
#include <queue>
#include <unordered_map>

class Trie
{
  struct Node
  {
    std::unordered_map<char, Node *> children;
    bool terminal;

    Node() : children(), terminal(false)
    {
    }
  };
  Node *root;

public:
  Trie() : root(new Node())
  {
  }

  void insert(std::string key)
  {
    Node *x = root;
    for (const auto &c : key)
    {
      if(x->children.find(c) == x->children.end())
      {
        x->children[c] = new Node();
      }
      x = x->children[c];
    }
    x->terminal = true;
  }

  std::vector<int> search(const std::string_view &s) const
  {
    const Node *x = root;
    std::vector<int> matches;
    for(size_t i = 0; i < s.size(); ++i)
    {
      char c = s[i];
      if (x->children.find(c) == x->children.end())
      {
        break;
      }
      x = x->children.at(c);
      if (x->terminal)
      {
        matches.push_back(i+1);
      }
    }
    return matches;
  }
};
