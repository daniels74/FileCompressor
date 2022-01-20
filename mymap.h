// mymap.h
// hold the contents for the mymap class which replicates the regular map functionality.
// This includes the ability to self balance itself using a self balancing system.
//
#pragma once

#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

template<typename keyType, typename valueType>
class mymap {
 private:
    struct NODE {
        keyType key;  // used to build BST
        valueType value;  // stored data for the map
        NODE* left;  // links to left child
        NODE* right;  // links to right child
        int nL;  // number of nodes in left subtree
        int nR;  // number of nodes in right subtree
        bool isThreaded;
    };
    NODE* root;  // pointer to root node of the BST
    int size;  // # of key/value pairs in the mymap
    //
    // iterator:
    // This iterator is used so that mymap will work with a foreach loop.
    //
    struct iterator {
     private:
        NODE* curr;  // points to current in-order node for begin/end

     public:
        iterator(NODE* node) {  // sets the current node
            curr = node;
        }

        keyType operator *() {  // () operator for access to the key
            return curr -> key;
        }

        bool operator ==(const iterator& rhs) {  //  = operator for comparing cur node with other
            if (curr == nullptr && rhs.curr == nullptr) {
              return this == &rhs;
            }
            return curr == rhs.curr;
        }

        bool operator !=(const iterator& rhs) {  // =! operator for comparing cur with another node
            if (curr == nullptr && rhs.curr == nullptr) {
              return this == &rhs;
            }
            return curr != rhs.curr;
        }

        bool isDefault() {   //  idk how this works
            return !curr;
        }

        //
        // operator++:
        //
        // This function should advance curr to the next in-order node.
        // O(logN)
        //
        iterator operator++() {
          // if the node is threaded
          if (curr->isThreaded) {
            curr = curr->right;
            // cur = (cur->isThreaded) ? nullptr : cur->right;
          }
          // if the node is not  threaded
          else if(!curr->isThreaded) {
            curr = curr->right;

            NODE* cur = curr;

            NODE* prev = nullptr;
            // Move left all the way
            while (cur != nullptr) {
              prev = cur;

              cur = cur->left;
            }
            // curr holds the left most node
            curr = prev;
          }
            return iterator(curr);
        }
    };

 public:
    //
    // default constructor:
    //
    // Creates an empty mymap.
    // Time complexity: O(1)
    //
    mymap() {
    this->root = nullptr;
    this->size = 0;
    }

    //
    //
    // deleteTree: deletes every node in the tree
    // In-Order fashion | Left, Action, Right
    //
    //
    void deleteTree(NODE*& node) {

      // nothing to delete
      if (node == nullptr) {
       return;
       }

      deleteTree(node->left);
      NODE* nodeNew = (node->isThreaded) ? nullptr : node->right;
      deleteTree(nodeNew);

      delete node;
    }

    //
    //
    // copyPreOrder: copies every node onto this tree
    // Pre-Order fashion | Action, Left, Right
    //
    //
    void copyPreOrder(NODE* other) {
      if (other == nullptr) {
       return;
      }
      else {
      put(other->key, other->value);

      copyPreOrder(other->left);  // left

      other = (other->isThreaded) ? nullptr : other->right;

      copyPreOrder(other);  // right
      }
    }

    //
    // copy constructor:
    //
    // Constructs a new mymap which is a copy of the "other" mymap.
    // Sets all member variables appropriately.
    // Time complexity: O(n), where n is total number of nodes in threaded,
    // self-balancing BST.
    //
    mymap(const mymap& other) {
      this->root = nullptr;
      this->size = 0;
      copyPreOrder(other.root);
    }

    //
    // operator=:
    //
    // Clears "this" mymap and then makes a copy of the "other" mymap.
    // Sets all member variables appropriately.
    // Time complexity: O(n), where n is total number of nodes in threaded,
    // self-balancing BST.
    //
    mymap& operator=(const mymap& other) {
      // delete this mymap
      deleteTree(this->root);
      this->size = 0;

      // if other mymap is empty just return an empty mymap
      if (other.root == nullptr) {
       return *this;
      }

      // create first node for tree copy and start copying
      //this->root = new NODE();
      this->root = nullptr;

      copyPreOrder(other.root);

        return *this;
    }

