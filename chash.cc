#include "chash.h"

class VNode {
public:
  VNode(CNode *cnode, int id);

  string Id() const;

  CNode* parent_;
  string id_;
};

class CNode {
public:
  CNode(CHash *hash, const string &id, int vnode);
  ~CNode();

  string Id() const {
    return id_;
  }

  string id_;
  list<VNode*> child_;
  CHash *chash_;
};

VNode::VNode(CNode *cnode, int id)
  : parent_(cnode) {
  char bff[256];
  snprintf(buf, sizeof(buf), "%s_%d", cnode->Id().c_str(), id);
  id_ = buf;
}

string VNode::Id() const {
  return id_;
}

CNode::CNode(CHash *chash, const string &id, int vnode) 
  : chash_(chash),
    id_(id) {
  while (vnode > 0) {
    --vnode;
    child_.insert(new VNode(this, vnode));
  }
}

CNode::~CNode() {
  list<VNode*>::iterator iter;

  for (iter = child_.begin(); iter != child_.end(); ) {
    VNode *child = *iter;
    ++iter;
    chash_->eraseVNode(child);
    delete child;
  }
}

CHash::CHash() {
}

CHash::~CHash() {
  list<CNode*>::iterator iter;

  for (iter = cnodes_.begin(); iter != cnodes_.end(); ) {
    CNode *cnode = *iter;
    ++iter;
    eraseCNode(cnode);
    delete cnode;
  }
}

inline int hash(const string &key) {
  return key.length() % 256;
}

bool CHash::insert(const string &id, int vnodes) {
  if (cnodes_.find(id) != cnodes_.end()) {
    return false;
  }
  CNode *cnode = new CNode(id, vnodes);
  if (cnode == NULL) {
    return false;
  }
  list<VNode*>::iterator iter;

  for (iter = cnode->child_.begin(); iter != cnode->child_.end(); ) {
    VNode *child = *iter;
    int idx = hash(child->Id);
    vnodes_[idx] = child;
    ++iter;
  }
  cnodes_[cnode->Id()] = cnode;

  return true;
}

void CHash::find(const string &key, string *id) {
  int idx = hash(key);

  map<int, VNode*>::const_iterator iter = vnodes_.lower_bound(idx);
  VNode *vnode;
  if (iter == vnodes_.end()) {
    iter = vnodes_.begin()
  }

  vnode = iter->second;

  *id = vnode->parent_->Id();
}

void CHash::erase(const string &id) {
  map<string, CNode*>::iterator iter = cnodes_.find(id);
  if (iter == cnodes_.end()) {
    return;
  }

  delete iter->second;
  cnodes_.erase(iter);
}

void CHash::eraseVNode(VNode *vnode) {
  int idx = hash(vnode->Id());

  map<int, VNode*>::iterator iter = vnodes_.find(idx);
  if (iter == vnodes_.end()) {
    return;
  }

  vnodes_.erase(iter);
}
