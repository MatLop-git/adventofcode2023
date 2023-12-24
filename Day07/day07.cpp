#include <algorithm>    // std::sort
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

enum HandType {
   AllDifferent = 0,
   OnePair = 1,
   TwoPair = 2,
   ThreeOfAKind = 3,
   FullHouse = 4,
   FourOfAKind = 5,
   FiveOfAKind = 6
};

struct HandBidPair
{
   std::string Hand = "";
   int Bid = 0;
   HandType Type = AllDifferent;

   HandBidPair() {}
   HandBidPair(std::string hand, int bid) : Hand(hand), Bid(bid)
   {
      this->obtainHandType();
   }
   ~HandBidPair() {}

   void obtainHandType()
   {
      this->Type = HandType::AllDifferent;

      std::string handAux = this->Hand;
      int cardIndex = handAux.size()-1;
      do
      {
         int matches = 0;
         char card = handAux[cardIndex];
         for(int j=cardIndex-1; j>=0; --j)
         {
            if(card == handAux[j])
            {
               matches++;
               handAux.erase(j, 1);
               cardIndex--;
            }
         }

         if( matches == 4 )
         {
            this->Type = HandType::FiveOfAKind;
         }
         else if( matches == 3 )
         {
            this->Type = HandType::FourOfAKind;
         }
         else if( matches == 2 )
         {
            if( this->Type == HandType::OnePair )
            {
               this->Type = HandType::FullHouse;
            }
            else
            {
               this->Type = HandType::ThreeOfAKind;
            }
         }
         else if( matches == 1 )
         {
            if( this->Type == HandType::ThreeOfAKind )
            {
               this->Type = HandType::FullHouse;
            }
            else if( this->Type == HandType::OnePair )
            {
               this->Type = HandType::TwoPair;
            }
            else
            {
               this->Type = HandType::OnePair;
            }
         }

         cardIndex--;
      }
      while(cardIndex > 0);
      

/*      for(int i=handAux.size()-1; i>0; --i)
      {
         int matches = 0;
         char card = handAux[i];
         for(int j=i-1; j>=0; --j)
         {
            if(card == handAux[j])
            {
               matches++;
               handAux.erase(j, 1);
            }
         }

         if( matches == 4 )
         {
            this->Type = HandType::FiveOfAKind;
         }
         else if( matches == 3 )
         {
            this->Type = HandType::FourOfAKind;
         }
         else if( matches == 2 )
         {
            if( this->Type == HandType::OnePair )
            {
               this->Type = HandType::FullHouse;
            }
            else
            {
               this->Type = HandType::ThreeOfAKind;
            }
         }
         else if( matches == 1 )
         {
            if( this->Type == HandType::ThreeOfAKind )
            {
               this->Type = HandType::FullHouse;
            }
            else if( this->Type == HandType::OnePair )
            {
               this->Type = HandType::TwoPair;
            }
            else
            {
               this->Type = HandType::OnePair;
            }
         }
      }
*/   }
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

   static int getCardWeight(const char card)
   {
      switch( card )
      {
         case '2': return 2;
         case '3': return 3;
         case '4': return 4;
         case '5': return 5;
         case '6': return 6;
         case '7': return 7;
         case '8': return 8;
         case '9': return 9;
         case 'T': return 10;
         case 'J': return 11;
         case 'Q': return 12;
         case 'K': return 13;
         case 'A': return 14;
      }
      return 0;
   }

   static bool getLowerHand(HandBidPair a, HandBidPair b)
   {
      // compare hand types
      if( a.Type != b.Type )
      {
         return (a.Type < b.Type);
      }

      // if equal hand types, compare chars one by one
      for(int i=0; i<a.Hand.size(); ++i)
      {
         char cardA = a.Hand[i];
         char cardB = b.Hand[i];
         if(cardA != cardB)
         {
            int weightA = getCardWeight(cardA);
            int weightB = getCardWeight(cardB);
            return (weightA < weightB);
         }
      }

      return false;
   }

   void calculateFirstPuzzleAnswer()
   {
      this->_firstPuzzleAnswer = 0;

      // obtain hands and bids
      std::vector<HandBidPair> handsBidPairs;
      for(int i=0; i<_fileInput.size(); ++i)
      {
         std::string line = _fileInput[i];
         std::string::size_type parsePos = line.find(" ");
         std::string hand = line.substr(0, parsePos);
         std::string bidString = line.substr(parsePos+1);
         int bid = atoi(bidString.c_str());
         handsBidPairs.push_back(HandBidPair(hand, bid));
      }

      // sort hands using sort method
      std::sort(handsBidPairs.begin(), handsBidPairs.end(), getLowerHand);

      for(int i=0; i<handsBidPairs.size(); ++i)
      {
         std::cout << handsBidPairs[i].Hand << " - " << handsBidPairs[i].Type << std::endl;
      }

      // calculate earnings
      for(int i=0; i<handsBidPairs.size(); ++i)
      {
         HandBidPair pair = handsBidPairs[i];
         int rank = i + 1;
         ulong winnings = rank * pair.Bid;
         std::cout << rank << " $ " << winnings << std::endl;
         this->_firstPuzzleAnswer += winnings;
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

   int getFirstPuzzleAnswer() { return _firstPuzzleAnswer; }
   int getSecondPuzzleAnswer() { return _secondPuzzleAnswer; }
};

int main()
{
   std::cout << "Advent of Code 2023 - Day 07" << std::endl;

   const std::string inputFileName = "input";
   Helper helper;
   helper.calculateAnswers(inputFileName);

   int answer = helper.getFirstPuzzleAnswer();
   std::cout << "First half answer: " << answer << std::endl;

   answer = helper.getSecondPuzzleAnswer();
   std::cout << "Second half answer: " << answer << std::endl;
}