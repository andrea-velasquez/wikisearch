#ifndef TRIE_CPP
#define TRIE_CPP

#include <fstream>

template <typename T>
Trie<T>::~Trie()
{
  delete root;
}

template <typename T>
Trie<T>::Trie():
root(new TrieNode<T>(T{})), nwords(0){}

template <typename T>
bool Trie<T>::empty()
{
  return !this->nwords;
}

template <typename T>
int Trie<T>::getTotalWords()
{
  return nwords;
}

template <typename T>
bool Trie<T>::insert(std::string word, T key)
{
  TrieNode<T> *curnode = this->root;
  for (const char& c: word)
  {
    if (!curnode->children.count(c))
      curnode->children[c] = new TrieNode<T>(T{});
    curnode = curnode->children[c];
  }
  
  // Set key in last character to mark end of word
  // only if the key is empty
  if (curnode->key!=T{})
    return false; // word exists
  curnode->key = key;
  ++nwords;
  return true; // word inserted
}

template <typename T>
T Trie<T>::getKeyOf(std::string word)
{
  TrieNode<T> *curnode = this->root;
  for (const char& c: word)
  {
    if (!curnode->children.count(c))
      return T{};
    curnode = curnode->children[c];
  }
  return curnode->key;
}

template <typename T>
void Trie<T>::getWords(TrieNode<T>* curnode, std::string prefix, std::vector<std::string>* pwords, std::ofstream* pfile)
{
  if (curnode->key!=T{}) // leaf
  {
    if (pwords) pwords->push_back(prefix);
    if (pfile) *pfile << prefix << "\n";
    return;
  }

  for (const auto& pair : curnode->children)
    // pair.first = letter, pair.second = node
    getWords(pair.second, prefix+pair.first, pwords, pfile);
}

template <typename T>
void Trie<T>::getWords(std::vector<std::string>& words)
{
  std::vector<std::string>* pwords = &words;
  this->getWords(this->root, "", pwords);
}

template <typename T>
void Trie<T>::exportWords(std::string filename)
{
  std::ofstream file(filename);
  std::ofstream* pfile = &file;
  this->getWords(this->root, "", nullptr, pfile);
}

#endif