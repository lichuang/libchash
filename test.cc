#include "chash.h"
#include <stdio.h>
#include <iostream>

int main() {
  CHash chash;

  chash.insert("127.0.0.1:11111", 10);
  chash.insert("100.84.44.134:22222", 10);
  chash.insert("8.8.8.8:22222", 10);
  chash.insert("192.168.0.1:22222", 10);
  chash.insert("123.456.7.89:22222", 10);

  map<string, int> count;
  char tmp[100];
  for (int i = 0; i < 500000; ++i) {
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
