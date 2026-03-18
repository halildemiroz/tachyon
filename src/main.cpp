#include "ringBuffer.hpp"
#include <logger.hpp>
#include <csv.hpp>
#include <memory>
#include <order.hpp>

#include <thread>

int main(){

	OrderBook activeBook = recoverOrderBook();
	
	auto tradeQueue = std::make_unique<LockFreeQueue<TradeEvent>>();
	auto orderQueue = std::make_unique<LockFreeQueue<Order>>();

	std::vector<Order> rawOrders = readCSV("../massive_book.csv");
	
	std::jthread t1(makeTrade, std::ref(*tradeQueue), std::ref(*orderQueue), std::ref(rawOrders));
	std::jthread t2(tradeLogger, std::ref(*tradeQueue));
	std::jthread t3(orderLogger, std::ref(*orderQueue));

	return 0;
}
