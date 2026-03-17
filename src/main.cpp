#include "ringBuffer.hpp"
#include <logger.hpp>
#include <csv.hpp>
#include <memory>
#include <order.hpp>

#include <thread>

int main(){
	
	auto tradeQueue = std::make_unique<LockFreeQueue>();
	std::vector<Order> rawOrders = readCSV("../massive_book.csv");
	
	std::jthread t1(makeTrade, std::ref(*tradeQueue), std::ref(rawOrders));
	std::jthread t2(Logger, std::ref(*tradeQueue));

	return 0;
}
