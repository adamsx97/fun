#include <iostream>
#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;

/*
  int *values;

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

LeafNode::LeafNode(int LSize, InternalNode *p,
  BTreeNode *left, BTreeNode *right) : BTreeNode(LSize, p, left, right)
{
  values = new int[LSize];
}  // LeafNode()



int LeafNode::getMinimum()const
{
  if(count > 0)  // should always be the case
    return values[0];
  else
    return 0;

} // LeafNode::getMinimum()


LeafNode* LeafNode::insert(int value)
{
  if (count < leafSize)
  {
    insertDirectly(value);
    return NULL;
  }
  else
  {
    if (leftSibling && leftSibling->getCount() < leafSize)
    {
      if (value < this->getMinimum())
        leftSibling->insert(value);
      else
      {

        moveToLeft();
        insertDirectly(value);
      }
    } // insert to left
    else if(rightSibling && rightSibling->getCount() < leafSize)
    {
      if (value > values[count - 1])
        rightSibling->insert(value);
      else // if value is not the biggest one
      {
        moveToRight();
        insertDirectly(value);
      }
    } // insert to right
    else // no place in current leaf node and left&right sibling
    {
      return split(value);
    }
  }
  return NULL; // to avoid warnings for now.
}  // LeafNode::insert()

void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()

LeafNode* LeafNode::split(int value)
{
  int last, i;
  if (value > values[count - 1])
    last = value;
  else
  {
    last = values[count - 1];
    for (i = count - 2; i >= 0 && values[i] > value; i--)
      values[i + 1] = values[i];
    values[i + 1] = value;
  }

  LeafNode *newNode = new LeafNode(leafSize, parent, this, rightSibling);

  if(rightSibling)
    rightSibling->setLeftSibling(newNode);
  rightSibling = newNode;
  for (i = (leafSize + 1) / 2; i < leafSize; i++)
    newNode->insert(values[i]);
  count = (leafSize + 1 ) / 2;
  newNode->insert(last);
  return newNode;
} // split()

void LeafNode::insertDirectly(int value)
{
  if (count == 0)
  {
    values[count] = value;
    count++;
    return;
  } // if no element in curr leaf node

  int i;
  for (i = count - 1; i >= 0 && value < values[i]; i--)
    values[i + 1] = values[i];
  values[i + 1] = value;
  count++; 
  return;
} // insertDirectly()

  
void LeafNode::moveToLeft()
{
  leftSibling->insert(values[0]);
  for (int i = 0; i < count - 1; i++)
    values[i] = values[i + 1];
  count--;
} // insertToLeft()

void LeafNode::moveToRight()
{
  rightSibling->insert(values[count - 1]);
  count--;
} // moveToRight()
