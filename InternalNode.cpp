#include <iostream>
#include "InternalNode.h"

using namespace std;

/*
  int internalSize;
  BTreeNode **children;
  int *keys;
  int count;
  int leafSize;
  InternalNode *parent;
  BTreeNode *leftSibling;
  BTreeNode *rightSibling;
  BTreeNode(int LSize, InternalNode *p, BTreeNode *left, BTreeNode *right);
  virtual ~BTreeNode() {}  
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

InternalNode::InternalNode(int ISize, int LSize,
  InternalNode *p, BTreeNode *left, BTreeNode *right) :
  BTreeNode(LSize, p, left, right), internalSize(ISize)
{
  keys = new int[internalSize]; // keys[i] is the minimum of children[i]
  children = new BTreeNode* [ISize];
} // InternalNode::InternalNode()


int InternalNode::getMinimum()const
{
  if(count > 0)   // should always be the case
    return children[0]->getMinimum();
  else
    return 0;
} // InternalNode::getMinimum()


InternalNode* InternalNode::insert(int value)
{
  updateKeys();
  BTreeNode* newNode = NULL;

  int flag = 1;
  for (int i = 0; i < count; i++)
    if (value < keys[i])
    {
      i = (i == 0) ? i : i - 1;
      newNode = children[i]->insert(value);
      flag = 0;
      break;
    } // find child to insert
  if (flag)
    newNode = children[count - 1]->insert(value); // insert at the largest if larger than all
  updateKeys();
  if (newNode)
  {
    if (count < internalSize)
      insert(newNode);
    else
    {
      if (leftSibling && leftSibling->getCount() < internalSize)
      {
        if (newNode->getMinimum() < this->getMinimum())
          ((InternalNode*)leftSibling)->insert(newNode);
        else
        {
          moveToLeft();
          insert(newNode);
        }
      } // insert to left
      else if(rightSibling && rightSibling->getCount() < internalSize)
      {
        if (newNode->getMinimum() > keys[count - 1])
          ((InternalNode*)rightSibling)->insert(newNode);
        else // if value is not the biggest one
        {
          moveToRight();
          insert(newNode);
        }
      } // insert to right
      else // no place in current leaf node and left&right sibling
      {
        InternalNode* temp = split(newNode);
        updateKeys();
        return temp;
      }
    }
  } // if children splits add newNode
  updateKeys();
  return NULL; // to avoid warnings for now.
} // InternalNode::insert()

void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2)
{ // Node must be the root, and node1
  children[0] = oldRoot;
  children[1] = node2;
  keys[0] = oldRoot->getMinimum();
  keys[1] = node2->getMinimum();
  count = 2;
  children[0]->setLeftSibling(NULL);
  children[0]->setRightSibling(children[1]);
  children[1]->setLeftSibling(children[0]);
  children[1]->setRightSibling(NULL);
  oldRoot->setParent(this);
  node2->setParent(this);
} // InternalNode::insert()

void InternalNode::insert(BTreeNode *newNode) // from a sibling
{
  updateKeys();
  newNode->setParent(this);
  if (count == 0)
  {
    children[count] = newNode;
    count++;
    updateKeys();
    return;
  } // if no element in curr leaf node

  int i;
  for (i = count - 1; i >= 0 && newNode->getMinimum() < keys[i]; i--)
    children[i + 1] = children[i];
  children[i + 1] = newNode;
  count++;
  updateKeys();
  return;
} // InternalNode::insert() insertDirectly()

void InternalNode::print(Queue <BTreeNode*> &queue)
{
  int i;

  cout << "Internal: ";
  for (i = 0; i < count; i++)
    cout << keys[i] << ' ';
  cout << endl;

  for(i = 0; i < count; i++)
    queue.enqueue(children[i]);

} // InternalNode::print()

InternalNode* InternalNode::split(BTreeNode* newNode)
{
  BTreeNode* last;
  int i;
  if (newNode->getMinimum() > keys[count - 1])
    last = newNode;
  else
  {
    last = children[count - 1];
    for (i = count - 2; i >= 0 && keys[i] > newNode->getMinimum(); i--)
      children[i + 1] = children[i];
    children[i + 1] = newNode;
    updateKeys();
  }

  InternalNode *newNode1 = new InternalNode(internalSize, leafSize, parent, this, rightSibling);

  if(rightSibling)
    rightSibling->setLeftSibling(newNode1);
  rightSibling = newNode1;
  for (i = (internalSize + 1) / 2; i < internalSize; i++)
    newNode1->insert(children[i]);
  count = (internalSize + 1 ) / 2;
  newNode1->insert(last);
  return newNode1;
} // split()
  
void InternalNode::moveToLeft()
{
  ((InternalNode*)leftSibling)->insert(children[0]);
  for (int i = 0; i < count - 1; i++)
    children[i] = children[i + 1];
  count--;
  updateKeys();
} // insertToLeft()

void InternalNode::moveToRight()
{
  ((InternalNode*)rightSibling)->insert(children[count - 1]);
  count--;
  updateKeys();
} // moveToRight()

void InternalNode::updateKeys()
{
  for (int i = 0; i < count; i++)
    keys[i] = children[i]->getMinimum();
} // updateKeys()
