#include <logger.hpp>

void tradeLogger(LockFreeQueue<TradeEvent>& tradeQueue){
	std::ofstream aofFile("../trade_ledger.aof", std::ios::trunc | std::ios::binary);
	
	while(1){
		TradeEvent event;

		if(tradeQueue.pop(event)){
			if(event.quantity == 0)
				break;

			aofFile.write(reinterpret_cast<const char*>(&event), sizeof(TradeEvent));
		}
	}
	aofFile.close();
}

void orderLogger(LockFreeQueue<Order>& orderQueue){
	std::ofstream aofFile("../order_book.aof", std::ios::trunc | std::ios::binary);
	
	while(1){
		Order order;
		if(orderQueue.pop(order)){
			if(order.quantity == 0)
				break;
			aofFile.write(reinterpret_cast<const char*>(&order), sizeof(Order));
		}
	}
	aofFile.close();
}
