#include "generator.h"

constexpr double PI = 3.141592653589793238;

Generator::Generator()
{
    amplituda = 1;
    okres = 1;
    czasAktywacji = 0;
    wyborTypu = Typ::skokowy;
    wartoscStala = 1;
    p = 1;
};

Generator::Generator(
    Typ typ, double amplituda, int okres, int czasAktywacji, double wartoscStala, double i_p)
    : wyborTypu(typ)
    , wartoscStala(wartoscStala)
    , czasAktywacji(czasAktywacji)
    , p(i_p)
{
    this->amplituda = (amplituda < 0) ? 1 : amplituda;
    this->okres = (okres < 0) ? 1 : okres;
}

double Generator::Generuj(int czas)
{
    if (wyborTypu == Typ::skokowy)
        return (czas >= czasAktywacji) ? wartoscStala : 0;
    else if (wyborTypu == Typ::sinus) {
        double x = ((czas % okres) / (double) okres) * 2 * PI;
        return amplituda * sin(x);
    } else if (wyborTypu == Typ::prostokatny) {
        int i_mod = czas % okres;
        double prog = ((double) p * (double) okres);
        if (i_mod < prog)
            return amplituda;
        else
            return 0.0;
    }
}

double Generator::getStala()
{
    return wartoscStala;
};

int Generator::getCzasAktywacji()
{
    return czasAktywacji;
};

int Generator::getOkres()
{
    return okres;
};

double Generator::getAmplituda()
{
    return amplituda;
};

double Generator::getP()
{
    return p;
};

Typ Generator::getTyp()
{
    return wyborTypu;
};

void Generator::setTyp(Typ i_typ)
{
    wyborTypu = i_typ;
};

void Generator::setParametry(
    double i_amplituda, int i_okres, int i_czasAktywacji, double i_wartoscStala, double i_p)
{
    amplituda = i_amplituda;
    okres = i_okres;
    czasAktywacji = i_czasAktywacji;
    wartoscStala = i_wartoscStala;
    p = i_p;
};

void Generator::setAmplituda(double i_amplituda) {
    amplituda = i_amplituda;
}

void Generator::setOkres(int i_okres) {
    okres = i_okres;
}

void Generator::setCzasAktywacji(int i_czasAktywacji) {
    czasAktywacji = i_czasAktywacji;
}

void Generator::setWartoscStala(double i_wartoscStala) {
    wartoscStala = i_wartoscStala;
}

void Generator::setP(double i_p) {
    p = i_p;
}
