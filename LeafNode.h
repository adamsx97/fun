#ifndef LeafNodeH
#define LeafNodeH

#include "BTreeNode.h"

/*
  int count;
  int leafSize;
  InternalNode *parent;
  BTreeNode *leftSibling;
  BTreeNode *rightSibling;
  int getCount() const;
  
  BTreeNode* getLeftSibling();
  virtual int getMinimum()const = 0;
  BTreeNode* getRightSibling();
  virtual BTreeNode* insert(int value) = 0;
  virtual void print(Queue <BTreeNode*> &queue) = 0;
  void setLeftSibling(BTreeNode *left);
  void setParent(InternalNode *p);
  void setRightSibling(BTreeNode *right);
*/

class LeafNode:public BTreeNode
{
  int *values;
public:
  LeafNode(int LSize, InternalNode *p, BTreeNode *left,
    BTreeNode *right);
  int getMinimum() const;
  LeafNode* insert(int value); // returns pointer to new Leaf if splits
  // else NULL
  void print(Queue <BTreeNode*> &queue);
  LeafNode* split(int value);
  void insertDirectly(int value);
  void moveToLeft();
  void moveToRight();
}; //LeafNode class

#endif
