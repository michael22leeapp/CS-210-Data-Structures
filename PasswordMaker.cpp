// Name: Michael Lee
// Program 3 - Password Maker
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <time.h>

using namespace std;

const int MIN_LENGTH = 3; // min length for token
int tokenCount; // keep track of the number of tokens
const std::string fileName{ "input.txt" }; // name of the file
std::ifstream ifile(fileName);
std::stringstream strStream;
std::string modStr; // string that will contain the words from input file
char16_t singleQuote {u'’'}; // was having an issue with this character


// changes the first letter of every word to uppercase
std::string changeToUpper(std::string str){
    int i = 3, temp = 0;
    while (str[i]){ // iterate through each character in the string
        str[i] = tolower(str[i]); // firstly, change all letters to lowercase
        if (temp == 0) {
            str[i] = toupper(str[i]); // change to uppercase
            temp = 1;
        }
        if (isspace(str[i]) || str[i] == '\n' || str[i] == '\r'){
            temp = 0;
        }
        i++;
    }
    return str;
}

std::vector<string> tokenize(std::string str){
    // copy string to char array
    int size = str.length();
    char charArray[size + 1];
    std::strcpy(charArray, str.c_str());

    // split string into tokens and remove punctuation
    char *token;
    int tokenLength;
    const char *delim = " ,.-:;'’\'\"\?!/)}]1234567890\r\n"; // ignore punctuation and numbers
    token = std::strtok(charArray, delim);
    vector<string> tokenVector; // vector to hold all the tokens
    while (token != NULL){
        tokenLength =  strlen(token);
        if (*token != singleQuote && tokenLength >= MIN_LENGTH) { // token must be at least 3 letters or longer
            tokenVector.push_back(token);
            tokenCount++; // increase count of tokens
            token = std::strtok(NULL, delim); // go to next token
        } else { // if not long enough, go to next token
            token = std::strtok(NULL, delim);
            continue;
        }
    }
    return tokenVector;
}

string generatePassword(vector<string> tokVec){
    //default_random_engine rand = default_random_engine(static_cast<unsigned int>(time(0)));
    //shuffle(tokVec.begin(), tokVec.end(), rand);
    srand (time(NULL)); // init random seed
    std::string randWords = "";
    for (int i = 0; i < 20; i++) { // generate 20 random passwords
        for (int j = 0; j < 4; j++) { // generate 4 random words for a password
            randWords = randWords + " " + tokVec[rand() % tokenCount];
        }
        randWords = randWords + "1\n"; // append the number '1' to end of password
    }
    return randWords;
}

int main(int argc, char **argv) {
    //Read from file and put into string
    if (!ifile.is_open()) { // error with opening file
        std::cerr << "\nCould not open file '" << fileName << "'\n";
    }
    else { // check if opening file was successful
        ifile.open(fileName);
        strStream << ifile.rdbuf();
        ifile.close();
        modStr = strStream.str();
    }
    std::cout << generatePassword(tokenize(changeToUpper(modStr)));
    return 0;
}