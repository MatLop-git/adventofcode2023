#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <cmath>

static const int WINNING_NUMBERS_POSITION = 10;
static const int WINNING_NUMBERS_SIZE = 10;
static const int YOUR_NUMBERS_POSITION = 42;
static const int YOUR_NUMBERS_SIZE = 25;
static const int NUMBERS_WIDTH = 2;

class Helper
{
private:
   int _firstPuzzleAnswer = 0;
   int _secondPuzzleAnswer = 0;
   std::vector<std::string> _fileInput;

public:
   Helper() {}
   ~Helper() {}

   void calculateFirstPuzzleAnswer()
   {
      this->_firstPuzzleAnswer = 0;

      for(int i=0; i<_fileInput.size(); ++i)
      {
         std::string line = _fileInput[i];
         int lineLength = line.length();

         // parse game id
//         std::string::size_type parsePos = line.find(":");
//         int gameId = atoi(line.substr(5, parsePos).c_str());
//         std::cout << line.substr(0, WINNING_NUMBERS_POSITION);

         // discriminate winning numbers
         std::vector<int> winningNumbers;
         int pos = WINNING_NUMBERS_POSITION;
         for(int j=0; j<WINNING_NUMBERS_SIZE; ++j)
         {
            std::string nString = line.substr(pos, NUMBERS_WIDTH+1);
//            std::cout << "- " << nString;
            winningNumbers.push_back(atoi(nString.c_str()));
//            std::cout << winningNumbers[j] << " # ";
            pos += NUMBERS_WIDTH+1;
         }
//      std::cout << std::endl;

         // discriminate your numbers
         std::vector<int> yourNumbers;
         pos = YOUR_NUMBERS_POSITION;
         for(int j=0; j<YOUR_NUMBERS_SIZE; ++j)
         {
            std::string nString = line.substr(pos, NUMBERS_WIDTH+1);
//            std::cout << "- " << nString;
            yourNumbers.push_back(atoi(nString.c_str()));
//            std::cout << yourNumbers[j] << " # ";
            pos += NUMBERS_WIDTH+1;
         }

         // verify points
         int pointsDelta = 0;
         for(int j=0; j<WINNING_NUMBERS_SIZE; ++j)
         {
            for(int k=0; k<YOUR_NUMBERS_SIZE; ++k)
            {
               if( winningNumbers[j] == yourNumbers[k] )
               {
//            std::cout << "- " << winningNumbers[j];
                  pointsDelta = (pointsDelta == 0) ? 1 : (pointsDelta *= 2);
               }
            }
         }
         this->_firstPuzzleAnswer += pointsDelta;

//         std::cout << " " << log2(pointsDelta) << std::endl;
      }
   }

   void calculateSecondPuzzleAnswer()
   {
      this->_secondPuzzleAnswer = 0;

      // save number of matching numbers per line
      int matches[_fileInput.size()];
      for(int i=0; i<_fileInput.size(); ++i)
      {
         std::string line = _fileInput[i];
         int lineLength = line.length();

         // parse game id
//         std::string::size_type parsePos = line.find(":");
//         int gameId = atoi(line.substr(5, parsePos).c_str());
//         std::cout << line.substr(0, WINNING_NUMBERS_POSITION);

         // discriminate winning numbers
         std::vector<int> winningNumbers;
         int pos = WINNING_NUMBERS_POSITION;
         for(int j=0; j<WINNING_NUMBERS_SIZE; ++j)
         {
            std::string nString = line.substr(pos, NUMBERS_WIDTH+1);
//            std::cout << "- " << nString;
            winningNumbers.push_back(atoi(nString.c_str()));
//            std::cout << winningNumbers[j] << " # ";
            pos += NUMBERS_WIDTH+1;
         }
//      std::cout << std::endl;

         // discriminate your numbers
         std::vector<int> yourNumbers;
         pos = YOUR_NUMBERS_POSITION;
         for(int j=0; j<YOUR_NUMBERS_SIZE; ++j)
         {
            std::string nString = line.substr(pos, NUMBERS_WIDTH+1);
//            std::cout << "- " << nString;
            yourNumbers.push_back(atoi(nString.c_str()));
//            std::cout << yourNumbers[j] << " # ";
            pos += NUMBERS_WIDTH+1;
         }

         // verify points
         matches[i] = 0;
         for(int j=0; j<WINNING_NUMBERS_SIZE; ++j)
         {
            for(int k=0; k<YOUR_NUMBERS_SIZE; ++k)
            {
               if( winningNumbers[j] == yourNumbers[k] )
               {
//            std::cout << "- " << winningNumbers[j];
                  matches[i]++;
               }
            }
         }
      }

      // calculate number of copies
      int copies[_fileInput.size()];
      for(int i=0; i<_fileInput.size(); ++i)
      {
         copies[i] = 1;
      }
      for(int i=0; i<_fileInput.size(); ++i)
      {
         for(int j=1; j<=matches[i]; ++j)
         {
            copies[i+j] += copies[i];
         }
      }

      // multiply points per copies
      for(int i=0; i<_fileInput.size(); ++i)
      {
         this->_secondPuzzleAnswer += copies[i];
            std::cout << i << " - " << copies[i] << " " << matches[i] << " > " << this->_secondPuzzleAnswer << std::endl;
//         if( matches[i] > 0 )
//         {
//            int points = (matches[i] == 1) ? 1 : pow(2,matches[i]-1);
//            std::cout << i << " - " << matches[i] << " " << points << " " << copies[i] << " = " << (points * copies[i]) << std::endl;
//            this->_secondPuzzleAnswer += (points * copies[i]);
//         }
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
   std::cout << "Advent of Code 2023 - Day 04" << std::endl;

   const std::string inputFileName = "input";
   Helper helper;
   helper.calculateAnswers(inputFileName);

   int answer = helper.getFirstPuzzleAnswer();
   std::cout << "First half answer: " << answer << std::endl;

   answer = helper.getSecondPuzzleAnswer();
   std::cout << "Second half answer: " << answer << std::endl;
}