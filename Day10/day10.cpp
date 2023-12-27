#include <algorithm>    // std::sort
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

enum Direction { None, Right, Left, Up, Down };

class Helper
{
private:
   long _firstPuzzleAnswer = 0;
   long _secondPuzzleAnswer = 0;
   std::vector<std::string> _fileInput;

public:
   Helper() {}
   ~Helper() {}

   Direction getNextDirection(Direction comesFrom, char pipe)
   {
      switch( pipe )
      {
         case '-':
         {
            if( comesFrom == Direction::Left )
            {
               return Direction::Right;
            }
            else if( comesFrom == Direction::Right )
            {
               return Direction::Left;
            }
            return Direction::None;
         }
         case '|':
         {
            if( comesFrom == Direction::Up )
            {
               return Direction::Down;
            }
            else if( comesFrom == Direction::Down )
            {
               return Direction::Up;
            }
            return Direction::None;
         }
         case '7':
         {
            if( comesFrom == Direction::Left )
            {
               return Direction::Down;
            }
            else if( comesFrom == Direction::Down )
            {
               return Direction::Left;
            }
            return Direction::None;
         }
         case 'J':
         {
            if( comesFrom == Direction::Left )
            {
               return Direction::Up;
            }
            else if( comesFrom == Direction::Up )
            {
               return Direction::Left;
            }
            return Direction::None;
         }
         case 'F':
         {
            if( comesFrom == Direction::Right )
            {
               return Direction::Down;
            }
            else if( comesFrom == Direction::Down )
            {
               return Direction::Right;
            }
            return Direction::None;
         }
         case 'L':
         {
            if( comesFrom == Direction::Right )
            {
               return Direction::Up;
            }
            else if( comesFrom == Direction::Up )
            {
               return Direction::Right;
            }
            return Direction::None;
         }
         default:
         {
            return None;
         }
      }
   }

   bool connectsToRight(int x, int y)
   {
      char pipe = this->_fileInput[y][x];
      return (pipe == 'F' || pipe == '-' || pipe == 'L');
   }

   bool connectsToLeft(int x, int y)
   {
      char pipe = this->_fileInput[y][x];
      return (pipe == '7' || pipe == '-' || pipe == 'J');
   }

   bool connectsToUp(int x, int y)
   {
      char pipe = this->_fileInput[y][x];
      return (pipe == 'L' || pipe == '|' || pipe == 'J');
   }

   bool connectsToDown(int x, int y)
   {
      char pipe = this->_fileInput[y][x];
      return (pipe == 'F' || pipe == '|' || pipe == '7');
   }

   std::vector<std::pair<int, int>> getPipesPath(Direction directionFrom, int nextX, int nextY)
   {
      std::vector<std::pair<int, int>> pipes;
      bool reachedUnconnection = false;
      do
      {
         pipes.push_back(std::pair<int, int>(nextX, nextY));
         char pipe = this->_fileInput[nextY][nextX];
         directionFrom = getNextDirection(directionFrom, pipe);
         char dir = (directionFrom == Direction::Left) ? '<' : 
                     (directionFrom == Direction::Right) ? '>' : 
                        (directionFrom == Direction::Up) ? '^' :
                           (directionFrom == Direction::Down) ? 'v' : 'X';
         std::cout << dir;
         reachedUnconnection = directionFrom == Direction::None;
         if( reachedUnconnection == false )
         {
            switch (directionFrom)
            {
               case Direction::Left:
               {
                  directionFrom = Direction::Right;
                  nextX--;
                  break;
               }
               case Direction::Right:
               {
                  directionFrom = Direction::Left;
                  nextX++;
                  break;
               }
               case Direction::Up:
               {
                  directionFrom = Direction::Down;
                  nextY--;
                  break;
               }
               case Direction::Down:
               {
                  directionFrom = Direction::Up;
                  nextY++;
                  break;
               }
            }
         }
      }
      while(!reachedUnconnection);
      std::cout << std::endl;

      return pipes;
   }

   std::vector<std::pair<int, int>> getLoopPipes(int startingX, int startingY)
   {
      std::vector<std::pair<int, int>> pipes;
      bool foundLoop = false;
      // Look connection on right direction
      Direction directionFrom = Direction::Left;
      int nextX = startingX+1;
      int nextY = startingY;
      if( (startingX < this->_fileInput[startingY].length()) && this->connectsToLeft(nextX, nextY) )
      {
         pipes = this->getPipesPath(directionFrom, nextX, nextY);
         foundLoop = (pipes[pipes.size()-1].first == startingX && pipes[pipes.size()-1].second == startingY);
      }
      if( !foundLoop && (startingX > 0) )
      {
         // Look connection on left direction
         directionFrom = Direction::Right;
         nextX = startingX-1;
         nextY = startingY;
         if( this->connectsToRight(nextX, nextY) )
         {
            pipes = this->getPipesPath(directionFrom, nextX, nextY);
            foundLoop = (pipes[pipes.size()-1].first == startingX && pipes[pipes.size()-1].second == startingY);
         }
      }
      if( !foundLoop && (startingY > 0) )
      {
         // Look connection on up direction
         directionFrom = Direction::Down;
         nextX = startingX;
         nextY = startingY-1;
         if( this->connectsToDown(nextX, nextY) )
         {
            pipes = this->getPipesPath(directionFrom, nextX, nextY);
            foundLoop = (pipes[pipes.size()-1].first == startingX && pipes[pipes.size()-1].second == startingY);
         }
      }
      if( !foundLoop && (startingY < this->_fileInput.size()) )
      {
         // Look connection on down direction
         directionFrom = Direction::Up;
         nextX = startingX;
         nextY = startingY+1;
         if( this->connectsToUp(nextX, nextY) )
         {
            pipes = this->getPipesPath(directionFrom, nextX, nextY);
            foundLoop = (pipes[pipes.size()-1].first == startingX && pipes[pipes.size()-1].second == startingY);
         }
      }
      return pipes;
   }

   void calculateFirstPuzzleAnswer()
   {
      this->_firstPuzzleAnswer = 0;

      // Obtain starting point
      int startingY = -1;
      std::string::size_type startingX = std::string::npos;
      do
      {
         ++startingY;
         std::string line = _fileInput[startingY];
         startingX = line.find("S");
      }
      while(startingX == std::string::npos);

      std::cout << "Starting point at [" << startingX << "," << startingY << "]" << std::endl;

      // Find loop
      std::vector<std::pair<int, int>> loopPipes = getLoopPipes(startingX, startingY);

      this->_firstPuzzleAnswer = loopPipes.size()/2;
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
   std::cout << "Advent of Code 2023 - Day 10" << std::endl;

   const std::string inputFileName = "input";
   Helper helper;
   helper.calculateAnswers(inputFileName);

   long answer = helper.getFirstPuzzleAnswer();
   std::cout << "First half answer: " << answer << std::endl;

   answer = helper.getSecondPuzzleAnswer();
   std::cout << "Second half answer: " << answer << std::endl;
}