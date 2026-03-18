#pragma once

#include <ringBuffer.hpp>
#include <order.hpp>
#include <fstream>

void tradeLogger(LockFreeQueue<TradeEvent>& tradeQueue);
void orderLogger(LockFreeQueue<Order>& orderQueue);
