#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

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
         std::cout << "- line " << line << std::endl;
         int lineLength = line.length();

         // for each line search for numeric characters
         std::string::size_type parsePos = line.find_first_of("0123456789");
         if( parsePos != std::string::npos )
         {
            do
            {
               // for each number character obtain the complete number by searching for the end of the number
               std::string::size_type parseEndPos = line.find_first_not_of("0123456789", parsePos);
               std::string numberString = line.substr(parsePos, parseEndPos-parsePos);

               // for each number character look around in the map for non '.' non number characters
               // add deltas to look one digit left and right of the number, except for numbers already on the border of the map
               bool isPartNumber = false;
               if( parsePos > 0 )
               {
                  parsePos--;
                  isPartNumber = line[parsePos] != '.';
               }
               if( !isPartNumber && parseEndPos < lineLength-1 )
               {
                  isPartNumber = line[parseEndPos] != '.';
                  parseEndPos++;
               }
               // look for characters on top
               if( !isPartNumber && i > 0 )
               {
                  std::string previosLine = _fileInput[i-1].substr(parsePos, parseEndPos-parsePos);
                  isPartNumber = ( previosLine.find_first_not_of(".") != std::string::npos );
               }
               if( !isPartNumber && i<_fileInput.size()-1 )
               {
                  std::string nextLine = _fileInput[i+1].substr(parsePos, parseEndPos-parsePos);
                  isPartNumber = ( nextLine.find_first_not_of(".") != std::string::npos );
               }

               // std::cout << "line " << line << " first number " << numberString << " is part number " << isPartNumber << std::endl;
               std::cout << " number " << numberString << " is part number: " << isPartNumber << std::endl;

               if( isPartNumber )
               {
                  _firstPuzzleAnswer += atoi(numberString.c_str());
               }

               parsePos = line.find_first_of("0123456789", parseEndPos);
            }
            while( parsePos != std::string::npos );
         }
      }
   }

   void calculateSecondPuzzleAnswer()
   {
      
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
   std::cout << "Advent of Code 2023 - Day 03" << std::endl;

   const std::string inputFileName = "input";
   Helper helper;
   helper.calculateAnswers(inputFileName);

   int answer = helper.getFirstPuzzleAnswer();
   std::cout << "First half answer: " << answer << std::endl;

   answer = helper.getSecondPuzzleAnswer();
   std::cout << "Second half answer: " << answer << std::endl;
}