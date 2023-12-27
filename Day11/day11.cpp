#include <algorithm>    // std::sort
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

struct Coord
{
   int X, Y = 0;

   Coord() {}
   Coord(int x, int y) : X(x), Y(y) {}
   ~Coord() {}
};

class Helper
{
private:
   ulong _firstPuzzleAnswer = 0;
   ulong _secondPuzzleAnswer = 0;
   std::vector<std::string> _fileInput;

public:
   Helper() {}
   ~Helper() {}

   std::vector<Coord> getGalaxies()
   {
      std::vector<Coord> galaxies;
      for(int y=0; y<this->_fileInput.size(); ++y)
      {
         for(int x=0; x<this->_fileInput[y].length(); ++x)
         {
            if(this->_fileInput[y][x] == '#')
            {
               galaxies.push_back(Coord(x, y));
            }
         }
      }
      return galaxies;
   }

   std::vector<std::pair<Coord, Coord>> getGalaxiesPairs(std::vector<Coord> &galaxies)
   {
      std::vector<std::pair<Coord, Coord>> pairs;
      for(int i=0; i<galaxies.size()-1; ++i)
      {
         for(int j=i+1; j<galaxies.size(); ++j)
         {
            pairs.push_back(std::pair(galaxies[i], galaxies[j]));
         }
      }
      return pairs;
   }

   std::vector<std::vector<long>> getExpandedUniverse(long expansionRate)
   {
      std::vector<std::vector<long>> universe;
      int xSize = this->_fileInput[0].length();
      int ySize = this->_fileInput.size();
      // Verify horizontal expansion
      for(int y=0; y<ySize; ++y)
      {
         bool expandedX = (this->_fileInput[y].find_first_not_of('.') == std::string::npos);
         std::vector<long> xVector;
         for(int x=0; x<xSize; ++x)
         {
            xVector.push_back( expandedX ? expansionRate : 1 );
         }
         universe.push_back(xVector);
      }
      // Verify vertical expansion
      for(int x=0; x<xSize; ++x)
      {
         bool expandedY = true;
         for(int y=0; y<ySize; ++y)
         {
            if(this->_fileInput[y][x] != '.')
            {
               expandedY = false;
               break;
            }
         }
         if( expandedY == true )
         {
            for(int y=0; y<ySize; ++y)
            {
               universe[y][x] = expansionRate;
            }
         }
      }
      return universe;
   }

   ulong getDistance(std::vector<std::vector<long>> &universe, Coord coordA, Coord coordB)
   {
      int x0 = coordA.X;
      int x1 = coordB.X;
      if(coordA.X > coordB.X)
      {
         x0 = coordB.X;
         x1 = coordA.X;
      }
      int y0 = coordA.Y;
      int y1 = coordB.Y;
      if(coordA.Y > coordB.Y)
      {
         y0 = coordB.Y;
         y1 = coordA.Y;
      }

      ulong dist = 0;
      if( x0 != x1 )
      {
         for(int x=x0; x<x1; ++x)
         {
            dist += universe[y0][x];
         }
      }
      if( y0 != y1 )
      {
         for(int y=y0; y<y1; ++y)
         {
            dist += universe[y][x1];
         }
      }

      return dist;
   }

   void printUniverse(std::vector<std::vector<long>> &universe)
   {
      for(int y=0; y<universe.size(); ++y)
      {
         std::vector<long> xVector = universe[y];
         for(int x=0; x<xVector.size(); ++x)
         {
            std::cout << xVector[x];
         }
         std::cout << std::endl;
      }
   }

   void calculateFirstPuzzleAnswer()
   {
      this->_firstPuzzleAnswer = 0;

      std::vector<Coord> galaxies = this->getGalaxies();
      std::vector<std::vector<long>> universe = this->getExpandedUniverse(2);

//      this->printUniverse(universe);

      std::vector<std::pair<Coord, Coord>> pairs = this->getGalaxiesPairs(galaxies);
      for(int i=0; i<pairs.size(); ++i)
      {
         Coord coordA = pairs[i].first;
         Coord coordB = pairs[i].second;
         ulong dist = this->getDistance(universe, coordA, coordB);
//         std::cout << "[" << coordA.X << "][" << coordA.Y << "]->[" << coordB.X << "][" << coordB.Y << "] = " << dist << std::endl;

         this->_firstPuzzleAnswer += dist;
      }
   }

   void calculateSecondPuzzleAnswer()
   {
      this->_secondPuzzleAnswer = 0;

      std::vector<Coord> galaxies = this->getGalaxies();
      std::vector<std::vector<long>> universe = this->getExpandedUniverse(1000000);

//      this->printUniverse(universe);

      std::vector<std::pair<Coord, Coord>> pairs = this->getGalaxiesPairs(galaxies);
      for(int i=0; i<pairs.size(); ++i)
      {
         Coord coordA = pairs[i].first;
         Coord coordB = pairs[i].second;
         ulong dist = this->getDistance(universe, coordA, coordB);
//         std::cout << "[" << coordA.X << "][" << coordA.Y << "]->[" << coordB.X << "][" << coordB.Y << "] = " << dist << std::endl;

         this->_secondPuzzleAnswer += dist;
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

   ulong getFirstPuzzleAnswer() { return _firstPuzzleAnswer; }
   ulong getSecondPuzzleAnswer() { return _secondPuzzleAnswer; }
};

int main()
{
   std::cout << "Advent of Code 2023 - Day 11" << std::endl;

   const std::string inputFileName = "input";
   Helper helper;
   helper.calculateAnswers(inputFileName);

   ulong answer = helper.getFirstPuzzleAnswer();
   std::cout << "First half answer: " << answer << std::endl;

   answer = helper.getSecondPuzzleAnswer();
   std::cout << "Second half answer: " << answer << std::endl;
}