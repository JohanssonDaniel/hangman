#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>
using namespace std;

const string alphabet  = " abcdefghijklmnopqrstuvwxyz";

void initGame();


void createDict(set<string>& dictionary) {
    string filename = "di2.txt";
    ifstream input;
    input.open(filename.c_str());
    while(true) {
        string word;
        input >> word;
        if (input.fail()) {
            break;
        }
        dictionary.insert(word);

    }
}

void selectWords(set<string>& dictionary, set<string>& wordsRemaining, int& size) { //Gets all words of users choice of length

    while (wordsRemaining.empty()) {
        cout << "Please type word length: ";
        size = 0;
        cin >> size;

        for (set<string>::iterator it = dictionary.begin(); it!=dictionary.end(); ++it) {
            if ((*it).size() == size) {
                wordsRemaining.insert(*it);
            }
        }

        if(wordsRemaining.empty()){
            cout << "No words with that size..." << endl << endl;
        }
    }
}

bool checkFormat(char& letterGuessed, set<char>& guessedLetters) {
    if (guessedLetters.count(letterGuessed) == 0) {
        if (alphabet.find(letterGuessed)) {
                return true;
            }
    }
    return false;
}

void makeWordFamilies(map<string, set<string>>& wordFamilies, char& letterGuessed, set<string>& wordsRemaining, int& wordSize, string& key) {
    wordFamilies.clear(); // make sure there no garbage.
    for (set<string>::iterator it= wordsRemaining.begin(); it!=wordsRemaining.end(); ++it) {
        string wordKey = key;
        string word = *it;

        for (int l = 0; l < wordSize; l++) {
            if (key[l] == '-') { //Only change key if it has -, in key -oo- we want to keep the o's.
                if (letterGuessed != word[l]) {
                    wordKey[l] = '-';
                }
                else {
                    wordKey[l] = letterGuessed;
                }
            }
            else {
                wordKey[l] = key[l];
            }
        }
        wordFamilies[wordKey].insert(word);
    }
}

void findBiggestFamily(map<string, set<string>>& wordFamilies, string& key, set<string>& wordsRemaining) {
    set<string> biggest;

    for (map<string, set<string>>::iterator it= wordFamilies.begin(); it!=wordFamilies.end(); ++it) {
        if ((it->second).size() > biggest.size()) {
            biggest = it->second;
            key = it->first;
        }
    }
    wordsRemaining = biggest;
}

void findSmallestKey(map<string, set<string>>& wordFamilies, string& key, set<string>& wordsRemaining){
    string smallestKey;         //The key with the most dashes
    int mostDashes = 0;
    for(map<string, set<string>>::iterator it = wordFamilies.begin(); it != wordFamilies.end(); it++){
        string tempKey = it->first;
        int tempMostDashes = 0;
        for(int n = 0; n < tempKey.size(); n++){
            if (tempKey[n] == '-'){
                ++tempMostDashes;
            }
        }
        if(tempMostDashes > mostDashes){
            mostDashes = tempMostDashes;
            smallestKey = tempKey;
        }
    }
    key = smallestKey;
    wordsRemaining = wordFamilies[smallestKey];
}

bool compareNewOldKey(string& newKey, string& oldKey){
    return (newKey.compare(oldKey) == 0);

}

bool keyEqualsLastWord(string& key, set<string>& wordsRemaining){
    set<string>::iterator it;
    it = wordsRemaining.begin();
    //Only checks if there is one word left
    if(wordsRemaining.size() == 1){
        if (*it == key){
            return true;
        }
        else{
            return false;
        }
    }
}

void printLosingMessage(){
    cout << "Hahaha, I have won over you!!!" << endl;
}

void printWinningMessage(string& key){
    cout << "You have won over me, the right word was: " << key << endl;
}

void endPrompt(){
    char answer;
    cout << "Would you like to try again(y/n): ";
    cin >> answer;
    switch(answer){
        case 'y':
            initGame();
            break;
        case 'n':
            break;
        default:
            break;
    }

}

void makeEmptyKey(int& size, string& key){
    for (int n = 0; n < size; n++) {
        key.append("-");
    }
}

void promptGuessLetter(char& letterGuessed, set<char>& guessedLetters){
    while (true) {
        cout << "\nGuess a letter: ";
        cin >> letterGuessed;
        if (checkFormat(letterGuessed, guessedLetters)) {
                guessedLetters.insert(letterGuessed);
                break;
            }
        else {
                cout << "Please type a LETTER" << endl;
            }
    }
}

void promptWordsLeft(set<string>& wordsRemaining){
    string yes_no;
    cout << "Do you want to see amount of words left? y/n: ";
    cin >> yes_no;

    if (yes_no == "y") {
        cout << "There are: " << wordsRemaining.size() << " words left" << endl;
    }
}

void printKey(string& key){
    cout << "\nCurrent key: " << key << endl;
}

void runGame(set<string> dictionary){
    int size;
    int guesses;
    string key;
    string oldKey;
    char letterGuessed;
    set<string> wordsRemaining;
    set<char> guessedLetters;
    map<string, set<string>> wordFamilies;

    selectWords(dictionary, wordsRemaining, size);

    makeEmptyKey(size, key);
    oldKey = key;

    cout << "\nHow many guesses do you want?: ";
    cin >> guesses;

    while (guesses != 0 && !keyEqualsLastWord(key,wordsRemaining)) {
        printKey(key);
        cout << "You have " << guesses << " guesses left" << endl;
        promptWordsLeft(wordsRemaining);

        promptGuessLetter(letterGuessed, guessedLetters);
        makeWordFamilies(wordFamilies,letterGuessed, wordsRemaining, size, key);

        if(guesses != 1){
            findBiggestFamily(wordFamilies, key, wordsRemaining);
        }
        else{
            findSmallestKey(wordFamilies, key, wordsRemaining);
        }
        if(compareNewOldKey(key, oldKey)){
            --guesses;
        }
        oldKey = key;
    }

    if(keyEqualsLastWord(key,wordsRemaining)){
        printWinningMessage(key);
        endPrompt();
    }
    else{
        printLosingMessage();
        endPrompt();
    }
}

void initGame(){
    set<string> dictionary;

    cout << "Loading... \n";
    createDict(dictionary);
    runGame(dictionary);
}

int main(){

    initGame();

    return 0;
}
