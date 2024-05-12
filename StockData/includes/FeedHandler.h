#ifndef FEEDHANDLER_H
#define FEEDHANDLER_H

#include<vector>
#include<regex>
#include <iostream>
#include <fstream>
#include <regex>
#include<mutex>
#include "Api.h"
#include "Publisher.h"

class FeedHandler
{
	public:

		FeedHandler(const std::string& apiKey, Publisher&	publisher);
		void CreateFeed (const std::string& symbol);
	
	private :
		void CreateJSONFromFeed (const std::string& symbol, const std::string& StockData);
		void ParseAndPublishData();
		void ExtractOpenValue(const std::string& line, bool isUpdateComplete);

		bool 														 isValidKey{true};
		bool 													 	 isUpdateComplete{false};
		int 															prevUpdateLine{17};
		std::string												 apiKey;
		std::string					 							 fileName;
		std::vector<std::string>					patterns;
		SymbolData											symbolData;
		std::mutex 											   feedHandlerMutex;
		Publisher&										publisher;
};
#endif