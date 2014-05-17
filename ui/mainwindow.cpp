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


#include "ui/mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <qmessagebox.h>
#include <ctime>
#include <QListWidgetItem>
#include <QFont>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    // total passwords generated
    numPasswds=0;

    // booleans that make sure that every password has everything in it that it should
    hasUpper=0;
    hasLower=0;
    hasNum=0;
    hasSym=0;

    // keep track of how many instances of each glyph type are in the password;
    symbolCount=0;
    upperCount=0;
    lowerCount=0;
    numberCount=0;


    //keep track of last glyphs created
    lastUpper='\0';
    lastLower='\0';
    lastNumber='\0';
    lastSymbol='\0';

    // Version 1.1 bugfix:
    // seed the random number generator based on current systime.
    // This fixes the Windows bug from v1.0 where the password sequence is always the same.
    // Linux was better at handling this for some reason so that's why it wasn't done for 1.0 release.
    srand(time(0));


}

MainWindow::~MainWindow()
{
    delete ui;
}

//################################################################################################################

int MainWindow::DiceRoll(bool useSpecial){
    using namespace std;

    int thisroll='\0';

    if(useSpecial){
        thisroll=rand() %4;
    }
    else{
        thisroll=rand() %3;
    }


    return thisroll;
}

//################################################################################################################

