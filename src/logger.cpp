#include <logger.hpp>

#include <ringBuffer.hpp>

void Logger(LockFreeQueue& tradeQueue){
	std::ofstream aofFile("../engine.aof", std::ios::app | std::ios::binary);
	
	while(1){
		TradeEvent event;

		if(tradeQueue.pop(event)){
			if(event.quantity == 0)
				break;

			aofFile.write(reinterpret_cast<const char*>(&event), sizeof(TradeEvent));
		}
	}
	aofFile.close();
}
