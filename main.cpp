#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>

using namespace std;


struct Node {
    bool isCompleteWord;
    unordered_map<char, Node *> linker;

    Node() {
        isCompleteWord = false;
    }
};

struct Trie {
    Node *root;

    Trie() {
        root = new Node();
    }

    ~Trie() {
        delete root;
    }

    void insert(const string &word) {
        Node *current = root;
        char currentKey;
        for (char i : word) {
            currentKey = i;
            if (current->linker.find(currentKey) == current->linker.end()) {
                Node *newNode = new Node();
                current->linker[currentKey] = newNode;
                newNode = nullptr;
                delete newNode;
            }
            current = current->linker[currentKey];
        }
        current->isCompleteWord = true;
        current = nullptr;
        delete current;
    }

    vector<string> prefixTraversal(const string &prefix) {
        Node *currentNode = root;
        vector<string> output;
        string currentWord;
        char currentKey;
        for (char i : prefix) {
            currentKey = i;
            if (currentNode->linker.find(currentKey) != currentNode->linker.end()) {
                currentWord += i;
            } else {
                output.push_back(""); // abort prefix traversal if specified prefix path is not complete
                return output;
            }
            currentNode = currentNode->linker[currentKey];
        }
        return autoSuggestions(output, currentWord, currentNode);
    }


    vector<string> autoSuggestions(vector<string> output, string currentWord, Node *currentNode) {

        if (currentNode->isCompleteWord) {
            output.push_back(currentWord);
        }

        if (currentNode->linker.empty()) {
            return output;
        }

        char currentKey;
        for (auto &x : currentNode->linker) {
            currentKey = x.first;
            currentWord += currentKey;

            output = autoSuggestions(output, currentWord, currentNode->linker[x.first]);
            currentWord.pop_back();
        }
        return output;
    }

    void loadData(fstream& dataset) {
        string str;
        while(getline(dataset, str)) {
            insert(str);
        }
    }
};

int main() {
    Trie trie;

    fstream file("/Users/egorkudelia/Trie/dataset.txt");

    trie.loadData(file);
    vector<string> output;
    string prefix = "ameri";

    output = trie.prefixTraversal(prefix);

    for (auto &i : output) {
        cout << i << endl;
    }

    return 0;
}