char MainWindow::NextDigit(bool useSpecial, int thisroll){
    using namespace std;
    char nextchar='\0';
    int choice;

    char seedupper[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char seedlower[]="abcdefghijklmnopqrstuvwxyz";
    char seednumber[]="0123456789";
    char seedsymbol[]="~`!@#$%^&*()-_+=:;?/"; // count=20

    if(useSpecial){
        if(thisroll==0){


            choice=rand() %26;
            nextchar=seedupper[choice];

            while(nextchar==lastSymbol){
                choice=rand() %20;
                nextchar=seedupper[choice];
            }

            hasUpper=1;
            upperCount++;

            lastUpper=nextchar;

        }

        else if(thisroll==1){

            choice=rand() %26;
            nextchar=seedlower[choice];

            while(nextchar==lastSymbol){
                choice=rand() %20;
                nextchar=seedlower[choice];
            }

            hasLower=1;
            lowerCount++;

            lastLower=nextchar;


        }

        else if(thisroll==2){

            choice=rand() %10;
            nextchar=seednumber[choice];



            while(nextchar==lastSymbol){
                choice=rand() %10;
                nextchar=seednumber[choice];
            }

            hasNum=1;
            numberCount++;


            lastNumber=nextchar;

        }
        else if(thisroll==3){

            choice=rand() %20;
            nextchar=seedsymbol[choice];

            while(nextchar==lastSymbol){
                choice=rand() %20;
                nextchar=seedsymbol[choice];
            }

            hasSym=1;
            symbolCount++;

            lastSymbol=nextchar;

        }
    }
    else{

        if(thisroll==0){


            choice=rand() %26;
            nextchar=seedupper[choice];

            while(nextchar==lastSymbol){
                choice=rand() %20;
                nextchar=seedupper[choice];
            }

            hasUpper=1;
            upperCount++;

        }

        else if(thisroll==1){

            choice=rand() %26;
            nextchar=seedlower[choice];

            while(nextchar==lastSymbol){
                choice=rand() %20;
                nextchar=seedlower[choice];
            }

            hasLower=1;
            lowerCount++;


        }

        else if(thisroll==2){

            choice=rand() %10;
            nextchar=seednumber[choice];

            while(nextchar==lastSymbol){
                choice=rand() %20;
                nextchar=seednumber[choice];
            }

            hasNum=1;
            numberCount++;

        }

        else if(thisroll==3){

            choice=rand() %26;
            nextchar=seedlower[choice];

            while(nextchar==lastSymbol){
                choice=rand() %20;
                nextchar=seedlower[choice];
            }

            hasLower=1;
            lowerCount++;


        }
    }

    return nextchar;

}

//####################################################################################################

QString MainWindow::Spawn_Next(){
    using namespace std;

    // enable clipboard button if it is not already active
    if(!ui->CopyButton->isEnabled()){
        ui->CopyButton->setEnabled(true);
    }

    // get password length, add 7 to it to offset true list index and basic password length
    int length=ui->ListLength->currentIndex()+7;



    // check to see if the user wants nonalphanumeric characters
    bool useSpecial=false;
    if(ui->UseSpecial->isChecked()){
        useSpecial=true;
    }
    else{
        useSpecial=false;
    }


    int count=-1; // the count increments up until the password length is reached
    char nextchar;

    char password[length]; // create a char array to store the password, make its size equivilent to passwd length
    int lastroll=-1; // last number rolled, we keep track of these so there are no identical rolls 2x in a row
    int thisroll=0; // current dice roll value

    while(count<length){


        // we use a random number to see what kind of character to generate next!
        if(useSpecial){

            // roll the dice...
            thisroll=DiceRoll(1);

            // if the same number comes up twice in a row, keep rolling until we get something different
            while(thisroll==lastroll){
                thisroll=DiceRoll(1);
            }

            // keep track of the last number rolled
            lastroll=thisroll;


            // Pick a glyph based on the dice roll
            nextchar=NextDigit(1,thisroll);

            //and add it to the main password array
            password[count]=nextchar;

            count++;


        }
        else{


            // roll the dice...
            thisroll=DiceRoll(0);



            // if the same number comes up twice in a row, keep rolling until we get something different
            while(thisroll==lastroll){
                //cout << "Got the same number twice... Rolling again!" << endl;
                thisroll=DiceRoll(0);
            }
            lastroll=thisroll;

            count++;

            // Pick a glyph based on the dice roll
            nextchar=NextDigit(0,thisroll);

            //And add it to the main password array
            password[count]=nextchar;
        }
    }


    for(int c=0; c<4;c++){
        // The next functions are safeguards to make sure every password has at least one of each glyph type...
        // if true, it forces one of the missing glyphs to take the place of an existing one
        // go though each step 4 times to make sure everything gets fixed

        if(!hasUpper){
            int nextchar=NextDigit(1,0);
            int replace= rand() %length;
            password[replace]=nextchar;
            cout << "WARNING: Had to insert uppercase at index: " << replace << endl;

        }

        if(!hasLower){
            int nextchar=NextDigit(1,1);
            int replace= rand() %length;
            password[replace]=nextchar;
            cout << "WARNING: Had to insert lowercase at index: " << replace << endl;

        }

        if(!hasNum){
            int nextchar=NextDigit(1,2);
            int replace= rand() %length;
            password[replace]=nextchar;
            cout << "WARNING: Had to insert number at index: " << replace << endl;

        }
    }


    // check to see if there are at least two symbols in the password (if applicable)
    // This is to prevent passwords without symbols from being created if useSpecial=true.
    if((useSpecial) && (symbolCount < 2)){

        cout << "WARNING: Password without enough symbols created. Fixing..." << endl;

        int i=0;
        int lastindex=0;

        while(i<2){
            char nextchar=NextDigit(1,3);
            int replace= rand() %length;

            // safeguard to prevent identical indices from being selected 2x in a row
            while(replace==lastindex){
                nextchar=NextDigit(1,3);
                replace= rand() %length;
            }
            lastindex=replace;
            password[replace]=nextchar;
            i++;
        }
        i=0;
    }

    // Now display it in the UI!

    ui->PasswordOutput->setMaxLength(length);


    if(!useSpecial){

        ui->PasswordOutput->setCursorPosition(1);
        ui->PasswordOutput->clear();
        ui->PasswordOutput->setMaxLength(length--);

    }

    //convert to QString
    QString pass=QString::fromUtf8(password);
    return pass;
}


//##################################################################################################3
// Generate a single password
void MainWindow::SpawnSingle(){

    ui->StrengthMeter->setValue(0);


    QString pass=Spawn_Next();
    ui->PasswordOutput->setText(pass);

    // get password strength
    int strength=PasswordStrength(ui->PasswordOutput->text());
    ui->StrengthMeter->setValue(strength);

    //Add current password to History
    AddtoHistory(ui->PasswordOutput->text());
}



//#################################################################################################
// Calculate password strength (0-100) based on certain rules and return the score as an integer.
// This code is directly from RoboJournal 0.5, with only a few modifications. Will Kraft 5/16/14.
//  --Will Kraft 5/29/13
int MainWindow::PasswordStrength(QString passwd){
    using namespace std;

    QStringList analysis;
    analysis << "<html><head></head><body>";
    analysis << "<h2>Strength Analysis of <b>&quot;" + passwd + "</b>&quot;</h2><br>";

    int score=0;
    int len=passwd.length();

    // count of each element in passwd string
    int alphaUC=passwd.count("[A-Z]");
    int alphaLC=passwd.count("[a-z]");
    int numbers=passwd.count("[0-9");
    int symbols=passwd.count("!@#$%^&*()[]{}?~`\"\"");

    // start the logic
    if(!passwd.isEmpty()){

        score=len*5; // multiply password length by five and start the score with that value
        score=score+(numbers*4); // add number diversity bonus (multiplied by four)
        score=score+(symbols*6); // add symbol diversity bonus (multiplied by 6)
        score=score+(len-alphaUC)*2.5; // calculate uppercase letters.
        score=score+(len-alphaLC)*2.5; // calculate lowercase letters.

        int total_deductions=0;

        // Dock points for adjacent lowercase letters
        QRegExp lowercase_adj("[a-z]{2,}",Qt::CaseSensitive);
        int repeat_lc=passwd.count(lowercase_adj);

        if(repeat_lc > 0){
            analysis << "WARNING: Docking " + QString::number(repeat_lc*len) + " points from score<br>because password has " +
                        QString::number(repeat_lc) + " adjacent lowercase letters.<br><br>";
            score=score-(repeat_lc*len);
            total_deductions=total_deductions+(repeat_lc*len);
        }

        //        //dock points for no symbols (added 5/17/14)
        //        if(symbols==0){
        //            cout << "WARNING: Docking 20 points from score because password has no symbols."  << endl;
        //            score=score-20;
        //            total_deductions=total_deductions+(score);
        //        }


        // Dock points for adjacent uppercase letters
        QRegExp uppercase_adj("[A-Z]{2,}",Qt::CaseSensitive);
        int repeat_uc=passwd.count(uppercase_adj);

        if(repeat_uc > 0){
            analysis << "WARNING: Docking " + QString::number(repeat_uc*len) + " points from score<br>because password has "
                        + QString::number(repeat_uc) + " adjacent uppercase letters.<br><br>";
            score=score-(repeat_uc*len);
            total_deductions=total_deductions+(repeat_uc*len);
        }


        // Dock points for adjacent numbers
        QRegExp int_consecutive("[0-9]{2,}");
        int repeat_int=passwd.count(int_consecutive);

        if(repeat_int > 0){
            analysis << "WARNING: Docking " + QString::number(repeat_int*len) + " points from score<br>because password has " +
                        QString::number(repeat_int) + " adjacent integers.<br><br>";
            score=score-(repeat_int*len);
            total_deductions=total_deductions+(repeat_int*len);
        }

        // Dock points for repeated numbers (6/26/13)
        QString num_seed="1,2,3,4,5,6,7,8,9,0";
        QStringList num_array=num_seed.split(",");

        for(int i=0; i < num_array.size(); i++){
            QString var1=num_array.at(i);
            int num_dupes=passwd.count(var1);

            if(num_dupes >= 2){
                analysis << "WARNING: Docking " + QString::number(num_dupes * 4) + " points from score<br>because password has "
                            +  QString::number(num_dupes) +  " occurrences of &quot;" + var1 + "&quot;.<br><br>";
                score=score-(num_dupes * 4);

                total_deductions=total_deductions+(num_dupes * 4);
            }
        }

        // Dock points for multiple occurrences of each UC/LC letter (6/26/13)
        QString alpha_seed="a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z"; // Now I know my ABC's...
        QStringList alpha_array=alpha_seed.split(",");

        for(int i=0; i < alpha_array.size(); i++){
            QString var2=alpha_array.at(i);
            int alpha_dupes=passwd.count(var2,Qt::CaseInsensitive);

            if(alpha_dupes >= 2){
                analysis << "WARNING: Docking "  + QString::number(alpha_dupes * 4) + " points from score\nbecause password has "
                            + QString::number(alpha_dupes) + " occurrences of &quot;" + var2 + "&quot;.<br><br>";
                score=score-(alpha_dupes * 4);

                total_deductions=total_deductions+(alpha_dupes * 4);
            }
        }

        // Dock points for multiple occurrences of symbols. (6/23/13)
        QString sym_seed="!,@,#,$,%,^,&,*,(,),[,],{,},?,~,`,\",\'";
        QStringList sym_array=sym_seed.split(",");

        for(int i=0; i < sym_array.size(); i++){
            QString var3=sym_array.at(i);
            int sym_dupes=passwd.count(var3);

            if(sym_dupes >= 2){
                analysis << "WARNING: Docking " + QString::number(sym_dupes * 4) + " points from score<br>because password has " +
                            QString::number(sym_dupes) + " occurrences of &quot;" + var3 + "&quot;.<br><br>";
                score=score-(sym_dupes * 4);

                total_deductions=total_deductions+(sym_dupes * 4);
            }
        }

        int raw_score=score;

        // prevent score from going out of range
        if(score>100){
            score=100;
        }

        if(score < 0){
            score=0;
        }

        if(total_deductions==0)
            analysis << "No issues to report.<br>";

        // make sure the score is a whole integer w/o decimals
        score=qRound(score);
        QString strength="Weak";

        // give strength rating
        if(raw_score < 55)
            strength="<span style=\"color: darkred; font-weight: bold\">Insecure</span>";

        if((raw_score >= 55) && (score < 75))
            strength="<span style=\"color: darkgoldenrod; font-weight: bold\">Nominal</span>";

        if((raw_score >= 75) && (score < 90))
            strength="<span style=\"color: limegreen; font-weight: bold\">Adequate</span>";

        if((raw_score > 91) && (score < 100))
            strength="<span style=\"color: darkgreen; font-weight: bold\">Secure</span>";

        if((raw_score >= 100))
            strength="<span style=\"color: darkgreen; font-weight: bold\">Exceptional</span>";


        analysis << "<hr>";
        analysis << "Raw score: " + QString::number(raw_score) + " points.<br>";
        analysis << "Total deductions: " + QString::number(total_deductions) + " point(s).<br>";
        analysis << "Strength rating: " + strength + "<br><br>";

        analysis << "<b>Total adjusted score: " + QString::number(score) + "/100 points.</b><br><br>";
        analysis << "</body></html>";

        ui->AnalysisBox->setHtml(analysis.join("\n"));

        //Change the analysis tab header so people knmow if there are problems
        if(total_deductions > 0){

            ui->Tabinterface->setTabText(2,"Analysis [WARNING]");
        }
        else{
              ui->Tabinterface->setTabText(2,"Analysis");
        }

    }


    return score;
}

//###########################################################################################################

void MainWindow::on_actionNew_Password_triggered()
{
    ui->GeneratePassword->click();
}

//###########################################################################################################
// This is what happens when the user clicks the Generate Password default button.
void MainWindow::on_GeneratePassword_clicked()
{
    switch (ui->Tabinterface->currentIndex()) {
    case 0:
        ui->AnalysisBox->clear();
        SpawnSingle();

        break;


    }
}

//###########################################################################################################
void MainWindow::AddtoHistory(QString passwd){

    QListWidgetItem *next_item=new QListWidgetItem();
    QFont passfont("Consolas",16);

    next_item->setText(passwd);
    next_item->setFont(passfont);
    ui->HistoryList->insertItem(0,next_item);

    //count the passwords
    ui->PasswordCount->setText(QString::number(ui->HistoryList->count()) + " password(s) generated this session.");
}

//###########################################################################################################
void MainWindow::on_actionQuit_triggered()
{
    exit(0);
}

//###########################################################################################################
void MainWindow::on_Tabinterface_currentChanged(int index)
{
    if(index==2 || index==3){
        ui->GeneratePassword->setDisabled(true);
    }
    else{
        ui->GeneratePassword->setDisabled(false);

    }
}
