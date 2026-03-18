#include "ringBuffer.hpp"
#include <logger.hpp>
#include <csv.hpp>
#include <memory>
#include <order.hpp>

#include <thread>
#include <iostream>

int main(){
    
    auto tradeQueue = std::make_unique<LockFreeQueue<TradeEvent>>();
    auto orderQueue = std::make_unique<LockFreeQueue<Order>>();

    std::vector<Order> rawOrders = readCSV("../massive_book.csv"); 
    
    auto start = std::chrono::high_resolution_clock::now();

    {
        std::jthread t1(makeTrade, std::ref(*tradeQueue), std::ref(*orderQueue), std::ref(rawOrders));
        std::jthread t2(orderLogger, std::ref(*orderQueue));
        std::jthread t3(tradeLogger, std::ref(*tradeQueue));
    }

    auto end = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double> diff = end - start; // Saniye cinsinden süre
    double gecenSure = diff.count();
    
    double islenenEmir = 100000.0; 
    double saniyedekiHiz = islenenEmir / gecenSure;

    std::cout << "--------------------------------------\n";
    std::cout << "Toplam Sure: " << gecenSure << " saniye\n";
    std::cout << "Motor Hizi : " << saniyedekiHiz << " emir/saniye (Throughput)\n";
    std::cout << "--------------------------------------\n";

    return 0;
}
