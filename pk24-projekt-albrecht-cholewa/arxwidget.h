#ifndef ARXWIDGET_H
#define ARXWIDGET_H

#include <QVector>
#include <QDialog>

namespace Ui {
class ArxWidget;
}

class ArxWidget : public QDialog
{
    Q_OBJECT

public:
    explicit ArxWidget(const QVector<double> &A, const QVector<double> &B, int opoznienie, double zaklocenia);
    ~ArxWidget();

signals:
    void ZmianaARX(const QVector<double> &wektorA,
                   const QVector<double> &wektorB,
                   int opoznienie,
                   double zaklocenia);

private slots:
    void on_btnDodajWiersz_clicked();
    void on_btnUsunWiersz_clicked();
    void on_btnZapisz_clicked();

private:
    Ui::ArxWidget *ui;

    void initializeTable();

    void ustawDane(const QVector<double> &A, const QVector<double> &B, int opoznienie, double zaklocenia);
};

#endif // ARXWIDGET_H
