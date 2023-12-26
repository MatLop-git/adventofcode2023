#include <algorithm>    // std::sort
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

static const int LABEL_WIDTH = 3;
static const int LEFT_POSITION = 7;
static const int RIGHT_POSITION = 12;

ulong gcd(ulong a, ulong b)
{
    for (;;)
    {
        if (a == 0) return b;
        b %= a;
        if (b == 0) return a;
        a %= b;
    }
}

ulong lcm(ulong a, ulong b)
{
    ulong temp = gcd(a, b);
    return temp ? (a / temp * b) : 0;
}

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

   static Node GetNode(std::string label, std::vector<Node> nodes)
   {
      int nodeIndex = GetNodeIndex(label, nodes);
      return nodes[nodeIndex];
   }

   static int GetNodeIndex(std::string label, std::vector<Node> nodes)
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

   static std::vector<Node> GetNodesEndingWith(char ending, std::vector<Node> nodes)
   {
      std::vector<Node> matchingNodes;
      for(int i=0; i<nodes.size(); ++i)
      {
         if(nodes[i].Label[LABEL_WIDTH-1] == ending)
         {
            matchingNodes.push_back(nodes[i]);
         }
      }
      return matchingNodes;
   }

   /** Returns directions from input file */
   std::string getDirections()
   {
      return _fileInput[0];
   }

   /** Returns network nodes from input file */
   std::vector<Node> getNodes()
   {
      std::vector<Node> nodes;
      for(int i=2; i<_fileInput.size(); ++i)
      {
         std::string line = _fileInput[i];
         std::string label = line.substr(0, LABEL_WIDTH);
         std::string left = line.substr(LEFT_POSITION, LABEL_WIDTH);
         std::string right = line.substr(RIGHT_POSITION, LABEL_WIDTH);
         nodes.push_back(Node(label, left, right));
      }
      return nodes;
   }

   void calculateFirstPuzzleAnswer()
   {
      this->_firstPuzzleAnswer = 0;

      // obtain directions
      std::string directions = getDirections();

      // obtain network nodes
      std::vector<Node> nodes = getNodes();

      // find path start (AAA)
      Node currentNode = GetNode("AAA", nodes);

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
         currentNode = GetNode(target, nodes);

         pathIndex++;
         if(pathIndex > directions.size()-1)
         {
            pathIndex = 0;
         }

//         std::cout << currentNode.Label << std::endl;
      }
      while(currentNode.Label != "ZZZ");
   }

   void calculateSecondPuzzleAnswer()
   {
      this->_secondPuzzleAnswer = 0;

      // obtain directions
      std::string directions = getDirections();

      // obtain network nodes
      std::vector<Node> nodes = getNodes();

      // find path start (AAA)
      std::vector<Node> startingNodes = GetNodesEndingWith('A', nodes);

      // calculate path
      std::vector<ulong> steps;
      for(int i=0; i<startingNodes.size(); ++i)
      {
         Node currentNode = startingNodes[i];
         int pathIndex = 0;
         ulong currentSteps = 0;
         do
         {
            currentSteps++;

            std::string target = "";
            if( directions[pathIndex] == 'L' )
            {
               target = currentNode.Direction[0];
            }
            else
            {
               target = currentNode.Direction[1];
            }
            currentNode = GetNode(target, nodes);

            pathIndex++;
            if(pathIndex > directions.size()-1)
            {
               pathIndex = 0;
            }
         }
         while(currentNode.Label[LABEL_WIDTH-1] != 'Z');

         std::cout << i << " - " << currentSteps << std::endl;
         steps.push_back(currentSteps);

         ulong pathIndexZ = pathIndex;
         currentSteps = 0;
         do
         {
            currentSteps++;

            std::string target = "";
            if( directions[pathIndex] == 'L' )
            {
               target = currentNode.Direction[0];
            }
            else
            {
               target = currentNode.Direction[1];
            }
            currentNode = GetNode(target, nodes);

            pathIndex++;
            if(pathIndex > directions.size()-1)
            {
               pathIndex = 0;
            }
         }
         while(currentNode.Label[LABEL_WIDTH-1] != 'Z' && pathIndexZ != pathIndex);         
         std::cout << i << " z " << currentSteps << std::endl;
      }

      this->_secondPuzzleAnswer = std::accumulate(steps.begin(), steps.end(), 1, lcm);
   }
/*
   void calculateSecondPuzzleAnswer()
   {
      this->_secondPuzzleAnswer = 0;

      // obtain directions
      std::string directions = getDirections();

      // obtain network nodes
      std::vector<Node> nodes = getNodes();

//      for(int i=0; i<handsBidPairs.size(); ++i)
//      {
//         std::cout << handsBidPairs[i].Hand << " - " << handsBidPairs[i].Type << std::endl;
//      }

      // find path starts (nodes that ends with A)
      std::vector<Node> currentNodes = GetNodesEndingWith('A', nodes);

      // calculate path
      bool pathEnd = true;
      int pathIndex = 0;
      do
      {
         this->_secondPuzzleAnswer++;

         pathEnd = true;
         for(int i=0; i<currentNodes.size(); ++i)
         {
            std::string target = "";
            if( directions[pathIndex] == 'L' )
            {
               target = currentNodes[i].Direction[0];
            }
            else
            {
               target = currentNodes[i].Direction[1];
            }
            currentNodes[i] = GetNode(target, nodes);
            pathEnd = pathEnd && (currentNodes[i].Label[LABEL_WIDTH-1] == 'Z');
         }

         pathIndex++;
         if(pathIndex > directions.size()-1)
         {
            pathIndex = 0;
         }

         for(int i=0; i<currentNodes.size(); ++i)
         {
            std::cout << currentNodes[i].Label;
         }
         std::cout << std::endl;
      }
      while(pathEnd == false);
   }
*/
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

   ulong answer = helper.getFirstPuzzleAnswer();
   std::cout << "First half answer: " << answer << std::endl;

   answer = helper.getSecondPuzzleAnswer();
   std::cout << "Second half answer: " << answer << std::endl;
}