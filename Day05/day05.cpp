#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <cmath>
#include <thread>
#include <unistd.h>

struct SeedsRange
{
   ulong Start = 0;
   ulong Range = 0;

   SeedsRange(ulong start, ulong range=1) : Start(start), Range(range)
   { }
};

struct MapRange
{
   ulong Source = 0;
   ulong Destination = 0;
   ulong Range = 0;

   MapRange(ulong source, ulong destination, ulong range): Source(source), Destination(destination), Range(range)
   { }

   bool isInRange(ulong source)
   {
      ulong maxSource = this->Source+this->Range-1;
      bool result = (source >= this->Source) && (source <= maxSource);
      return result;
   }

   bool isDestinationInRange(ulong destination)
   {
      ulong maxDestination = this->Destination+this->Range-1;
      bool result = (destination >= this->Destination) && (destination <= maxDestination);
      return result;
   }

   ulong getSource(ulong destination)
   {
      // look if number is in range
      long delta = destination - this->Destination;
      if( delta >= 0 && delta <= this->Range )
      {
         return (this->Source + delta);
      }

      // if not in range, source is the same as destination
      return destination;
   }

   ulong getDestination(ulong source)
   {
      // look if number is in range
      long delta = source - this->Source;
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

   void fillEmptyRanges()
   {
      std::cout << "Filling empty ranges" << std::endl;
      // obtain the MapRange for the highest source
      MapRange maxMapRange = this->_mapRanges[0];
      for(int i=1;i<this->_mapRanges.size(); ++i)
      {
         if(this->_mapRanges[i].Source > maxMapRange.Source)
         {
            maxMapRange = this->_mapRanges[i];
         }
      }
//      std::cout << "Max MapRange: " << maxMapRange.Source << " - " << maxMapRange.Destination << " - " << maxMapRange.Range << std::endl;

      // search for sources outside of the MapRanges
      ulong source = 0;
      do
      {
         MapRange mapRange = this->getMapRangeForSource(source);
         if(mapRange.Source == mapRange.Destination)
         {
            MapRange mapRangeAux = mapRange;
            do
            {
               mapRangeAux = this->getMapRangeForSource(mapRangeAux.Source+1);
            }
            while(mapRangeAux.Source == mapRangeAux.Destination);
            mapRange.Range = mapRangeAux.Source - mapRange.Source;
            this->_mapRanges.push_back(mapRange);
            std::cout << "New MapRange: " << mapRange.Source << " - " << mapRange.Destination << " - " << mapRange.Range << std::endl;
         }
         source += mapRange.Range;
      }
      while(source < maxMapRange.Source);
   }

   MapRange getMapRangeForSource(ulong source)
   {
      for(int i=0; i<_mapRanges.size(); ++i)
      {
         if( _mapRanges[i].isInRange(source) )
         {
            return _mapRanges[i];
         }
      }
      return MapRange(source, source, 1);
   }

   MapRange getMapRangeForDestination(ulong destination)
   {
      for(int i=0; i<_mapRanges.size(); ++i)
      {
         if( _mapRanges[i].isDestinationInRange(destination) )
         {
            return _mapRanges[i];
         }
      }
      return MapRange(destination, destination, 1);
   }

   ulong getDestination(ulong source)
   {
      MapRange mapRange = this->getMapRangeForSource(source);
      return mapRange.getDestination(source);
   }

   void printMapRanges()
   {
      for(int i=0; i<this->_mapRanges.size(); ++i)
      {
         std::cout << " >> " << _mapRanges[i].Destination << " " << _mapRanges[i].Source << " " << _mapRanges[i].Range << std::endl;
      }
   }

   std::vector<MapRange> getKeySeeds(SeedsRange seedsRange)
   {
      std::vector<MapRange> keySeeds;
      bool lastSeed = false;
      SeedsRange seedsLeft = seedsRange;
//      std::cout << " - Seeds " << seedsLeft.Start << " " << seedsLeft.Range << std::endl;
      do
      {
         // find MapRange for source
         ulong seed = seedsLeft.Start;
         MapRange destinationMapRange = this->getMapRangeForSource(seed);
         // calculate how much range does the MapRange covers
         long delta = seed - destinationMapRange.Source;
         ulong destination = destinationMapRange.Destination + delta;
         ulong range = (destinationMapRange.Range - delta);
         lastSeed = (seedsLeft.Range <= range);
         if( lastSeed )
         {
            range = seedsLeft.Range;
         }
         // add new MapRange for the calculation
         MapRange newMapRange(seed, destination, range);
         keySeeds.push_back(newMapRange);
//            std::cout << " - [" << newMapRange.Source << ", " << newMapRange.Destination << "] [" << newMapRange.Source+newMapRange.Range << ", " << newMapRange.Destination+newMapRange.Range << "] - " << newMapRange.Range << std::endl;

         // verify if there's more seeds to cover
         if( !lastSeed )
         {
            seedsLeft.Start = (newMapRange.Source + newMapRange.Range);
            seedsLeft.Range = (seedsLeft.Range - newMapRange.Range);
//            std::cout << " - Seeds " << seedsLeft.Start << " " << seedsLeft.Range << std::endl;
         }
      }
      while(lastSeed == false);
      
      return keySeeds;
   }
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

   /** Gets the lowest location for the seeds.*/
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

   /** Given a range of seeds, returns the seeds that corresponds to different destination 'gaps'.
    *  It verifies the destinations until reaching the final map.
   */
   std::vector<ulong> getKeySeeds(SeedsRange seedsRange, int mapIndex)
   {
      std::vector<ulong> keySeeds;

      Map map = this->_maps[mapIndex];
      std::vector<MapRange> seedsDestinationsAux = map.getKeySeeds(seedsRange);
      
      mapIndex++;
      if( mapIndex < this->_maps.size() )
      {
         for(int i=0; i<seedsDestinationsAux.size(); ++i)
         {
            MapRange mapRangeAux = seedsDestinationsAux[i];
            SeedsRange seedsRangeAux(mapRangeAux.Destination, mapRangeAux.Range);
            std::vector<ulong> keySeedsAux = this->getKeySeeds(seedsRangeAux, mapIndex);
            for(int j=0; j<keySeedsAux.size(); ++j)
            {
               ulong destination = keySeedsAux[j];
               MapRange mapRangeAux2 = map.getMapRangeForDestination(destination);
               ulong source = mapRangeAux2.getSource(destination);
               keySeeds.push_back(source);
            }
         }
      }
      else
      {
         for(int j=0; j<seedsDestinationsAux.size(); ++j)
         {
            ulong source = seedsDestinationsAux[j].Source;
            keySeeds.push_back(source);
         }
      }

      return keySeeds;
   }

   /** Gets the lowest location for a wide range of seeds.
    *  It first obtains the key seeds to lower the amount of seeds to check.
   */
   ulong getLowestLocationWithKeySeeds()
   {
      ulong lowestLocation = 999999999;
      for(int i=0; i<this->_seeds.size(); ++i)
      {
         SeedsRange seedsRange = this->_seeds[i];
         std::vector<ulong> keySeeds = this->getKeySeeds(seedsRange, 0);
         for(int j=0; j<keySeeds.size(); ++j)
         {
            ulong seed = keySeeds[j];
            ulong location = this->getSeedLocation(seed);
            if( location < lowestLocation )
            {
               lowestLocation = location;
            }
         }
      }
      return lowestLocation;
   }

   ulong getKeySeeds()
   {
      for(int i=0; i<1; ++i)
      {
         SeedsRange seedsRange = this->_seeds[i];
         std::vector<ulong> seedsDestinations = this->getKeySeeds(seedsRange, 0);
//         for(int i=0; i<seedsDestinations.size(); ++i)
//         {
//            std::cout << " - [" << seedsDestinations[i].Source << ", " << seedsDestinations[i].Destination << "] [" << seedsDestinations[i].Source+seedsDestinations[i].Range << ", " << seedsDestinations[i].Destination+seedsDestinations[i].Range << "] - " << seedsDestinations[i].Range << std::endl;
//         }
      }
      return 0;
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
//            newMap.fillEmptyRanges();
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
            newMap.fillEmptyRanges();
            almanac.addMap(newMap);
            newMap = Map();
         }
      }
      newMap.fillEmptyRanges();
      almanac.addMap(newMap);

      this->_secondPuzzleAnswer = almanac.getLowestLocationWithKeySeeds();
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

void calculateAnswers(Helper* helper, const std::string inputFileName)
{
   helper->calculateAnswers(inputFileName);
}

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