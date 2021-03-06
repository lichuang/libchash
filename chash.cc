#include <stdio.h>
#include <list>
#include "chash.h"
#include "md5.h"

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
  int vnode_;
};

static hashindex_t hash(const char *str, int len) {
  MD5 md5(str, len);
  const byte *digest = md5.digest();
  unsigned int hash = 0;
  int i;
  for(i = 0; i < 4; i++) {
    hash += ((long)(digest[i*4 + 3]&0xFF) << 24)
      | ((long)(digest[i*4 + 2]&0xFF) << 16)
      | ((long)(digest[i*4 + 1]&0xFF) <<  8)
      | ((long)(digest[i*4 + 0]&0xFF));
  }

  return (hash & 0x7FFFFFFF);
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
    id_(id),
    vnode_(vnode) {
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
    hashindex_t idx = hash(child->Id().c_str(), child->Id().length());
    chash_->vnodes_.erase(idx);
    delete child;
  }
}

CHash::CHash() {
}

CHash::~CHash() {
  clean();
}

void CHash::clean() {

  map<string, CNode*>::iterator iter;

  for (iter = cnodes_.begin(); iter != cnodes_.end(); ) {
    CNode *cnode = iter->second;
    ++iter;
    delete cnode;
  }

  cnodes_.clear();
}

CHash * CHash::clone() {
  CHash *chash = new CHash();

  map<string, CNode*>::iterator iter;

  for (iter = cnodes_.begin(); iter != cnodes_.end(); ++iter) {
    CNode *cnode = iter->second;

    chash->insert(cnode->Id());
  }

  return chash;
}

bool CHash::insert(const string &id) {
  int vnodes = 512;
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
    hashindex_t idx = hash(child->Id().c_str(), child->Id().length());
    while (vnodes_.find(idx) != vnodes_.end()) {
      char buf[200];
      snprintf(buf, sizeof(buf), "%s_%d", child->Id().c_str(), idx);
      child->id_ = buf;
      idx = hash(child->Id().c_str(), child->Id().length());
    }
    vnodes_[idx] = child;
    ++iter;
  }
  cnodes_[cnode->Id()] = cnode;

  return true;
}

bool CHash::find(const string &key, string *id) {
  hashindex_t idx = hash(key.c_str(), key.length());

  map<hashindex_t, VNode*>::const_iterator iter = vnodes_.lower_bound(idx);
  VNode *vnode;
  if (iter == vnodes_.end()) {
    iter = vnodes_.begin();
  }

  if (iter == vnodes_.end()) {
    return false;
  }

  vnode = iter->second;

  *id = vnode->parent_->Id();

  return true;
}

void CHash::erase(const string &id) {
  map<string, CNode*>::iterator iter = cnodes_.find(id);
  if (iter == cnodes_.end()) {
    return;
  }

  delete iter->second;
  cnodes_.erase(iter);
}
