#pragma once

#include <algorithm>
#include <order.h>

class OrderBook{

	public:
	std::vector<Trade> addOrder(Order newOrder);

	void cancelOrder(Side side, uint64_t orderID);

	std::vector<Order>Bids;
	std::vector<Order>Asks;
	
	private:
	void insertBid(const Order& order){
		auto it = std::lower_bound(Bids.begin(), Bids.end(), order, [](const Order& a, const Order& b){return a.price < b.price;});

		Bids.insert(it, order);
	}

	void insertAsk(const Order& order){
		auto it = std::lower_bound(Asks.begin(), Asks.end(), order, [](const Order& a, const Order& b){return a.price > b.price;});
		Asks.insert(it, order);
	}

};

