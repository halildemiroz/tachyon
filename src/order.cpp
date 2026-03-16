#include <algorithm>
#include <cstdint>
#include <mutex>
#include <order.hpp>
#include <csv.hpp>
#include <iostream>

void addOrder(OrderBook& book, Order order, std::queue<TradeEvent>& q, std::mutex& mtx){
	if(order.side == OrderSide::BIDS){
		while(!book.asks.empty() && order.quantity > 0){
			PriceLevel& bestAskLevel = book.asks.front();
			
			if(order.price < bestAskLevel.price)
				break;

			while(!bestAskLevel.orders.empty() && order.quantity > 0){
				Order& restingAsk = bestAskLevel.orders.front();
				
				if(order.quantity >= restingAsk.quantity){
					order.quantity -= restingAsk.quantity;
					std::cout << "TRADE: " << restingAsk.quantity << " units @ "  << bestAskLevel.price << "\n";
				
					// AOF
					TradeEvent trade;
					trade.timeStamp = order.timeStamp;
					trade.buyerID = order.id;
					trade.sellerID = restingAsk.id;
					trade.price = bestAskLevel.price;
					trade.quantity = restingAsk.quantity;

					{
						std::lock_guard<std::mutex> lock(mtx);
						q.push(trade);
					}
					
					bestAskLevel.orders.pop_front();

				}else{
					restingAsk.quantity -= order.quantity;
					std::cout << "TRADE: " << order.quantity << " units @ " << bestAskLevel.price << "\n";

					// AOF
					TradeEvent trade;
					trade.timeStamp = order.timeStamp;
					trade.buyerID = order.id;
					trade.sellerID = restingAsk.id;
					trade.price = bestAskLevel.price;
					trade.quantity = order.quantity;
					
					{
						std::lock_guard<std::mutex> lock(mtx);
						q.push(trade);
					}

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
	
										TradeEvent trade;
										trade.timeStamp = order.timeStamp;
										trade.buyerID = restingBid.id;
										trade.sellerID = order.id;
										trade.price = bestBidLevel.price;
										trade.quantity = restingBid.quantity;

										{
											std::lock_guard<std::mutex> lock(mtx);
											q.push(trade);
										}

										bestBidLevel.orders.pop_front();

                } else {
                    restingBid.quantity -= order.quantity;
                    std::cout << "TRADE: " << order.quantity << " units @ " << bestBidLevel.price << "\n";
                    
										TradeEvent trade;
										trade.timeStamp = order.timeStamp;
										trade.buyerID = restingBid.id;
										trade.sellerID = order.id;
										trade.price = bestBidLevel.price;
										trade.quantity = order.quantity;

										{
											std::lock_guard<std::mutex> lock(mtx);
											q.push(trade);
										}

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
		if(order.side == OrderSide::BIDS){
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
