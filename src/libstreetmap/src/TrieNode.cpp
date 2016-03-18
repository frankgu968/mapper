/*
 * TrieNode.cpp
 *
 *  Created on: Mar 10, 2016
 *      Author: eddd
 */

#include "TrieNode.h"

TrieNode::TrieNode() {
	isTerminus = false;
	importance = -1;
	maxContainedImportance = -1;
	mostImportantChar = '\0';
}

TrieNode::~TrieNode() {

	//Iterate through all nodes
	for (nodeMap::iterator it = nodes.begin(); it != nodes.end(); it++){
		//Delete node
		delete it->second;
	}
}

/**
 * Retrieves the node element corresponding the requested letter (e.g. querying '0' returns the 'a' elem
 * Will create a node element if it doesn't exist
 * @param input The requested letter
 * @return pointer to requested element
 */
TrieNode* TrieNode::getOrCreateTrieNode(char input){
	//Search for input
	nodeMap::iterator insertResponse = nodes.find(input);

	//Create new node if one wasn't found
	if(insertResponse == nodes.end()){
		nodes[input] = new TrieNode();
		return nodes[input];
	}

	//Return existing node
	else
		return insertResponse->second;

}



/**
 * Gets pointer to node for a given char
 * @param input the character to query
 * @return pointer to the character's node
 */
TrieNode* TrieNode::getTrieNodeForLetter(char input){
	//Search for input
	nodeMap::iterator insertResponse = nodes.find(input);

	//Return NULL if one wasn't found
	if(insertResponse == nodes.end()){
		return NULL;
	}

	//Return existing node
	else
		return insertResponse->second;

}

/**
 * Returns the most promising string that can be build from this node
 * @param curString string to autocomplete
 */
void TrieNode::buildMostPromisingString(std::string &curString){
	//No autocomplete is possible from here
	if(maxContainedImportance == -1)
		return;

	//Most important one already found
	else if(importance >= maxContainedImportance)
		return;

	//Add this char to autocompleted string
	curString += mostImportantChar;

	//Query on to next node
	TrieNode* bestNode = getTrieNodeForLetter(mostImportantChar);
	bestNode->buildMostPromisingString(curString);
}

/**
 * Recursively iterates through all words that can be made from this node
 * Pushes all words onto common vector
 * @param curWord current string from which to build on
 * @param allWords the vector to store found words
 */
void TrieNode::recursiveWordFind(std::string curWord, wordPriorityVector &allWords){
	TrieNode* curNode;
	std::string curString;

	//Word is found - store it
	if(isTerminus)
		allWords.push_back(std::make_pair(curWord, importance));

	//Don't make recursive calls if no nodes are contained
	if(nodes.empty())
		return;

	//Iterate through all letters known, make recursive calls
	for(auto it = nodes.begin(); it != nodes.end();  it++){
		//Get next letter's node
		curNode = it->second;
		//Add next letter to string
		curString = curWord + it->first;

		//Make recursive call
		curNode->recursiveWordFind(curString, allWords);
	}
}


