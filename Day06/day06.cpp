#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

struct TimeDistancePair
{
   ulong Time = 0;
   ulong Distance = 0;

   TimeDistancePair() {}
   TimeDistancePair(ulong time, ulong distance) : Time(time), Distance(distance) {}
   ~TimeDistancePair() {}
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

   void calculateFirstPuzzleAnswer()
   {
      this->_firstPuzzleAnswer = 0;

      std::string lineTime = _fileInput[0];
      std::string lineDistance = _fileInput[1];

      // obtain times and distances
      std::vector<TimeDistancePair> timeDistancePairs;
      std::string::size_type parsePos = lineDistance.find(":")+1;
      do
      {
         parsePos = lineDistance.find_first_of("0123456789", parsePos);
         std::string::size_type parseEndPos = lineDistance.find(" ", parsePos);
         std::string timeString = lineTime.substr(parsePos, parseEndPos);
         std::string distanceString = lineDistance.substr(parsePos, parseEndPos);
         int time = atoi(timeString.c_str());
         int distance = atoi(distanceString.c_str());
         timeDistancePairs.push_back(TimeDistancePair(time, distance));
         parsePos = parseEndPos;
         if(parsePos != std::string::npos)
         {
            parsePos++;
         }
      }
      while(parsePos != std::string::npos);

      // calculate number of ways you can beat the record of each race
      std::vector<ulong> waysToBeatRecords;
      for(int i=0; i<timeDistancePairs.size(); ++i)
      {
         ulong waysToBeatRecord = 0;
         TimeDistancePair pair = timeDistancePairs[i];
         std::cout << "Ways to beat " << pair.Distance << " in " << pair.Time << std::endl;
         for(ulong j=1; j<pair.Time-1; ++j)
         {
            ulong distance = j * (pair.Time-j);
            if( distance > pair.Distance )
            {
               std::cout << j << " goes " << distance << std::endl;
               waysToBeatRecord++;
            }
         }
         waysToBeatRecords.push_back(waysToBeatRecord);
      }

      // calculate margin of error
      ulong margin = waysToBeatRecords[0];
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

      std::string lineTime = _fileInput[0];
      std::string lineDistance = _fileInput[1];

      // obtain time and distance strings and remove spaces
      std::string::size_type parsePos = lineDistance.find(":")+1;
      std::string timeString = lineTime.substr(parsePos);
      timeString.erase(std::remove(timeString.begin(), timeString.end(), ' '), timeString.end());
      std::string distanceString = lineDistance.substr(parsePos);
      distanceString.erase(std::remove(distanceString.begin(), distanceString.end(), ' '), distanceString.end());
      char* end = nullptr;
      ulong time = strtoul(timeString.c_str(),&end,10);
      ulong distance = strtoul(distanceString.c_str(),&end,10);
      TimeDistancePair pair(time, distance);

      // find the minimum time to beat the record
      std::cout << "Ways to beat " << pair.Distance << " in " << pair.Time << std::endl;
      ulong minTime = 0, maxTime = pair.Time;
      time = minTime;
      do
      {
         time++;
         distance = time * (pair.Time-time);
         if( distance > pair.Distance )
         {
            std::cout << time << " goes " << distance << std::endl;
            minTime = time;
         }
      }
      while(minTime == 0);
      // find the maximum time after which it stops beating the record
      time = maxTime;
      do
      {
         time--;
         distance = time * (pair.Time-time);
         if( distance > pair.Distance )
         {
            std::cout << time << " goes " << distance << std::endl;
            maxTime = time;
         }
      }
      while(maxTime == pair.Time);

      // calculate number of ways you can beat the record
      // by substracting the min time from the max time we found the number of ways without having to go through all of them
      ulong waysToBeatRecord = maxTime - minTime + 1;

      // calculate margin of error
      this->_secondPuzzleAnswer = waysToBeatRecord;
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