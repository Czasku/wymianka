#include "arx.h"

ARX::ARX(vector<double> wektorA, vector<double> wektorB, int opoznienie)
{
    setOpoznienie(opoznienie);
    setWektory(wektorA, wektorB);
    inicjalizujBufory();
}

ARX::ARX(vector<double> wektorA, vector<double> wektorB, int opoznienie, double zakl)
{
    setOpoznienie(opoznienie);
    setWektory(wektorA, wektorB);
    inicjalizujBufory();
    setZaklocenia(zakl);
}

void ARX::setOpoznienie(int i_opoznienie)
{
    if (i_opoznienie < 1)
        opoznienie = 1;
    else if (i_opoznienie != opoznienie) {
        opoznienie = i_opoznienie;

        std::vector<double> stareBufory = bufory;

        inicjalizujBufory();
        bufory = stareBufory;
    }
}

void ARX::setWektory(vector<double> A, vector<double> B)
{

    wektorA.clear();
    wektorB.clear();
    wektorA.resize(A.size());
    wektorB.resize(B.size());
    wektorA = A;
    wektorB = B;
}

void ARX::inicjalizujBufory()
{
    if (opoznienie < 1)
        opoznienie = 1;

    opoznienieTransportowe.clear();
    opoznienieTransportowe.assign(opoznienie, 0.0);

    historiaU.clear();
    historiaU.resize(wektorB.size(), 0.0);

    historiaY.clear();
    historiaY.resize(wektorA.size(), 0.0);
}

double ARX::Oblicz(double u)
{
    double mean = 0, stdev = zaklocenia;
    double noweZaklocenia = 0;
    if(stdev != 0)
    {
    random_device gen_los;
    mt19937 gen_plos;
    gen_plos.seed(gen_los());

    normal_distribution<double> rozkladGausa(mean, stdev);
    noweZaklocenia = rozkladGausa(gen_plos);
    }

    double wyjscie = 0.0f;

    double opoznione_u = 0.0;

    if (!opoznienieTransportowe.empty())
    {
        opoznione_u = opoznienieTransportowe.front();
        opoznienieTransportowe.pop_front();
    }
    else
    {
        qDebug() << "Błąd: opoznienieTransportowe jest puste!";
    }

        opoznienieTransportowe.push_back(u);

    historiaU.push_front(opoznione_u);

    historiaU.pop_back();

    for (int i = 0; i < std::min((int)wektorA.size(), (int)historiaY.size()); i++)
        wyjscie -= wektorA[i] * historiaY[i];

    for (int j = 0; j < std::min((int)wektorB.size(), (int)historiaU.size()); j++)
        wyjscie += wektorB[j] * historiaU[j];

    historiaY.push_front(wyjscie);
    if (historiaY.size() > wektorA.size())
        historiaY.pop_back();
#ifdef MAIN
    return wyjscie + noweZaklocenia;
#endif

#ifdef DEBUG_ARX
    return wyjscie + zaklocenia;
#endif
#ifdef DEBUG_UAR
    return wyjscie;
#endif
#ifdef DEBUG_PID
    return wyjscie;
#endif
}
void ARX::reset()
{
    opoznienie = 1.0f;
    inicjalizujBufory();
}

void ARX::setZaklocenia(double wartosc)
{
    zaklocenia = wartosc;
}

vector<double> ARX::getVecA()
{
    return wektorA;
}

vector<double> ARX::getVecB()
{
    return wektorB;
}

int ARX::getOpoznienie()
{
    return opoznienie;
}

double ARX::getZaklocenia()
{
    return zaklocenia;
}
