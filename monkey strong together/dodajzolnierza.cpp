#include "dodajzolnierza.h"
#include "ui_dodajzolnierza.h"

DodajZolnierza::DodajZolnierza(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DodajZolnierza)
{
    ui->setupUi(this);
}

DodajZolnierza::~DodajZolnierza()
{
    delete ui;
}

Zolnierz DodajZolnierza::getZolnierz()
{
    Zolnierz z;
    int wiek = ui->spinbox_wiek->value();
    z.nazwisko = ui->txt_Nazwisko->toPlainText();
    z.wiek = ui->spinbox_wiek->value();
    z.stopien = static_cast<Ranga>(ui->combo_ranga->currentIndex());
    return z;
}
void DodajZolnierza::on_btn_okey_clicked()
{
    accept();
}


void DodajZolnierza::on_btn_anuluj_clicked()
{
    reject();
}

