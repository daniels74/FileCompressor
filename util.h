// util.h: Contains the main functions required to compress and decompress
// files using the Huffman encoding method. This is done by making a map
// of chars read in the file to the number of times they occur in the file.
// Based on this map, a tree is built in which at every leaf node, a char
// and a value are stored. The path taken to each unique leaf node will
// be put in another map as string data along with its key, the char of
// the leaf node. When encoding, each char will be represnted by a path
// of 1s ands 0s and this pattern will be the encoded file along with the
// freq map contents in the front. With this freq map in the front, the
// encoded file can be decoded by constructing a tree again based on the
// freq map and using the 1s and 0s to go down the tree to its corresponding
// char.

#pragma once
#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <queue>          // std::priority_queue
#include <vector>         // std::vector
#include <functional>     // std::greater
#include <string>
#include "bitstream.h"
#include "util.h"
#include "hashmap.h"
#include "mymap.h"

struct HuffmanNode {
    int character;
    int count;
    HuffmanNode* zero;
    HuffmanNode* one;
};

class prioritize {  // ordering queue requirment
    public:
    bool operator()(const HuffmanNode* p1, const HuffmanNode* p2) const {
        return p1 -> count > p2 -> count;  // HuffmanNode* will be different
    }
};

void deleteTree(HuffmanNode* cur) {
  if (cur == nullptr) {
    return;
  }

  deleteTree(cur -> zero);
  deleteTree(cur -> one);

  delete(cur);
}

//
// *This method frees the memory allocated for the Huffman tree.
//
void freeTree(HuffmanNode* node) {
  deleteTree(node);
  node = nullptr;
}

//
// *This function build the frequency map.  If isFile is true, then it reads
// from filename.  If isFile is false, then it reads from a string filename.
//
void buildFrequencyMap(string filename, bool isFile, hashmap &map) {
	if (isFile == true) {
		ifstream inFile(filename);
		char c;
		while (inFile.get(c)) {
			map.put(c, 1);
		}
	}

	else if (isFile == false) {
		for (auto& i : filename) {
			map.put(i, 1);
		}
	}
  map.put(PSEUDO_EOF, 1);
}

//
// Makenode: makes a HuffmanNode with nullptr childs
//
HuffmanNode* makeNode(int key, int val) {
  HuffmanNode* newNode = new HuffmanNode();
  newNode -> character = key;
  newNode -> count = val;
  newNode -> zero = nullptr;
  newNode -> one = nullptr;

  return newNode;
}


//
// *This function builds an encoding tree from the frequency map.
//
HuffmanNode* buildEncodingTree(hashmap &map) {
  priority_queue<HuffmanNode*, vector<HuffmanNode*>, prioritize> pq;

  // obtain all keys
  vector<int> keys = map.keys();

  // search for each keys value using get
  // put both into a node
  // put node into prioity queue
  for (auto j : keys) {
    int val = map.get(j);
    HuffmanNode* newNode = makeNode(j, val);
    pq.push(newNode);
  }

  // build tree
  // while there is more than 1 node in the pq
  while (pq.size() > 1) {
    // take first two nodes
    HuffmanNode* n1 = pq.top();
    pq.pop();
    HuffmanNode* n2 = pq.top();
    pq.pop();

    // make new node with new count
    int newCount = n1 -> count + n2 -> count;
    HuffmanNode* newnode = makeNode(NOT_A_CHAR, newCount);
    newnode -> zero = n1;
    newnode -> one = n2;

    pq.push(newnode);
  }

    return pq.top();
}


void preOrder(HuffmanNode* tree, string str, mymap<int, string>& encodingMap) {
  //  root, left, right
  if (tree == nullptr) {
    return;
  }
  if (tree -> character != NOT_A_CHAR) {
    encodingMap.put(tree->character, str);
  }
  preOrder(tree -> zero, str+ "0", encodingMap);
  preOrder(tree -> one, str+ "1", encodingMap);
}

//
// Makes use of mymap to build a map of int keys to string values
//
mymap <int, string> buildEncodingMap(HuffmanNode* tree) {
    mymap <int, string> encodingMap;

    // go through the tree fidning each char node?
    // pre order
    string str;
    preOrder(tree, str, encodingMap);

    return encodingMap;
}

