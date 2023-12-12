#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Helper
{
public:
   Helper() {}
   ~Helper() {}

   bool isNumber(const char character)
   {
      return (character>='0' && character<='9');
   }

   int getLineValue(std::string line)
   {
      int lineLength = line.length();

      int currentValue = 0;
      // find first number
      for(int i=0; i<lineLength; i++)
      {
         const char currentChar = line[i];
         if( this->isNumber(currentChar) )
         {
//         std::cout << currentChar << " " << currentChar-'0' << " ";
            currentValue = 10 * (currentChar-'0');
            break;
         }
      }

      // find last number
      for(int i=lineLength-1; i>=0; i--)
      {
         const char currentChar = line[i];
         if( this->isNumber(currentChar) )
         {
//         std::cout << currentChar << " " << currentChar-'0' << " - ";
            currentValue += (currentChar-'0');
            break;
         }
      }

//      std::cout << currentValue << " - " << line << std::endl;
      return currentValue;
   }

   int getTotalValue(std::string inputFileName)
   {
      int totalSum = 0;
      std::string currentLine;
      std::ifstream inputFile(inputFileName);
      if ( inputFile.is_open() ) {
         while ( inputFile ) {
            std::getline(inputFile, currentLine);
            int currentValue = this->getLineValue(currentLine);
            totalSum += currentValue;
         }
      }

      return totalSum;
   }
};

int main()
{
   std::cout << "Advent of Code 2023 - Day 01" << std::endl;

   const std::string inputFileName = "input";
   Helper helper;
   int totalSum = helper.getTotalValue(inputFileName);

   std::cout << "Total: " << totalSum << std::endl;
}