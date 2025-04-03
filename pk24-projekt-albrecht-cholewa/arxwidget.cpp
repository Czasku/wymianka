#include "arxwidget.h"
#include "ui_arxwidget.h"

#include <QTableWidgetItem>
#include <QMessageBox>
#include <QDebug>

ArxWidget::ArxWidget(const QVector<double> &A, const QVector<double> &B, int opoznienie, double zaklocenia)
    : ui(new Ui::ArxWidget)
{
    ui->setupUi(this);

    ui->tabelaWektorow->setColumnCount(2);
    QStringList naglowki;
    naglowki << "A" << "B";
    ui->tabelaWektorow->setHorizontalHeaderLabels(naglowki);

    ui->tabelaWektorow->setRowCount(3);
    initializeTable();

    ui->spinUsunWiersz->setRange(1, ui->tabelaWektorow->rowCount());

    ustawDane(A, B, opoznienie, zaklocenia);
}

ArxWidget::~ArxWidget()
{
    delete ui;
}

void ArxWidget::initializeTable()
{
    for (int r = 0; r < ui->tabelaWektorow->rowCount(); ++r) {
        ui->tabelaWektorow->setItem(r, 0, new QTableWidgetItem(""));
        ui->tabelaWektorow->setItem(r, 1, new QTableWidgetItem(""));
    }
}

void ArxWidget::ustawDane(const QVector<double> &A, const QVector<double> &B, int opoznienie, double zaklocenia)
{
    int liczbaWierszy = A.size() > 3 ? A.size() : 3;
    ui->tabelaWektorow->setRowCount(liczbaWierszy);

    for (int r = 0; r < A.size(); ++r) {
        ui->tabelaWektorow->setItem(r, 0, new QTableWidgetItem(QString::number(A[r])));
        ui->tabelaWektorow->setItem(r, 1, new QTableWidgetItem(QString::number(B[r])));
    }

    for (int r = A.size(); r < liczbaWierszy; ++r) {
        ui->tabelaWektorow->setItem(r, 0, new QTableWidgetItem(""));
        ui->tabelaWektorow->setItem(r, 1, new QTableWidgetItem(""));
    }

    ui->spinOpoznienie->setValue(opoznienie);
    ui->spinZaklocenia->setValue(zaklocenia);
    ui->spinUsunWiersz->setRange(1, ui->tabelaWektorow->rowCount());
}

void ArxWidget::on_btnDodajWiersz_clicked()
{
    int wiersz = ui->tabelaWektorow->rowCount();
    ui->tabelaWektorow->insertRow(wiersz);
    ui->tabelaWektorow->setItem(wiersz, 0, new QTableWidgetItem("0"));
    ui->tabelaWektorow->setItem(wiersz, 1, new QTableWidgetItem("0"));
    ui->spinUsunWiersz->setRange(1, ui->tabelaWektorow->rowCount());
}

void ArxWidget::on_btnUsunWiersz_clicked()
{
    if (ui->tabelaWektorow->rowCount() <= 3) {
        QMessageBox::warning(this, "Błąd", "Musi pozostać co najmniej 3 wiersze.");
        return;
    }
    int wierszDoUsuniecia = ui->spinUsunWiersz->value() - 1;
    if (wierszDoUsuniecia >= 0 && wierszDoUsuniecia < ui->tabelaWektorow->rowCount()) {
        ui->tabelaWektorow->removeRow(wierszDoUsuniecia);
        ui->spinUsunWiersz->setRange(1, ui->tabelaWektorow->rowCount());
    }
}

void ArxWidget::on_btnZapisz_clicked()
{
    QVector<double> A, B;
    int rows = ui->tabelaWektorow->rowCount();
    if (rows < 3) {
        QMessageBox::warning(this, "Błąd", "Musi być co najmniej 3 wiersze!");
        return;
    }

    for (int r = 0; r < rows; r++) {
        QTableWidgetItem *itemA = ui->tabelaWektorow->item(r, 0);
        QTableWidgetItem *itemB = ui->tabelaWektorow->item(r, 1);
        if (!itemA || !itemB) {
            QMessageBox::warning(this, "Błąd", QString("Pusta komórka w wierszu %1").arg(r+1));
            return;
        }
        bool okA = false, okB = false;
        double valA = itemA->text().toDouble(&okA);
        double valB = itemB->text().toDouble(&okB);
        if (!okA || !okB) {
            QMessageBox::warning(this, "Błąd konwersji", QString("Niepoprawna liczba w wierszu %1").arg(r+1));
            return;
        }
        A.push_back(valA);
        B.push_back(valB);
    }

    int opoznienie = ui->spinOpoznienie->value();
    double zaklocenia = ui->spinZaklocenia->value();
    emit ZmianaARX(A, B, opoznienie, zaklocenia);
    close();
}