    //     // clear:
    //     //
    //     // Frees the memory associated with the mymap; can be used for testing.
    //     // Time complexity: O(n), where n is total number of nodes in threaded,
    //     // self-balancing BST.
    //     //
    void clear() {
      deleteTree(this->root);
      size = 0;
    }

    //
    // destructor:
    //
    // Frees the memory associated with the mymap.
    // Time complexity: O(n), where n is total number of nodes in threaded,
    // self-balancing BST.
    //
    ~mymap() {
      if(this->size != 0) {
        deleteTree(this->root);
       }
    }

    //
    //
    // findNode: searches for given node in tree
    // if its found returns pointer to it
    // if not found it reuturns the given node
    //
    //
    NODE* findNode(NODE* i) {
      NODE* cur = root;
      NODE* notFound = nullptr;

      while (cur != nullptr) {
        if (cur == i) {  // node found
          return cur;
       }
        if (cur->key > i->key) {
          cur = cur->left;
       }
        else if (cur->key < i->key) {
         cur = (cur->isThreaded) ? nullptr : cur->right;
       }
     }
     return notFound;
  }

    //
    //
    // makeNewNode() makes a new node with given key and value and returns it
    // default values for the rest of the node's variables
    //
    //
    NODE* makeNewNode(keyType key, valueType value) {
      NODE* newNode = new NODE();

      newNode->key = key;

      newNode->value = value;

      newNode->nL = 0;

      newNode->nR = 0;

      newNode->left = nullptr;

      newNode->right = nullptr;

      newNode->isThreaded = true;

      return newNode;
    }

    //
    //
    // SearchTree: searches the tree from the root given a key and cur prev pointers.
    // Updates the travelNodes vector and sets isKeyFound = true if the node is found while storing it in foundNode
    //
    //
    void searchTree(NODE*& cur, NODE*& prev, keyType& key, vector<NODE*>& travelNodes, bool& isKeyFound, NODE*& foundNode) {

      while (cur != nullptr) {
        prev = cur;

        if (cur->key == key) {
           isKeyFound = true;

           foundNode = cur;

           return;
        }
        if (cur->key > key) {
          travelNodes.push_back(cur);

          cur = cur->left;
        }
        else if (cur->key < key) {
          travelNodes.push_back(cur);

          cur = (cur->isThreaded) ? nullptr : cur->right;
        }
      }
    return;
    }

    //
    //
    // leftOrRight: inserts newNode into this tree
    // based on key, it is inserted Left or Right
    //
    //
    void leftOrRight(NODE*& prev, NODE*& newNode, bool& nodeInserted) {
      // INSERT Root ?
		  if (prev == nullptr) {
        newNode->isThreaded = true;

        newNode->nL = 0;

        newNode->nR = 0;

        newNode->right = nullptr;

        // newNode->left = nullptr;

        this->root = newNode;

        nodeInserted = true;

        return;
      }

      // INSERT LEFT ?
      else if (prev->key > newNode->key) {
        newNode->right = prev;

        newNode->isThreaded = true;
        // prev->isThreaded = false;

        newNode->left = nullptr;

        newNode->nL = 0;

        newNode->nR = 0;

        prev->left = newNode;

        nodeInserted = true;
      }

      // INSERT RIGHT ?
      else if (prev->key < newNode->key) {
        newNode->right = prev->right;

        prev->isThreaded = false;
        newNode->isThreaded = true;

        newNode->nL = 0;

        newNode->nR = 0;

        newNode->left = nullptr;

        prev->right = newNode;

        nodeInserted = true;
      }
    }

