#include <algorithm>    // std::sort
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

class AshRocksPattern
{
private:
//   const std::vector<ulong> TRUNCATE_BITMASK {
//      0x0000000000000001,
//      0x0000000000000011,
//      0x0000000000000011,
//      0x0000000000000111,
//      0x0000000000001111,
//      0x0000000000011111,
//      0x0000000000111111,
//      0x0000000001111111,
//      0x0000000001111111,
//      0x0000000011111111,
//      0x0000000111111111,
//      0x0000001111111111,
//      0x0000011111111111,
//      0x0000111111111111,
//      0x0001111111111111,
//      0x0011111111111111,
//      0x0111111111111111,
//      0x1111111111111111,
//   };

   std::vector<uint> _horizontalBits;
   std::vector<uint> _verticalBits;

public:
   AshRocksPattern() {}
   ~AshRocksPattern() {}

   void initialize(std::vector<std::string> &input, int initialLine, int lastLine)
   {
      // Horizontal bits
      for(int i=initialLine; i<lastLine; ++i)
      {
         ulong bits = 0;
         for(int j=0; j<input[i].size(); ++j)
         {
            if(input[i][j] == '#')
            {
               // Set 1 at bit position j
               bits = bits | (1 << j);
            }
         }
         std::cout << bits << std::endl;
         this->_horizontalBits.push_back( bits );
      }

      // Vertical bits
      for(int i=0; i<input[initialLine].size(); ++i)
      {
         ulong bits = 0;
         for(int j=initialLine; j<lastLine; ++j)
         {
            if(input[j][i] == '#')
            {
               // Set 1 at bit position j
               bits = bits | (1 << j);
            }
         }
         this->_verticalBits.push_back( bits );
      }
   }

   void printBits()
   {
      for(int i=0; i<this->_horizontalBits.size(); ++i)
      {
         for(int j=0; j<this->_verticalBits.size(); ++j)
         {
            // Get bit value at position j
            bool rock = this->_horizontalBits[i] & (1 << j);
            std::cout << rock;
         }
         std::cout << std::endl;
      }
   }

   ulong getMirroredBits(ulong value, int lenght)
   {
      ulong result = 0;
      for(int i=0; i<lenght; ++i)
      {
         // Get bit value at position i
         bool rock = value & (1 << i);
         result = result | (rock << (lenght-1-i));
      }
      return result;
   }

   void printMirroredBits()
   {
      for(int i=0; i<this->_horizontalBits.size(); ++i)
      {
         ulong mirrored = this->getMirroredBits(this->_horizontalBits[i], this->_verticalBits.size());
         for(int j=0; j<this->_verticalBits.size(); ++j)
         {
            // Get bit value at position j
            bool rock = mirrored & (1 << j);
            std::cout << rock;
         }
         std::cout << std::endl;
      }
   }

   ulong getReflectionOutput()
   {
      ulong sum = 0;

      // Find horizontal mirror line
      bool found = false;
      int yMirror = 0;
      do
      {
         int delta = 0;
         found = true;
         do
         {
            found = this->_horizontalBits[yMirror-delta] == this->_horizontalBits[yMirror+1+delta];
            delta++;
         }
         while((found == true) && (yMirror-delta >= 0) && (yMirror+delta < this->_horizontalBits.size()-1));
         yMirror++;
      }
      while((found == false) && (yMirror < this->_horizontalBits.size()-1));

      if( found == true )
      {
         sum = yMirror * 100;
      }

      // Find vertical mirror line
      found = false;
      int xMirror = 0;
      do
      {
         int delta = 0;
         found = true;
         do
         {
            found = this->_verticalBits[xMirror-delta] == this->_verticalBits[xMirror+1+delta];
            delta++;
         }
         while((found == true) && (xMirror-delta >= 0) && (xMirror+delta < this->_verticalBits.size()-1));
         xMirror++;
      }
      while((found == false) && (xMirror < this->_verticalBits.size()-1));

      if( found == true )
      {
         sum += xMirror;
      }


      return sum;
   }
};

class Helper
{
private:
   long _firstPuzzleAnswer = 0;
   long _secondPuzzleAnswer = 0;
   std::vector<std::string> _fileInput;

public:
   Helper() {}
   ~Helper() {}

   void calculateFirstPuzzleAnswer()
   {
      this->_firstPuzzleAnswer = 0;

      int initialLine = 0;
      int lastLine = 0;
      do
      {
         do
         {
            lastLine++;
         }
         while((lastLine < this->_fileInput.size()) && (this->_fileInput[lastLine].size() > 0));

         AshRocksPattern pattern;
         pattern.initialize(this->_fileInput, initialLine, lastLine);
         this->_firstPuzzleAnswer += pattern.getReflectionOutput();

         lastLine++;
         initialLine = lastLine;
         std::cout << std::endl;
      }
      while(initialLine < this->_fileInput.size());
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

   long getFirstPuzzleAnswer() { return _firstPuzzleAnswer; }
   long getSecondPuzzleAnswer() { return _secondPuzzleAnswer; }
};

int main()
{
   std::cout << "Advent of Code 2023 - Day 13" << std::endl;

   const std::string inputFileName = "testinput";
   Helper helper;
   helper.calculateAnswers(inputFileName);

   long answer = helper.getFirstPuzzleAnswer();
   std::cout << "First half answer: " << answer << std::endl;

   answer = helper.getSecondPuzzleAnswer();
   std::cout << "Second half answer: " << answer << std::endl;
}