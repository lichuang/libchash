#ifndef __CHASH_H__
#define __CHASH_H__

#include <list>
#include <map>
#include <string>

using namespace std;

class CNode;
class VNode;

class CHash {
  friend class CNode;
  friend class VNode;
public:
  CHash();
  ~CHash();

  void find(const string &key, string *id);
  bool insert(const string &id, int vnodes);

  void erase(const string &id);

private:
  void eraseVNode(VNode *);

private:
  map<int, VNode *> vnodes_;
  map<string, CNode*> cnodes_;
};

#endif // __CHASH_H__