    //
    //
    // isTraveledNodesBalanced: goes through the vector of travled nodes checking the balancing condition
    // If it finds an unbalanced node, it will return it in a vector with its parent if it has one
    //
    //
    bool isTraveledNodesBalanced(vector<NODE*>& travelNodes, vector<NODE*>& problematicNodes, int& leftOrRightProblem) {
    int index = 0;  // represents the current node in travelNodes through the loop

    for(NODE*& i : travelNodes) {

      // if condition doesnt hold save this node and the previous
      // the previous can be the root or this can be the root.
      if ((max(i->nL, i->nR)) > (2 * min(i->nL, i->nR) + 1)) {
        problematicNodes.push_back(i);

       if (index > 0) {
          // problematic node is not root, so save the previous node too.
          problematicNodes.push_back(travelNodes[index-1]);

          // is the problematic node a left or right child? 1 - left / 2 - right
          // [0] child problem , [1] parent of problem
          if (problematicNodes[1]->left == problematicNodes[0]) {
            leftOrRightProblem = 1;
          }
          else if (problematicNodes[1]->right == problematicNodes[0]) {
          leftOrRightProblem = 2;
          }
        }

        return false;
      }
      index++;
    }
    return true;
    }

    //
    // balance_It: given an inorder vector of nodes, will make a tree starting
    // with the middle node
    //
    NODE* balance_It(vector<NODE*>& tree, int left, int right ) {

      int mid = (left + right) / 2;
      // cout << "left: " << left << " right: " << right << " Mid: " << mid << endl;

      // end of left side of vector
      if (mid-1 == left){
        // cout << "mid-1 == left" << endl;
        tree[left]->isThreaded = true;

        tree[left]->right = tree[mid];

        tree[left]->left = nullptr;

        tree[left]->nL = 0;

        tree[left]->nR = 0;

        tree[mid]->left = tree[left];

        tree[mid]->nL = 1;

        // cout << "Assinged left a child" << endl;

      }

      // no left child
      if (left > mid-1) {
        // cout << "left > mid-1" << endl;
        tree[mid]->left = nullptr;

        tree[mid]->nL = 0;

        // cout << "Set left child to NULL" << endl;
      }

      // assign left child to another subtree root
      else if (mid-1 > left) {
//         cout << "mid-1 > left" << endl;
//         cout << "recursive left" << endl;

        tree[mid]->left = balance_It(tree, left, mid-1);

        tree[mid]->nL = tree[mid]->left->nR + tree[mid]->left->nL + 1;
        // cout << "Assigned a new balanced tree to left side" << endl;

      }

      // end of right side of vector
      if (mid+1 == right){
        // cout << "mid+1 == right" << endl;
        tree[right]->nR = 0;

        tree[right]->nL = 0;

        tree[right]->isThreaded = true;

        tree[right]->left = nullptr;

        // tree[right]->right = nullptr;

//         if (tree[right]->key < this->root->key) {
//           // cout << "Assiged thread to root" << endl;
//           tree[right]->right = this->root;
//         }

//         else if (tree[right]->key > this->root->key) {
//         tree[right]->right = nullptr;
//         }

        tree[mid]->right = tree[right];

        tree[mid]->isThreaded = false;

        tree[mid]->nR = 1;

//         cout << "Assigned right a child" << endl;

      }

      // no right child
      if (mid+1 > right) {
//       cout << "mid+1 > right" << endl;
        tree[mid]->right = nullptr;

        tree[mid]->isThreaded = true;

        tree[mid]->nR = 0;
//         cout << "Set right child to NULL" << endl;
      }

      // assign right child to another subtree root
      else if (mid+1 < right) {
//         cout << "mid+1 < right" << endl;
//         cout << "recursive right" << endl;

        tree[mid]->right = balance_It(tree, mid+1, right);

        tree[mid]->nR = tree[mid]->right->nR + tree[mid]->right->nL + 1;

        tree[mid]->isThreaded = false;

//         cout << "Assigned a new balanced tree to right side" << endl;
//         cout << "Its root: " << tree[mid]->right->key << endl;
      }

      return tree[mid];
    }
    //
    //
    // nodeCounterUpdater: using the vector of travled nodes, this function
    // will visit each one and compare its key with the added node's key
    // to find which side to increment for node count.
    //
    //
    void nodeCountUpdater(NODE*& newNode, vector<NODE*>& travelNodes) {
        for (auto& i : travelNodes) {

          NODE* foundNode = findNode(i);

          if (newNode->key < foundNode->key) {
            foundNode->nL++;
          }
          else if (newNode->key > foundNode->key) {
            foundNode->nR++;
          }

        }
    }

