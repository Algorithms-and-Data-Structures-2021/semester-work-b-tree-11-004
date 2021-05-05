#include <iostream>
#include <BTree.hpp>

using namespace std;

int main() {
  itis::BTree t(3);
  t.insert(8);
  t.insert(9);
  t.insert(10);
  t.insert(11);
  t.insert(15);
  t.insert(16);
  t.insert(17);
  t.insert(18);
  t.insert(20);
  t.insert(23);

  cout << "The B-tree is: ";
  t.traverse();
  t.remove(20);
  cout << "\nThe B-tree is: ";
  t.traverse();
  cout << "\n";
  t.~BTree();
  cout << "всё";
}