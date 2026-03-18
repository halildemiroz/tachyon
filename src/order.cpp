#include "ringBuffer.hpp"
#include <algorithm>
#include <cstdint>
#include <order.hpp>
#include <csv.hpp>
#include <fstream>
#include <iostream>

void addOrder(OrderBook& book, Order order, LockFreeQueue<TradeEvent>& tradeQueue, bool isRecovery){
	int orderIndex = priceToIndex(order.price);

	if(order.side == OrderSide::BIDS){
		while(order.quantity > 0 && book.bestAskIndex < NUM_PRICE_LEVELS){
			if(book.asks[book.bestAskIndex].orders.empty()){
				book.bestAskIndex++;
				continue;
			}
			
			if(book.bestAskIndex > orderIndex)
				break;

			Order& restingAsk = book.asks[book.bestAskIndex].orders.front();
			
			uint64_t tradeQuantity = (order.quantity >= restingAsk.quantity) ? restingAsk.quantity : order.quantity;

			order.quantity -= tradeQuantity;
			restingAsk.quantity -= tradeQuantity;

			TradeEvent trade;
			trade.timeStamp = order.timeStamp;
			trade.buyerID = order.id;
			trade.sellerID = restingAsk.id;
			trade.price = restingAsk.price;
			trade.quantity = tradeQuantity;

			if(!isRecovery)
				while(!tradeQueue.push(trade)){}

			if(restingAsk.quantity == 0)
				book.asks[book.bestAskIndex].orders.pop_front();
		}

		if(order.quantity > 0){
			book.bids[orderIndex].orders.push_back(order);
			if(orderIndex > book.bestBidIndex)
				book.bestBidIndex = orderIndex;
		}
	}
	else{
		while(order.quantity > 0 && book.bestBidIndex >= 0){
			if(book.bids[book.bestBidIndex].orders.empty()){
				book.bestBidIndex--;
				continue;
			}
			if(book.bestBidIndex < orderIndex)
				break;

			Order& restingBid = book.bids[book.bestBidIndex].orders.front();
		
			uint64_t tradeQuantity = (order.quantity >= restingBid.quantity) ? restingBid.quantity : order.quantity;

			order.quantity -= tradeQuantity;
			restingBid.quantity -= tradeQuantity;

			TradeEvent trade;
			trade.timeStamp = order.timeStamp;
			trade.buyerID = restingBid.id;
			trade.sellerID = order.id;
			trade.price = restingBid.price;
			trade.quantity = tradeQuantity;

			while(!tradeQueue.push(trade)){}

			if(restingBid.quantity == 0)
				book.bids[book.bestBidIndex].orders.pop_front();
		}
		if(order.quantity > 0){
			book.asks[orderIndex].orders.push_back(order);
			if(orderIndex < book.bestAskIndex)
				book.bestAskIndex = orderIndex;
		}
	}
}

void makeTrade(LockFreeQueue<TradeEvent>& tradeQueue, LockFreeQueue<Order>& orderQueue, const std::vector<Order>& rawOrders){
	OrderBook book;

	for(const auto& incomingOrders : rawOrders){
		while(!orderQueue.push(incomingOrders)){}
		addOrder(book, incomingOrders, tradeQueue);
	}

	Order poisonOrder; poisonOrder.quantity = 0;
	while(!orderQueue.push(poisonOrder)){}

	TradeEvent poisonTrade; poisonTrade.quantity = 0;
	while(!tradeQueue.push(poisonTrade)){}

}

OrderBook recoverOrderBook(){
	OrderBook book;
	std::ifstream file("../order_book.aof", std::ios::binary);

	if(!file.is_open()){
		std::cout << "Could not find order_book.aof";
		return book;
	}

	Order order;
	uint64_t recoveredCount = 0;
	LockFreeQueue<TradeEvent> dummyQueue;
	
	while(file.read(reinterpret_cast<char*>(&order), sizeof(Order))){
		if(order.quantity > 0){
			addOrder(book, order, dummyQueue, true);
			recoveredCount++;
		}
	}
	file.close();

	return book;
}

