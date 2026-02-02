#include "zbior.h"
#include <new>
#include <random>
#include <cassert>
#include <algorithm>

void Zbior::alokuj(int n)
{
    assert(n > 0);
    m_elementy = new(std::nothrow) double[n] {};
    m_liczbaElementow = n;
}

void Zbior::zwolnij()
{
    delete[] m_elementy;
    m_elementy = nullptr;
    m_liczbaElementow = 0;
}

Zbior::Zbior(size_t liczElem, double dGran, double gGran)
{
    m_dolnaGranica = dGran;
    m_gornaGranica = gGran;

    alokuj(liczElem);

    std::uniform_real_distribution<double> zakres(dGran, gGran);
    std::random_device generator;

    for (size_t elem = 0; elem < liczElem; elem++)
        m_elementy[elem] = zakres(generator);
}

Zbior::~Zbior()
{
    zwolnij();
}

Zbior::operator double() const
{
    double suma = 0.0;
    for (size_t i = 0; i < m_liczbaElementow; i++)
        suma += m_elementy[i];

    return suma / m_liczbaElementow;
}

Zbior Zbior::operator+(const Zbior& other) const
{
    Zbior wynik(m_liczbaElementow + other.m_liczbaElementow,
        m_dolnaGranica, m_gornaGranica);

    for (size_t i = 0; i < m_liczbaElementow; i++)
        wynik.m_elementy[i] = m_elementy[i];

    for (size_t i = 0; i < other.m_liczbaElementow; i++)
        wynik.m_elementy[m_liczbaElementow + i] = other.m_elementy[i];

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(wynik.m_elementy, wynik.m_elementy + wynik.m_liczbaElementow, gen);

    return wynik;
}

Zbior::Zbior(const Zbior& other)
{
    m_liczbaElementow = other.m_liczbaElementow;
    m_dolnaGranica = other.m_dolnaGranica;
    m_gornaGranica = other.m_gornaGranica;

    m_elementy = new double[m_liczbaElementow];

    for (size_t i = 0; i < m_liczbaElementow; i++)
        m_elementy[i] = other.m_elementy[i];
}

Zbior& Zbior::operator=(const Zbior& other)
{
    if (this == &other)
        return *this;

    zwolnij();

    m_liczbaElementow = other.m_liczbaElementow;
    m_dolnaGranica = other.m_dolnaGranica;
    m_gornaGranica = other.m_gornaGranica;

    m_elementy = new double[m_liczbaElementow];

    for (size_t i = 0; i < m_liczbaElementow; i++)
        m_elementy[i] = other.m_elementy[i];

    return *this;
}

Zbior::Zbior(Zbior&& other)
{
    m_liczbaElementow = other.m_liczbaElementow;
    m_dolnaGranica = other.m_dolnaGranica;
    m_gornaGranica = other.m_gornaGranica;

    m_elementy = new double[m_liczbaElementow];
    for (size_t i = 0; i < m_liczbaElementow; i++)
        m_elementy[i] = other.m_elementy[i];

    other.m_elementy = nullptr;
    other.m_liczbaElementow = 0;
}