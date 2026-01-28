#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dodajzolnierza.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sortuj()
{
    for (int i = 0; i < liczbaZolnierzy - 1; ++i) {
        for (int j = 0; j < liczbaZolnierzy - i - 1; ++j) {
            if (listaZolnierzy[j].wiek < listaZolnierzy[j + 1].wiek) {
                Zolnierz tmp = listaZolnierzy[j];
                listaZolnierzy[j] = listaZolnierzy[j + 1];
                listaZolnierzy[j + 1] = tmp;
            }
        }
    }
}

void MainWindow::odswiezListe()
{
    ui->ListaZolnierzy->clear();

    for (int i = 0; i < liczbaZolnierzy; i++) {
        ui->ListaZolnierzy->addItem(
            rangaToString(listaZolnierzy[i].stopien) + " " +
            listaZolnierzy[i].nazwisko + " (" +
            QString::number(listaZolnierzy[i].wiek) + " lat)"
            );
    }
}

QString MainWindow::rangaToString(Ranga r)
{
    switch (r) {
    case Ranga::Szeregowy: return "Szeregowy";
    case Ranga::Pulkownik: return "Pułkownik";
    case Ranga::General:   return "Generał";
    }
    return "";
}


void MainWindow::on_btn_dodajZolnierza_clicked()
{
    DodajZolnierza dialog(this);

    if(liczbaZolnierzy >= 20) return;

    if(dialog.exec() == QDialog::Accepted)
    {
        listaZolnierzy[liczbaZolnierzy++] = dialog.getZolnierz();
        sortuj();
        odswiezListe();
    }
}


void MainWindow::on_btn_najmlodszy_clicked()
{
    Ranga r = static_cast<Ranga>(ui->comboBox->currentIndex());

    Zolnierz *najmlodszy = nullptr;

    for (int i = 0; i < liczbaZolnierzy; ++i) {
        if (listaZolnierzy[i].stopien == r) {
            if (!najmlodszy || listaZolnierzy[i].wiek < najmlodszy->wiek) {
                najmlodszy = &listaZolnierzy[i];
            }
        }
    }

    if (najmlodszy) {
        ui->WyswietlNajmlodszego->setText(
            rangaToString(r) + " " +
            najmlodszy->nazwisko + ", " +
            "wiek: " + QString::number(najmlodszy->wiek)
            );
    } else {
        ui->WyswietlNajmlodszego->setText(
            "Brak żołnierzy tej rangi"
            );
    }
}
