#include <orderbook.h>
#include <iostream>

int main() {
    OrderBook orderBook;

    std::cout << "--- Initializing Order Book ---\n";

    // 1. SETUP: Add some Sellers (Asks) to the book
    // OrderID 1: Sell 100 @ $100
    orderBook.addOrder(Order{1, 100.0, 100, getCurrentTimestamp(), Side::Ask, OrderType::Limit});
    
    // OrderID 2: Sell 100 @ $101
    orderBook.addOrder(Order{2, 101.0, 100, getCurrentTimestamp(), Side::Ask, OrderType::Limit});
    
    // OrderID 3: Sell 100 @ $102
    orderBook.addOrder(Order{3, 102.0, 100, getCurrentTimestamp(), Side::Ask, OrderType::Limit});

    std::cout << "Sellers placed. Book is primed.\n\n";

    // 2. ACTION: Incoming "Market" Buy Order
    // Buyer wants 250 shares. They are willing to pay up to $105 (Aggressive)
    // This should eat Order #1 (100 shares), Order #2 (100 shares), and half of Order #3 (50 shares).
    std::cout << "--- Incoming BUY Order (250 shares @ $105) ---\n";
    
    Order incomingOrder = Order{4, 105.0, 250, getCurrentTimestamp(), Side::Bid, OrderType::Limit};
    
    // Run the engine
    std::vector<Trade> trades = orderBook.addOrder(incomingOrder);

    // 3. VERIFICATION: Print the Trades
    std::cout << "--- Trade Report (" << trades.size() << " trades executed) ---\n";
    
    for (const auto& trade : trades) {
        std::cout << "Trade Executed:\n";
        std::cout << "  Bid Order ID: " << trade.bidOrderId << "\n";
        std::cout << "  Ask Order ID: " << trade.askOrderId << "\n";
        std::cout << "  Price: $" << trade.price << "\n";
        std::cout << "  Quantity: " << trade.quantity << "\n";
        std::cout << "-------------------------\n";
    }

    return 0;
}
