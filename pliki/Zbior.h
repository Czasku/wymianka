#pragma once
#include <algorithm>
#include <random>

class Zbior
{
    size_t m_liczbaElementow = 0;
    double* m_elementy = nullptr;
    double m_dolnaGranica = 0;
    double m_gornaGranica = 0;

    void alokuj(int);
    void zwolnij();

public:
    Zbior(size_t liczElem, double dGran, double gGran);

    explicit operator double() const;

    Zbior operator+(const Zbior& other) const;

    Zbior(const Zbior& other);
    Zbior& operator=(const Zbior& other);

    Zbior(Zbior&& other);
    Zbior& operator=(Zbior&& other) = delete;

    ~Zbior();
};
