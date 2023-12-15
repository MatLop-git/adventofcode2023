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
//         std::cout << "- line " << line << std::endl;
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

//               std::cout << " number " << numberString << " is part number: " << isPartNumber << std::endl;

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
      this->_secondPuzzleAnswer = 0;

      for(int i=0; i<_fileInput.size(); ++i)
      {
         std::string line = _fileInput[i];
//         std::cout << "- line " << line << std::endl;
         int lineLength = line.length();

         // for each line search for '*' characters
         std::string::size_type parsePos = line.find_first_of("*");
         if( parsePos != std::string::npos )
         {
            do
            {
               int connectedNumbers[2];
               int index = 0;

               // look for number on previous line
               if( i > 0 )
               {
                  std::string topLine = _fileInput[i-1];
                  // look for the character on top of the '*', if it is a number there's no possibility of connecting with two numbers diagonally on top
                  if( topLine[parsePos] >= '0' && topLine[parsePos] <= '9' )
                  {
                     connectedNumbers[index] = this->getNumber(topLine, parsePos);
                     index++;
                  }
                  else
                  {
                     // look for the character on top left, if it's a number obtain the full number
                     if( (parsePos > 0) && (topLine[parsePos-1] >= '0' && topLine[parsePos-1] <= '9') )
                     {
                        connectedNumbers[index] = this->getNumber(topLine, parsePos-1);
                        index++;
                     }
                     // look for the character on top right, if it's a number obtain the full number
                     if( (parsePos < lineLength-1) && (topLine[parsePos+1] >= '0' && topLine[parsePos+1] <= '9') )
                     {
                        connectedNumbers[index] = this->getNumber(topLine, parsePos+1);
                        index++;
                     }
                  }
               }
               // look for numbers on next line
               if( i < _fileInput.size()-1 )
               {
                  std::string bottomLine = _fileInput[i+1];
                  // look for the character on bottom of the '*', if it is a number there's no possibility of connecting with two numbers diagonally on bottom
                  if( bottomLine[parsePos] >= '0' && bottomLine[parsePos] <= '9' )
                  {
                     if( index < 2 )
                     {
                        connectedNumbers[index] = this->getNumber(bottomLine, parsePos);
                     }
                     index++;
                  }
                  else
                  {
                     // look for the character on bottom left, if it's a number obtain the full number
                     if( (parsePos > 0) && (bottomLine[parsePos-1] >= '0' && bottomLine[parsePos-1] <= '9') )
                     {
                        if( index < 2 )
                        {
                           connectedNumbers[index] = this->getNumber(bottomLine, parsePos-1);
                        }
                        index++;
                     }
                     // look for the character on bottom right, if it's a number obtain the full number
                     if( (parsePos < lineLength-1) && (bottomLine[parsePos+1] >= '0' && bottomLine[parsePos+1] <= '9') )
                     {
                        if( index < 2 )
                        {
                           connectedNumbers[index] = this->getNumber(bottomLine, parsePos+1);
                        }
                        index++;
                     }
                  }
               }
               // look for the character on left, if it is a number obtain the full number
               if( (parsePos > 0) && (index < 3) )
               {
                  if( line[parsePos-1] >= '0' && line[parsePos-1] <= '9' )
                  {
                     if( index < 2 )
                     {
                        connectedNumbers[index] = this->getNumber(line, parsePos-1);
                     }
                     index++;
                  }
               }
               // look for the character on right, if it is a number obtain the full number
               if( (parsePos < lineLength-1) && (index < 3) )
               {
                  if( line[parsePos+1] >= '0' && line[parsePos+1] <= '9' )
                  {
                     if( index < 2 )
                     {
                        connectedNumbers[index] = this->getNumber(line, parsePos+1);
                     }
                     index++;
                  }
               }

//               std::cout << " * connected " << index << std::endl;

               // if we obtained two different numbers, multiply them and sum the result to the answer
               if( index == 2 )
               {
//               std::cout << " values " << connectedNumbers[0] << "x" << connectedNumbers[1] << "=" << (connectedNumbers[0] * connectedNumbers[1]) << std::endl;
                  _secondPuzzleAnswer += (connectedNumbers[0] * connectedNumbers[1]);
               }

               parsePos = (parsePos < lineLength-1) ? line.find_first_of("*", parsePos+1) : std::string::npos;
            }
            while( parsePos != std::string::npos );
         }
      }
   }

   int getNumber(std::string line, std::string::size_type index)
   {
      std::string::size_type parsePos = index;
      while( (parsePos > 0) && (line[parsePos-1] >= '0' && line[parsePos-1] <= '9') )
      {
//         std::cout << "pos " << parsePos << " val "<< line[parsePos] << std::endl;
         parsePos--;
      }
      std::string::size_type parseEndPos = line.find_first_not_of("0123456789", index);
      std::string numberString = line.substr(parsePos, parseEndPos-parsePos).c_str();
      int number = atoi(numberString.c_str());
//         std::cout << "getNumber " << numberString << " val "<< number << std::endl;
      return number;
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