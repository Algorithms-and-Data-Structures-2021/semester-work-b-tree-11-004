#pragma once

#include <utility>
#include <unordered_set>
namespace itis {

#define SEARCH_KEY_NOT_FOUND 's'
#define REMOVE_KEY_NOT_FOUND 'r'

  /// Структура для узлов в нашем дереве

  struct BNode {
    BNode **child;  // Массив указателей на детей
    int *key;       // Массив ключей
    unsigned size;  // Количество ключей
    bool leaf;      // Является ли узел листом
  };

  typedef char BTREE_EXCEPTION;

  class BTree {
   public:
    // Конструктор
    // Параметр - минимальная степень дерева
    // Сложность O(1)
    explicit BTree(unsigned);

    // Деструктор
    // Сложность O(n)
    ~BTree();

    // Вставка ключа в дерево
    // Сложность O(log n)
    void insert(int);

    // Убирает ключ из дерева
    // Бросает ошибку BTREE_EXCEPTION если не найден объект, который надо удалить
    // Сложность O(log n)
    int remove(int);

    // Функция поиска ключа в дереве
    // Сложность O(log n)
    std::pair<BNode *, unsigned> search(int);

    // Использует поиск, но выводит только ключ
    // Бросает ошибку BTREE_EXCEPTION если не найден нужный объект
    // Сложность O(log n)
    int searchKey(int);

    // Принт
    // Сложность O(n)
    void print();

   private:
    // Нужна для инициализации узлов
    void initializeNode(BNode *);

    // Рекурсивная функция, используемая деструктором
    void freeNode(BNode *);

    // Находит индекс ключа в узле
    unsigned findIndex(BNode *, int);

    // Добавляет ключ в узел
    unsigned nodeInsert(BNode *, int);

    // Удаляет ключ по указанному индексу
    int nodeDelete(BNode *, unsigned);

    // Функция для разделения заполненных узлов
    void splitChild(BNode *, int);

    // Объединяет двух детей из узла по указанному индексу в одного ребёнка
    char mergeChildren(BNode *, unsigned);

    // Проверка того, что количество детей узла >= minDegree
    char fixChildSize(BNode *, unsigned);

    // Рекурсия, принт поддерева
    void printNode(BNode *, unsigned);

    // Корневой узел
    BNode *root;

    // Сравнение, позволяющее организовавать расположение элементов
    bool lessThan(int, int);

    // Вывод элементов в дереве
    void printKey(int);

    // Минимальная степень дерева
    unsigned minDegree;
  };

#include "data_structure.hpp"
}  // namespace itis