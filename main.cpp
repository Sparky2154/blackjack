#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;



struct Deck{
    short content[52] = {0};

private:
    short partition(short low, short high){
        short i = low-1;
        short hold;
        for (short j = low; j <= high-1; ++j) {
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
        return (short)(i + 1);
    }

    void quickShuffle(short low, short high){
        short index;
        if(low < high){
            index = partition(low, high);
            quickShuffle(low, index-1);
            quickShuffle(index+1, high);
        }
    }

public:
    void shuffle(){
        quickShuffle(0, 51);
    }
    void createDeck(){
        for (int i = 0; i < 52; ++i) {
            content[i] = i;
        }
    }
};


class Hand {
    short value = 0;
    short length = 0;

public:
    short content[11] = {0};
    short checkValue() {
        short aces = 0;
        for (int i = 0; i < length; ++i) {
            short mod = (content[i])%13;
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
    void addCardToHand(Deck* deck, short* currentCard){
        content[length] = deck->content[*currentCard];
        (*currentCard)++;
    }
};

short game(Deck* deck, short currentCard){
    string reply;
    bool hit = true;

    Hand* dealerCards = (Hand*)malloc(sizeof(Hand)*11);
    Hand* playerCards = (Hand*)malloc(sizeof(Hand)*11);

    while (currentCard < 26){
        std::cout << "Dealer: ";
        dealerCards->addCardToHand(deck, &currentCard);
        dealerCards->addCardToHand(deck, &currentCard);
        dealerCards->printHand();

        std::cout << "Player: ";
        playerCards->addCardToHand(deck, &currentCard);
        playerCards->addCardToHand(deck, &currentCard);
        playerCards->printHand();



        while (hit) {
            while (!(reply == "s" || reply == "h")){
                std::cout << "Hit or stay? (h/s)\n";
                cin >> reply;
            }
            hit = (reply == "h");
            playerCards->addCardToHand(deck, &currentCard);
        }



    }
    return currentCard;
}

int main() {

    Deck* deckPointer = (Deck*)malloc(sizeof(Deck));
    Deck deck = *deckPointer;
    deck.createDeck();


    for (int i : deck.content) {
        std::cout << i << '_';
    }

    std::cout << "\n------------------------\n";

    deck.shuffle();

    for (int i : deck.content) {
        std::cout << i << "_";
    }
    std::cout << "\n";

    short currentCard = 0;
    while (currentCard != -1){
        currentCard = game(&deck, currentCard);
    }

    return 0;
}
