/*
Author:      Jacob Hanafin
Assignment:  Binary Search Tree

Summary:

*/

#include <iostream>
#include <string>   //Just used to print blank lines to clear screen
#include <stdlib.h> //exit()
#include <fstream>  // file handling support
#include <cmath>    // floor and log2 functions

using namespace std;

//Tree nodes
struct NODE
{
  int number;
  NODE * Lchild;
  NODE * Rchild;
};

//Prototypes First
void insert( NODE * & root, const int );
void loadtree(NODE * & tree);
void repeat();
int Menu(int);
void BFT(NODE * tree);
int count_nodes(NODE * t);
void balance_tree(NODE * & tree);
void fill_data(NODE * t,int data[],int &count);
void delete_all(NODE * & t);
void lookUp(NODE * & t, int);
void balance(NODE * &root, int, int data[], int &fp);
int tree_height(NODE * tree, int);
void sorted(NODE * tree);
void rootNode(NODE * tree);
void AddNode(NODE * tree);
void removeNode(NODE * &tree, int);
void addRemoveMenu(NODE * tree);
NODE * findMin(NODE * tree);
void delete_all(NODE * &t);


int main(void){
  NODE * tree = NULL;  //empty tree
  int n; 				// # of nodes in a tree
  int UserChoice = 1;
  int node_select = 0;

  while (UserChoice != 0){
    UserChoice = Menu(UserChoice);
    if (UserChoice == 1){
      loadtree(tree);
    //  repeat();
    } else if (UserChoice == 2){
      balance_tree(tree);
    //  repeat();
    } else if (UserChoice == 3){
      cout << "What number would you like to look up?" << endl;
      cout << "User  input: ";
      cin >> node_select;
      lookUp(tree, node_select);
    } else if (UserChoice == 4){
      cout << "TREE HEIGHT " << tree_height(tree,0) << endl << endl;
    } else if (UserChoice == 5){
      sorted(tree);
    } else if (UserChoice == 6){
      rootNode(tree);
    } else if (UserChoice == 7){
      addRemoveMenu(tree);
    } else if (UserChoice == 8){
      cout << delete_all(tree) << endl;
    } else {
      cout << "" << endl;
    }
  }
  return 0;
}
//*****************************************************************************
int Menu(int input){

  input = 0;

  cout << string(50, '\n'); // 100 \n's
  cout << "Author: Jacob Hanafin        " << endl;
  cout << "User's Choices:              " << endl;
  cout << " -----------------------------" << endl;
  cout << "| Create a New tree:        1 |" << endl;
  cout << "| Balance the Tree:         2 |" << endl;
  cout << "| Number Lookup:            3 |" << endl;
  cout << "| Tree Height Report:       4 |" << endl;
  cout << "| A Sorted Listing:         5 |" << endl;
  cout << "| Display Root Node Value:  6 |" << endl;
  cout << "| Add/Remove Value to tree: 7 |" << endl;
  cout << "| Delete Tree               8 |" << endl;
  cout << "| End Program:              0 |" << endl;
  cout << " -----------------------------" << endl;
  cout << "User Choice: ";
  cin >> input;
//  cout << input << endl;
  return input;
}
//*****************************************************************************
void repeat(){
  int r;
  cout << "" << endl;
  cout << "Would you like to continue?" << endl;
  cout << "1. Yes" << endl;
  cout << "2. No" << endl;
  cout << "User Choice: ";
  cin >> r;
  if (r != 1){
    throw;
  }
}
//*****************************************************************************
void insert( NODE * & root, const int num)
{
   if ( root == NULL) // empty  so create leaf  - ground
   {
      root = new NODE;
	    root->number = num;
      root->Lchild = NULL;
      root->Rchild = NULL;
   }
   if (root->number == num)  //duplicate insert attempt - ground
      return;
   // not empty
   if (num < root->number) // go left
      insert(root->Lchild,num);
   else
      insert(root->Rchild,num); //go right
}
//*****************************************************************************
void loadtree(NODE * & tree)
{
  ifstream infile;
  int num;
  // open the file
  infile.open("treedata.txt");  // must be in same directory as program
  if (!infile) {  //If the file is not found
    cerr << "Unable to open file treedata.txt";
    exit(1); //System stop, if failed
  }
  infile >> num; //Will read first number in file (make sure it is not -1 on start)
  while (num != -1){  // num not the EOF flag -1
    insert(tree, num);
	   infile >> num;
  }
  infile.close();
  cout << "File Is Loaded" << endl;
}

//*****************************************************************************
// Balance a Binary Search Tree

