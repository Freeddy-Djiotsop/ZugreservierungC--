// Thema: Zugreservierung mit Zeiger und dynamischer Speicherallokation

#include <iostream>
#include <iomanip>

#include <fstream>
#include <string>

using namespace std;

void speicherplatzreservierung(int***& zug, const int& wagen, const int& reihe, const int& sitzplaetze)
{
    zug = new int**[wagen];
    for(int i=0; i<wagen; i++)
        zug[i] = new int*[reihe];
    for(int i=0; i<wagen; i++)
        for(int j=0; j<reihe; j++)
            zug[i][j]= new int[sitzplaetze];
}

void speicherplatzfreigeben(int***& zug, const int& wagen, const int& reihe)
{
    //In Umgekerhter Reihenfolge der Speicherallokation freigeben
    for(int i=0; i<wagen; i++)
        for(int j=0; j<reihe; j++)
            delete[] zug[i][j];
    for(int i=0; i<wagen; i++)
        delete zug[i];

    delete zug;
}

void mitNullenauffuellen(int***& zug, const int& wagen, const int& reihe, const int& sitzplaetze)
{
    for(int i=0; i<wagen; i++)
        for(int j=0; j<reihe; j++)
            for(int k=0; k<sitzplaetze; k++)
                zug[i][j][k]=0;
}

void ausgebenMitPassagierNummer(int***& zug, const int& wagen, const int& reihe, const int& sitzplaetze)
{
    for(int i=0; i<wagen; i++)
    {
        cout<<"Wagen "<<i+1<<":"<<endl;
        for(int j=0; j<reihe; j++)
        {
            for(int k=0; k<sitzplaetze; k++)
            {

                if(k==1)
                {
                    cout<<setw(4)<<zug[i][j][k];
                    cout<<" | ";
                }
                else if(k==2)
                {
                    cout<<" |";
                    cout<<setw(4)<<zug[i][j][k];
                }
                else
                    cout<<setw(4)<<zug[i][j][k];
            }
            cout<<endl;

        }
        cout<<endl<<endl;
    }
}

bool passagierenplatzieren(int***& zug, const int& passagiernummer, const int& position, const int& wagen, const int& reihe, const int& sitzplaetze)
{
    //Ich gehe davon aus Datein immer vorgegebene Struktut entspricht

    for(int i=0; i<wagen; i++)//Ich fange immer mit dem ersten Wagen
    {
        for(int j=0; j<reihe; j++)//danke gehe ich der Reihe nach
        {
            for(int k=0; k<sitzplaetze; k++)//Und für jeden Platz
            {

                if(position=='F'||position=='f')//wenn position Fenster ist, dann belege ich den Platz wenn er frei ist
                {
                    if((k==0||k==sitzplaetze-1)&& zug[i][j][k]==0)//zug[i][j][k]=0 sthet für frei
                    {
                        zug[i][j][k]=passagiernummer;
                        return true;
                    }
                }
                else//Wenn aber Position Gang ist dann
                {
                    if((k==1||k==sitzplaetze-2)&& zug[i][j][k]==0)//zug[i][j][k]=0 sthet für frei
                    {
                        zug[i][j][k]=passagiernummer;
                        return true;
                    }
                }

                //Ich betonne dass k=0 oder k=3 Fenster entsprechen
                //und k=1 und k=2 Gang
                //deswegen ueberpruefe nur entsprechend der Position
            }

        }
    }
    return false;
}

void graphischeausgabe(int***& zug, const int& wagen, const int& reihe, const int& sitzplaetze)
{
    for(int i=0; i<wagen; i++)
    {
        cout<<"Wagen "<<i+1<<":"<<endl;
        for(int j=0; j<reihe; j++)
        {
            cout<<"+-+-+-+-+-+"<<endl;
            for(int k=0; k<sitzplaetze; k++)
            {
                if(zug[i][j][k]&&k==0)
                    cout<<"|*+";
                else if(zug[i][j][k]&&k==1)
                    cout<<"*|";
                else if(zug[i][j][k]&&k==2)
                    cout<<" |*";
                else if(zug[i][j][k]&&k==3)
                    cout<<"+*|"<<endl;
                else if(!zug[i][j][k]&&k==0)
                    cout<<"| +";
                else if(!zug[i][j][k]&&k==1)
                    cout<<" |";
                else if(!zug[i][j][k]&&k==2)
                    cout<<" | ";
                else if(!zug[i][j][k]&&k==3)
                    cout<<"+ |"<<endl;
            }


        }
        cout<<"+-+-+-+-+-+"<<endl;
        cout<<endl<<endl;
    }
}

void textuelleAusgabe(int***& zug, const int& wagen, const int& reihe, const int& sitzplaetze)
{
    cout<<"Reservierte Plaetze:"<<endl;

    for(int i=0; i<wagen; i++)
        for(int j=0; j<reihe; j++)
            for(int k=0; k<sitzplaetze; k++)
                if(zug[i][j][k])
                    cout<<"Passagier Nr. "<<right<<setw(4)<<zug[i][j][k]<<": Wagen "<<i+1<<", Reihe "<<j+1<<", Platz "<<k+1<<"."<<endl;
}



int main(int argc, char* argv[])
{
    //Die Umsetzung ist in der Form: Wagen x Reihe x Sitzplaetze, also 3-Dimensional Arrays
    //Und wir bekommen eine korrekte Datei als argument int der main-Funktion

    int*** zug=nullptr;//int weil wir die Passagiernummer speicher wollen
    const int sitzplaetze=4;//eine Reihe hat immer 4 Plaetze in der Aufgabe
    int wagen=0, reihe=0, passagiernummer=0;
    char position = '0';//Gang(G) oder Fenster(F)

    if(argc==1)//wenn keine Datei als Argument, dann brechen ich das Programm ab
    {
        cout<<"FEHLERMELDUNG: Es fehlt noch eine Datei."<<endl;
        exit(-1);
    }

    ifstream input(argv[1]);//Oeffnen Datei zur reservierung
    if(!input)
    {
        cout<<"FEHLERMELDUNG: Datei \""<<argv[1]<<"\" konnte nicht geoeffnet werden."<<endl;
        exit(-1);
    }

    input>>reihe>>wagen;//Als erste steht in Datei immer die Anzahl der Reihe und der Wagen
                        //und ich speichere das jetzt, und erzeuge der Zug


    cout<<endl<<endl;
    speicherplatzreservierung(zug, wagen, reihe, sitzplaetze);//Zug erzeugen
    mitNullenauffuellen(zug, wagen, reihe, sitzplaetze);//Plaetze in der Zug mit Nullbelegen, dh der platz ist noch frei

    while(!input.eof())//
    {
        input>>passagiernummer>>position;
        if(!passagierenplatzieren(zug, passagiernummer, position, wagen, reihe, sitzplaetze))
            cout<<"Passagier Nr."<<passagiernummer<<" konnte nicht platziert werden!"<<endl;
    }
    cout<<endl<<endl;
    graphischeausgabe(zug, wagen, reihe, sitzplaetze);
    cout<<endl<<endl;
    textuelleAusgabe(zug, wagen, reihe, sitzplaetze);
    cout<<endl<<endl;
    ausgebenMitPassagierNummer(zug, wagen, reihe, sitzplaetze);
    speicherplatzfreigeben(zug, wagen, reihe);



    return 0;
}
