#include <algorithm>    // std::sort
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

class ConditionRecord
{
protected:
   std::string _springs;
   std::vector<int> _damagedSpringsGroupsSizes;

   std::vector<int> _unknownStatePositions;

public:
   ConditionRecord() {}
   ~ConditionRecord() {}

   virtual void initialize(std::string rawRecord)
   {
      std::string::size_type posGroups = rawRecord.find_first_of("123456789");
      this->_springs = rawRecord.substr(0, posGroups-1);
      std::cout << "Springs " << this->_springs;
      for(int i=0; i<this->_springs.size(); ++i)
      {
         if(this->_springs[i] == '?')
         {
            this->_unknownStatePositions.push_back(i);
         }
      }
      std::cout << " Unknown " << this->_unknownStatePositions.size() << " Groups ";

      std::string::size_type parsePos = posGroups;
      do
      {
         std::string::size_type parseEndPos = rawRecord.find(",", parsePos);
         std::string valueString = rawRecord.substr(parsePos, parseEndPos);
         int value = atoi(valueString.c_str());
         this->_damagedSpringsGroupsSizes.push_back(value);
         std::cout << value << "-";
         parsePos = parseEndPos;
         if(parsePos != std::string::npos)
         {
            parsePos++;
         }
      }
      while(parsePos != std::string::npos);
      std::cout << std::endl;
   }

   ulong getKnownSpringsBits()
   {
      ulong bits = 0;
      for(int i=0; i<this->_springs.size(); ++i)
      {
         if(this->_springs[i] == '#')
         {
            bits = bits | (1 << i);
         }
      }
      return bits;
   }

   virtual long getPossibleArrangementsCount()
   {
      long possibleArrangements = 0;

      // use brute forze, try every value possible for ?s and verify if it matches the criteria
      int unknownPositionsCount = this->_unknownStatePositions.size();
      long nPossibleArrangements = pow(2, unknownPositionsCount);
      for(int i=0; i<nPossibleArrangements; ++i)
      {
         // generate possible arrangement string
//         std::string possibleArrangement = this->_springs;
         ulong possibleArrangementBits = this->getKnownSpringsBits();
         for(int bit=0; bit<unknownPositionsCount; ++bit)
         {
            bool damaged = (i >> bit) & 1;
            int pos = this->_unknownStatePositions[bit];
//            possibleArrangement[pos] = (damaged ? '#' : '.');
            possibleArrangementBits = possibleArrangementBits | (damaged ? (1<<pos) : (0<<pos));
         }
         std::cout << "Possible arrangement " << " " << possibleArrangementBits;

         // obtain damaged groups from the possible arrangement
         std::vector<int> currentArrangementGroups;
         bool started = false;
         int groupSize = 0;
         for(int j=0; j<this->_springs.size(); ++j)
         {
            bool damaged = (possibleArrangementBits >> j) & 1;
            if( damaged )
            {
               groupSize++;
               if( !started )
               {
                  started = true;
               }
            }
            else if( started )
            {
               currentArrangementGroups.push_back(groupSize);
               started = false;
               groupSize = 0;
            }
         }
         if( started )
         {
            currentArrangementGroups.push_back(groupSize);
         }

         bool isValid = (currentArrangementGroups.size() == this->_damagedSpringsGroupsSizes.size());
         if( isValid )
         {
            for(int j=0; j<this->_damagedSpringsGroupsSizes.size(); ++j)
            {
               if( currentArrangementGroups[j] != this->_damagedSpringsGroupsSizes[j] )
               {
                  isValid = false;
                  break;
               }
            }
         }

         if( isValid )
         {
            possibleArrangements++;
         }
         std::cout << " " << isValid << std::endl;

//         std::vector<int> currentArrangementGroups;
//         bool started = false;
//         bool groupSize = 0;
//         for(int bit=0; bit<unknownPositionsCount; ++bit)
//         {
//            bool damaged = (i >> bit) & 1;
//            if( damaged )
//            {
//               groupSize++;
//               if( !started )
//               {
//                  started = true;
//               }
//            }
//            else if( started )
//            {
//               currentArrangementGroups.push_back(groupSize);
//               started = false;
//               groupSize = 0;
//            }
//         }
      }

      return possibleArrangements;
   }
};

class ConditionRecordUnfolded : public ConditionRecord
{
public:
   ConditionRecordUnfolded() : ConditionRecord() {}
   ~ConditionRecordUnfolded() {}

