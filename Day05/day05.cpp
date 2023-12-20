#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <cmath>

struct MapRange
{
   ulong Source = 0;
   ulong Destination = 0;
   ulong Range = 0;

   MapRange(ulong source, ulong destination, ulong range): Source(source), Destination(destination), Range(range)
   { }

   bool isInRange(ulong source)
   {
      ulong maxSource = this->Source+this->Range;
      bool result = (source >= this->Source) && (source <= maxSource);
      return result;
   }

   ulong getDestination(ulong source)
   {
      // look if number is in range
      int delta = source - this->Source;
//      std::cout << " >> " << source << " - " << Destination << " " << Source << " " << Range << " : " << this->Destination + delta << std::endl;
      if( delta >= 0 && delta <= this->Range )
      {
         return (this->Destination + delta);
      }

      // if not in range, destination is the same as source
      return source;
   }
};

class Map
{
private:
   std::vector<MapRange> _mapRanges;

public:
   Map() { }
   ~Map() { }

   void addRange(ulong source, ulong destination, ulong range)
   {
      MapRange mapRange = MapRange(source, destination, range);
      _mapRanges.push_back(mapRange);
   }

   ulong getDestination(ulong source)
   {
      for(int i=0; i<_mapRanges.size(); ++i)
      {
         if( _mapRanges[i].isInRange(source) )
         {
            return _mapRanges[i].getDestination(source);
         }
      }
      return source;
   }

   void printMapRanges()
   {
      for(int i=0; i<this->_mapRanges.size(); ++i)
      {
         std::cout << " >> " << _mapRanges[i].Destination << " " << _mapRanges[i].Source << " " << _mapRanges[i].Range << std::endl;
      }
   }
};

struct SeedsRange
{
   ulong Start = 0;
   ulong Range = 0;

   SeedsRange(ulong start, ulong range=1) : Start(start), Range(range)
   { }
};

class Almanac
{
private:
   std::vector<SeedsRange> _seeds;
   std::vector<Map> _maps;

public:
   Almanac() { }
   ~Almanac() { }

   void addSeed(ulong seed, ulong range=1)
   {
      this->_seeds.push_back(SeedsRange(seed, range));
   }

   void addMap(Map map)
   {
      this->_maps.push_back(map);
   }

   ulong getSeedLocation(ulong seed)
   {
      ulong destination = seed;
      for(int i=0; i<this->_maps.size(); ++i)
      {
//         std::cout << " >> " << destination;
         destination = this->_maps[i].getDestination(destination);
      }
//      std::cout << " >> " << destination << std::endl;
//      std::cout << "- " << seed << " " << destination << std::endl;
      return destination;
   }

   ulong getLowestLocation()
   {
      ulong lowestSeed = this->_seeds[0].Start;
      ulong lowestLocation = this->getSeedLocation(lowestSeed);
      for(int i=0; i<this->_seeds.size(); ++i)
      {
         SeedsRange seedsRange = this->_seeds[i];
         for(ulong j=0; j<seedsRange.Range; ++j)
         {
            ulong seed = seedsRange.Start + j;
            ulong location = this->getSeedLocation(seed);
            if( location < lowestLocation )
            {
               lowestLocation = location;
               lowestSeed = seed;
            }
         }
      }
      return lowestLocation;
   }

   void printMaps()
   {
      for(int i=0; i<this->_maps.size(); ++i)
      {
         std::cout << " - " << i << std::endl;
         this->_maps[i].printMapRanges();
      }
   }
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

      Almanac almanac;
      // obtain seeds
      char* end = nullptr;
      std::string line = _fileInput[0];
      std::string::size_type parsePos = line.find(":")+2;
      do
      {
         std::string::size_type parseEndPos = line.find(" ", parsePos);
         std::string seedString = line.substr(parsePos, parseEndPos);
         almanac.addSeed(strtoul(seedString.c_str(),&end,10));
         parsePos = parseEndPos;
         if(parsePos != std::string::npos)
         {
            parsePos++;
         }
      }
      while(parsePos != std::string::npos);
      
