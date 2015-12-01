#include <stdio.h>
#include "chash.h"

class VNode {
public:
  VNode(CNode *cnode, int id);

  const string& Id() const;

  CNode *parent_;
  string id_;
};

class CNode {
public:
  CNode(CHash *hash, const string &id, int vnode);
  ~CNode();

  const string& Id() const {
    return id_;
  }

  CHash *chash_;
  string id_;
  list<VNode*> child_;
};

static inline hashindex_t hash(const char *str) {
  hashindex_t hash = 5381;
  const char *s;

  if (str == NULL) {
    return 0;
  }

  for (s = str; *s; s++) { 
    hash = ((hash << 5) + hash) + *s;
  }
  hash &= 0x7FFFFFFF;

  return hash / 65535;
}

VNode::VNode(CNode *cnode, int id)
  : parent_(cnode) {
  char buf[256];
  snprintf(buf, sizeof(buf), "%s_%d", cnode->Id().c_str(), id);
  id_ = buf;
}

const string& VNode::Id() const {
  return id_;
}

CNode::CNode(CHash *chash, const string &id, int vnode) 
  : chash_(chash),
    id_(id) {
  while (vnode > 0) {
    --vnode;
    child_.push_back(new VNode(this, vnode));
  }
}

CNode::~CNode() {
  list<VNode*>::iterator iter;

  for (iter = child_.begin(); iter != child_.end(); ) {
    VNode *child = *iter;
    ++iter;
    hashindex_t idx = hash(child->Id().c_str());
    chash_->vnodes_.erase(idx);
    delete child;
  }
}

CHash::CHash() {
}

CHash::~CHash() {
  map<string, CNode*>::iterator iter;

  for (iter = cnodes_.begin(); iter != cnodes_.end(); ) {
    CNode *cnode = iter->second;
    ++iter;
    delete cnode;
  }
}

bool CHash::insert(const string &id, int vnodes) {
  if (cnodes_.find(id) != cnodes_.end()) {
    return false;
  }
  CNode *cnode = new CNode(this, id, vnodes);
  if (cnode == NULL) {
    return false;
  }
  list<VNode*>::iterator iter;

  for (iter = cnode->child_.begin(); iter != cnode->child_.end(); ) {
    VNode *child = *iter;
    hashindex_t idx = hash(child->Id().c_str());
    while (vnodes_.find(idx) != vnodes_.end()) {
      char buf[200];
      snprintf(buf, sizeof(buf), "%s_%d", child->Id().c_str(), idx);
      child->id_ = buf;
      idx = hash(child->Id().c_str());
    }
    vnodes_[idx] = child;
    ++iter;
  }
  cnodes_[cnode->Id()] = cnode;

  return true;
}

void CHash::find(const string &key, string *id) {
  hashindex_t idx = hash(key.c_str());

  map<hashindex_t, VNode*>::const_iterator iter = vnodes_.lower_bound(idx);
  VNode *vnode;
  if (iter == vnodes_.end()) {
    iter = vnodes_.begin();
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
