#include <iostream>
#include <fstream>
#include <random>
#include <string>

int main() {
    std::ofstream file("massive_book.csv");
    if (!file.is_open()) {
        std::cerr << "Dosya acilamadi!\n";
        return 1;
    }

    // Rastgele sayı motoru (Sabit seed kullanıyoruz ki her seferinde aynı piyasayı üretsin)
    std::mt19937 rng(42); 
    
    // %50 BUY, %50 SELL ihtimali
    std::uniform_int_distribution<int> sideDist(0, 1);
    
    // 6.500.000 fiyat etrafında, 50.000 standart sapma ile normal dağılım (Çan eğrisi)
    std::normal_distribution<double> priceDist(6500000.0, 50000.0);
    
    // 10 ile 5000 birim arası rastgele miktar
    std::uniform_int_distribution<int> qtyDist(10, 5000);

    uint64_t timestamp = 1678731000000;
    uint64_t order_id = 1000;

    for (int i = 0; i < 100000; ++i) {
        std::string side = (sideDist(rng) == 0) ? "BUY" : "SELL";
        
        // Fiyatı daha gerçekçi yapmak için son 3 haneyi yuvarlıyoruz (Tick size)
        uint64_t price = std::max(10000.0, priceDist(rng)); 
        price = (price / 1000) * 1000; 
        
        uint64_t qty = qtyDist(rng);

        // CSV formatında yazdır (Eğer senin okuyucun farklı bir format bekliyorsa burayı ona göre değiştir)
        file << timestamp << "," << order_id << "," << side << "," << price << "," << qty << "\n";

        timestamp += 1; // Her emir 1 milisaniye arayla geliyor
        order_id += 1;
    }

    file.close();
    return 0;
}
