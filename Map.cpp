#include "Map.h"
#include <stack>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>

using namespace std;

//Map Implementation
Map::Map()
{
    mName = "";
    numContinent = 0;
    numTerritory = 0;
}
Map::Map(string mName, int numContinent, int numTerritory)
{
    this->mName = mName;
    this->numContinent = numContinent;
    this->numTerritory = numTerritory;
}

//Getters
string Map::getMName() { return mName; }
int Map::getNumContinent() { return numContinent; }
int Map::getNumTerritory() { return numTerritory; }
vector<Territory*> Map::getTerritories() { return territories; }

//Setters
void Map::setMName(string mName)
{
    mName = mName;
}

void Map::displayTerritories()
{
    cout << "TERRITORIES: \n\n";
    for (auto i = territories.begin(); i != territories.end(); i++)
        cout << (*i)->getCID() << " -> " << (*i)->getTID() << " -> " << (*i)->getTName() << endl;
}
void Map::displayContinents()
{
    cout << "CONTINENTS: \n\n";
    for (auto i = continents.begin(); i != continents.end(); i++)
        cout << (*i)->getCID() << " -> " << (*i)->getCName() << endl;
}

//Validate() method: Check if all continents in map are connected
//Map is valid if all continents are connected
bool Map::validate()
{
    int length = this->continents.size();
    set<int> visitedContinents;         //Set of Visited Continents
    set<int>::const_iterator iterator;   //Iterator to iterate over map and modify territories

    //Check if Continents are connected
    for (int i = 0; i < length; i++) {
        if (this->continents[i]->isConnected()) {
            cout << "Map Continent " << this->continents[i]->getCName() << " is connected." << endl;
        }
        else {
            cout << "MAP IS INVALID - Map Continent " << this->continents[i]->getCName() << " is not connected." << endl;
            break;
        }
    }

    //Iterate through continents and add visited continents to visited vector
    for (int i = 0; i < length; i++) {
        for (iterator = this->continents[i]->connectedContinents.begin(); iterator != this->continents[i]->connectedContinents.end(); ++iterator) {
            visitedContinents.insert(*iterator);
        }
    }

    //After iteration, check if the # visited continents = # of total continents to validate map
    if (visitedContinents.size() != length) {
        cout << "MAP IS INVALID - One or more continents is unreachable." << endl;
        return   false;
    }
    else {
        cout << "Map is valid." << endl;
        return true;
    }
}

//Continent Implementation
Continent::Continent()
{
    cName = "";
    cID = 0;
}
Continent::Continent(string cName, int cID)
{
    this->cName = cName;
    this->cID = cID;
}

//Getters
string Continent::getCName() { return cName; }
int Continent::getCID() { return cID; }
vector<Territory*> Continent::getTerritoriesInContinent() { return territoriesInContinent; }
set<int> Continent::getConnectedContinent() { return connectedContinents; }

//Check if territoris in Continent are connected
bool Continent::isConnected()
{
    int length = this->territoriesInContinent.size(); //# of territories in one continent
    vector<int> visited;                            //Vector of territories that have been visited
    for (int i = 0; i < length; i++) {                      //Loop through all territories in one continent
        if (visited.empty()) {                        //Visit first territory node
            visited.push_back(this->territoriesInContinent[i]->getTID());
        }

        int length1 = territoriesInContinent[i]->adjTerritoriesInContinent.size();    //Get adjacent territories list length
        for (int j = 0; j < length1; j++) {                                                 //Loop adjacent territories      
            int temp = this->territoriesInContinent[i]->adjTerritoriesInContinent[j];   //Get adjacent territory ID
            int length2 = visited.size();                                             //Length of visited territories
            for (int k = 0; k < length2; k++) {                                             //Loop visited territories
                if (temp == visited[k]) {   //If adjacent territory has already been visited, dont add to visited list
                    break;
                }
                if (k == (length2 - 1)) {     //If adjacent territory has not been visited, add to visited list
                    if (temp != visited[k]) {
                        visited.push_back(temp);
                    }
                }
            }
        }
    }
    if (visited.size() != length) {         //Check if # of territories in continent = # of visited territories
        return false;
    }
    return true;                        //Graph is connected if all territories have been visited
}

