#include <logger.hpp>

void Logger(std::queue<TradeEvent>& tradeQueue, std::mutex& queueMutex){

	std::ofstream aofFile("../engine.aof", std::ios::app | std::ios::binary);

	while(1){
		TradeEvent event;
		bool isEvent = false;
	
		{
			std::lock_guard<std::mutex> lock(queueMutex);
			if(!tradeQueue.empty()){
				event = tradeQueue.front();
				tradeQueue.pop();
				isEvent = true;
			}
		}
		if(isEvent){
			if(event.quantity == 0)
				break;

			aofFile.write(reinterpret_cast<const char*>(&event), sizeof(TradeEvent));

			// aofFile << event.timeStamp << "," << event.buyerID << "," << event.sellerID << "," << event.price << "," << event.quantity << "\n";

			// std::cout << "TRADE: " << event.quantity << " units @ " << event.price << "\n";
		}
	}
	aofFile.close();
}
