#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

enum class Ranga {
    Szeregowy,
    Pulkownik,
    General
};

struct Zolnierz {
    QString nazwisko;
    int wiek;
    Ranga stopien;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Zolnierz listaZolnierzy[20];
    int liczbaZolnierzy = 0;

    void sortuj();
    void odswiezListe();
    QString rangaToString(Ranga r);

private slots:
    void on_btn_dodajZolnierza_clicked();

    void on_btn_najmlodszy_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
