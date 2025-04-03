#ifndef PID_H
#define PID_H

#include "glowne.h"
class Symulacja;
class PID
{
private:
    double wzmocnienie;
    double stalaCalkowania;
    double stalaRozniczkowania;
    double sumaCalkowania;
    double minWyjscie;
    double maxWyjscie;
    double poprzedniaWartosc;
    double proporcjonalne;
    double calka;
    double rozniczka;

    bool czyLiczycPod;

public:
    PID(double, double = 0, double = 0, double = -100.0f, double = 100.0f);
    void UstawOgraniczenia(double, double);

    double ObliczProporcjonalne(double);
    double ObliczCalka(double);
    double ObliczCalkaPod(double);
    double ObliczRozniczka(double);
    double ObliczSterowanie(double);

    double Sumator(double, double);

    double getProporcjonalne();
    double getCalka();
    double getRozniczka();

    void resetCalki();

    void setRegulator(double wzmocnienie,
                      double stalaCalkowania,
                      double stalaRozniczkowania,
                      double minWyjscie,
                      double maxWyjscie);

    void Reset();

    void setWzmocnienie(double i_wzmocnienie);
    void setStalaCalkowania(double i_stalaCalkowania);
    void setStalaRozniczkowania(double i_stalaRozniczkowania);
    void setMinWyjscie(double i_minWyjscie);
    void setMaxWyjscie(double i_maxWyjscie);

    void zmienTypLiczeniaCalki(bool);
};

#endif // PID_H
