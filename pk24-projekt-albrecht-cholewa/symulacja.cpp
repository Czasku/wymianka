#include "symulacja.h"
#include <QTimer>
#include "arx.h"
#include "generator.h"
#include "pid.h"
#include "qdebug.h"

Symulacja::Symulacja(PID pid, ARX arx, Generator gen, QObject *parent)
    : QObject(parent)
    , regulator(pid)
    , obiekt(arx)
    , generator(gen)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Symulacja::wykonajKrok);
}

Symulacja::~Symulacja()
{
    if (timer->isActive()) {
        timer->stop();
    }
    delete timer;
}

Symulacja::Symulacja()
    : regulator(PID(0.5, 10, 0.2, -100, 100))
    , obiekt(ARX({-0.4, 0, 0}, {0.6, 0, 0}, 1))
    , generator(Generator(Typ::skokowy, 1.0, 10, 0, 1.0, 0.5))
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Symulacja::wykonajKrok);
    interwal = 0.1;
}

void Symulacja::startSymulacji()
{
    timer->start();
    qDebug() << "symulacja rozpoczęta.";
}
void Symulacja::stopSymulacji()
{
    if (timer->isActive()) {
        timer->stop();
        qDebug() << "Symulacja zatrzymana.";
    }
}
void Symulacja::restart()
{
    y = 0.0;
    czasSym = 0.0;
    czasy.clear();
    regulator.Reset();
    sterowanie.clear();
    zadana.clear();
    uchyb.clear();
    regulowana.clear();
    skladowaP.clear();
    skladowaI.clear();
    skladowaD.clear();
    sterowanie.clear();
    obiekt.reset();
}

void Symulacja::wykonajKrok()
{
    const int MAX_PUNKTY = 100;
    const int RESET_PUNKTY = 50;
    if (sterowanie.size() > MAX_PUNKTY) {
        czasy.erase(czasy.begin(), czasy.begin() + RESET_PUNKTY);
        sterowanie.erase(sterowanie.begin(), sterowanie.begin() + RESET_PUNKTY);
        zadana.erase(zadana.begin(), zadana.begin() + RESET_PUNKTY);
        regulowana.erase(regulowana.begin(), regulowana.begin() + RESET_PUNKTY);
        uchyb.erase(uchyb.begin(), uchyb.begin() + RESET_PUNKTY);
        skladowaP.erase(skladowaP.begin(), skladowaP.begin() + RESET_PUNKTY);
        skladowaI.erase(skladowaI.begin(), skladowaI.begin() + RESET_PUNKTY);
        skladowaD.erase(skladowaD.begin(), skladowaD.begin() + RESET_PUNKTY);
    }

    czasSym += interwal;
    czasy.push_back(czasSym);

    double w = generator.Generuj(regulowana.size());
    double e = regulator.Sumator(w, y);
    double u = regulator.ObliczSterowanie(e);
    y = obiekt.Oblicz(u);

    zadana.push_back(w);
    uchyb.push_back(e);
    sterowanie.push_back(u);
    regulowana.push_back(y);
    double p = regulator.getProporcjonalne();
    double i = regulator.getCalka();
    double d = regulator.getRozniczka();

    skladowaP.push_back(p);
    skladowaI.push_back(i);
    skladowaD.push_back(d);
    emit wykresyAktualizacja(zadana, uchyb, sterowanie, regulowana, skladowaP, skladowaI, skladowaD);
}

double Symulacja::wykonajKrokTest(double i)
{
    double e = regulator.Sumator(i, y);
    double u = regulator.ObliczSterowanie(e);
    y = obiekt.Oblicz(u);

    return y;
}

void Symulacja::setPID(double wzmocnienie,
                       double stalaCalkowania,
                       double stalaRozniczkowania,
                       double minWyjscie,
                       double maxWyjscie)
{
    regulator.setRegulator(wzmocnienie, stalaCalkowania, stalaRozniczkowania, minWyjscie, maxWyjscie);
}

void Symulacja::setARXWektory(const vector<double> &wektorA, const vector<double> &wektorB)
{
    obiekt.setWektory(wektorA, wektorB);
}

void Symulacja::setARXOpoznienie(int opoznienie)
{
    obiekt.setOpoznienie(opoznienie);
}

void Symulacja::setGeneratorTyp(Typ typ)
{
    generator.setTyp(typ);
}

void Symulacja::setGeneratorParametry(
    double amplituda, int okres, int czasAktywacji, double wartoscStala, double p)
{
    generator.setParametry(amplituda, okres, czasAktywacji, wartoscStala, p);
}
void Symulacja::setZaklocenia(double wartosc)
{
    obiekt.setZaklocenia(wartosc);
}

QTimer *Symulacja::getTimer()
{
    return timer;
}
void Symulacja::setInterwal(double i)
{
    interwal = i;
}
double Symulacja::getInterwal()
{
    return interwal;
}
double Symulacja::getCzasSymulacji()
{
    return czasSymulacji;
}
const vector<double> &Symulacja::getCzasy()
{
    return czasy;
}

void Symulacja::resetCalki()
{
    regulator.resetCalki();
}

void Symulacja::zmienCalke(bool b)
{
    regulator.zmienTypLiczeniaCalki(b);
}

vector<double> Symulacja::getA()
{
    return obiekt.getVecA();
}

vector<double> Symulacja::getB()
{
    return obiekt.getVecB();
}

int Symulacja::getOpoznienie()
{
    return obiekt.getOpoznienie();
}

double Symulacja::getZaklocenia()
{
    return obiekt.getZaklocenia();
}

void Symulacja::setWzmocnienie(double i_wzmocnienie) {
    regulator.setWzmocnienie(i_wzmocnienie);
}

void Symulacja::setStalaCalkowania(double i_stalaCalkowania) {
    regulator.setStalaCalkowania(i_stalaCalkowania);
}

void Symulacja::setStalaRozniczkowania(double i_stalaRozniczkowania) {
    regulator.setStalaRozniczkowania(i_stalaRozniczkowania);
}

void Symulacja::setMinWyjscie(double i_minWyjscie) {
    regulator.setMinWyjscie(i_minWyjscie);
}

void Symulacja::setMaxWyjscie(double i_maxWyjscie) {
    regulator.setMaxWyjscie(i_maxWyjscie);
}

void Symulacja::setAmplituda(double i_amplituda) {
    generator.setAmplituda(i_amplituda);
}

void Symulacja::setOkres(int i_okres) {
    generator.setOkres(i_okres);
}

void Symulacja::setCzasAktywacji(int i_czasAktywacji) {
    generator.setCzasAktywacji(i_czasAktywacji);
}

void Symulacja::setWartoscStala(double i_wartoscStala) {
    generator.setWartoscStala(i_wartoscStala);
}

void Symulacja::setP(double i_p) {
    generator.setP(i_p);
}

void Symulacja::setTyp(Typ i_typ)
{
    generator.setTyp(i_typ);
};

