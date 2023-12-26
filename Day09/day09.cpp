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
   ulong _firstPuzzleAnswer = 0;
   ulong _secondPuzzleAnswer = 0;
   std::vector<std::string> _fileInput;

public:
   Helper() {}
   ~Helper() {}

   ulong getExtrapolatedValue(std::vector<ulong> history)
   {
      std::vector<ulong> sequence = history;
//      std::vector<ulong> sequencesEnds;
      ulong extrapolatedValue = 0;
      // Find the sequence where there are all zeros
      bool allZeros = true;
      do
      {
         allZeros = true;
         extrapolatedValue += sequence[sequence.size()-1];
//         sequencesEnds.push_back(sequence[sequence.size()-1]);
         std::vector<ulong> nextSequence;
         for(int i=0; i<sequence.size()-1; ++i)
         {
            ulong difference = sequence[i+1] - sequence[i];
            nextSequence.push_back(difference);
            allZeros = allZeros && (difference == 0);
            std::cout << difference << " ";
         }
         std::cout << std::endl;
         sequence = nextSequence;
      }
      while (allZeros == false);
      
//      // Go up in sequences to find the extrapolated value
//      ulong extrapolatedValue = sequencesEnds[sequencesEnds.size()-1];
//      for(int i=sequencesEnds.size()-2; i>=0; --i)
//      {
//         extrapolatedValue += sequencesEnds[i]
//      }
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
         std::vector<ulong> history;
         std::string::size_type parsePos = 0;
         do
         {
            std::string::size_type parseEndPos = line.find(" ", parsePos);
            std::string valueString = line.substr(parsePos, parseEndPos);
            ulong value = strtoul(valueString.c_str(),&end,10);
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
         ulong extrapolatedValue = this->getExtrapolatedValue(history);
         std::cout << "extrapolated " << extrapolatedValue << std::endl;
         this->_firstPuzzleAnswer += extrapolatedValue;
      }

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
   std::cout << "Advent of Code 2023 - Day 09" << std::endl;

   const std::string inputFileName = "input";
   Helper helper;
   helper.calculateAnswers(inputFileName);

   ulong answer = helper.getFirstPuzzleAnswer();
   std::cout << "First half answer: " << answer << std::endl;

   answer = helper.getSecondPuzzleAnswer();
   std::cout << "Second half answer: " << answer << std::endl;
}