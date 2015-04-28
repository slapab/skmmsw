#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/sem.h>
#include <cstdio>

using namespace std;

#define FILE_SIZE 2000

int main(int argc, char *argv[])
{
    key_t key;
    int sem_id;
    key = ftok(".", 'A');
    sem_id = semget(key, 1, IPC_CREAT | 0600);
    semctl(sem_id, 0, SETVAL, 1);
    volatile unsigned int pid, i = 0, j= 0;
    pid = fork();
    if (pid == 0) {
        while (1) {
            semctl(sem_id, 0, SETVAL, 0);
            system("sudo timeout -s SIGINT 2 hcidump --raw > file.txt");
            sleep(1);
            semctl(sem_id, 0, SETVAL, 1);

        }
    }

    ifstream myfile;
    QString qstr;
    volatile unsigned int length;
    volatile unsigned int temp_calk=0, temp_ulam=0, pres=0, hour=0, min=0, year=0, month=0, day=0, w_id=0;
    char buffer[FILE_SIZE], c[3], str[130];
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    string tmp, tmp1;

    const char weather_descr[10][19] = { "", "Slonecznie", "Male zachmurzenie", "Pochmurno", "Silne zachmurzenie",
            "Silne opady", "Opady", "Burzowo", "Opady sniegu", "Mgla" } ;

    while(1) {
        system("sudo timeout -s SIGINT 2 hcitool lescan > /dev/null");


        semctl(sem_id, 0, SETVAL, 0);
        tmp.clear();
        myfile.open("file.txt");
        if (myfile.is_open()) {
            myfile.seekg(0, std::ios::end);    // go to the end
            length = myfile.tellg();           // report location (this is the length)
            myfile.seekg(0, std::ios::beg);    // go back to the beginning
            myfile.read(buffer, length);       // read the whole file into the buffer
            myfile.close();

            for (i = 0; i < length; i++) {
                if (buffer[i] != ' ' && buffer[i] != '\n')
                    tmp.append(buffer,i,1);
            }
            for (i = 0; i < (tmp.length()-3); i++) {
                    tmp1.append(tmp, i, 4);

                if (tmp1 == "0AB3") {
                    j++;
                    if (j == 2) {
                        c[0]=tmp.at(i+4);
                        c[1]=tmp.at(i+5);
                        c[2]='\0';
                        temp_calk=strtol(c, NULL, 16);
                        c[0]=tmp.at(i+6);
                        c[1]=tmp.at(i+7);
                        temp_ulam = strtol(c, NULL, 16)<<8;
                        c[0]=tmp.at(i+8);
                        c[1]=tmp.at(i+9);
                        temp_ulam += strtol(c, NULL, 16);
                        c[0]=tmp.at(i+10);
                        c[1]=tmp.at(i+11);
                        pres = strtol(c, NULL, 16)<<8;
                        c[0]=tmp.at(i+12);
                        c[1]=tmp.at(i+13);
                        pres += strtol(c, NULL, 16);
                        j = 0;
                    }
                }
               else if (tmp1 == "1009") {
                    c[0] = ( ( (tmp.at(i+4) - 0x30) * 10 ) + (tmp.at(i+5) - 0x30 ) ) -30 ;
                    c[1] = ( ( (tmp.at(i+6) - 0x30) * 10 ) + (tmp.at(i+7) - 0x30 ) ) -30;
                    c[2] = '\0';
                    w_id = 10*(unsigned int)c[0]+(unsigned int)c[1];
                    if ( w_id == 9)
                        w_id = 5;
                    else if ( w_id == 10)
                        w_id = 6;
                    else if ( w_id == 11)
                        w_id = 7;
                    else if ( w_id == 13)
                        w_id = 8;
                    else if ( w_id == 50)
                        w_id = 9;
                    c[0] = ( ( (tmp.at(i+8) - 0x30) * 10 ) + (tmp.at(i+9) - 0x30 ) )-30 ;
                    c[1] = ( ( (tmp.at(i+10) - 0x30) * 10 ) + (tmp.at(i+11) - 0x30 ) ) -30;
                    year = 10*(unsigned int)c[0]+(unsigned int)c[1];
                    c[0] = ( ( (tmp.at(i+14) - 0x30) * 10 ) + (tmp.at(i+15) - 0x30 ) ) -30;
                    c[1] = ( ( (tmp.at(i+16) - 0x30) * 10 ) + (tmp.at(i+17) - 0x30 ) ) -30;
                    month = 10*(unsigned int)c[0]+(unsigned int)c[1];
                    c[0] = ( ( (tmp.at(i+20) - 0x30) * 10 ) + (tmp.at(i+21) - 0x30 ) ) -30;
                    c[1] = ( ( (tmp.at(i+22) - 0x30) * 10 ) + (tmp.at(i+23) - 0x30 ) ) -30;
                    day = 10*(unsigned int)c[0]+(unsigned int)c[1];
                    c[0] = ( ( (tmp.at(i+24) - 0x30) * 10 ) + (tmp.at(i+25) - 0x30 ) ) -30;
                    c[1] = ( ( (tmp.at(i+26) - 0x30) * 10 ) + (tmp.at(i+27) - 0x30 ) ) -30;
                    hour =  10*(unsigned int)c[0]+(unsigned int)c[1];
                    c[0] = ( ( (tmp.at(i+30) - 0x30) * 10 ) + (tmp.at(i+31) - 0x30 ) ) -30;
                    c[1] = ( ( (tmp.at(i+32) - 0x30) * 10 ) + (tmp.at(i+33) - 0x30 ) ) -30;
                    min = 10*(unsigned int)c[0]+(unsigned int)c[1];
                }

                 tmp1.clear();

            }
            snprintf(str, 130, "Data: %u-%u-%u\nGodzina: %u:%u\nPogoda: %s\nTemperatura: %u.%u C\nCisnienie: %u HPa\nWilgotnosc: 0%%\nStezenie CO2: 0%%", year, month, day, hour, min, weather_descr[w_id], temp_calk, temp_ulam, pres);
            qstr = QString::fromAscii(str);
            w.change_Text(qstr);
            w.update();
            QCoreApplication::processEvents();
        }
        sleep(1);
        semctl(sem_id, 0, SETVAL, 1);
    }

    return a.exec();
}
