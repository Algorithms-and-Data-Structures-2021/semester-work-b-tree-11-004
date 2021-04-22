#include "data_structure.hpp"

#pragma once

#include <stdlib.h>
#include <utility>
#include <stdio.h>

using namespace std;

#define NEW_ROOT 2
#define MODIFIED_NOT_ROOT 1
#define NOT_MODIFIED 0

namespace itis {
  /* Конструктор для Б-дерева.
  * t - минимальная степень дерева.
  * compare - функция сравнения, используемая для управления элементами внутри дерева.
  * printK - функция, которая печатает ключи.
  */
  template<typename T>
  BTree<T>::BTree(unsigned t, bool (*compare)(T, T), void (*printK)(T)) {
    minDegree = t;
    lessThan = compare;
    root = (BNode<T> *) malloc(sizeof(BNode<T>));
    initializeNode(root);
    root->leaf = true;
    printKey = printK;
  }

  // Деструктор.
  template<typename T>
  BTree<T>::~BTree<T>() {
    freeNode(root);
  }

  // Вставляет ключ k в дерево.
  template<typename T>
  void BTree<T>::insert(T k) {

    // Дерево растёт вверх, если корень полон.
    if (root->size == 2 * minDegree - 1) {
      BNode<T> *newRoot = (BNode<T> *) malloc(sizeof(BNode<T>));
      initializeNode(newRoot);
      newRoot->leaf = false;
      newRoot->child[0] = root;
      root = newRoot;
      splitChild(newRoot, 0);
    }

    // Работа вниз по дереву.
    BNode<T> *curr = root;
    while (!curr->leaf) {

      // Находим подходящего ребенка.
      int index = curr->size - 1;
      while (index >= 0 && lessThan(k, curr->key[index])) {
        index--;
      }
      index++;

      // Делим ребенка, если он полон.
      if (curr->child[index]->size == 2 * minDegree - 1) {
        splitChild(curr, index);
        if (lessThan(curr->key[index], k)) {
          index++;
        }
      }
      curr = curr->child[index];
    }

    nodeInsert(curr, k);
  }

  // Удаляет ключ k из дерева. Возвращает удаленный ключ.
  // Выдает исключение BTREE_EXCEPTION, если ключ не найден.
  // curr - текущий корень.
  template<typename T>
  T BTree<T>::remove(T k) {
    BNode<T> *curr = root;
    while (true) {
      unsigned i = findIndex(curr, k);

      // Если элемент, подлежащий удалению, был найден.
      if (i < curr->size && !(lessThan(curr->key[i], k) || lessThan(k, curr->key[i]))) {
        T toReturn = curr->key[i];

        // Если на листе, просто удаляем его.
        if (curr->leaf) {
          nodeDelete(curr, i);
        }

        // Иначе заменим родителя или объединим детей
        else {
          BNode<T> *leftKid = curr->child[i];
          BNode<T> *rightKid = curr->child[i + 1];

          // Заменить на родителя
          if (leftKid->size >= minDegree) {
            while (!(leftKid->leaf)) {
              fixChildSize(leftKid, leftKid->size);
              leftKid = leftKid->child[leftKid->size];
            }
            curr->key[i] = nodeDelete(leftKid, leftKid->size - 1);
          }

          // Заменить на ребёнка
          else if (rightKid->size >= minDegree) {
            while (!(rightKid->leaf)) {
              fixChildSize(rightKid, 0);
              rightKid = rightKid->child[0];
            }
            curr->key[i] = nodeDelete(rightKid, 0);
          }

          // Объединяем детей и двигаемся вниз по дереву.
          else {
            mergeChildren(curr, i);
            curr = leftKid;
            continue;
          }
        }
        return toReturn;
      }

      // Если элемент не найден, двигайтесь вниз по дереву.
      else {

        // Если у листа, то элемента нет.
        if (curr->leaf) {
          throw(BTREE_EXCEPTION) REMOVE_KEY_NOT_FOUND;
        }

        // Отрегулируем curr и двигаемся вниз по дереву.
        char result = fixChildSize(curr, i);
        if (result == NEW_ROOT) {
          curr = root;
        } else {
          curr = curr->child[findIndex(curr, k)];
        }
      }
    }
  }

