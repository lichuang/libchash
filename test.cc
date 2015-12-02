#include "chash.h"
#include <stdio.h>
#include <iostream>

int main() {
  CHash chash;

  chash.insert("a", 10);
  chash.insert("b", 10);
  chash.insert("c", 10);
  chash.insert("d", 10);
  chash.insert("e", 10);

  map<string, int> count;
  char tmp[100];
  for (int i = 0; i < 200000; ++i) {
    snprintf(tmp, sizeof(tmp), "%d", i);
    string id;
    chash.find(tmp, &id);
    count[id] += 1;
  }

  map<string, int>::const_iterator iter;
  for (iter = count.begin(); iter != count.end(); ++iter) {
    cout << iter->first << " : " << iter->second << "\n";
  }
  return 0;
}
