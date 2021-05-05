// Searching a key on a B-tree in C++

#include <iostream>
#include <BTree.hpp>

using namespace std;
using namespace itis;

namespace itis {

  TreeNode::TreeNode(int t1, bool leaf1) {
    t = t1;
    leaf = leaf1;

    keys = new int[2 * t - 1];
    C = new TreeNode *[2 * t];

    n = 0;
  }

  int TreeNode::findKey(int k) {
    int idx = 0;
    while (idx < n && keys[idx] < k)
      ++idx;
    return idx;
  }

  void TreeNode::traverse() {
    int i;
    for (i = 0; i < n; i++) {
      if (leaf == false)
        C[i]->traverse();
      cout << " " << keys[i];
    }

    if (leaf == false)
      C[i]->traverse();
  }

  TreeNode *TreeNode::search(int k) {
    int i = 0;
    while (i < n && k > keys[i])
      i++;

    if (keys[i] == k)
      return this;

    if (leaf == true)
      return NULL;

    return C[i]->search(k);
  }

  void BTree::insert(int k) {
    if (root == NULL) {
      root = new TreeNode(t, true);
      root->keys[0] = k;
      root->n = 1;
    } else {
      if (root->n == 2 * t - 1) {
        TreeNode *s = new TreeNode(t, false);

        s->C[0] = root;

        s->splitChild(0, root);

        int i = 0;
        if (s->keys[0] < k)
          i++;
        s->C[i]->insertNonFull(k);

        root = s;
      } else
        root->insertNonFull(k);
    }
  }

  BTree::BTree(int temp) {
    root = NULL;
    t = temp;
  }

  BTree::~BTree() {
    deleteNode(root);
    root = nullptr;
  }

  void BTree::deleteNode(TreeNode *x) {
    if (!x->leaf) {
      for (int i = 0; i <= x->n; i++) {
        deleteNode(x->C[i]);
      }
    }
    delete (x->C);
    for (int i = 0; i < x->n; ++i) {
      remove(x->keys[i]);
    }
    delete (x->keys);
    delete (x);
  }

  void BTree::traverse() {
    if (root != NULL)
      root->traverse();
  }

  // Insert non full condition
  void TreeNode::insertNonFull(int k) {
    int i = n - 1;

    if (leaf == true) {
      while (i >= 0 && keys[i] > k) {
        keys[i + 1] = keys[i];
        i--;
      }

      keys[i + 1] = k;
      n = n + 1;
    } else {
      while (i >= 0 && keys[i] > k)
        i--;

      if (C[i + 1]->n == 2 * t - 1) {
        splitChild(i + 1, C[i + 1]);

        if (keys[i + 1] < k)
          i++;
      }
      C[i + 1]->insertNonFull(k);
    }
  }

  // split the child
  void TreeNode::splitChild(int i, TreeNode *y) {
    TreeNode *z = new TreeNode(y->t, y->leaf);
    z->n = t - 1;

    for (int j = 0; j < t - 1; j++)
      z->keys[j] = y->keys[j + t];

    if (y->leaf == false) {
      for (int j = 0; j < t; j++)
        z->C[j] = y->C[j + t];
    }

    y->n = t - 1;
    for (int j = n; j >= i + 1; j--)
      C[j + 1] = C[j];

    C[i + 1] = z;

    for (int j = n - 1; j >= i; j--)
      keys[j + 1] = keys[j];

    keys[i] = y->keys[t - 1];
    n = n + 1;
  }

  void TreeNode::remove(int k) {
    int idx = findKey(k);

    if (idx < n && keys[idx] == k) {
      if (leaf)
        removeFromLeaf(idx);
      else
        removeFromNonLeaf(idx);
    } else {
      if (leaf) {
        cout << "The key " << k << " is does not exist in the tree\n";
        return;
      }

      bool flag = ((idx == n) ? true : false);

      if (C[idx]->n < t)
        fill(idx);

      if (flag && idx > n)
        C[idx - 1]->remove(k);
      else
        C[idx]->remove(k);
    }
    return;
  }

