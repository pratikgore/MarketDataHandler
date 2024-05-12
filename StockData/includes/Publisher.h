#ifndef PUBLISHER_H
#define PUBLISHER_H

#include<iostream>
#include<stack>
#include <chrono>
#include <mutex>

using AlphaTime = std::chrono::high_resolution_clock::time_point;

struct SymbolData
{
	std::string symbol;
	std::string  timeStamp;
	double open;
	double close;
	double low;
	double high;
	double volume;
};

class Publisher
{
	public:
		Publisher();
		Publisher(std::chrono::seconds time);
		void PublishUpdates();
		void PushData(SymbolData symbolData);
	
	private:
		std::chrono::seconds 					waitTime;
		AlphaTime										lastPublishTime;
		std::mutex										 publisherMtx;
		std::stack<SymbolData> 			  InQ;
};

#endif