#include <iostream>
#include <stdlib.h>
#include <string>
#include <thread>
#include "source.h"
using namespace std;

string user_name;
int current_balance;
int bettingAmount;

int main() {
    srand(time(NULL));

    cout << "\n~~~ Welcome to Casino++ ~~~" << endl;

    cout << "Please enter your name: ";
    getline(cin, user_name);

    Casino casino;
    casino.getStartingBalance();

    int userMenuChoice;
    bool userPlayAgain = true;
    do {
        userMenuChoice = casino.gamemenu();
        switch (userMenuChoice) {
            case 1:
                casino.numberGuessing();
                userPlayAgain = casino.continuePlaying();
                break;
            case 2:
                casino.diceRoll();
                userPlayAgain = casino.continuePlaying();
                break;
            case 3:
                casino.blackjack();
                userPlayAgain = casino.continuePlaying();
                break;
            case 4:
                current_balance += casino.depositMoney();
                break;
            case 5:
                userPlayAgain = false;
                break;
        }
    } while (userPlayAgain);


    cout << "\n\n\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";
    cout << "\n\t\t\t\t Thank you for playing Casino++, " << user_name << endl;
    if (current_balance > 0) {
        cout << "\t\t\t\t\t Withdrawing $" << current_balance;
        for (int i = 0; i < 3; ++i) {
            cout << "." << flush;  // Print a dot and flush the output
            this_thread::sleep_for(chrono::milliseconds(500));  // Pause for the specified duration
        }
    }
    cout << "\n\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";
    Logger logger("/Users/sduphy/desktop/casino_log.txt");
    logger.log(user_name + " goes home with $" + to_string(current_balance));
    logger.log("Casino Session Ended.\n");
}