#include <csv.hpp>
#include <order.hpp>

#include <thread>
#include <queue>

std::queue<TradeEvent> tradeQueue;
std::mutex queueMutex;

void makeTrade(){
	std::vector<Order> rawOrders = readCSV("./../stressTest.csv");
	
	OrderBook book;

	for(const auto& incomingOrders : rawOrders){
		addOrder(book, incomingOrders, tradeQueue, queueMutex);
	}
}

void Logger(){
	while(1){
		
	}
}

int main(){
	std::jthread t(makeTrade, std::ref(tradeQueue), std::ref(queueMutex));
	std::jthread t2(Logger, std::ref(tradeQueue), std::ref(queueMutex));

	return 0;
}