void balance_tree(NODE * & tree)
{
  if (tree == NULL) return; // empty tree

  // count the number of nodes in the tree
  int node_count = 0;
  node_count = count_nodes(tree);
  // create a dymaic arra to hold sorted list of terr values
  int * data = new int[node_count];

  int count = -1;
  fill_data(tree,data,count); // fill date

  // remove old tree
  delete_all(tree); // delete the whole tree
  tree = NULL;  // just in case :)
  //crerate  a balanced  the tree from values in data array
  int fp = 0;
  balance(tree,node_count,data,fp);

  //clean up the dynamic data array from heap.
  delete[]  data;
}

void balance(NODE * &root, int num, int data[], int &fp)
{
	if (num > 0) //ground condition
	{
		root = new NODE;  // creat a new empty node to fill up after balancing Left SUb tree
		root->Lchild = NULL;
		root->Rchild = NULL;
		balance(root->Lchild,num/2,data,fp);
		root->number = data[fp];
		fp++;  // move to next value in data array
		balance(root->Rchild,(num-1)/2,data,fp); //blance right sub-tree
	}
}

void delete_all(NODE * & t)
{
  if (t == NULL)  // ground condition
     return;
  delete_all(t->Lchild); // delete left subtree
  delete_all(t->Rchild); // delete right subtree
  delete t;  // free parent node on the heap
  t = NULL;  // set tree to empty
}

void fill_data(NODE * t,int data[],int &count)
{
	if (t == NULL) return;  // GROUND cond.

	fill_data(t->Lchild,data,count);

	count++;
	data[count] = t->number;

	fill_data(t->Rchild,data,count);
}

//Testing Functions
int count_nodes(NODE * t)
{
   if (t == NULL)  // ground condition
     return 0;

   return(count_nodes(t->Lchild)+ count_nodes(t->Rchild)+ 1);
}

void BFT(NODE * tree)
{
   if (tree == NULL)
      return;

   NODE ** queue = new NODE *[count_nodes(tree)];

   int enqueue = 0;
   int dequeue = 0;
   NODE * p;

   p = tree;

   if (p != NULL)
   {
      queue[enqueue++] = p;
      while (!(enqueue == dequeue)) // queue not empty
      {
	     p = queue[dequeue++];
		 cout << p->number << endl; // processing
		 if (p->Lchild != NULL)
		     queue[enqueue++] = p-> Lchild;
		 if (p->Rchild != NULL)
		     queue[enqueue++] = p-> Rchild;
      }
    }
}

void lookUp( NODE * & t, int i){
  NODE *temp = t;
  while (temp != NULL){
    if (temp->number == i){
      break;
    } else if (temp->number > i){
      temp = temp->Lchild;
    } else if (temp->number < i){
        temp = temp->Rchild;
    }
  }
  if (temp == NULL){
      cout << "NOT FOUND" << endl;
  } else if (temp ->number == i){
      cout << "FOUND" << endl;
  }
}

// find the height of a binary tree
int tree_height(NODE * tree, int height)
{
   if (tree == NULL)  // ground
      return height;

   height = height + 1;

   int Ltree_height = tree_height(tree->Lchild, height);
   int Rtree_height = tree_height(tree->Rchild, height);

   if (Ltree_height >= Rtree_height)
       return Ltree_height;
   else
       return Rtree_height;
}

void sorted(NODE * tree){

  if (tree == NULL){
    return;
  }
  sorted(tree->Lchild);
  cout << tree->number << endl;
  sorted(tree->Rchild);
}

void rootNode(NODE * tree){
  cout << tree->number << endl;
}

void addRemoveMenu(NODE * tree){
  int i = 0;
  int val = 0;

  cout << "Would you like to add or remove a value?" << endl;
  cout << "1. Add" << endl;
  cout << "2. Remove" << endl;
  cout << "User choice: ";
  cin >> i;
  if (i == 1){
    AddNode(tree);
  } else if (i == 2){
    cout << "What value would you like to remove?" << endl;
    cout << "User Choice: ";
    cin >> val;
    removeNode(tree, val);
  } else {
    cout << "" << endl;
  }
}

void AddNode(NODE * tree){
  int val = 0;
  cout << "What value would you like to add?" << endl;;
  cout << "User Choice: ";
  cin >> val;
  insert(tree, val);
}

void removeNode(NODE * &tree, int val){
  NODE *tmp = NULL;

  if (tree == NULL){
    return;
  }

  if (tree->number == val){
    if (tree->Lchild == NULL){
      tmp = tree;
      tree = tree->Rchild;
      delete tmp;
    } else if (tree->Rchild == NULL){
      tmp = tree;
      tree = tree->Lchild;
      delete tmp;
    } else {
      tmp = findMin(tree->Rchild);
      tree->number = tmp->number;
      removeNode(tree->Rchild, tree->number);
    }
  } else if( tree->Rchild == NULL) {
    removeNode(tree->Lchild, val);
  } else {
    removeNode(tree->Rchild, val);
  }
}

NODE * findMin(NODE * tree){
    if(tree->Lchild == NULL){
      return tree;
    } else {
      return findMin(tree->Lchild);
    }
}
