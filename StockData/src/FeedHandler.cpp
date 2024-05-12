
#include "../includes/FeedHandler.h"


FeedHandler::FeedHandler(const std::string& userApiKey, Publisher&	publisher) : publisher(publisher)
{
	if (userApiKey.empty() || userApiKey.size() != 16)
	{
		std::cerr << " Received invalid API key !!! No subscription request will accepted... " << std::endl;
		isValidKey = false;
		return;
	}
	apiKey = userApiKey;
    patterns.reserve(6);
    patterns.push_back("\"(\\d+\\. open)\":\\s*\"(\\d+\\.\\d+)\"");
    patterns.push_back("\"(\\d+\\. high)\":\\s*\"(\\d+\\.\\d+)\"");
    patterns.push_back("\"(\\d+\\. low)\":\\s*\"(\\d+\\.\\d+)\"");
    patterns.push_back("\"(\\d+\\. close)\":\\s*\"(\\d+\\.\\d+)\"");
    patterns.push_back("\"(\\d+\\. volume)\":\\s*\"(\\d+)\"");
    patterns.push_back("\"(2\\. Symbol)\":\\s*\"([^\"]+)\"");
    patterns.push_back("\"(\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2})\"");
}

void FeedHandler::CreateFeed(const std::string& symbol)
{
	if (!isValidKey)
		return;
	
	Api ap(apiKey);
    std::string stockData;
    ap.fetchData(symbol, "TIME_SERIES_INTRADAY", stockData);
	CreateJSONFromFeed(symbol , stockData);
}

void FeedHandler::CreateJSONFromFeed(const std::string& symbol, const std::string& stockData)
{
	//TODO : create directory if not present and insert file in it.
	std::fstream file; 
    fileName = symbol + ".json";
    
    //For sample demo if you reach API limit per day. 
    // fileName = "INFY.json";

   file.open(fileName, std::ios::out); 
   if(!file) 
   { 
       std::cout << "Error in creating file!!!"; 
       return; 
   } 
  
    std::cout << "File created successfully." << std::endl; 
    file << stockData;
    file.close();
	ParseAndPublishData();
}

void FeedHandler::ParseAndPublishData()
{
	std::ifstream file(fileName);
    int lineCt = 0; 
	if (file.is_open()) 
	{
        std::cout << "File exists and is opened successfully." << std::endl;

		std::string line;
		while(std::getline(file, line))
		{
            lineCt++;
            if (lineCt == 17 )
            {
                isUpdateComplete = true;
                //DEBUG
                //std::cout << "Setting publish flag to true" <<  lineCt << " : " << lineCt - prevUpdateLine <<std::endl;
                prevUpdateLine = lineCt;
            }
            else if ( lineCt - prevUpdateLine  == 7)
            {
                isUpdateComplete = true;
                //DEBUG
                //std::cout << "Setting publish flag to true" <<  lineCt <<  " ? " <<lineCt  << " : " << prevUpdateLine <<std::endl;
                prevUpdateLine = lineCt;
            }
            else 
                isUpdateComplete = false;
            
			ExtractOpenValue(line, isUpdateComplete);
		}
		file.close();
	}
}

void FeedHandler::ExtractOpenValue(const std::string& line, bool isUpdateComplete) 
{ 
	//TODO : Regex parsing logic is insufficient, find another way.
    // Regular expression pattern for OPEN, HIGH, LOW, CLOSE, VOLUME. 

    for( auto& pattern : patterns)
    {
        std::smatch matches;
        // Try try match the pattern in the line
        if (std::regex_search(line, matches, std::regex(pattern))) 
        {
            // Extract the value from the first captured group
            std::string field = matches[1];
            std::string valueStr = matches[2];
            if (field == "1. open")
            {
                //std::cout << "Matched with open " << field <<  " value :  " << valueStr << std::endl;
                symbolData.open = std::stod(valueStr);
            }
            else if (field == "2. high")
            {
                //std::cout << "Matched with high " << field << " value :  " << valueStr << std::endl;
                symbolData.high = std::stod(valueStr);
            }
            else if (field == "3. low")
            {
                //std::cout << "Matched with low " <<  field   << " value :  " << valueStr << std::endl;
                symbolData.low = std::stod(valueStr);

            }
            else if (field == "4. close")
            {
                //std::cout << "Matched with close " << field << " value :  " << valueStr << std::endl;
                symbolData.close = std::stod(valueStr);

            }
            else if (field == "5. volume")
            {
                //std::cout << "Matched with volume " << field << " value :  " << valueStr << std::endl;
                symbolData.volume = std::stod(valueStr);

            }
            else if (field == "2. Symbol")
            {
                //std::cout << "Matched with symbol " << field << " value :  " << valueStr << std::endl;
                symbolData.symbol = valueStr;
            }
            else
            {
                //DEBUG
                //std::cout << " Date and time matched " << field  << std::endl;
                symbolData.timeStamp = field;
            }
        }
    }

    if (isUpdateComplete)
    {
        std::lock_guard<std::mutex>lock(feedHandlerMutex);
        {
            //DEBUG
            //std::cout << "Adding data in map " << std::endl;
            publisher.PushData(symbolData);
        }
    }
}