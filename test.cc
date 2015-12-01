#include "chash.h"
#include <iostream>

int main() {
  CHash chash;

  chash.insert("a", 10);
  chash.insert("b", 10);
  chash.insert("c", 10);
  chash.insert("d", 10);
  chash.insert("e", 10);

  string id;
  chash.find("123456", &id);
  cout << id << endl;

  return 0;
}
