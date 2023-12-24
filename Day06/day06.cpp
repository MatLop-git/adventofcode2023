#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

struct TimeDistancePair
{
   int Time = 0;
   int Distance = 0;

   TimeDistancePair() {}
   TimeDistancePair(int time, int distance) : Time(time), Distance(distance) {}
   ~TimeDistancePair() {}
};

class Helper
{
private:
   int _firstPuzzleAnswer = 0;
   int _secondPuzzleAnswer = 0;
   std::vector<std::string> _fileInput;

   std::vector<TimeDistancePair> _timeDistancePairs;

public:
   Helper() {}
   ~Helper() {}

   void calculateFirstPuzzleAnswer()
   {
      this->_firstPuzzleAnswer = 0;

      std::string lineTime = _fileInput[0];
      std::string lineDistance = _fileInput[1];

      // obtain times and distances
      std::string::size_type parsePos = lineDistance.find(":")+1;
      do
      {
         parsePos = lineDistance.find_first_of("0123456789", parsePos);
         std::string::size_type parseEndPos = lineDistance.find(" ", parsePos);
         std::string timeString = lineTime.substr(parsePos, parseEndPos);
         std::string distanceString = lineDistance.substr(parsePos, parseEndPos);
         int time = atoi(timeString.c_str());
         int distance = atoi(distanceString.c_str());
         this->_timeDistancePairs.push_back(TimeDistancePair(time, distance));
         parsePos = parseEndPos;
         if(parsePos != std::string::npos)
         {
            parsePos++;
         }
      }
      while(parsePos != std::string::npos);

      // calculate number of ways you can beat the record of each race
      std::vector<int> waysToBeatRecords;
      for(int i=0; i<this->_timeDistancePairs.size(); ++i)
      {
         int waysToBeatRecord = 0;
         TimeDistancePair pair = this->_timeDistancePairs[i];
         std::cout << "Ways to beat " << pair.Distance << " in " << pair.Time << std::endl;
         for(int j=1; j<pair.Time-1; ++j)
         {
            int distance = j * (pair.Time-j);
            if( distance > pair.Distance )
            {
               std::cout << j << " goes " << distance << std::endl;
               waysToBeatRecord++;
            }
         }
         waysToBeatRecords.push_back(waysToBeatRecord);
      }

      // calculate margin of error
      int margin = waysToBeatRecords[0];
      std::cout << waysToBeatRecords[0];
      for(int i=1; i<waysToBeatRecords.size(); ++i)
      {
         std::cout << " * " << waysToBeatRecords[i];
         margin = margin * waysToBeatRecords[i];
      }
      std::cout << " = " << margin << std::endl;

      this->_firstPuzzleAnswer = margin;
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
   std::cout << "Advent of Code 2023 - Day 06" << std::endl;

   const std::string inputFileName = "input";
   Helper helper;
   helper.calculateAnswers(inputFileName);

   ulong answer = helper.getFirstPuzzleAnswer();
   std::cout << "First half answer: " << answer << std::endl;

   answer = helper.getSecondPuzzleAnswer();
   std::cout << "Second half answer: " << answer << std::endl;
}