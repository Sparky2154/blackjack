#include <iostream>
#include <string>
#include <cstdlib>
#include <random>
#include <zconf.h>

using namespace std;

/**
 @author Spencer Valentine
 @section DESCRIPTION
A small blackjack game I made for fun
 */

class Deck{
    int content[52] = {0}; //The content of the deck
    int index = -2; //The current position in the deck

    /**
     * part of the quicksort algorithm remade for shuffling
     * @param low is the index of the first number the section of content that is being shuffled
     * @param high is the index of the last number the section of content that is being shuffled
     * @return the index of the partition
     */
    int partition(int low, int high){
        int i = low-1;
        int hold;
        for (int j = low; j <= high-1; ++j) {
            if (random()%2 == 1){
                i++;
                hold = content[i];
                content[i] = content[j];
                content[j] = hold;
            }
        }
        hold = content[i + 1];
        content[i + 1] = content[high];
        content[high] = hold;
        return i + 1;
    }

    /**
     * The main part of the quicksort algorithm
     * @param low is the beginning of the section of content that needs to be shuffled
     * @param high is the end of the section of content that needs to be shuffled
     */
    void quickShuffle(int low, int high){
        int ind;
        if(low < high){
            ind = partition(low, high);
            quickShuffle(low, ind-1);
            quickShuffle(ind+1, high);
        }
    }

public:
    /**
     * The constructor that sets the beginning values of content and calls shuffle
     */
    Deck(){
        for (int i = 0; i < 52; ++i) {
            content[i] = i;
        }
        shuffle();
    }

    /**
     * Shuffles the deck using the quicksort algorithm
     */
    void shuffle(){
        quickShuffle(0, 51);
        index = -1;
    }

    /**
     * @return the ID of the next card in the list
     */
    int getCard(){
        index++;
        return content[index];
    }

    /**
     * @return the index of the previous card issued from the deck
     */
    int getIndex() const{
        return index;
    }
};


class Hand {
    int length = 0; //Number of cards in hand
    int content[11] = {0}; //Cards in hand; 11 is the maximum possible before bust

public:
    /**
     * @return Value of the hand
     */
    int checkValue() {
        int aces = 0;
        int value = 0;
        for (int i = 0; i < length; ++i) {
            int mod = ((content[i])%13)+2;
            if (mod<=10){
                value += mod;
            } else if(mod<14){
                value += 10;
            } else{
                value += 11;
                aces++;
            }
        }
        for (int i = 0; i < aces && value > 21; ++i) {
            value -= 10;
        }
        return value;
    }

    /**
     * Prints the label of the card that the number refers to
     * @param number ID of the card
     */
    static void printCard(int number){
        if(number < 11){
            std::cout << number;
        } else if(number < 2){
            std::cout << "Bad card number 1";
        } else {

            switch (number) {
                case 11:
                    std::cout << "J";
                    break;
                case 12:
                    std::cout << "Q";
                    break;
                case 13:
                    std::cout << "K";
                    break;
                case 14:
                    std::cout << "A";
                    break;
                default:
                    std::cout << "Bad card number 2";
            }
        }
    }

    /**
     * Prints the whole hand
     */
    void printHand(){
        for (int i = 0; i < length; ++i) {
            printCard((((content[i])%13)+2));
            std::cout << " ";
        }
        std::cout << '\n';
    }

    /**
     * Prints the first card in the dealers hand and an asterisk to represent the down-turned card
     */
    void printDealer(){
        std::cout << ((content[0])%13)+2 << " *" << '\n';
    }

    /**
     * Adds a specific card to this hand
     * @param card is the ID of the card
     */
    void addCardToHand(int card){
        content[length] = card;
        length++;
    }
    /**
     * Sets the length of this hand to zero to make it empty
     */
    void resetHand(){
        length = 0;
    }
};

void game(Deck* deck){
    string reply; //Used to hold user input

    //One hand each for the dealer and player
    Hand* dealerCards = (Hand*)malloc(sizeof(Hand));
    Hand* playerCards = (Hand*)malloc(sizeof(Hand));

    //While the deck is full enough to not be predictable
    while (deck->getIndex() < 26){

        //Prints "Dealer: " Followed by the first card and an asterisk to represent the downturned card
        std::cout << "Dealer: ";
        dealerCards->addCardToHand(deck->getCard());
        dealerCards->addCardToHand(deck->getCard());
        dealerCards->printDealer();

        //Prints "Player: " Followed by both of the players cards
        std::cout << "Player: ";
        playerCards->addCardToHand(deck->getCard());
        playerCards->addCardToHand(deck->getCard());
        playerCards->printHand();
        std::cout << "\n";

        //Check if the value of either hand is 21 (player gets priority)
        if(playerCards->checkValue() == 21){
            std::cout << "You got blackjack!";
            return;
        } else if (dealerCards->checkValue() == 21){
            std::cout << "Dealer has Blackjack";
            return;
        }

        //While the player has not lost, ask if they want another card
        while (playerCards->checkValue() < 21) {
            reply = "";
            std::cout << "Hit or stay? (h/s)\n";
            cin >> reply;
            if(reply[0] == 'h' || reply[0] == 'H'){
                playerCards->addCardToHand(deck->getCard());
                std::cout << "Player: ";
                playerCards->printHand();
            } else if(reply[0] == 's' || reply[0] == 'S'){
                //Player chose to not get another card
                break;
            } else{
                std::cout << "Invalid entry\n";
            }
        }

        //Dealer must hit until the value of their hand is under 16
        while (dealerCards->checkValue() < 16){
            dealerCards->addCardToHand(deck->getCard());
        }

        //Check the results

        int dealerValue = dealerCards->checkValue();
        int playerValue = playerCards->checkValue();

        std::cout << "\nDealer: ";
        dealerCards->printHand();
        std::cout << "Player: ";
        playerCards->printHand();

        if(playerValue > 21){
            std::cout << "You bust\n";
        }else if(dealerValue > 21){
            std::cout << "Dealer Busts\n";
        }else if (playerValue > dealerValue){
            std::cout << "You win!\n";
        } else if(playerValue < dealerValue){
            std::cout << "Dealer wins.\n";
        }else if(playerValue == dealerValue){
            std::cout << "Push\n";
        }else{
            std::cout << "Something is wrong*********************************************************************";
            playerCards->printHand();
            dealerCards->printHand();
        }

        //Wait 1 second to let the player see the results and not get confused by sudden change
        sleep(1);

        //Reset both hands
        playerCards->resetHand();
        dealerCards->resetHand();

        //Draw a line to separate each play
        std::cout << "*********************************************************************\n";
    }

    //New lines and free both hands
    std::cout << "\n\n\n";
    free(dealerCards);
    free(playerCards);
}

int main() {

    Deck deck = Deck();
    bool cont = true;
    string reply;
    bool badInput = false;

    while (cont){
        game(&deck);
        do {
            badInput = false;
            std::cout << "Play again? (y/n)";
            std::cin >> reply;
            if (reply[0] == 'y' || reply[0] == 'Y') {
                deck.shuffle();
            } else if (reply[0] == 'n' || reply[0] == 'N') {
                cont = false;
            } else{
                std::cout << "Bad input";
                badInput = true;
            }
        }while (badInput);
    }
    return 0;
}