  // Функция поиска ключа в дереве.
  // returnValue.first - это узел, в котором находится элемент.
  // returnValue.second - это корректный индекс в массиве ключей этого узла.
  template<typename T>
  pair<BNode<T> *, unsigned> BTree<T>::search(T k) {

    // Начинаем с корня.
    BNode<T> *x = root;

    // Работаем вниз по дереву.
    while (true) {

      // Находим правильный индекс в массиве текущего узла.
      unsigned i = findIndex(x, k);

      // Нашли.
      if (i < x->size && !(lessThan(k, x->key[i]) || lessThan(x->key[i], k))) {
        return pair<BNode<T> *, unsigned>(x, i);
      }

      // Упёрлись в листья.
      else if (x->leaf) {
        return pair<BNode<T> *, unsigned>(NULL, 0);
      }

      // Продолжаем.
      else {
        x = x->child[i];
      }
    }
  }

  // Функция поиска ключа в дереве.
  // Возвращает ключ.
  // Если элемент не был найден, создается исключение.
  template<typename T>
  T BTree<T>::searchKey(T k) {
    pair<BNode<T> *, unsigned> node = search(k);
    if (node.first == NULL) {
      throw(BTREE_EXCEPTION) SEARCH_KEY_NOT_FOUND;
    }
    return node.first->key[node.second];
  }

  // Вывод дерева в консоль
  template<typename T>
  void BTree<T>::print() {
    if (printKey != NULL && root != NULL) {
      printf("\n");
      printNode(root, 0);
      printf("\n");
    }
  }

  // Инициализировать узел Би-дерева.
  // x - указатель на узел.
  // t - минимальная степень дерева.
  template<typename T>
  void BTree<T>::initializeNode(BNode<T> *x) {
    x->size = 0;
    x->key = (T *) malloc((2 * minDegree - 1) * sizeof(T));
    x->child = (BNode<T> **) malloc(2 * minDegree * sizeof(BNode<T> *));
  }

  // Рекурсивно удаляет поддерево с корнем x.
  // Выполняет грязную работу для деструктора.
  template<typename T>
  void BTree<T>::freeNode(BNode<T> *x) {
    if (!x->leaf) {
      for (unsigned i = 0; i <= x->size; i++) {
        freeNode(x->child[i]);
      }
    }
    free(x->child);
    free(x->key);
    free(x);
  }

  // Находит индекс k в x->key.
  // Если k отсутствует, возвращает индекс поддерева
  // который может содержать k в x->child.
  template<typename T>
  unsigned BTree<T>::findIndex(BNode<T> *x, T k) {
    unsigned i = 0;
    while (i < x->size && lessThan(x->key[i], k)) {
      i++;
    }
    return i;
  }

  // Вставляет ключ k в x.
  // Возвращает индекс k в x->key.
  template<typename T>
  unsigned BTree<T>::nodeInsert(BNode<T> *x, T k) {
    int index;

    // Освобождаем место для k.
    for (index = x->size; index > 0 && lessThan(k, x->key[index - 1]); index--) {
      x->key[index] = x->key[index - 1];
      x->child[index + 1] = x->child[index];
    }

    // Вставляем k.
    x->child[index + 1] = x->child[index];
    x->key[index] = k;
    x->size++;

    return index;
  }

  // Удаляет элемент по индексу в x->key.
  // Возвращает удалённый ключ.
  template<typename T>
  T BTree<T>::nodeDelete(BNode<T> *x, unsigned index) {

    T toReturn = x->key[index];

    x->size--;
    while (index < x->size) {
      x->key[index] = x->key[index + 1];
      x->child[index + 1] = x->child[index + 2];
      index++;
    }
    return toReturn;
  }

  // Функция для разделения узлов, которые слишком заполнены.
  // x указывает на родительский узел для разбиения.
  // i - индекс в дочернем массиве x разбитого узла.
  template<typename T>
  void BTree<T>::splitChild(BNode<T> *x, int i) {

    // Z - новый узел, а Y - узел для разделения.
    BNode<T> *toSplit = x->child[i];
    BNode<T> *newNode = (BNode<T> *) malloc(sizeof(BNode<T>));
    ;
    initializeNode(newNode);
    newNode->leaf = toSplit->leaf;
    newNode->size = minDegree - 1;

    // Скопируем вторую половину ключей Y и детей в Z.
    for (unsigned j = 0; j < minDegree - 1; j++) {
      newNode->key[j] = toSplit->key[j + minDegree];
    }
    if (!toSplit->leaf) {
      for (unsigned j = 0; j < minDegree; j++) {
        newNode->child[j] = toSplit->child[j + minDegree];
      }
    }
    toSplit->size = minDegree - 1;

    nodeInsert(x, toSplit->key[minDegree - 1]);
    x->child[i + 1] = newNode;
  }

