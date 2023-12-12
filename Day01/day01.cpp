#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main()
{
   std::cout << "Advent of Code 2023 - Day 01" << std::endl;

   int totalSum = 0;
   std::string currentLine;
   const std::string inputFileName = "input";
   std::ifstream inputFile(inputFileName);
   if ( inputFile.is_open() ) {
      while ( inputFile ) {
         std::getline(inputFile, currentLine);
         int lineLength = currentLine.length();

         int currentValue = 0;
         // find first number
         for(int i=0; i<lineLength; i++)
         {
            const char currentChar = currentLine[i];
            if(currentChar>='0' && currentChar<='9')
            {
//         std::cout << currentChar << " " << currentChar-'0' << " ";
               currentValue = 10 * (currentChar-'0');
               break;
            }
         }

         // find last number
         for(int i=lineLength-1; i>=0; i--)
         {
            const char currentChar = currentLine[i];
            if(currentChar>='0' && currentChar<='9')
            {
//         std::cout << currentChar << " " << currentChar-'0' << " - ";
               currentValue += (currentChar-'0');
               break;
            }
         }

         std::cout << currentValue << " - " << currentLine << std::endl;

         totalSum += currentValue;
      }
   }

   std::cout << "Total: " << totalSum << std::endl;
}