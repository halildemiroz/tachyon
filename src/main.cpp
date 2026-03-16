#include <logger.hpp>
#include <csv.hpp>
#include <mutex>
#include <order.hpp>

#include <thread>
#include <queue>
#include <iostream>


int main(){
	std::queue<TradeEvent> tradeQueue;
	std::mutex queueMutex;
	
	std::jthread t1(makeTrade, std::ref(tradeQueue), std::ref(queueMutex));
	std::jthread t2(Logger, std::ref(tradeQueue), std::ref(queueMutex));

	return 0;
}
