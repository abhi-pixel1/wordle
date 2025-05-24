#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
using namespace std;

// Node structure for Trie
struct Node {
    // Array to store links to child nodes,
    // each index represents a letter
    Node* links[26];
    // Flag indicating if the node
    // marks the end of a word
    bool flag = false;

    // Check if the node contains
    // a specific key (letter)
    bool containsKey(char ch) {
        return links[ch - 'a'] != NULL;
    }

    // Insert a new node with a specific
    // key (letter) into the Trie
    void put(char ch, Node* node) {
        links[ch - 'a'] = node;
    }

    // Get the node with a specific
    // key (letter) from the Trie
    Node* get(char ch) {
        return links[ch - 'a'];
    }

    // Set the current node
    // as the end of a word
    void setEnd() {
        flag = true;
    }

    // Check if the current node
    // marks the end of a word
    bool isEnd() {
        return flag;
    }
};

// struct Description {
//     vector<char> l1;
//     vector<char> l2;
//     vector<char> l3;
//     vector<char> l4;
//     vector<char> l5;

//     Description() {
//         for(char letter = 'a'; letter<='z'; letter++){
//             l1.push_back(letter);
//             l2.push_back(letter);
//             l3.push_back(letter);
//             l4.push_back(letter);
//             l5.push_back(letter);
//         }
//     }
// };

// Wordle class
class Wordle {
private:
    Node* trie;
    int word_length;
    vector<vector<char>> scope;
    unordered_set<char> include;


public:
    // Constructor to initialize the
    // Trie with an empty root node
    Wordle(int word_length) {
        trie = new Node();

        this->word_length = word_length;

        for(int i=0; i<word_length; i++){
            scope.push_back({});
        }

        for(char letter = 'a'; letter<='z'; letter++){
            scope[0].push_back(letter);
            scope[1].push_back(letter);
            scope[2].push_back(letter);
            scope[3].push_back(letter);
            scope[4].push_back(letter);
        }
    }

    // Inserts a word into the Trie
    // Time Complexity O(len), where len
    // is the length of the word
    void insert(string word) {
        Node* node = trie;
        for (int i = 0; i < word.length(); i++) {
            if (!node->containsKey(word[i])) {
                // Create a new node for
                // the letter if not present
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

    void update_description(string guess, string colors){
        for(int i=0; i<word_length; i++){
            if(colors[i] == 'g'){
                scope[i] = {guess[i]};
            }
            else if(colors[i] == 'y'){
                scope[i].erase(std::remove(scope[i].begin(), scope[i].end(), guess[i]), scope[i].end()); // Erase–remove idiom
                include.insert(guess[i]);
            }
            else if(colors[i] == 'x'){
                scope[0].erase(std::remove(scope[0].begin(), scope[0].end(), guess[i]), scope[0].end());
                scope[1].erase(std::remove(scope[1].begin(), scope[1].end(), guess[i]), scope[1].end());
                scope[2].erase(std::remove(scope[2].begin(), scope[2].end(), guess[i]), scope[2].end());
                scope[3].erase(std::remove(scope[3].begin(), scope[3].end(), guess[i]), scope[3].end());
                scope[4].erase(std::remove(scope[4].begin(), scope[4].end(), guess[i]), scope[4].end());
            }
        }
    }

    // Returns if the word
    // is in the trie
    bool search(string word) {
        Node* node = trie;
        for (int i = 0; i < word.length(); i++) {
            if (!node->containsKey(word[i])) {
                // If a letter is not found,
                // the word is not in the Trie
                return false;
            }
            // Move to the next node
            node = node->get(word[i]);
        }
        // Check if the last node
        // marks the end of a word
        return node->isEnd();
    }

    vector<string> surf(){
        vector<string> hits;
        rec(0, trie, "", hits, include);

        cout<< hits.size()<< " matches"<< endl;

        for(int i=0; i<hits.size(); i++){
            cout<< hits[i]<< ", ";
        }

        cout<< endl;

        return hits;
    }

    void rec(int level, Node* node, string curr_word, vector<string>& hits, unordered_set<char> include){
        if(node->isEnd()){
            if(include.empty()){ hits.push_back(curr_word); }
            return;
        }

        vector<char> search_space = scope[level];

        for(int i=0; i<search_space.size(); i++){
            if(node->containsKey(search_space[i])){
                int flag = include.erase(search_space[i]);
                rec(level+1, node->get(search_space[i]), curr_word+search_space[i], hits, include);
                if(flag) { include.insert(search_space[i]); }
            }
        }

        return;
    }
};


int main() {
    Wordle wordle(5);
    cout << "Inserting words: Striver, Striving, String, Strike" << endl;
    // wordle.insert("striver");
    // wordle.insert("striving");
    // wordle.insert("string");
    // wordle.insert("strike");

    wordle.populate_dictionary({"abcde", "after", "altef", "birto", "axaya"});
    
    cout << "Search if abcde exists in trie: " <<
    (wordle.search("abcde") ? "True" : "False")<< endl;
    
    cout << "Search if axaya exists in trie: " <<
    (wordle.search("axaya") ? "True" : "False" )<< endl;

    wordle.update_description("afxgh", "gxxxx");

    wordle.surf();

    return 0;
}