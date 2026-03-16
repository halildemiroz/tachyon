#include <logger.hpp>
#include <csv.hpp>
#include <mutex>
#include <order.hpp>

#include <thread>
#include <queue>
#include <iostream>



void makeTrade(std::queue<TradeEvent>& tradeQueue, std::mutex& queueMutex){
	std::vector<Order> rawOrders = readCSV("./../stressTest.csv");
	
	OrderBook book;

	for(const auto& incomingOrders : rawOrders){
		addOrder(book, incomingOrders, tradeQueue, queueMutex);
	}

	TradeEvent poisonPill;
	poisonPill.quantity = 0;

	{
		std::lock_guard<std::mutex> lock(queueMutex);
		tradeQueue.push(poisonPill);
	}

}

int main(){

	std::queue<TradeEvent> tradeQueue;
	std::mutex queueMutex;
	
	std::jthread t1(makeTrade, std::ref(tradeQueue), std::ref(queueMutex));
	std::jthread t2(Logger, std::ref(tradeQueue), std::ref(queueMutex));

	return 0;
}
