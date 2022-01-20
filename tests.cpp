#include <gtest/gtest.h>
#include "mymap.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <queue>
#include <vector>
#include <functional>
#include <ctype.h>
#include <math.h>
#include "bitstream.h"
#include "util.h"
#include "hashmap.cpp"

//using namespace std;

TEST(hashmap, milestone_zero) {
  // build a hashmap

  // Function prototypes
  hashmap frequencyMap;
  string filename = "MLK-Dream.txt";
  bool isFile = true;
   
    // Build Frequency Map
  //buildFrequencyMap(filename, isFile, frequencyMap);
  char b[] = {'A','B','A','A','A'};
  
    for (int i = 0; i < 5; i++) {
    frequencyMap.put(b[i], 1);
    cout << b[i] << "  " ;
    cout << frequencyMap.get(b[i]) << endl;
  }
  cout << frequencyMap.get('D') << endl;
  
  
  for (int i = 0; i < 5; i++) {
    frequencyMap.put(i, 1);
  }
  for (int v = 0; v < 5; v++) {
    ASSERT_EQ(frequencyMap.get(v), 1);
  }
  
  for (int k = 101; k < 5; k++) {
    ASSERT_EQ(frequencyMap.containsKey(k), false);
  }
    for (int t = 0; t < 5; t++) {
    ASSERT_EQ(frequencyMap.containsKey(t), true);
  }
//   vector<int> correctVec;
//   for (int y = 0; y < 100; y++) {
//     correctVec.push_back(7);
//   }
//   ASSERT_EQ(frequencyMap.keys, correctVec);

}

// TEST(hashmap, milestone_one_frequencyMap) {
// 	bool isFile = true;
// 	hashmap amap;
// 	string filename = "example.txt";
	
// 	buildFrequencyMap(filename, isFile, amap);
	
//   ASSERT_EQ(amap.size(), 5);
// }

// TEST(hashmap, milestone_two_endcodingTree) {
//   bool isFile = true;
//   hashmap amap;
//   string filename = "example.txt";
	
//   buildFrequencyMap(filename, isFile, amap);
  
//   HuffmanNode* node = buildEncodingTree(amap);
  
//   ASSERT_EQ(node->count, 10);
// }

// TEST(hashmap, milestone_three_encodingMyMap) {
//   bool isFile = true;
//   hashmap amap;
//   string filename = "example.txt";
	
//   buildFrequencyMap(filename, isFile, amap);
  
//   HuffmanNode* tree = buildEncodingTree(amap);
  
//   mymap<int, string> em = buildEncodingMap(tree);
  
//   for(auto i : em) {
//     cout << "char: " << i;
//     string s = em.get(i);
//     cout << "  path: " << s << endl;
//   }
  
// }

// TEST(hashmap, milestone_four_encode) {
  
//   bool isFile = true;
//   hashmap amap;
//   string filename = "example.txt";
	
//   buildFrequencyMap(filename, isFile, amap);
  
//   HuffmanNode* tree = buildEncodingTree(amap);
  
//   mymap<int, string> encodingMap = buildEncodingMap(tree);
  
//   ofbitstream output(filename + ".huf");
//   ifstream input(filename);
        
//   stringstream ss;
//   // note: << is overloaded for the hashmap class.  super nice!
//   ss << amap;
//   output << amap;  // add the frequency map to the file
//   int size = 0;
//   string codeStr = encode(input, encodingMap, output, size, true);
  
//   output.close();
  
//   cout << "file size: " << size << endl;
// }

// TEST(hashmap, milestone_five_decode) {
  
//   bool isFile = true;
//   hashmap amap;
//   string filename = "example.txt";
	
//   buildFrequencyMap(filename, isFile, amap);
  
//   HuffmanNode* tree = buildEncodingTree(amap);
  
//   mymap<int, string> encodingMap = buildEncodingMap(tree);
  
//   ofbitstream output(filename + ".huf");
//   ifstream input(filename);
        
//   stringstream ss;
//   // note: << is overloaded for the hashmap class.  super nice!
//   ss << amap;
//   output << amap;  // add the frequency map to the file
//   int size = 0;
//   string codeStr = encode(input, encodingMap, output, size, true);
  
//   cout << codeStr << endl; 
  
//   output.close();
  
//   cout << "file size: " << size << endl;
  
//   cout << "Decoding..." << endl;
//         size_t pos = filename.find(".huf");
//         if ((int)pos >= 0) {
//             filename = filename.substr(0, pos);
//         }
//         pos = filename.find(".");
//         string ext = filename.substr(pos, filename.length() - pos);
//         filename = filename.substr(0, pos);
//         ifbitstream inputTwo(filename + ext + ".huf");
//         ofstream outputTwo(filename + "_unc" + ext);
        
//         hashmap dump;
//         inputTwo >> dump;  // get rid of frequency map at top of file
        
//         string decodeStr  = decode(inputTwo, tree, outputTwo);
//         cout << "STRING OUT:" << decodeStr << endl;
//         cout << endl;
//         output.close(); // must close file so autograder can open for testing

// }

// TEST(hashmap, milestone_six_freeTree) {
  
//   bool isFile = true;
//   hashmap amap;
//   string filename = "example.txt";
	
//   buildFrequencyMap(filename, isFile, amap);
  
//   HuffmanNode* tree = buildEncodingTree(amap);
  
//   freeTree(tree);
  
// }

// TEST(compress, milstone_seven){
//   string a = compress("secretemessage.txt");
//   cout << a << endl;
// }

// TEST(de_compress, milstone_eight) {
//   string a = decompress("example.txt.huf");
//   cout << a << endl;
// }
















