    //
    // put:
    //
    // Inserts the key/value into the threaded, self-balancing BST based on
    // the key.
    // Time complexity: O(logn + mlogm), where n is total number of nodes in the
    // threaded, self-balancing BST and m is the number of nodes in the
    // sub-tree that needs to be re-balanced.
    // Space complexity: O(1)
    //
    void put(keyType key, valueType value) {
      vector<NODE*> travelNodes;
      bool nodeInserted = false;
      bool isKeyFound = false;
      NODE* foundNode = nullptr;
      vector<NODE*> problematicNodes;
      int leftOrRightProblem = 0;

			NODE* cur = this->root;
			NODE* prev = nullptr;

      // 1) Look for given key
      searchTree(cur, prev, key, travelNodes, isKeyFound, foundNode);

      // CASE 1 : Found key
      if (isKeyFound == true) {
       // 2) return value
       foundNode->value = value;
        return;
      }

      // CASE 2 : Did not find key
      else if (isKeyFound == false) {

        // 2) make new node
        NODE* newNode = makeNewNode(key, value);

        // 3) Insert Node
        leftOrRight(prev, newNode, nodeInserted);

        // cout << "Node inserted: " << newNode->key << " with value: " << newNode->value << endl;

        if (nodeInserted == true) {
          // 4) Now update nL and nR for nodes travled
          nodeCountUpdater(newNode, travelNodes);

          // 5) check if traveled nodes are balanced
          if (isTraveledNodesBalanced(travelNodes, problematicNodes, leftOrRightProblem) == false) {

            vector<NODE*> treeListToBalance;

            // 6) create an IN-Order vector of NODES starting at the problematic node, going down
            BST_Node_VectorInOrder(problematicNodes[0], treeListToBalance);

            // cout << "IN-Order tree to be Balanced: " << endl;
            //print_Node_Vector(treeListToBalance);

            int left = 0;
            int right = treeListToBalance.size()-1;

            // 7) balance
            NODE* aNode = balance_It(treeListToBalance, left, right);  //  balancing act ---------------------------------------------------------------------------

            // 8) Place balanced sub tree back into parent
            // if problem was at root

            if (problematicNodes.size() == 1) {
              this->root = aNode;
            }

            // make the parent of the old problematic node point to the root of the new tree
            else if (problematicNodes.size() == 2) {
              if (leftOrRightProblem == 1) {
                problematicNodes[1]->left = aNode;
              }

              else if (leftOrRightProblem == 2) {
                problematicNodes[1]->right = aNode;
              }
            }

          }

          size++;
        }
      }
//             // print new balanced WHOLE tree
//             string theString;
//             inToStringPreOrder(this->root, theString);
//             cout << "WHOLE Balanced Tree: " << endl;
//             cout << theString << endl;
      return;
    }

    //
    // contains:
    // Returns true if the key is in mymap, return false if not.
    // Time complexity: O(logn), where n is total number of nodes in the
    // threaded, self-balancing BST
    //
    bool contains(keyType key) {
    NODE* cur = root;
    while (cur != nullptr) {
      if (cur->key == key) {
         return true;
      }
      if (cur->key > key) {
        cur = cur->left;
      }
      else if (cur->key < key) {
        cur = (cur->isThreaded) ? nullptr : cur->right;
      }
    }
        return false;
    }

    //
    // get:
    //
    // Returns the value for the given key; if the key is not found, the
    // default value, valueType(), is returned (but not added to mymap).
    // Time complexity: O(logn), where n is total number of nodes in the
    // threaded, self-balancing BST
    //
    valueType get(keyType key) {
    NODE* cur = root;
    while (cur != nullptr) {
      if (cur->key == key) {
         return cur->value;
      }
      if (cur->key > key) {
        cur = cur->left;
      }
      else if (cur->key < key) {
        //cur = cur->right;
        cur = (cur->isThreaded) ? nullptr : cur->right;  // isThreaded updated ----------------------------------------------------------------
      }
    }
        return valueType();
    }

