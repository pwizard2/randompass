/*
    This file is part of Randompass.
    Copyright (c) 2014 by Will Kraft <pwizard@gmail.com>.

    Randompass 2.0 and later contains GPL3 code from RoboJournal 0.5.

    Randompass is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Randompass is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Randompass.  If not, see <http://www.gnu.org/licenses/>.
*/


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

    void on_actionNew_Password_triggered();

    void on_GeneratePassword_clicked();

    void on_actionQuit_triggered();

    void on_Tabinterface_currentChanged(int index);

private:
    Ui::MainWindow *ui;

    int DiceRoll(bool useSpecial);
    char NextDigit(bool useSpecial, int thisroll);
    QString Spawn_Next();
    void SpawnSingle();
    void SpawnMulti();
    int PasswordStrength(QString passwd);
    void AddtoHistory(QString passwd);

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
