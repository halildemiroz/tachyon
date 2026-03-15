#include <order.hpp>
#include <csv.hpp>

int main(){

	std::vector<Order> rawOrders = readCSV("./../stressTest.csv");
	
	OrderBook book;

	for(const auto& incomingOrders : rawOrders){
		addOrder(book, incomingOrders);
	}
}
