#ifndef DODAJZOLNIERZA_H
#define DODAJZOLNIERZA_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class DodajZolnierza;
}

class DodajZolnierza : public QDialog
{
    Q_OBJECT

public:
    explicit DodajZolnierza(QWidget *parent = nullptr);
    ~DodajZolnierza();

    Zolnierz getZolnierz();

private slots:
    void on_btn_okey_clicked();

    void on_btn_anuluj_clicked();

private:
    Ui::DodajZolnierza *ui;
};

#endif // DODAJZOLNIERZA_H
