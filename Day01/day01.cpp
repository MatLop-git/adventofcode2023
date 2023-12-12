#include <fstream>
#include <iostream>
#include <string>
#include <utility>

class Helper
{
public:
   static const int VALIDDIGITS_SIZE = 20;
   std::pair<std::string, int> validDigits[VALIDDIGITS_SIZE] = {
      {"0", 0},
      {"1", 1},
      {"2", 2},
      {"3", 3},
      {"4", 4},
      {"5", 5},
      {"6", 6},
      {"7", 7},
      {"8", 8},
      {"9", 9},
      {"zero", 0},
      {"one", 1},
      {"two", 2},
      {"three", 3},
      {"four", 4},
      {"five", 5},
      {"six", 6},
      {"seven", 7},
      {"eight", 8},
      {"nine", 9}
   };

   Helper() {}
   ~Helper() {}

   int getLineValue(std::string line)
   {
      int lineLength = line.length();

      int currentValue = 0;
      // find first number
      int currentMatchValue = 0;
      for(int i=0; i<lineLength; i++)
      {
         std::string::size_type lowestMatchPosition = lineLength;
         // compare all the keys of the hash to find the one has the lower position
         for(int j=0; j<VALIDDIGITS_SIZE; j++)
         {
            std::string key = validDigits[j].first;
            std::string::size_type matchPosition = line.find(key);
//         std::cout << "Looking for " << key << " on line " << line << std::endl;
            if((matchPosition != std::string::npos) && (matchPosition < lowestMatchPosition))
            {
               lowestMatchPosition = matchPosition;
               currentMatchValue = validDigits[j].second;
//         std::cout << "Match " << key << " on line " << line << " - " << currentMatchValue << std::endl;
            }
         }
      }
      currentValue = currentMatchValue * 10;

      // find last number
      for(int i=0; i<lineLength; i++)
      {
         std::string::size_type highestMatchPosition = 0;
         // compare all the keys of the hash to find the one has the lower position
         for(int j=VALIDDIGITS_SIZE-1; j>=0; j--)
         {
            std::string key = validDigits[j].first;
            std::string::size_type matchPosition = line.rfind(key);
            if((matchPosition != std::string::npos) && (matchPosition > highestMatchPosition))
            {
               highestMatchPosition = matchPosition;
               currentMatchValue = validDigits[j].second;
            }
         }
      }
      currentValue += currentMatchValue;

//      std::cout << currentValue << " - " << line << std::endl;
      return currentValue;
   }

   int getTotalValue(std::string inputFileName)
   {
      int totalSum = 0;
      std::string currentLine;
      std::ifstream inputFile(inputFileName);
      if ( inputFile.is_open() ) {
         while ( std::getline(inputFile, currentLine) ) {
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