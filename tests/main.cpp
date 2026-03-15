#include <csv.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(){
    const std::vector<std::string> candidatePaths = {
        "./orders.csv",
        "../orders.csv",
        "tests/orders.csv"
    };
    std::vector<Order> orders;
    for (const auto& path : candidatePaths) {
        std::ifstream file(path);
        if (!file.good()) continue;
        orders = readCSV(path);
        if (!orders.empty()) break;
    }
    if (orders.empty()) {
        std::cerr << "No orders loaded.\n";
        return 1;
    }
    renderCSV(orders);
    // Test addOrder
    OrderBook book;
    addOrder(book, orders);
    // Verify bids (should be descending by price)
    std::cout << "\n--- BIDS (highest first) ---\n";
    for (const auto& level : book.bids) {
        std::cout << "Price: " << level.price
                  << " (" << level.orders.size() << " orders)\n";
    }
    // Verify asks (should be ascending by price)
    std::cout << "\n--- ASKS (lowest first) ---\n";
    for (const auto& level : book.asks) {
        std::cout << "Price: " << level.price
                  << " (" << level.orders.size() << " orders)\n";
    }
    return 0;
}
