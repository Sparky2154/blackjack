#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;



class Deck{
    int content[52] = {0};
    int index = -2;

    int partition(int low, int high){
        int i = low-1;
        int hold;
        for (int j = low; j <= high-1; ++j) {
            if (rand()%2 == 1){
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
        int index;
        if(low < high){
            index = partition(low, high);
            quickShuffle(low, index-1);
            quickShuffle(index+1, high);
        }
    }

public:
    void shuffle(){
        quickShuffle(0, 51);
        index = -1;
    }
    void createDeck(){
        for (int i = 0; i < 52; ++i) {
            content[i] = i;
        }
    }
    int getCard(){
        index++;
        return content[index];
    }
    int getIndex(){
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
            int mod = (content[i])%13;
            if (mod<9){
                value += mod;
            } else if(mod<12){
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
    void printHand(){
        for (int i = 0; i <= content[0]; ++i) {
            std::cout << content[i+1] << " ";
        }
    }
    void addCardToHand(int card){
        content[length] = card;
        length++;
    }
};

void game(Deck* deck){
    string reply;
    bool hit = true;

    Hand* dealerCards = (Hand*)malloc(sizeof(Hand)*11);
    Hand* playerCards = (Hand*)malloc(sizeof(Hand)*11);

    while (deck->getIndex() < 26){
        std::cout << "Dealer: ";
        dealerCards->addCardToHand(deck->getCard());
        dealerCards->addCardToHand(deck->getCard());
        dealerCards->printHand();


        std::cout << "Player: ";
        playerCards->addCardToHand(deck->getCard());
        playerCards->addCardToHand(deck->getCard());
        playerCards->printHand();

        if (dealerCards->checkValue() == 21){
            std::cout << "Dealer has Blackjack";
            return;
        }

        while (hit) {
            while (!(reply == "s" || reply == "h")){
                std::cout << "Hit or stay? (h/s)\n";
                cin >> reply;
                if(reply[0] == 'h' || reply[0] == 'H'){
                    hit = true;
                } else if(reply[0] == 's' || reply[0] == 'S'){
                    hit = false;
                    break;
                } else{
                    hit = true;
                    std::cout << "Invalid entry\n";
                    continue;
                }
            }
            playerCards->addCardToHand(deck->getCard());
        }
    }

    while (dealerCards->checkValue() < 16){
        dealerCards->addCardToHand(deck->getCard());
    }

    int dealerValue = dealerCards->checkValue();
    int playerValue = playerCards->checkValue();

}

int main() {

    Deck* deckPointer = (Deck*)malloc(sizeof(Deck));
    Deck deck = *deckPointer;
    deck.createDeck();

    deck.shuffle();

    bool cont = true;
    string reply;
    while (cont){
        game(&deck);
        std::cout << "Play again? (y/n)";
        std::cin >> reply;
        if(reply[0] == 'y' || reply[0] == 'Y'){
            cont = true;
        } else if (reply[0] == 'n' || reply[0] == 'N'){
            cont = false;
        }
    }

    return 0;
}