//Territory Implementation
Territory::Territory()
{
    tID = 0;
    cID = 0;
    pID = 0;
    numArmy = 0;
    tName = "";
    cName = "";
    isAdjacent = false;
}
Territory::Territory(int tID, int cID, string tName, string cName)
{
    this->tID = tID;
    this->cID = cID;
    this->pID = 0;
    this->numArmy = 0;
    this->tName = tName;
    this->cName = cName;
    this->isAdjacent = false;
}
//Getters
int Territory::getTID() { return tID; }
int Territory::getCID() { return cID; }
int Territory::getPID() { return pID; }
int Territory::getNumArmy() { return numArmy; }
string Territory::getTName() { return tName; }
string Territory::getCName() { return cName; }
bool Territory::getAdjContinent() { return isAdjacent; }
vector<int> Territory::getAdjTerritoryOnMap() { return adjTerritoryOnMap; }
vector<int> Territory::getAdjTerritoryInContinent() { return adjTerritoriesInContinent; }
int Territory::getBelongedContinentID() {
    return Territory::isAdjacent;
}

//Setters
void Territory::setTID(int tID)
{
    tID = tID;
}
void Territory::setTName(string tName)
{
    tName = tName;
}
void Territory::setAdjContinent(bool canLeave)
{
    isAdjacent = canLeave;
}

void Territory::displayBorders()
{
    cout << "\n" << getTName() << ": " << endl;
    for (auto i : adjTerritoryOnMap)
    {
        cout << adjTerritoryOnMap[i] << " -> ";
    }
}

//Map Loader Implementation
MapLoader::MapLoader() {
    worldMap;
    mFile = "";
}
string MapLoader::getMapFile() {
    return mFile;
}
void MapLoader::setMapFile(const string& mf) {
    this->mFile += mf;
}
vector<string> MapLoader::parseString(const string& s) {
    vector<string> tokens;  //Vector of tokens parsed
    stringstream check(s);
    string temp;

    while (getline(check, temp, ' ')) {
        tokens.push_back(temp);
    }
    return tokens;
}
vector<int> MapLoader::parseStringBorder(const string& s) {
    vector<int> tokens;  //Vector of tokens parsed
    stringstream check(s);
    string temp;

    while (getline(check, temp, ' ')) {
        tokens.push_back(stoi(temp));
    }
    return tokens;
}
void MapLoader::readMap() {
    int cNum = 0;
    int tNum = 0;
    int adjRow = 0;
    string text;
    regex rx("; map:.");
    smatch mr;

    cout << getMapFile() << endl;
    ifstream file(getMapFile());
    getline(file, text);

    if (regex_search(text, mr, rx)) {
        cout << "Map Name is valid :)\n\n\n";
    }
    else {
        cout << "Wrong Map Format! Closing Program...";
        exit(0);
    }

    while (getline(file, text))    //Read file line by line
    {
        if (text == "[continents]")
        {
            while (getline(file, text))
            {
                if (text == "")
                    break;
                vector<string> tokens = parseString(text);
                Continent* c = new Continent(tokens[0], cNum++);
                worldMap.continents.push_back(c);
            }
            //for(auto i : worldMap.continents){
            //    cout << *i << endl;
            //}
        }

        if (text == "[countries]")
        {
            while (getline(file, text))
            {
                tNum++;
                if (text == "")
                    break;
                vector<string> tokens = parseString(text);
                Territory* t = new Territory(stoi(tokens[0]), stoi(tokens[2]), tokens[1], "");
                worldMap.territories.push_back(t);
            }
            //for(auto i : worldMap.territories){
            //    cout << *i << endl;
            //}
        }

        if (text == "[borders]")
        {
            while (getline(file, text))
            {
                if (text == "")
                    break;
                vector<int> line = parseStringBorder(text);
                //worldMap.adjTerritoriesInContinent.push_back(parseStringBorder(text));
                for (auto i : line)
                {
                    worldMap.territories[adjRow]->adjTerritoryOnMap.push_back(i);

                    int currentContinent = worldMap.territories[adjRow]->getCID();
                    if (worldMap.territories[i - 1]->getCID() == currentContinent)
                        worldMap.territories[adjRow]->adjTerritoriesInContinent.push_back(i);
                }
                cout << "Territory #" << adjRow + 1 << ": ";
                for (auto i : worldMap.territories[adjRow]->adjTerritoryOnMap) {
                    cout << i << ", ";
                }
                cout << "\nTerritories in Continent: ";
                for (auto i : worldMap.territories[adjRow]->adjTerritoriesInContinent) {
                    cout << i << ", ";
                }
                cout << "\n\n";
                adjRow++;
            }

        }
    }
    file.close();
}

//Global Functions
ostream& operator << (ostream& out, const Continent& c) {
    out << c.cName << " -> #" << c.cID << endl;
    return out;
}

ostream& operator << (ostream& out, const Territory& t) {
    out << t.tName << " -> #" << t.tID << ", Continent: " << t.cID << endl;
    return out;
}