    //
    // operator[]:
    //
    // Returns the value for the given key; if the key is not found,
    // the default value, valueType(), is returned (and the resulting new
    // key/value pair is inserted into the map).
    // Time complexity: O(logn + mlogm), where n is total number of nodes in the
    // threaded, self-balancing BST and m is the number of nodes in the
    // sub-trees that need to be re-balanced.
    // Space complexity: O(1)
    //
    valueType operator[](keyType key) {
    vector<NODE*> travelNodes;
    bool isKeyFound = false;
    NODE* cur = root;
    NODE* prev = nullptr;
    NODE* foundNode = nullptr;
    bool isNodeInserted = false;
    vector<NODE*> problematicNodes;
    int leftOrRightProblem = 0;

    // 1) functiion that searches tree for key,
    // * updates boolean isKeyFound = true if found
    // * and updates pointer to last node visited (prev)
    searchTree(cur, prev, key, travelNodes, isKeyFound, foundNode);
    if(isKeyFound == true) {
      return foundNode->value;
    }

    // make new node if key is not in the tree
    if (isKeyFound == false) {
      NODE* newNode = makeNewNode(key, valueType());

      // left or right add ?
      leftOrRight(prev, newNode, isNodeInserted);

      // Add to vector of traveled nodes
      travelNodes.push_back(newNode);

      // Now update the left and right subtree sizes for each node traveled
      if (isNodeInserted == true) {
        nodeCountUpdater(newNode, travelNodes);

        // 5) check if traveled nodes are balanced
        if (isTraveledNodesBalanced(travelNodes, problematicNodes, leftOrRightProblem) == false) {

          vector<NODE*> treeListToBalance;

          // 6) create an IN-Order vector of NODES starting at the problematic node, going down
          BST_Node_VectorInOrder(problematicNodes[0], treeListToBalance);

          int left = 0;
          int right = treeListToBalance.size()-1;

          // 7) balance
          NODE* aNode = balance_It(treeListToBalance, left, right);  //  balancing act ---------------------------------------------------------------------------

          // 8) Place balanced sub tree back into parent
          // if problem was at root
          if (problematicNodes.size() == 1) {
            this->root = aNode;
          }

          // make the parent of the old problematic node point to the root of the new tree
          else if (problematicNodes.size() == 2) {
            if (leftOrRightProblem == 1) {
              problematicNodes[1]->left = aNode;
            }

            else if (leftOrRightProblem == 2) {
              problematicNodes[1]->right = aNode;
            }
          }

         }

        // increment size of tree
        this->size++;
       } // node is inserted action over
      } // if key is not found action over
    return valueType();
    }

    //
    // Size:
    //
    // Returns the # of key/value pairs in the mymap, 0 if empty.
    // O(1)
    //
    int Size() {
        return this->size;
    }

    //
    // begin:
    //
    // returns an iterator to the first in order NODE.
    // Time complexity: O(logn), where n is total number of nodes in the
    // threaded, self-balancing BST
    //
    iterator begin() {
    NODE* cur = root;
    NODE* prev = nullptr;
    while (cur != nullptr) {
      prev = cur;
      cur = cur->left;
    }
        return iterator(prev);
    }

    //
    // end:
    //
    // returns an iterator to the last in order NODE.
    // this function is given to you.
    //
    // Time Complexity: O(1)
    //
    iterator end() {
        return iterator(nullptr);
    }

    //
    //
    // BSTPrintInOrder: will simply print the tree in order fashion onto an output stream
    //
    //
    void BSTPrintInorder(NODE* node, ostream& theString) {
      if (node == nullptr) {
        return;
       }
      BSTPrintInorder(node->left, theString);

      stringstream ssK;

      ssK << node->key;

      string theKey;

      ssK >> theKey;

      stringstream ssV;

      ssV << node->value;

      string theVal;

      ssV >> theVal;

      theString << "key: " << theKey << " " << "value: " << theVal << "\n";

      node = (node->isThreaded) ? nullptr : node->right;

      BSTPrintInorder(node, theString);
}

    //
    // toString:
    //
    // Returns a string of the entire mymap, in order.
    // Format for 8/80, 15/150, 20/200:
    // "key: 8 value: 80\nkey: 15 value: 150\nkey: 20 value: 200\n
    // Time complexity: O(n), where n is total number of nodes in the
    // threaded, self-balancing BST
    //
    string toString() {
    stringstream theString;

    BSTPrintInorder(this->root, theString);

        return theString.str();
    }

