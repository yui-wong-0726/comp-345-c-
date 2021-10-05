//
// Created by Yqi Kang on 2021-10-03.
//
#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "Order.h"
#include "OrdersList.h"


using namespace std;

enum CardType{
    bomb, reinforcement, blockade, airlift, diplomacy
};

const int CardTYPE_ITEMS = 5;

class Card{
public:
    Card();
    Card(CardType cardType);
    Card(const Card&);
    ~Card();

    friend ostream& operator<<(ostream& output, const Card& card);
    Card& operator=(const Card& card);
    CardType getType() const;
    static string cardTypeToString(CardType cardTypeToString);
    void play(vector<Order*> &olist, vector<Card *> &cards, vector<Card *> &handCards);

private:
    CardType cardType;
};


class Deck{
public:
    //default constructor
    Deck();
    //destructor
    ~Deck();
    //copy constructor
    Deck(const Deck&);
    //assignment operator
    Deck& operator=(const Deck& obj);
    //stream insertion operator
    friend ostream& operator<<(ostream& output, const Deck& deck);
    //get method
    vector<Card*> getCards();
    friend ostream& operator<<(ostream& output, const Deck& deck);
    //draw method
    Card* draw();
    void shuffleDeck();

private:
    vector<Card*> cards;
    int max_deck_size = 50;
    void initializeDeck();
};

class Hand {
public:
    Hand();
    Hand(Deck *deck);
    ~Hand();
    Hand(const Hand& obj);
    Hand& operator= (const Hand& obj);
    friend ostream& operator<<(ostream& output, const Hand& hand);
    vector<Card*> getHandCards();
private:
    vector<Card*> handCards;
};