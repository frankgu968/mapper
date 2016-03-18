#include "TrieNode.h"
#include <string>
#include "utility.h"
#include <algorithm>
/*
 * Trie.h
 *
 *  Created on: Mar 10, 2016
 *      Author: eddd
 *
 *  Standard implementation of a trie tree for autocompletion of strings
 *  Stores words letter by letter in an unordered map, where the value is a pointer to the corresponding node
 *  By traversing the tree one letter at a time, all known words may be found
 */

#ifndef MAPPER_LIBSTREETMAP_SRC_TRIE_H_
#define MAPPER_LIBSTREETMAP_SRC_TRIE_H_
#define MIN_SIZE_TO_SEARCH 1

class Trie{
public:
	Trie();
	virtual ~Trie();

	/**
	 * Inserts a word into the trie
	 * @param word The string to insert
	 * @param importance Weighting to assign the word within autocorrect
	 */
	void insertWord(std::string word, int importance);


	/**
	 * Finds the most promising word that can be built from this node
	 * * Returns "" if none can be made
	 * @param word incomplete query to autocomplete
	 * @return most promising word that can be made
	 */
	std::string queryTopWord(std::string word);

	std::vector<std::string> queryAllWords(std::string word);

private:
	TrieNode* head;

	/**
	 * Finds the node corresponding to the last letter of a word
	 * Returns null if trie cannot follow the word
	 * @param word the word to track
	 * @return pointer to the last node for this word, if it exists
	 */
	TrieNode* trackLastNode(std::string word);

	/**
	 * Comparator of two word pairs
	 * Used for sorting of pair vector
	 * @param first first pair to compare
	 * @param second second pair to compare
	 * @return (first > second)?
	 */
	static bool compareWordPairs(const wordPair &first, const wordPair &second);
};

#endif /* MAPPER_LIBSTREETMAP_SRC_TRIE_H_ */
