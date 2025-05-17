#include "source.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <thread>
#include <cctype>
#include <limits>
#include <stdexcept>
#include <random>
#include <vector>
using namespace std;


Logger::Logger(const string& filename) {
    logFile.open(filename, ios_base::app);
    if (!logFile.is_open()) {
        cerr << "Failed to open log file." << endl;
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::log(const string& message) {
    if (logFile.is_open()) {
        logFile << message << endl;
    }
}

Casino::Casino() : logger("/Users/sduphy/desktop/casino_log.txt") {
    logger.log("\nCasino session started.");
}

int Casino::gamemenu() {
    system("clear");
    int menuChoice;
    while (true) {
        try {
            cout << "\n\n\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";
            cout << "\n\t\t\t\t Welcome, " << user_name << " to Casino++ !" << endl;
            cout << "\n\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";

            cout << "Current wallet balance: $" << current_balance << endl;
            cout << "\nChoose game to play" << endl;
            cout << "1. Number Guessing (10x)" << endl;
            cout << "2. Dice Roll (2x)" << endl;
            cout << "3. Black Jack (3x)" << endl;
            cout << "4. Deposit Money" << endl;
            cout << "5. Exit" << endl;
            cout << "Your choice : ";
            string input;
            cin >> input;

            size_t pos;
            menuChoice = stoi(input, &pos);

            if (pos != input.length() || menuChoice < 1 || menuChoice > 6) {
                throw invalid_argument("Invalid choice. Please enter a number between 1 and 6.");
            }
            logger.log("User " + user_name + " selected menu option: " + to_string(menuChoice));

            break;

        } catch (const invalid_argument& e) {
            cout << e.what() << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return menuChoice;
}

void Casino::getStartingBalance() {
    while (true) {
        try {
            cout << "First deposit your starting money: $";
            string input;
            cin >> input;

            size_t pos;
            double amount = stod(input, &pos);

            if (pos != input.length() || amount <= 0) {
                throw invalid_argument("Invalid input. Please enter a positive number.");
            }

            current_balance = amount;
            logger.log("User " + user_name + " deposited $" + to_string(amount) + " as starting balance.");
            break;

        } catch (const invalid_argument& e) {
            cout << e.what() << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

bool Casino::continuePlaying() {
    char choice = 'y';
    while (true) {
        try {
            cout << "\n Do you want to continue? (Y/N): ";
            cin >> choice;
            choice = tolower(choice);

            if (choice == 'y' || choice == 'n') {
                logger.log("User " + user_name + (choice == 'y' ? " continues playing." : " stops playing."));
                break; // valid input, exit loop
            } else {
                throw invalid_argument("Invalid input. Please enter 'Y' or 'N'.");
            }
        } catch (const invalid_argument& e) {
            cout << e.what() << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return choice == 'y';
}

void Casino::diceRoll() {
    cout << "\n\n\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";
    cout << "Welcome to Dice Roll, " << user_name << endl;
    cout << "Rules: Roll dice higher than Computer. (2x Winning Prize)" << endl;

    //betting cash
    cout << "\nEnter your betting amount: $";
    cin >> bettingAmount;
    if (bettingAmount > current_balance) {
        cout << "Insufficient balance." << endl;
        return;
    }
    cout << "$" << bettingAmount << " is on the line.\n" << endl;
    current_balance -= bettingAmount;
    logger.log("User " + user_name + " bet $" + to_string(bettingAmount) + " on Dice Roll.");

    sleep(1);
    srand(time(NULL));

    int user_dice1 = (rand() % 6) + 1;
    int user_dice2 = (rand() % 6) + 1;
    int computer_dice1 = (rand() % 6) + 1;
    int computer_dice2 = (rand() % 6) + 1;

    this_thread::sleep_for(chrono::milliseconds(100));
    cout << "Computer rolling the First dice";
    for (int i = 0; i < 3; ++i) {
        cout << ". " << flush;  // Print a dot and flush the output
        std::this_thread::sleep_for(chrono::seconds(1));  // Pause for the specified duration
    } cout << "Landed on: " << computer_dice1 << endl;

    this_thread::sleep_for(chrono::milliseconds(100));
    cout << "Computer rolling the Second dice";
    for (int i = 0; i < 3; ++i) {
        cout << ". " << flush;  // Print a dot and flush the output
        this_thread::sleep_for(chrono::seconds(1));  // Pause for the specified duration
    } cout << "Landed on: " << computer_dice2 << endl;

    char rollDice;
    do {
        cout << "\n Please press r/R to start rolling the dice: ";
        cin >> rollDice;
        tolower(rollDice);
        if (rollDice != 'r') cout << "Invalid input." << endl;
    } while (rollDice != 'r' && rollDice != 'R');

    this_thread::sleep_for(chrono::milliseconds(100));
    cout << user_name << " is rolling the First dice";
    for (int i = 0; i < 3; ++i) {
        cout << ". " << flush;  // Print a dot and flush the output
        std::this_thread::sleep_for(chrono::seconds(1));  // Pause for the specified duration
    } cout << "Landed on: " << user_dice1 << endl;

    this_thread::sleep_for(chrono::milliseconds(100));
    cout << user_name << " is rolling the Second dice";
    for (int i = 0; i < 3; ++i) {
        cout << ". " << flush;  // Print a dot and flush the output
        this_thread::sleep_for(chrono::seconds(1));  // Pause for the specified duration
    } cout << "Landed on: " << user_dice2 << endl;

    int user_dice_sum = user_dice1 + user_dice2;
    int computer_dice_sum = computer_dice1 + computer_dice2;

    if (computer_dice_sum == user_dice_sum) {
        cout << "Haha, sorry, no one won as both players' dice rolled the same number." << endl;
        logger.log("Dice Roll: Tie. Both rolled " + to_string(user_dice_sum));
    } else if (computer_dice_sum > user_dice_sum) {
        cout << "\nComputer got the higher number on their dice: " << computer_dice_sum << endl;
        cout << "Whereas you got: " << user_dice_sum << endl;
        cout << "Better luck next time." << endl;
        cout << "Sorry you've lost " << bettingAmount << "$" << endl;
        logger.log("Dice Roll: User lost. Computer rolled " + to_string(computer_dice_sum) + ", User rolled " + to_string(user_dice_sum));
    } else {
        cout << endl << user_name << " got the higher number on their dice: " << user_dice_sum << endl;
        cout << "Whereas Computer got: " << computer_dice_sum << endl;
        cout << "Congratulations, " << user_name << " You have won $" << bettingAmount * 2 << endl;
        current_balance += bettingAmount * 2;
        cout << " Your new balance is $" << current_balance << endl;
        logger.log("Dice Roll: User won. User rolled " + to_string(user_dice_sum) + ", Computer rolled " + to_string(computer_dice_sum) + ". Winnings: $" + to_string(bettingAmount * 2));
    }

    logger.log(user_name + " New Balance is $" + to_string(current_balance));
    cout << "\nThank you for playing Dice Roll." << endl;
}

void Casino::numberGuessing() {
    cout << "\n\n\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";
    cout << "Welcome to Number Guessing, " << user_name << endl;
    cout << "Rules: Guess number 1 to 10 that computer has guessed. (10x Winning Prize)" << endl;

    //betting cash
    cout << "\nEnter your betting amount: $";
    cin >> bettingAmount;
    if (bettingAmount > current_balance) {
        cout << "Insufficient balance." << endl;
        return;
    }
    cout << "$" << bettingAmount << " is on the line." << endl;
    current_balance -= bettingAmount;
    logger.log("User " + user_name + " bet $" + to_string(bettingAmount) + " on Number Guessing.");

    srand(time(NULL));

    int ComputerRandomNumber = (rand() % 10) + 1;
    int userGuess;

    cout << "Good Luck ~" << endl;
    sleep(1);
    cout << "\nComputer has generated a number between (1 - 10) ~" << endl;

    while(true) {
        try {
            cout << "Enter your guess : ";
            string input;
            cin >> input;

            size_t pos;
            userGuess = stoi(input, &pos);

            if (pos != input.length() || userGuess < 1 || userGuess > 10) {
                throw invalid_argument("Invalid choice. Please enter a number between 1 and 10.");
            }

            break;
        } catch (const invalid_argument &e) {
            cout << e.what() << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    cout << "\n";
    // Checking user Guess
    if (userGuess == ComputerRandomNumber) {
        cout << "Congratulations Your guess is correct." << endl;
        cout << "Computer Generated Number was " << ComputerRandomNumber << endl;
        cout << "You have won x10 money " << bettingAmount * 10 << " $ " << endl;
        current_balance = current_balance + (bettingAmount * 10);
        cout << "Your new balance is " << current_balance << "$" << endl;
        logger.log("Number Guessing: User WON. Computer Generated Number: " + to_string(ComputerRandomNumber));

    } else {
        cout << "Computer Generated Number was " << ComputerRandomNumber << endl;
        cout << "Whereas your guess was " << userGuess << endl;
        cout << "Sorry you've lost " << bettingAmount << "$" << endl;
        logger.log("Number Guessing: User LOST. Computer Generated Number: " + to_string(ComputerRandomNumber));
    }

    cout << "\nThank you for playing Guessing the Number." << endl;
    logger.log(user_name + " New Balance is $" + to_string(current_balance));
}

void Casino::blackjack() {
    cout << "\n\n\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";
    cout << "Welcome to BlackJack, " << user_name << endl;
    cout << "Rules: Have a higher hand value than the dealer, without going over 21. (3x Winning Prize)" << endl;
    //betting cash
    cout << "\nEnter your betting amount: $";
    cin >> bettingAmount;
    if (bettingAmount > current_balance) {
        cout << "Insufficient balance." << endl;
        return;
    }
    cout << "$" << bettingAmount << " is on the line.\n" << endl;
    current_balance -= bettingAmount;
    logger.log("User " + user_name + " bet $" + to_string(bettingAmount) + " on BlackJack.");

    Deck deck;
    deck.shuffleDeck();

    vector<Card> playerHand;
    vector<Card> dealerHand;

    // Initial deal
    playerHand.push_back(deck.dealCard());
    playerHand.push_back(deck.dealCard());
    dealerHand.push_back(deck.dealCard());
    dealerHand.push_back(deck.dealCard());

    bool playerBust = false;
    bool dealerBust = false;

    // Player's turn
    while (true) {
        cout << "\nYour hand: ";
        for (Card card : playerHand) {
            cout << card.rank << " of " << card.suit << ", ";
        }
        cout << "\nYour total: " << deck.calculateHandValue(playerHand) << endl;

        if (deck.calculateHandValue(playerHand) > 21) {
            playerBust = true;
            break;
        }

        char choice;
        cout << "Do you want to hit or stand? (h/s): ";
        cin >> choice;

        if (choice == 'h') {
            playerHand.push_back(deck.dealCard());
        } else {
            break;
        }
    }

    // Dealer's turn
    if (!playerBust) {
        while (deck.calculateHandValue(dealerHand) < 17) {
            dealerHand.push_back(deck.dealCard());
        }

        cout << "\nDealer's hand: ";
        for (Card card : dealerHand) {
            cout << card.rank << " of " << card.suit << ", ";
        }
        cout << "\nDealer's total: " << deck.calculateHandValue(dealerHand) << endl;

        if (deck.calculateHandValue(dealerHand) > 21) {
            dealerBust = true;
        }
    }

    // Determine the outcome
    if (playerBust) {
        cout << "\nYou busted! Dealer wins." << endl;
        logger.log("BLACKJACK: User LOST $" + to_string(bettingAmount) + " Computer's hand: " + to_string(deck.calculateHandValue(dealerHand)) + ", Whereas User's hand: " + to_string(deck.calculateHandValue(playerHand)));
    } else if (dealerBust) {
        cout << "\nDealer busted! You win! You have won $" << bettingAmount * 3 << endl;
        current_balance += bettingAmount * 3;
        logger.log("BLACKJACK: User WON $" + to_string(bettingAmount * 3) + " User's hand: " + to_string(deck.calculateHandValue(playerHand)) + ", Whereas Computer's hand: " + to_string(deck.calculateHandValue(dealerHand)));
    } else {
        int playerTotal = deck.calculateHandValue(playerHand);
        int dealerTotal = deck.calculateHandValue(dealerHand);

        if (playerTotal > dealerTotal) {
            cout << "\nYou win! You have won $" << bettingAmount * 3 << endl;
            current_balance += bettingAmount * 3;
            logger.log("BLACKJACK: User WON $" + to_string(bettingAmount * 3) + " User's hand: " + to_string(deck.calculateHandValue(playerHand)) + ", Whereas Computer's hand: " + to_string(deck.calculateHandValue(dealerHand)));
        } else if (playerTotal < dealerTotal) {
            cout << "\nDealer wins." << endl;
            logger.log("BLACKJACK: User LOST $" + to_string(bettingAmount) + " Computer's hand: " + to_string(deck.calculateHandValue(dealerHand)) + ", Whereas User's hand: " + to_string(deck.calculateHandValue(playerHand)));
        } else {
            cout << "\nIt's a tie!" << endl;
            current_balance += bettingAmount;
        }
    }
    cout << "\nYour new balance is $" << current_balance << endl;
    logger.log(user_name + " New Balance is $" + to_string(current_balance));
    cout << "Thank you for playing BlackJack." << endl;
}

Deck::Deck() {
    vector<string> suits = {"Hearts", "Diamonds", "Clubs", "Spades"};
    vector<string> ranks = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

    for (const string& suit : suits) {
        for (const string& rank : ranks) {
            cards.emplace_back(rank, suit);
        }
    }
}

void Deck::shuffleDeck() {
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle(cards.begin(), cards.end(), default_random_engine(seed));
}

Card Deck::dealCard() {
    Card dealtCard = cards.back();
    cards.pop_back();
    return dealtCard;
}

int Deck::calculateHandValue(vector<Card>& hand) {
    int value = 0;
    int aces = 0;

    for (Card& card : hand) {
        if (card.rank == "J" || card.rank == "Q" || card.rank == "K") {
            value += 10;
        } else if (card.rank == "A") {
            value += 11;
            aces++;
        } else {
            value += stoi(card.rank);
        }
    }

    while (value > 21 && aces > 0) {
        value -= 10;
        aces--;
    }

    return value;
}

double Casino::depositMoney() {
    cout << "\n\n\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";
    cout << "Daring aren't we today? " << user_name << endl;
    double amount;
    while (true) {
        try {
            cout << " \nEnter the amount you'd like to deposit: $";
            string input;
            cin >> input;

            size_t pos;
            amount = stod(input, &pos);

            if (pos != input.length() || amount <= 0) {
                throw invalid_argument("Invalid input. Please enter a positive number.");
            }
            logger.log(user_name + " deposited $" + to_string(amount));

            break;

        } catch (const invalid_argument& e) {
            cout << e.what() << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    cout << "\nDeposit Successful!" << endl;
    return amount;
}