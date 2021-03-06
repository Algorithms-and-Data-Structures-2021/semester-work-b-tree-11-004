#include <fstream>
#include <iostream>
#include <string>       // string, string to int
#include <string_view>  // string_view
#include <chrono>       // high_resolution_clock, duration_cast, nanoseconds
#include <vector>
#include <sstream>

// подключаем нашу структуру данных
#include "BTree.hpp"

using namespace std;
using namespace itis;

// абсолютный путь до набора данных и папки проекта
static constexpr auto kDatasetPath = string_view{PROJECT_DATASET_DIR_REMOVE};
static constexpr auto kProjectPath = string_view{PROJECT_SOURCE_DIR};

int main() {

  // Tip 1: входные аргументы позволяют более гибко контролировать параметры вашей программы
  const auto path = string(kDatasetPath);
  const auto output_path = string(kProjectPath) + "/benchmark/result/removeResults.csv";

  auto output_file1 = ofstream(output_path);
  output_file1.close();
  // работа с набором данных
  vector<string> files = {"100", "500", "1000", "5000", "10000", "50000", "100000", "500000", "1000000", "5000000"};
  BTree bTree(50);

  for (const string& file : files) {  // Проходим по всем 10 .csv файлам
    for (int i = 1; i < 11; i++) {    // Запускаем замерку времени 10 раз
      auto input_file = ifstream(path + "/" + file + ".csv");
      auto time_diff_erase = chrono::nanoseconds::zero();
      cout << (path + "/" + file) << endl;

      if (input_file) {
        int line_number = 0;
        for (string line; getline(input_file, line, ','); /* ... */) {
          auto ss = stringstream(line);  // оборачиваем строку в объект "поток"
          bTree.insert(stoi(line));
          line_number++;
        }
      }
      input_file.close();
      input_file = ifstream(path + "/" + file + ".csv");

      // здесь находится участок кода, время которого необходимо замерить
      if (input_file) {
        int line_number = 0;
        for (string line; getline(input_file, line, ','); /* ... */) {
          auto ss = stringstream(line);
          const auto time_point_before_erase = chrono::high_resolution_clock::now();
          bTree.remove(stoi(line));
          const auto time_point_after_erase = chrono::high_resolution_clock::now();
          line_number++;
          time_diff_erase += time_point_after_erase - time_point_before_erase;
        }
      }

      const auto time_elapsed_ns_erase = chrono::duration_cast<chrono::nanoseconds>(time_diff_erase).count();
      cout << time_elapsed_ns_erase << endl;

      bTree.deleteNode(bTree.root);
      input_file.close();

      //Открываем файл для записи и вносим полученые данные
      auto output_file = ofstream(output_path, ios::app);
      output_file << file << "," << i << "," << time_elapsed_ns_erase << endl;
      output_file.close();
    }
  }
  return 0;
}