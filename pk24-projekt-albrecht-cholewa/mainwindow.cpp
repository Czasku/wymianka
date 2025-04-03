#include "mainwindow.h"
#include <QDebug>
#include <QPalette>
#include "arx.h"
#include "arxwidget.h"
#include "generator.h"
#include "glowne.h"
#include "pid.h"
#include "symulacja.h"
#include "ui_mainwindow.h"
#include <algorithm>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , Symulator()
{
    ui->setupUi(this);
    ui->Stop->setEnabled(false);
    this->setWindowTitle("Symulator UAR");

    inicjalizujWszystkieWykresy();

    connect(&Symulator, &Symulacja::wykresyAktualizacja, this, &MainWindow::aktualizujWykresy);

    ui->Reset->setEnabled(false);

    ui->wzmocnienie->setValue(0.5);
    ui->stalaCalkowania->setValue(10);
    ui->stalaRozniczkowania->setValue(0.2);
    ui->minWyjscie->setMinimum(-500);
    ui->maxWyjscie->setMaximum(500);
    ui->minWyjscie->setValue(-100);
    ui->maxWyjscie->setValue(100);

    ui->amplituda->setValue(3);
    ui->okres->setValue(5);
    ui->czasAktywacji->setValue(0);
    ui->p->setValue(0.5);
    ui->p->setSingleStep(0.1);
    ui->Interwal->setValue(0.1);
    ui->Interwal->setSingleStep(0.1);
    ui->wartoscStala->setValue(1);
    ui->amplituda->setDisabled(true);
    ui->okres->setDisabled(true);
    ui->p->setDisabled(true);
    ui->okres->setMinimum(1);

    this->setStyleSheet("background-color: rgb(225,225,225); color: black;");
    ui->Kolor->setCheckable(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Start_clicked()
{
    ui->Start->setEnabled(false);
    ui->Reset->setEnabled(false);
    ui->Stop->setEnabled(true);

    Symulator.startSymulacji();
}

void MainWindow::on_Stop_clicked()
{
    ui->Start->setEnabled(true);
    ui->Reset->setEnabled(true);
    ui->Stop->setEnabled(false);

    Symulator.stopSymulacji();
}

void MainWindow::on_Reset_clicked()
{
    Symulator.restart();

    series->clear();
    seriesP->clear();
    seriesI->clear();
    seriesD->clear();
    seriesUchyb->clear();
    seriesZadana->clear();
    seriesRegulowana->clear();

    chart->axes(Qt::Horizontal).first()->setRange(0, 1);
    chart->axes(Qt::Vertical).first()->setRange(0, 1);

    chartUchyb->axes(Qt::Horizontal).first()->setRange(0, 1);
    chartUchyb->axes(Qt::Vertical).first()->setRange(0, 1);

    chartZadanaRegulowana->axes(Qt::Horizontal).first()->setRange(0, 1);
    chartZadanaRegulowana->axes(Qt::Vertical).first()->setRange(0, 1);

    chartP->axes(Qt::Horizontal).first()->setRange(0, 1);
    chartP->axes(Qt::Vertical).first()->setRange(0, 1);

    chartI->axes(Qt::Horizontal).first()->setRange(0, 1);
    chartI->axes(Qt::Vertical).first()->setRange(0, 1);

    chartD->axes(Qt::Horizontal).first()->setRange(0, 1);
    chartD->axes(Qt::Vertical).first()->setRange(0, 1);
}

void MainWindow::inicjalizujWszystkieWykresy()
{
    // Główny wykres (sterowanie PID)
    chart = new QChart();
    series = new QLineSeries();
    chartView = new QChartView(chart, ui->chartWidget);

    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Wykres PID");
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *layout = new QVBoxLayout(ui->chartWidget);
    layout->addWidget(chartView);
    ui->chartWidget->setLayout(layout);
    chart->legend()->hide();

    // Wykres P (Proporcjonalny)
    chartP = new QChart();
    seriesP = new QLineSeries();
    chartViewP = new QChartView(chartP, ui->wykresP);

    chartP->addSeries(seriesP);
    chartP->createDefaultAxes();
    chartP->setTitle("Wykres Proporcjonalny (P)");
    chartViewP->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *layoutP = new QVBoxLayout(ui->wykresP);
    layoutP->addWidget(chartViewP);
    ui->wykresP->setLayout(layoutP);
    chartP->legend()->hide();

    // Wykres I (Całkujący)
    chartI = new QChart();
    seriesI = new QLineSeries();
    chartViewI = new QChartView(chartI, ui->wykresI);

    chartI->addSeries(seriesI);
    chartI->createDefaultAxes();
    chartI->setTitle("Wykres Całkujący (I)");
    chartViewI->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *layoutI = new QVBoxLayout(ui->wykresI);
    layoutI->addWidget(chartViewI);
    ui->wykresI->setLayout(layoutI);
    chartI->legend()->hide();

    // Wykres D (Różniczkujący)
    chartD = new QChart();
    seriesD = new QLineSeries();
    chartViewD = new QChartView(chartD, ui->wykresD);

    chartD->addSeries(seriesD);
    chartD->createDefaultAxes();
    chartD->setTitle("Wykres Różniczkujący (D)");
    chartViewD->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *layoutD = new QVBoxLayout(ui->wykresD);
    layoutD->addWidget(chartViewD);
    ui->wykresD->setLayout(layoutD);
    chartD->legend()->hide();

    // Wykres Uchyb
    chartUchyb = new QChart();
    seriesUchyb = new QLineSeries();
    chartViewUchyb = new QChartView(chartUchyb, ui->wykresUchyb);

    chartUchyb->addSeries(seriesUchyb);
    chartUchyb->createDefaultAxes();
    chartUchyb->setTitle("Wykres Uchyb");
    chartViewUchyb->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *layoutUchyb = new QVBoxLayout(ui->wykresUchyb);
    layoutUchyb->addWidget(chartViewUchyb);
    ui->wykresUchyb->setLayout(layoutUchyb);
    chartUchyb->legend()->hide();

    // Wykres Zadana i Regulowana
    chartZadanaRegulowana = new QChart();
    seriesRegulowana = new QLineSeries();
    seriesZadana = new QLineSeries();

    seriesZadana->setName("Wartość Zadana");
    seriesRegulowana->setName("Wartość Regulowana");

    chartZadanaRegulowana->addSeries(seriesRegulowana);
    chartZadanaRegulowana->addSeries(seriesZadana);
    chartZadanaRegulowana->createDefaultAxes();
    chartZadanaRegulowana->setTitle("Wartość Zadana i Regulowana");

    chartViewZadanaRegulowana = new QChartView(chartZadanaRegulowana, ui->wykresZadanaRegulowana);
    chartViewZadanaRegulowana->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *layoutZadanaRegulowana = new QVBoxLayout(ui->wykresZadanaRegulowana);
    layoutZadanaRegulowana->addWidget(chartViewZadanaRegulowana);
    ui->wykresZadanaRegulowana->setLayout(layoutZadanaRegulowana);

    chart->axes(Qt::Horizontal).first()->setTitleText("Czas [s]");
    chart->axes(Qt::Vertical).first()->setTitleText("Wartość");

    chartP->axes(Qt::Horizontal).first()->setTitleText("Czas [s]");
    chartP->axes(Qt::Vertical).first()->setTitleText("Wartość P");

    chartI->axes(Qt::Horizontal).first()->setTitleText("Czas [s]");
    chartI->axes(Qt::Vertical).first()->setTitleText("Wartość I");

    chartD->axes(Qt::Horizontal).first()->setTitleText("Czas [s]");
    chartD->axes(Qt::Vertical).first()->setTitleText("Wartość D");

    chartUchyb->axes(Qt::Horizontal).first()->setTitleText("Czas [s]");
    chartUchyb->axes(Qt::Vertical).first()->setTitleText("Uchyb");

    chartZadanaRegulowana->axes(Qt::Horizontal).first()->setTitleText("Czas [s]");
    chartZadanaRegulowana->axes(Qt::Vertical).first()->setTitleText("Wartość Zadana i Regulowana");

}

void MainWindow::aktualizujWykresy(const vector<double> &zadana,
                                   const vector<double> &uchyb,
                                   const vector<double> &sterowanie,
                                   const vector<double> &regulowana,
                                   const vector<double> &skladowaP,
                                   const vector<double> &skladowaI,
                                   const vector<double> &skladowaD)
{
    const vector<double> &czasy = Symulator.getCzasy();
    const int maxWartosc = 50;
    int dataSize = static_cast<int>(sterowanie.size());
    double interwal = Symulator.getInterwal();
    int startIndex = std::max(0, dataSize - maxWartosc);
    series->clear();

    for (int i = startIndex; i < dataSize; ++i) {
        double t = czasy[i];
        double val = sterowanie[i];
        series->append(t, val);
    }

    if (dataSize > 0) {
        double xMin = czasy[startIndex];
        double xMax = czasy[dataSize - 1];

        auto minVal = *std::min_element(sterowanie.begin() + startIndex, sterowanie.end());
        auto maxVal = *std::max_element(sterowanie.begin() + startIndex, sterowanie.end());
        double range = maxVal - minVal;
        if (range < 1e-9)
            range = 0.01;
        double margin = 0.1 * range;

        double yMin = minVal - margin;
        double yMax = maxVal + margin;

        chart->axes(Qt::Horizontal).first()->setRange(xMin, xMax);
        chart->axes(Qt::Vertical).first()->setRange(yMin, yMax);
    } else {
        chart->axes(Qt::Horizontal).first()->setRange(0, 1);
        chart->axes(Qt::Vertical).first()->setRange(0, 1);
    }

    seriesP->clear();
    int pSize = static_cast<int>(skladowaP.size());
    int startIndexP = std::max(0, pSize - maxWartosc);

    for (int i = startIndexP; i < pSize; ++i) {
        double t = czasy[i];
        double val = skladowaP[i];
        seriesP->append(t, val);
    }

    if (pSize > 0) {
        double xMin = czasy[startIndexP];
        double xMax = czasy[pSize - 1];

        auto minVal = *std::min_element(skladowaP.begin() + startIndexP, skladowaP.end());
        auto maxVal = *std::max_element(skladowaP.begin() + startIndexP, skladowaP.end());
        double range = maxVal - minVal;
        if (range < 1e-9)
            range = 0.01;
        double margin = 0.1 * range;

        chartP->axes(Qt::Horizontal).first()->setRange(xMin, xMax);
        chartP->axes(Qt::Vertical).first()->setRange(minVal - margin, maxVal + margin);
    } else {
        chartP->axes(Qt::Horizontal).first()->setRange(0, 1);
        chartP->axes(Qt::Vertical).first()->setRange(-1, 1);
    }

    seriesI->clear();
    int iSize = static_cast<int>(skladowaI.size());
    int startIndexI = std::max(0, iSize - maxWartosc);

    for (int i = startIndexI; i < iSize; ++i) {
        double t = czasy[i];
        double val = skladowaI[i];
        seriesI->append(t, val);
    }

    if (iSize > 0) {
        double xMin = czasy[startIndexI];
        double xMax = czasy[iSize - 1];

        auto minVal = *std::min_element(skladowaI.begin() + startIndexI, skladowaI.end());
        auto maxVal = *std::max_element(skladowaI.begin() + startIndexI, skladowaI.end());
        double range = maxVal - minVal;
        if (range < 1e-9)
            range = 0.01;
        double margin = 0.1 * range;

        chartI->axes(Qt::Horizontal).first()->setRange(xMin, xMax);
        chartI->axes(Qt::Vertical).first()->setRange(minVal - margin, maxVal + margin);
    } else {
        chartI->axes(Qt::Horizontal).first()->setRange(0, 1);
        chartI->axes(Qt::Vertical).first()->setRange(-1, 1);
    }

    seriesD->clear();
    int dSize = static_cast<int>(skladowaD.size());
    int startIndexD = std::max(0, dSize - maxWartosc);

    for (int i = startIndexD; i < dSize; ++i) {
        double t = czasy[i];
        double val = skladowaD[i];
        seriesD->append(t, val);
    }

    if (dSize > 0) {
        double xMin = czasy[startIndexD];
        double xMax = czasy[dSize - 1];

        auto minVal = *std::min_element(skladowaD.begin() + startIndexD, skladowaD.end());
        auto maxVal = *std::max_element(skladowaD.begin() + startIndexD, skladowaD.end());
        double range = maxVal - minVal;
        if (range < 1e-9)
            range = 0.01;
        double margin = 0.1 * range;

        chartD->axes(Qt::Horizontal).first()->setRange(xMin, xMax);
        chartD->axes(Qt::Vertical).first()->setRange(minVal - margin, maxVal + margin);
    } else {
        chartD->axes(Qt::Horizontal).first()->setRange(0, 1);
        chartD->axes(Qt::Vertical).first()->setRange(-1, 1);
    }
    seriesZadana->clear();
    seriesRegulowana->clear();

    int zSize = static_cast<int>(zadana.size());
    int startIndexZ = std::max(0, zSize - maxWartosc);

    for (int i = startIndexZ; i < zSize; ++i) {
        double t = czasy[i];
        seriesZadana->append(t, zadana[i]);
    }

    int rSize = static_cast<int>(regulowana.size());
    int startIndexR = std::max(0, rSize - maxWartosc);

    for (int i = startIndexR; i < rSize; ++i) {
        double t = czasy[i];
        seriesRegulowana->append(t, regulowana[i]);
    }

    if (zSize > 0 && rSize > 0) {
        double xMin = czasy[startIndexZ];
        double xMax = czasy[zSize - 1];

        double minZad = *std::min_element(zadana.begin() + startIndexZ, zadana.end());
        double maxZad = *std::max_element(zadana.begin() + startIndexZ, zadana.end());
        double minReg = *std::min_element(regulowana.begin() + startIndexR, regulowana.end());
        double maxReg = *std::max_element(regulowana.begin() + startIndexR, regulowana.end());
        double minVal = std::min(minZad, minReg);
        double maxVal = std::max(maxZad, maxReg);

        double range = maxVal - minVal;
        if (range < 1e-9)
            range = 0.01;
        double margin = 0.1 * range;

        chartZadanaRegulowana->axes(Qt::Horizontal).first()->setRange(xMin, xMax);
        chartZadanaRegulowana->axes(Qt::Vertical).first()->setRange(minVal - margin, maxVal + margin);
    } else {
        chartZadanaRegulowana->axes(Qt::Horizontal).first()->setRange(0, 1);
        chartZadanaRegulowana->axes(Qt::Vertical).first()->setRange(-1, 1);
    }

    seriesUchyb->clear();
    int uSize = static_cast<int>(uchyb.size());
    int startIndexU = std::max(0, uSize - maxWartosc);

    for (int i = startIndexU; i < uSize; ++i) {
        double t = czasy[i];
        seriesUchyb->append(t, uchyb[i]);
    }

    if (uSize > 0) {
        double xMin = czasy[startIndexU];
        double xMax = czasy[uSize - 1];

        auto minVal = *std::min_element(uchyb.begin() + startIndexU, uchyb.end());
        auto maxVal = *std::max_element(uchyb.begin() + startIndexU, uchyb.end());
        double range = maxVal - minVal;
        if (range < 1e-9)
            range = 0.01;
        double margin = 0.1 * range;

        chartUchyb->axes(Qt::Horizontal).first()->setRange(xMin, xMax);
        chartUchyb->axes(Qt::Vertical).first()->setRange(minVal - margin, maxVal + margin);
    } else {
        chartUchyb->axes(Qt::Horizontal).first()->setRange(0, 1);
        chartUchyb->axes(Qt::Vertical).first()->setRange(-1, 1);
    }
}

void MainWindow::on_zapiszInne_clicked()
{
    Typ typ = static_cast<Typ>(ui->typComboBox->currentIndex());
    double amplituda = ui->amplituda->value();
    int okres = ui->okres->value();
    int czasAktywacji = ui->czasAktywacji->value();
    double wartoscStala = ui->wartoscStala->value();
    double p = ui->p->value();
    double interwal = ui->Interwal->value();

    Symulator.setInterwal(interwal);
    Symulator.setGeneratorTyp(typ);
    Symulator.setGeneratorParametry(amplituda, okres, czasAktywacji, wartoscStala, p);

    Symulator.getTimer()->setInterval(interwal * 1000);

    qDebug() << "Wprowadzone dane:";
    qDebug() << "Typ =" << static_cast<int>(typ);
    qDebug() << "Amplituda =" << amplituda;
    qDebug() << "Okres =" << okres;
    qDebug() << "Czas aktywacji =" << czasAktywacji;
    qDebug() << "Wartosc zadana (stala) =" << wartoscStala;
    qDebug() << "p =" << p;
    qDebug() << "interwal =" << interwal;
}

void MainWindow::on_ZapiszARX_clicked()
{

    vector<double> wecA = Symulator.getA();
    vector<double> wecB = Symulator.getB();

    QVector<double> A(wecA.begin(), wecA.end());
    QVector<double> B(wecB.begin(), wecB.end());

    double zaklocenia = Symulator.getZaklocenia();
    int opoznienie = Symulator.getOpoznienie();

    ArxWidget *aw = new ArxWidget(A, B, opoznienie, zaklocenia);
    aw->setWindowFlag(Qt::Window);

    connect(aw, &ArxWidget::ZmianaARX,
            this, [this, aw](const QVector<double> &A,
                       const QVector<double> &B,
                       int opoznienie,
                       double zaklocenia)
            {
                vector<double> tempA(A.begin(), A.end());
                vector<double> tempB(B.begin(), B.end());

                Symulator.setARXWektory(tempA, tempB);
                Symulator.setARXOpoznienie(opoznienie);
                Symulator.setZaklocenia(zaklocenia);

                //aw->close();
            });

    aw->exec();
}

void MainWindow::on_ZapiszPID_clicked()
{
    double wzmocnienie = ui->wzmocnienie->value();
    double stalaCalkowania = ui->stalaCalkowania->value();
    double stalaRozniczkowania = ui->stalaRozniczkowania->value();
    double minWyjscie = ui->minWyjscie->value();
    double maxWyjscie = ui->maxWyjscie->value();

    Symulator.setPID(wzmocnienie, stalaCalkowania, stalaRozniczkowania, minWyjscie, maxWyjscie);
}

void MainWindow::odblokuj()
{
    ui->amplituda->setDisabled(false);
    ui->okres->setDisabled(false);
    ui->czasAktywacji->setDisabled(false);
    ui->wartoscStala->setDisabled(false);
    ui->p->setDisabled(false);
    ui->Interwal->setDisabled(false);
}

void MainWindow::on_typComboBox_activated(int index)
{
    switch (index) {
    default:
        odblokuj();
        break;
    case 0: // Skokowy
        odblokuj();
        ui->amplituda->setDisabled(true);
        ui->okres->setDisabled(true);
        ui->p->setDisabled(true);
        break;
    case 1: // Sinusoidalny
        odblokuj();
        ui->czasAktywacji->setDisabled(true);
        ui->wartoscStala->setDisabled(true);
        ui->p->setDisabled(true);
        break;
    case 2: // Prostokątny
        odblokuj();
        ui->czasAktywacji->setDisabled(true);
        ui->wartoscStala->setDisabled(true);
        break;
    }
}

void MainWindow::on_Kolor_toggled(bool checked)
{
    if (checked) {
        ui->Kolor->setText("Tryb Jasny");
        this->setStyleSheet("background-color: rgb(33,33,33); color: white;");
    } else {
        ui->Kolor->setText("Tryb Ciemny");
        this->setStyleSheet("background-color: rgb(225,225,225); color: black;");
    }
}

void MainWindow::on_usunCalke_clicked()
{
    Symulator.resetCalki();
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    Symulator.zmienCalke(!!arg1);
}

void MainWindow::on_wzmocnienie_editingFinished()
{
    double wzmocnienie = ui->wzmocnienie->value();
    Symulator.setWzmocnienie(wzmocnienie);
}


void MainWindow::on_stalaCalkowania_editingFinished()
{
    double stalaCalkowania = ui->stalaCalkowania->value();
    Symulator.setStalaCalkowania(stalaCalkowania);
}


void MainWindow::on_stalaRozniczkowania_editingFinished()
{
    double stalaRozniczkowania = ui->stalaRozniczkowania->value();
    Symulator.setStalaRozniczkowania(stalaRozniczkowania);
}


void MainWindow::on_minWyjscie_editingFinished()
{
    double minWyjscie = ui->minWyjscie->value();
    Symulator.setMinWyjscie(minWyjscie);
}


void MainWindow::on_maxWyjscie_editingFinished()
{
    double maxWyjscie = ui->maxWyjscie->value();
    Symulator.setMaxWyjscie(maxWyjscie);
}


void MainWindow::on_amplituda_editingFinished()
{
    double amplituda = ui->amplituda->value();
    Symulator.setAmplituda(amplituda);
}


void MainWindow::on_okres_editingFinished()
{;
    int okres = ui->okres->value();
    Symulator.setOkres(okres);
}


void MainWindow::on_czasAktywacji_editingFinished()
{
    int czasAktywacji = ui->czasAktywacji->value();
    Symulator.setCzasAktywacji(czasAktywacji);
}


void MainWindow::on_wartoscStala_editingFinished()
{
    double wartoscStala = ui->wartoscStala->value();
    Symulator.setWartoscStala(wartoscStala);
}


void MainWindow::on_p_editingFinished()
{
    double p = ui->p->value();
    Symulator.setP(p);
}


void MainWindow::on_Interwal_editingFinished()
{
    double interwal = ui->Interwal->value();
    Symulator.setInterwal(interwal);
}




void MainWindow::on_typComboBox_currentIndexChanged(int index)
{
    Typ typ = static_cast<Typ>(index);
    Symulator.setTyp(typ);
}

