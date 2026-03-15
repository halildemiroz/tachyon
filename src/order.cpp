#include <algorithm>
#include <cstdint>
#include <order.hpp>
#include <csv.hpp>
#include <iostream>

void addOrder(OrderBook& book, Order order){
	if(order.side == OrderType::BIDS){
		while(!book.asks.empty() && order.quantity > 0){
			PriceLevel& bestAskLevel = book.asks.front();
			
			if(order.price < bestAskLevel.price)
				break;

			while(!bestAskLevel.orders.empty() && order.quantity > 0){
				Order& restingAsk = bestAskLevel.orders.front();
				
				if(order.quantity >= restingAsk.quantity){
					order.quantity -= restingAsk.quantity;
					std::cout << "TRADE: " << restingAsk.quantity << " units @ "  << bestAskLevel.price << "\n";

					bestAskLevel.orders.pop_front();
				}else{
					restingAsk.quantity -= order.quantity;
					std::cout << "TRADE: " << order.quantity << " units @ " << bestAskLevel.price << "\n";
					order.quantity = 0;
					break;
				}
			}

			if(bestAskLevel.orders.empty())
				book.asks.erase(book.asks.begin());
		} 
	}/*Incoming Order is a SELL*/else {
        while (!book.bids.empty() && order.quantity > 0) {
            PriceLevel& bestBidLevel = book.bids.front();

            if (order.price > bestBidLevel.price) {
                break; 
            }

            while (!bestBidLevel.orders.empty() && order.quantity > 0) {
                Order& restingBid = bestBidLevel.orders.front();

                if (order.quantity >= restingBid.quantity) {
                    order.quantity -= restingBid.quantity;
                    std::cout << "TRADE: " << restingBid.quantity << " units @ " << bestBidLevel.price << "\n";
                    bestBidLevel.orders.pop_front();
                } else {
                    restingBid.quantity -= order.quantity;
                    std::cout << "TRADE: " << order.quantity << " units @ " << bestBidLevel.price << "\n";
                    order.quantity = 0;
                    break;
                }
            }

            if (bestBidLevel.orders.empty()) {
                book.bids.erase(book.bids.begin());
            }
        }
    }
	
	if(order.quantity > 0){
		if(order.side == OrderType::BIDS){
			auto it = std::lower_bound(book.bids.begin(), book.bids.end(), order.price,
					[](const PriceLevel& pl, uint64_t price) {return pl.price > price;});
			if(it != book.bids.end() && it->price == order.price)
				it->orders.push_back(order);
			else
				book.bids.insert(it, PriceLevel{order.price, {order}});
		}else{
			auto it = std::lower_bound(book.asks.begin(), book.asks.end(), order.price,
					[](const PriceLevel& pl, uint64_t price){return pl.price < price;});
			if(it != book.asks.end() && it->price == order.price)
				it->orders.push_back(order);
			else
				book.asks.insert(it, PriceLevel{order.price, {order}});
		}
	}
}
