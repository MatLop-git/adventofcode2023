#include <algorithm>    // std::sort
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

class Helper
{
private:
   long _firstPuzzleAnswer = 0;
   long _secondPuzzleAnswer = 0;
   std::vector<std::string> _fileInput;

public:
   Helper() {}
   ~Helper() {}

   long getNextExtrapolatedValue(std::vector<long> history)
   {
      std::vector<long> sequence = history;
      long extrapolatedValue = sequence[sequence.size()-1];
      // Find the sequence where there are all zeros
      bool allZeros = true;
      do
      {
         allZeros = true;
         std::vector<long> nextSequence;
         for(int i=0; i<sequence.size()-1; ++i)
         {
            long difference = sequence[i+1] - sequence[i];
            nextSequence.push_back(difference);
            allZeros = allZeros && (difference == 0);
            std::cout << difference << " ";
         }
         std::cout << std::endl;
         sequence = nextSequence;
         extrapolatedValue += sequence[sequence.size()-1];
      }
      while (allZeros == false);
      
      return extrapolatedValue;
   }

   long getPreviousExtrapolatedValue(std::vector<long> history)
   {
      std::vector<long> sequence = history;
      long extrapolatedValue = sequence[0];
      // Find the sequence where there are all zeros
      bool allZeros = true;
      int multiplier = -1; // Don't ask why, I just noticed that instead of always sum (like getNext) going backwards you need to interleave sums and substractions
      do
      {
         allZeros = true;
         std::vector<long> nextSequence;
         for(int i=0; i<sequence.size()-1; ++i)
         {
            long difference = sequence[i+1] - sequence[i];
            nextSequence.push_back(difference);
            allZeros = allZeros && (difference == 0);
            std::cout << difference << " ";
         }
         std::cout << std::endl;
         sequence = nextSequence;
         extrapolatedValue += (sequence[0] * multiplier);
         multiplier *= -1;
      }
      while (allZeros == false);
      
      return extrapolatedValue;
   }

   void calculateFirstPuzzleAnswer()
   {
      this->_firstPuzzleAnswer = 0;

      // Obtain OASIS report
      char* end = nullptr;
      for(int i=0; i<this->_fileInput.size(); ++i)
      {
         std::string line = _fileInput[i];
         // Get history values
         std::vector<long> history;
         std::string::size_type parsePos = 0;
         do
         {
            std::string::size_type parseEndPos = line.find(" ", parsePos);
            std::string valueString = line.substr(parsePos, parseEndPos);
            long value = strtoul(valueString.c_str(),&end,10);
            history.push_back(value);
            std::cout << value << " ";
            parsePos = parseEndPos;
            if(parsePos != std::string::npos)
            {
               parsePos++;
            }
         }
         while(parsePos != std::string::npos);

         std::cout << std::endl;
         long extrapolatedValue = this->getNextExtrapolatedValue(history);
         std::cout << "extrapolated " << extrapolatedValue << std::endl;
         this->_firstPuzzleAnswer += extrapolatedValue;
      }
   }

   void calculateSecondPuzzleAnswer()
   {
      this->_secondPuzzleAnswer = 0;

      // Obtain OASIS report
      char* end = nullptr;
      for(int i=0; i<this->_fileInput.size(); ++i)
      {
         std::string line = _fileInput[i];
         // Get history values
         std::vector<long> history;
         std::string::size_type parsePos = 0;
         do
         {
            std::string::size_type parseEndPos = line.find(" ", parsePos);
            std::string valueString = line.substr(parsePos, parseEndPos);
            long value = strtoul(valueString.c_str(),&end,10);
            history.push_back(value);
            std::cout << value << " ";
            parsePos = parseEndPos;
            if(parsePos != std::string::npos)
            {
               parsePos++;
            }
         }
         while(parsePos != std::string::npos);

         std::cout << std::endl;
         long extrapolatedValue = this->getPreviousExtrapolatedValue(history);
         std::cout << "extrapolated " << extrapolatedValue << std::endl;
         this->_secondPuzzleAnswer += extrapolatedValue;
      }
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
   std::cout << "Advent of Code 2023 - Day 09" << std::endl;

   const std::string inputFileName = "input";
   Helper helper;
   helper.calculateAnswers(inputFileName);

   long answer = helper.getFirstPuzzleAnswer();
   std::cout << "First half answer: " << answer << std::endl;

   answer = helper.getSecondPuzzleAnswer();
   std::cout << "Second half answer: " << answer << std::endl;
}