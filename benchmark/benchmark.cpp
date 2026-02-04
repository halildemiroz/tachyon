#include <iostream>
#include <vector>
#include <chrono>
#include <orderbook.h>

int main() {
    OrderBook orderBook;
    const int NUM_ORDERS = 1000000;

    std::cout << "--- PRE-GENERATING " << NUM_ORDERS << " ORDERS ---\n";
    
    // We pre-generate orders so we don't measure the time it takes to "create" them,
    // only the time it takes to PROCESS them.
    std::vector<Order> orders;
    orders.reserve(NUM_ORDERS);

    for (int i = 0; i < NUM_ORDERS; ++i) {
        // Toggle between Buy and Sell to force matching
        Side side = (i % 2 == 0) ? Side::Bid : Side::Ask;
        // Random-ish prices around 100 to force trades
        double price = 100 + (i % 5); 
        
        orders.push_back(Order{
            (uint64_t)i, 
            price, 
            10, 
            getCurrentTimestamp(), 
            side, 
            OrderType::Limit
        });
    }

    std::cout << "--- STARTING BENCHMARK ---\n";

    auto start = std::chrono::high_resolution_clock::now();

    for (const auto& order : orders) {
        orderBook.addOrder(order);
    }

    auto end = std::chrono::high_resolution_clock::now();
    
    // Calculate Stats
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    double seconds = duration.count() / 1000.0;
    double ordersPerSec = NUM_ORDERS / seconds;

    std::cout << "--------------------------------\n";
    std::cout << "Time: " << seconds << " seconds\n";
    std::cout << "Throughput: " << (size_t)ordersPerSec << " orders/sec\n";
    std::cout << "--------------------------------\n";

    return 0;
}
