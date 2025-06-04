#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <map>
#include <fstream>
#include <algorithm>
using namespace std;

// Node structure for Trie
struct Node {
    Node* links[26];
    bool flag = false;

    // Check if the node contains a specific key (letter)
    bool containsKey(char ch) {
        return links[ch - 'a'] != NULL;
    }

    // Insert a new node with a specific key (letter) into the Trie
    void put(char ch, Node* node) {
        links[ch - 'a'] = node;
    }

    // Get the node with a specific key (letter) from the Trie
    Node* get(char ch) {
        return links[ch - 'a'];
    }

    // Set the current node as the end of a word
    void setEnd() {
        flag = true;
    }

    // Check if the current node marks the end of a word
    bool isEnd() {
        return flag;
    }
};

// Wordle class
class Wordle {
private:
    Node* trie;
    int word_length;
    vector<vector<char>> scope;
    unordered_set<char> include;


public:
    // Constructor
    Wordle(int word_length) {
        trie = new Node();

        this->word_length = word_length;

        scope.resize(word_length);

        vector<char> all_letters;
        for (char letter = 'a'; letter <= 'z'; ++letter) {
            all_letters.push_back(letter);
        }

        for (int i = 0; i < word_length; ++i) {
            scope[i] = all_letters;
        }
    }

    // Inserts a word into the Trie
    void insert(string word) {
        if(word.size() != word_length){ cout<< "======================400============================="; return; }

        Node* node = trie;
        for (int i = 0; i < word.length(); i++) {
            if (!node->containsKey(word[i])) {
                // Create a new node for the letter if not present
                node->put(word[i], new Node());
            }
            // Move to the next node
            node = node->get(word[i]);
        }
        // Mark the end of the word
        node->setEnd();
    }

    // Adds a list of words to the Trie
    void populate_dictionary(vector<string> words){
        for(int i=0; i<words.size(); i++){
            insert(words[i]);
        }
    }

    // update the description based on the new guess
    void update_description(string guess, string feedback){
        if(guess.size() != word_length || feedback.size() != word_length){ cout<< "======================400============================="; return; }

        map<char, int> char_freq;
        for(int i=0; i<word_length; i++){
            if(feedback[i]=='g' || feedback[i]=='y'){
                char_freq[guess[i]]++;
            }
        }

        // green pass
        for(int i=0; i<word_length; i++){
            if(feedback[i] == 'g' && char_freq[guess[i]]){
                scope[i] = {guess[i]};
            }
        }

        // yellow pass
        for(int i=0; i<word_length; i++){
            if(feedback[i] == 'y' && char_freq[guess[i]]){
                scope[i].erase(std::remove(scope[i].begin(), scope[i].end(), guess[i]), scope[i].end()); // Erase–remove idiom
                include.insert(guess[i]);
            }
        }

        // grey pass
        for(int i=0; i<word_length; i++){
            if(feedback[i] == 'x' && !char_freq[guess[i]]){
               for(int j=0; j<word_length; j++){
                    scope[j].erase(std::remove(scope[j].begin(), scope[j].end(), guess[i]), scope[j].end());
                }
            }
        }
    }

    // Returns if the word is in the trie
    bool search(string word) {
        Node* node = trie;
        for (int i = 0; i < word.length(); i++) {
            if (!node->containsKey(word[i])) {
                // If a letter is not found, the word is not in the Trie
                return false;
            }
            // Move to the next node
            node = node->get(word[i]);
        }
        // Check if the last node marks the end of a word
        return node->isEnd();
    }

    // apply the description filter on the trie and get the words that match the filter
    vector<string> filter(){
        vector<string> hits;
        crawl(0, trie, "", hits, include);

        cout<< hits.size()<< " matches"<< endl;

        for(int i=0; i<hits.size(); i++){
            cout<< hits[i]<< ", ";
        }

        cout<< endl;

        return hits;
    }

    // DFS traversal through the trie
    void crawl(int level, Node* node, string curr_word, vector<string>& hits, unordered_set<char> include){
        if(node->isEnd()){
            // push to hits only if all the "include" letters are in the word
            if(include.empty()){ hits.push_back(curr_word); }
            return;
        }

        vector<char> possible_chars = scope[level];

        for(int i=0; i<possible_chars.size(); i++){
            if(node->containsKey(possible_chars[i])){
                int is_erased = include.erase(possible_chars[i]);
                crawl(level+1, node->get(possible_chars[i]), curr_word+possible_chars[i], hits, include);
                if(is_erased) { include.insert(possible_chars[i]); }
            }
        }

        return;
    }
};


int main() {
    int word_length = 5;
    Wordle wordle(word_length);

    ifstream dictionary("dictionary.txt");
    string word;
    while (getline (dictionary, word)) {
        wordle.insert(word);
    }
    dictionary.close();

    while(true){
        cout<< "enter guess: ";
        string input;
        cin >> input;
        
        string guess = input.substr(0, word_length);
        string feedback = input.substr(word_length+1, word_length);

        cout<< endl;

        wordle.update_description(guess, feedback);
        wordle.filter();

        cout<< endl;
    }



    return 0;
}