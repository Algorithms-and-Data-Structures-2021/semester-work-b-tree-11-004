#include <iostream>
#include <parser.hpp>

using namespace std;

int main() {
  cout << "Time test:\n";
  string path = "E:\\CppProjects\\semester-work-b-tree-11-004\\dataset\\data\\insert\\data(10000).txt";

  string a = Parser::process_data(path);
  return 0;
}