  // Объединяет (i + 1) - ый ребёнка с i-м ребёнком.
  // Возвращает индикатор того, повлияло ли изменение на корень.
  template<typename T>
  char BTree<T>::mergeChildren(BNode<T> *parent, unsigned i) {

    BNode<T> *leftKid = parent->child[i];
    BNode<T> *rightKid = parent->child[i + 1];

    // Переместите элемент из родителя в левого ребёнка.
    leftKid->key[leftKid->size] = nodeDelete(parent, i);
    unsigned j = ++(leftKid->size);

    // Переместите всё из правого ребёнка в левого.
    for (unsigned k = 0; k < rightKid->size; k++) {
      leftKid->key[j + k] = rightKid->key[k];
      leftKid->child[j + k] = rightKid->child[k];
    }
    leftKid->size += rightKid->size;
    leftKid->child[leftKid->size] = rightKid->child[rightKid->size];

    // Освобождаем память, используемую правым ребёнком
    free(rightKid->child);
    free(rightKid->key);
    free(rightKid);

    // Если родитель пуст, то это должен быть корень.
    if (parent->size == 0) {
      root = leftKid;
      free(parent->child);
      free(parent->key);
      free(parent);
      return NEW_ROOT;
    }

    return MODIFIED_NOT_ROOT;
  }

  // Удостоверяется, что parent->child[index] имеет по крайней мере элементы minDegree.
  // Если это не так, то все меняется, чтобы убедиться, что это так.
  // Возвращает код, указывающий, какое действие было предпринято.
  template<typename T>
  char BTree<T>::fixChildSize(BNode<T> *parent, unsigned index) {
    BNode<T> *kid = parent->child[index];

    // Если что-то нужно исправить.
    if (kid->size < minDegree) {

      // Одолжим у левого брата, если это возможно.
      if (index != 0 && parent->child[index - 1]->size >= minDegree) {
        BNode<T> *leftKid = parent->child[index - 1];

        // При наличии множества эквивалентных ключей,
        // nodeInsert может вставляться в индекс, отличный от 0.
        // Цикл for фиксирует дочерние указатели, если это происходит.
        for (unsigned i = nodeInsert(kid, parent->key[index - 1]); i != 0; i--) {
          kid->child[i] = kid->child[i - 1];
        }
        kid->child[0] = leftKid->child[leftKid->size];
        parent->key[index - 1] = nodeDelete(leftKid, leftKid->size - 1);
      }

      // Одолжим у правого брата, если это возможно
      else if (index != parent->size && parent->child[index + 1]->size >= minDegree) {
        BNode<T> *rightKid = parent->child[index + 1];
        // Move curr->key[i] into kid->key
        nodeInsert(kid, parent->key[index]);
        kid->child[kid->size] = rightKid->child[0];
        rightKid->child[0] = rightKid->child[1];
        // Move rightKid->key[0] into curr->key
        parent->key[index] = nodeDelete(rightKid, 0);
      }

      // Если заимствование невозможно, тогда слияние.
      else if (index != 0) {
        return mergeChildren(parent, index - 1);
      } else {
        return mergeChildren(parent, index);
      }
      return MODIFIED_NOT_ROOT;
    }

    // Если элементы не нуждаются в починке.
    return NOT_MODIFIED;
  }

  // Рекурсивная функция для печати дерева или поддерева.
  // node - корень поддерева, которое будет напечатано.
  // tab - расстояние отступа поддерева.
  template<typename T>
  void BTree<T>::printNode(BNode<T> *node, unsigned tab) {

    // Отступ
    for (unsigned i = 0; i < tab; i++) {
      printf("\t");
    }

    // Печать текущего узла.
    for (unsigned i = 0; i < node->size; i++) {
      printKey(node->key[i]);
      printf(" ");
    }
    printf("\n");

    // Печать всех детей.
    if (!node->leaf) {
      tab++;
      for (unsigned i = 0; i <= node->size; i++) {
        printNode(node->child[i], tab);
      }
    }
  }

}  // namespace itis