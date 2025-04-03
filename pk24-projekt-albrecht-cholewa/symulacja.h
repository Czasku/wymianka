#ifndef SYMULACJA_H
#define SYMULACJA_H

#include "arx.h"
#include "generator.h"
#include "glowne.h"
#include "pid.h"

class MainWindow;

class Symulacja : public QObject
{
    Q_OBJECT
private:
    PID regulator;
    ARX obiekt;
    Generator generator;
    int liczbaKrokow;
    int aktualnyKrok;
    QTimer *timer;
    double czasSymulacji;
    double interwal;
    vector<double> zadana;
    vector<double> uchyb;
    vector<double> sterowanie;
    vector<double> regulowana;
    vector<double> skladowaP;
    vector<double> skladowaI;
    vector<double> skladowaD;
    vector<double> czasy;
    double czasSym = 0.0;

public:
    Symulacja();
    Symulacja(PID pid, ARX arx, Generator gen, QObject *parent = nullptr);
    ~Symulacja();

    void uruchom();

    void startSymulacji();
    void stopSymulacji();
    void restart();

    void setPID(double wzmocnienie,
                double stalaCalkowania,
                double stalaRozniczkowania,
                double minWyjscie,
                double maxWyjscie);
    void setARXWektory(const vector<double> &wektorA, const vector<double> &wektorB);
    void setARXOpoznienie(int opoznienie);
    void setGeneratorTyp(Typ typ);
    void setGeneratorParametry(
        double amplituda, int okres, int czasAktywacji, double wartoscStala, double p);
    void setZaklocenia(double wartosc);
    void setInterwal(double i);
    double getInterwal();
    double getCzasSymulacji();
    QTimer *getTimer();
    const vector<double> &getCzasy();

    vector<double> getA();
    vector<double> getB();
    int getOpoznienie();
    double getZaklocenia();

    double wykonajKrokTest(double i);
    void resetCalki();

    void zmienCalke(bool);

    void setWzmocnienie(double i_wzmocnienie);
    void setStalaCalkowania(double i_stalaCalkowania);
    void setStalaRozniczkowania(double i_stalaRozniczkowania);
    void setMinWyjscie(double i_minWyjscie);
    void setMaxWyjscie(double i_maxWyjscie);

    void setAmplituda(double i_amplituda);
    void setOkres(int i_okres);
    void setCzasAktywacji(int i_czasAktywacji);
    void setWartoscStala(double i_wartoscStala);
    void setP(double i_p);

    void setTyp(Typ i_typ);

    double y = 0.0;

private slots:
    void wykonajKrok();

signals:
    void wykresyAktualizacja(const vector<double> &zadana,
                             const vector<double> &uchyb,
                             const vector<double> &sterowanie,
                             const vector<double> &regulowana,
                             const vector<double> &skladowaP,
                             const vector<double> &skladowaI,
                             const vector<double> &skladowaD);
};
#endif // SYMULACJA_H