   void initialize(std::string rawRecord)
   {
      std::string::size_type posGroups = rawRecord.find_first_of("123456789");
      std::string springsFolded = rawRecord.substr(0, posGroups-1);
      this->_springs = springsFolded;
      for(int i=0; i<4; ++i)
      {
         this->_springs.append("?").append(springsFolded);
      }
      std::cout << "Springs " << this->_springs;
      for(int i=0; i<this->_springs.size(); ++i)
      {
         if(this->_springs[i] == '?')
         {
            this->_unknownStatePositions.push_back(i);
         }
      }
      std::cout << " Unknown " << this->_unknownStatePositions.size() << " Groups ";

      std::string::size_type parsePos = posGroups;
      do
      {
         std::string::size_type parseEndPos = rawRecord.find(",", parsePos);
         std::string valueString = rawRecord.substr(parsePos, parseEndPos);
         int value = atoi(valueString.c_str());
         this->_damagedSpringsGroupsSizes.push_back(value);
         parsePos = parseEndPos;
         if(parsePos != std::string::npos)
         {
            parsePos++;
         }
      }
      while(parsePos != std::string::npos);

      std::vector<int> groupsFolded = this->_damagedSpringsGroupsSizes;
      for(int i=0; i<4; ++i)
      {
         this->_damagedSpringsGroupsSizes.insert(this->_damagedSpringsGroupsSizes.end(), groupsFolded.begin(), groupsFolded.end());
      }
      for(int i=0; i<this->_damagedSpringsGroupsSizes.size(); ++i)
      {
         std::cout << this->_damagedSpringsGroupsSizes[i] << "-";
      }
      std::cout << std::endl;
   }

   virtual long getPossibleArrangementsCount()
   {
      long possibleArrangements = 0;

      // use elegant forze
      std::string poss;
      int cursor = 0;
      bool allPlaced = false;
      for(int i=0; i<this->_damagedSpringsGroupsSizes.size(); ++i)
      {
         int groupSize = this->_damagedSpringsGroupsSizes[i];
         bool placed = false;
         do
         {
            for(int j=0; j<groupSize; ++j)
            {
               if( this->_springs[cursor+j] == '.' )
               {
                  break;
               }
            }
         }
         while(!placed && cursor < (this->_springs.size()-groupSize));
      }


      // use brute forze, try every value possible for ?s and verify if it matches the criteria
      int unknownPositionsCount = this->_unknownStatePositions.size();
      long nPossibleArrangements = pow(2, unknownPositionsCount);
      for(int i=0; i<nPossibleArrangements; ++i)
      {
         // generate possible arrangement string
//         std::string possibleArrangement = this->_springs;
         ulong possibleArrangementBits = this->getKnownSpringsBits();
         for(int bit=0; bit<unknownPositionsCount; ++bit)
         {
            bool damaged = (i >> bit) & 1;
            int pos = this->_unknownStatePositions[bit];
//            possibleArrangement[pos] = (damaged ? '#' : '.');
            possibleArrangementBits = possibleArrangementBits | (damaged ? (1<<pos) : (0<<pos));
         }
         std::cout << "Possible arrangement " << " " << possibleArrangementBits;

         // obtain damaged groups from the possible arrangement
         std::vector<int> currentArrangementGroups;
         bool started = false;
         int groupSize = 0;
         for(int j=0; j<this->_springs.size(); ++j)
         {
            bool damaged = (possibleArrangementBits >> j) & 1;
            if( damaged )
            {
               groupSize++;
               if( !started )
               {
                  started = true;
               }
            }
            else if( started )
            {
               currentArrangementGroups.push_back(groupSize);
               started = false;
               groupSize = 0;
            }
         }
         if( started )
         {
            currentArrangementGroups.push_back(groupSize);
         }

         bool isValid = (currentArrangementGroups.size() == this->_damagedSpringsGroupsSizes.size());
         if( isValid )
         {
            for(int j=0; j<this->_damagedSpringsGroupsSizes.size(); ++j)
            {
               if( currentArrangementGroups[j] != this->_damagedSpringsGroupsSizes[j] )
               {
                  isValid = false;
                  break;
               }
            }
         }

         if( isValid )
         {
            possibleArrangements++;
         }
         std::cout << " " << isValid << std::endl;
      }

      return possibleArrangements;
   }
};

class Helper
{
private:
   long _firstPuzzleAnswer = 0;
   long _secondPuzzleAnswer = 0;
   std::vector<std::string> _fileInput;

public:
   Helper() {}
   ~Helper() {}

   void calculateFirstPuzzleAnswer()
   {
      this->_firstPuzzleAnswer = 0;

      for(int i; i<this->_fileInput.size(); ++i)
      {
         ConditionRecord record;
         record.initialize(this->_fileInput[i]);

         this->_firstPuzzleAnswer += record.getPossibleArrangementsCount();
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

   long getFirstPuzzleAnswer() { return _firstPuzzleAnswer; }
   long getSecondPuzzleAnswer() { return _secondPuzzleAnswer; }
};

int main()
{
   std::cout << "Advent of Code 2023 - Day 12" << std::endl;

   const std::string inputFileName = "testinput";
   Helper helper;
   helper.calculateAnswers(inputFileName);

   long answer = helper.getFirstPuzzleAnswer();
   std::cout << "First half answer: " << answer << std::endl;

   answer = helper.getSecondPuzzleAnswer();
   std::cout << "Second half answer: " << answer << std::endl;
}