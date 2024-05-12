#ifndef API_H
#define API_H

#include <curl/curl.h>
#include<iostream>
#include<string>

class Api
{
	public:
		Api(const std::string& Key);
		~Api();
	
		void fetchData(const std::string& symbol, const std::string& function, const std::string& stockData);

	private:
		std::string 									baseURL;
		std::string 									apikey;
		CURL											   *apiHandle;
		CURLcode									  curlResult{CURLE_OK};
};

#endif