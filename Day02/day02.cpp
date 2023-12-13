#include <fstream>
#include <iostream>
#include <string>
#include <utility>

static const char* RED_VALUE = " red";
static const char* GREEN_VALUE = " green";
static const char* BLUE_VALUE = " blue";

static const int MAX_RED_CUBES = 12;
static const int MAX_GREEN_CUBES = 13;
static const int MAX_BLUE_CUBES = 14;

class Helper
{
public:

   Helper() {}
   ~Helper() {}

   int getLineValue(std::string line)
   {
      // parse game id
      std::string::size_type parsePos = line.find(":");
      int gameId = atoi(line.substr(5, parsePos).c_str());

      std::cout << gameId << " - " << line << std::endl;

      // parse cubes
      std::string::size_type parseEndPos = line.find_first_of(",;", parsePos);
      bool hasValues = true;
      do
      {
         hasValues = parseEndPos != std::string::npos;
         std::string setString = "";
         if( hasValues )
         {
            setString = line.substr(parsePos+1, parseEndPos-parsePos);
         }
         else
         {
            setString = line.substr(parsePos+1);
         }
         std::cout << "- " << setString << std::endl;

         // determine cube color
         std::string::size_type colourPos = setString.find(RED_VALUE);
         if( colourPos != std::string ::npos )
         {
            int cubes = atoi(setString.substr(0, colourPos).c_str());
         std::cout << "- " << cubes << " red on set " << setString << std::endl;
            if( cubes > MAX_RED_CUBES )
               return 0;
         }
         
         colourPos = setString.find(GREEN_VALUE);
//         std::cout << "- " << colourPos << std::endl;
         if( colourPos != std::string ::npos )
         {
            int cubes = atoi(setString.substr(0, colourPos).c_str());
         std::cout << "- " << cubes << " green on set " << setString << std::endl;
            if( cubes > MAX_GREEN_CUBES )
               return 0;
         }

         colourPos = setString.find(BLUE_VALUE);
         if( colourPos != std::string ::npos )
         {
            int cubes = atoi(setString.substr(0, colourPos).c_str());
         std::cout << "- " << cubes << " blue on set " << setString << std::endl;
            if( cubes > MAX_BLUE_CUBES )
               return 0;
         }

         if( hasValues )
         {
            parsePos = parseEndPos+1;
            parseEndPos = line.find_first_of(",;", parsePos);
         }
      }
      while(hasValues);

      return gameId;
   }

   int getPuzzleAnswer(std::string inputFileName)
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
   std::cout << "Advent of Code 2023 - Day 02" << std::endl;

   const std::string inputFileName = "input";
   Helper helper;
   int totalSum = helper.getPuzzleAnswer(inputFileName);

   std::cout << "Total: " << totalSum << std::endl;
}
