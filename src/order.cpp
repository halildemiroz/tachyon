#include "ringBuffer.hpp"
#include <algorithm>
#include <cstdint>
#include <order.hpp>
#include <csv.hpp>

void addOrder(OrderBook& book, Order order, LockFreeQueue& tradeQueue){
	if(order.side == OrderSide::BIDS){
		while(!book.asks.empty() && order.quantity > 0){
			PriceLevel& bestAskLevel = book.asks.front();
			
			if(order.price < bestAskLevel.price)
				break;

			while(!bestAskLevel.orders.empty() && order.quantity > 0){
				Order& restingAsk = bestAskLevel.orders.front();
				
				if(order.quantity >= restingAsk.quantity){
					order.quantity -= restingAsk.quantity;
				
					// AOF
					TradeEvent trade;
					trade.timeStamp = order.timeStamp;
					trade.buyerID = order.id;
					trade.sellerID = restingAsk.id;
					trade.price = bestAskLevel.price;
					trade.quantity = restingAsk.quantity;

					while(!tradeQueue.push(trade)){

					}
					
					bestAskLevel.orders.pop_front();

				}else{
					restingAsk.quantity -= order.quantity;

					// AOF
					TradeEvent trade;
					trade.timeStamp = order.timeStamp;
					trade.buyerID = order.id;
					trade.sellerID = restingAsk.id;
					trade.price = bestAskLevel.price;
					trade.quantity = order.quantity;
						
					while(!tradeQueue.push(trade)){

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
	
										TradeEvent trade;
										trade.timeStamp = order.timeStamp;
										trade.buyerID = restingBid.id;
										trade.sellerID = order.id;
										trade.price = bestBidLevel.price;
										trade.quantity = restingBid.quantity;

										while(!tradeQueue.push(trade)){

										}

										bestBidLevel.orders.pop_front();

                } else {
                    restingBid.quantity -= order.quantity;
                    
										TradeEvent trade;
										trade.timeStamp = order.timeStamp;
										trade.buyerID = restingBid.id;
										trade.sellerID = order.id;
										trade.price = bestBidLevel.price;
										trade.quantity = order.quantity;
										
										while(!tradeQueue.push(trade)){}

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

void makeTrade(LockFreeQueue& tradeQueue, const std::vector<Order>& rawOrders){
	
	OrderBook book;

	for(const auto& incomingOrders : rawOrders){
		addOrder(book, incomingOrders, tradeQueue);
	}

	TradeEvent poisonPill;
	poisonPill.quantity = 0;

	while(!tradeQueue.push(poisonPill)){}

}
