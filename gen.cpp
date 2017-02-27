#include <iostream>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <cctype>
#include <fstream>

using namespace std;

void convertWord(char word[], int lineCount);
void parseWord(ifstream& fin);

class TrieNode
{
public:
	TrieNode();
	~TrieNode();
	TrieNode* children[26];
	int count, line[30000];
};

class Trie
{
public:
	Trie();
	~Trie();
	void insert(const char *word, int lineCount) const;
	void printAll(TrieNode* cur, char* str);
	TrieNode* root;
};

Trie trie;

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		cout << "No enough perimeter. Please call program correctly." << endl;
		return 0;
	} /* argv not enough */

	ifstream fin(argv[1]);
	if (!(fin.is_open()))
		exit(1);

	parseWord(fin);

	char str[50];
	memset(str, 0, sizeof(str));
	trie.printAll(trie.root, str);

	fin.close();
	return 0;
} /* main() */

Trie::Trie()
{
	root = new TrieNode;
} /* Trie::Trie() constructor */

Trie::~Trie()
{
	delete root;
	root = NULL;
} /* Trie::~Trie() destructor */

void Trie::insert(const char *word, int lineCount) const
{
	TrieNode *cur = root;
	
	for (int i = 0; i < (int)strlen(word); i++)
	{
		if (!cur->children[(int)(word[i] - 'a')])
		{
			cur->children[(int)(word[i] - 'a')] = new TrieNode;
			cur = cur->children[(int)(word[i] - 'a')];
		}
		else
			cur = cur->children[(int)(word[i] - 'a')];
	}

	cur->line[cur->count] = lineCount;
	cur->count++;
	
} /* Trie::insert() */

void Trie::printAll(TrieNode* cur, char* str)
{
	if (cur->count != 0)
	{
		cout << setw(18) << left << str << cur->count << "   ";
		for (int i = 0; i < cur->count - 1; i++)
			cout<< cur->line[i] << ',';

  		cout << cur->line[cur->count - 1] << endl;
	}

	for (int i = 0; i < 26; i++)
		if (cur->children[i] != NULL)
		{
			str[strlen(str)] = i + 97;
			printAll(cur->children[i], str);
			str[strlen(str) - 1] = '\0';
		}
} /* Trie::printAll() */

TrieNode::TrieNode()
{
	count = 0;
	for (int i = 0; i < 26; ++i)
		children[i] = NULL;
} /* TrieNode::TrieNode() constructor */

TrieNode::~TrieNode()
{
	for (int i = 0; i < 26; ++i)
	{
		delete children[i];
		children[i] = NULL;
	}
} /* TrieNode::~TrieNode() destructor */

void convertWord(char *word, int lineCount)
{
	for (int i = 0; i < (int)strlen(word); i++)
		word[i] = (word[i] <= 90) ? (word[i] + 32) : (word[i]);
	trie.insert(word, lineCount);
} // convertWord() convert all capital words to lower case and pass in to tree

void parseWord(ifstream& fin)
{
	int newWord = 0, i = 0, lineCount = 1;
	char chr, word[100];

	while((fin.get(chr)).good())
	{

		if (isalpha(chr))
			{
				if (newWord)
					word[i++] = chr;
				else
				{
					memset(word, 0, sizeof(word));
					i = 0;
					word[i++] = chr;
					newWord = 1;
				}
			}	
		else if (newWord)
			{
				newWord = 0;
				convertWord(word, lineCount);	
			}

		if (chr == '\n')
			lineCount++;
	}
} // parseWord() parse word out of enTrie document
