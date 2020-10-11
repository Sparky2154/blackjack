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
    int content[52] = {0};
    int index = -2;

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

    void quickShuffle(int low, int high){
        int ind;
        if(low < high){
            ind = partition(low, high);
            quickShuffle(low, ind-1);
            quickShuffle(ind+1, high);
        }
    }

public:
    Deck(){
        for (int i = 0; i < 52; ++i) {
            content[i] = i;
        }
        shuffle();
    }

    void shuffle(){
        quickShuffle(0, 51);
        index = -1;
    }

    int getCard(){
        index++;
        return content[index];
    }

    int getIndex() const{
        return index;
    }
};


class Hand {
    int length = 0;
    int content[11] = {0};

public:
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
    void printHand(){
        for (int i = 0; i < length; ++i) {
            printCard((((content[i])%13)+2));
            std::cout << " ";
        }
        std::cout << '\n';
    }
    void printDealer(){
        std::cout << ((content[0])%13)+2 << " *" << '\n';
    }
    void addCardToHand(int card){
        content[length] = card;
        length++;
    }
    void resetHand(){
        length = 0;
    }
};

void game(Deck* deck){
    string reply;

    Hand* dealerCards = (Hand*)malloc(sizeof(Hand));
    Hand* playerCards = (Hand*)malloc(sizeof(Hand));

    while (deck->getIndex() < 26){
        std::cout << "Dealer: ";
        dealerCards->addCardToHand(deck->getCard());
        dealerCards->addCardToHand(deck->getCard());
        dealerCards->printDealer();


        std::cout << "Player: ";
        playerCards->addCardToHand(deck->getCard());
        playerCards->addCardToHand(deck->getCard());
        playerCards->printHand();
        std::cout << "\n";

        if(playerCards->checkValue() == 21){
            std::cout << "You got blackjack!";
            return;
        } else if (dealerCards->checkValue() == 21){
            std::cout << "Dealer has Blackjack";
            return;
        }

        while (playerCards->checkValue() < 21) {
            reply = "";
            std::cout << "Hit or stay? (h/s)\n";
            cin >> reply;
            if(reply[0] == 'h' || reply[0] == 'H'){
                playerCards->addCardToHand(deck->getCard());
                std::cout << "Player: ";
                playerCards->printHand();
            } else if(reply[0] == 's' || reply[0] == 'S'){
                break;
            } else{
                std::cout << "Invalid entry\n";
            }
        }
        while (dealerCards->checkValue() < 16){
            dealerCards->addCardToHand(deck->getCard());
        }

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
        sleep(1);
        playerCards->resetHand();
        dealerCards->resetHand();

        std::cout << "*********************************************************************\n";
    }
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
