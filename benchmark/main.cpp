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
    
    // 1. KRONOMETREYİ BAŞLAT
    auto start = std::chrono::high_resolution_clock::now();

    // 2. SUNİ SCOPE YARAT (Thread'leri hapset)
    {
        // Thread'ler burada doğar ve çalışmaya başlar
        std::jthread t1(makeTrade, std::ref(tradeQueue), std::ref(queueMutex));
        std::jthread t2(Logger, std::ref(tradeQueue), std::ref(queueMutex));
        
    } // DİKKAT: Bu süslü parantez bittiğinde, main fonksiyonu t1 ve t2'nin İŞİNİ BİTİRMESİNİ BEKLER! 
      // Logger dosyayı kapatmadan ve thread'ler ölmeden bu satırdan aşağıya inilmez.

    // 3. KRONOMETREYİ DURDUR
    auto end = std::chrono::high_resolution_clock::now();
    
    // 4. MATEMATİĞİ YAP
    std::chrono::duration<double> diff = end - start; // Saniye cinsinden süre
    double gecenSure = diff.count();
    
    // (Eğer 100.000 satırlık dosyayı okutuyorsan)
    double islenenEmir = 100000.0; 
    double saniyedekiHiz = islenenEmir / gecenSure;

    std::cout << "--------------------------------------\n";
    std::cout << "Toplam Sure: " << gecenSure << " saniye\n";
    std::cout << "Motor Hizi : " << saniyedekiHiz << " emir/saniye (Throughput)\n";
    std::cout << "--------------------------------------\n";

    return 0;
}
