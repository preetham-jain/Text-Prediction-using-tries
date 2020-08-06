#include<iostream>
#include<unordered_map>
#include<vector>
#include<algorithm>
#include<fstream>
#include<iomanip>
using namespace std;

struct wordhits {
    string word;
    int hits;
};

struct TrieNode 
{
    unordered_map<char, TrieNode*> children;
    bool isEnd;
    int noOfhits;
};

//=================== Function prototypes =====================
TrieNode* getNode();
TrieNode* insert( TrieNode*, string);
void search( TrieNode*, string);
void giveSuggestions(TrieNode*, string);
vector<wordhits> getlist(TrieNode*, string);
bool compareWordlist(wordhits, wordhits);
//==============================================================

//function to reserve memory and initialize variables.
TrieNode* getNode() {
    TrieNode* temp = new TrieNode;
    temp->isEnd = false;
    temp->noOfhits = 0;

    return temp;
}

//function used to compare two wordhits objects used for sorting.
bool compareWordlist(wordhits a, wordhits b) {
    return (a.hits > b.hits); 
}

//function to insert new words into the trie.
TrieNode* insert( TrieNode* root, string word) {
    TrieNode* temp = root;

    for(int i=0; i<word.length(); i++) {
        if(!temp->children[word[i]]) {
            // cout << word[i] << " is new " << temp->isEnd << endl;
            TrieNode* newNode = getNode();
            temp->children[word[i]] = newNode;
        }
        temp = temp->children[word[i]];
        
    }
    temp->isEnd = true;
    temp->noOfhits++;
    return root;
}

//function to check whether a given word exists in the trie.
void search( TrieNode* root, string word) {
    TrieNode* temp = root;
    int i;

    for(i=0; i<word.length(); i++) {
        if(temp->children[word[i]]) {
            temp = temp->children[word[i]];
        }
        else {
            break;
        }
    }
    if(temp->isEnd && i==word.length())
        cout << "It's in there..." << endl;
    else 
         cout << "WORD DOES NOT EXIST LUL!" << endl;   

}

//function to get list of all possible words from 'node'.
vector<wordhits> getlist(TrieNode* node, string word) {
    string tword;
    vector<wordhits> wlist, sublist;
    wordhits wits;
    TrieNode* temp;

    //increase the hit count of the node.
    node->noOfhits++;

    //if end of a word is found, add that word to the list.
    if(node->isEnd) {
        wits.word = word;
        wits.hits = node->noOfhits;
        wlist.push_back(wits);
    }

    //recursively go through all the nodes of the subtree and store them in the list.
    unordered_map<char, TrieNode*>::iterator it;
    for(it = node->children.begin(); it!=node->children.end(); it++) {
        tword = word + it->first;
        temp = node->children[it->first];
        sublist = getlist(temp, tword);
        wlist.insert(wlist.end(), sublist.begin(), sublist.end());
    }

    return wlist;
}

//function to give top 10 best suggestions for the prefix 'word' based on the noOfhits.
void giveSuggestions(TrieNode* root, string word) {
    TrieNode* node = root;

    //moves to the node of the last letter in 'word' and exits if it doesn't exist.
    for(int i=0; i<word.length(); i++) {
        if(node->children.find(word[i]) != node->children.end()) {
            node = node->children[word[i]];
        }
        else {
            cout << "No words exist with the prefix " << word << endl;
            return;
        }
    }

    //get all possible words from that node and create an array of the words with their noOfhits.
    vector<wordhits> wordslist;
    wordslist = getlist(node, word);

    //sort the vector based on the 'hits' value of the structure in decreasing order.
    sort(wordslist.begin(), wordslist.end(), compareWordlist);

    int len;
    //print the top 10 most used words from the list.
    for(int i=0; i<wordslist.size() && i<10; i++) {
        len = wordslist[i].word.length();
        if(len%2 == 0) wordslist[i].word += " ";
        cout << right << setw(20+len/2) << wordslist[i].word << " " << setw(5) << wordslist[i].hits << endl;
    }

}

//================================================= Main execution ========================================================
int main()
{
    int t;
    string word;
    TrieNode* root = getNode();
    ifstream file("words_alpha.txt");

    //get all words from the file and build trie.
    while(getline(file, word)) {
        root = insert(root, word);
    }
    cout << "Trie built successfully!" << endl;

    while(true) {
        cout << endl <<  "Enter word to give suggestions(0 to exit) " ;
        cin >> word;
        if(word == "0")
            break;
        giveSuggestions(root, word);
    }    

    return 0;
}