  // Remove from the leaf
  void TreeNode::removeFromLeaf(int idx) {
    for (int i = idx + 1; i < n; ++i)
      keys[i - 1] = keys[i];

    n--;

    return;
  }

  // Delete from non leaf node
  void TreeNode::removeFromNonLeaf(int idx) {
    int k = keys[idx];

    if (C[idx]->n >= t) {
      int pred = getPredecessor(idx);
      keys[idx] = pred;
      C[idx]->remove(pred);
    }

    else if (C[idx + 1]->n >= t) {
      int succ = getSuccessor(idx);
      keys[idx] = succ;
      C[idx + 1]->remove(succ);
    }

    else {
      merge(idx);
      C[idx]->remove(k);
    }
    return;
  }

  int TreeNode::getPredecessor(int idx) {
    TreeNode *cur = C[idx];
    while (!cur->leaf)
      cur = cur->C[cur->n];

    return cur->keys[cur->n - 1];
  }

  int TreeNode::getSuccessor(int idx) {
    TreeNode *cur = C[idx + 1];
    while (!cur->leaf)
      cur = cur->C[0];

    return cur->keys[0];
  }

  void TreeNode::fill(int idx) {
    if (idx != 0 && C[idx - 1]->n >= t)
      borrowFromPrev(idx);

    else if (idx != n && C[idx + 1]->n >= t)
      borrowFromNext(idx);

    else {
      if (idx != n)
        merge(idx);
      else
        merge(idx - 1);
    }
    return;
  }

  // Borrow from previous
  void TreeNode::borrowFromPrev(int idx) {
    TreeNode *child = C[idx];
    TreeNode *sibling = C[idx - 1];

    for (int i = child->n - 1; i >= 0; --i)
      child->keys[i + 1] = child->keys[i];

    if (!child->leaf) {
      for (int i = child->n; i >= 0; --i)
        child->C[i + 1] = child->C[i];
    }

    child->keys[0] = keys[idx - 1];

    if (!child->leaf)
      child->C[0] = sibling->C[sibling->n];

    keys[idx - 1] = sibling->keys[sibling->n - 1];

    child->n += 1;
    sibling->n -= 1;

    return;
  }

  // Borrow from the next
  void TreeNode::borrowFromNext(int idx) {
    TreeNode *child = C[idx];
    TreeNode *sibling = C[idx + 1];

    child->keys[(child->n)] = keys[idx];

    if (!(child->leaf))
      child->C[(child->n) + 1] = sibling->C[0];

    keys[idx] = sibling->keys[0];

    for (int i = 1; i < sibling->n; ++i)
      sibling->keys[i - 1] = sibling->keys[i];

    if (!sibling->leaf) {
      for (int i = 1; i <= sibling->n; ++i)
        sibling->C[i - 1] = sibling->C[i];
    }

    child->n += 1;
    sibling->n -= 1;

    return;
  }

  // Merge
  void TreeNode::merge(int idx) {
    TreeNode *child = C[idx];
    TreeNode *sibling = C[idx + 1];

    child->keys[t - 1] = keys[idx];

    for (int i = 0; i < sibling->n; ++i)
      child->keys[i + t] = sibling->keys[i];

    if (!child->leaf) {
      for (int i = 0; i <= sibling->n; ++i)
        child->C[i + t] = sibling->C[i];
    }

    for (int i = idx + 1; i < n; ++i)
      keys[i - 1] = keys[i];

    for (int i = idx + 2; i <= n; ++i)
      C[i - 1] = C[i];

    child->n += sibling->n + 1;
    n--;

    delete (sibling);
    return;
  }

  void BTree::remove(int k) {
    if (!root) {
      cout << "The tree is empty\n";
      return;
    }

    root->remove(k);

    if (root->n == 0) {
      TreeNode *tmp = root;
      if (root->leaf)
        root = NULL;
      else
        root = root->C[0];

      delete tmp;
    }
    return;
  }
}  // namespace itis