//
// *This function encodes the data in the input stream into the output stream
// using the encodingMap.  This function calculates the number of bits
// written to the output stream and sets result to the size parameter, which is
// passed by reference.  This function also returns a string representation of
// the output file, which is particularly useful for testing.
//
string encode(ifstream& input, mymap <int, string> &encodingMap,
 ofbitstream& output, int &size, bool makeFile) {
  string filestr;
  char c;

  // find all chars in encoding map and add there paths to filestr
  // this excludes all eofs and not_a_chars
  while (input.get(c)) {
    string str = encodingMap.get((int)c);
    if (c != 256 && c != 257) {
      filestr+= str;
    }
  }

  string psuedoPath = encodingMap.get(256);

  // go through the binary filestr
  // convert every binary char to int
  // and put them in output stream
  if (makeFile == true) {
    for (char& h : filestr) {
      output.writeBit((int)h-48);
      size++;
    }

    // find eof's path and add to end of output stream
    for (char& i : psuedoPath) {
      output.writeBit((int)i - 48);
      size++;
    }
  }

  // add eof path to the filestr
  filestr += psuedoPath;

  return filestr;
}

//
// walk_tree: will move the current position in the tree to the next
// given bit direction. It will return the new position in the tree
//
HuffmanNode* walk_tree(HuffmanNode* cur, int bit) {
  if (bit == 0) {
    cur = cur -> zero;
  }

  else if (bit == 1) {
    cur = cur -> one;
  }

  return cur;
}

//
// *This function decodes the input stream and writes the result to the output
// stream using the encodingTree.  This function also returns a string
// representation of the output file, which is particularly useful for testing.
//
string decode(ifbitstream &input, HuffmanNode* encodingTree, ofstream &output) {
  HuffmanNode* cur = encodingTree;
  string outString;

  while (!input.eof()) {
    int bit = input.readBit();

    cur = walk_tree(cur, bit);

    // reached a leaf
    if (cur -> one == nullptr && cur -> zero == nullptr) {
      if (cur -> character == 256) {  // eof
        break;
      }

      outString += (char)cur->character;

      cur = encodingTree;  // reset pos to root node
    }
  }
  for (char& o : outString) {
    output.put(o);
  }

  return outString;
}

//
// *This function completes the entire compression process.  Given a file,
// filename, this function (1) builds a frequency map; (2) builds an encoding
// tree; (3) builds an encoding map; (4) encodes the file (don't forget to
// include the frequency map in the header of the output file).  This function
// should create a compressed file named (filename + ".huf") and should also
// return a string version of the bit pattern.
//
string compress(string filename) {
  bool isFile = true;
  hashmap amap;

  buildFrequencyMap(filename, isFile, amap);

  HuffmanNode* tree = buildEncodingTree(amap);

  mymap<int, string> encodingMap = buildEncodingMap(tree);

  ofbitstream output(filename + ".huf");
  ifstream input(filename);

  stringstream ss;
  // note: << is overloaded for the hashmap class
  ss << amap;
  output << amap;  // add the frequency map to the file

  int size = amap.size() * 2 + 2;  // a pair so x2 plus the 2 {}
  string codeStr = encode(input, encodingMap, output, size, true);

  output.close();

  freeTree(tree);

    return codeStr;
}

//
// *This function completes the entire decompression process.  Given the file,
// filename (which should end with ".huf"), (1) extract the header and build
// the frequency map; (2) build an encoding tree from the frequency map; (3)
// using the encoding tree to decode the file.  This function should create a
// compressed file using the following convention.
// If filename = "example.txt.huf", then the uncompressed file should be named
// "example_unc.txt".  The function should return a string version of the
// uncompressed file.  Note: this function should reverse what the compress
// function did.
//
string decompress(string filename) {
  size_t pos = filename.find(".huf");

  if ((int)pos >= 0) {
    filename = filename.substr(0, pos);
  }

  pos = filename.find(".");

  string ext = filename.substr(pos, filename.length() - pos);

  filename = filename.substr(0, pos);

  ifbitstream input(filename + ext + ".huf");

  ofstream output(filename + "_unc" + ext);

  hashmap dump;

  input >> dump;  // get rid of frequency map at top of file

  // make tree
  HuffmanNode* tree = buildEncodingTree(dump);

  string decodeStr = decode(input, tree, output);

  output.close();  // must close file so autograder can open for testing

  freeTree(tree);

  return decodeStr;
}
