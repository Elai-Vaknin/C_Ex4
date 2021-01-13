#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>

#define NUM_LETTERS		((int)26)
#define LINE			256
#define WORD			30

/* Structs */

typedef enum {
	FALSE = 0,
	TRUE = 1
} 	boolean;

typedef struct Node {
	char letter;
	int count;
	struct Node* children[NUM_LETTERS];
}	Node;

typedef struct Trie{
	Node* root;
}	Trie;

typedef enum {
	Default = 0,
	Reversed = 1
}	Frequency;

/* Functions */

Node* createNode() {
	Node* node = (Node*) calloc(1,sizeof(Node));
	
	for(int i = 0; i < NUM_LETTERS; i++) {
		node->children[i] = (Node*) calloc(NUM_LETTERS, sizeof(Node));
	}
	
	return node;
}

Trie* createTrie() {
	Trie* tree = (Trie*) malloc(sizeof(Trie));
	tree->root = createNode();

	return tree;
}

int getIndex(char letter) {
	char temp = tolower(letter);

	return temp - 'a';
}

boolean trieContainsLetter(Node* root, char letter) {
	int index = letter - 'a';
	
	if(root->children[index]->letter) {
		return TRUE;
	}

	return FALSE;
}

void insertToTrie(Node* root, char* word) {
	char letter = word[0];

	if(!trieContainsLetter(root, letter)) {
		Node* node = createNode();
		node->letter = letter;
		root->children[getIndex(letter)] = node;
	}

	if(strlen(word) > 1) {
		insertToTrie(root->children[getIndex(letter)], word + 1);
	} else { 
		root->children[getIndex(letter)]->count++;
	}
}

boolean printCondition(int i, Frequency fr) {
	if(fr == Reversed)
		return i >= 0;

	return i < NUM_LETTERS;
}

void printWords(Node* root, Frequency fr, char* word) {
	int start = fr == Reversed ? NUM_LETTERS - 1 : 0;
	int inc = 1 - 2*fr;

	char newWord[WORD];

	for(int i = start; printCondition(i,fr); i += inc) { 
		if(root->children[i]->letter) {
			sprintf(newWord, "%s%c", word, 'a' + i);

			if(root->children[i]->count) {
				printf("%s %d\n", newWord, root->children[i]->count);
			}

			printWords(root->children[i], fr, newWord);
		}
	}
}

void printWordsAppearances(Node* root, Frequency fr) {
	char word[WORD];

	printWords(root, fr, word);
}

void freeNode(Node* node) {
	for(int i = 0; i < NUM_LETTERS; i++) {
		if(node->children[i])
			freeNode(node->children[i]);
	}
	
	free(node);
}

void freeTrie(Trie* tree) {
	Node* root = tree-> root;

	free(tree);

	freeNode(root);
}

int main(int argc, char* argv[]) {
	/* Variables */

	Frequency fr = Default;
	Trie* tree = createTrie();
	Node* root = tree->root;

	
	char buffer[LINE];
	char word[WORD];
	
	/* Checks argument */ 

	if( argc == 2 ) {
		if(argv[1][0] == 'r') {
			fr = Reversed;
		}
   	}
	
	/* Read from stdin */
	
	int j = 0;
	
	while (fgets(buffer, LINE, stdin)) {
		for(int i = 0; i < strlen(buffer); i++) {
			if(buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\t') {
				if(j == 0)
					continue;

				word[j] = '\0';

				insertToTrie(root, word);
				
				j = 0;
			} else if (tolower(buffer[i]) >= 'a' && tolower(buffer[i]) <= 'z'){
				word[j++] = tolower(buffer[i]);
			}
		}
	}
	
	/* Print results */

	printWordsAppearances(root, fr);

	/* Free memory */

	freeTrie(tree);
}

