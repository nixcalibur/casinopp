#ifndef UNTITLED1_SOURCE_H
#define UNTITLED1_SOURCE_H
#include <string>
#include <vector>
#include <fstream>
using namespace std;

extern string user_name;
extern int current_balance;
extern int bettingAmount;

enum Suit { HEARTS, DIAMONDS, CLUBS, SPADES };
enum Rank { TWO = 2, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK = 10, QUEEN = 10, KING = 10, ACE = 11 };

class Logger {
private:
    ofstream logFile;
public:
    Logger(const string& filename);
    ~Logger();
    void log(const string& message);
};

class Casino {
protected:
    Logger logger;
public:
    Casino();
    int gamemenu();
    void getStartingBalance();
    void diceRoll();
    bool continuePlaying();
    void numberGuessing();
    void blackjack();
    double depositMoney();
};

class Card {
public:
    string rank;
    string suit;
    Card(string r, string s) : rank(r), suit(s) {}
};

class Deck : public Casino {
private:
    vector<Card> cards;
public:
    Deck();
    void shuffleDeck();
    Card dealCard();
    int calculateHandValue(vector<Card>& hand);
};


#endif //UNTITLED1_SOURCE_H
