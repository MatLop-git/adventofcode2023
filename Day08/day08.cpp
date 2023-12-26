#include <algorithm>    // std::sort
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

static const int LABEL_WIDTH = 3;
static const int LEFT_POSITION = 7;
static const int RIGHT_POSITION = 12;

struct Node
{
   std::string Label = "";
   std::string Direction[2];

   Node() {}
   Node(std::string label, std::string left, std::string right) : Label(label), Direction{left, right} {}
   ~Node() {}
};

class Helper
{
private:
   ulong _firstPuzzleAnswer = 0;
   ulong _secondPuzzleAnswer = 0;
   std::vector<std::string> _fileInput;

public:
   Helper() {}
   ~Helper() {}

   static int GetNode(std::string label, std::vector<Node> nodes)
   {
      for(int i=0; i<nodes.size(); ++i)
      {
         if(nodes[i].Label == label)
         {
            return i;
         }
      }
      return -1;
   }

   void calculateFirstPuzzleAnswer()
   {
      this->_firstPuzzleAnswer = 0;

      // obtain directions
      std::string directions = _fileInput[0];

      std::vector<Node> nodes;
      for(int i=2; i<_fileInput.size(); ++i)
      {
         std::string line = _fileInput[i];
         std::string label = line.substr(0, LABEL_WIDTH);
         std::string left = line.substr(LEFT_POSITION, LABEL_WIDTH);
         std::string right = line.substr(RIGHT_POSITION, LABEL_WIDTH);
         nodes.push_back(Node(label, left, right));
      }

//      for(int i=0; i<handsBidPairs.size(); ++i)
//      {
//         std::cout << handsBidPairs[i].Hand << " - " << handsBidPairs[i].Type << std::endl;
//      }

      // find path start (AAA)
      int nodeIndex = GetNode("AAA", nodes);
      Node currentNode = nodes[nodeIndex];

      // calculate path
      int pathIndex = 0;
      do
      {
         this->_firstPuzzleAnswer++;

         std::string target = "";
         if( directions[pathIndex] == 'L' )
         {
            target = currentNode.Direction[0];
         }
         else
         {
            target = currentNode.Direction[1];
         }
         nodeIndex = GetNode(target, nodes);
         currentNode = nodes[nodeIndex];

         pathIndex++;
         if(pathIndex > directions.size()-1)
         {
            pathIndex = 0;
         }

         std::cout << currentNode.Label << std::endl;
      }
      while(currentNode.Label != "ZZZ");
   }

   void calculateSecondPuzzleAnswer()
   {
      this->_secondPuzzleAnswer = 0;

   }

   void calculateAnswers(std::string inputFileName)
   {
      this->readFileInput(inputFileName);
      this->calculateFirstPuzzleAnswer();
      this->calculateSecondPuzzleAnswer();
   }

   void readFileInput(std::string inputFileName)
   {
      std::string currentLine;
      std::ifstream inputFile(inputFileName);
      if ( inputFile.is_open() ) {
         while ( std::getline(inputFile, currentLine) ) {
            _fileInput.push_back(currentLine);
         }
      }
   }

   int getFirstPuzzleAnswer() { return _firstPuzzleAnswer; }
   int getSecondPuzzleAnswer() { return _secondPuzzleAnswer; }
};

int main()
{
   std::cout << "Advent of Code 2023 - Day 08" << std::endl;

   const std::string inputFileName = "input";
   Helper helper;
   helper.calculateAnswers(inputFileName);

   int answer = helper.getFirstPuzzleAnswer();
   std::cout << "First half answer: " << answer << std::endl;

   answer = helper.getSecondPuzzleAnswer();
   std::cout << "Second half answer: " << answer << std::endl;
}