      // obtain mapranges
      Map newMap;
      for(int i=3; i<_fileInput.size(); ++i)
      {
         std::string line = _fileInput[i];
         int lineLength = line.length();
         if(lineLength > 0)
         {
            if( line[0] >= '0' && line[0] <= '9' )
            {
               // parse maps
               std::string::size_type parseEndPos = line.find(" ");
               std::string auxString = line.substr(0, parseEndPos);

               ulong destination = strtoul(auxString.c_str(), &end, 10);
               parsePos = parseEndPos+1;

               parseEndPos = line.find(" ", parsePos);
               auxString = line.substr(parsePos, parseEndPos);
               ulong source = strtoul(auxString.c_str(), &end, 10);
               parsePos = parseEndPos+1;

               auxString = line.substr(parsePos);
               ulong range = strtoul(auxString.c_str(), &end, 10);

               newMap.addRange(source, destination, range);
//               std::cout << "- " << destination << " " << source << " " << range << std::endl;
            }
         }
         else
         {
            almanac.addMap(newMap);
            newMap = Map();
         }
      }
      almanac.addMap(newMap);

      this->_firstPuzzleAnswer = almanac.getLowestLocation();
   }

   void calculateSecondPuzzleAnswer()
   {
      this->_secondPuzzleAnswer = 0;

      Almanac almanac;
      // obtain seeds
      char* end = nullptr;
      std::string line = _fileInput[0];
      std::string::size_type parsePos = line.find(":")+2;
      do
      {
         std::string::size_type parseEndPos = line.find(" ", parsePos);
         std::string seedString = line.substr(parsePos, parseEndPos);
         ulong seedStart = strtoul(seedString.c_str(),&end,10);

         parsePos = parseEndPos+1;
         parseEndPos = line.find(" ", parsePos);
         std::string rangeString = line.substr(parsePos, parseEndPos);
         ulong seedsRange = strtoul(rangeString.c_str(),&end,10);

         almanac.addSeed(seedStart, seedsRange);

         parsePos = parseEndPos;
         if(parsePos != std::string::npos)
         {
            parsePos++;
         }
      }
      while(parsePos != std::string::npos);
      
      // obtain mapranges
      Map newMap;
      for(int i=3; i<_fileInput.size(); ++i)
      {
         std::string line = _fileInput[i];
         int lineLength = line.length();
         if(lineLength > 0)
         {
            if( line[0] >= '0' && line[0] <= '9' )
            {
               // parse maps
               std::string::size_type parseEndPos = line.find(" ");
               std::string auxString = line.substr(0, parseEndPos);

               ulong destination = strtoul(auxString.c_str(), &end, 10);
               parsePos = parseEndPos+1;

               parseEndPos = line.find(" ", parsePos);
               auxString = line.substr(parsePos, parseEndPos);
               ulong source = strtoul(auxString.c_str(), &end, 10);
               parsePos = parseEndPos+1;

               auxString = line.substr(parsePos);
               ulong range = strtoul(auxString.c_str(), &end, 10);

               newMap.addRange(source, destination, range);
//               std::cout << "- " << destination << " " << source << " " << range << std::endl;
            }
         }
         else
         {
            almanac.addMap(newMap);
            newMap = Map();
         }
      }
      almanac.addMap(newMap);

      this->_secondPuzzleAnswer = almanac.getLowestLocation();
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
   std::cout << "Advent of Code 2023 - Day 04" << std::endl;

   const std::string inputFileName = "input";
   Helper helper;
   helper.calculateAnswers(inputFileName);

   ulong answer = helper.getFirstPuzzleAnswer();
   std::cout << "First half answer: " << answer << std::endl;

   answer = helper.getSecondPuzzleAnswer();
   std::cout << "Second half answer: " << answer << std::endl;
}