//Why another thred required ?
//As we want to publish data in after defined way (after defined interval )

#include "../includes/Publisher.h"

Publisher::Publisher()
{

}

Publisher::Publisher(std::chrono::seconds time)
{
	waitTime = time; 
	lastPublishTime = std::chrono::high_resolution_clock::now();
	// std::cout << " Setting wait time for : " << time << " seconds " << std::endl;
}

void Publisher::PushData(SymbolData symbolData)
{
	InQ.push(symbolData);
}

void Publisher::PublishUpdates()
{
	while (true)
	{
		AlphaTime currTime = std::chrono::high_resolution_clock::now();
		auto secondsSinceEpoch = std::chrono::duration_cast<std::chrono::seconds>(currTime.time_since_epoch());
		auto lastsecondsEpoc = std::chrono::duration_cast<std::chrono::seconds>(lastPublishTime.time_since_epoch());

		//DEBUG
		//std::cout << "Time diff is : " << secondsSinceEpoch.count() - lastsecondsEpoc.count() << std::endl;
		if (currTime - lastPublishTime > waitTime)
		{
			//DEBUG
			//std::cout<<"Qsize : " << InQ.size() << std::endl;
			if (InQ.size() > 0)
			{
				std::lock_guard<std::mutex>gurad(publisherMtx);
				{
						SymbolData& data = InQ.top(); 
						std::cout << " Update received : " << data.timeStamp <<std::endl;
						std::cout << " Symbol : " << data.symbol <<std::endl;
						std::cout << " Open : " << data.open <<std::endl;
						std::cout << " Close : " << data.close <<std::endl;
						std::cout << " High : " << data.high <<std::endl;
						std::cout << " High : " << data.low <<std::endl;
						std::cout << " Volume : " << data.volume <<std::endl;
						std::cout << std::endl << std::endl;
						InQ.pop();
				}
				lastPublishTime = currTime;
			}
		}
	}
}