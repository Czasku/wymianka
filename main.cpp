#include "Zbior.h"
#include <cassert>
#include <stdexcept>
#include <iostream>
#include <algorithm>

class Funktor
{
    double d;

public:
    explicit Funktor(double d) : d(d) {}

    bool operator()(const Zbior& z) const
    {
        return static_cast<double>(z) == d;
    }
};


void test_kopia_gleboka()
{
    std::cerr << "Test kopi glebokiej: ";
    try
    {
        Zbior z1(5, -1.0, 1.0);
        Zbior z2 = z1;

        double* p1 = reinterpret_cast<double*>(&z1);
        double* p2 = reinterpret_cast<double*>(&z2);

        if (p1 == p2)
            throw std::logic_error("Kopia nie jest gleboka");
       
        if (static_cast<double>(z1) != static_cast<double>(z2))
            throw std::logic_error("Bledne dane w kopii");

        std::cerr << "OK\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "B³¹d\n";
    }
}

std::vector<Zbior> utworzZbiory()
{
    std::vector<Zbior> zbiory;
    zbiory.reserve(20);

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> rozmiarDist(5, 10);
    std::uniform_real_distribution<double> dolnaDist(-2.0, 1.0);
    std::uniform_real_distribution<double> gornaDist(2.0, 3.0);

    for (int i = 0; i < 20; i++)
    {
        int rozmiar = rozmiarDist(gen);
        double dolna = dolnaDist(gen);
        double gorna = gornaDist(gen);

        zbiory.push_back(Zbior(rozmiar, dolna, gorna));
    }

    return zbiory;
}



std::vector<Zbior> filtrujZera(const std::vector<Zbior>& wektor)
{
    std::vector<Zbior> wynik;

    std::copy_if(wektor.begin(), wektor.end(), std::back_inserter(wynik), Funktor(0.0));

    return wynik;
}


int main()
{
    test_kopia_gleboka();
}