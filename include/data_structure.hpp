/* B-Tree
 * Структура данных Б-дерева.
 * Операции поиска вставки и удаления за время O(lg(n)).
 * Использует O(n) память, где n-количество элементов в дереве.
 */

#pragma once

#include <utility>

#define NULL 0
#define SEARCH_KEY_NOT_FOUND 's'
#define REMOVE_KEY_NOT_FOUND 'r'

namespace itis {

  // Пример: объявление константы времени компиляции в заголовочном файле
  inline constexpr auto kStringConstant = "Hello, stranger!";

  // Структура для представление узлов Б-дереву
  template<typename T>
  struct BNode {
    BNode<T> **child;  // Массив указателей на детей.
    T *key;            // Массив ключей
    unsigned size;     // Количество ключей.
    bool leaf;         // Является ли узел листом.
  };

  template<typename T>
  struct BTree {
   public:
    // Конструктор.
    // Первый параметр - это минимальная степень дерева.
    // Второй параметр - это функция сравнения ключей дерева.
    // Третий параметр - это функция, которая печатает ключи.
    // Константное время.
    BTree(unsigned, bool (*)(T, T), void (*)(T) = NULL);

    // Деструктор.
    // Линейное время.
    ~BTree<T>();

    //Вставляет ключ в дерево.
    // Логорифмическое время.
    void insert(T);

    // Удаляет ключ из дерева.
    // Вызывает исключение BTREE_EXCEPTION, если не найден элемент для удаления.
    // Логорифмическое время.
    T remove(T);

    // Функция поиска ключа в дереве.
    // returnValue.first - это узел, в котором находится элемент.
    // returnValue.second - это корректный индекс в массиве ключей этого узла.
    // Логорифмическое время.
    std::pair<BNode<T> *, unsigned> search(T);

    // Использует поиск, но возвращает просто ключ, а не весь узел.
    // Полезно, когда T-это пара значений ключа, а LessThan смотрит только на ключ.
    // Выдает исключение BTREE_EXCEPTION, если не найден элемент, соответствующий параметру
    // Логорифмическое время.
    T searchKey(T);

    // Вывод дерева в консоль.
    // Линейное время.
    void print();

   private:
    // Используется для инициализации узлов
    void initializeNode(BNode<T> *);

    // Рекурсивная функция, вызываемая в деструкторе
    void freeNode(BNode<T> *);

    // Находит индекс ключа в узле
    unsigned findIndex(BNode<T> *, T);

    // Вставляет ключ в узел
    unsigned nodeInsert(BNode<T> *, T);

    // Удаляет ключ по заданному индексу из узла.
    T nodeDelete(BNode<T> *, unsigned);

    // Функция для разделения переполненных узлов.
    void splitChild(BNode<T> *, int);

    // Слияние двух дочерних узлов с заданным индексом в одного ребёнка.
    char mergeChildren(BNode<T> *, unsigned);

    // Убеждаемся, что ребёнок узла с указанным индексом имеет элементы >= minDegree.
    char fixChildSize(BNode<T> *, unsigned);

    // Рекурсивно выводит поддерево.
    void printNode(BNode<T> *, unsigned);

    // Корневой узел.
    BNode<T> *root;

    // Функция сравнения, используемая для управления размещением элементов.
    bool (*lessThan)(T, T);

    // Функция, используемая для печати элементов в дереве.
    void (*printKey)(T);

    // Минимальная степень дерева.
    unsigned minDegree;
  };
}  // namespace itis