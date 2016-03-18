#include <stdlib.h>
#include "utility.h"
#include <unordered_map>

/*
 * TrieNode.h
 *
 *  Created on: Mar 10, 2016
 *      Author: eddd
 *
 *    A node within a trie tree
 *    Contains pointers to the nodes for each character that might follow
 */

#ifndef MAPPER_LIBSTREETMAP_SRC_TRIENODE_H_
#define MAPPER_LIBSTREETMAP_SRC_TRIENODE_H_
typedef std::pair<std::string, int> wordPair;
typedef std::vector<wordPair> wordPriorityVector;

class TrieNode {
	typedef std::unordered_map<char, TrieNode*> nodeMap;



public:
	TrieNode();
	virtual ~TrieNode();

	/**
	 * Retrieves the node element corresponding the requested letter (e.g. querying '0' returns the 'a' elem
	 * Will create a node element if it doesn't exist
	 * @param input The requested letter
	 * @return pointer to requested element
	 */
	TrieNode* getOrCreateTrieNode(char input);

	/**
	 * Gets pointer to node for a given char
	 * @param input the character to query
	 * @return pointer to the character's node
	 */
	TrieNode* getTrieNodeForLetter(char input);

	/**
	 * Returns the most promising string that can be build from this node
	 * @param curString string to autocomplete
	 */
	void buildMostPromisingString(std::string &curString);

	/**
	 * Getter for terminus status (is end of word)
	 * @return terminus status
	 */
	bool getTerminus(){ return isTerminus;	}

	/**
	 * Setter for terminus status
	 * @param _isTerminus new status
	 */
	void setTerminus(bool _isTerminus){ isTerminus = _isTerminus;	}

	/**
	 * Returns importance of the string terminating at this node (-1 if no terminating string)
	 * @return Importance of node
	 */
	int getImportance(){	return importance;	}

	/**
	 * Sets importance of the node
	 * @param _importance new importance
	 */
	void setImportance(int _importance){
		if(_importance > importance)
			importance = _importance;
	}

	/**
	 * Returns the importance of the most important child from this node
	 * @return importance of most important child
	 */
	int getMaxContainedImportance(){	return maxContainedImportance;	}

	/**
	 * Sets the highest contained importance among this node's children
	 * @param _maxContainedImportance new highest contained importance
	 */
	void updateMaxContainedImportance(int _maxContainedImportance, char importantChar){
		if(_maxContainedImportance > maxContainedImportance){
			maxContainedImportance = _maxContainedImportance;
			mostImportantChar = importantChar;
		}
	}

	/**
	 * Recursively iterates through all words that can be made from this node
	 * Pushes all words onto common vector
	 * @param curWord current string from which to build on
	 * @param allWords the vector to store found words
	 */
	void recursiveWordFind(std::string curWord, wordPriorityVector &allWords);

private:
	//Array of pointers to all the
	std::unordered_map<char, TrieNode*> nodes;
	bool isTerminus;
	char mostImportantChar;
	int importance, maxContainedImportance;


	/**
	 * Converts the character to an index for accessing data array
	 * @param input
	 * @return corresponding index of char
	 */
	int convertCharToIndex(char input);

	/**
	 * Converts an index back into the character
	 * @param idx index to convert
	 * @return original character
	 */
	char convertIndexToChar(int idx);
};

#endif /* MAPPER_LIBSTREETMAP_SRC_TRIENODE_H_ */
