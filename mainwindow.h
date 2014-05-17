#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    int DiceRoll(bool useSpecial);
    char NextDigit(bool useSpecial, int thisroll);
    QString Spawn_Next();
    void SpawnSingle();
    void SpawnMulti();
    int PasswordStrength(QString passwd);

    // total passwords generated
    int numPasswds;

    // booleans that make sure that every password has everything in it that it should
    bool hasUpper;
    bool hasLower;
    bool hasNum;
    bool hasSym;

    // keep track of how many instances of each glyph type are in the password;
    int symbolCount;
    int upperCount;
    int lowerCount;
    int numberCount;


    //keep track of last glyphs created

    char lastUpper;
    char lastLower;
    char lastNumber;
    char lastSymbol;
};

#endif // MAINWINDOW_H