    //
    //
    // BSTVectorInOrder: will put the tree into a vector of pairs
    //
    //
    void BSTVectorInorder(NODE* node, vector<pair<keyType,valueType>>& treeList) {
      if (node == nullptr) {
        return;
       }
      BSTVectorInorder(node->left, treeList);

      treeList.emplace_back(node->key, node->value);

      node = (node->isThreaded) ? nullptr : node->right;

      BSTVectorInorder(node, treeList);
}

    //
    //
    // printVector: Helper function that prints the vector of pairs
    //
    //
    void printVector(vector<pair<keyType,valueType>>& treeList) {
      for(auto& i : treeList) {
        cout << "Key: " << i.first << " Value: " << i.second << endl;
       }
       return;
    }

    //
    // toVector:
    //
    // Returns a vector of the entire map, in order.  For 8/80, 15/150, 20/200:
    // {{8, 80}, {15, 150}, {20, 200}}
    // Time complexity: O(n), where n is total number of nodes in the
    // threaded, self-balancing BST
    //
    vector<pair<keyType, valueType> > toVector() {
      vector<pair<keyType,valueType>> treeList;

      BSTVectorInorder(this->root, treeList);

      printVector(treeList);

      return treeList;//{};  // TODO: Update this return.
    }

    //
    //
    // BST_Node_VectorInOrder: will put the tree into an inorder vector of NODES
    //
    //
    void BST_Node_VectorInOrder(NODE* node, vector<NODE*>& treeList) {
      if (node == nullptr) {
        return;
       }
      BST_Node_VectorInOrder(node->left, treeList);

      treeList.emplace_back(node);

      node = (node->isThreaded) ? nullptr : node->right;

      BST_Node_VectorInOrder(node, treeList);
}
    //
    //
    // printVector: Helper function that prints the vector of pairs
    //
    //
    void print_Node_Vector(vector<NODE*>& treeList) {
      for(auto& i : treeList) {
        cout << "Key: " << i->key << " Value: " << i->value << endl;
       }
       return;
    }

    //
    // to_Node_Vector:
    //
    // Returns a vector of the entire map, in order.  For 8/80, 15/150, 20/200:
    // {{8, 80}, {15, 150}, {20, 200}}
    // Time complexity: O(n), where n is total number of nodes in the
    // threaded, self-balancing BST
    //
    vector<NODE*> to_Node_Vector() {
      vector<NODE*> treeList;

      BST_Node_VectorInOrder(this->root, treeList);

      print_Node_Vector(treeList);

      return treeList;
    }

    //
    //
    // intoStringPlease: takes a node and puts its contents into a string
    // returns that string
    //
    //
    string intoStringPlease(NODE* node){
    string theString;
    stringstream SS;
    SS << node->key;
    string keySS;
    SS >> keySS;

       theString = "key: " + keySS + ", nL: " + to_string(node->nL) + ", nR: " + to_string(node->nR) + "\n" ;

      return theString;
    }

    //
    //
    // IntoStringPreOrder: will update the string parameter with the tree contents
    //  this is done in preOrder fashion
    //
    //
    void inToStringPreOrder(NODE* other, string& theString) {
      if (other == nullptr) {
       return;
      }
      else {
      theString += intoStringPlease(other);

      inToStringPreOrder(other->left, theString);  // left

      other = (other->isThreaded) ? nullptr : other->right;

      inToStringPreOrder(other, theString);  // right
      }
    }

    //
    // checkBalance:
    //
    // Returns a string of mymap that verifies that the tree is properly
    // balanced.
    // returns a string in this format (in pre-order):
    // "key: 2, nL: 1, nR: 1\nkey: 1, nL: 0, nR: 0\nkey: 3, nL: 0, nR: 0\n";
    // Time complexity: O(n), where n is total number of nodes in the
    // threaded, self-balancing BST
    //
    string checkBalance() {
      string theString;
      inToStringPreOrder(this->root, theString);

        return theString;
    }
};
