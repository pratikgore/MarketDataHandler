#include <iostream>
#include <string>
#include <curl/curl.h>
#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <chrono>
#include "../includes/Api.h"
#include "../includes/FeedHandler.h"
#include "../includes/Publisher.h"

int main(int argc , char **argv) {

    if (argc != 4)
    {
        std::cout << " Two required parameters " <<std::endl;
        std::cout << " 1. API KEY : get you api key from Alphavantage.com" << std::endl;
        std::cout << " 2. Symbol name :  Please provide valid symbol name " << std::endl;
        std::cout << " e.g. AAPL, AMD, TCS, INFY ...." << std::endl;
        return 0;
    }
    
    auto waitTime = std::chrono::seconds(std::stoi(argv[3]));
    Publisher publisher(waitTime);
    FeedHandler feed(argv[1], publisher);
    feed.CreateFeed(argv[2]);
    publisher.PublishUpdates();

    //Infinitely running thread 
    std::thread t1(&Publisher::PublishUpdates, &publisher);

    return 0;
}
