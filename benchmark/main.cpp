#include <logger.hpp>
#include <csv.hpp>
#include <mutex>
#include <order.hpp>

#include <thread>
#include <queue>
#include <iostream>

int main(){
    std::queue<TradeEvent> tradeQueue;
    std::mutex queueMutex;
    
    auto start = std::chrono::high_resolution_clock::now();

    {
        std::jthread t1(makeTrade, std::ref(tradeQueue), std::ref(queueMutex));
        std::jthread t2(Logger, std::ref(tradeQueue), std::ref(queueMutex));
        
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
