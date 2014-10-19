#include "CommandParser.h"
#include <sstream>
#include <algorithm>

using namespace DDEngine;
using namespace std;

void CommandParser::parse(vector<string>* store, char delimiter, char argDelimiter, string line) {

	stringstream ssLine(line);
	string item;

	getline(ssLine, item, delimiter);
	store->push_back(item);

	getline(ssLine, item, delimiter);
	ssLine = stringstream(item);
	
	while (getline(ssLine, item, argDelimiter)) {
		store->push_back(item);
	}

	for (size_t i = 0; i < store->size(); i++) {
		stringstream ssQuotes(store->at(i));
		string inQuotes;
		
		getline(ssQuotes, inQuotes, '\"');
		getline(ssQuotes, inQuotes, '\"');

		if(inQuotes != store->at(i)) {
			store->at(i) = inQuotes;
		} else {
			string noSpaces = store->at(i);
			noSpaces.erase(remove_if(noSpaces.begin(), noSpaces.end(), isspace), noSpaces.end());
			store->at(i) = noSpaces;
		}
	}
}