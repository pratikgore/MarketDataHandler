#include "../includes/Api.h"
#include <curl/curl.h>
#include <cstdlib>                         

// Callback function to receive response data
size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *buffer) {
    size_t total_size = size * nmemb;
    buffer->append((char*)contents, total_size);
    return total_size;
}

Api::Api(const std::string& Key)
	{
		apikey = Key;
		baseURL = "https://www.alphavantage.co/query?";
		apiHandle = curl_easy_init();
		if(!apiHandle)
		{
			std::cerr << "Failed to initialize CURL !!" << std::endl;
			return;
		}
	}

void Api::fetchData(const std::string& symbol, const std::string& function, const std::string& stockData)
{
		std::string url{baseURL};
		baseURL += "function=" + function;
  		baseURL += "&symbol=" + symbol;

		if (function == "TIME_SERIES_INTRADAY")
		{
			baseURL += "&interval=1min";
		}

		baseURL += "&outputsize=";
		//Always append apikey at last for every URL.
		baseURL += "&apikey=" + apikey; 
		std::cout << " apikey : " << apikey <<std::endl;

		std::cout << " URL : [ " << baseURL << " ]" << std::endl; 
		std::cout << " For symbol : [  " << symbol << "  ]" <<std::endl; 

	   curl_easy_setopt(apiHandle, CURLOPT_URL, baseURL.c_str());
        curl_easy_setopt(apiHandle, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(apiHandle, CURLOPT_WRITEDATA, &stockData);
		curlResult = curl_easy_perform(apiHandle);

		if (curlResult != CURLE_OK)
			std::cerr << " Failed to perform request for symbol [  " << symbol << "  ]" << std::endl;
		else
		{
			std::cout << " Response received : " << std::endl;
			//DEBUG
			// std::cout << stockData << std::endl;
		}
}

Api::~Api()
{
	curl_easy_cleanup(apiHandle);
}
