#include "utilities.h"
#include <iostream>
#include <cstring>
#include <cctype>

using namespace std;

//constant declarations
const char WORDFILENAME[] = "REPLACE WITH PATH TO FILE CONTAINING LIST OF WORDS";
const int MAXWORDS = 10000;
const int WORDLENMAX = 6;

//function declarations
int playOneRound(const char words[][7], int nWords, int wordnum);
bool isProbeFound(const char words[][7], const char probeWord[7], int nWords);

int main()
{
    char words[MAXWORDS][WORDLENMAX+1]; //initializes empty array of words to fill
    
    int numWords = getWords(words, MAXWORDS, WORDFILENAME); //fills empty array of words and declares numWords to be the number of words filled
    if (numWords < 1) { //if there are no words or the filename is broken, the game cannot continue
        cout << "No words were loaded, so I can't play the game. " << endl;
        return 0;
    }
    
    int numRounds;
    cout << "How many rounds do you want to play? ";
    cin >> numRounds;
    cin.ignore(10000, '\n');
    

    if (numRounds < 0) {
        cout << "The number of rounds must be positive." << endl;
    }
    
    double roundsPlayed = 0;
    int totalScore = 0;
    int minScore = 0;
    int maxScore = 0;
    
    cout << endl;
    for (int i = 1; i <= numRounds; i++) { //loops for number of rounds the user wants to play
        cout << "Round " << i << endl;
        int hiddenWordIndex = randInt(0,numWords-1); //randomly assigns the index of the hidden word to use in the game
        char hiddenWord[7] = "";
        strcpy(hiddenWord, words[hiddenWordIndex]);
        cout << "The hidden word is " << strlen(hiddenWord) << " letters long." << endl;
        
        int n = playOneRound(words, numWords, hiddenWordIndex);
        
        
        //keeps track of maximum/minimum score
        if (i == 1) {
            minScore = n;
            maxScore = n;
        }
        else if (n > maxScore) {
            maxScore = n;
        }
        else if (n < minScore) {
            minScore = n;
        }
        
        roundsPlayed++;
        totalScore += n;
        
        if (n == 1) {
            cout << "You got it in 1 try." << endl;
        }
        else {
            cout << "You got it in " << n << " tries. " << endl;
        }
        cout.setf(ios::fixed);
        cout.precision(2);
        cout << "Average: " << totalScore/roundsPlayed << ", minimum: " << minScore << ", maximum: " << maxScore << endl << endl;
        
    }
    
}

int playOneRound(const char words[][7], int nWords, int wordnum) {
    
    if (nWords < 0 || wordnum < 0 || wordnum >= nWords) {
        return -1;
    }
    
    char hiddenWord[7] = "";
    strcpy(hiddenWord, words[wordnum]);
    
    bool correctGuess = false;
    int tries = 1;
    
    while (!correctGuess) {
        
        //getting probe word
        char probeWord[101] = "";
        cout << "Probe word: ";
        cin.getline(probeWord, 101);
        
        //checking conditions of probe word
        bool isLower = true;
        for (int i = 0; i<strlen(probeWord); i++) {
            if (!islower(probeWord[i])) {
                isLower = false;
                break;
            }
        }
        if (strlen(probeWord) < 4 || strlen(probeWord) > 6 || !isLower) { //checks to see length of probe word
            cout << "Your probe word must be a word of 4 to 6 lower case letters. " << endl;
            continue;
        }
        else if (!isProbeFound(words, probeWord, nWords)) { //calls other function to check if probe word is in the list of words
            cout << "I don't know that word." << endl;
            continue;
        }
        
        //if the word is guessed correctly, break out of the loop
        if (strcmp(probeWord, hiddenWord) == 0) {
            correctGuess = true;
            break;
        }
        
        int numGolds = 0, numSilvers = 0; //locally declares number of golds and silvers to keep track for current probe
        
        char copyHidden[7] = ""; //declares local copy of hidden word so it can be modified without consequence for later probes
        strcpy(copyHidden, hiddenWord);
        
        for (int i = 0; i<(strlen(probeWord)); i++) {
            if (probeWord[i] == copyHidden[i]) { //if a character in the probe word and hidden word match up, both of them are erased so they aren't double counted as silvers
                probeWord[i] = '2';
                copyHidden[i] = '1';
                numGolds++;
            }
        }
        
        for (int i = 0; i<(strlen(hiddenWord)); i++) {
            for (int j = 0; j<strlen(probeWord); j++) {
                if (probeWord[j] == copyHidden[i]){
                    numSilvers++;
                    probeWord[j] = '0'; //if the current character in the probe word is a silver, its erased so it isn't double counted later in the loop
                    break;
                }
            }
        }
        
        tries ++;
        cout << "Golds: " << numGolds << ", Silvers: " << numSilvers << endl;

    }
    
    return tries;
}

bool isProbeFound(const char words[][7], const char probeWord[7], int nWords) { //checks if probe word is in the array of valid words
    bool isProbeFound = true;
    for (int i = 0; i < nWords; i++) {
        if (strcmp(probeWord, words[i]) == 0) {
            break;
        }
        if (i == nWords-1) {
            isProbeFound = false;
        }
    }
    
    return isProbeFound;
}
