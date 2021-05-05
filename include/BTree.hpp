
namespace itis {
  class TreeNode {
    int t;
    int n;
    bool leaf;
    TreeNode **C;
    int *keys;

   public:

    TreeNode(int temp, bool bool_leaf);
    void traverse();
    int findKey(int k);
    void insertNonFull(int k);
    void splitChild(int i, TreeNode *y);
    void remove(int k);
    void removeFromLeaf(int idx);
    void removeFromNonLeaf(int idx);
    int getPredecessor(int idx);
    int getSuccessor(int idx);
    void fill(int idx);
    void borrowFromPrev(int idx);
    void borrowFromNext(int idx);
    void merge(int idx);

    TreeNode *search(int k);

    friend class BTree;
  };

  class BTree {
    int t;

   public:
    TreeNode *root;
    BTree(int t);
    ~BTree();
    void deleteNode(TreeNode *x);
    void traverse();
    TreeNode *search(int k) {
      return (root == NULL) ? NULL : root->search(k);
    }
    void insert(int k);
    void remove(int k);
  };

}