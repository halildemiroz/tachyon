#include <cstdint>
#include <orderbook.h>

std::vector<Trade> OrderBook::addOrder(Order order){

	std::vector<Trade> trades;

	std::vector<Order>* oppositeSide = (order.side == Side::Bid) ? &Asks : &Bids;
	
	while(order.quantity!=0 && !oppositeSide->empty()){

		bool canTrade = false;
		
		if(order.side == Side::Bid){
			if(oppositeSide->back().price <= order.price) canTrade = true;
		}else{
			if(oppositeSide->back().price >= order.price) canTrade = true;
		}

		if(canTrade){
			uint32_t tradeQuantity = std::min(oppositeSide->back().quantity, order.quantity);
			
			trades.push_back(Trade{
				(order.side == Side::Bid) ? order.orderID : oppositeSide->back().orderID,
				(order.side == Side::Bid) ? oppositeSide->back().orderID : order.orderID,
				oppositeSide->back().price,
				tradeQuantity,
				getCurrentTimestamp()
				});
			
			order.quantity-=tradeQuantity;
			oppositeSide->back().quantity-=tradeQuantity;
			
			if(oppositeSide->back().quantity == 0)
				oppositeSide->pop_back();
		}else{
			break;
		}
		
	}

	if(order.quantity > 0){
		if(order.side == Side::Bid) insertBid(order);
		else insertAsk(order);
	}

	return trades;
}

void OrderBook::cancelOrder(Side side, uint64_t orderID){
	if(side == Side::Ask){
		std::vector<Order>::iterator it;
		for(it = Asks.begin(); it != Asks.end(); ++it){
			if(orderID == it->orderID){
				Asks.erase(it);
				return;
			}
		}
	}
}


