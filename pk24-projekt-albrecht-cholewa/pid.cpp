#include "pid.h"

PID::PID(double up, double ui, double ud, double minWyjscie, double maxWyjscie)
    : wzmocnienie(up)
    , stalaCalkowania(ui)
    , stalaRozniczkowania(ud)
    , sumaCalkowania(0.0f)
{
    poprzedniaWartosc = 0.0f;
    UstawOgraniczenia(minWyjscie, maxWyjscie);
}

void PID::UstawOgraniczenia(double minWyjscie, double maxWyjscie)
{
    this->minWyjscie = minWyjscie;
    this->maxWyjscie = maxWyjscie;
}

double PID::ObliczProporcjonalne(double uchyb)
{
    return wzmocnienie * uchyb;
}

double PID::ObliczCalka(double uchyb)
{
    if (stalaCalkowania) {
        sumaCalkowania += uchyb;
        return sumaCalkowania / stalaCalkowania;
    } else
        return 0;
}

double PID::ObliczCalkaPod(double uchyb)
{
    if (stalaCalkowania) {
        sumaCalkowania += uchyb / stalaCalkowania;
        return sumaCalkowania;
    } else
        return 0;
}
double PID::ObliczRozniczka(double uchyb)
{
    double wartoscRozniczkujaca = 0.0f;
    if (!poprzedniaWartosc == NULL)
        wartoscRozniczkujaca = stalaRozniczkowania * (uchyb - poprzedniaWartosc);
    else
        wartoscRozniczkujaca = stalaRozniczkowania * uchyb;

    poprzedniaWartosc = uchyb;

    return wartoscRozniczkujaca;
}

double PID::ObliczSterowanie(double uchyb)
{
    proporcjonalne = ObliczProporcjonalne(uchyb);
    rozniczka = ObliczRozniczka(uchyb);

    if (czyLiczycPod)
        calka = ObliczCalkaPod(uchyb);
    else
        calka = ObliczCalka(uchyb);

    double sterowanie = proporcjonalne + calka + rozniczka;

    if (sterowanie < minWyjscie)
        sterowanie = minWyjscie;
    else if (sterowanie > maxWyjscie)
        sterowanie = maxWyjscie;

    return sterowanie;
}

double PID::Sumator(double wejscie, double wyjscie)
{
    return wejscie - wyjscie;
}

void PID::Reset()
{
    sumaCalkowania = 0.0f;
    poprzedniaWartosc = 0.0f;
}

double PID::getProporcjonalne()
{
    return proporcjonalne;
}
double PID::getCalka()
{
    return calka;
}
double PID::getRozniczka()
{
    return rozniczka;
}

void PID::setRegulator(double i_wzmocnienie,
                       double i_stalaCalkowania,
                       double i_stalaRozniczkowania,
                       double i_minWyjscie,
                       double i_maxWyjscie)
{
    wzmocnienie = i_wzmocnienie;
    stalaCalkowania = i_stalaCalkowania;
    stalaRozniczkowania = i_stalaRozniczkowania;
    maxWyjscie = i_maxWyjscie;
    minWyjscie = i_minWyjscie;
};

void PID::setWzmocnienie(double i_wzmocnienie) {
    wzmocnienie = i_wzmocnienie;
}

void PID::setStalaCalkowania(double i_stalaCalkowania) {
    stalaCalkowania = i_stalaCalkowania;
}

void PID::setStalaRozniczkowania(double i_stalaRozniczkowania) {
    stalaRozniczkowania = i_stalaRozniczkowania;
}

void PID::setMinWyjscie(double i_minWyjscie) {
    minWyjscie = i_minWyjscie;
}

void PID::setMaxWyjscie(double i_maxWyjscie) {
    maxWyjscie = i_maxWyjscie;
}

void PID::resetCalki()
{
    calka = 0;
    sumaCalkowania = 0;
};

void PID::zmienTypLiczeniaCalki(bool typ)
{
    czyLiczycPod = typ;
}
