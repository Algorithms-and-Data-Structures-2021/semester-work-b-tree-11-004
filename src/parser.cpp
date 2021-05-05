#include <utility>
#include "ctime"
#include <fstream>
#include <sstream>
#include <vector>
#include <functional>
#include <parser.hpp>

#include "BTree.cpp"

using namespace std;
using namespace itis;

vector<int> split(const std::string &s, char delimiter) {
  vector<int> tokens;

  string token;
  istringstream tokenStream(s);
  while (getline(tokenStream, token, delimiter)) {
    tokens.push_back(stoi(token));
  }
  return tokens;
}

void Parser::process_data(const string &pathToInputFile, const string &pathToResult, int iterations) {
  string line;
  vector<float> insert;
  vector<float> search;
  vector<float> remove;
  std::ofstream out;
  out.open(pathToResult);

  for (int j = 0; j < iterations; ++j) {

    ifstream file(pathToInputFile);

    while (getline(file, line)) {
      auto *btree = new BTree(2);

      vector<int> intValues = split(line, ' ');

      //insert
      double startTime = clock();
      for (int value : intValues) {
        btree->insert(value);
      }
      double endTime = clock();
      insert.emplace_back(endTime - startTime);

      if (out.is_open()) {
        out << to_string(endTime - startTime) << " ";
      }

      //search
      startTime = clock();
      for (int value : intValues) {
        btree->search(value);
      }
      endTime = clock();
      search.emplace_back(endTime - startTime);

      if (out.is_open()) {
        out << to_string(endTime - startTime) << " ";
      }

      //remove
      startTime = clock();
      for (int value : intValues) {
        btree->remove(value);
      }
      endTime = clock();
      remove.emplace_back(endTime - startTime);

      if (out.is_open()) {
        out << to_string(endTime - startTime);
      }

      out << std::endl;
    }
    file.close();
  }

  float average = 0;
  for (float f : insert) {
    average += f;
  }
  average /= static_cast<float>(insert.size());
  if (out.is_open()) {
    out << "insert: " << to_string(average) << "\n";
  }

  average = 0;
  for (float f : search) {
    average += f;
  }
  average /= static_cast<float>(search.size());
  if (out.is_open()) {
    out << "search: " << to_string(average) << "\n";
  }

  average = 0;
  for (float f : remove) {
    average += f;
  }
  average /= static_cast<float>(remove.size());
  if (out.is_open()) {
    out << "remove: " << to_string(average) << "\n";
  }

  out.close();
}