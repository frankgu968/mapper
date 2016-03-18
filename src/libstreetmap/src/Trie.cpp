#include "Trie.h"


/*
 * CityTrieTree.cpp
 *
 *  Created on: Mar 10, 2016
 *      Author: eddd
 */


Trie::Trie() {
	head = new TrieNode();
}

Trie::~Trie() {
	delete head;
}

/**
 * Inserts a word into the trie
 * @param word The string to insert
 * @param importance Weighting to assign the word within autocorrect
 */
void Trie::insertWord(std::string word, int importance){
	int wordLength = word.size();
	TrieNode* curNode, *lastNode;

	//Initial node
	lastNode = head;

	//Iterate through every letter
	for(int i = 0; i < wordLength; i++){
		char curChar = word[i];

		//Get or create reference to next node
		curNode = lastNode->getOrCreateTrieNode(curChar);

		//If incoming word is more important, update most important word that can be found from this node
		lastNode ->updateMaxContainedImportance(importance, curChar);

		//If character is last in word, flag as terminus
		if(i == (wordLength-1)){
			curNode->setTerminus(true);
			curNode->setImportance(importance);
		}

		//Update pre node
		lastNode = curNode;
	}

	return;
}

/**
 * Finds the node corresponding to the last letter of a word
 * Returns null if trie cannot follow the word
 * @param word the word to track
 * @return pointer to the last node for this word, if it exists
 */
TrieNode* Trie::trackLastNode(std::string word){
	TrieNode* curNode = head;

	//No words loaded
	if(curNode == NULL)
		return NULL;

	//Track tree up to last input letter
	for(unsigned i = 0; i<word.size(); i++){
		char curChar = word[i];

		//Get next node
		curNode = curNode->getTrieNodeForLetter(curChar);

		//There is no known word for the current input
		if(curNode == NULL)
			return NULL;
	}

	return curNode;
}
/**
 * Finds the most promising word that can be built from this node
 * Returns "" if none can be made
 * @param word incomplete query to autocomplete
 * @return most promising word that can be made
 */
std::string Trie::queryTopWord(std::string word){
	//Don't bother querying words that are too small
	if(word.size() <= MIN_SIZE_TO_SEARCH) return word;

	std::string completedString = word;
	//Get node corresponding to last node of word
	TrieNode*curNode = trackLastNode(word);

	//Word doesn't exist, or no words saved
	if(curNode == NULL)
		return "";

	//We've reached point in trie corresponding to end of user's input
	//User has entered a valid string - leave it as is
	if(curNode->getTerminus())
		return word;

	//There are no words possible from this string
	else if (curNode->getMaxContainedImportance() == -1)
		return "";

	//Find most promising word starting from this node
	curNode->buildMostPromisingString(completedString);
	return completedString;
}



/**
 * Returns a vector of all words found
 * @param word the word for which to search
 * @return a vector containing all possible words, sorted by decreasing importance
 */
std::vector<std::string> Trie::queryAllWords(std::string word){
	wordPriorityVector wordPairsFound; //holds pairs of strings + priority
	std::vector<std::string> wordsFound;  //holds just strings found

	//Ignore queries that are too small
	if(word.size() <= MIN_SIZE_TO_SEARCH)
		return wordsFound;

	//Get node corresponding to last node of word
	TrieNode*curNode = trackLastNode(word);

	//No node possible
	if(curNode == NULL)
		return wordsFound;

	//Perform a recursive search for all possible words
	curNode->recursiveWordFind(word, wordPairsFound);

	//Sort all words found by their priority
	std::sort(wordPairsFound.begin(), wordPairsFound.end(), compareWordPairs);

	//Copy over just the strings
	for(unsigned i = 0; i<wordPairsFound.size(); i++){
		wordsFound.push_back(wordPairsFound[i].first);
	}

	return wordsFound;
}

/**
 * Comparator of two word pairs
 * Used for sorting of pair vector
 * @param first first pair to compare
 * @param second second pair to compare
 * @return (first > second)?
 */
bool Trie::compareWordPairs(const wordPair &first, const wordPair &second){
	int priority1, priority2;

	//Load priorities
	priority1 = first.second;
	priority2 = second.second;

	//Compare
	return priority1>priority2;
}


