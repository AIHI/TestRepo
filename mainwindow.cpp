/* Контроль РХ */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include <QByteArray>
#include <windows.h>
#include <QMessageBox>
#include <QSettings>
#include <QFile>
#include <QList>
#include <QTimer>
#include <QThread>
#include <QTextCodec>
#include <QStringList>
#include <QtCore/qmath.h>
#include <QtSql>
#include <QtCore>






int status;
int status_tab2;
int status_tab3;
int status_tab4;



QSerialPort *serial;
QSerialPort *serial_tab2;
QSerialPort *serial_tab3;
QSerialPort *serial_tab4;

QByteArray ba;
QByteArray ba_tab2;
QByteArray ba_tab3;
QByteArray ba_tab4;

QStringList list, listhex;
QStringList list_tab2, listhex_tab2;
QStringList list_tab3, listhex_tab3;
QStringList list_tab4, listhex_tab4;

QString str,sum;
QString str_tab2,sum_tab2;
QString str_tab3,sum_tab3;
QString str_tab4,sum_tab4;


QList<int> posledov; //собираем номера-последовательность датчиков
QList<int> posledov_tab2; //собираем номера-последовательность датчиков // Tab 2
QList<int> posledov_tab3; //собираем номера-последовательность датчиков // Tab 3
QList<int> posledov_tab4; //собираем номера-последовательность датчиков // Tab 4


QTimer* timer2 = new QTimer;
QTimer* timer2_tab2 = new QTimer;
QTimer* timer2_tab3 = new QTimer;
QTimer* timer2_tab4 = new QTimer;


//глобальные переменные для значения с датчиков БДХК
QString gHp10, gMetan, gFtvk, gMonoYgl, gSerVod, gHlor, gGeksan, gOksSer,
        gOksAzot, gSolKisl, gPropan, gAmmiak, gYglGaz, gGB, gHD, gHCN, gH2CO;


//глобальные переменные для значения с датчиков БДХК // Tab 2
QString gHp10_tab2, gMetan_tab2, gFtvk_tab2, gMonoYgl_tab2, gSerVod_tab2, gHlor_tab2, gGeksan_tab2, gOksSer_tab2,
        gOksAzot_tab2, gSolKisl_tab2, gPropan_tab2, gAmmiak_tab2, gYglGaz_tab2, gGB_tab2, gHD_tab2, gHCN_tab2, gH2CO_tab2;


//глобальные переменные для значения с датчиков БДХК // Tab 3
QString gHp10_tab3, gMetan_tab3, gFtvk_tab3, gMonoYgl_tab3, gSerVod_tab3, gHlor_tab3, gGeksan_tab3, gOksSer_tab3,
        gOksAzot_tab3, gSolKisl_tab3, gPropan_tab3, gAmmiak_tab3, gYglGaz_tab3, gGB_tab3, gHD_tab3, gHCN_tab3, gH2CO_tab3;

\
//глобальные переменные для значения с датчиков БДХК // Tab 4
QString gHp10_tab4, gMetan_tab4, gFtvk_tab4, gMonoYgl_tab4, gSerVod_tab4, gHlor_tab4, gGeksan_tab4, gOksSer_tab4,
        gOksAzot_tab4, gSolKisl_tab4, gPropan_tab4, gAmmiak_tab4, gYglGaz_tab4, gGB_tab4, gHD_tab4, gHCN_tab4, gH2CO_tab4;




QTimer* timer  = new QTimer;


QString latitude, longitude; //широта, долгота для GPS-датчика


//глобальные переменные для значения с метео БДХК
QString Sm, Dm, Pa, Ta, Ua;



//глобальные переменные для ПДК датчиков БДХК
float PDKSolKisl, PDKMonoYgl, PDKYglGaz, PDKSerVod, PDKOksSer, PDKFtvk,
      PDKHlor, PDKAmiak, PDKH2CO, PDKOksAzot, PDKHP10, PDKMetan, PDKPropan,
      PDKGeksan, PDKHCN, PDKGB, PDKHD;


int statusdanger=0;//статус знака опасности




/*

//глобальные переменные для значения с датчиков БР РХО
QString gBRAmmiak, gBRChlor, gBRDioksSer, gBRDioksAzot, gBRDoza;


//глобальная переменная - кол-во записей в БК РХО
QString rec;


QString brlatitude, brlongitude; //широта, долгота для GPS-датчика


QString sum2; //для GPS


QSerialPort *port = new QSerialPort();
QSerialPort *portkrxo;

*/





QString trap="54524150",
        sek="bd656b09",
        sec="736563",
        min="6d696e",
        hour="686f7572",
        mounth="6d6f6e7468",
        day="646179",
        year="79656172",
        hp10="48703130",
        metan="cce5f2e0ed",
        ftorkisl="d4f2eef0eee2eee4eef0eee4ede0ff20eae8f1ebeef2e0",
        monoygl="cceeedeeeeeaf1e8e420f3e3ebe5f0eee4e0",
        servod="d1e5f0eee2eee4eef0eee4",
        hlor="d5ebeef0",
        geksan="c3e5eaf1e0ed",
        oksser="ceeaf1e8e420f1e5f0fb",
        oksazot="ceeaf1e8e420e0e7eef2e0",
        solkisl="d1eeebffede0ff20eae8f1ebeef2e0",
        propan="cff0eeefe0ed",
        amiak="c0ecece8e0ea",
        yglgaz="d3e3ebe5eae8f1ebfbe920e3e0e7",
        gb="4742",
        hd="4844",
        hcn="d1e8ede8ebfcede0ff20eae8f1ebeef2e0"/*"48434e"*/, //синильная кислота
        h2co="d4eef0ece0ebfce4e5e3e8e4"/*"4832434f"*/, //формальдегид

        comand1="01211001",
        zpt="2c";




QStringList getComPortList() //получаем список com-портов из реестра
{
    QStringList list;
    HKEY hKey = 0;

    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), NULL, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        DWORD cbMaxValueNameLen, cbMaxValueLen;

        if (RegQueryInfoKey(hKey, // дескриптор ключа
                            NULL, // адрес буфера для имени класса
                            NULL, // адрес размера буфер для имени класса
                            NULL, // зарезервировано
                            NULL, // адрес буфера для количества подключей
                            NULL, // адрес буфера для наибольшего размера имени подключа
                            NULL, // адрес буфера для наибольшего размера имени класса
                            NULL, // адрес буфера для количества вхождений значений
                            &cbMaxValueNameLen,       // адрес буфера для наибольшего размера имени значения
                            &cbMaxValueLen, // адрес буфера для наибольшего размера данных значения
                            NULL,       // адрес буфера для длины дескриптора безопасности
                            NULL        // адрес буфера для получения времени последней записи
            ) == ERROR_SUCCESS)
        {
            cbMaxValueNameLen++;
            cbMaxValueLen++;
            TCHAR ValueName[cbMaxValueNameLen];
            DWORD Type = REG_SZ;
            BYTE Data[cbMaxValueLen];
            char temp[cbMaxValueLen];
            DWORD cbValueNameLen, cbValueLen;

            DWORD index = 0;
            cbValueNameLen = cbMaxValueNameLen;
            cbValueLen = cbMaxValueLen;
            while (RegEnumValue(hKey,  // дескриптор запрашиваемого ключа
                            index++,  // индекс запрашиваемого значения
                            ValueName, // адрес буфера для имени значения
                            &cbValueNameLen, // адрес переменной с  размером буфера для имени значения
                            NULL, // зарезервировано
                            &Type,  // адрес переменной с типом данных
                            Data,  // адрес буфера для данных значения
                            &cbValueLen  // адрес переменной с  размером буфера для данных
                   ) == ERROR_SUCCESS)
            {
                wcstombs(temp, (WCHAR*)Data, cbValueLen/2);
                list.append(temp);
                cbValueNameLen = cbMaxValueNameLen;
                cbValueLen = cbMaxValueLen;
            }
        }
        RegCloseKey(hKey);
    }



    QRegExp COM("COM");
    QString temp;
    QStringList strlsttemp;

    for (int i=0; i<list.length(); i++) //убираем слово "COM"
    {

        if ( COM.indexIn(list.at(i)) == 0 )
        {
            temp = list.at(i).mid(3,list.at(i).length()-3);
            strlsttemp.append(temp);
        }
    }

    for (int i=0; i<strlsttemp.length()-1; i++) // упорядочиваем номера по возрастанию
    {
        if (strlsttemp.at(i+1).toFloat() < strlsttemp.at(i).toFloat())
        {
            QString s;
            s = strlsttemp.at(i+1);
            strlsttemp[i+1] = strlsttemp.at(i);
            strlsttemp[i] = s;
        }

        else
        {
        i = i + 1;
        }
    }

    for (int i=0; i<strlsttemp.length()-1; i++) //добавляем слово "COM"
    {
        list[i] = "COM" + strlsttemp.at(i);
    }

    return list;
}


//===========================================================================================================
//===========================================================================================================
//============================= БДХК ========================================================================
//===========================================================================================================
//===========================================================================================================

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    status=1;
    status_tab2=1;
    SetPDK();

    //connect(this, SIGNAL(read(QString)), this, SLOT(parse_rxo(QString)));

    //=== Tab 1 ===/
    ui->pb_closeportkrxo->setEnabled(false);
    ui->le_countkrxo->setEnabled(false);

    ui->groupBox_8->setVisible(false); //tab1
    ui->pb_giveGPSBDHK->setVisible(false);


    ui->progressBar->setVisible(false);
    ui->img_danger->setVisible(false);


    for (int i=0; i<getComPortList().length(); i++) //добавляем в комбо список в соответствии с количеством портов
    {
        ui->comboBox->addItem(getComPortList().at(i)); //для БДХК
        ui->cb_gpscomport->addItem(getComPortList().at(i)); //для GPS
        ui->cb_portkrxo->addItem(getComPortList().at(i)); //для АСКРХО
    }

    portCloseStyleSheet();
    //portCloseStyleGPS();
    deviceOff(); //убираем элементы, т.к. не прошли идентиф.
    groupbox(false);



    //=== Tab 2 ===/

    ui->pb_closeportkrxo_3->setEnabled(false);
    ui->le_countkrxo_3->setEnabled(false);

    ui->groupBox_23->setVisible(false); //tab2
    ui->pb_giveGPSBDHK_3->setVisible(false);


    ui->progressBar_3->setVisible(false);
    ui->img_danger_3->setVisible(false);


    for (int i=0; i<getComPortList().length(); i++) //добавляем в комбо список в соответствии с количеством портов
    {
        ui->comboBox_3->addItem(getComPortList().at(i)); //для БДХК
        ui->cb_gpscomport_3->addItem(getComPortList().at(i)); //для GPS
        ui->cb_portkrxo_3->addItem(getComPortList().at(i)); //для АСКРХО
    }

    portCloseStyleSheet_3();
    //portCloseStyleGPS_3();
    deviceOff_3(); //убираем элементы, т.к. не прошли идентиф.
    groupbox_3(false);



    //=== Tab 3 ===/

    ui->pb_closeportkrxo_4->setEnabled(false);
    ui->le_countkrxo_4->setEnabled(false);

    ui->groupBox_31->setVisible(false); //tab3
    ui->pb_giveGPSBDHK_4->setVisible(false);


    ui->progressBar_4->setVisible(false);
    ui->img_danger_4->setVisible(false);


    for (int i=0; i<getComPortList().length(); i++) //добавляем в комбо список в соответствии с количеством портов
    {
        ui->comboBox_4->addItem(getComPortList().at(i)); //для БДХК
        ui->cb_gpscomport_4->addItem(getComPortList().at(i)); //для GPS
        ui->cb_portkrxo_4->addItem(getComPortList().at(i)); //для АСКРХО
    }

    portCloseStyleSheet_4();
    //portCloseStyleGPS_3();
    deviceOff_4(); //убираем элементы, т.к. не прошли идентиф.
    groupbox_4(false);



    //=== Tab 4 ===/

    ui->pb_closeportkrxo_5->setEnabled(false);
    ui->le_countkrxo_5->setEnabled(false);

    ui->groupBox_39->setVisible(false); //tab4
    ui->pb_giveGPSBDHK_5->setVisible(false);


    ui->progressBar_5->setVisible(false);
    ui->img_danger_5->setVisible(false);


    for (int i=0; i<getComPortList().length(); i++) //добавляем в комбо список в соответствии с количеством портов
    {
        ui->comboBox_5->addItem(getComPortList().at(i)); //для БДХК
        ui->cb_gpscomport_5->addItem(getComPortList().at(i)); //для GPS
        ui->cb_portkrxo_5->addItem(getComPortList().at(i)); //для АСКРХО
    }

    portCloseStyleSheet_5();
    //portCloseStyleGPS_3();
    deviceOff_5(); //убираем элементы, т.к. не прошли идентиф.
    groupbox_5(false);

}

MainWindow::~MainWindow()
{

   //serial->close();
    delete ui;
}




//=====================================================================================================
//======================== Data =======================================================================

void MainWindow::serialReceived()  //получаем пакеты //для Tab 1
{

    if (status==0)
    {
    ui->statusBar->showMessage("Читаем данные");

    ba=serial->read(4);//считываем по 4 байта
    list.append(ba);
    listhex.append(ba.toHex());

    //qDebug()<<"list.append(ba)"<<list;
    //qDebug()<<"list.append(ba.toHex())"<<listhex;

    ui->statusBar->showMessage("Обмен осуществлён");
    }
    else ui->statusBar->showMessage("Подключение отсутствует. Не удалось serialReceived");
}

void MainWindow::ComConnect() //инициализируем порт
{

    serial = new QSerialPort(this);
    serial->setPortName(ui->comboBox->currentText());
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if (serial->open(QIODevice::ReadWrite))
    {
        //qDebug()<< "COMPORT OPEN!";
        status=0;
        portOpenStyleSheet();
        connect(serial, SIGNAL(readyRead()),this,SLOT(serialReceived()));
        ui->statusBar->showMessage("Соединение установлено");
        monitorOff();

    }
    else
    {
        //qDebug()<<"COMPORT CLOSE!";
        status=1;
        portCloseStyleSheet();
        ui->statusBar->showMessage("Соединение отсутствует");
        serial->close();
    }

}

bool MainWindow::getType() /* Запрос типов датчиков */
{
    list.clear();
    listhex.clear();

    char data[8]; //0x54 0x52 0x41 0x50 1 0x08 0x81 0x08
    data[0]=0x54;
    data[1]=0x52;
    data[2]=0x41;
    data[3]=0x50;
    data[4]=0x01; //1 = 0x01
    data[5]=0x08;
    data[6]=0x81;
    data[7]=0x08;

   if (serial->isOpen()) //если порт открыт
    {
    serial->write(data,8);
        if (serial->waitForBytesWritten(3000)) //ждем пока не запишутся все данные
            return true;
        else
            return false;
    }
   else                   //если порт закрыт
       return false;
}

void MainWindow::monitorOff() //Выключить передачу каждую минуту
{

    list.clear();
    listhex.clear();

    if (status==0) //status=0 - подключение установленно
    {

    ui->statusBar->showMessage("Отключаем мониторинг каждую минуту");
    char data[9]; //0x54 0x52 0x41 0x50 2 0x21 0x10 0x00 0x01


    data[0]=0x54;
    data[1]=0x52;
    data[2]=0x41;
    data[3]=0x50;
    data[4]=0x02; //2 = 0x02
    data[5]=0x21;
    data[6]=0x10;
    data[7]=0x00;
    data[8]=0x01;


    serial->write(data,9);

    serial->waitForBytesWritten(500);
    qDebug()<<"Пакет послан monitorOff";
    ui->statusBar->showMessage("Мониторинг каждую минуту - отключить - команда послана");
    }
    else ui->statusBar->showMessage(tr("Соединение отсутствует. Не удалось monitorOff"));

    listhex.clear();
}

void MainWindow::monitorOn() //Включить передачу каждую минуту
{
    char data[9]; //0x54 0x52 0x41 0x50 2 0x00 0x00 0x00 0x00

    list.clear();
    listhex.clear();

    data[0]=0x54;
    data[1]=0x52;
    data[2]=0x41;
    data[3]=0x50;
    data[4]=0x02; //2 = 0x02
    data[5]=0x00;
    data[6]=0x00;
    data[7]=0x00;
    data[8]=0x00;
    serial->write(data,9);
    serial->waitForBytesWritten(500);
    qDebug()<<"Пакет послан monitorOn";
    ui->statusBar->showMessage("Мониторинг каждую минуту включён");

    listhex.clear();

}

void MainWindow::getlist() /* парсим Идент. */
{

    str=listhex.join(""); /* соединяем в строку весь HEX код */
    posledov.clear();


    if (comparestr(trap,0)) /* заголовок верный (TRAP) */
                {
                    ui->statusBar->showMessage("Верный заголовок пакета"); //если верный заголовок, тогда читаем дальше
                        qDebug()<<str;
                    /* Верный формат времени SEK SEC MIN HOUR */
                    if (/*(comparestr(sek,8))&*/comparestr(sec,16)&&comparestr(min,24)&&comparestr(hour,32))
                    {
                        ui->statusBar->showMessage("Верный заголовок времени");
                        ui->label_time->setVisible(true);
                        ui->label_time->setText("Время");
                    }
                    /* Верный формат даты MOUNTH DAY YEAR */
                    if ((comparestr(mounth,42))&&comparestr(day,54)&&comparestr(year,62))
                    {
                        ui->statusBar->showMessage("Верный заголовок даты");
                        ui->label_date->setVisible(true);
                        ui->label_date->setText("Дата");
                    }


                        for (int i=72; i<=436/*374*/; i++) //начинаем шагать с 72 по 1 биту и проверять на совпадения
                        {
                    /* Верный формат МЭД */
                    if ((comparestr(hp10,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'МЭД'");
                        ui->label_hp10->setVisible(true);
                        ui->label_hp10->setText("МЭД");
                        ui->lineEdit_Hp10->setVisible(true);
                        i=i+hp10.length()-1;
                        posledov.append(1);
                    }
                    /* Верный формат Метан */
                    if ((comparestr(metan,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Метан'");
                        ui->label_metan->setVisible(true);
                        ui->label_metan->setText("Метан, этан");
                        ui->lineEdit_metan->setVisible(true);
                        i=i+metan.length()-1;
                        posledov.append(2);
                    }
                    /* Верный формат Фторовод. кислота */
                    if ((comparestr(ftorkisl,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Фт-вод. кислота'");
                        ui->label_ftvk->setVisible(true);
                        ui->label_ftvk->setText("Фт-вод. кислота");
                        ui->lineEdit_ftvk->setVisible(true);
                        i=i+ftorkisl.length()-1;
                        posledov.append(3);
                    }
                    /* Верный формат Моноксид углерода */
                    if ((comparestr(monoygl,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Моноксид углерода'");
                        ui->label_monoygl->setVisible(true);
                        ui->label_monoygl->setText("Моноксид углерода");
                        ui->lineEdit_monoygl->setVisible(true);
                        i=i+monoygl.length()-1;
                        posledov.append(4);
                    }
                    /* Верный формат Сероводород */
                    if ((comparestr(servod,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Сероводород'");
                        ui->label_servod->setVisible(true);
                        ui->label_servod->setText("Сероводород");
                        ui->lineEdit_servod->setVisible(true);
                        i=i+servod.length()-1;
                        posledov.append(5);
                    }
                    /* Верный формат Хлор */
                    if ((comparestr(hlor,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Хлор'");
                        ui->label_hlor->setVisible(true);
                        ui->label_hlor->setText("Хлор");
                        ui->lineEdit_hlor->setVisible(true);
                        i=i+hlor.length()-1;
                        posledov.append(6);
                    }
                    /* Верный формат Гексан */
                    if ((comparestr(geksan,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Гексан'");
                        ui->label_geksan->setVisible(true);
                        ui->label_geksan->setText("Пентан, гексан");
                        ui->lineEdit_geksan->setVisible(true);
                        i=i+geksan.length()-1;
                        posledov.append(7);
                    }
                    /* Верный формат Оксид серы */
                    if ((comparestr(oksser,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Оксид серы'");
                        ui->label_oksser->setVisible(true);
                        ui->label_oksser->setText("Оксид серы");
                        ui->lineEdit_oksser->setVisible(true);
                        i=i+oksser.length()-1;
                        posledov.append(8);
                    }
                    /* Верный формат Оксид азота */
                    if ((comparestr(oksazot,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Оксид азота'");
                        ui->label_oksazot->setVisible(true);
                        ui->label_oksazot->setText("Оксид азота");
                        ui->lineEdit_oksazot->setVisible(true);
                        i=i+oksazot.length()-1;
                        posledov.append(9);
                    }
                    /* Верный формат Соляная кислота */
                    if ((comparestr(solkisl,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Соляная кислота'");
                        ui->label_solkisl->setVisible(true);
                        ui->label_solkisl->setText("Соляная кислота");
                        ui->lineEdit_solkisl->setVisible(true);
                        i=i+solkisl.length()-1;
                        posledov.append(10);
                    }
                    /* Верный формат Пропан */
                    if ((comparestr(propan,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Пропан'");
                        ui->label_propan->setVisible(true);
                        ui->label_propan->setText("Пропан, бутан");
                        ui->lineEdit_propan->setVisible(true);
                        i=i+propan.length()-1;
                        posledov.append(11);
                    }
                    /* Верный формат Аммиак */
                    if ((comparestr(amiak,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Аммиак'");
                        ui->label_amiak->setVisible(true);
                        ui->label_amiak->setText("Аммиак");
                        ui->lineEdit_amiak->setVisible(true);
                        i=i+amiak.length()-1;
                        posledov.append(12);
                    }
                    /* Верный формат Углекислый газ */
                    if ((comparestr(yglgaz,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Углекислый газ'");
                        ui->label_yglgaz->setVisible(true);
                        ui->label_yglgaz->setText("Углекислый газ");
                        ui->lineEdit_yglgaz->setVisible(true);
                        i=i+yglgaz.length()-1;
                        posledov.append(13);
                    }
                    /* Верный формат GB */
                    if ((comparestr(gb,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'GB'");
                        ui->label_gb->setVisible(true);
                        ui->label_gb->setText("GB");
                        ui->lineEdit_gb->setVisible(true);
                        i=i+gb.length()-1;
                        posledov.append(14);
                    }
                    /* Верный формат HD */
                    if ((comparestr(hd,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'HD'");
                        ui->label_hd->setVisible(true);
                        ui->label_hd->setText("HD");
                        ui->lineEdit_hd->setVisible(true);
                        i=i+hd.length()-1;
                        posledov.append(15);
                    }
                    /* Верный формат HCN */
                    if ((comparestr(hcn,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'HCN'");
                        ui->label_hcn->setVisible(true);
                        ui->label_hcn->setText("Синильная кислота");
                        ui->lineEdit_hcn->setVisible(true);
                        i=i+hcn.length()-1;
                        posledov.append(16);
                    }
                    /* Верный формат H2CO */
                    if ((comparestr(h2co,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'H2CO'");
                        ui->label_h2co->setVisible(true);
                        ui->label_h2co->setText("Формальдегид");
                        ui->lineEdit_h2co->setVisible(true);
                        i=i+h2co.length()-1;
                        posledov.append(17);
                    }
                    if (comparestr(zpt,i)) //если нет датчика - запятая
                    {
                        posledov.append(0);
                    }
              ui->pushButton_getValue->setVisible(true);
                        }
                     groupbox(true); /* "зажигаем" все GroupBox */
    }

    else
    {
        ui->statusBar->showMessage("Неверный заголовок пакета. Переподключитесь");
        ui->pbtn_type->setEnabled(true);
    }

    str.clear();
    list.clear();
    listhex.clear();

}

bool MainWindow::Value() /* парсим значения */
{
    str.clear();
    str=listhex.join("");
    qDebug()<<str;
    if (comparestr(trap,0)) /* заголовок верный (TRAP) */
                {

                    ui->statusBar->showMessage("Верный заголовок пакета"); /* если верный заголовок, тогда читаем дальше */
                    if (comparestr(comand1,8) && comparestr(trap,16)) /* 0121100154524150 - верно */
                     {
                        if (comparestr("05",30)) /* вход для считывания значений 0x05 */
                            ui->statusBar->showMessage("Верный вход для считывания");
                        {
                            /* час:минута:сек */
                            ui->label_time->setText(str.at(36) + (str.at(37).toLatin1()) + tr(":") +
                                                   (str.at(34).toLatin1()) + (str.at(35).toLatin1()) + tr(":") +
                                                   (str.at(32).toLatin1()) + (str.at(33).toLatin1()));

                            /* день:месяц:год */
                            ui->label_date->setText(str.at(38) + (str.at(39).toLatin1()) + tr(".") +
                                                   (str.at(40).toLatin1()) + (str.at(41).toLatin1()) + tr(".") +
                                                    tr("20") +(str.at(42).toLatin1()) + (str.at(43).toLatin1()));


                            int b=51; /* 51 - конечный бит первого пакета значений; 179 - конечный бит последнего пакета */

                                    for (int p=0; p<posledov.length(); p++) /* 17 - общее количество значений с датчиков */

                                    {
                                        {
                                            qDebug()<<"POSLEDOV ALL"<<posledov;
                                            qDebug()<<"POSLEDOV.at(p)"<<posledov.at(p);

                                            switch (posledov.at(p))
                                                {
                                            case 1:
                                                /* Hp10 */

                                                qDebug()<<"B-Hp10"<<b;
                                                gHp10 = FormatText(IEEE754(b));
                                                ui->lineEdit_Hp10->setText(gHp10);
                                                ui->lineEdit_Hp10->insert(" [мкЗв/ч]");
                                                b=b+8;
                                                break;

                                            case 2:

                                                /* Метан */

                                                qDebug()<<"B-Метан"<<b;
                                                gMetan = FormatText(IEEE754(b));
                                                ui->lineEdit_metan->setText(gMetan);
                                                ui->lineEdit_metan->insert(" [об.%]");
                                                b=b+8;
                                                break;

                                            case 3:
                                                /* Фтороводородная кислота */

                                                qDebug()<<"B-Фторвод"<<b;
                                                gFtvk = FormatText(IEEE754(b));
                                                ui->lineEdit_ftvk->setText(gFtvk);
                                                ui->lineEdit_ftvk->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 4:
                                                /*  Моноксид углерода */

                                                qDebug()<<"B-Моноксид углерода"<<b;
                                                gMonoYgl = FormatText(IEEE754(b));
                                                ui->lineEdit_monoygl->setText(gMonoYgl);
                                                ui->lineEdit_monoygl->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 5:

                                                /*  Сероводород */

                                                qDebug()<<"B-Сероводород"<<b;
                                                gSerVod = FormatText(IEEE754(b));
                                                ui->lineEdit_servod->setText( gSerVod );
                                                ui->lineEdit_servod->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 6:
                                                /*  Хлор */

                                                qDebug()<<"B-Хлор"<<b;
                                                gHlor = FormatText(IEEE754(b));
                                                ui->lineEdit_hlor->setText( gHlor );
                                                ui->lineEdit_hlor->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 7:
                                                /*  Гексан */

                                                qDebug()<<"B-Гексан"<<b;
                                                gGeksan = FormatText(IEEE754(b));
                                                ui->lineEdit_geksan->setText( gGeksan );
                                                ui->lineEdit_geksan->insert(" [мг/л]");
                                                b=b+8;
                                                break;

                                            case 8:
                                                /*  Оксид серы */

                                                qDebug()<<"B-Оксид серы"<<b;
                                                gOksSer = FormatText(IEEE754(b));
                                                ui->lineEdit_oksser->setText( gOksSer );
                                                ui->lineEdit_oksser->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 9:

                                                /*  Оксид азота */

                                                qDebug()<<"B-Оксид азота"<<b;
                                                gOksAzot = FormatText(IEEE754(b));
                                                ui->lineEdit_oksazot->setText( gOksAzot );
                                                ui->lineEdit_oksazot->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 10:

                                                /*  Соляная кислота */

                                                qDebug()<<"B-Соляная кислота"<<b;
                                                gSolKisl = FormatText(IEEE754(b));
                                                ui->lineEdit_solkisl->setText( gSolKisl );
                                                ui->lineEdit_solkisl->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 11:
                                                /*  Пропан */

                                                qDebug()<<"B-Пропан"<<b;
                                                gPropan = FormatText(IEEE754(b));
                                                ui->lineEdit_propan->setText( gPropan );
                                                ui->lineEdit_propan->insert(" [об.%]");
                                                b=b+8;
                                                break;

                                            case 12:
                                                /*  Аммиак */

                                                qDebug()<<"B-Аммиак"<<b;
                                                gAmmiak = FormatText(IEEE754(b));
                                                ui->lineEdit_amiak->setText( gAmmiak );
                                                ui->lineEdit_amiak->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 13:
                                                /* Углекислый газ */

                                                qDebug()<<"B-Углекислый газ"<<b;
                                                gYglGaz = FormatText(IEEE754(b));
                                                ui->lineEdit_yglgaz->setText( gYglGaz );
                                                ui->lineEdit_yglgaz->insert(" [об.%]");
                                                b=b+8;
                                                break;

                                            case 14:
                                                /*  Иприт  (GB) */


                                                qDebug()<<"B-Иприт  (GB)"<<b;


                                                if (FormatText(IEEE754(b))=="0.0")
                                                {
                                                  ui->lineEdit_gb->setText("В норме");
                                                  gGB = "0";
                                                }
                                                if (IEEE754(b)=="1")
                                                {
                                                  ui->lineEdit_gb->setText("1 порог");
                                                  gGB = "1";
                                                }
                                                if (IEEE754(b)=="100")
                                                {
                                                  ui->lineEdit_gb->setText("2 порог");
                                                  gGB = "100";
                                                }


                                                /* ui->lineEdit_gb->setText(FormatText(IEEE754(b))); */
                                                /* ui->lineEdit_gb->setText(IEEE754(b)); */

                                                b=b+8;
                                                break;

                                            case 15:
                                                /*  Зарин (HD)  */

                                                qDebug()<<"B-HD"<<b;
                                                if (FormatText(IEEE754(b))=="0.0")
                                                {
                                                  ui->lineEdit_hd->setText("В норме");
                                                  gHD = "0";
                                                }
                                                if (FormatText(IEEE754(b))=="2.000")
                                                {
                                                  ui->lineEdit_hd->setText("1 порог");
                                                  gHD = "2";
                                                }
                                                if (FormatText(IEEE754(b))=="200.0")
                                                {
                                                  ui->lineEdit_hd->setText("2 порог");
                                                  gHD = "200";
                                                }
                                                /* ui->lineEdit_hd->setText(FormatText(IEEE754(b))); */
                                                b=b+8;
                                                break;

                                            case 16:
                                                /*  HCN */

                                                qDebug()<<"B-HCN"<<b;
                                                gHCN = FormatText(IEEE754(b));
                                                ui->lineEdit_hcn->setText( gHCN );
                                                ui->lineEdit_hcn->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 17:
                                                /*  H2CO - формальдегид */

                                                qDebug()<<"B-H2CO"<<b;
                                                gH2CO = FormatText(IEEE754(b));
                                                ui->lineEdit_h2co->setText( gH2CO );
                                                b=b+8;
                                                break;

                                            default:
                                                if (posledov.at(p-1)==0)
                                                {
                                                    b=b+8;
                                                }
                                                /* b=b+8; */
                                                break;
                                                }


                               } /* _for */
                            }/* _for */

                                    if (ui->cb_automode->isChecked())
                                    {
                                       SQLBDHK();
                                    }

                        }/* _if */
                     }


                }
    else ui->statusBar->showMessage(tr("Не верный заголовок пакета данных"));




    str.clear();
    listhex.clear();
    list.clear();
    ui->pushButton_getValue->setEnabled(true);
    ui->pushButton_getValue->setEnabled(true);
    all_lineedit_enable(true);
    monitorOff();

/*
    if (ui->cb_automode->isChecked())
    {
      on_pb_giveMeteo_clicked();
    }
*/
}


//========================== Tab 2

void MainWindow::serialReceived_3()  //получаем пакеты //для Tab 2
{

    if (status_tab2==0)
    {
    ui->statusBar->showMessage("Читаем данные / Tab 2");

    ba_tab2=serial_tab2->read(4);//считываем по 4 байта
    list_tab2.append(ba_tab2);
    listhex_tab2.append(ba_tab2.toHex());

    //qDebug()<<"list.append(ba)"<<list;
    //qDebug()<<"list.append(ba.toHex())"<<listhex;

    ui->statusBar->showMessage("Обмен осуществлён");
    }
    else
    ui->statusBar->showMessage("Подключение отсутствует. Не удалось serialReceived");
}

void MainWindow::ComConnect_3() //инициализируем порт //tab 2
{

    serial_tab2 = new QSerialPort(this);
    serial_tab2->setPortName(ui->comboBox_3->currentText());
    serial_tab2->setBaudRate(QSerialPort::Baud9600);
    serial_tab2->setDataBits(QSerialPort::Data8);
    serial_tab2->setParity(QSerialPort::NoParity);
    serial_tab2->setStopBits(QSerialPort::OneStop);
    serial_tab2->setFlowControl(QSerialPort::NoFlowControl);
    if (serial_tab2->open(QIODevice::ReadWrite))
    {
        //qDebug()<< "COMPORT OPEN!";
        status_tab2=0;
        portOpenStyleSheet_3();
        connect(serial_tab2, SIGNAL(readyRead()),this,SLOT(serialReceived_3()));
        ui->statusBar->showMessage("Соединение установлено / Tab 2");
        monitorOff_3();

    }
    else
    {
        //qDebug()<<"COMPORT CLOSE!";
        status_tab2=1;
        portCloseStyleSheet_3();
        ui->statusBar->showMessage("Соединение отсутствует / Tab 2");
        serial_tab2->close();


    }

}

bool MainWindow::getType_3() /* Запрос типов датчиков // Для Tab 2 */
{
    list_tab2.clear();
    listhex_tab2.clear();

    char data[8]; //0x54 0x52 0x41 0x50 1 0x08 0x81 0x08
    data[0]=0x54;
    data[1]=0x52;
    data[2]=0x41;
    data[3]=0x50;
    data[4]=0x01; //1 = 0x01
    data[5]=0x08;
    data[6]=0x81;
    data[7]=0x08;

   if (serial_tab2->isOpen()) //если порт открыт
    {
    serial_tab2->write(data,8);
        if (serial_tab2->waitForBytesWritten(3000)) //ждем пока не запишутся все данные
            return true;
        else
            return false;
    }
   else                   //если порт закрыт
       return false;
}

void MainWindow::monitorOff_3() //Выключить передачу каждую минуту //для Tab 2
{

    list_tab2.clear();
    listhex_tab2.clear();

    if (status_tab2==0) //status=0 - подключение установленно
    {

    ui->statusBar->showMessage("Отключаем мониторинг каждую минуту / Tab 2");
    char data[9]; //0x54 0x52 0x41 0x50 2 0x21 0x10 0x00 0x01


    data[0]=0x54;
    data[1]=0x52;
    data[2]=0x41;
    data[3]=0x50;
    data[4]=0x02; //2 = 0x02
    data[5]=0x21;
    data[6]=0x10;
    data[7]=0x00;
    data[8]=0x01;


    serial_tab2->write(data,9);

    serial_tab2->waitForBytesWritten(500);
    qDebug()<<"Пакет послан monitorOff / Tab 2";
    ui->statusBar->showMessage("Мониторинг каждую минуту - отключить - команда послана / Tab 2");
    }
    else ui->statusBar->showMessage(tr("Соединение отсутствует. Не удалось monitorOff / Tab 2"));

    listhex_tab2.clear();
}

void MainWindow::monitorOn_3() //Включить передачу каждую минуту
{
    char data[9]; //0x54 0x52 0x41 0x50 2 0x00 0x00 0x00 0x00

    list_tab2.clear();
    listhex_tab2.clear();

    data[0]=0x54;
    data[1]=0x52;
    data[2]=0x41;
    data[3]=0x50;
    data[4]=0x02; //2 = 0x02
    data[5]=0x00;
    data[6]=0x00;
    data[7]=0x00;
    data[8]=0x00;
    serial_tab2->write(data,9);
    serial_tab2->waitForBytesWritten(500);
    qDebug()<<"Пакет послан monitorOn";
    ui->statusBar->showMessage("Мониторинг каждую минуту включён / Tab 2");

    listhex_tab2.clear();
}

void MainWindow::getlist_3() /* парсим Идент. // Tab 2*/
{

    str_tab2=listhex_tab2.join(""); /* соединяем в строку весь HEX код */
    posledov_tab2.clear();


    if (comparestr_3(trap,0)) /* заголовок верный (TRAP) */
                {
                    ui->statusBar->showMessage("Верный заголовок пакета / Tab 2"); //если верный заголовок, тогда читаем дальше
                        qDebug()<<str_tab2;
                    /* Верный формат времени SEK SEC MIN HOUR */
                    if (/*(comparestr(sek,8))&*/comparestr_3(sec,16)&&comparestr_3(min,24)&&comparestr_3(hour,32))
                    {
                        ui->statusBar->showMessage("Верный заголовок времени / Tab 2");
                        ui->label_time_3->setVisible(true);
                        ui->label_time_3->setText("Время");
                    }
                    /* Верный формат даты MOUNTH DAY YEAR */
                    if ((comparestr_3(mounth,42))&&comparestr_3(day,54)&&comparestr_3(year,62))
                    {
                        ui->statusBar->showMessage("Верный заголовок даты / Tab 2");
                        ui->label_date_3->setVisible(true);
                        ui->label_date_3->setText("Дата");
                    }


                        for (int i=72; i<=436/*374*/; i++) //начинаем шагать с 72 по 1 биту и проверять на совпадения
                        {
                    /* Верный формат МЭД */
                    if ((comparestr_3(hp10,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'МЭД' // Tab 2");
                        ui->label_hp10_3->setVisible(true);
                        ui->label_hp10_3->setText("МЭД");
                        ui->lineEdit_Hp10_3->setVisible(true);
                        i=i+hp10.length()-1;
                        posledov_tab2.append(1);
                    }
                    /* Верный формат Метан */
                    if ((comparestr_3(metan,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Метан' // Tab 2");
                        ui->label_metan_3->setVisible(true);
                        ui->label_metan_3->setText("Метан, этан");
                        ui->lineEdit_metan_3->setVisible(true);
                        i=i+metan.length()-1;
                        posledov_tab2.append(2);
                    }
                    /* Верный формат Фторовод. кислота */
                    if ((comparestr_3(ftorkisl,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Фт-вод. кислота' // Tab 2");
                        ui->label_ftvk_3->setVisible(true);
                        ui->label_ftvk_3->setText("Фт-вод. кислота");
                        ui->lineEdit_ftvk_3->setVisible(true);
                        i=i+ftorkisl.length()-1;
                        posledov_tab2.append(3);
                    }
                    /* Верный формат Моноксид углерода */
                    if ((comparestr_3(monoygl,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Моноксид углерода' // Tab 2");
                        ui->label_monoygl_3->setVisible(true);
                        ui->label_monoygl_3->setText("Моноксид углерода");
                        ui->lineEdit_monoygl_3->setVisible(true);
                        i=i+monoygl.length()-1;
                        posledov_tab2.append(4);
                    }
                    /* Верный формат Сероводород */
                    if ((comparestr_3(servod,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Сероводород' // Tab 2");
                        ui->label_servod_3->setVisible(true);
                        ui->label_servod_3->setText("Сероводород");
                        ui->lineEdit_servod_3->setVisible(true);
                        i=i+servod.length()-1;
                        posledov_tab2.append(5);
                    }
                    /* Верный формат Хлор */
                    if ((comparestr_3(hlor,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Хлор' // Tab 2");
                        ui->label_hlor_3->setVisible(true);
                        ui->label_hlor_3->setText("Хлор");
                        ui->lineEdit_hlor_3->setVisible(true);
                        i=i+hlor.length()-1;
                        posledov_tab2.append(6);
                    }
                    /* Верный формат Гексан */
                    if ((comparestr_3(geksan,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Гексан' // Tab 2");
                        ui->label_geksan_3->setVisible(true);
                        ui->label_geksan_3->setText("Пентан, гексан");
                        ui->lineEdit_geksan_3->setVisible(true);
                        i=i+geksan.length()-1;
                        posledov_tab2.append(7);
                    }
                    /* Верный формат Оксид серы */
                    if ((comparestr_3(oksser,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Оксид серы' // Tab 2");
                        ui->label_oksser_3->setVisible(true);
                        ui->label_oksser_3->setText("Оксид серы");
                        ui->lineEdit_oksser_3->setVisible(true);
                        i=i+oksser.length()-1;
                        posledov_tab2.append(8);
                    }
                    /* Верный формат Оксид азота */
                    if ((comparestr_3(oksazot,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Оксид азота' // Tab 2");
                        ui->label_oksazot_3->setVisible(true);
                        ui->label_oksazot_3->setText("Оксид азота");
                        ui->lineEdit_oksazot_3->setVisible(true);
                        i=i+oksazot.length()-1;
                        posledov_tab2.append(9);
                    }
                    /* Верный формат Соляная кислота */
                    if ((comparestr_3(solkisl,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Соляная кислота' // Tab 2");
                        ui->label_solkisl_3->setVisible(true);
                        ui->label_solkisl_3->setText("Соляная кислота");
                        ui->lineEdit_solkisl_3->setVisible(true);
                        i=i+solkisl.length()-1;
                        posledov_tab2.append(10);
                    }
                    /* Верный формат Пропан */
                    if ((comparestr_3(propan,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Пропан' // Tab 2");
                        ui->label_propan_3->setVisible(true);
                        ui->label_propan_3->setText("Пропан, бутан");
                        ui->lineEdit_propan_3->setVisible(true);
                        i=i+propan.length()-1;
                        posledov_tab2.append(11);
                    }
                    /* Верный формат Аммиак */
                    if ((comparestr_3(amiak,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Аммиак' // Tab 2");
                        ui->label_amiak_3->setVisible(true);
                        ui->label_amiak_3->setText("Аммиак");
                        ui->lineEdit_amiak_3->setVisible(true);
                        i=i+amiak.length()-1;
                        posledov_tab2.append(12);
                    }
                    /* Верный формат Углекислый газ */
                    if ((comparestr_3(yglgaz,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Углекислый газ' // Tab 2");
                        ui->label_yglgaz_3->setVisible(true);
                        ui->label_yglgaz_3->setText("Углекислый газ");
                        ui->lineEdit_yglgaz_3->setVisible(true);
                        i=i+yglgaz.length()-1;
                        posledov_tab2.append(13);
                    }
                    /* Верный формат GB */
                    if ((comparestr_3(gb,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'GB' // Tab 2");
                        ui->label_gb_3->setVisible(true);
                        ui->label_gb_3->setText("GB");
                        ui->lineEdit_gb_3->setVisible(true);
                        i=i+gb.length()-1;
                        posledov_tab2.append(14);
                    }
                    /* Верный формат HD */
                    if ((comparestr_3(hd,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'HD' // Tab 2");
                        ui->label_hd_3->setVisible(true);
                        ui->label_hd_3->setText("HD");
                        ui->lineEdit_hd_3->setVisible(true);
                        i=i+hd.length()-1;
                        posledov_tab2.append(15);
                    }
                    /* Верный формат HCN */
                    if ((comparestr_3(hcn,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'HCN' // Tab 2");
                        ui->label_hcn_3->setVisible(true);
                        ui->label_hcn_3->setText("Синильная кислота");
                        ui->lineEdit_hcn_3->setVisible(true);
                        i=i+hcn.length()-1;
                        posledov_tab2.append(16);
                    }
                    /* Верный формат H2CO */
                    if ((comparestr_3(h2co,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'H2CO' // Tab 2");
                        ui->label_h2co_3->setVisible(true);
                        ui->label_h2co_3->setText("Формальдегид");
                        ui->lineEdit_h2co_3->setVisible(true);
                        i=i+h2co.length()-1;
                        posledov_tab2.append(17);
                    }
                    if (comparestr_3(zpt,i)) //если нет датчика - запятая
                    {
                        posledov_tab2.append(0);
                    }
              ui->pushButton_getValue_3->setVisible(true);
                        }
                     groupbox_3(true); /* "зажигаем" все GroupBox */
    }

    else
    {
        ui->statusBar->showMessage("Неверный заголовок пакета. Переподключитесь // Tab 2");
        ui->pbtn_type_3->setEnabled(true);

        str_tab2.clear();
        list_tab2.clear();
        listhex_tab2.clear();
    }

    str_tab2.clear();
    list_tab2.clear();
    listhex_tab2.clear();

}

bool MainWindow::Value_3() /* парсим значения // для Tab 2*/
{
    str_tab2.clear();
    str_tab2=listhex_tab2.join("");
    qDebug()<<str_tab2;
    if (comparestr_3(trap,0)) /* заголовок верный (TRAP) */
                {

                    ui->statusBar->showMessage("Верный заголовок пакета // Tab 2"); /* если верный заголовок, тогда читаем дальше */
                    if (comparestr_3(comand1,8) && comparestr_3(trap,16)) /* 0121100154524150 - верно */
                     {
                        if (comparestr_3("05",30)) /* вход для считывания значений 0x05 */
                            ui->statusBar->showMessage("Верный вход для считывания // Tab 2");
                        {
                            /* час:минута:сек */
                            ui->label_time_3->setText(str_tab2.at(36) + (str_tab2.at(37).toLatin1()) + tr(":") +
                                                   (str_tab2.at(34).toLatin1()) + (str_tab2.at(35).toLatin1()) + tr(":") +
                                                   (str_tab2.at(32).toLatin1()) + (str_tab2.at(33).toLatin1()));

                            /* день:месяц:год */
                            ui->label_date_3->setText(str_tab2.at(38) + (str_tab2.at(39).toLatin1()) + tr(".") +
                                                   (str_tab2.at(40).toLatin1()) + (str_tab2.at(41).toLatin1()) + tr(".") +
                                                    tr("20") +(str_tab2.at(42).toLatin1()) + (str_tab2.at(43).toLatin1()));


                            int b=51; /* 51 - конечный бит первого пакета значений; 179 - конечный бит последнего пакета */

                                    for (int p=0; p<posledov_tab2.length(); p++) /* 17 - общее количество значений с датчиков */

                                    {
                                        {
                                            qDebug()<<"POSLEDOV ALL"<<posledov_tab2;
                                            qDebug()<<"POSLEDOV.at(p)"<<posledov_tab2.at(p);

                                            switch (posledov_tab2.at(p))
                                                {
                                            case 1:
                                                /* Hp10 */

                                                qDebug()<<"B-Hp10"<<b;
                                                gHp10_tab2 = FormatText_3(IEEE754_3(b));
                                                ui->lineEdit_Hp10_3->setText(gHp10_tab2);
                                                ui->lineEdit_Hp10_3->insert(" [мкЗв/ч]");
                                                b=b+8;
                                                break;

                                            case 2:

                                                /* Метан */

                                                qDebug()<<"B-Метан"<<b;
                                                gMetan_tab2 = FormatText_3(IEEE754_3(b));
                                                ui->lineEdit_metan_3->setText(gMetan_tab2);
                                                ui->lineEdit_metan_3->insert(" [об.%]");
                                                b=b+8;
                                                break;

                                            case 3:
                                                /* Фтороводородная кислота */

                                                qDebug()<<"B-Фторвод"<<b;
                                                gFtvk_tab2 = FormatText_3(IEEE754_3(b));
                                                ui->lineEdit_ftvk_3->setText(gFtvk_tab2);
                                                ui->lineEdit_ftvk_3->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 4:
                                                /*  Моноксид углерода */

                                                qDebug()<<"B-Моноксид углерода"<<b;
                                                gMonoYgl_tab2 = FormatText_3(IEEE754_3(b));
                                                ui->lineEdit_monoygl_3->setText(gMonoYgl_tab2);
                                                ui->lineEdit_monoygl_3->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 5:

                                                /*  Сероводород */

                                                qDebug()<<"B-Сероводород"<<b;
                                                gSerVod_tab2 = FormatText_3(IEEE754_3(b));
                                                ui->lineEdit_servod_3->setText( gSerVod_tab2 );
                                                ui->lineEdit_servod_3->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 6:
                                                /*  Хлор */

                                                qDebug()<<"B-Хлор"<<b;
                                                gHlor_tab2 = FormatText_3(IEEE754_3(b));
                                                ui->lineEdit_hlor_3->setText( gHlor_tab2 );
                                                ui->lineEdit_hlor_3->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 7:
                                                /*  Гексан */

                                                qDebug()<<"B-Гексан"<<b;
                                                gGeksan_tab2 = FormatText_3(IEEE754_3(b));
                                                ui->lineEdit_geksan_3->setText( gGeksan_tab2 );
                                                ui->lineEdit_geksan_3->insert(" [мг/л]");
                                                b=b+8;
                                                break;

                                            case 8:
                                                /*  Оксид серы */

                                                qDebug()<<"B-Оксид серы"<<b;
                                                gOksSer_tab2 = FormatText_3(IEEE754_3(b));
                                                ui->lineEdit_oksser_3->setText( gOksSer_tab2 );
                                                ui->lineEdit_oksser_3->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 9:

                                                /*  Оксид азота */

                                                qDebug()<<"B-Оксид азота"<<b;
                                                gOksAzot_tab2 = FormatText_3(IEEE754_3(b));
                                                ui->lineEdit_oksazot_3->setText( gOksAzot_tab2 );
                                                ui->lineEdit_oksazot_3->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 10:

                                                /*  Соляная кислота */

                                                qDebug()<<"B-Соляная кислота"<<b;
                                                gSolKisl_tab2 = FormatText_3(IEEE754_3(b));
                                                ui->lineEdit_solkisl_3->setText( gSolKisl_tab2 );
                                                ui->lineEdit_solkisl_3->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 11:
                                                /*  Пропан */

                                                qDebug()<<"B-Пропан"<<b;
                                                gPropan_tab2 = FormatText_3(IEEE754_3(b));
                                                ui->lineEdit_propan_3->setText( gPropan_tab2 );
                                                ui->lineEdit_propan_3->insert(" [об.%]");
                                                b=b+8;
                                                break;

                                            case 12:
                                                /*  Аммиак */

                                                qDebug()<<"B-Аммиак"<<b;
                                                gAmmiak_tab2 = FormatText_3(IEEE754_3(b));
                                                ui->lineEdit_amiak_3->setText( gAmmiak_tab2 );
                                                ui->lineEdit_amiak_3->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 13:
                                                /* Углекислый газ */

                                                qDebug()<<"B-Углекислый газ"<<b;
                                                gYglGaz_tab2 = FormatText_3(IEEE754_3(b));
                                                ui->lineEdit_yglgaz_3->setText( gYglGaz_tab2 );
                                                ui->lineEdit_yglgaz_3->insert(" [об.%]");
                                                b=b+8;
                                                break;

                                            case 14:
                                                /*  Иприт  (GB) */


                                                qDebug()<<"B-Иприт  (GB)"<<b;


                                                if (FormatText_3(IEEE754_3(b))=="0.0")
                                                {
                                                  ui->lineEdit_gb_3->setText("В норме");
                                                  gGB_tab2 = "0";
                                                }
                                                if (IEEE754_3(b)=="1")
                                                {
                                                  ui->lineEdit_gb_3->setText("1 порог");
                                                  gGB_tab2 = "1";
                                                }
                                                if (IEEE754_3(b)=="100")
                                                {
                                                  ui->lineEdit_gb_3->setText("2 порог");
                                                  gGB_tab2 = "100";
                                                }


                                                /* ui->lineEdit_gb->setText(FormatText(IEEE754(b))); */
                                                /* ui->lineEdit_gb->setText(IEEE754(b)); */

                                                b=b+8;
                                                break;

                                            case 15:
                                                /*  Зарин (HD)  */

                                                qDebug()<<"B-HD"<<b;
                                                if (FormatText_3(IEEE754_3(b))=="0.0")
                                                {
                                                  ui->lineEdit_hd_3->setText("В норме");
                                                  gHD_tab2 = "0";
                                                }
                                                if (FormatText_3(IEEE754_3(b))=="2.000")
                                                {
                                                  ui->lineEdit_hd_3->setText("1 порог");
                                                  gHD_tab2 = "2";
                                                }
                                                if (FormatText_3(IEEE754_3(b))=="200.0")
                                                {
                                                  ui->lineEdit_hd_3->setText("2 порог");
                                                  gHD_tab2 = "200";
                                                }
                                                /* ui->lineEdit_hd->setText(FormatText(IEEE754(b))); */
                                                b=b+8;
                                                break;

                                            case 16:
                                                /*  HCN */

                                                qDebug()<<"B-HCN"<<b;
                                                gHCN_tab2 = FormatText_3(IEEE754_3(b));
                                                ui->lineEdit_hcn_3->setText( gHCN_tab2 );
                                                ui->lineEdit_hcn_3->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 17:
                                                /*  H2CO - формальдегид */

                                                qDebug()<<"B-H2CO"<<b;
                                                gH2CO_tab2 = FormatText_3(IEEE754_3(b));
                                                ui->lineEdit_h2co_3->setText( gH2CO_tab2 );
                                                b=b+8;
                                                break;

                                            default:
                                                if (posledov_tab2.at(p-1)==0)
                                                {
                                                    b=b+8;
                                                }
                                                /* b=b+8; */
                                                break;
                                                }


                               } /* _for */
                            }/* _for */

                                    if (ui->cb_automode_3->isChecked())
                                    {
                                       SQLBDHK_3();
                                    }

                        }/* _if */
                     }


                }
    else ui->statusBar->showMessage(tr("Не верный заголовок пакета данных // Tab 2"));




    str_tab2.clear();
    listhex_tab2.clear();
    list_tab2.clear();
    ui->pushButton_getValue_3->setEnabled(true);
    ui->pushButton_getValue_3->setEnabled(true);
    all_lineedit_enable_3(true);
    monitorOff_3();

/*
    if (ui->cb_automode->isChecked())
    {
      on_pb_giveMeteo_clicked();
    }
*/
}




//========================== Tab 3

void MainWindow::serialReceived_4()  //получаем пакеты //для Tab 3
{

    if (status_tab3==0)
    {
    ui->statusBar->showMessage("Читаем данные / Tab 3");

    ba_tab3=serial_tab3->read(4);//считываем по 4 байта
    list_tab3.append(ba_tab3);
    listhex_tab3.append(ba_tab3.toHex());

    //qDebug()<<"list.append(ba)"<<list;
    //qDebug()<<"list.append(ba.toHex())"<<listhex;

    ui->statusBar->showMessage("Обмен осуществлён // Tab 3");
    }
    else
    ui->statusBar->showMessage("Подключение отсутствует. Не удалось serialReceived // Tab 3");
}

void MainWindow::ComConnect_4() //инициализируем порт //Tab 3
{

    serial_tab3 = new QSerialPort(this);
    serial_tab3->setPortName(ui->comboBox_4->currentText());
    serial_tab3->setBaudRate(QSerialPort::Baud9600);
    serial_tab3->setDataBits(QSerialPort::Data8);
    serial_tab3->setParity(QSerialPort::NoParity);
    serial_tab3->setStopBits(QSerialPort::OneStop);
    serial_tab3->setFlowControl(QSerialPort::NoFlowControl);
    if (serial_tab3->open(QIODevice::ReadWrite))
    {
        //qDebug()<< "COMPORT OPEN!";
        status_tab3=0;
        portOpenStyleSheet_4();
        connect(serial_tab3, SIGNAL(readyRead()),this,SLOT(serialReceived_4()));
        ui->statusBar->showMessage("Соединение установлено / Tab 3");
        monitorOff_4();

    }
    else
    {
        //qDebug()<<"COMPORT CLOSE!";
        status_tab3=1;
        portCloseStyleSheet_4();
        ui->statusBar->showMessage("Соединение отсутствует / Tab 3");
        serial_tab3->close();


    }

}

bool MainWindow::getType_4() /* Запрос типов датчиков // Для Tab 3 */
{
    list_tab3.clear();
    listhex_tab3.clear();

    char data[8]; //0x54 0x52 0x41 0x50 1 0x08 0x81 0x08
    data[0]=0x54;
    data[1]=0x52;
    data[2]=0x41;
    data[3]=0x50;
    data[4]=0x01; //1 = 0x01
    data[5]=0x08;
    data[6]=0x81;
    data[7]=0x08;

   if (serial_tab3->isOpen()) //если порт открыт
    {
    serial_tab3->write(data,8);
        if (serial_tab3->waitForBytesWritten(3000)) //ждем пока не запишутся все данные
            return true;
        else
            return false;
    }
   else                   //если порт закрыт
       return false;
}

void MainWindow::monitorOff_4() //Выключить передачу каждую минуту //для Tab 2
{

    list_tab3.clear();
    listhex_tab3.clear();

    if (status_tab3==0) //status=0 - подключение установленно
    {

    ui->statusBar->showMessage("Отключаем мониторинг каждую минуту / Tab 3");
    char data[9]; //0x54 0x52 0x41 0x50 2 0x21 0x10 0x00 0x01


    data[0]=0x54;
    data[1]=0x52;
    data[2]=0x41;
    data[3]=0x50;
    data[4]=0x02; //2 = 0x02
    data[5]=0x21;
    data[6]=0x10;
    data[7]=0x00;
    data[8]=0x01;


    serial_tab3->write(data,9);

    serial_tab3->waitForBytesWritten(500);
    qDebug()<<"Пакет послан monitorOff / Tab 3";
    ui->statusBar->showMessage("Мониторинг каждую минуту - отключить - команда послана / Tab 3");
    }
    else ui->statusBar->showMessage(tr("Соединение отсутствует. Не удалось monitorOff / Tab 3"));

    listhex_tab3.clear();
}

void MainWindow::monitorOn_4() //Включить передачу каждую минуту
{
    char data[9]; //0x54 0x52 0x41 0x50 2 0x00 0x00 0x00 0x00

    list_tab3.clear();
    listhex_tab3.clear();

    data[0]=0x54;
    data[1]=0x52;
    data[2]=0x41;
    data[3]=0x50;
    data[4]=0x02; //2 = 0x02
    data[5]=0x00;
    data[6]=0x00;
    data[7]=0x00;
    data[8]=0x00;
    serial_tab3->write(data,9);
    serial_tab3->waitForBytesWritten(500);
    qDebug()<<"Пакет послан monitorOn // Tab 3";
    ui->statusBar->showMessage("Мониторинг каждую минуту включён / Tab 3");

    listhex_tab3.clear();
}

void MainWindow::getlist_4() /* парсим Идент. // Tab 3*/
{

    str_tab3=listhex_tab3.join(""); /* соединяем в строку весь HEX код */
    posledov_tab3.clear();


    if (comparestr_4(trap,0)) /* заголовок верный (TRAP) */
                {
                    ui->statusBar->showMessage("Верный заголовок пакета / Tab 3"); //если верный заголовок, тогда читаем дальше
                        qDebug()<<str_tab3;
                    /* Верный формат времени SEK SEC MIN HOUR */
                    if (/*(comparestr(sek,8))&*/comparestr_4(sec,16)&&comparestr_4(min,24)&&comparestr_4(hour,32))
                    {
                        ui->statusBar->showMessage("Верный заголовок времени / Tab 3");
                        ui->label_time_4->setVisible(true);
                        ui->label_time_4->setText("Время");
                    }
                    /* Верный формат даты MOUNTH DAY YEAR */
                    if ((comparestr_4(mounth,42))&&comparestr_4(day,54)&&comparestr_4(year,62))
                    {
                        ui->statusBar->showMessage("Верный заголовок даты / Tab 3");
                        ui->label_date_4->setVisible(true);
                        ui->label_date_4->setText("Дата");
                    }


                        for (int i=72; i<=436/*374*/; i++) //начинаем шагать с 72 по 1 биту и проверять на совпадения
                        {
                    /* Верный формат МЭД */
                    if ((comparestr_4(hp10,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'МЭД' // Tab 3");
                        ui->label_hp10_4->setVisible(true);
                        ui->label_hp10_4->setText("МЭД");
                        ui->lineEdit_Hp10_4->setVisible(true);
                        i=i+hp10.length()-1;
                        posledov_tab3.append(1);
                    }
                    /* Верный формат Метан */
                    if ((comparestr_4(metan,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Метан' // Tab 3");
                        ui->label_metan_4->setVisible(true);
                        ui->label_metan_4->setText("Метан, этан");
                        ui->lineEdit_metan_4->setVisible(true);
                        i=i+metan.length()-1;
                        posledov_tab3.append(2);
                    }
                    /* Верный формат Фторовод. кислота */
                    if ((comparestr_4(ftorkisl,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Фт-вод. кислота' // Tab 3");
                        ui->label_ftvk_4->setVisible(true);
                        ui->label_ftvk_4->setText("Фт-вод. кислота");
                        ui->lineEdit_ftvk_4->setVisible(true);
                        i=i+ftorkisl.length()-1;
                        posledov_tab3.append(3);
                    }
                    /* Верный формат Моноксид углерода */
                    if ((comparestr_4(monoygl,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Моноксид углерода' // Tab 3");
                        ui->label_monoygl_4->setVisible(true);
                        ui->label_monoygl_4->setText("Моноксид углерода");
                        ui->lineEdit_monoygl_4->setVisible(true);
                        i=i+monoygl.length()-1;
                        posledov_tab3.append(4);
                    }
                    /* Верный формат Сероводород */
                    if ((comparestr_4(servod,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Сероводород' // Tab 3");
                        ui->label_servod_4->setVisible(true);
                        ui->label_servod_4->setText("Сероводород");
                        ui->lineEdit_servod_4->setVisible(true);
                        i=i+servod.length()-1;
                        posledov_tab3.append(5);
                    }
                    /* Верный формат Хлор */
                    if ((comparestr_4(hlor,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Хлор' // Tab 3");
                        ui->label_hlor_4->setVisible(true);
                        ui->label_hlor_4->setText("Хлор");
                        ui->lineEdit_hlor_4->setVisible(true);
                        i=i+hlor.length()-1;
                        posledov_tab3.append(6);
                    }
                    /* Верный формат Гексан */
                    if ((comparestr_4(geksan,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Гексан' // Tab 3");
                        ui->label_geksan_4->setVisible(true);
                        ui->label_geksan_4->setText("Пентан, гексан");
                        ui->lineEdit_geksan_4->setVisible(true);
                        i=i+geksan.length()-1;
                        posledov_tab3.append(7);
                    }
                    /* Верный формат Оксид серы */
                    if ((comparestr_4(oksser,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Оксид серы' // Tab 3");
                        ui->label_oksser_4->setVisible(true);
                        ui->label_oksser_4->setText("Оксид серы");
                        ui->lineEdit_oksser_4->setVisible(true);
                        i=i+oksser.length()-1;
                        posledov_tab3.append(8);
                    }
                    /* Верный формат Оксид азота */
                    if ((comparestr_4(oksazot,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Оксид азота' // Tab 3");
                        ui->label_oksazot_4->setVisible(true);
                        ui->label_oksazot_4->setText("Оксид азота");
                        ui->lineEdit_oksazot_4->setVisible(true);
                        i=i+oksazot.length()-1;
                        posledov_tab3.append(9);
                    }
                    /* Верный формат Соляная кислота */
                    if ((comparestr_4(solkisl,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Соляная кислота' // Tab 3");
                        ui->label_solkisl_4->setVisible(true);
                        ui->label_solkisl_4->setText("Соляная кислота");
                        ui->lineEdit_solkisl_4->setVisible(true);
                        i=i+solkisl.length()-1;
                        posledov_tab3.append(10);
                    }
                    /* Верный формат Пропан */
                    if ((comparestr_4(propan,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Пропан' // Tab 3");
                        ui->label_propan_4->setVisible(true);
                        ui->label_propan_4->setText("Пропан, бутан");
                        ui->lineEdit_propan_4->setVisible(true);
                        i=i+propan.length()-1;
                        posledov_tab3.append(11);
                    }
                    /* Верный формат Аммиак */
                    if ((comparestr_4(amiak,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Аммиак' // Tab 3");
                        ui->label_amiak_4->setVisible(true);
                        ui->label_amiak_4->setText("Аммиак");
                        ui->lineEdit_amiak_4->setVisible(true);
                        i=i+amiak.length()-1;
                        posledov_tab3.append(12);
                    }
                    /* Верный формат Углекислый газ */
                    if ((comparestr_4(yglgaz,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Углекислый газ' // Tab 3");
                        ui->label_yglgaz_4->setVisible(true);
                        ui->label_yglgaz_4->setText("Углекислый газ");
                        ui->lineEdit_yglgaz_4->setVisible(true);
                        i=i+yglgaz.length()-1;
                        posledov_tab3.append(13);
                    }
                    /* Верный формат GB */
                    if ((comparestr_4(gb,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'GB' // Tab 3");
                        ui->label_gb_4->setVisible(true);
                        ui->label_gb_4->setText("GB");
                        ui->lineEdit_gb_4->setVisible(true);
                        i=i+gb.length()-1;
                        posledov_tab3.append(14);
                    }
                    /* Верный формат HD */
                    if ((comparestr_4(hd,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'HD' // Tab 3");
                        ui->label_hd_4->setVisible(true);
                        ui->label_hd_4->setText("HD");
                        ui->lineEdit_hd_4->setVisible(true);
                        i=i+hd.length()-1;
                        posledov_tab3.append(15);
                    }
                    /* Верный формат HCN */
                    if ((comparestr_4(hcn,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'HCN' // Tab 3");
                        ui->label_hcn_4->setVisible(true);
                        ui->label_hcn_4->setText("Синильная кислота");
                        ui->lineEdit_hcn_4->setVisible(true);
                        i=i+hcn.length()-1;
                        posledov_tab3.append(16);
                    }
                    /* Верный формат H2CO */
                    if ((comparestr_4(h2co,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'H2CO' // Tab 3");
                        ui->label_h2co_4->setVisible(true);
                        ui->label_h2co_4->setText("Формальдегид");
                        ui->lineEdit_h2co_4->setVisible(true);
                        i=i+h2co.length()-1;
                        posledov_tab3.append(17);
                    }
                    if (comparestr_4(zpt,i)) //если нет датчика - запятая
                    {
                        posledov_tab3.append(0);
                    }
              ui->pushButton_getValue_4->setVisible(true);
                        }
                     groupbox_4(true); /* "зажигаем" все GroupBox */
    }

    else
    {
        ui->statusBar->showMessage("Неверный заголовок пакета. Переподключитесь // Tab 3");
        ui->pbtn_type_3->setEnabled(true);

        str_tab3.clear();
        list_tab3.clear();
        listhex_tab3.clear();
    }

    str_tab3.clear();
    list_tab3.clear();
    listhex_tab3.clear();

}

bool MainWindow::Value_4() /* парсим значения // для Tab 3*/
{
    str_tab3.clear();
    str_tab3=listhex_tab3.join("");
    qDebug()<<str_tab3;
    if (comparestr_4(trap,0)) /* заголовок верный (TRAP) */
                {

                    ui->statusBar->showMessage("Верный заголовок пакета // Tab 3"); /* если верный заголовок, тогда читаем дальше */
                    if (comparestr_4(comand1,8) && comparestr_4(trap,16)) /* 0121100154524150 - верно */
                     {
                        if (comparestr_4("05",30)) /* вход для считывания значений 0x05 */
                            ui->statusBar->showMessage("Верный вход для считывания // Tab 3");
                        {
                            /* час:минута:сек */
                            ui->label_time_4->setText(str_tab3.at(36) + (str_tab3.at(37).toLatin1()) + tr(":") +
                                                   (str_tab3.at(34).toLatin1()) + (str_tab3.at(35).toLatin1()) + tr(":") +
                                                   (str_tab3.at(32).toLatin1()) + (str_tab3.at(33).toLatin1()));

                            /* день:месяц:год */
                            ui->label_date_4->setText(str_tab3.at(38) + (str_tab3.at(39).toLatin1()) + tr(".") +
                                                   (str_tab3.at(40).toLatin1()) + (str_tab3.at(41).toLatin1()) + tr(".") +
                                                    tr("20") +(str_tab3.at(42).toLatin1()) + (str_tab3.at(43).toLatin1()));


                            int b=51; /* 51 - конечный бит первого пакета значений; 179 - конечный бит последнего пакета */

                                    for (int p=0; p<posledov_tab3.length(); p++) /* 17 - общее количество значений с датчиков */

                                    {
                                        {
                                            qDebug()<<"POSLEDOV ALL"<<posledov_tab3;
                                            qDebug()<<"POSLEDOV.at(p)"<<posledov_tab3.at(p);

                                            switch (posledov_tab3.at(p))
                                                {
                                            case 1:
                                                /* Hp10 */

                                                qDebug()<<"B-Hp10"<<b;
                                                gHp10_tab3 = FormatText_4(IEEE754_4(b));
                                                ui->lineEdit_Hp10_4->setText(gHp10_tab3);
                                                ui->lineEdit_Hp10_4->insert(" [мкЗв/ч]");
                                                b=b+8;
                                                break;

                                            case 2:

                                                /* Метан */

                                                qDebug()<<"B-Метан"<<b;
                                                gMetan_tab3 = FormatText_4(IEEE754_4(b));
                                                ui->lineEdit_metan_4->setText(gMetan_tab3);
                                                ui->lineEdit_metan_4->insert(" [об.%]");
                                                b=b+8;
                                                break;

                                            case 3:
                                                /* Фтороводородная кислота */

                                                qDebug()<<"B-Фторвод"<<b;
                                                gFtvk_tab3 = FormatText_4(IEEE754_4(b));
                                                ui->lineEdit_ftvk_4->setText(gFtvk_tab3);
                                                ui->lineEdit_ftvk_4->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 4:
                                                /*  Моноксид углерода */

                                                qDebug()<<"B-Моноксид углерода"<<b;
                                                gMonoYgl_tab3 = FormatText_4(IEEE754_4(b));
                                                ui->lineEdit_monoygl_4->setText(gMonoYgl_tab3);
                                                ui->lineEdit_monoygl_4->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 5:

                                                /*  Сероводород */

                                                qDebug()<<"B-Сероводород"<<b;
                                                gSerVod_tab3 = FormatText_4(IEEE754_4(b));
                                                ui->lineEdit_servod_4->setText( gSerVod_tab3 );
                                                ui->lineEdit_servod_4->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 6:
                                                /*  Хлор */

                                                qDebug()<<"B-Хлор"<<b;
                                                gHlor_tab3 = FormatText_4(IEEE754_4(b));
                                                ui->lineEdit_hlor_4->setText( gHlor_tab3 );
                                                ui->lineEdit_hlor_4->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 7:
                                                /*  Гексан */

                                                qDebug()<<"B-Гексан"<<b;
                                                gGeksan_tab3 = FormatText_4(IEEE754_4(b));
                                                ui->lineEdit_geksan_4->setText( gGeksan_tab3 );
                                                ui->lineEdit_geksan_4->insert(" [мг/л]");
                                                b=b+8;
                                                break;

                                            case 8:
                                                /*  Оксид серы */

                                                qDebug()<<"B-Оксид серы"<<b;
                                                gOksSer_tab3 = FormatText_4(IEEE754_4(b));
                                                ui->lineEdit_oksser_4->setText( gOksSer_tab3 );
                                                ui->lineEdit_oksser_4->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 9:

                                                /*  Оксид азота */

                                                qDebug()<<"B-Оксид азота"<<b;
                                                gOksAzot_tab3 = FormatText_4(IEEE754_4(b));
                                                ui->lineEdit_oksazot_4->setText( gOksAzot_tab3 );
                                                ui->lineEdit_oksazot_4->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 10:

                                                /*  Соляная кислота */

                                                qDebug()<<"B-Соляная кислота"<<b;
                                                gSolKisl_tab3 = FormatText_4(IEEE754_4(b));
                                                ui->lineEdit_solkisl_4->setText( gSolKisl_tab3 );
                                                ui->lineEdit_solkisl_4->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 11:
                                                /*  Пропан */

                                                qDebug()<<"B-Пропан"<<b;
                                                gPropan_tab3 = FormatText_4(IEEE754_4(b));
                                                ui->lineEdit_propan_4->setText( gPropan_tab3 );
                                                ui->lineEdit_propan_4->insert(" [об.%]");
                                                b=b+8;
                                                break;

                                            case 12:
                                                /*  Аммиак */

                                                qDebug()<<"B-Аммиак"<<b;
                                                gAmmiak_tab3 = FormatText_4(IEEE754_4(b));
                                                ui->lineEdit_amiak_4->setText( gAmmiak_tab3 );
                                                ui->lineEdit_amiak_4->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 13:
                                                /* Углекислый газ */

                                                qDebug()<<"B-Углекислый газ"<<b;
                                                gYglGaz_tab3 = FormatText_4(IEEE754_4(b));
                                                ui->lineEdit_yglgaz_4->setText( gYglGaz_tab3 );
                                                ui->lineEdit_yglgaz_4->insert(" [об.%]");
                                                b=b+8;
                                                break;

                                            case 14:
                                                /*  Иприт  (GB) */


                                                qDebug()<<"B-Иприт  (GB)"<<b;


                                                if (FormatText_4(IEEE754_4(b))=="0.0")
                                                {
                                                  ui->lineEdit_gb_4->setText("В норме");
                                                  gGB_tab3 = "0";
                                                }
                                                if (IEEE754_4(b)=="1")
                                                {
                                                  ui->lineEdit_gb_4->setText("1 порог");
                                                  gGB_tab3 = "1";
                                                }
                                                if (IEEE754_4(b)=="100")
                                                {
                                                  ui->lineEdit_gb_4->setText("2 порог");
                                                  gGB_tab3 = "100";
                                                }


                                                /* ui->lineEdit_gb->setText(FormatText(IEEE754(b))); */
                                                /* ui->lineEdit_gb->setText(IEEE754(b)); */

                                                b=b+8;
                                                break;

                                            case 15:
                                                /*  Зарин (HD)  */

                                                qDebug()<<"B-HD"<<b;
                                                if (FormatText_4(IEEE754_4(b))=="0.0")
                                                {
                                                  ui->lineEdit_hd_4->setText("В норме");
                                                  gHD_tab3 = "0";
                                                }
                                                if (FormatText_4(IEEE754_4(b))=="2.000")
                                                {
                                                  ui->lineEdit_hd_4->setText("1 порог");
                                                  gHD_tab3 = "2";
                                                }
                                                if (FormatText_4(IEEE754_4(b))=="200.0")
                                                {
                                                  ui->lineEdit_hd_4->setText("2 порог");
                                                  gHD_tab3 = "200";
                                                }
                                                /* ui->lineEdit_hd->setText(FormatText(IEEE754(b))); */
                                                b=b+8;
                                                break;

                                            case 16:
                                                /*  HCN */

                                                qDebug()<<"B-HCN"<<b;
                                                gHCN_tab3 = FormatText_4(IEEE754_4(b));
                                                ui->lineEdit_hcn_4->setText( gHCN_tab3 );
                                                ui->lineEdit_hcn_4->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 17:
                                                /*  H2CO - формальдегид */

                                                qDebug()<<"B-H2CO"<<b;
                                                gH2CO_tab3 = FormatText_4(IEEE754_4(b));
                                                ui->lineEdit_h2co_4->setText( gH2CO_tab3 );
                                                b=b+8;
                                                break;

                                            default:
                                                if (posledov_tab3.at(p-1)==0)
                                                {
                                                    b=b+8;
                                                }
                                                /* b=b+8; */
                                                break;
                                                }


                               } /* _for */
                            }/* _for */

                                    if (ui->cb_automode_4->isChecked())
                                    {
                                       SQLBDHK_4();
                                    }

                        }/* _if */
                     }


                }
    else ui->statusBar->showMessage(tr("Не верный заголовок пакета данных // Tab 3"));




    str_tab3.clear();
    listhex_tab3.clear();
    list_tab3.clear();
    ui->pushButton_getValue_4->setEnabled(true);
    ui->pushButton_getValue_4->setEnabled(true);
    all_lineedit_enable_4(true);
    monitorOff_4();

/*
    if (ui->cb_automode->isChecked())
    {
      on_pb_giveMeteo_clicked();
    }
*/
}



//========================== Tab 4

void MainWindow::serialReceived_5()  //получаем пакеты //для Tab 4
{

    if (status_tab3==0)
    {
    ui->statusBar->showMessage("Читаем данные / Tab 4");

    ba_tab4=serial_tab4->read(4);//считываем по 4 байта
    list_tab4.append(ba_tab4);
    listhex_tab4.append(ba_tab4.toHex());

    //qDebug()<<"list.append(ba)"<<list;
    //qDebug()<<"list.append(ba.toHex())"<<listhex;

    ui->statusBar->showMessage("Обмен осуществлён // Tab 4");
    }
    else
    ui->statusBar->showMessage("Подключение отсутствует. Не удалось serialReceived // Tab 4");
}

void MainWindow::ComConnect_5() //инициализируем порт //Tab 4
{

    serial_tab4 = new QSerialPort(this);
    serial_tab4->setPortName(ui->comboBox_5->currentText());
    serial_tab4->setBaudRate(QSerialPort::Baud9600);
    serial_tab4->setDataBits(QSerialPort::Data8);
    serial_tab4->setParity(QSerialPort::NoParity);
    serial_tab4->setStopBits(QSerialPort::OneStop);
    serial_tab4->setFlowControl(QSerialPort::NoFlowControl);
    if (serial_tab4->open(QIODevice::ReadWrite))
    {
        //qDebug()<< "COMPORT OPEN!";
        status_tab4=0;
        portOpenStyleSheet_5();
        connect(serial_tab4, SIGNAL(readyRead()),this,SLOT(serialReceived_5()));
        ui->statusBar->showMessage("Соединение установлено / Tab 4");
        monitorOff_5();

    }
    else
    {
        //qDebug()<<"COMPORT CLOSE!";
        status_tab4=1;
        portCloseStyleSheet_5();
        ui->statusBar->showMessage("Соединение отсутствует / Tab 4");
        serial_tab4->close();


    }

}

bool MainWindow::getType_5() /* Запрос типов датчиков // Для Tab 4 */
{
    list_tab4.clear();
    listhex_tab4.clear();

    char data[8]; //0x54 0x52 0x41 0x50 1 0x08 0x81 0x08
    data[0]=0x54;
    data[1]=0x52;
    data[2]=0x41;
    data[3]=0x50;
    data[4]=0x01; //1 = 0x01
    data[5]=0x08;
    data[6]=0x81;
    data[7]=0x08;

   if (serial_tab4->isOpen()) //если порт открыт
    {
    serial_tab4->write(data,8);
        if (serial_tab4->waitForBytesWritten(3000)) //ждем пока не запишутся все данные
            return true;
        else
            return false;
    }
   else                   //если порт закрыт
       return false;
}

void MainWindow::monitorOff_5() //Выключить передачу каждую минуту //для Tab 2
{

    list_tab4.clear();
    listhex_tab4.clear();

    if (status_tab4==0) //status=0 - подключение установленно
    {

    ui->statusBar->showMessage("Отключаем мониторинг каждую минуту / Tab 4");
    char data[9]; //0x54 0x52 0x41 0x50 2 0x21 0x10 0x00 0x01


    data[0]=0x54;
    data[1]=0x52;
    data[2]=0x41;
    data[3]=0x50;
    data[4]=0x02; //2 = 0x02
    data[5]=0x21;
    data[6]=0x10;
    data[7]=0x00;
    data[8]=0x01;


    serial_tab4->write(data,9);

    serial_tab4->waitForBytesWritten(500);
    qDebug()<<"Пакет послан monitorOff / Tab 4";
    ui->statusBar->showMessage("Мониторинг каждую минуту - отключить - команда послана / Tab 4");
    }
    else ui->statusBar->showMessage(tr("Соединение отсутствует. Не удалось monitorOff / Tab 4"));

    listhex_tab4.clear();
}

void MainWindow::monitorOn_5() //Включить передачу каждую минуту
{
    char data[9]; //0x54 0x52 0x41 0x50 2 0x00 0x00 0x00 0x00

    list_tab4.clear();
    listhex_tab4.clear();

    data[0]=0x54;
    data[1]=0x52;
    data[2]=0x41;
    data[3]=0x50;
    data[4]=0x02; //2 = 0x02
    data[5]=0x00;
    data[6]=0x00;
    data[7]=0x00;
    data[8]=0x00;
    serial_tab4->write(data,9);
    serial_tab4->waitForBytesWritten(500);
    qDebug()<<"Пакет послан monitorOn // Tab 4";
    ui->statusBar->showMessage("Мониторинг каждую минуту включён / Tab 4");

    listhex_tab4.clear();
}

void MainWindow::getlist_5() /* парсим Идент. // Tab 4 */
{

    str_tab4=listhex_tab4.join(""); /* соединяем в строку весь HEX код */
    posledov_tab4.clear();


    if (comparestr_5(trap,0)) /* заголовок верный (TRAP) */
                {
                    ui->statusBar->showMessage("Верный заголовок пакета / Tab 4"); //если верный заголовок, тогда читаем дальше
                        qDebug()<<str_tab4;
                    /* Верный формат времени SEK SEC MIN HOUR */
                    if (/*(comparestr(sek,8))&*/comparestr_5(sec,16)&&comparestr_5(min,24)&&comparestr_5(hour,32))
                    {
                        ui->statusBar->showMessage("Верный заголовок времени / Tab 4");
                        ui->label_time_5->setVisible(true);
                        ui->label_time_5->setText("Время");
                    }
                    /* Верный формат даты MOUNTH DAY YEAR */
                    if ((comparestr_5(mounth,42))&&comparestr_5(day,54)&&comparestr_5(year,62))
                    {
                        ui->statusBar->showMessage("Верный заголовок даты / Tab 4");
                        ui->label_date_5->setVisible(true);
                        ui->label_date_5->setText("Дата");
                    }


                        for (int i=72; i<=436/*374*/; i++) //начинаем шагать с 72 по 1 биту и проверять на совпадения
                        {
                    /* Верный формат МЭД */
                    if ((comparestr_5(hp10,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'МЭД' // Tab 4");
                        ui->label_hp10_5->setVisible(true);
                        ui->label_hp10_5->setText("МЭД");
                        ui->lineEdit_Hp10_5->setVisible(true);
                        i=i+hp10.length()-1;
                        posledov_tab4.append(1);
                    }
                    /* Верный формат Метан */
                    if ((comparestr_5(metan,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Метан' // Tab 4");
                        ui->label_metan_5->setVisible(true);
                        ui->label_metan_5->setText("Метан, этан");
                        ui->lineEdit_metan_5->setVisible(true);
                        i=i+metan.length()-1;
                        posledov_tab4.append(2);
                    }
                    /* Верный формат Фторовод. кислота */
                    if ((comparestr_5(ftorkisl,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Фт-вод. кислота' // Tab 4");
                        ui->label_ftvk_5->setVisible(true);
                        ui->label_ftvk_5->setText("Фт-вод. кислота");
                        ui->lineEdit_ftvk_5->setVisible(true);
                        i=i+ftorkisl.length()-1;
                        posledov_tab4.append(3);
                    }
                    /* Верный формат Моноксид углерода */
                    if ((comparestr_5(monoygl,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Моноксид углерода' // Tab 4");
                        ui->label_monoygl_5->setVisible(true);
                        ui->label_monoygl_5->setText("Моноксид углерода");
                        ui->lineEdit_monoygl_5->setVisible(true);
                        i=i+monoygl.length()-1;
                        posledov_tab4.append(4);
                    }
                    /* Верный формат Сероводород */
                    if ((comparestr_5(servod,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Сероводород' // Tab 4");
                        ui->label_servod_5->setVisible(true);
                        ui->label_servod_5->setText("Сероводород");
                        ui->lineEdit_servod_5->setVisible(true);
                        i=i+servod.length()-1;
                        posledov_tab4.append(5);
                    }
                    /* Верный формат Хлор */
                    if ((comparestr_5(hlor,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Хлор' // Tab 4");
                        ui->label_hlor_5->setVisible(true);
                        ui->label_hlor_5->setText("Хлор");
                        ui->lineEdit_hlor_5->setVisible(true);
                        i=i+hlor.length()-1;
                        posledov_tab4.append(6);
                    }
                    /* Верный формат Гексан */
                    if ((comparestr_5(geksan,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Гексан' // Tab 4");
                        ui->label_geksan_5->setVisible(true);
                        ui->label_geksan_5->setText("Пентан, гексан");
                        ui->lineEdit_geksan_5->setVisible(true);
                        i=i+geksan.length()-1;
                        posledov_tab4.append(7);
                    }
                    /* Верный формат Оксид серы */
                    if ((comparestr_5(oksser,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Оксид серы' // Tab 4");
                        ui->label_oksser_5->setVisible(true);
                        ui->label_oksser_5->setText("Оксид серы");
                        ui->lineEdit_oksser_5->setVisible(true);
                        i=i+oksser.length()-1;
                        posledov_tab4.append(8);
                    }
                    /* Верный формат Оксид азота */
                    if ((comparestr_5(oksazot,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Оксид азота' // Tab 4");
                        ui->label_oksazot_5->setVisible(true);
                        ui->label_oksazot_5->setText("Оксид азота");
                        ui->lineEdit_oksazot_5->setVisible(true);
                        i=i+oksazot.length()-1;
                        posledov_tab4.append(9);
                    }
                    /* Верный формат Соляная кислота */
                    if ((comparestr_5(solkisl,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Соляная кислота' // Tab 4");
                        ui->label_solkisl_5->setVisible(true);
                        ui->label_solkisl_5->setText("Соляная кислота");
                        ui->lineEdit_solkisl_5->setVisible(true);
                        i=i+solkisl.length()-1;
                        posledov_tab4.append(10);
                    }
                    /* Верный формат Пропан */
                    if ((comparestr_5(propan,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Пропан' // Tab 4");
                        ui->label_propan_5->setVisible(true);
                        ui->label_propan_5->setText("Пропан, бутан");
                        ui->lineEdit_propan_5->setVisible(true);
                        i=i+propan.length()-1;
                        posledov_tab4.append(11);
                    }
                    /* Верный формат Аммиак */
                    if ((comparestr_5(amiak,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Аммиак' // Tab 4");
                        ui->label_amiak_5->setVisible(true);
                        ui->label_amiak_5->setText("Аммиак");
                        ui->lineEdit_amiak_5->setVisible(true);
                        i=i+amiak.length()-1;
                        posledov_tab4.append(12);
                    }
                    /* Верный формат Углекислый газ */
                    if ((comparestr_5(yglgaz,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'Углекислый газ' // Tab 4");
                        ui->label_yglgaz_5->setVisible(true);
                        ui->label_yglgaz_5->setText("Углекислый газ");
                        ui->lineEdit_yglgaz_5->setVisible(true);
                        i=i+yglgaz.length()-1;
                        posledov_tab4.append(13);
                    }
                    /* Верный формат GB */
                    if ((comparestr_5(gb,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'GB' // Tab 4");
                        ui->label_gb_5->setVisible(true);
                        ui->label_gb_5->setText("GB");
                        ui->lineEdit_gb_5->setVisible(true);
                        i=i+gb.length()-1;
                        posledov_tab4.append(14);
                    }
                    /* Верный формат HD */
                    if ((comparestr_5(hd,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'HD' // Tab 4");
                        ui->label_hd_5->setVisible(true);
                        ui->label_hd_5->setText("HD");
                        ui->lineEdit_hd_5->setVisible(true);
                        i=i+hd.length()-1;
                        posledov_tab4.append(15);
                    }
                    /* Верный формат HCN */
                    if ((comparestr_5(hcn,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'HCN' // Tab 4");
                        ui->label_hcn_5->setVisible(true);
                        ui->label_hcn_5->setText("Синильная кислота");
                        ui->lineEdit_hcn_5->setVisible(true);
                        i=i+hcn.length()-1;
                        posledov_tab4.append(16);
                    }
                    /* Верный формат H2CO */
                    if ((comparestr_5(h2co,i)))
                    {
                        ui->statusBar->showMessage("Верный формат 'H2CO' // Tab 4");
                        ui->label_h2co_5->setVisible(true);
                        ui->label_h2co_5->setText("Формальдегид");
                        ui->lineEdit_h2co_5->setVisible(true);
                        i=i+h2co.length()-1;
                        posledov_tab4.append(17);
                    }
                    if (comparestr_5(zpt,i)) //если нет датчика - запятая
                    {
                        posledov_tab4.append(0);
                    }
              ui->pushButton_getValue_5->setVisible(true);
                        }
                     groupbox_5(true); /* "зажигаем" все GroupBox */
    }

    else
    {
        ui->statusBar->showMessage("Неверный заголовок пакета. Переподключитесь // Tab 4");
        ui->pbtn_type_4->setEnabled(true);

        str_tab4.clear();
        list_tab4.clear();
        listhex_tab4.clear();
    }

    str_tab4.clear();
    list_tab4.clear();
    listhex_tab4.clear();

}

bool MainWindow::Value_5() /* парсим значения // для Tab 4 */
{
    str_tab4.clear();
    str_tab4=listhex_tab4.join("");
    qDebug()<<str_tab4;
    if (comparestr_5(trap,0)) /* заголовок верный (TRAP) */
                {

                    ui->statusBar->showMessage("Верный заголовок пакета // Tab 4"); /* если верный заголовок, тогда читаем дальше */
                    if (comparestr_5(comand1,8) && comparestr_5(trap,16)) /* 0121100154524150 - верно */
                     {
                        if (comparestr_5("05",30)) /* вход для считывания значений 0x05 */
                            ui->statusBar->showMessage("Верный вход для считывания // Tab 4");
                        {
                            /* час:минута:сек */
                            ui->label_time_5->setText(str_tab4.at(36) + (str_tab4.at(37).toLatin1()) + tr(":") +
                                                   (str_tab4.at(34).toLatin1()) + (str_tab4.at(35).toLatin1()) + tr(":") +
                                                   (str_tab4.at(32).toLatin1()) + (str_tab4.at(33).toLatin1()));

                            /* день:месяц:год */
                            ui->label_date_5->setText(str_tab4.at(38) + (str_tab4.at(39).toLatin1()) + tr(".") +
                                                   (str_tab4.at(40).toLatin1()) + (str_tab4.at(41).toLatin1()) + tr(".") +
                                                    tr("20") +(str_tab4.at(42).toLatin1()) + (str_tab4.at(43).toLatin1()));


                            int b=51; /* 51 - конечный бит первого пакета значений; 179 - конечный бит последнего пакета */

                                    for (int p=0; p<posledov_tab4.length(); p++) /* 17 - общее количество значений с датчиков */

                                    {
                                        {
                                            qDebug()<<"POSLEDOV ALL"<<posledov_tab4;
                                            qDebug()<<"POSLEDOV.at(p)"<<posledov_tab4.at(p);

                                            switch (posledov_tab4.at(p))
                                                {
                                            case 1:
                                                /* Hp10 */

                                                qDebug()<<"B-Hp10"<<b;
                                                gHp10_tab4 = FormatText_5(IEEE754_5(b));
                                                ui->lineEdit_Hp10_5->setText(gHp10_tab4);
                                                ui->lineEdit_Hp10_5->insert(" [мкЗв/ч]");
                                                b=b+8;
                                                break;

                                            case 2:

                                                /* Метан */

                                                qDebug()<<"B-Метан"<<b;
                                                gMetan_tab4 = FormatText_5(IEEE754_5(b));
                                                ui->lineEdit_metan_5->setText(gMetan_tab4);
                                                ui->lineEdit_metan_5->insert(" [об.%]");
                                                b=b+8;
                                                break;

                                            case 3:
                                                /* Фтороводородная кислота */

                                                qDebug()<<"B-Фторвод"<<b;
                                                gFtvk_tab4 = FormatText_5(IEEE754_5(b));
                                                ui->lineEdit_ftvk_5->setText(gFtvk_tab4);
                                                ui->lineEdit_ftvk_5->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 4:
                                                /*  Моноксид углерода */

                                                qDebug()<<"B-Моноксид углерода"<<b;
                                                gMonoYgl_tab4 = FormatText_5(IEEE754_5(b));
                                                ui->lineEdit_monoygl_5->setText(gMonoYgl_tab4);
                                                ui->lineEdit_monoygl_5->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 5:

                                                /*  Сероводород */

                                                qDebug()<<"B-Сероводород"<<b;
                                                gSerVod_tab4 = FormatText_5(IEEE754_5(b));
                                                ui->lineEdit_servod_5->setText( gSerVod_tab4 );
                                                ui->lineEdit_servod_5->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 6:
                                                /*  Хлор */

                                                qDebug()<<"B-Хлор"<<b;
                                                gHlor_tab4 = FormatText_5(IEEE754_5(b));
                                                ui->lineEdit_hlor_5->setText( gHlor_tab4 );
                                                ui->lineEdit_hlor_5->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 7:
                                                /*  Гексан */

                                                qDebug()<<"B-Гексан"<<b;
                                                gGeksan_tab4 = FormatText_5(IEEE754_5(b));
                                                ui->lineEdit_geksan_5->setText( gGeksan_tab4 );
                                                ui->lineEdit_geksan_5->insert(" [мг/л]");
                                                b=b+8;
                                                break;

                                            case 8:
                                                /*  Оксид серы */

                                                qDebug()<<"B-Оксид серы"<<b;
                                                gOksSer_tab4 = FormatText_5(IEEE754_5(b));
                                                ui->lineEdit_oksser_5->setText( gOksSer_tab4 );
                                                ui->lineEdit_oksser_5->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 9:

                                                /*  Оксид азота */

                                                qDebug()<<"B-Оксид азота"<<b;
                                                gOksAzot_tab4 = FormatText_5(IEEE754_5(b));
                                                ui->lineEdit_oksazot_5->setText( gOksAzot_tab4 );
                                                ui->lineEdit_oksazot_5->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 10:

                                                /*  Соляная кислота */

                                                qDebug()<<"B-Соляная кислота"<<b;
                                                gSolKisl_tab4 = FormatText_5(IEEE754_5(b));
                                                ui->lineEdit_solkisl_5->setText( gSolKisl_tab4 );
                                                ui->lineEdit_solkisl_5->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 11:
                                                /*  Пропан */

                                                qDebug()<<"B-Пропан"<<b;
                                                gPropan_tab4 = FormatText_5(IEEE754_5(b));
                                                ui->lineEdit_propan_5->setText( gPropan_tab4 );
                                                ui->lineEdit_propan_5->insert(" [об.%]");
                                                b=b+8;
                                                break;

                                            case 12:
                                                /*  Аммиак */

                                                qDebug()<<"B-Аммиак"<<b;
                                                gAmmiak_tab4 = FormatText_5(IEEE754_5(b));
                                                ui->lineEdit_amiak_5->setText( gAmmiak_tab4 );
                                                ui->lineEdit_amiak_5->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 13:
                                                /* Углекислый газ */

                                                qDebug()<<"B-Углекислый газ"<<b;
                                                gYglGaz_tab4 = FormatText_5(IEEE754_5(b));
                                                ui->lineEdit_yglgaz_5->setText( gYglGaz_tab4 );
                                                ui->lineEdit_yglgaz_5->insert(" [об.%]");
                                                b=b+8;
                                                break;

                                            case 14:
                                                /*  Иприт  (GB) */


                                                qDebug()<<"B-Иприт  (GB)"<<b;


                                                if (FormatText_5(IEEE754_5(b))=="0.0")
                                                {
                                                  ui->lineEdit_gb_5->setText("В норме");
                                                  gGB_tab4 = "0";
                                                }
                                                if (IEEE754_5(b)=="1")
                                                {
                                                  ui->lineEdit_gb_5->setText("1 порог");
                                                  gGB_tab4 = "1";
                                                }
                                                if (IEEE754_4(b)=="100")
                                                {
                                                  ui->lineEdit_gb_5->setText("2 порог");
                                                  gGB_tab4 = "100";
                                                }


                                                /* ui->lineEdit_gb->setText(FormatText(IEEE754(b))); */
                                                /* ui->lineEdit_gb->setText(IEEE754(b)); */

                                                b=b+8;
                                                break;

                                            case 15:
                                                /*  Зарин (HD)  */

                                                qDebug()<<"B-HD"<<b;
                                                if (FormatText_5(IEEE754_5(b))=="0.0")
                                                {
                                                  ui->lineEdit_hd_5->setText("В норме");
                                                  gHD_tab4 = "0";
                                                }
                                                if (FormatText_5(IEEE754_5(b))=="2.000")
                                                {
                                                  ui->lineEdit_hd_5->setText("1 порог");
                                                  gHD_tab4 = "2";
                                                }
                                                if (FormatText_5(IEEE754_5(b))=="200.0")
                                                {
                                                  ui->lineEdit_hd_5->setText("2 порог");
                                                  gHD_tab4 = "200";
                                                }
                                                /* ui->lineEdit_hd->setText(FormatText(IEEE754(b))); */
                                                b=b+8;
                                                break;

                                            case 16:
                                                /*  HCN */

                                                qDebug()<<"B-HCN"<<b;
                                                gHCN_tab4 = FormatText_5(IEEE754_5(b));
                                                ui->lineEdit_hcn_5->setText( gHCN_tab4 );
                                                ui->lineEdit_hcn_5->insert(" [мг/м3]");
                                                b=b+8;
                                                break;

                                            case 17:
                                                /*  H2CO - формальдегид */

                                                qDebug()<<"B-H2CO"<<b;
                                                gH2CO_tab4 = FormatText_5(IEEE754_5(b));
                                                ui->lineEdit_h2co_5->setText( gH2CO_tab4 );
                                                b=b+8;
                                                break;

                                            default:
                                                if (posledov_tab4.at(p-1)==0)
                                                {
                                                    b=b+8;
                                                }
                                                /* b=b+8; */
                                                break;
                                                }


                               } /* _for */
                            }/* _for */

                                    if (ui->cb_automode_5->isChecked())
                                    {
                                       SQLBDHK_5();
                                    }

                        }/* _if */
                     }


                }
    else ui->statusBar->showMessage(tr("Не верный заголовок пакета данных // Tab 4"));




    str_tab4.clear();
    listhex_tab4.clear();
    list_tab4.clear();
    ui->pushButton_getValue_5->setEnabled(true);
    ui->pushButton_getValue_5->setEnabled(true);
    all_lineedit_enable_5(true);
    monitorOff_5();

/*
    if (ui->cb_automode->isChecked())
    {
      on_pb_giveMeteo_clicked();
    }
*/
}



//======================== _Data ======================================================================
//=====================================================================================================




//=====================================================================================================
//======================== Кнопки =====================================================================


void MainWindow::on_pushButton_opport_clicked() //кнопка открыть порт
{
    ui->pushButton_opport->setEnabled(false); //отркыть порт не активна
    ui->statusBar->showMessage("Подключаемся к порту");
    timer->singleShot(50, this, SLOT(ComConnect())); //переходим в ComConnect через 50 мс
}

void MainWindow::on_pushButton_clport_clicked() //кнопка закрыть порт
{
    serial->close();
    status=1;
    portCloseStyleSheet();
    all_lineedit_enable(false);//гасим все LineEdit
    text_all_lineedit("");//чистим все LineEdit
    groupbox(false);//гасим все GroupBox
    ui->statusBar->showMessage(tr("Порт закрыт"));
}

void MainWindow::on_pbtn_type_clicked() //кнопка. получить тип датчиков
{

    if (status==0)
{
   getType(); //отправляем команду
   timer->singleShot(1000, this, SLOT(getlist())); //переходим в getlist через 1 с - парсим данные
   ui->pbtn_type->setEnabled(false);
   ui->pushButton_getValue->setEnabled(true);
  }
   else ui->statusBar->showMessage("Порт закрыт. Типы датчиков не получены");


}

void MainWindow::on_pushButton_getValue_clicked() /* кнопка получить значения */
{
    disconnect(timer2, SIGNAL(timeout()), this, SLOT(TimerTick()));

    listhex.clear();
    list.clear();
    monitorOn();//включаем мониторинг

    ui->pushButton_getValue->setEnabled(false);

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);


    text_all_lineedit("Считываю...");
    all_lineedit_enable(false);

    /*=== Для ProgressBar === */


    timer2->start(600);
    ui->progressBar->setVisible(true);
    ui->progressBar->setValue(2);
    connect(timer2, SIGNAL(timeout()), this, SLOT(TimerTick()));

    /*
    if (timer2->isActive())
    {
    ui->pbtn_type->setEnabled(false);
    ui->pushButton_getValue->setEnabled(false);
    }
    else
    {
        ui->pbtn_type->setEnabled(true);
        ui->pushButton_getValue->setEnabled(true);
    }
    */



    timer->singleShot(1000*60, this, SLOT(Value())); /* переходим в Value через 1 мин */



}


//========================== Tab 2


void MainWindow::on_pushButton_opport_3_clicked() //кнопка открыть порт // Tab 2
{
    ui->pushButton_opport_3->setEnabled(false); //отркыть порт не активна
    ui->statusBar->showMessage("Подключаемся к порту / Tab 2");
    timer->singleShot(50, this, SLOT(ComConnect_3())); //переходим в ComConnect через 50 мс
}

void MainWindow::on_pushButton_clport_3_clicked() //кнопка закрыть порт // Tab 2
{
    serial_tab2->close();
    status_tab2=1;
    portCloseStyleSheet_3();
    all_lineedit_enable_3(false);//гасим все LineEdit
    text_all_lineedit_3("");//чистим все LineEdit
    groupbox_3(false);//гасим все GroupBox
    ui->statusBar->showMessage(tr("Порт закрыт / Tab 2"));
}

void MainWindow::on_pbtn_type_3_clicked() //кнопка. получить тип датчиков // Tab 2
{
    if (status_tab2==0)
{
   getType_3(); //отправляем команду
   timer->singleShot(1000, this, SLOT(getlist_3())); //переходим в getlist через 1 с - парсим данные
   ui->pbtn_type_3->setEnabled(false);
   ui->pushButton_getValue_3->setEnabled(true);
  }
   else ui->statusBar->showMessage("Порт закрыт. Типы датчиков не получены // Tab 2");
}

void MainWindow::on_pushButton_getValue_3_clicked() /* кнопка получить значения // Tab 2*/
{

    disconnect(timer2_tab2, SIGNAL(timeout()), this, SLOT(TimerTick_3()));

    listhex_tab2.clear();
    list_tab2.clear();
    monitorOn_3();//включаем мониторинг

    ui->pushButton_getValue_3->setEnabled(false);

    ui->progressBar_3->setMinimum(0);
    ui->progressBar_3->setMaximum(100);


    text_all_lineedit_3("Считываю...");
    all_lineedit_enable_3(false);

    /*=== Для ProgressBar === */


    timer2_tab2->start(600);
    ui->progressBar_3->setVisible(true);
    ui->progressBar_3->setValue(2);
    connect(timer2_tab2, SIGNAL(timeout()), this, SLOT(TimerTick_3()));

    /*
    if (timer2->isActive())
    {
    ui->pbtn_type->setEnabled(false);
    ui->pushButton_getValue->setEnabled(false);
    }
    else
    {
        ui->pbtn_type->setEnabled(true);
        ui->pushButton_getValue->setEnabled(true);
    }
    */



    timer->singleShot(1000*60, this, SLOT(Value_3())); /* переходим в Value через 1 мин */

}


//========================== Tab 3


void MainWindow::on_pushButton_opport_4_clicked() //кнопка открыть порт // Tab 3
{
    ui->pushButton_opport_4->setEnabled(false); //отркыть порт не активна
    ui->statusBar->showMessage("Подключаемся к порту / Tab 3");
    timer->singleShot(50, this, SLOT(ComConnect_4())); //переходим в ComConnect через 50 мс
}

void MainWindow::on_pushButton_clport_4_clicked() //кнопка закрыть порт // Tab 3
{
    serial_tab3->close();
    status_tab3=1;
    portCloseStyleSheet_4();
    all_lineedit_enable_4(false);//гасим все LineEdit
    text_all_lineedit_4("");//чистим все LineEdit
    groupbox_4(false);//гасим все GroupBox
    ui->statusBar->showMessage(tr("Порт закрыт / Tab 3"));
}

void MainWindow::on_pbtn_type_4_clicked() //кнопка. получить тип датчиков // Tab 3
{
    if (status_tab3==0)
{
   getType_4(); //отправляем команду
   timer->singleShot(1000, this, SLOT(getlist_4())); //переходим в getlist через 1 с - парсим данные
   ui->pbtn_type_4->setEnabled(false);
   ui->pushButton_getValue_4->setEnabled(true);
  }
   else ui->statusBar->showMessage("Порт закрыт. Типы датчиков не получены // Tab 3");
}

void MainWindow::on_pushButton_getValue_4_clicked() /* кнопка получить значения // Tab 3*/
{

    disconnect(timer2_tab3, SIGNAL(timeout()), this, SLOT(TimerTick_4()));

    listhex_tab3.clear();
    list_tab3.clear();
    monitorOn_4();//включаем мониторинг

    ui->pushButton_getValue_4->setEnabled(false);

    ui->progressBar_4->setMinimum(0);
    ui->progressBar_4->setMaximum(100);


    text_all_lineedit_4("Считываю...");
    all_lineedit_enable_4(false);

    /*=== Для ProgressBar === */


    timer2_tab3->start(600);
    ui->progressBar_4->setVisible(true);
    ui->progressBar_4->setValue(2);
    connect(timer2_tab3, SIGNAL(timeout()), this, SLOT(TimerTick_4()));

    /*
    if (timer2->isActive())
    {
    ui->pbtn_type->setEnabled(false);
    ui->pushButton_getValue->setEnabled(false);
    }
    else
    {
        ui->pbtn_type->setEnabled(true);
        ui->pushButton_getValue->setEnabled(true);
    }
    */



    timer->singleShot(1000*60, this, SLOT(Value_4())); /* переходим в Value через 1 мин */

}



//========================== Tab 4


void MainWindow::on_pushButton_opport_5_clicked() //кнопка открыть порт // Tab 4
{
    ui->pushButton_opport_5->setEnabled(false); //отркыть порт не активна
    ui->statusBar->showMessage("Подключаемся к порту / Tab 4");
    timer->singleShot(50, this, SLOT(ComConnect_5())); //переходим в ComConnect через 50 мс
}

void MainWindow::on_pushButton_clport_5_clicked() //кнопка закрыть порт // Tab 4
{
    serial_tab4->close();
    status_tab4=1;
    portCloseStyleSheet_5();
    all_lineedit_enable_5(false);//гасим все LineEdit
    text_all_lineedit_5("");//чистим все LineEdit
    groupbox_5(false);//гасим все GroupBox
    ui->statusBar->showMessage(tr("Порт закрыт / Tab 4"));
}

void MainWindow::on_pbtn_type_5_clicked() //кнопка. получить тип датчиков // Tab 3
{
    if (status_tab4==0)
{
   getType_5(); //отправляем команду
   timer->singleShot(1000, this, SLOT(getlist_5())); //переходим в getlist через 1 с - парсим данные
   ui->pbtn_type_5->setEnabled(false);
   ui->pushButton_getValue_5->setEnabled(true);
  }
   else ui->statusBar->showMessage("Порт закрыт. Типы датчиков не получены // Tab 4");
}

void MainWindow::on_pushButton_getValue_5_clicked() /* кнопка получить значения // Tab 4*/
{

    disconnect(timer2_tab4, SIGNAL(timeout()), this, SLOT(TimerTick_5()));

    listhex_tab4.clear();
    list_tab4.clear();
    monitorOn_5();//включаем мониторинг

    ui->pushButton_getValue_5->setEnabled(false);

    ui->progressBar_5->setMinimum(0);
    ui->progressBar_5->setMaximum(100);


    text_all_lineedit_5("Считываю...");
    all_lineedit_enable_5(false);

    /*=== Для ProgressBar === */


    timer2_tab4->start(600);
    ui->progressBar_5->setVisible(true);
    ui->progressBar_5->setValue(2);
    connect(timer2_tab4, SIGNAL(timeout()), this, SLOT(TimerTick_5()));

    /*
    if (timer2->isActive())
    {
    ui->pbtn_type->setEnabled(false);
    ui->pushButton_getValue->setEnabled(false);
    }
    else
    {
        ui->pbtn_type->setEnabled(true);
        ui->pushButton_getValue->setEnabled(true);
    }
    */



    timer->singleShot(1000*60, this, SLOT(Value_5())); /* переходим в Value через 1 мин */

}



//======================== _Кнопки ====================================================================
//=====================================================================================================






//=====================================================================================================
//========================= Interface =================================================================


void MainWindow::deviceOff() //не получили идент. пакет от устройства
{
    ui->label_time->setVisible(false);//убираем время
    ui->label_date->setVisible(false);//убираем дату

    ui->label_hp10->setVisible(false);//убираем МЭД
    ui->lineEdit_Hp10->setVisible(false);

    ui->label_metan->setVisible(false);//убираем Метан
    ui->lineEdit_metan->setVisible(false);

    ui->label_ftvk->setVisible(false);//убираем Фт-Вод. Кислота
    ui->lineEdit_ftvk->setVisible(false);

    ui->label_monoygl->setVisible(false);//убираем Моноксид углерода
    ui->lineEdit_monoygl->setVisible(false);

    ui->label_servod->setVisible(false);//убираем Сероводород
    ui->lineEdit_servod->setVisible(false);

    ui->label_hlor->setVisible(false);//убираем Хлор
    ui->lineEdit_hlor->setVisible(false);

    ui->label_geksan->setVisible(false);//убираем Гексан
    ui->lineEdit_geksan->setVisible(false);

    ui->label_oksser->setVisible(false);//убираем Оксид серы
    ui->lineEdit_oksser->setVisible(false);

    ui->label_oksazot->setVisible(false);//убираем Оксид азота
    ui->lineEdit_oksazot->setVisible(false);

    ui->label_solkisl->setVisible(false);//убираем Соляная кислота
    ui->lineEdit_solkisl->setVisible(false);

    ui->label_propan->setVisible(false);//убираем Пропан
    ui->lineEdit_propan->setVisible(false);

    ui->label_amiak->setVisible(false);//убираем Аммиак
    ui->lineEdit_amiak->setVisible(false);

    ui->label_yglgaz->setVisible(false);//убираем Углекислый газ
    ui->lineEdit_yglgaz->setVisible(false);

    ui->label_gb->setVisible(false);//убираем GB
    ui->lineEdit_gb->setVisible(false);

    ui->label_hd->setVisible(false);//убираем HD
    ui->lineEdit_hd->setVisible(false);

    ui->label_hcn->setVisible(false);//убираем HCN
    ui->lineEdit_hcn->setVisible(false);

    ui->label_h2co->setVisible(false);//убираем H2CO
    ui->lineEdit_h2co->setVisible(false);


    ui->groupBox_3->setEnabled(false);
    ui->groupBox_4->setEnabled(false);
    ui->groupBox_6->setEnabled(false);

}

void MainWindow::portCloseStyleSheet() /*описание стиля. Порт закрыт */
{
    ui->pushButton_clport->setVisible(false);
    ui->pushButton_opport->setVisible(true);
    ui->pushButton_opport->setEnabled(true);
    ui->groupBox->setStyleSheet("color: rgb(255, 0, 0)"); //красный
    ui->groupBox->setTitle("Порт закрыт");
    ui->comboBox->setEnabled(true);
    ui->pbtn_type->setVisible(false); //кнопка Тип
    ui->pushButton_getValue->setVisible(false);
    ui->progressBar->setVisible(false); //убираем ProgressBar



    ui->lineEdit_amiak->setVisible(false);
    ui->lineEdit_ftvk->setVisible(false);
    ui->lineEdit_gb->setVisible(false);
    ui->lineEdit_geksan->setVisible(false);
    ui->lineEdit_h2co->setVisible(false);
    ui->lineEdit_hcn->setVisible(false);
    ui->lineEdit_hd->setVisible(false);
    ui->lineEdit_hlor->setVisible(false);
    ui->lineEdit_Hp10->setVisible(false);
    ui->lineEdit_metan->setVisible(false);
    ui->lineEdit_monoygl->setVisible(false);
    ui->lineEdit_oksazot->setVisible(false);
    ui->lineEdit_oksser->setVisible(false);
    ui->lineEdit_propan->setVisible(false);
    ui->lineEdit_servod->setVisible(false);
    ui->lineEdit_solkisl->setVisible(false);
    ui->lineEdit_yglgaz->setVisible(false);


    ui->label_solkisl->setVisible(false);
    ui->label_amiak->setVisible(false);
    ui->label_ftvk->setVisible(false);
    ui->label_gb->setVisible(false);
    ui->label_geksan->setVisible(false);
    ui->label_h2co->setVisible(false);
    ui->label_hcn->setVisible(false);
    ui->label_hd->setVisible(false);
    ui->label_hlor->setVisible(false);
    ui->label_hp10->setVisible(false);
    ui->label_metan->setVisible(false);
    ui->label_monoygl->setVisible(false);
    ui->label_oksazot->setVisible(false);
    ui->label_oksser->setVisible(false);
    ui->label_propan->setVisible(false);
    ui->label_servod->setVisible(false);
    ui->label_yglgaz->setVisible(false);

    ui->label_date->setVisible(false);
    ui->label_time->setVisible(false);

    ui->pb_giveMeteo->setVisible(false);
    ui->radioButton->setVisible(false);

    ui->te_meteo->clear();
}

void MainWindow::portOpenStyleSheet() //описание стиля. Порт открыт //для Tab 1
{
    ui->pushButton_clport->setVisible(true);
    ui->pushButton_opport->setVisible(false);
    ui->pushButton_opport->setEnabled(false);
    ui->groupBox->setStyleSheet("color: rgb(255, 228, 0)"); //светло-зелёный
    ui->groupBox->setTitle("Порт открыт");
    ui->comboBox->setEnabled(false);
    ui->pbtn_type->setVisible(true);
    ui->pbtn_type->setEnabled(true);

    ui->pb_giveMeteo->setVisible(true);
    ui->radioButton->setVisible(true);
}

void MainWindow::text_all_lineedit(QString text) //текст на всех LineEdit датчиков
{
    ui->lineEdit_amiak->setText(text);
    ui->lineEdit_ftvk->setText(text);
    ui->lineEdit_gb->setText(text);
    ui->lineEdit_geksan->setText(text);
    ui->lineEdit_h2co->setText(text);
    ui->lineEdit_hcn->setText(text);
    ui->lineEdit_hd->setText(text);
    ui->lineEdit_hlor->setText(text);
    ui->lineEdit_Hp10->setText(text);
    ui->lineEdit_metan->setText(text);
    ui->lineEdit_monoygl->setText(text);
    ui->lineEdit_oksazot->setText(text);
    ui->lineEdit_oksser->setText(text);
    ui->lineEdit_propan->setText(text);
    ui->lineEdit_servod->setText(text);
    ui->lineEdit_solkisl->setText(text);
    ui->lineEdit_yglgaz->setText(text);
}

void MainWindow::all_lineedit_enable(bool en) //состояние всех LineEdit датчиков
{
    ui->lineEdit_amiak->setEnabled(en);
    ui->lineEdit_ftvk->setEnabled(en);
    ui->lineEdit_gb->setEnabled(en);
    ui->lineEdit_geksan->setEnabled(en);
    ui->lineEdit_h2co->setEnabled(en);
    ui->lineEdit_hcn->setEnabled(en);
    ui->lineEdit_hd->setEnabled(en);
    ui->lineEdit_hlor->setEnabled(en);
    ui->lineEdit_Hp10->setEnabled(en);
    ui->lineEdit_metan->setEnabled(en);
    ui->lineEdit_monoygl->setEnabled(en);
    ui->lineEdit_oksazot->setEnabled(en);
    ui->lineEdit_oksser->setEnabled(en);
    ui->lineEdit_propan->setEnabled(en);
    ui->lineEdit_servod->setEnabled(en);
    ui->lineEdit_solkisl->setEnabled(en);
    ui->lineEdit_yglgaz->setEnabled(en);
}

void MainWindow::groupbox(bool stat) //состояние всех GroupBox для датчиков
{

    ui->groupBox_3->setEnabled(stat);
    ui->groupBox_4->setEnabled(stat);
    ui->groupBox_6->setEnabled(stat);

}

void MainWindow::TimerTick() //тащим ProgressBar
{


    if( (ui->progressBar->value()) >= (ui->progressBar->maximum()))
    {
        ui->progressBar->setVisible(false);
        ui->pushButton_getValue->setEnabled(true);
        //timer2->stop();

    }
    else
    {

       ui->statusBar->showMessage("Идёт считывание");
       ui->progressBar->setValue((ui->progressBar->value())+1);
    }

}



//========================== Tab 2

void MainWindow::deviceOff_3() //не получили идент. пакет от устройства //для Tab 2
{
    ui->label_time_3->setVisible(false);//убираем время
    ui->label_date_3->setVisible(false);//убираем дату

    ui->label_hp10_3->setVisible(false);//убираем МЭД
    ui->lineEdit_Hp10_3->setVisible(false);

    ui->label_metan_3->setVisible(false);//убираем Метан
    ui->lineEdit_metan_3->setVisible(false);

    ui->label_ftvk_3->setVisible(false);//убираем Фт-Вод. Кислота
    ui->lineEdit_ftvk_3->setVisible(false);

    ui->label_monoygl_3->setVisible(false);//убираем Моноксид углерода
    ui->lineEdit_monoygl_3->setVisible(false);

    ui->label_servod_3->setVisible(false);//убираем Сероводород
    ui->lineEdit_servod_3->setVisible(false);

    ui->label_hlor_3->setVisible(false);//убираем Хлор
    ui->lineEdit_hlor_3->setVisible(false);

    ui->label_geksan_3->setVisible(false);//убираем Гексан
    ui->lineEdit_geksan_3->setVisible(false);

    ui->label_oksser_3->setVisible(false);//убираем Оксид серы
    ui->lineEdit_oksser_3->setVisible(false);

    ui->label_oksazot_3->setVisible(false);//убираем Оксид азота
    ui->lineEdit_oksazot_3->setVisible(false);

    ui->label_solkisl_3->setVisible(false);//убираем Соляная кислота
    ui->lineEdit_solkisl_3->setVisible(false);

    ui->label_propan_3->setVisible(false);//убираем Пропан
    ui->lineEdit_propan_3->setVisible(false);

    ui->label_amiak_3->setVisible(false);//убираем Аммиак
    ui->lineEdit_amiak_3->setVisible(false);

    ui->label_yglgaz_3->setVisible(false);//убираем Углекислый газ
    ui->lineEdit_yglgaz_3->setVisible(false);

    ui->label_gb_3->setVisible(false);//убираем GB
    ui->lineEdit_gb_3->setVisible(false);

    ui->label_hd_3->setVisible(false);//убираем HD
    ui->lineEdit_hd_3->setVisible(false);

    ui->label_hcn_3->setVisible(false);//убираем HCN
    ui->lineEdit_hcn_3->setVisible(false);

    ui->label_h2co_3->setVisible(false);//убираем H2CO
    ui->lineEdit_h2co_3->setVisible(false);
}

void MainWindow::portCloseStyleSheet_3() //описание стиля. Порт закрыт // для Tab 2
{
    ui->pushButton_clport_3->setVisible(false);
    ui->pushButton_opport_3->setVisible(true);
    ui->pushButton_opport_3->setEnabled(true);
    ui->groupBox_17->setStyleSheet("color: rgb(255, 0, 0)"); //красный
    ui->groupBox_17->setTitle("Порт закрыт");
    ui->comboBox_3->setEnabled(true);
    ui->pbtn_type_3->setVisible(false); //кнопка Тип
    ui->pushButton_getValue_3->setVisible(false);
    ui->progressBar_3->setVisible(false); //убираем ProgressBar



    ui->lineEdit_amiak_3->setVisible(false);
    ui->lineEdit_ftvk_3->setVisible(false);
    ui->lineEdit_gb_3->setVisible(false);
    ui->lineEdit_geksan_3->setVisible(false);
    ui->lineEdit_h2co_3->setVisible(false);
    ui->lineEdit_hcn_3->setVisible(false);
    ui->lineEdit_hd_3->setVisible(false);
    ui->lineEdit_hlor_3->setVisible(false);
    ui->lineEdit_Hp10_3->setVisible(false);
    ui->lineEdit_metan_3->setVisible(false);
    ui->lineEdit_monoygl_3->setVisible(false);
    ui->lineEdit_oksazot_3->setVisible(false);
    ui->lineEdit_oksser_3->setVisible(false);
    ui->lineEdit_propan_3->setVisible(false);
    ui->lineEdit_servod_3->setVisible(false);
    ui->lineEdit_solkisl_3->setVisible(false);
    ui->lineEdit_yglgaz_3->setVisible(false);


    ui->label_solkisl_3->setVisible(false);
    ui->label_amiak_3->setVisible(false);
    ui->label_ftvk_3->setVisible(false);
    ui->label_gb_3->setVisible(false);
    ui->label_geksan_3->setVisible(false);
    ui->label_h2co_3->setVisible(false);
    ui->label_hcn_3->setVisible(false);
    ui->label_hd_3->setVisible(false);
    ui->label_hlor_3->setVisible(false);
    ui->label_hp10_3->setVisible(false);
    ui->label_metan_3->setVisible(false);
    ui->label_monoygl_3->setVisible(false);
    ui->label_oksazot_3->setVisible(false);
    ui->label_oksser_3->setVisible(false);
    ui->label_propan_3->setVisible(false);
    ui->label_servod_3->setVisible(false);
    ui->label_yglgaz_3->setVisible(false);

    ui->label_date_3->setVisible(false);
    ui->label_time_3->setVisible(false);

    ui->pb_giveMeteo_3->setVisible(false);
    ui->radioButton_3->setVisible(false);

    ui->te_meteo_3->clear();
}

void MainWindow::portOpenStyleSheet_3() //описание стиля. Порт открыт //для Tab 2
{
    ui->pushButton_clport_3->setVisible(true);
    ui->pushButton_opport_3->setVisible(false);
    ui->pushButton_opport_3->setEnabled(false);
    ui->groupBox_17->setStyleSheet("color: rgb(255, 228, 0)"); //светло-зелёный
    ui->groupBox_17->setTitle("Порт открыт");
    ui->comboBox_3->setEnabled(false);
    ui->pbtn_type_3->setVisible(true);
    ui->pbtn_type_3->setEnabled(true);

    ui->pb_giveMeteo_3->setVisible(true);
    ui->radioButton_3->setVisible(true);
}

void MainWindow::text_all_lineedit_3(QString text) //текст на всех LineEdit датчиков // Tab 2
{
    ui->lineEdit_amiak_3->setText(text);
    ui->lineEdit_ftvk_3->setText(text);
    ui->lineEdit_gb_3->setText(text);
    ui->lineEdit_geksan_3->setText(text);
    ui->lineEdit_h2co_3->setText(text);
    ui->lineEdit_hcn_3->setText(text);
    ui->lineEdit_hd_3->setText(text);
    ui->lineEdit_hlor_3->setText(text);
    ui->lineEdit_Hp10_3->setText(text);
    ui->lineEdit_metan_3->setText(text);
    ui->lineEdit_monoygl_3->setText(text);
    ui->lineEdit_oksazot_3->setText(text);
    ui->lineEdit_oksser_3->setText(text);
    ui->lineEdit_propan_3->setText(text);
    ui->lineEdit_servod_3->setText(text);
    ui->lineEdit_solkisl_3->setText(text);
    ui->lineEdit_yglgaz_3->setText(text);
}

void MainWindow::all_lineedit_enable_3(bool en) //состояние всех LineEdit датчиков //для Tab 2
{
    ui->lineEdit_amiak_3->setEnabled(en);
    ui->lineEdit_ftvk_3->setEnabled(en);
    ui->lineEdit_gb_3->setEnabled(en);
    ui->lineEdit_geksan_3->setEnabled(en);
    ui->lineEdit_h2co_3->setEnabled(en);
    ui->lineEdit_hcn_3->setEnabled(en);
    ui->lineEdit_hd_3->setEnabled(en);
    ui->lineEdit_hlor_3->setEnabled(en);
    ui->lineEdit_Hp10_3->setEnabled(en);
    ui->lineEdit_metan_3->setEnabled(en);
    ui->lineEdit_monoygl_3->setEnabled(en);
    ui->lineEdit_oksazot_3->setEnabled(en);
    ui->lineEdit_oksser_3->setEnabled(en);
    ui->lineEdit_propan_3->setEnabled(en);
    ui->lineEdit_servod_3->setEnabled(en);
    ui->lineEdit_solkisl_3->setEnabled(en);
    ui->lineEdit_yglgaz_3->setEnabled(en);
}

void MainWindow::groupbox_3(bool stat) //состояние всех GroupBox для датчиков //tab 2
{
    ui->groupBox_19->setEnabled(stat);
    ui->groupBox_20->setEnabled(stat);
    ui->groupBox_21->setEnabled(stat);
}

void MainWindow::TimerTick_3() //тащим ProgressBar // Tab 2
{


    if( (ui->progressBar_3->value()) >= (ui->progressBar_3->maximum()))
    {
        ui->progressBar_3->setVisible(false);
        ui->pushButton_getValue_3->setEnabled(true);
        //timer2->stop();

    }
    else
    {

       ui->statusBar->showMessage("Идёт считывание // Tab 2");
       ui->progressBar_3->setValue((ui->progressBar_3->value())+1);
    }

}



//========================== Tab 3

void MainWindow::deviceOff_4() //не получили идент. пакет от устройства //для Tab 3
{
    ui->label_time_4->setVisible(false);//убираем время
    ui->label_date_4->setVisible(false);//убираем дату

    ui->label_hp10_4->setVisible(false);//убираем МЭД
    ui->lineEdit_Hp10_4->setVisible(false);

    ui->label_metan_4->setVisible(false);//убираем Метан
    ui->lineEdit_metan_4->setVisible(false);

    ui->label_ftvk_4->setVisible(false);//убираем Фт-Вод. Кислота
    ui->lineEdit_ftvk_4->setVisible(false);

    ui->label_monoygl_4->setVisible(false);//убираем Моноксид углерода
    ui->lineEdit_monoygl_4->setVisible(false);

    ui->label_servod_4->setVisible(false);//убираем Сероводород
    ui->lineEdit_servod_4->setVisible(false);

    ui->label_hlor_4->setVisible(false);//убираем Хлор
    ui->lineEdit_hlor_4->setVisible(false);

    ui->label_geksan_4->setVisible(false);//убираем Гексан
    ui->lineEdit_geksan_4->setVisible(false);

    ui->label_oksser_4->setVisible(false);//убираем Оксид серы
    ui->lineEdit_oksser_4->setVisible(false);

    ui->label_oksazot_4->setVisible(false);//убираем Оксид азота
    ui->lineEdit_oksazot_4->setVisible(false);

    ui->label_solkisl_4->setVisible(false);//убираем Соляная кислота
    ui->lineEdit_solkisl_4->setVisible(false);

    ui->label_propan_4->setVisible(false);//убираем Пропан
    ui->lineEdit_propan_4->setVisible(false);

    ui->label_amiak_4->setVisible(false);//убираем Аммиак
    ui->lineEdit_amiak_4->setVisible(false);

    ui->label_yglgaz_4->setVisible(false);//убираем Углекислый газ
    ui->lineEdit_yglgaz_4->setVisible(false);

    ui->label_gb_4->setVisible(false);//убираем GB
    ui->lineEdit_gb_4->setVisible(false);

    ui->label_hd_4->setVisible(false);//убираем HD
    ui->lineEdit_hd_4->setVisible(false);

    ui->label_hcn_4->setVisible(false);//убираем HCN
    ui->lineEdit_hcn_4->setVisible(false);

    ui->label_h2co_4->setVisible(false);//убираем H2CO
    ui->lineEdit_h2co_4->setVisible(false);
}

void MainWindow::portCloseStyleSheet_4() //описание стиля. Порт закрыт // для Tab 3
{
    ui->pushButton_clport_4->setVisible(false);
    ui->pushButton_opport_4->setVisible(true);
    ui->pushButton_opport_4->setEnabled(true);
    ui->groupBox_25->setStyleSheet("color: rgb(255, 0, 0)"); //красный
    ui->groupBox_25->setTitle("Порт закрыт");
    ui->comboBox_4->setEnabled(true);
    ui->pbtn_type_4->setVisible(false); //кнопка Тип
    ui->pushButton_getValue_4->setVisible(false);
    ui->progressBar_4->setVisible(false); //убираем ProgressBar



    ui->lineEdit_amiak_4->setVisible(false);
    ui->lineEdit_ftvk_4->setVisible(false);
    ui->lineEdit_gb_4->setVisible(false);
    ui->lineEdit_geksan_4->setVisible(false);
    ui->lineEdit_h2co_4->setVisible(false);
    ui->lineEdit_hcn_4->setVisible(false);
    ui->lineEdit_hd_4->setVisible(false);
    ui->lineEdit_hlor_4->setVisible(false);
    ui->lineEdit_Hp10_4->setVisible(false);
    ui->lineEdit_metan_4->setVisible(false);
    ui->lineEdit_monoygl_4->setVisible(false);
    ui->lineEdit_oksazot_4->setVisible(false);
    ui->lineEdit_oksser_4->setVisible(false);
    ui->lineEdit_propan_4->setVisible(false);
    ui->lineEdit_servod_4->setVisible(false);
    ui->lineEdit_solkisl_4->setVisible(false);
    ui->lineEdit_yglgaz_4->setVisible(false);


    ui->label_solkisl_4->setVisible(false);
    ui->label_amiak_4->setVisible(false);
    ui->label_ftvk_4->setVisible(false);
    ui->label_gb_4->setVisible(false);
    ui->label_geksan_4->setVisible(false);
    ui->label_h2co_4->setVisible(false);
    ui->label_hcn_4->setVisible(false);
    ui->label_hd_4->setVisible(false);
    ui->label_hlor_4->setVisible(false);
    ui->label_hp10_4->setVisible(false);
    ui->label_metan_4->setVisible(false);
    ui->label_monoygl_4->setVisible(false);
    ui->label_oksazot_4->setVisible(false);
    ui->label_oksser_4->setVisible(false);
    ui->label_propan_4->setVisible(false);
    ui->label_servod_4->setVisible(false);
    ui->label_yglgaz_4->setVisible(false);

    ui->label_date_4->setVisible(false);
    ui->label_time_4->setVisible(false);

    ui->pb_giveMeteo_4->setVisible(false);
    ui->radioButton_4->setVisible(false);

    ui->te_meteo_4->clear();
}

void MainWindow::portOpenStyleSheet_4() //описание стиля. Порт открыт //для Tab 3
{
    ui->pushButton_clport_4->setVisible(true);
    ui->pushButton_opport_4->setVisible(false);
    ui->pushButton_opport_4->setEnabled(false);
    ui->groupBox_25->setStyleSheet("color: rgb(255, 228, 0)"); //светло-зелёный
    ui->groupBox_25->setTitle("Порт открыт");
    ui->comboBox_4->setEnabled(false);
    ui->pbtn_type_4->setVisible(true);
    ui->pbtn_type_4->setEnabled(true);

    ui->pb_giveMeteo_4->setVisible(true);
    ui->radioButton_4->setVisible(true);
}

void MainWindow::text_all_lineedit_4(QString text) //текст на всех LineEdit датчиков // Tab 3
{
    ui->lineEdit_amiak_4->setText(text);
    ui->lineEdit_ftvk_4->setText(text);
    ui->lineEdit_gb_4->setText(text);
    ui->lineEdit_geksan_4->setText(text);
    ui->lineEdit_h2co_4->setText(text);
    ui->lineEdit_hcn_4->setText(text);
    ui->lineEdit_hd_4->setText(text);
    ui->lineEdit_hlor_4->setText(text);
    ui->lineEdit_Hp10_4->setText(text);
    ui->lineEdit_metan_4->setText(text);
    ui->lineEdit_monoygl_4->setText(text);
    ui->lineEdit_oksazot_4->setText(text);
    ui->lineEdit_oksser_4->setText(text);
    ui->lineEdit_propan_4->setText(text);
    ui->lineEdit_servod_4->setText(text);
    ui->lineEdit_solkisl_4->setText(text);
    ui->lineEdit_yglgaz_4->setText(text);
}

void MainWindow::all_lineedit_enable_4(bool en) //состояние всех LineEdit датчиков //для Tab 3
{
    ui->lineEdit_amiak_4->setEnabled(en);
    ui->lineEdit_ftvk_4->setEnabled(en);
    ui->lineEdit_gb_4->setEnabled(en);
    ui->lineEdit_geksan_4->setEnabled(en);
    ui->lineEdit_h2co_4->setEnabled(en);
    ui->lineEdit_hcn_4->setEnabled(en);
    ui->lineEdit_hd_4->setEnabled(en);
    ui->lineEdit_hlor_4->setEnabled(en);
    ui->lineEdit_Hp10_4->setEnabled(en);
    ui->lineEdit_metan_4->setEnabled(en);
    ui->lineEdit_monoygl_4->setEnabled(en);
    ui->lineEdit_oksazot_4->setEnabled(en);
    ui->lineEdit_oksser_4->setEnabled(en);
    ui->lineEdit_propan_4->setEnabled(en);
    ui->lineEdit_servod_4->setEnabled(en);
    ui->lineEdit_solkisl_4->setEnabled(en);
    ui->lineEdit_yglgaz_4->setEnabled(en);
}

void MainWindow::groupbox_4(bool stat) //состояние всех GroupBox для датчиков //tab 3
{
    ui->groupBox_27->setEnabled(stat);
    ui->groupBox_28->setEnabled(stat);
    ui->groupBox_29->setEnabled(stat);
}

void MainWindow::TimerTick_4() //тащим ProgressBar // Tab 3
{


    if( (ui->progressBar_4->value()) >= (ui->progressBar_4->maximum()))
    {
        ui->progressBar_4->setVisible(false);
        ui->pushButton_getValue_4->setEnabled(true);
        //timer2->stop();

    }
    else
    {

       ui->statusBar->showMessage("Идёт считывание // Tab 3");
       ui->progressBar_4->setValue((ui->progressBar_4->value())+1);
    }

}



//========================== Tab 4

void MainWindow::deviceOff_5() //не получили идент. пакет от устройства //для Tab 4
{
    ui->label_time_5->setVisible(false);//убираем время
    ui->label_date_5->setVisible(false);//убираем дату

    ui->label_hp10_5->setVisible(false);//убираем МЭД
    ui->lineEdit_Hp10_5->setVisible(false);

    ui->label_metan_5->setVisible(false);//убираем Метан
    ui->lineEdit_metan_5->setVisible(false);

    ui->label_ftvk_5->setVisible(false);//убираем Фт-Вод. Кислота
    ui->lineEdit_ftvk_5->setVisible(false);

    ui->label_monoygl_5->setVisible(false);//убираем Моноксид углерода
    ui->lineEdit_monoygl_5->setVisible(false);

    ui->label_servod_5->setVisible(false);//убираем Сероводород
    ui->lineEdit_servod_5->setVisible(false);

    ui->label_hlor_5->setVisible(false);//убираем Хлор
    ui->lineEdit_hlor_5->setVisible(false);

    ui->label_geksan_5->setVisible(false);//убираем Гексан
    ui->lineEdit_geksan_5->setVisible(false);

    ui->label_oksser_5->setVisible(false);//убираем Оксид серы
    ui->lineEdit_oksser_5->setVisible(false);

    ui->label_oksazot_5->setVisible(false);//убираем Оксид азота
    ui->lineEdit_oksazot_5->setVisible(false);

    ui->label_solkisl_5->setVisible(false);//убираем Соляная кислота
    ui->lineEdit_solkisl_5->setVisible(false);

    ui->label_propan_5->setVisible(false);//убираем Пропан
    ui->lineEdit_propan_5->setVisible(false);

    ui->label_amiak_5->setVisible(false);//убираем Аммиак
    ui->lineEdit_amiak_5->setVisible(false);

    ui->label_yglgaz_5->setVisible(false);//убираем Углекислый газ
    ui->lineEdit_yglgaz_5->setVisible(false);

    ui->label_gb_5->setVisible(false);//убираем GB
    ui->lineEdit_gb_5->setVisible(false);

    ui->label_hd_5->setVisible(false);//убираем HD
    ui->lineEdit_hd_5->setVisible(false);

    ui->label_hcn_5->setVisible(false);//убираем HCN
    ui->lineEdit_hcn_5->setVisible(false);

    ui->label_h2co_5->setVisible(false);//убираем H2CO
    ui->lineEdit_h2co_5->setVisible(false);
}

void MainWindow::portCloseStyleSheet_5() //описание стиля. Порт закрыт // для Tab 4
{
    ui->pushButton_clport_5->setVisible(false);
    ui->pushButton_opport_5->setVisible(true);
    ui->pushButton_opport_5->setEnabled(true);
    ui->groupBox_33->setStyleSheet("color: rgb(255, 0, 0)"); //красный
    ui->groupBox_33->setTitle("Порт закрыт");
    ui->comboBox_4->setEnabled(true);
    ui->pbtn_type_5->setVisible(false); //кнопка Тип
    ui->pushButton_getValue_5->setVisible(false);
    ui->progressBar_5->setVisible(false); //убираем ProgressBar



    ui->lineEdit_amiak_5->setVisible(false);
    ui->lineEdit_ftvk_5->setVisible(false);
    ui->lineEdit_gb_5->setVisible(false);
    ui->lineEdit_geksan_5->setVisible(false);
    ui->lineEdit_h2co_5->setVisible(false);
    ui->lineEdit_hcn_5->setVisible(false);
    ui->lineEdit_hd_5->setVisible(false);
    ui->lineEdit_hlor_5->setVisible(false);
    ui->lineEdit_Hp10_5->setVisible(false);
    ui->lineEdit_metan_5->setVisible(false);
    ui->lineEdit_monoygl_5->setVisible(false);
    ui->lineEdit_oksazot_5->setVisible(false);
    ui->lineEdit_oksser_5->setVisible(false);
    ui->lineEdit_propan_5->setVisible(false);
    ui->lineEdit_servod_5->setVisible(false);
    ui->lineEdit_solkisl_5->setVisible(false);
    ui->lineEdit_yglgaz_5->setVisible(false);


    ui->label_solkisl_5->setVisible(false);
    ui->label_amiak_5->setVisible(false);
    ui->label_ftvk_5->setVisible(false);
    ui->label_gb_5->setVisible(false);
    ui->label_geksan_5->setVisible(false);
    ui->label_h2co_5->setVisible(false);
    ui->label_hcn_5->setVisible(false);
    ui->label_hd_5->setVisible(false);
    ui->label_hlor_5->setVisible(false);
    ui->label_hp10_5->setVisible(false);
    ui->label_metan_5->setVisible(false);
    ui->label_monoygl_5->setVisible(false);
    ui->label_oksazot_5->setVisible(false);
    ui->label_oksser_5->setVisible(false);
    ui->label_propan_5->setVisible(false);
    ui->label_servod_5->setVisible(false);
    ui->label_yglgaz_5->setVisible(false);

    ui->label_date_5->setVisible(false);
    ui->label_time_5->setVisible(false);

    ui->pb_giveMeteo_5->setVisible(false);
    ui->radioButton_5->setVisible(false);

    ui->te_meteo_5->clear();
}

void MainWindow::portOpenStyleSheet_5() //описание стиля. Порт открыт //для Tab 4
{
    ui->pushButton_clport_5->setVisible(true);
    ui->pushButton_opport_5->setVisible(false);
    ui->pushButton_opport_5->setEnabled(false);
    ui->groupBox_33->setStyleSheet("color: rgb(255, 228, 0)"); //светло-зелёный
    ui->groupBox_33->setTitle("Порт открыт");
    ui->comboBox_5->setEnabled(false);
    ui->pbtn_type_5->setVisible(true);
    ui->pbtn_type_5->setEnabled(true);

    ui->pb_giveMeteo_5->setVisible(true);
    ui->radioButton_5->setVisible(true);
}

void MainWindow::text_all_lineedit_5(QString text) //текст на всех LineEdit датчиков // Tab 4
{
    ui->lineEdit_amiak_5->setText(text);
    ui->lineEdit_ftvk_5->setText(text);
    ui->lineEdit_gb_5->setText(text);
    ui->lineEdit_geksan_5->setText(text);
    ui->lineEdit_h2co_5->setText(text);
    ui->lineEdit_hcn_5->setText(text);
    ui->lineEdit_hd_5->setText(text);
    ui->lineEdit_hlor_5->setText(text);
    ui->lineEdit_Hp10_5->setText(text);
    ui->lineEdit_metan_5->setText(text);
    ui->lineEdit_monoygl_5->setText(text);
    ui->lineEdit_oksazot_5->setText(text);
    ui->lineEdit_oksser_5->setText(text);
    ui->lineEdit_propan_5->setText(text);
    ui->lineEdit_servod_5->setText(text);
    ui->lineEdit_solkisl_5->setText(text);
    ui->lineEdit_yglgaz_5->setText(text);
}

void MainWindow::all_lineedit_enable_5(bool en) //состояние всех LineEdit датчиков //для Tab 4
{
    ui->lineEdit_amiak_5->setEnabled(en);
    ui->lineEdit_ftvk_5->setEnabled(en);
    ui->lineEdit_gb_5->setEnabled(en);
    ui->lineEdit_geksan_5->setEnabled(en);
    ui->lineEdit_h2co_5->setEnabled(en);
    ui->lineEdit_hcn_5->setEnabled(en);
    ui->lineEdit_hd_5->setEnabled(en);
    ui->lineEdit_hlor_5->setEnabled(en);
    ui->lineEdit_Hp10_5->setEnabled(en);
    ui->lineEdit_metan_5->setEnabled(en);
    ui->lineEdit_monoygl_5->setEnabled(en);
    ui->lineEdit_oksazot_5->setEnabled(en);
    ui->lineEdit_oksser_5->setEnabled(en);
    ui->lineEdit_propan_5->setEnabled(en);
    ui->lineEdit_servod_5->setEnabled(en);
    ui->lineEdit_solkisl_5->setEnabled(en);
    ui->lineEdit_yglgaz_5->setEnabled(en);
}

void MainWindow::groupbox_5(bool stat) //состояние всех GroupBox для датчиков // Tab 4
{
    ui->groupBox_35->setEnabled(stat);
    ui->groupBox_36->setEnabled(stat);
    ui->groupBox_37->setEnabled(stat);
}

void MainWindow::TimerTick_5() //тащим ProgressBar // Tab 4
{


    if( (ui->progressBar_5->value()) >= (ui->progressBar_5->maximum()))
    {
        ui->progressBar_5->setVisible(false);
        ui->pushButton_getValue_5->setEnabled(true);
        //timer2->stop();

    }
    else
    {

       ui->statusBar->showMessage("Идёт считывание // Tab 4");
       ui->progressBar_5->setValue((ui->progressBar_5->value())+1);
    }

}



//========================= _Interface =======================================================================
//============================================================================================================




//============================================================================================================
//=================================== Utils ==================================================================





bool MainWindow::comparestr(QString a, int c)
{
//a - слово для сравнения (заголовки датчиков)
//c - номер ячейки в str с которой начать сравнивать

bool result=true;

    for(int i=0;i<a.length();i++)
 {

        if (str[c+i]==a.at(i))
            result=result && true;
        else
            result=result && false;
 }

    return result;

}

QString MainWindow::FormatText(QString IEEE754) //полученный результат приводим к 4-х значному виду: значение*(10^-6)
{

    //В IEEE754 приходит число. Пр.: 1.21487e-07

    QRegExp e("[e]");

    if (IEEE754.mid((IEEE754.length()-4),1)=="e") //начинаем искать 4-ый символ с конца строки (e) чтобы узнать, что число в степени
    {

        int stepen=IEEE754.right(3).toInt(); //последние три знака в строке в int: -07


        float fl;

        if (e.indexIn(IEEE754)<5)
        {
           IEEE754.insert(4,"0");
        }

        fl=IEEE754.left(5).toFloat(); //первые 5 символов слева: 1.214

        if (stepen>-9 & stepen<-4) // если число не больше 10^-4 и не меньше 10^-9
        {
            QString strl=QString::number(fl/(qPow(10,(-6-(stepen))))); // 1.214/10^(-6-(-07)) т.е. получаем 0.1214
            strl.resize(5);//возвращаем значение. Снова ограничиваем 5 первыми знаками: 0.121
            return strl;
        }
        else
        {
            if (stepen>-4) //если больше 10^-4 т.е. очень большое число
            {
                return ">999";
            }

            if (stepen<-9) //если больше 10^-9 т.е. очень маленькое
            {
                return "0.0";
            }
        }
    }
    else //если число без степени
    {
        IEEE754.resize(5);
        return IEEE754;
    }
}

QString MainWindow::IEEE754(int b) //входной HEX (32bit) переводим в BIN и высчитываем значение по формату IEEE754
{

    //b - конечный байт

    qDebug()<<"STR AT b-8"<<str.at(b-7);
    qDebug()<<"STR AT b-8"<<str.at(b-6);
    qDebug()<<"STR AT b-8"<<str.at(b-5);
    qDebug()<<"STR AT b-8"<<str.at(b-4);
    qDebug()<<"STR AT b-8"<<str.at(b-3);
    qDebug()<<"STR AT b-8"<<str.at(b-2);
    qDebug()<<"STR AT b-8"<<str.at(b-1);
    qDebug()<<"STR AT"<<str.at(b);



    QString qs, qsE, qsM;
    int E,M, znak;
    float F;

    qs.clear();
    qsE.clear();
    qsM.clear();
    E=0;
    M=0;
    F=0;

    qDebug()<<"STRIEEEEE"<<str;
    qDebug()<<"STRIEEEEELEN"<<str.length();

    while (str.length()<180)
    {
        str.insert(str.length(),"0");
    }


    for (int i=0; i<4; ++i) //c b по b-8 перевели в BIN
    {
    qs.append(hextobin(str.at(b-1-(i*2)))); // Пр: 50,48,46...шаг=2
    qs.append(hextobin(str.at(b-(i*2)))); // Пр: 51,49,47...шаг=2
    }

    qDebug()<<"QSSSSTRIEEEEE"<<qs;

    /*=== Вычисляем S ===*/
    if (realint(qs.at(31-31))==0) //Если 31 байт 0, тогда знак "+". Иначе "-" (Вычисляем S из FLOAT)

         znak=1;

    else znak=-1;

    /*=== Вычисляем E ===*/
    for (int i=0; i<8; ++i) //c 30 по 23 байт - вычисляем E из FLOAT
   {
    qsE.append(qs.at((31-30)+i)); //31-23 = 8 т.е. меняем направление счёта. Двигаемся справа налево <-|
       qDebug()<<"qsE"<<qsE;
    }

    /*=== Вычисляем M ===*/
    for (int i=0; i<=22; ++i) //c 22 по 0 байт - вычисляем M из FLOAT
   {
    qsM.append(qs.at((32-23)+i)); //Двигаемся справа налево <-|
           qDebug()<<"qsM"<<qsM;
   }

    E=bintodec(qsE);
    M=bintodec(qsM);
               qDebug()<<"E"<<E;
                          qDebug()<<"M"<<M;
    F=znak*(qPow(2,E-127))*(1+(M/(qPow(2,23))));

    qDebug()<<"FFFF"<<F;
    return QString::number(F).toLatin1();
}

int MainWindow::realint(QString string) //переводим из QString "0" и "1" в Int (почему то .toint не поперло)
{
    if (string=="0") return 0;
    else return 1;
}

int MainWindow::bintodec(QString bin) //переводим из BIN в DEC
{

int result=0;

    for (int i=0; i < bin.length(); i++)
    {

            result=result+ (realint(bin.at(i))*qPow(2,((bin.length()-1)-i))); //значение бита * 2 в степени длина - 1
    }

return result;
}

QString MainWindow::hextobin(QString hex) //конвертация из HEX в BIN
{

QString hexStr = "0123456789abcdef";
QStringList List;

List << "0000" << "0001" << "0010" << "0011" << "0100" << "0101" << "0110" << "0111" << "1000" << "1001" << "1010" << "1011" << "1100" << "1101" << "1110"<< "1111";



    for (int i=0; i <= 15; ++i)
    {
        if (hex == hexStr.at(i))
        {
            hex.clear();
        return List.at(i);
        }
    }
}



//========================== Tab 2

bool MainWindow::comparestr_3(QString a, int c) // для Tab 2
{
//a - слово для сравнения (заголовки датчиков)
//c - номер ячейки в str с которой начать сравнивать

bool result=true;

    for(int i=0;i<a.length();i++)
 {

        if (str_tab2[c+i]==a.at(i))
            result=result && true;
        else
            result=result && false;
 }

    return result;

}

QString MainWindow::FormatText_3(QString IEEE754) //полученный результат приводим к 4-х значному виду: значение*(10^-6) // Tab 2
{

    //В IEEE754 приходит число. Пр.: 1.21487e-07

    QRegExp e("[e]");

    if (IEEE754.mid((IEEE754.length()-4),1)=="e") //начинаем искать 4-ый символ с конца строки (e) чтобы узнать, что число в степени
    {

        int stepen=IEEE754.right(3).toInt(); //последние три знака в строке в int: -07


        float fl;

        if (e.indexIn(IEEE754)<5)
        {
           IEEE754.insert(4,"0");
        }

        fl=IEEE754.left(5).toFloat(); //первые 5 символов слева: 1.214

        if (stepen>-9 & stepen<-4) // если число не больше 10^-4 и не меньше 10^-9
        {
            QString strl=QString::number(fl/(qPow(10,(-6-(stepen))))); // 1.214/10^(-6-(-07)) т.е. получаем 0.1214
            strl.resize(5);//возвращаем значение. Снова ограничиваем 5 первыми знаками: 0.121
            return strl;
        }
        else
        {
            if (stepen>-4) //если больше 10^-4 т.е. очень большое число
            {
                return ">999";
            }

            if (stepen<-9) //если больше 10^-9 т.е. очень маленькое
            {
                return "0.0";
            }
        }
    }
    else //если число без степени
    {
        IEEE754.resize(5);
        return IEEE754;
    }
}

QString MainWindow::IEEE754_3(int b) //входной HEX (32bit) переводим в BIN и высчитываем значение по формату IEEE754 // Tab 2
{

    //b - конечный байт

    qDebug()<<"STR AT b-8"<<str_tab2.at(b-7);
    qDebug()<<"STR AT b-8"<<str_tab2.at(b-6);
    qDebug()<<"STR AT b-8"<<str_tab2.at(b-5);
    qDebug()<<"STR AT b-8"<<str_tab2.at(b-4);
    qDebug()<<"STR AT b-8"<<str_tab2.at(b-3);
    qDebug()<<"STR AT b-8"<<str_tab2.at(b-2);
    qDebug()<<"STR AT b-8"<<str_tab2.at(b-1);
    qDebug()<<"STR AT"<<str_tab2.at(b);



    QString qs, qsE, qsM;
    int E,M, znak;
    float F;

    qs.clear();
    qsE.clear();
    qsM.clear();
    E=0;
    M=0;
    F=0;

    qDebug()<<"STRIEEEEE"<<str_tab2;
    qDebug()<<"STRIEEEEELEN"<<str_tab2.length();

    while (str_tab2.length()<180)
    {
        str_tab2.insert(str_tab2.length(),"0");
    }


    for (int i=0; i<4; ++i) //c b по b-8 перевели в BIN
    {
    qs.append(hextobin_3(str_tab2.at(b-1-(i*2)))); // Пр: 50,48,46...шаг=2
    qs.append(hextobin_3(str_tab2.at(b-(i*2)))); // Пр: 51,49,47...шаг=2
    }

    qDebug()<<"QSSSSTRIEEEEE"<<qs;

    /*=== Вычисляем S ===*/
    if (realint_3(qs.at(31-31))==0) //Если 31 байт 0, тогда знак "+". Иначе "-" (Вычисляем S из FLOAT)

         znak=1;

    else znak=-1;

    /*=== Вычисляем E ===*/
    for (int i=0; i<8; ++i) //c 30 по 23 байт - вычисляем E из FLOAT
   {
    qsE.append(qs.at((31-30)+i)); //31-23 = 8 т.е. меняем направление счёта. Двигаемся справа налево <-|
       qDebug()<<"qsE"<<qsE;
    }

    /*=== Вычисляем M ===*/
    for (int i=0; i<=22; ++i) //c 22 по 0 байт - вычисляем M из FLOAT
   {
    qsM.append(qs.at((32-23)+i)); //Двигаемся справа налево <-|
           qDebug()<<"qsM"<<qsM;
   }

    E=bintodec_3(qsE);
    M=bintodec_3(qsM);
               qDebug()<<"E"<<E;
                          qDebug()<<"M"<<M;
    F=znak*(qPow(2,E-127))*(1+(M/(qPow(2,23))));

    qDebug()<<"FFFF"<<F;
    return QString::number(F).toLatin1();
}

int MainWindow::realint_3(QString string) //переводим из QString "0" и "1" в Int (почему то .toint не поперло) // Tab 2
{
    if (string=="0") return 0;
    else return 1;
}

int MainWindow::bintodec_3(QString bin) //переводим из BIN в DEC // Для Tab 2
{

int result=0;

    for (int i=0; i < bin.length(); i++)
    {

            result=result+ (realint_3(bin.at(i))*qPow(2,((bin.length()-1)-i))); //значение бита * 2 в степени длина - 1
    }

return result;
}

QString MainWindow::hextobin_3(QString hex) //конвертация из HEX в BIN
{

QString hexStr = "0123456789abcdef";
QStringList List;

List << "0000" << "0001" << "0010" << "0011" << "0100" << "0101" << "0110" << "0111" << "1000" << "1001" << "1010" << "1011" << "1100" << "1101" << "1110"<< "1111";



    for (int i=0; i <= 15; ++i)
    {
        if (hex == hexStr.at(i))
        {
            hex.clear();
        return List.at(i);
        }
    }
}





//========================== Tab 3

bool MainWindow::comparestr_4(QString a, int c) // для Tab 3
{
//a - слово для сравнения (заголовки датчиков)
//c - номер ячейки в str с которой начать сравнивать

bool result=true;

    for(int i=0;i<a.length();i++)
 {

        if (str_tab3[c+i]==a.at(i))
            result=result && true;
        else
            result=result && false;
 }

    return result;

}

QString MainWindow::FormatText_4(QString IEEE754) //полученный результат приводим к 4-х значному виду: значение*(10^-6) // Tab 3
{

    //В IEEE754 приходит число. Пр.: 1.21487e-07

    QRegExp e("[e]");

    if (IEEE754.mid((IEEE754.length()-4),1)=="e") //начинаем искать 4-ый символ с конца строки (e) чтобы узнать, что число в степени
    {

        int stepen=IEEE754.right(3).toInt(); //последние три знака в строке в int: -07


        float fl;

        if (e.indexIn(IEEE754)<5)
        {
           IEEE754.insert(4,"0");
        }

        fl=IEEE754.left(5).toFloat(); //первые 5 символов слева: 1.214

        if (stepen>-9 & stepen<-4) // если число не больше 10^-4 и не меньше 10^-9
        {
            QString strl=QString::number(fl/(qPow(10,(-6-(stepen))))); // 1.214/10^(-6-(-07)) т.е. получаем 0.1214
            strl.resize(5);//возвращаем значение. Снова ограничиваем 5 первыми знаками: 0.121
            return strl;
        }
        else
        {
            if (stepen>-4) //если больше 10^-4 т.е. очень большое число
            {
                return ">999";
            }

            if (stepen<-9) //если больше 10^-9 т.е. очень маленькое
            {
                return "0.0";
            }
        }
    }
    else //если число без степени
    {
        IEEE754.resize(5);
        return IEEE754;
    }
}

QString MainWindow::IEEE754_4(int b) //входной HEX (32bit) переводим в BIN и высчитываем значение по формату IEEE754 // Tab 3
{

    //b - конечный байт

    qDebug()<<"STR AT b-8"<<str_tab3.at(b-7);
    qDebug()<<"STR AT b-8"<<str_tab3.at(b-6);
    qDebug()<<"STR AT b-8"<<str_tab3.at(b-5);
    qDebug()<<"STR AT b-8"<<str_tab3.at(b-4);
    qDebug()<<"STR AT b-8"<<str_tab3.at(b-3);
    qDebug()<<"STR AT b-8"<<str_tab3.at(b-2);
    qDebug()<<"STR AT b-8"<<str_tab3.at(b-1);
    qDebug()<<"STR AT"<<str_tab3.at(b);



    QString qs, qsE, qsM;
    int E,M, znak;
    float F;

    qs.clear();
    qsE.clear();
    qsM.clear();
    E=0;
    M=0;
    F=0;

    qDebug()<<"STRIEEEEE"<<str_tab3;
    qDebug()<<"STRIEEEEELEN"<<str_tab3.length();

    while (str_tab3.length()<180)
    {
        str_tab3.insert(str_tab3.length(),"0");
    }


    for (int i=0; i<4; ++i) //c b по b-8 перевели в BIN
    {
    qs.append(hextobin_4(str_tab3.at(b-1-(i*2)))); // Пр: 50,48,46...шаг=2
    qs.append(hextobin_4(str_tab3.at(b-(i*2)))); // Пр: 51,49,47...шаг=2
    }

    qDebug()<<"QSSSSTRIEEEEE"<<qs;

    /*=== Вычисляем S ===*/
    if (realint_4(qs.at(31-31))==0) //Если 31 байт 0, тогда знак "+". Иначе "-" (Вычисляем S из FLOAT)

         znak=1;

    else znak=-1;

    /*=== Вычисляем E ===*/
    for (int i=0; i<8; ++i) //c 30 по 23 байт - вычисляем E из FLOAT
   {
    qsE.append(qs.at((31-30)+i)); //31-23 = 8 т.е. меняем направление счёта. Двигаемся справа налево <-|
       qDebug()<<"qsE"<<qsE;
    }

    /*=== Вычисляем M ===*/
    for (int i=0; i<=22; ++i) //c 22 по 0 байт - вычисляем M из FLOAT
   {
    qsM.append(qs.at((32-23)+i)); //Двигаемся справа налево <-|
           qDebug()<<"qsM"<<qsM;
   }

    E=bintodec_4(qsE);
    M=bintodec_4(qsM);
               qDebug()<<"E"<<E;
                          qDebug()<<"M"<<M;
    F=znak*(qPow(2,E-127))*(1+(M/(qPow(2,23))));

    qDebug()<<"FFFF"<<F;
    return QString::number(F).toLatin1();
}

int MainWindow::realint_4(QString string) //переводим из QString "0" и "1" в Int (почему то .toint не поперло) // Tab 3
{
    if (string=="0") return 0;
    else return 1;
}

int MainWindow::bintodec_4(QString bin) //переводим из BIN в DEC // Для Tab 3
{

int result=0;

    for (int i=0; i < bin.length(); i++)
    {

            result=result+ (realint_4(bin.at(i))*qPow(2,((bin.length()-1)-i))); //значение бита * 2 в степени длина - 1
    }

return result;
}

QString MainWindow::hextobin_4(QString hex) //конвертация из HEX в BIN
{

QString hexStr = "0123456789abcdef";
QStringList List;

List << "0000" << "0001" << "0010" << "0011" << "0100" << "0101" << "0110" << "0111" << "1000" << "1001" << "1010" << "1011" << "1100" << "1101" << "1110"<< "1111";



    for (int i=0; i <= 15; ++i)
    {
        if (hex == hexStr.at(i))
        {
            hex.clear();
        return List.at(i);
        }
    }
}



//========================== Tab 4

bool MainWindow::comparestr_5(QString a, int c) // для Tab 4
{
//a - слово для сравнения (заголовки датчиков)
//c - номер ячейки в str с которой начать сравнивать

bool result=true;

    for(int i=0;i<a.length();i++)
 {

        if (str_tab4[c+i]==a.at(i))
            result=result && true;
        else
            result=result && false;
 }

    return result;

}

QString MainWindow::FormatText_5(QString IEEE754) //полученный результат приводим к 4-х значному виду: значение*(10^-6) // Tab 4
{

    //В IEEE754 приходит число. Пр.: 1.21487e-07

    QRegExp e("[e]");

    if (IEEE754.mid((IEEE754.length()-4),1)=="e") //начинаем искать 4-ый символ с конца строки (e) чтобы узнать, что число в степени
    {

        int stepen=IEEE754.right(3).toInt(); //последние три знака в строке в int: -07


        float fl;

        if (e.indexIn(IEEE754)<5)
        {
           IEEE754.insert(4,"0");
        }

        fl=IEEE754.left(5).toFloat(); //первые 5 символов слева: 1.214

        if (stepen>-9 & stepen<-4) // если число не больше 10^-4 и не меньше 10^-9
        {
            QString strl=QString::number(fl/(qPow(10,(-6-(stepen))))); // 1.214/10^(-6-(-07)) т.е. получаем 0.1214
            strl.resize(5);//возвращаем значение. Снова ограничиваем 5 первыми знаками: 0.121
            return strl;
        }
        else
        {
            if (stepen>-4) //если больше 10^-4 т.е. очень большое число
            {
                return ">999";
            }

            if (stepen<-9) //если больше 10^-9 т.е. очень маленькое
            {
                return "0.0";
            }
        }
    }
    else //если число без степени
    {
        IEEE754.resize(5);
        return IEEE754;
    }
}

QString MainWindow::IEEE754_5(int b) //входной HEX (32bit) переводим в BIN и высчитываем значение по формату IEEE754 // Tab 4
{

    //b - конечный байт

    qDebug()<<"STR AT b-8"<<str_tab4.at(b-7);
    qDebug()<<"STR AT b-8"<<str_tab4.at(b-6);
    qDebug()<<"STR AT b-8"<<str_tab4.at(b-5);
    qDebug()<<"STR AT b-8"<<str_tab4.at(b-4);
    qDebug()<<"STR AT b-8"<<str_tab4.at(b-3);
    qDebug()<<"STR AT b-8"<<str_tab4.at(b-2);
    qDebug()<<"STR AT b-8"<<str_tab4.at(b-1);
    qDebug()<<"STR AT"<<str_tab4.at(b);



    QString qs, qsE, qsM;
    int E,M, znak;
    float F;

    qs.clear();
    qsE.clear();
    qsM.clear();
    E=0;
    M=0;
    F=0;

    qDebug()<<"STRIEEEEE"<<str_tab4;
    qDebug()<<"STRIEEEEELEN"<<str_tab4.length();

    while (str_tab4.length()<180)
    {
        str_tab4.insert(str_tab4.length(),"0");
    }


    for (int i=0; i<4; ++i) //c b по b-8 перевели в BIN
    {
    qs.append(hextobin_5(str_tab4.at(b-1-(i*2)))); // Пр: 50,48,46...шаг=2
    qs.append(hextobin_5(str_tab4.at(b-(i*2)))); // Пр: 51,49,47...шаг=2
    }

    qDebug()<<"QSSSSTRIEEEEE"<<qs;

    /*=== Вычисляем S ===*/
    if (realint_5(qs.at(31-31))==0) //Если 31 байт 0, тогда знак "+". Иначе "-" (Вычисляем S из FLOAT)

         znak=1;

    else znak=-1;

    /*=== Вычисляем E ===*/
    for (int i=0; i<8; ++i) //c 30 по 23 байт - вычисляем E из FLOAT
   {
    qsE.append(qs.at((31-30)+i)); //31-23 = 8 т.е. меняем направление счёта. Двигаемся справа налево <-|
       qDebug()<<"qsE"<<qsE;
    }

    /*=== Вычисляем M ===*/
    for (int i=0; i<=22; ++i) //c 22 по 0 байт - вычисляем M из FLOAT
   {
    qsM.append(qs.at((32-23)+i)); //Двигаемся справа налево <-|
           qDebug()<<"qsM"<<qsM;
   }

    E=bintodec_5(qsE);
    M=bintodec_5(qsM);
               qDebug()<<"E"<<E;
                          qDebug()<<"M"<<M;
    F=znak*(qPow(2,E-127))*(1+(M/(qPow(2,23))));

    qDebug()<<"FFFF"<<F;
    return QString::number(F).toLatin1();
}

int MainWindow::realint_5(QString string) //переводим из QString "0" и "1" в Int (почему то .toint не поперло) // Tab 4
{
    if (string=="0") return 0;
    else return 1;
}

int MainWindow::bintodec_5(QString bin) //переводим из BIN в DEC // Для Tab 4
{

int result=0;

    for (int i=0; i < bin.length(); i++)
    {

            result=result+ (realint_5(bin.at(i))*qPow(2,((bin.length()-1)-i))); //значение бита * 2 в степени длина - 1
    }

return result;
}

QString MainWindow::hextobin_5(QString hex) //конвертация из HEX в BIN
{

QString hexStr = "0123456789abcdef";
QStringList List;

List << "0000" << "0001" << "0010" << "0011" << "0100" << "0101" << "0110" << "0111" << "1000" << "1001" << "1010" << "1011" << "1100" << "1101" << "1110"<< "1111";



    for (int i=0; i <= 15; ++i)
    {
        if (hex == hexStr.at(i))
        {
            hex.clear();
        return List.at(i);
        }
    }
}


//============================= _Utils =======================================================================
//============================================================================================================


//==================================== ????? =================================================================


bool MainWindow::compare(QString a, QStringList b, int c)
{
//a - слово для сравнения (заголовки датчиков)
//b - название списка
//c - номер группы


    for(int i=0;i<a.length();i++)
 {

        if (b[c].at(i)==a.at(i))
            return true;
        else
            return false;
 }
}




//=================================== ETC. ===================================================================

void MainWindow::SetPDK()
{

    QSettings *settings = new QSettings("HKEY_CURRENT_USER\\Software\\SPECTR\\ASCRHO", QSettings::NativeFormat);


    if (!settings->contains("PDKSolKisl"))
    {
        settings->setValue("PDKSolKisl", 5);
        ui->statusBar->showMessage("ПДК для соляной кислоты выставлено по умолчанию = 5.0");
    }

    PDKSolKisl = settings->value("PDKSolKisl").toFloat();
}

void MainWindow::on_pushButton_2_clicked() //Кнопка About
{
    QMessageBox::about(0, "About", "Контроль-РХ Ver. 1.0 | ВНИИ <СПЕКТР> | 2015");
}

void MainWindow::on_pushButton_clicked()
{
    timer->start(500);
    connect(timer,SIGNAL(timeout()), this, SLOT(Danger()));

}

void MainWindow::Danger()
{
   if (statusdanger==0)
   {
        ui->img_danger->setVisible(true);
        statusdanger=1;
   }
   else
   {
       ui->img_danger->setVisible(false);
       statusdanger=0;
   }
}


//============================================================================================================
//============================================================================================================
//============================================== Метео =======================================================
//============================================================================================================
//============================================================================================================


void MainWindow::on_radioButton_clicked() // Каждые 5 секунд
{
    QTimer* timermeteo = new QTimer;

    if (serial->isOpen())
    {
        if (ui->radioButton->isChecked())
        {
            timermeteo->singleShot(5000, this, SLOT(on_pb_giveMeteo_clicked()));
        }
        else
        {
            timermeteo->stop();
        }
    }

}


void MainWindow::on_pb_giveMeteo_clicked()
{

   QByteArray getmeteo=QByteArray::fromHex("54524150018cc10c");

    //0x30 0x52 0xd 0xa - test
   listhex.clear();
   //serial->clear();

   if (serial->isOpen())
   {
       serial->write(getmeteo,8);
   }
   //ui->te_meteo->setText(list);
   timer->singleShot(1000, this, SLOT(parse_meteo()));

}


void MainWindow::parse_meteo()
{

   str.clear();
   str=listhex.join("");

   ui->te_meteo->clear();


   int pos; //позиция бита заголовка


   pos=24; //Для Dn
   QString name="Мин. направление ветра";
   if (str.mid(pos,6)=="446e3d") //Dn=
   {
          if (str.mid(pos+12,2)=="23") //#
          {
             ui->te_meteo->append(name + " - недопустимые данные");
          }
          else
          {
              if (str.mid(pos+12,2)=="44") //D
              {
                 ui->te_meteo->append(name + " "+ HexToAscii(str.mid(pos+6,6))+" [град]");
              }
              else
              {
                 ui->te_meteo->append(name + " - данные не получены");
              }

          }
   }

   pos=40; //Для Dm
   name="Среднее направление ветра";
   if (str.mid(pos,6)=="446d3d") //Dm=
   {
          if (str.mid(pos+12,2)=="23") //#
          {
             ui->te_meteo->append(name + " - недопустимые данные");
          }
          else
          {
              if (str.mid(pos+12,2)=="44") //D
              {
                  Dm = HexToAscii(str.mid(pos+6,6));
                  ui->te_meteo->append(name+ " " + Dm+" [град]");
              }
              else
              {
                 ui->te_meteo->append(name + " - данные не получены");
              }

          }
   }


   pos=56; //Для Dx
   name="Максимальное направление ветра";
   if (str.mid(pos,6)=="44783d") //Dx=
   {
          if (str.mid(pos+12,2)=="23") //#
          {
             ui->te_meteo->append(name + " - недопустимые данные");
          }
          else
          {
              if (str.mid(pos+12,2)=="44") //D
              {
                 ui->te_meteo->append(name + " " + HexToAscii(str.mid(pos+6,6))+" [град]");
              }
              else
              {
                 ui->te_meteo->append(name + " - данные не получены");
              }

          }
   }

   pos=72; //Для Sn
   name="Минимальная скорость ветра";
   if (str.mid(pos,6)=="536e3d") //Sn=
   {
          if (str.mid(pos+12,2)=="23") //#
          {
             ui->te_meteo->append(name + " - недопустимые данные");
          }
          else
          {
              if (str.mid(pos+12,2)=="4d") //М
              {
                 ui->te_meteo->append(name + " " + HexToAscii(str.mid(pos+6,6))+" [м/с]");
              }
              if (str.mid(pos+12,2)=="4b") //K
              {
                 ui->te_meteo->append(name + " " + HexToAscii(str.mid(pos+6,6))+" [км/ч]");
              }
              if (str.mid(pos+12,2)=="53") //S
              {
                 ui->te_meteo->append(name + " " + HexToAscii(str.mid(pos+6,6))+" [миль/ч]");
              }
              if (str.mid(pos+12,2)=="4e") //N
              {
                 ui->te_meteo->append(name + " " + HexToAscii(str.mid(pos+6,6))+" [узлов]");
              }

          }
   }


   pos=88; //Для Sm
   name="Средняя скорость ветра";
   if (str.mid(pos,6)=="536d3d") //Sm=
   {
          if (str.mid(pos+12,2)=="23") //#
          {
             ui->te_meteo->append(name + " - недопустимые данные");
          }
          else
          {
              if (str.mid(pos+12,2)=="4d") //М
              {
                 Sm = HexToAscii(str.mid(pos+6,6));
                 ui->te_meteo->append(name + " " + Sm+" [м/с]");
              }
              if (str.mid(pos+12,2)=="4b") //K
              {
                 Sm = HexToAscii(str.mid(pos+6,6));
                 ui->te_meteo->append(name + " " + Sm+" [км/ч]");
              }
              if (str.mid(pos+12,2)=="53") //S
              {
                 Sm = HexToAscii(str.mid(pos+6,6));
                 ui->te_meteo->append(name + " " + Sm+" [миль/ч]");
              }
              if (str.mid(pos+12,2)=="4e") //N
              {
                 Sm = HexToAscii(str.mid(pos+6,6));
                  ui->te_meteo->append(name + " " + Sm+" [узлов]");
              }

          }
   }


   pos=104; //Для Sx
   name="Максимальная скорость ветра";
   if (str.mid(pos,6)=="53783d") //Sx=
   {
          if (str.mid(pos+12,2)=="23") //#
          {
             ui->te_meteo->append(name + " - недопустимые данные");
          }
          else
          {
              if (str.mid(pos+12,2)=="4d") //М
              {
                 ui->te_meteo->append(name + " " + HexToAscii(str.mid(pos+6,6))+" [м/с]");
              }
              if (str.mid(pos+12,2)=="4b") //K
              {
                 ui->te_meteo->append(name + " " + HexToAscii(str.mid(pos+6,6))+" [км/ч]");
              }
              if (str.mid(pos+12,2)=="53") //S
              {
                 ui->te_meteo->append(name + " " + HexToAscii(str.mid(pos+6,6))+" [миль/ч]");
              }
              if (str.mid(pos+12,2)=="4e") //N
              {
                 ui->te_meteo->append(name + " " + HexToAscii(str.mid(pos+6,6))+" [узлов]");
              }

          }
   }



   pos=130; //Для Ta
   qDebug()<<"pos"<<str.at(128);
   qDebug()<<"pos"<<str.at(129);
   qDebug()<<"pos"<<str.at(130);
   qDebug()<<"pos"<<str.at(131);
   qDebug()<<"pos"<<str.at(132);
   name="Температура воздуха";
   if (str.mid(pos,6)=="54613d") //Ta=
   {
       if(str.mid(pos+8,2)=="2e") //если вместо xx.x имеем формат температуры x.x, то вставляем в начало 0
       {                        //чтобы счет байт не менялся в дальнейшем
           str.insert(pos+6,"30");
       }

       if (str.mid(pos+14,2)=="23") //#
          {
             ui->te_meteo->append(name + " - недопустимые данные");
          }
          else
          {
              if (str.mid(pos+14,2)=="43") //C
              {
                 Ta = HexToAscii(str.mid(pos+6,8));
                 ui->te_meteo->append(name + " " + Ta+" [C]");
              }
              if (str.mid(pos+14,2)=="46") //F
              {
                 Ta = HexToAscii(str.mid(pos+6,8));
                 ui->te_meteo->append(name + " " + Ta+" [F]");
              }

          }
   }


   pos=148; //Для Ua
   name="Относительная влажность";
   if (str.mid(pos,6)=="55613d") //Ua=
   {

       if(str.mid(pos+8,2)=="2e") //если вместо xx.x имеем формат x.x, то вставляем в начало 0
       {                        //чтобы счет байт не менялся в дальнейшем
           str.insert(pos+6,"30");
       }

       if (str.mid(pos+14,2)=="23") //#
          {
             ui->te_meteo->append(name + " - недопустимые данные");
          }
          else
          {
              if (str.mid(pos+14,2)=="50") //P
              {
                 Ua = HexToAscii(str.mid(pos+6,8));
                 ui->te_meteo->append(name + " " + Ua+" [%]");
              }
              else
              {
                 ui->te_meteo->append(name + " - данные не получены");
              }

          }
   }


   QRegExp rePa("50613d");
   QRegExp reRc("52633d");
   QRegExp reUnit("[HPBMI]");

   int indPa, indRc, indUnit;

   indPa = rePa.indexIn(str);
   indRc = reRc.indexIn(str);
   indUnit = reUnit.indexIn(str.mid(indPa, indRc)); //выделяем строку между Pa и Rc


   pos=166; //Для Pa
   name="Атмосферное давление";
   if (str.mid(pos,6)=="50613d") //Pa=
   {


       if (str.mid(indRc-14,2)=="23") //#
          {
             ui->te_meteo->append(name + " - недопустимые данные");
          }
          else
          {
              if (str.mid(indUnit,2)=="48") //H
              {
                 Pa = HexToAscii(str.mid(indPa+6,indUnit-6));
                 ui->te_meteo->append(name + " " + Pa+" [гПа]");
              }
              if (str.mid(indUnit,2)=="50") //P
              {
                 Pa = HexToAscii(str.mid(indPa+6,indUnit-6));
                 ui->te_meteo->append(name + " " + Pa+" [Па]");
              }
              if (str.mid(indUnit,2)=="42") //B
              {
                 Pa = HexToAscii(str.mid(indPa+6,indUnit-6));
                 ui->te_meteo->append(name + " " + Pa+" [бар]");
              }
              if (str.mid(indUnit,2)=="4d") //M
              {
                 Pa = Pa = HexToAscii(str.mid(indPa+6,indUnit-6));
                 ui->te_meteo->append(name + " " + Pa+" [мм.рт.ст.]");
              }
              if (str.mid(indUnit,2)=="49") //I
              {
                 Pa = HexToAscii(str.mid(indPa+6,indUnit-6));
                 ui->te_meteo->append(name + " " + Pa+" [дюймы рт.ст.]");
              }

          }
   }


   pos=indRc; //Для Rc
   name="Накопление дождя";
   if (str.mid(pos,6)=="52633d") //Rc=
   {
          if (str.mid(pos+14,2)=="23") //#
          {
             ui->te_meteo->append(name + " - недопустимые данные");
          }
          else
          {
              if (str.mid(pos+14,2)=="4d") //M
              {
                 ui->te_meteo->append(name + " " + HexToAscii(str.mid(pos+6,8))+" [мм]");
              }
              if (str.mid(pos+14,2)=="49") //I
              {
                 ui->te_meteo->append(name + " " + HexToAscii(str.mid(pos+6,8))+" [дюймы]");
              }

          }
   }







   name="Интенсивность дождя";
   QRegExp Ri("52693d"); //для Ri
   QRegExp zpt("2c"); //для запятой
   QRegExp Octotorp("23"); //для #
   QRegExp Ravno("3d"); //для =
   QRegExp M("4d"); //для M
   QRegExp I("49"); //для I
   int poszpt, len, posravno;


   if (Ri.indexIn(str)>0) //Ri=
   {
       pos=Ri.indexIn(str);
       poszpt = zpt.indexIn(str,pos);
       posravno = Ravno.indexIn(str,pos);
       len=poszpt-pos; //длина "куска" строки

       if (Octotorp.indexIn(str.mid(pos,len))>0) //#
          {
             ui->te_meteo->append(name + " - недопустимые данные");
          }
          else
          {
              if (M.indexIn(str.mid(pos,poszpt-pos))>0)
              {
                 ui->te_meteo->append(name + " " + HexToAscii(str.mid(Ravno.indexIn(str,pos)+2,(poszpt-2)-(posravno+2)))+" [мм/ч]");
              }
              if (I.indexIn(str.mid(pos,poszpt-pos))>0)
              {
                 ui->te_meteo->append(name + " " + HexToAscii(str.mid(Ravno.indexIn(str,pos)+2,(poszpt-2)-(posravno+2)))+" [дюйм/ч]");
              }

          }
   }





   name="Накопление града";
   QRegExp Hc("48633d"); //для Hc
   QRegExp H("48"); //для H


   if (Hc.indexIn(str)>0) //Ri=
   {
       pos=Hc.indexIn(str);
       poszpt = zpt.indexIn(str,pos);
       posravno = Ravno.indexIn(str,pos);
       len=poszpt-pos; //длина "куска" строки

       if (Octotorp.indexIn(str.mid(pos,len))>0) //#
          {
             ui->te_meteo->append(name + " - недопустимые данные");
          }
          else
          {
              if (M.indexIn(str.mid(pos,poszpt-pos))>0)
              {
                 ui->te_meteo->append(name + " " + HexToAscii(str.mid(Ravno.indexIn(str,pos)+2,(poszpt-2)-(posravno+2)))+" [удары/см2]");
              }
              if (I.indexIn(str.mid(pos,poszpt-pos))>0)
              {
                 ui->te_meteo->append(name + " " + HexToAscii(str.mid(Ravno.indexIn(str,pos)+2,(poszpt-2)-(posravno+2)))+" [удары/дюйм2]");
              }
              if (H.indexIn(str.mid(pos,poszpt-pos))>0)
              {
                 ui->te_meteo->append(name + " " + HexToAscii(str.mid(Ravno.indexIn(str,pos)+2,(poszpt-2)-(posravno+2)))+" [удары]");
              }

          }
   }



   name="Продолжительность града";
   QRegExp Hd("48643d"); //для Hd
   QRegExp S("73"); //для s


   if (Hd.indexIn(str)>0) //Hd=
   {
       pos=Hd.indexIn(str);
       poszpt = zpt.indexIn(str,pos);
       posravno = Ravno.indexIn(str,pos);
       len=poszpt-pos; //длина "куска" строки

       if (Octotorp.indexIn(str.mid(pos,len))>0) //#
          {
             ui->te_meteo->append(name + " - недопустимые данные");
          }
          else
          {
              if (S.indexIn(str.mid(pos,poszpt-pos))>0)
              {
                 ui->te_meteo->append(name + " " + HexToAscii(str.mid(Ravno.indexIn(str,pos)+2,(poszpt-2)-(posravno+2)))+" [с]");
              }


          }
   }



   name="Интенсивность града"; //изменена конструкция парсинга!!!
   QRegExp Hi("48693d"); //для Hi


   if (Hi.indexIn(str)>0) //Hi=
   {
       pos=Hi.indexIn(str);
       poszpt = zpt.indexIn(str,pos);
       posravno = Ravno.indexIn(str,pos);
       len=poszpt-pos; //длина "куска" строки



       if (Octotorp.indexIn(str.mid(pos,len))>0 && str.mid(((Octotorp.indexIn(str.mid(pos,len)))+2),2)=="2c") //#
          {
             ui->te_meteo->append(name + " - недопустимые данные");
          }
          else
          {
           if (M.indexIn(str.mid(pos,poszpt-pos))>0)
           {
              //от = до первой буквы - это и есть значения
              ui->te_meteo->append(name + " " + HexToAscii(str.mid(Ravno.indexIn(str,pos)+2,6))+" [удары/см2ч]");
           }
           if (I.indexIn(str.mid(pos,poszpt-pos))>0)
           {
              ui->te_meteo->append(name + " " + HexToAscii(str.mid(Ravno.indexIn(str,pos)+2,6))+" [удары/дюйм2ч]");
           }
           if (H.indexIn(str.mid(pos,poszpt-pos))>0)
           {
              ui->te_meteo->append(name + " " + HexToAscii(str.mid(Ravno.indexIn(str,pos)+2,6))+" [удары/ч]");
           }

          }
   }

/*
   if (ui->cb_automode->isChecked())
   {
       SQLMeteo(); // пашем результаты в базу
   }
*/
}


QString MainWindow::HexToAscii(QString String)
{
    QByteArray ByteArray1=String.toUtf8();
    const char* chArr1=ByteArray1.constData();

    QByteArray ByteArray2=QByteArray::fromHex(chArr1);
    const char* chArr2=ByteArray2.constData();

    return QString::fromUtf8(chArr2);
}



//===========================================================================================================
//===========================================================================================================
//============================= GPS-датчик ==================================================================
//===========================================================================================================
//===========================================================================================================


/*

void MainWindow::on_pb_opportgps_clicked() //кнопка открыть порт-GPS
{


    sum2.clear();


    port->close();
    port->setPortName(ui->cb_gpscomport->currentText());
    port->setBaudRate(QSerialPort::Baud4800);
    port->setDataBits(QSerialPort::Data8);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
    port->setFlowControl(QSerialPort::NoFlowControl);
    if (port->open(QIODevice::ReadWrite))
    {

        qDebug()<< "COMPORT OPEN!";
        ui->statusBar->showMessage("Порт GPS открыт");
        portOpenStyleGPS();
        connect (port,SIGNAL(readyRead()),this,SLOT(ReadAll()));

    }
    else
    {
        for (int i=0;i<5;i++) //делаем ещё 5 попыток
        {
            qDebug()<< "SHOULD OPEN";
               if (port->open(QIODevice::ReadWrite))
               {
                   i=5;
                   qDebug()<< "COMPORT OPEN!";
                   ui->statusBar->showMessage("Порт GPS открыт");
                   portOpenStyleGPS();
               }
               else
               {
                   ui->statusBar->showMessage("Не удалось открыть GPS-порт");
                   qDebug()<<"COMPORT CLOSE!";
               }
        }

    }

}

void MainWindow::ReadAll ()
{

    QByteArray ba;
    QString gps;


    ba=port->readAll();
    sum2.append(ba);

    qDebug()<<"OXOXO"<<sum2;

    QRegExp reg1("RMC");
    QRegExp reg2("VTG");

    if (sum2.contains(reg1)==true)
    {
        qDebug()<<"PERVBI"<<reg1.indexIn(sum2);
        sum2.remove(0,reg1.indexIn(sum2));
        if (sum2.contains(reg2)==true)
        {
                    qDebug()<<"VTOROI"<<reg2.indexIn(sum2);
            if ((reg2.indexIn(sum2))>(reg1.indexIn(sum2))) //если $GPVTG после $GNRMC
            {
                qDebug()<<sum2;
                //ui->textEdit_2->clear();
                int a=reg1.indexIn(sum2)-2;
                int b=reg2.indexIn(sum2);
                parse_gps(gps=sum2.mid(a,(b-a)-3));
                qDebug()<<"MID"<<gps;
                //ui->textEdit_2->setText(gps);
             }

         }
    }
    else
    {

    }
}

void MainWindow::parse_gps(QString gps) //парсим GPS строчку
{

    QRegExp zpt("[,]");
    QRegExp tch("[.]");
    QStringList bl;
    QString dolg1,dolg2, dolg22;
    QString dolg1temp, dolg2temp;
    ui->textEdit_2->append("");
    ui->textEdit_2->append("===================================================");
    ui->textEdit_2->append(gps);

    for (int i=0;i<13;i++) //
    {
        bl.append(gps.left(zpt.indexIn(gps)));
        gps.remove(0,zpt.indexIn(gps)+1);
        qDebug()<<"!!!Block's!!!"<<bl;

        switch (i)
        {

        case 0:
            if (bl.at(i)=="GNRMC")
            {
                  ui->textEdit_2->append("Cпутниковая система - GNSS");
            }
            if (bl.at(i)=="GPRMC")
            {
                  ui->textEdit_2->append("Cпутниковая система - GPS");
            }
            if (bl.at(i)=="GLRMC")
            {
                  ui->textEdit_2->append("Cпутниковая система - GPS");
            }
            if (bl.at(i)=="GARMC")
            {
                  ui->textEdit_2->append("Cпутниковая система - Галилео");
            }
            break;


        case 1:
            if (bl.at(i)=="" && bl.at(i).length()==0)
            {
                ui->textEdit_2->append("Время (по Гринвичу) - данные неизвестны");
            }
            else
            {
                ui->textEdit_2->append("Время (по Гринвичу) - " + bl.at(i).mid(0,2)+":"+bl.at(i).mid(2,2)+":"+ bl.at(i).mid(4,2));
            }

            break;



        case 2:

            if (bl.at(i)=="" && bl.at(i).length()==0)
            {
                ui->textEdit_2->append("О достоверности данных неизвестно");
            }
            else
            {

                if ((bl.at(i).mid(0,1)==QString::fromLocal8Bit("A")))
                {
                    ui->textEdit_2->append("Данные достоверны. Флаг "+bl.at(i).mid(0,1));
                }
                if ((bl.at(i).mid(0,1)==QString::fromLocal8Bit("V")))
                {
                    ui->textEdit_2->append("Данные недостоверны. Флаг "+bl.at(i).mid(0,1));
                }
            }
            break;


        case 3:

            if (bl.at(i)=="" && bl.at(i).length()==0)
            {
                ui->textEdit_2->append("Широта, градусы и минуты - данные неизвестны");
            }
            else
            {
                ui->textEdit_2->append("Широта по протоколу - "+bl.at(i).mid(0,2)+" градус и "+bl.at(i).mid(2,7)+" минут");
            }
            dolg1=bl.at(i).mid(0,2)+"."+bl.at(i).mid(2,7);

            //удаляем точку на 4 или 5 знакоместе
            if (dolg1.mid(4,1)=="." )
            {
                dolg1.remove(4,1);
            }
            if (dolg1.mid(5,1)=="." )
            {
                dolg1.remove(5,1);
            }

            dolg1temp = dolg1.mid(3,dolg1.length()-3); //выносим дробную часть
            dolg1temp = QString::number((dolg1temp.toFloat())/60); //делим её на 60
            dolg1temp.remove(tch.indexIn(dolg1temp),1); //удаляем из неё точку
            dolg1 = dolg1.mid(0,3) + dolg1temp; //соединяем целую и дробную часть
            latitude = dolg1 ; //передаем в глобальную var
            break;


        case 4:
            if (bl.at(i)=="" && bl.at(i).length()==0)
            {
                ui->textEdit_2->append("Полушарие (северное/южное) - данные неизвестны");
            }
            else
            {
                if (bl.at(i)=="N")
                {
                    ui->textEdit_2->append("Полушарие - Северное");
                }
                if (bl.at(i)=="S")
                {
                    ui->textEdit_2->append("Полушарие - Южное");
                }
            }
            break;


        case 5:

            if (bl.at(i)=="" && bl.at(i).length()==0)
            {
                ui->textEdit_2->append("Долгота, градусы и минуты - данные неизвестны");
            }
            else
            {
                ui->textEdit_2->append("Долгота по протоколу - "+bl.at(i).mid(0,3)+" градус и "+bl.at(i).mid(3,7)+" минут");
            }
            dolg2=bl.at(i).mid(0,3)+"."+bl.at(i).mid(3,7);
            dolg22 = bl.at(i).mid(3,7);

            qDebug() << "DOLG22" << dolg22;



            //удаляем точку на 5 или 6 знакоместе
            if (dolg2.mid(5,1)=="." )
            {
                dolg2.remove(5,1);
            }
            if (dolg2.mid(6,1)=="." )
            {
                dolg2.remove(6,1);
            }


            dolg2temp = QString::number((dolg22.toFloat())/6); //делим её на 6 (а не на 60, поскольку там трезначное число в долготе
            qDebug() << "DOLG2temp2" << dolg2temp;
            if (dolg2temp.length()>7)
            {
                dolg2temp = dolg2temp.mid(0,7); //ограничиваем дробную часть 6-ю знаками
            }
            dolg2temp.remove(tch.indexIn(dolg2temp),1); //удаляем из неё точку
            dolg2 = dolg2.mid(0,4) + dolg2temp; //соединяем целую и дробную часть
            longitude = dolg2; //передаем в глобальную var

            break;


        case 6:
            if (bl.at(i)=="" && bl.at(i).length()==0)
            {
                ui->textEdit_2->append("Полушарие (западное/восточное) - данные неизвестны");
            }
            else
            {
                if (bl.at(i)=="E")
                {
                    ui->textEdit_2->append("Полушарие - Западное");
                }
                if (bl.at(i)=="W")
                {
                    ui->textEdit_2->append("Полушарие - Восточное");
                }
            }
            break;


        case 7:
            if (bl.at(i)=="" && bl.at(i).length()==0)
            {
                ui->textEdit_2->append("Скорость (узлов) - данные неизвестны");
            }
            else
            {
                ui->textEdit_2->append("Скорость - "+bl.at(i)+" узлов");
                ui->textEdit_2->append("Скорость - "+QString::number(bl.at(i).toInt()*1.85)+" [км/ч]");
            }
            break;


        case 8:
            if (bl.at(i)=="" && bl.at(i).length()==0)
            {
                ui->textEdit_2->append("Курс над землей (град.) - данные неизвестны");
            }
            else
            {
                ui->textEdit_2->append("Курс над землей - "+bl.at(i)+" градусов");
            }
            if (bl.at(i)==0)
            {
                ui->textEdit_2->append("Курс над землей - "+bl.at(i)+" градусов. Движение строго на север");
            }
            break;
        case 9:
            if (bl.at(i)=="" && bl.at(i).length()==0)
            {
                ui->textEdit_2->append("Дата - данные неизвестны");
            }
            else
            {
                ui->textEdit_2->append("Дата - "+bl.at(i).mid(0,2)+"."+bl.at(i).mid(2,2)+"."+bl.at(i).mid(4,2));
            }
            break;


        case 10:
            if (bl.at(i)=="" && bl.at(i).length()==0)
            {
                ui->textEdit_2->append("Магнитное склонение - данные неизвестны");
            }
            else
            {
                ui->textEdit_2->append("Магнитное склонение - "+bl.at(i));
            }
            break;
        case 11:
            if (bl.at(i)=="" && bl.at(i).length()==0)
            {
                ui->textEdit_2->append("Направление магнитного склонения - данные неизвестны");
            }
            else
            {
                ui->textEdit_2->append("Направление магнитного склонения - "+bl.at(i));
            }
            break;


        case 12:
            if (bl.at(i)=="" && bl.at(i).length()==0)
            {
                ui->textEdit_2->append("Режим  - данные неизвестны");
            }
            else
            {
                if (bl.at(i).left(1)=="m")
                {
                    ui->textEdit_2->append("Режим - навигации");
                }
                if (bl.at(i).left(1)=="N")
                {
                    ui->textEdit_2->append("Режим - недостоверные данные");
                }
                if (bl.at(i).left(1)=="A")
                {
                    ui->textEdit_2->append("Режим - автономный");
                }
                if (bl.at(i).left(1)=="D")
                {
                    ui->textEdit_2->append("Режим - дифференциальный");
                }
            }
            break;


        default:
            break;

        }
    }

    ui->textEdit_2->append("Result: "+dolg1+", "+dolg2);
    ui->textEdit_2->append("===================================================");
    ui->textEdit_2->append("");


    if (ui->cb_automode->isChecked())
    {
        SQLGPS();
    }

    port->close();
    sum2.clear();
}

void MainWindow::Readyread() // порт инициализоран. Просто читаем
{

    if (port->open(QIODevice::ReadWrite))
    {

        qDebug()<< "COMPORT OPEN!";
        connect (port,SIGNAL(readyRead()),this,SLOT(ReadAll()));

    }

    //else
    //{
    //    ui->statusBar->showMessage("Не удалось получить доступ к порту");
    //}

}

void MainWindow::on_pb_clportgps_clicked()  //кнопка "Закрыть порт"
{
    port->close();
    portCloseStyleGPS();
    timer->stop();
    ui->statusBar->showMessage("GPS-порт закрыт");
}

void MainWindow::on_pb_givegps_clicked() //кнопка "Получить координаты"
{
    if (port->open(QIODevice::ReadWrite))
    {
        connect(port, SIGNAL(readyRead()), this, SLOT(ReadAll()));
        ui->statusBar->showMessage("Получить координаты");
    }
    else
    {
        ui->statusBar->showMessage("Не удалось получить доступ к порту");
    }

}

void MainWindow::on_cb_everysecond_clicked()
{

    if (ui->cb_everysecond->isChecked())
    {
        disconnect(timer,SIGNAL(timeout()),this,SLOT(Readyread()));
        ui->pb_givegps->setEnabled(false);
        timer->start(5000);
        connect(timer,SIGNAL(timeout()),this,SLOT(Readyread()));

    }
    else
    {
        if (!ui->cb_automode)
        {
        timer->stop();
        ui->pb_givegps->setEnabled(true);
        }
    }
}

void MainWindow::portCloseStyleGPS() // описание стиля закрытого GPS-порта
{
    ui->pb_opportgps->setVisible(true);
    // С вкладки GPS-модуля убираем...
    ui->pb_clportgps->setVisible(false); //убираем "закрыть порт"
    ui->pb_givegps->setVisible(false); // убираем "Получить данные"
    ui->cb_everysecond->setVisible(false); // убираем "Каждую секунду"
    ui->textEdit_2->clear();
    ui->cb_everysecond->setChecked(false);
}

void MainWindow::portCloseStyleGPS_3() // описание стиля закрытого GPS-порта //Для Tab 2
{
    ui->pb_opportgps_3->setVisible(true);
    // С вкладки GPS-модуля убираем...
    ui->pb_clportgps_3->setVisible(false); //убираем "закрыть порт"
    ui->pb_givegps_3->setVisible(false); // убираем "Получить данные"
    ui->cb_everysecond_3->setVisible(false); // убираем "Каждую секунду"
    ui->textEdit_8->clear();
    ui->cb_everysecond_3->setChecked(false);
}

void MainWindow::portOpenStyleGPS() // описание стиля открытого GPS-порта
{
    ui->pb_opportgps->setVisible(false);
    ui->pb_clportgps->setVisible(true);
    ui->pb_givegps->setVisible(true);
    ui->cb_everysecond->setVisible(true);
}

void MainWindow::on_cb_visibleTeGPS_clicked()
{
    if(ui->cb_visibleTeGPS->isChecked())
    {
        ui->textEdit_2->setHidden(true);
    }
    else
    {
        ui->textEdit_2->setHidden(false);
    }
}
*/



//===========================================================================================================
//===========================================================================================================
//============================= Database ====================================================================
//===========================================================================================================
//===========================================================================================================



bool MainWindow::SQLBDHK() // Запись в базу с датчиков с БДХК
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");

    db.setDatabaseName("BDHK");
    if (!db.open())
    {
        qDebug()<<"DB NOT OPEN!!!"<<db.lastError();
        ui->statusBar->showMessage("Не удалось открыть базу данных");
        return false;
    }
    else
    {
        QStringList dbtables = db.tables();
        ui->statusBar->showMessage("База данных открыта");

        foreach (QString str, dbtables)
        {
            qDebug()<<"Tables:"<<str;
            //ui->textEdit_3->append(str);
        }


        // Тестовые значения

        //gHp10 = "0.35";
        //gMetan = "0.36";
        //gFtvk = "0.36";
        //gMonoYgl = "0.38";
        //gSerVod = "0.39";
        //gHlor = "0.40";
        //gGeksan =  "0.41";
        //gOksSer = "0.42";
        //gOksAzot = "0.43";
        //gSolKisl =  "0.44";
        //gPropan =  "0.45";
        //gAmmiak =  "0.46";
        //gYglGaz = "0.47";
        //gGB =  "0.48";
        //gHD =  "0.49";
        //gHCN =  "0.50";
        //gH2CO = "0.51";

        // _Тестовые значения


        // Ищем таблицу Measurement и пишем туда данные
        for (int a; a < dbtables.length(); a++)
            {
                if (dbtables.at(a)=="Measurement")
                {


                    //вычисяем дату и время

                    QTime time;
                    QString strTime;
                    QDate date;
                    QString strDate;

                    time = QTime::currentTime();
                    strTime = time.toString("hh:mm:ss");
                    date = QDate::currentDate();
                    strDate = date.toString("yyyyMMdd");

                    qDebug() << "strTime" << strTime;
                    qDebug() << "strDate" << strDate;


                    // пишем в таблицу Measurement
                    QSqlQuery query;

                    ui->statusBar->showMessage("Таблица Measurement найдена. Работаем...");



                    query.prepare("INSERT INTO Measurement (DeviceSensorId, DateTime, Value)"
                                  "VALUES(:DeviceSensorId, :DateTime, :Value);");


                    // DeviceSensorID - Уникальный идентификатор записи в таблице DeviceSensor (внешний ключ)


                    int DSID_SolKisl = 118;
                    int DSID_MonoYgl = 119;
                    int DSID_YglGaz  = 120;
                    int DSID_SerVod  = 121;
                    int DSID_OksSer  = 122;
                    int DSID_Hlor    = 123;
                    int DSID_H2CO    = 124;
                    int DSID_Ammiak  = 125;
                    int DSID_Metan   = 126;
                    int DSID_Propan  = 127;
                    int DSID_Geksan  = 128;
                    int DSID_HCN     = 129;
                    int DSID_HD      = 130;  // Зарин
                    int DSID_GB      = 131;  // Иприт
                    int DSID_HP10    = 132;
                    int DSID_Ftvk    = 133;
                    int DSID_OksAzot = 134;





                    // Hp10

                    query.bindValue(":DateTime", strDate + " " + strTime);
                    query.bindValue(":DeviceSensorId", DSID_HP10);
                    query.bindValue(":Value", gHp10); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Hp10)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Hp10) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement) успешно выполнен");
                    }



                    // Метан - CH4


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_Metan );
                    query.bindValue( ":Value", gMetan ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Метан - CH4)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Метан - CH4)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Метан - CH4) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Метан - CH4) успешно выполнен");
                    }


                    // Фтороводород - HF


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_Ftvk );
                    query.bindValue( ":Value", gFtvk ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Фтороводород - HF)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Фтороводород - HF)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Фтороводород - HF) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Фтороводород - HF) успешно выполнен");
                    }


                    // Моноксид углерода - CO


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_MonoYgl );
                    query.bindValue( ":Value", gMonoYgl ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Моноксид углерода - CO)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Моноксид углерода - CO)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Моноксид углерода - CO) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Моноксид углерода - CO) успешно выполнен");
                    }


                    // Сероводород - H2S


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_SerVod );
                    query.bindValue( ":Value", gSerVod ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Сероводород - H2S)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Сероводород - H2S)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Сероводород - H2S) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Сероводород - H2S) успешно выполнен");
                    }


                    // Хлор - Cl


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_Hlor );
                    query.bindValue( ":Value", gHlor ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Хлор - Cl)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Хлор - Cl)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Хлор - Cl) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Хлор - Cl) успешно выполнен");
                    }



                    // Гексан - C6H14


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_Geksan );
                    query.bindValue( ":Value", gGeksan ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Гексан - C6H14)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Гексан - C6H14)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Гексан - C6H14) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Гексан - C6H14) успешно выполнен");
                    }



                    // Оксид Серы - SO2


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_OksSer );
                    query.bindValue( ":Value", gOksSer ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Оксид Серы - SO2)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Оксид Серы - SO2)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Оксид Серы - SO2) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Оксид Серы - SO2) успешно выполнен");
                    }



                    // Оксид Азота - NO2


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_OksAzot );
                    query.bindValue( ":Value", gOksAzot ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Оксид Азота - NO2)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Оксид Азота - NO2)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Оксид Азота - NO2) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Оксид Азота - NO2) успешно выполнен");
                    }



                    // Соляная кислота - HCl


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_SolKisl );
                    query.bindValue( ":Value", gSolKisl ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Соляная кислота - HCl)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Соляная кислота - HCl)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Соляная кислота - HCl) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Соляная кислота - HCl) успешно выполнен");
                    }



                    // Пропан - C3H8


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_Propan );
                    query.bindValue( ":Value", gPropan ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Пропан - C3H8)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Пропан - C3H8)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Пропан - C3H8) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Пропан - C3H8) успешно выполнен");
                    }



                    // Аммиак - NH3


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_Ammiak );
                    query.bindValue( ":Value", gAmmiak ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Аммиак - NH3)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Аммиак - NH3)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Аммиак - NH3) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Аммиак - NH3) успешно выполнен");
                    }



                    // Углекислый газ - CO2


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_YglGaz );
                    query.bindValue( ":Value", gYglGaz ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Углекислый газ - CO2)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Углекислый газ - CO2)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Углекислый газ - CO2) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Углекислый газ - CO2) успешно выполнен");
                    }



                    // Иприт - GB


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_GB );
                    query.bindValue( ":Value", gGB ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Иприт - GB)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Иприт - GB)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Иприт - GB) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Иприт - GB) успешно выполнен");
                    }



                    // Зарин - HD


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_HD );
                    query.bindValue( ":Value", gHD ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Зарин - HD)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Зарин - HD)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Зарин - HD) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Зарин - HD) успешно выполнен");
                    }



                    // Соляная кислота - HCN


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_HCN );
                    query.bindValue( ":Value", gHCN ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Соляная кислота - HCN)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Соляная кислота - HCN)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Соляная кислота - HCN) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Соляная кислота - HCN) успешно выполнен");
                    }


                    // Формальдегид - H2CO


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_H2CO );
                    query.bindValue( ":Value", gH2CO ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Формальдегид - H2CO)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Формальдегид - H2CO)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Формальдегид - H2CO) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Формальдегид - H2CO) успешно выполнен");
                    }
                }
            }

       }

}

bool MainWindow::SQLBDHK_3() // Запись в базу с датчиков с БДХК // Tab 2
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");

    db.setDatabaseName("BDHK");
    if (!db.open())
    {
        qDebug()<<"DB NOT OPEN!!!"<<db.lastError();
        ui->statusBar->showMessage("Не удалось открыть базу данных // Tab 2");
        return false;
    }
    else
    {
        QStringList dbtables = db.tables();
        ui->statusBar->showMessage("База данных открыта // Tab 2");

        foreach (QString str, dbtables)
        {
            qDebug()<<"Tables:"<<str;
            //ui->textEdit_3->append(str);
        }


        // Тестовые значения

        //gHp10 = "0.35";
        //gMetan = "0.36";
        //gFtvk = "0.36";
        //gMonoYgl = "0.38";
        //gSerVod = "0.39";
        //gHlor = "0.40";
        //gGeksan =  "0.41";
        //gOksSer = "0.42";
        //gOksAzot = "0.43";
        //gSolKisl =  "0.44";
        //gPropan =  "0.45";
        //gAmmiak =  "0.46";
        //gYglGaz = "0.47";
        //gGB =  "0.48";
        //gHD =  "0.49";
        //gHCN =  "0.50";
        //gH2CO = "0.51";

        // _Тестовые значения


        // Ищем таблицу Measurement и пишем туда данные
        for (int a; a < dbtables.length(); a++)
            {
                if (dbtables.at(a)=="Measurement")
                {


                    //вычисяем дату и время

                    QTime time;
                    QString strTime;
                    QDate date;
                    QString strDate;

                    time = QTime::currentTime();
                    strTime = time.toString("hh:mm:ss");
                    date = QDate::currentDate();
                    strDate = date.toString("yyyyMMdd");

                    qDebug() << "strTime" << strTime;
                    qDebug() << "strDate" << strDate;


                    // пишем в таблицу Measurement
                    QSqlQuery query;

                    ui->statusBar->showMessage("Таблица Measurement найдена. Работаем... // Tab 2");



                    query.prepare("INSERT INTO Measurement (DeviceSensorId, DateTime, Value)"
                                  "VALUES(:DeviceSensorId, :DateTime, :Value);");


                    // DeviceSensorID - Уникальный идентификатор записи в таблице DeviceSensor (внешний ключ)


                    int DSID_SolKisl = 135;
                    int DSID_MonoYgl = 136;
                    int DSID_YglGaz  = 137;
                    int DSID_SerVod  = 138;
                    int DSID_OksSer  = 139;
                    int DSID_Hlor    = 140;
                    int DSID_H2CO    = 141;
                    int DSID_Ammiak  = 142;
                    int DSID_Metan   = 143;
                    int DSID_Propan  = 144;
                    int DSID_Geksan  = 145;
                    int DSID_HCN     = 146;
                    int DSID_HD      = 147;  // Зарин
                    int DSID_GB      = 148;  // Иприт
                    int DSID_HP10    = 149;
                    int DSID_Ftvk    = 150;
                    int DSID_OksAzot = 151;



                    // Hp10

                    query.bindValue(":DateTime", strDate + " " + strTime);
                    query.bindValue(":DeviceSensorId", DSID_HP10);
                    query.bindValue(":Value", gHp10_tab2); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Hp10 // Tab 2)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement) // Tab 2");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Hp10) успешно выполнен // Tab 2";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement) успешно выполнен // Tab 2");
                    }



                    // Метан - CH4


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_Metan );
                    query.bindValue( ":Value", gMetan_tab2 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Метан - CH4) // Tab 2";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Метан - CH4) // Tab 2");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Метан - CH4) успешно выполнен // Tab 2";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Метан - CH4) успешно выполнен // Tab 2");
                    }


                    // Фтороводород - HF


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_Ftvk );
                    query.bindValue( ":Value", gFtvk_tab2 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Фтороводород - HF) // Tab 2";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Фтороводород - HF) // Tab 2");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Фтороводород - HF) успешно выполнен // Tab 2";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Фтороводород - HF) успешно выполнен // Tab 2");
                    }


                    // Моноксид углерода - CO


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_MonoYgl );
                    query.bindValue( ":Value", gMonoYgl_tab2 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Моноксид углерода - CO) // Tab 2";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Моноксид углерода - CO) // Tab 2");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Моноксид углерода - CO) успешно выполнен // Tab 2";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Моноксид углерода - CO) успешно выполнен // Tab 2");
                    }


                    // Сероводород - H2S


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_SerVod );
                    query.bindValue( ":Value", gSerVod_tab2 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Сероводород - H2S) // Tab2";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Сероводород - H2S) // Tab 2");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Сероводород - H2S) успешно выполнен // Tab 2";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Сероводород - H2S) успешно выполнен // Tab 2");
                    }


                    // Хлор - Cl


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_Hlor );
                    query.bindValue( ":Value", gHlor_tab2 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Хлор - Cl) // Tab 2";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Хлор - Cl) // Tab 2");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Хлор - Cl) успешно выполнен // Tab 2";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Хлор - Cl) успешно выполнен // Tab 2");
                    }



                    // Гексан - C6H14


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_Geksan );
                    query.bindValue( ":Value", gGeksan_tab2 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Гексан - C6H14) // Tab 2";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Гексан - C6H14) // Tab 2");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Гексан - C6H14) успешно выполнен // Tab 2";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Гексан - C6H14) успешно выполнен // Tab 2");
                    }



                    // Оксид Серы - SO2


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_OksSer );
                    query.bindValue( ":Value", gOksSer_tab2 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Оксид Серы - SO2) // Tab 2";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Оксид Серы - SO2) // Tab 2");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Оксид Серы - SO2) успешно выполнен // Tab 2";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Оксид Серы - SO2) успешно выполнен // Tab 2");
                    }



                    // Оксид Азота - NO2


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_OksAzot );
                    query.bindValue( ":Value", gOksAzot_tab2 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Оксид Азота - NO2) // Tab 2";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Оксид Азота - NO2) // Tab 2");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Оксид Азота - NO2) успешно выполнен // Tab 2";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Оксид Азота - NO2) успешно выполнен // Tab 2");
                    }



                    // Соляная кислота - HCl


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_SolKisl );
                    query.bindValue( ":Value", gSolKisl_tab2 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Соляная кислота - HCl) // Tab 2";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Соляная кислота - HCl) // Tab 2");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Соляная кислота - HCl) успешно выполнен // Tab 2";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Соляная кислота - HCl) успешно выполнен // Tab 2");
                    }



                    // Пропан - C3H8


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_Propan );
                    query.bindValue( ":Value", gPropan_tab2 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Пропан - C3H8) // Tab 2";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Пропан - C3H8) // Tab 2");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Пропан - C3H8) успешно выполнен // Tab 2";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Пропан - C3H8) успешно выполнен // Tab 2");
                    }



                    // Аммиак - NH3


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_Ammiak );
                    query.bindValue( ":Value", gAmmiak_tab2 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Аммиак - NH3) // Tab 2";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Аммиак - NH3) // Tab 2");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Аммиак - NH3) успешно выполнен // Tab 2";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Аммиак - NH3) успешно выполнен // Tab 2");
                    }



                    // Углекислый газ - CO2


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_YglGaz );
                    query.bindValue( ":Value", gYglGaz_tab2 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Углекислый газ - CO2) // Tab 2";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Углекислый газ - CO2) // Tab 2");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Углекислый газ - CO2) успешно выполнен // Tab 2";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Углекислый газ - CO2) успешно выполнен // Tab 2");
                    }



                    // Иприт - GB


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_GB );
                    query.bindValue( ":Value", gGB_tab2 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Иприт - GB) // Tab 2";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Иприт - GB) // Tab 2");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Иприт - GB) успешно выполнен // Tab 2";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Иприт - GB) успешно выполнен // Tab 2");
                    }



                    // Зарин - HD


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_HD );
                    query.bindValue( ":Value", gHD_tab2 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Зарин - HD) // Tab 2";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Зарин - HD) // Tab 2");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Зарин - HD) успешно выполнен // Tab 2";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Зарин - HD) успешно выполнен // Tab 2");
                    }



                    // Соляная кислота - HCN


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_HCN );
                    query.bindValue( ":Value", gHCN_tab2 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Соляная кислота - HCN) // Tab 2";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Соляная кислота - HCN) // Tab 2");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Соляная кислота - HCN) успешно выполнен // Tab 2";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Соляная кислота - HCN) успешно выполнен // Tab 2");
                    }


                    // Формальдегид - H2CO


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_H2CO );
                    query.bindValue( ":Value", gH2CO_tab2 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Формальдегид - H2CO) // Tab 2";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Формальдегид - H2CO) // Tab 2");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Формальдегид - H2CO) успешно выполнен // Tab 2";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Формальдегид - H2CO) успешно выполнен // Tab 2");
                    }
                }
            }


       }

}

bool MainWindow::SQLBDHK_4() // Запись в базу с датчиков с БДХК /// Tab 3
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");

    db.setDatabaseName("BDHK");
    if (!db.open())
    {
        qDebug()<<"DB NOT OPEN!!!"<<db.lastError();
        ui->statusBar->showMessage("Не удалось открыть базу данных // Tab 3");
        return false;
    }
    else
    {
        QStringList dbtables = db.tables();
        ui->statusBar->showMessage("База данных открыта // Tab 3");

        foreach (QString str, dbtables)
        {
            qDebug()<<"Tables:"<<str;
            //ui->textEdit_3->append(str);
        }


        // Тестовые значения

        //gHp10 = "0.35";
        //gMetan = "0.36";
        //gFtvk = "0.36";
        //gMonoYgl = "0.38";
        //gSerVod = "0.39";
        //gHlor = "0.40";
        //gGeksan =  "0.41";
        //gOksSer = "0.42";
        //gOksAzot = "0.43";
        //gSolKisl =  "0.44";
        //gPropan =  "0.45";
        //gAmmiak =  "0.46";
        //gYglGaz = "0.47";
        //gGB =  "0.48";
        //gHD =  "0.49";
        //gHCN =  "0.50";
        //gH2CO = "0.51";

        // _Тестовые значения


        // Ищем таблицу Measurement и пишем туда данные
        for (int a; a < dbtables.length(); a++)
            {
                if (dbtables.at(a)=="Measurement")
                {


                    //вычисяем дату и время

                    QTime time;
                    QString strTime;
                    QDate date;
                    QString strDate;

                    time = QTime::currentTime();
                    strTime = time.toString("hh:mm:ss");
                    date = QDate::currentDate();
                    strDate = date.toString("yyyyMMdd");

                    qDebug() << "strTime" << strTime;
                    qDebug() << "strDate" << strDate;


                    // пишем в таблицу Measurement
                    QSqlQuery query;

                    ui->statusBar->showMessage("Таблица Measurement найдена. Работаем... // Tab 3");



                    query.prepare("INSERT INTO Measurement (DeviceSensorId, DateTime, Value)"
                                  "VALUES(:DeviceSensorId, :DateTime, :Value);");


                    // DeviceSensorID - Уникальный идентификатор записи в таблице DeviceSensor (внешний ключ)


                    int DSID_SolKisl = 152;
                    int DSID_MonoYgl = 153;
                    int DSID_YglGaz  = 154;
                    int DSID_SerVod  = 155;
                    int DSID_OksSer  = 156;
                    int DSID_Hlor    = 157;
                    int DSID_H2CO    = 158;
                    int DSID_Ammiak  = 159;
                    int DSID_Metan   = 160;
                    int DSID_Propan  = 161;
                    int DSID_Geksan  = 162;
                    int DSID_HCN     = 163;
                    int DSID_HD      = 164;  // Зарин
                    int DSID_GB      = 165;  // Иприт
                    int DSID_HP10    = 166;
                    int DSID_Ftvk    = 167;
                    int DSID_OksAzot = 168;



                    // Hp10

                    query.bindValue(":DateTime", strDate + " " + strTime);
                    query.bindValue(":DeviceSensorId", DSID_HP10);
                    query.bindValue(":Value", gHp10_tab3); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Hp10 // Tab 3)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement) // Tab 3");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Hp10) успешно выполнен // Tab 3";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement) успешно выполнен // Tab 3");
                    }



                    // Метан - CH4


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_Metan );
                    query.bindValue( ":Value", gMetan_tab3 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Метан - CH4) // Tab 3";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Метан - CH4) // Tab 3");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Метан - CH4) успешно выполнен // Tab 3";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Метан - CH4) успешно выполнен // Tab 3");
                    }


                    // Фтороводород - HF


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_Ftvk );
                    query.bindValue( ":Value", gFtvk_tab3 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Фтороводород - HF) // Tab 3";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Фтороводород - HF) // Tab 3");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Фтороводород - HF) успешно выполнен // Tab 3";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Фтороводород - HF) успешно выполнен // Tab 3");
                    }


                    // Моноксид углерода - CO


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_MonoYgl );
                    query.bindValue( ":Value", gMonoYgl_tab3 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Моноксид углерода - CO) // Tab 3";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Моноксид углерода - CO) // Tab 3");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Моноксид углерода - CO) успешно выполнен // Tab 3";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Моноксид углерода - CO) успешно выполнен // Tab 3");
                    }


                    // Сероводород - H2S


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_SerVod );
                    query.bindValue( ":Value", gSerVod_tab3 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Сероводород - H2S) // Tab3";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Сероводород - H2S) // Tab 3");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Сероводород - H2S) успешно выполнен // Tab 3";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Сероводород - H2S) успешно выполнен // Tab 3");
                    }


                    // Хлор - Cl


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_Hlor );
                    query.bindValue( ":Value", gHlor_tab3 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Хлор - Cl) // Tab 3";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Хлор - Cl) // Tab 3");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Хлор - Cl) успешно выполнен // Tab 3";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Хлор - Cl) успешно выполнен // Tab 3");
                    }



                    // Гексан - C6H14


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_Geksan );
                    query.bindValue( ":Value", gGeksan_tab3 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Гексан - C6H14) // Tab 3";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Гексан - C6H14) // Tab 3");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Гексан - C6H14) успешно выполнен // Tab 3";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Гексан - C6H14) успешно выполнен // Tab 3");
                    }



                    // Оксид Серы - SO2


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_OksSer );
                    query.bindValue( ":Value", gOksSer_tab3 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Оксид Серы - SO2) // Tab 3";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Оксид Серы - SO2) // Tab 3");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Оксид Серы - SO2) успешно выполнен // Tab 3";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Оксид Серы - SO2) успешно выполнен // Tab 3");
                    }



                    // Оксид Азота - NO2


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_OksAzot );
                    query.bindValue( ":Value", gOksAzot_tab3 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Оксид Азота - NO2) // Tab 3";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Оксид Азота - NO2) // Tab 3");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Оксид Азота - NO2) успешно выполнен // Tab 3";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Оксид Азота - NO2) успешно выполнен // Tab 3");
                    }



                    // Соляная кислота - HCl


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_SolKisl );
                    query.bindValue( ":Value", gSolKisl_tab3 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Соляная кислота - HCl) // Tab 3";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Соляная кислота - HCl) // Tab 3");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Соляная кислота - HCl) успешно выполнен // Tab 3";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Соляная кислота - HCl) успешно выполнен // Tab 3");
                    }



                    // Пропан - C3H8


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_Propan );
                    query.bindValue( ":Value", gPropan_tab3 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Пропан - C3H8) // Tab 3";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Пропан - C3H8) // Tab 3");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Пропан - C3H8) успешно выполнен // Tab 3";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Пропан - C3H8) успешно выполнен // Tab 3");
                    }



                    // Аммиак - NH3


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_Ammiak );
                    query.bindValue( ":Value", gAmmiak_tab3 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Аммиак - NH3) // Tab 3";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Аммиак - NH3) // Tab 3");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Аммиак - NH3) успешно выполнен // Tab 3";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Аммиак - NH3) успешно выполнен // Tab 3");
                    }



                    // Углекислый газ - CO2


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_YglGaz );
                    query.bindValue( ":Value", gYglGaz_tab3 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Углекислый газ - CO2) // Tab 3";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Углекислый газ - CO2) // Tab 3");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Углекислый газ - CO2) успешно выполнен // Tab 3";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Углекислый газ - CO2) успешно выполнен // Tab 3");
                    }



                    // Иприт - GB


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_GB );
                    query.bindValue( ":Value", gGB_tab3 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Иприт - GB) // Tab 3";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Иприт - GB) // Tab 3");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Иприт - GB) успешно выполнен // Tab 3";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Иприт - GB) успешно выполнен // Tab 3");
                    }



                    // Зарин - HD


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_HD );
                    query.bindValue( ":Value", gHD_tab3 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Зарин - HD) // Tab 3";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Зарин - HD) // Tab 3");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Зарин - HD) успешно выполнен // Tab 3";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Зарин - HD) успешно выполнен // Tab 3");
                    }



                    // Соляная кислота - HCN


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_HCN );
                    query.bindValue( ":Value", gHCN_tab3 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Соляная кислота - HCN) // Tab 3";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Соляная кислота - HCN) // Tab 3");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Соляная кислота - HCN) успешно выполнен // Tab 3";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Соляная кислота - HCN) успешно выполнен // Tab 3");
                    }


                    // Формальдегид - H2CO


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_H2CO );
                    query.bindValue( ":Value", gH2CO_tab3 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Формальдегид - H2CO) // Tab 3";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Формальдегид - H2CO) // Tab 3");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Формальдегид - H2CO) успешно выполнен // Tab 3";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Формальдегид - H2CO) успешно выполнен // Tab 3");
                    }
                }
            }


       }

}

bool MainWindow::SQLBDHK_5() // Запись в базу с датчиков с БДХК /// Tab 4
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");

    db.setDatabaseName("BDHK");
    if (!db.open())
    {
        qDebug()<<"DB NOT OPEN!!!"<<db.lastError();
        ui->statusBar->showMessage("Не удалось открыть базу данных // Tab 4");
        return false;
    }
    else
    {
        QStringList dbtables = db.tables();
        ui->statusBar->showMessage("База данных открыта // Tab 4");

        foreach (QString str, dbtables)
        {
            qDebug()<<"Tables:"<<str;
            //ui->textEdit_3->append(str);
        }


        // Тестовые значения

        //gHp10 = "0.35";
        //gMetan = "0.36";
        //gFtvk = "0.36";
        //gMonoYgl = "0.38";
        //gSerVod = "0.39";
        //gHlor = "0.40";
        //gGeksan =  "0.41";
        //gOksSer = "0.42";
        //gOksAzot = "0.43";
        //gSolKisl =  "0.44";
        //gPropan =  "0.45";
        //gAmmiak =  "0.46";
        //gYglGaz = "0.47";
        //gGB =  "0.48";
        //gHD =  "0.49";
        //gHCN =  "0.50";
        //gH2CO = "0.51";

        // _Тестовые значения


        // Ищем таблицу Measurement и пишем туда данные
        for (int a; a < dbtables.length(); a++)
            {
                if (dbtables.at(a)=="Measurement")
                {


                    //вычисяем дату и время

                    QTime time;
                    QString strTime;
                    QDate date;
                    QString strDate;

                    time = QTime::currentTime();
                    strTime = time.toString("hh:mm:ss");
                    date = QDate::currentDate();
                    strDate = date.toString("yyyyMMdd");

                    qDebug() << "strTime" << strTime;
                    qDebug() << "strDate" << strDate;


                    // пишем в таблицу Measurement
                    QSqlQuery query;

                    ui->statusBar->showMessage("Таблица Measurement найдена. Работаем... // Tab 4");



                    query.prepare("INSERT INTO Measurement (DeviceSensorId, DateTime, Value)"
                                  "VALUES(:DeviceSensorId, :DateTime, :Value);");


                    // DeviceSensorID - Уникальный идентификатор записи в таблице DeviceSensor (внешний ключ)


                    int DSID_SolKisl = 169;
                    int DSID_MonoYgl = 170;
                    int DSID_YglGaz  = 171;
                    int DSID_SerVod  = 172;
                    int DSID_OksSer  = 173;
                    int DSID_Hlor    = 174;
                    int DSID_H2CO    = 175;
                    int DSID_Ammiak  = 176;
                    int DSID_Metan   = 177;
                    int DSID_Propan  = 178;
                    int DSID_Geksan  = 179;
                    int DSID_HCN     = 180;
                    int DSID_HD      = 181;  // Зарин
                    int DSID_GB      = 182;  // Иприт
                    int DSID_HP10    = 183;
                    int DSID_Ftvk    = 184;
                    int DSID_OksAzot = 185;



                    // Hp10

                    query.bindValue(":DateTime", strDate + " " + strTime);
                    query.bindValue(":DeviceSensorId", DSID_HP10);
                    query.bindValue(":Value", gHp10_tab4); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Hp10 // Tab 4)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement) // Tab 4");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Hp10) успешно выполнен // Tab 4";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement) успешно выполнен // Tab 4");
                    }



                    // Метан - CH4


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_Metan );
                    query.bindValue( ":Value", gMetan_tab4 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Метан - CH4) // Tab 4";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Метан - CH4) // Tab 4");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Метан - CH4) успешно выполнен // Tab 4";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Метан - CH4) успешно выполнен // Tab 4");
                    }


                    // Фтороводород - HF


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_Ftvk );
                    query.bindValue( ":Value", gFtvk_tab4 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Фтороводород - HF) // Tab 4";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Фтороводород - HF) // Tab 4");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Фтороводород - HF) успешно выполнен // Tab 4";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Фтороводород - HF) успешно выполнен // Tab 4");
                    }


                    // Моноксид углерода - CO


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_MonoYgl );
                    query.bindValue( ":Value", gMonoYgl_tab4 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Моноксид углерода - CO) // Tab 4";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Моноксид углерода - CO) // Tab 4");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Моноксид углерода - CO) успешно выполнен // Tab 4";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Моноксид углерода - CO) успешно выполнен // Tab 4");
                    }


                    // Сероводород - H2S


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_SerVod );
                    query.bindValue( ":Value", gSerVod_tab4 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Сероводород - H2S) // Tab 4";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Сероводород - H2S) // Tab 4");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Сероводород - H2S) успешно выполнен // Tab 4";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Сероводород - H2S) успешно выполнен // Tab 4");
                    }


                    // Хлор - Cl


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_Hlor );
                    query.bindValue( ":Value", gHlor_tab4 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Хлор - Cl) // Tab 4";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Хлор - Cl) // Tab 4");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Хлор - Cl) успешно выполнен // Tab 4";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Хлор - Cl) успешно выполнен // Tab 4");
                    }



                    // Гексан - C6H14


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_Geksan );
                    query.bindValue( ":Value", gGeksan_tab4 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Гексан - C6H14) // Tab 4";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Гексан - C6H14) // Tab 4");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Гексан - C6H14) успешно выполнен // Tab 4";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Гексан - C6H14) успешно выполнен // Tab 4");
                    }



                    // Оксид Серы - SO2


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_OksSer );
                    query.bindValue( ":Value", gOksSer_tab4 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Оксид Серы - SO2) // Tab 4";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Оксид Серы - SO2) // Tab 4");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Оксид Серы - SO2) успешно выполнен // Tab 4";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Оксид Серы - SO2) успешно выполнен // Tab 4");
                    }



                    // Оксид Азота - NO2


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_OksAzot );
                    query.bindValue( ":Value", gOksAzot_tab4 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Оксид Азота - NO2) // Tab 4";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Оксид Азота - NO2) // Tab 4");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Оксид Азота - NO2) успешно выполнен // Tab 4";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Оксид Азота - NO2) успешно выполнен // Tab 4");
                    }



                    // Соляная кислота - HCl


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_SolKisl );
                    query.bindValue( ":Value", gSolKisl_tab4 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Соляная кислота - HCl) // Tab 4";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Соляная кислота - HCl) // Tab 4");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Соляная кислота - HCl) успешно выполнен // Tab 4";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Соляная кислота - HCl) успешно выполнен // Tab 4");
                    }



                    // Пропан - C3H8


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_Propan );
                    query.bindValue( ":Value", gPropan_tab4 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Пропан - C3H8) // Tab 4";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Пропан - C3H8) // Tab 4");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Пропан - C3H8) успешно выполнен // Tab 4";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Пропан - C3H8) успешно выполнен // Tab 4");
                    }



                    // Аммиак - NH3


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_Ammiak );
                    query.bindValue( ":Value", gAmmiak_tab4 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Аммиак - NH3) // Tab 4";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Аммиак - NH3) // Tab 4");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Аммиак - NH3) успешно выполнен // Tab 4";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Аммиак - NH3) успешно выполнен // Tab 4");
                    }



                    // Углекислый газ - CO2


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_YglGaz );
                    query.bindValue( ":Value", gYglGaz_tab4 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Углекислый газ - CO2) // Tab 4";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Углекислый газ - CO2) // Tab 4");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Углекислый газ - CO2) успешно выполнен // Tab 4";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Углекислый газ - CO2) успешно выполнен // Tab 4");
                    }



                    // Иприт - GB


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_GB );
                    query.bindValue( ":Value", gGB_tab4 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Иприт - GB) // Tab 4";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Иприт - GB) // Tab 4");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Иприт - GB) успешно выполнен // Tab 4";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Иприт - GB) успешно выполнен // Tab 4");
                    }



                    // Зарин - HD


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_HD );
                    query.bindValue( ":Value", gHD_tab4 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Зарин - HD) // Tab 4";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Зарин - HD) // Tab 4");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Зарин - HD) успешно выполнен // Tab 4";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Зарин - HD) успешно выполнен // Tab 4");
                    }



                    // Соляная кислота - HCN


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_HCN );
                    query.bindValue( ":Value", gHCN_tab4 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Соляная кислота - HCN) // Tab 4";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Соляная кислота - HCN) // Tab 4");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Соляная кислота - HCN) успешно выполнен // Tab 4";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Соляная кислота - HCN) успешно выполнен // Tab 4");
                    }


                    // Формальдегид - H2CO


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_H2CO );
                    query.bindValue( ":Value", gH2CO_tab4 ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Формальдегид - H2CO) // Tab 4";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Формальдегид - H2CO) // Tab 4");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Формальдегид - H2CO) успешно выполнен // Tab 4";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Формальдегид - H2CO) успешно выполнен // Tab 4");
                    }
                }
            }


       }

}




bool MainWindow::SQLMeteo() // Запись в базу с метео
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");

    db.setDatabaseName("BDHK");
    if (!db.open())
    {
        qDebug()<<"DB NOT OPEN!!!"<<db.lastError();
        ui->statusBar->showMessage("Не удалось открыть базу данных");
        return false;
    }
    else
    {
        QStringList dbtables = db.tables();
        ui->statusBar->showMessage("База данных открыта");

        foreach (QString str, dbtables)
        {
            qDebug()<<"Tables:"<<str;
            //ui->textEdit_3->append(str);
        }


        // Ищем таблицу Measurement и пишем туда данные
        for (int a; a < dbtables.length(); a++)
            {
                if (dbtables.at(a)=="Measurement")
                {


                    //вычисяем дату и время

                    QTime time;
                    QString strTime;
                    QDate date;
                    QString strDate;

                    time = QTime::currentTime();
                    strTime = time.toString("hh:mm:ss");
                    date = QDate::currentDate();
                    strDate = date.toString("yyyyMMdd");

                    qDebug() << "strTime" << strTime;
                    qDebug() << "strDate" << strDate;


                    // пишем в таблицу Measurement
                    QSqlQuery query;

                    ui->statusBar->showMessage("Таблица Measurement найдена. Работаем...");



                    query.prepare("INSERT INTO Measurement (DeviceSensorId, DateTime, Value)"
                                  "VALUES(:DeviceSensorId, :DateTime, :Value);");


                    int DSID_Sm = 75; // DeviceSensorID - Уникальный идентификатор записи в таблице DeviceSensor
                    int DSID_Dm = 76;
                    int DSID_Pa = 77;
                    int DSID_Ta = 78;
                    int DSID_Ua = 79;


                    // Sm - Скорость ветра

                    query.bindValue(":DateTime", strDate + " " + strTime);
                    query.bindValue(":DeviceSensorId", DSID_Sm);
                    query.bindValue(":Value", Sm); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Sm - Скорость ветра)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Sm - Скорость ветра)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Sm - Скорость ветра) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Sm - Скорость ветра) успешно выполнен");
                    }



                    // Dm - Направление ветра


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_Dm );
                    query.bindValue( ":Value", Dm ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Dm - Направление ветра)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Dm - Направление ветра)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Dm - Направление ветра) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Dm - Направление ветра) успешно выполнен");
                    }


                    // Pa - Атмосферное давление


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_Pa );
                    query.bindValue( ":Value", Pa ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Pa - Атмосферное давление)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Pa - Атмосферное давление)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Pa - Атмосферное давление) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Pa - Атмосферное давление) успешно выполнен");
                    }


                    // Ta - Температура воздуха


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_Ta );
                    query.bindValue( ":Value", Ta ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Ta - Температура воздуха)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Ta - Температура воздуха)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Ta - Температура воздуха) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Ta - Температура воздуха) успешно выполнен");
                    }


                    // Ua - Относительная влажность


                    query.bindValue( ":DateTime", strDate + " " + strTime );
                    query.bindValue( ":DeviceSensorId", DSID_Ua );
                    query.bindValue( ":Value", Ua ); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - Ua - Относительная влажность)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - Ua - Относительная влажность)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - Ua - Относительная влажность) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - Ua - Относительная влажность) успешно выполнен");
                    }
                }
            }

    }

}


bool MainWindow::SQLGPS() //запись в базу для GPS
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");

    db.setDatabaseName("BDHK");
    if (!db.open())
    {
        qDebug()<<"DB NOT OPEN!!!"<<db.lastError();
        ui->statusBar->showMessage("Не удалось открыть базу данных");
        return false;
    }
    else
    {
        QStringList dbtables = db.tables();
        ui->statusBar->showMessage("База данных открыта");

        foreach (QString str, dbtables)
        {
            qDebug()<<"Tables:"<<str;
        }



        // Ищем таблицу DeviceLocation и пишем туда GPS
        for (int a; a < dbtables.length(); a++)
            {
                if (dbtables.at(a)=="DeviceLocation")
                {
                    //вычисяем дату и время
                    QTime time;
                    QString strTime;
                    QDate date;
                    QString strDate;

                    time = QTime::currentTime();
                    strTime = time.toString("hh:mm:ss");
                    date = QDate::currentDate();
                    strDate = date.toString("yyyyMMdd");

                    qDebug() << "strTime" << strTime;
                    qDebug() << "strDate" << strDate;


                    // пишем в таблицу DeviceLocation
                    QSqlQuery query;

                    ui->statusBar->showMessage("Таблица DeviceLocation найдена. Работаем...");

                    query.prepare("INSERT INTO DeviceLocation (DateTime, DeviceId, DeviceLocationObjectId, Latitude, LocationDescription, Longitude)"
                                  "VALUES(:DateTime, :DeviceId, :DeviceLocationObjectId, :Latitude, :LocationDescription, :Longitude);");

                    query.bindValue(":DateTime", strDate + " " + strTime);
                    query.bindValue(":DeviceId", "24");
                    query.bindValue(":Latitude", latitude ); //глобальная var с блока GPS
                    query.bindValue(":Longitude", longitude); //глобальная var с блока GPS

                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (DeviceLocation)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (DeviceLocation)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (DeviceLocation) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (DeviceLocation) успешно выполнен");
                    }

                 }
            }

    }
}



/*

void MainWindow::testconnect()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");

    db.setDatabaseName("BDHK");
    if (!db.open())
    {
        qDebug()<<"DB NOT OPEN!!!"<<db.lastError();
        ui->statusBar->showMessage("Не удалось открыть базу данных");

    }
    else
    {
        QStringList dbtables = db.tables();
        ui->statusBar->showMessage("База данных открыта");

        foreach (QString str, dbtables)
        {
            qDebug()<<"Tables:"<<str;

        }



        // Ищем таблицу Measurement и пишем туда данные
        for (int a; a < dbtables.length(); a++)
            {
                if (dbtables.at(a)=="DeviceLocation")
                {



                    QSqlQuery query;




                    query.prepare("INSERT INTO DeviceLocation (DeviceId, DateTime, Latitude, Longitude, LocationDescription, DeviceLocationObjectId)"
                                  "VALUES(:DeviceId, :DateTime, :Latitude, :Longitude, :LocationDescription, :DeviceLocationObjectId);");



                    query.bindValue(":DeviceId", 13);
                    //query.bindValue(":DateTime", "2015.06.22");
                    query.bindValue(":Latitude", 00.000000);
                    query.bindValue(":Longitude", 00.000000);

                    if (!query.exec())
                    {
                        qDebug() << "LOSE";
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "YEAH!";
                    }

                }
            }
    }

}

bool MainWindow::SQLKRXO6() //запись в базу для БР РХО - 6
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");

    db.setDatabaseName("BDHK");
    if (!db.open())
    {
        qDebug()<<"DB NOT OPEN!!!"<<db.lastError();
        ui->statusBar->showMessage("Не удалось открыть базу данных");
        return false;
    }
    else
    {
        QStringList dbtables = db.tables();
        ui->statusBar->showMessage("База данных открыта");

        foreach (QString str, dbtables)
        {
            qDebug()<<"Tables:"<<str;
        }

        // Ищем таблицу Measurement и пишем туда данные
        for (int a; a < dbtables.length(); a++)
            {
                if (dbtables.at(a)=="Measurement")
                {


                    //вычисяем дату и время

                    QTime time;
                    QString strTime;
                    QDate date;
                    QString strDate;

                    time = QTime::currentTime();
                    strTime = time.toString("hh:mm:ss");
                    date = QDate::currentDate();
                    strDate = date.toString("yyyyMMdd");

                    qDebug() << "strTime" << strTime;
                    qDebug() << "strDate" << strDate;


                    // пишем в таблицу Measurement
                    QSqlQuery query;

                    ui->statusBar->showMessage("Таблица Measurement найдена. Работаем...");



                    query.prepare("INSERT INTO Measurement (DeviceSensorId, DateTime, Value)"
                                  "VALUES(:DeviceSensorId, :DateTime, :Value);");


                    int DSID_BRAmmiak = 81; // DeviceSensorID - Уникальный идентификатор записи в таблице DeviceSensor (внешний ключ)
                    int DSID_BRChlor = 82;
                    int DSID_BRDioksSer = 83;
                    int DSID_BRDioksAzot = 84;
                    int DSID_BRDoza = 85;





                    // БР Аммиак

                    query.bindValue(":DateTime", strDate + " " + strTime);
                    query.bindValue(":DeviceSensorId", DSID_BRAmmiak);
                    query.bindValue(":Value", gBRAmmiak); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - БР Аммиак)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - БР Аммиак)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - БР Аммиак) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - БР Аммиак) успешно выполнен");
                    }



                    // БР Хлор

                    query.bindValue(":DateTime", strDate + " " + strTime);
                    query.bindValue(":DeviceSensorId", DSID_BRChlor);
                    query.bindValue(":Value", gBRChlor); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - БР Хлор)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - БР Хлор)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - БР Хлор) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - БР Хлор) успешно выполнен");
                    }





                    // БР Диоксид Серы

                    query.bindValue(":DateTime", strDate + " " + strTime);
                    query.bindValue(":DeviceSensorId", DSID_BRDioksSer);
                    query.bindValue(":Value", gBRDioksSer); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - БР Диоксид Серы)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - БР Диоксид Серы)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - БР Диоксид Серы) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - БР Диоксид Серы) успешно выполнен");
                    }


                    // БР Диоксид Азота

                    query.bindValue(":DateTime", strDate + " " + strTime);
                    query.bindValue(":DeviceSensorId", DSID_BRDioksAzot);
                    query.bindValue(":Value", gBRDioksAzot); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - БР Диоксид Азота)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - БР Диоксид Азота)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - БР Диоксид Азота) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - БР Диоксид Азота) успешно выполнен");
                    }


                    // БР Доза

                    query.bindValue(":DateTime", strDate + " " + strTime);
                    query.bindValue(":DeviceSensorId", DSID_BRDoza);
                    query.bindValue(":Value", gBRDoza); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - БР Доза)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - БР Доза)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - БР Доза) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - БР Доза) успешно выполнен");
                    }


                }
        }

        for (int a; a < dbtables.length(); a++)
            {
                if (dbtables.at(a)=="DeviceLocation")
                {



                    QSqlQuery query;

                    QTime time;
                    QString strTime;
                    QDate date;
                    QString strDate;

                    time = QTime::currentTime();
                    strTime = time.toString("hh:mm:ss");
                    date = QDate::currentDate();
                    strDate = date.toString("yyyyMMdd");




                    query.prepare("INSERT INTO DeviceLocation (DeviceId, DateTime, Latitude, Longitude, LocationDescription, DeviceLocationObjectId)"
                                  "VALUES(:DeviceId, :DateTime, :Latitude, :Longitude, :LocationDescription, :DeviceLocationObjectId);");



                    query.bindValue(":DeviceId", 25);
                    query.bindValue(":DateTime", strDate + " " + strTime);
                    query.bindValue(":Latitude", brlatitude);
                    query.bindValue(":Longitude", brlongitude);

                    if (!query.exec())
                    {
                        qDebug() << "LOSE";
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "YEAH!";
                    }

                }
            }
    }
}


bool MainWindow::SQLKRXO7() //запись в базу для БР РХО - 7
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");

    db.setDatabaseName("BDHK");
    if (!db.open())
    {
        qDebug()<<"DB NOT OPEN!!!"<<db.lastError();
        ui->statusBar->showMessage("Не удалось открыть базу данных");
        return false;
    }
    else
    {
        QStringList dbtables = db.tables();
        ui->statusBar->showMessage("База данных открыта");

        foreach (QString str, dbtables)
        {
            qDebug()<<"Tables:"<<str;
        }

        // Ищем таблицу Measurement и пишем туда данные
        for (int a; a < dbtables.length(); a++)
            {
                if (dbtables.at(a)=="Measurement")
                {


                    //вычисяем дату и время

                    QTime time;
                    QString strTime;
                    QDate date;
                    QString strDate;

                    time = QTime::currentTime();
                    strTime = time.toString("hh:mm:ss");
                    date = QDate::currentDate();
                    strDate = date.toString("yyyyMMdd");

                    qDebug() << "strTime" << strTime;
                    qDebug() << "strDate" << strDate;


                    // пишем в таблицу Measurement
                    QSqlQuery query;

                    ui->statusBar->showMessage("Таблица Measurement найдена. Работаем...");



                    query.prepare("INSERT INTO Measurement (DeviceSensorId, DateTime, Value)"
                                  "VALUES(:DeviceSensorId, :DateTime, :Value);");


                    int DSID_BRAmmiak = 86; // DeviceSensorID - Уникальный идентификатор записи в таблице DeviceSensor (внешний ключ)
                    int DSID_BRChlor = 87;
                    int DSID_BRDioksSer = 88;
                    int DSID_BRDioksAzot = 89;
                    int DSID_BRDoza = 90;





                    // БР Аммиак

                    query.bindValue(":DateTime", strDate + " " + strTime);
                    query.bindValue(":DeviceSensorId", DSID_BRAmmiak);
                    query.bindValue(":Value", gBRAmmiak); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - БР Аммиак)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - БР Аммиак)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - БР Аммиак) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - БР Аммиак) успешно выполнен");
                    }



                    // БР Хлор

                    query.bindValue(":DateTime", strDate + " " + strTime);
                    query.bindValue(":DeviceSensorId", DSID_BRChlor);
                    query.bindValue(":Value", gBRChlor); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - БР Хлор)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - БР Хлор)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - БР Хлор) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - БР Хлор) успешно выполнен");
                    }





                    // БР Диоксид Серы

                    query.bindValue(":DateTime", strDate + " " + strTime);
                    query.bindValue(":DeviceSensorId", DSID_BRDioksSer);
                    query.bindValue(":Value", gBRDioksSer); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - БР Диоксид Серы)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - БР Диоксид Серы)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - БР Диоксид Серы) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - БР Диоксид Серы) успешно выполнен");
                    }


                    // БР Диоксид Азота

                    query.bindValue(":DateTime", strDate + " " + strTime);
                    query.bindValue(":DeviceSensorId", DSID_BRDioksAzot);
                    query.bindValue(":Value", gBRDioksAzot); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - БР Диоксид Азота)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - БР Диоксид Азота)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - БР Диоксид Азота) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - БР Диоксид Азота) успешно выполнен");
                    }


                    // БР Доза

                    query.bindValue(":DateTime", strDate + " " + strTime);
                    query.bindValue(":DeviceSensorId", DSID_BRDoza);
                    query.bindValue(":Value", gBRDoza); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - БР Доза)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - БР Доза)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - БР Доза) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - БР Доза) успешно выполнен");
                    }


                }
        }

        for (int a; a < dbtables.length(); a++)
            {
                if (dbtables.at(a)=="DeviceLocation")
                {



                    QSqlQuery query;

                    QTime time;
                    QString strTime;
                    QDate date;
                    QString strDate;

                    time = QTime::currentTime();
                    strTime = time.toString("hh:mm:ss");
                    date = QDate::currentDate();
                    strDate = date.toString("yyyyMMdd");




                    query.prepare("INSERT INTO DeviceLocation (DeviceId, DateTime, Latitude, Longitude, LocationDescription, DeviceLocationObjectId)"
                                  "VALUES(:DeviceId, :DateTime, :Latitude, :Longitude, :LocationDescription, :DeviceLocationObjectId);");



                    query.bindValue(":DeviceId", 28);
                    query.bindValue(":DateTime", strDate + " " + strTime);
                    query.bindValue(":Latitude", brlatitude);
                    query.bindValue(":Longitude", brlongitude);

                    if (!query.exec())
                    {
                        qDebug() << "LOSE";
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "YEAH!";
                    }

                }
            }
    }
}

bool MainWindow::SQLKRXO9() //запись в базу для БР РХО - 7
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");

    db.setDatabaseName("BDHK");
    if (!db.open())
    {
        qDebug()<<"DB NOT OPEN!!!"<<db.lastError();
        ui->statusBar->showMessage("Не удалось открыть базу данных");
        return false;
    }
    else
    {
        QStringList dbtables = db.tables();
        ui->statusBar->showMessage("База данных открыта");

        foreach (QString str, dbtables)
        {
            qDebug()<<"Tables:"<<str;
        }

        // Ищем таблицу Measurement и пишем туда данные
        for (int a; a < dbtables.length(); a++)
            {
                if (dbtables.at(a)=="Measurement")
                {


                    //вычисяем дату и время

                    QTime time;
                    QString strTime;
                    QDate date;
                    QString strDate;

                    time = QTime::currentTime();
                    strTime = time.toString("hh:mm:ss");
                    date = QDate::currentDate();
                    strDate = date.toString("yyyyMMdd");

                    qDebug() << "strTime" << strTime;
                    qDebug() << "strDate" << strDate;


                    // пишем в таблицу Measurement
                    QSqlQuery query;

                    ui->statusBar->showMessage("Таблица Measurement найдена. Работаем...");



                    query.prepare("INSERT INTO Measurement (DeviceSensorId, DateTime, Value)"
                                  "VALUES(:DeviceSensorId, :DateTime, :Value);");


                    int DSID_BRAmmiak = 91; // DeviceSensorID - Уникальный идентификатор записи в таблице DeviceSensor (внешний ключ)
                    int DSID_BRChlor = 92;
                    int DSID_BRDioksSer = 93;
                    int DSID_BRDioksAzot = 94;
                    int DSID_BRDoza = 95;





                    // БР Аммиак

                    query.bindValue(":DateTime", strDate + " " + strTime);
                    query.bindValue(":DeviceSensorId", DSID_BRAmmiak);
                    query.bindValue(":Value", gBRAmmiak); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - БР Аммиак)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - БР Аммиак)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - БР Аммиак) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - БР Аммиак) успешно выполнен");
                    }



                    // БР Хлор

                    query.bindValue(":DateTime", strDate + " " + strTime);
                    query.bindValue(":DeviceSensorId", DSID_BRChlor);
                    query.bindValue(":Value", gBRChlor); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - БР Хлор)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - БР Хлор)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - БР Хлор) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - БР Хлор) успешно выполнен");
                    }





                    // БР Диоксид Серы

                    query.bindValue(":DateTime", strDate + " " + strTime);
                    query.bindValue(":DeviceSensorId", DSID_BRDioksSer);
                    query.bindValue(":Value", gBRDioksSer); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - БР Диоксид Серы)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - БР Диоксид Серы)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - БР Диоксид Серы) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - БР Диоксид Серы) успешно выполнен");
                    }


                    // БР Диоксид Азота

                    query.bindValue(":DateTime", strDate + " " + strTime);
                    query.bindValue(":DeviceSensorId", DSID_BRDioksAzot);
                    query.bindValue(":Value", gBRDioksAzot); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - БР Диоксид Азота)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - БР Диоксид Азота)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - БР Диоксид Азота) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - БР Диоксид Азота) успешно выполнен");
                    }


                    // БР Доза

                    query.bindValue(":DateTime", strDate + " " + strTime);
                    query.bindValue(":DeviceSensorId", DSID_BRDoza);
                    query.bindValue(":Value", gBRDoza); //глобальная var


                    if (!query.exec())
                    {
                        qDebug() << "Не удалось выполнить запрос к SQL (Measurement - БР Доза)";
                        ui->statusBar->showMessage("Не удалось выполнить запрос к SQL (Measurement - БР Доза)");
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "Запрос к SQL (Measurement - БР Доза) успешно выполнен";
                        ui->statusBar->showMessage("Запрос к SQL (Measurement - БР Доза) успешно выполнен");
                    }


                }
        }

        for (int a; a < dbtables.length(); a++)
            {
                if (dbtables.at(a)=="DeviceLocation")
                {



                    QSqlQuery query;

                    QTime time;
                    QString strTime;
                    QDate date;
                    QString strDate;

                    time = QTime::currentTime();
                    strTime = time.toString("hh:mm:ss");
                    date = QDate::currentDate();
                    strDate = date.toString("yyyyMMdd");




                    query.prepare("INSERT INTO DeviceLocation (DeviceId, DateTime, Latitude, Longitude, LocationDescription, DeviceLocationObjectId)"
                                  "VALUES(:DeviceId, :DateTime, :Latitude, :Longitude, :LocationDescription, :DeviceLocationObjectId);");



                    query.bindValue(":DeviceId", 29);
                    query.bindValue(":DateTime", strDate + " " + strTime);
                    query.bindValue(":Latitude", brlatitude);
                    query.bindValue(":Longitude", brlongitude);

                    if (!query.exec())
                    {
                        qDebug() << "LOSE";
                        qDebug() << query.lastError();
                    }
                    else
                    {
                        qDebug() << "YEAH!";
                    }

                }
            }
    }
}

void MainWindow::on_pb_openBD_clicked()
{
    //SQLConnection();
    //testconnect();
}

*/

//====================================================================================================================
//====================================================================================================================
//=============================  БК РХО ==============================================================================
//====================================================================================================================
//====================================================================================================================

/*

void MainWindow::on_pb_openportkrxo_clicked()
{

    portkrxo = new QSerialPort(this);
    portkrxo->close();
    portkrxo->setPortName(ui->cb_portkrxo->currentText());
    portkrxo->setBaudRate(QSerialPort::Baud4800);
    portkrxo->setDataBits(QSerialPort::Data8);
    portkrxo->setParity(QSerialPort::NoParity);
    portkrxo->setStopBits(QSerialPort::OneStop);
    portkrxo->setFlowControl(QSerialPort::NoFlowControl);
    if (portkrxo->open(QIODevice::ReadWrite))
    {

        qDebug()<< "COMPORT KRXO OPEN!";
        connect (portkrxo,SIGNAL(readyRead()),this,SLOT(Readkrxo()));
        ui->pb_openportkrxo->setEnabled(false);
        ui->pb_closeportkrxo->setEnabled(true);
        ui->le_countkrxo->setEnabled(true);
        ui->spinBox->setEnabled(false);
        ui->statusBar->showMessage("Порт БР АСКРХО открыт!");

    }
    else
    {
        qDebug()<<"COMPORT CLOSE!";

    }


}

void MainWindow::Readkrxo()
{
    QString readport;
    readport=portkrxo->readAll().toHex();
    qDebug()<<"P"<<readport;
    emit read(readport);
}

void MainWindow::parse_rxo(QString data)
{
    QRegExp tch("[.]");
    bool ok;
    readkrxo.append(data);

    qDebug()<<"DATA:"<<readkrxo;

    qDebug()<<"DATA:lenght"<<readkrxo.length();

    if (readkrxo.length()==62 && readkrxo.endsWith("02")) //запрос параметров. Команда 0х02
    {
        rec = readkrxo.mid(readkrxo.length()-10,2) + readkrxo.mid(readkrxo.length()-12,2);
        ui->le_countkrxo->setText(QString::number(rec.toInt(&ok, 16)));
    }

    if (readkrxo.length()==230 && readkrxo.startsWith("5452415064")) //запрос результатов. Команда 09 & 03
    {
        //qDebug() << "WE NO SPEAK" << readkrxo.mid(210,8);
        //qDebug() << "FLOAT CRACK" <<FormatText(IEEE754forKRXO(readkrxo.mid(210,8),7));
        //qDebug() << "READDOLG" << readkrxo.mid(170,8);

// высчитываем широту //
            QString temp, temp2;

            temp = readkrxo.mid(170,8);
            temp2.append(temp.at(6));
            temp2.append(temp.at(7));
            temp2.append(temp.at(4));
            temp2.append(temp.at(5));
            temp2.append(temp.at(2));
            temp2.append(temp.at(3));
            temp2.append(temp.at(0));
            temp2.append(temp.at(1));

            temp = QString::number(temp2.toInt(&ok, 16));

            while (temp.length()>8) // сокращаем до 8 -ми знаков, если больше
            {
                temp.remove(temp.length()-1,1);
            }

            while (temp.length()<8) // добавляем до 8 -ми знаков, если меньше
            {
                temp.insert(0,"0");
            }

            if (temp.length() == 8)
            {
                temp.insert(2,".");
            }

            temp2.clear();
            temp2 = temp.mid(3, temp.length()-3);
            temp2 = QString::number((temp2.toFloat())/60);
            temp2 = temp2.remove(tch.indexIn(temp2), 1);


            brlatitude = temp.mid(0,3) + temp2; //пишем широту в глобальную переменную

            ui->le_krxolatit->setText(brlatitude);



            //dolg1temp = dolg1.mid(3,dolg1.length()-3); //выносим дробную часть
            //dolg1temp = QString::number((dolg1temp.toFloat())/60); //делим её на 60
            //dolg1temp.remove(tch.indexIn(dolg1temp),1); //удаляем из неё точку
            //dolg1 = dolg1.mid(0,3) + dolg1temp; //соединяем целую и дробную часть
            //latitude = dolg1 ; //передаем в глобальную var



// =========================================== //


// высчитываем долготу //

            temp.clear();
            temp2.clear();


                        temp = readkrxo.mid(178,8);
                        temp2.append(temp.at(6));
                        temp2.append(temp.at(7));
                        temp2.append(temp.at(4));
                        temp2.append(temp.at(5));
                        temp2.append(temp.at(2));
                        temp2.append(temp.at(3));
                        temp2.append(temp.at(0));
                        temp2.append(temp.at(1));


                        qDebug() << "OLOLOO2" << temp2;
                        temp = QString::number(temp2.toInt(&ok, 16));

                                                qDebug() << "OLOLOO" << temp;

                        while (temp.length()>8) // сокращаем до 8-и знаков, если больше
                        {
                            temp.remove(temp.length()-1,1);
                        }

                        while (temp.length()<9) // добавляем до 9-и знаков, если меньше
                        {
                            temp.insert(0,"0");
                        }

                        if (temp.length() == 9)
                        {
                            temp.insert(3,".");
                        }

                        int start;
                        temp2.clear();
                        temp2 = temp.mid(4, temp.length()-4);
                        qDebug() << "TEMP222" << temp2;

                        if (temp.startsWith("0"))
                        {
                            start = 0;
                        }
                        temp2 = QString::number((temp2.toFloat())/60);
                        temp2 = temp2.remove(tch.indexIn(temp2), 1);

                        if (start == 0)
                        {
                            temp2.insert(0,"0");
                        }

                        while ( temp2.length() > 6 )
                        {
                            temp2.remove(temp2.length()-1,1);
                        }



                        brlongitude = temp.mid(0,4) + temp2; //пишем широту в глобальную переменную

                        ui->le_krxolong->setText(brlongitude);

// =========================================== //






        gBRAmmiak = FormatText(IEEE754forKRXO(readkrxo.mid(186,8),7));
        ui->le_krxoammiak->setText(gBRAmmiak + " [мг/м3]");

        gBRChlor = FormatText(IEEE754forKRXO(readkrxo.mid(194,8),7));
        ui->le_krxochlor->setText(gBRChlor + " [мг/м3]");

        gBRDioksSer = FormatText(IEEE754forKRXO(readkrxo.mid(202,8),7));
        ui->le_krxodioksser->setText(gBRDioksSer + " [мг/м3]");

        gBRDioksAzot = FormatText(IEEE754forKRXO(readkrxo.mid(210,8),7));
        ui->le_krxodioksazota->setText(gBRDioksAzot + " [мг/м3]");

        gBRDoza = FormatText(IEEE754forKRXO(readkrxo.mid(218,8),7));
        qDebug() << "TEST DEBUG" << readkrxo.mid(218,8);
        ui->le_krxodoza->setText(gBRDoza + " [мкЗв]");

        if (ui->cb_automode->isChecked())
        {
            if (ui->spinBox->text() == "6")
            {
             SQLKRXO6();
            }

            if (ui->spinBox->text() == "7")
            {
             SQLKRXO7();
            }

            if (ui->spinBox->text() == "9")
            {
             SQLKRXO9();
            }

        }

    }

}

QString MainWindow::IEEE754forKRXO(QString str, int b) //входной HEX (32bit) переводим в BIN и высчитываем значение по формату IEEE754
    {

        //b - конечный байт


        qDebug()<<"STR AT"<<str.at(b);
        qDebug()<<"STR AT b-8"<<str.at(b-8);

        QString qs, qsE, qsM;
        int E,M, znak;
        float F;

        qs.clear();
        qsE.clear();
        qsM.clear();
        E=0;
        M=0;
        F=0;

        qDebug()<<"STRIEEEEE"<<str;

        for (int i=0; i<4; ++i) //c b по b-8 перевели в BIN
        {
            qs.append(hextobin(str.at(b-1-(i*2)))); // Пр: 50,48,46...шаг=2
            qs.append(hextobin(str.at(b-(i*2)))); // Пр: 51,49,47...шаг=2
        }

        qDebug()<<"QSSSSTRIEEEEE"<<qs;

        //=== Вычисляем S ===
        if (realint(qs.at(31-31))==0) //Если 31 байт 0, тогда знак "+". Иначе "-" (Вычисляем S из FLOAT)

        znak=1;

        else znak=-1;

        //=== Вычисляем E ===
        for (int i=0; i<8; ++i) //c 30 по 23 байт - вычисляем E из FLOAT
        {
            qsE.append(qs.at((31-30)+i)); //31-23 = 8 т.е. меняем направление счёта. Двигаемся справа налево <-|
            qDebug()<<"qsE"<<qsE;
        }

        //=== Вычисляем M ===
        for (int i=0; i<=22; ++i) //c 22 по 0 байт - вычисляем M из FLOAT
        {
            qsM.append(qs.at((32-23)+i)); //Двигаемся справа налево <-|
            qDebug()<<"qsM"<<qsM;
        }

        E=bintodec(qsE);
        M=bintodec(qsM);
        qDebug()<<"E"<<E;
        qDebug()<<"M"<<M;
        F=znak*(qPow(2,E-127))*(1+(M/(qPow(2,23))));

        qDebug()<<"FFFF"<<F;
        return QString::number(F).toLatin1();
    }

void MainWindow::on_pb_closeportkrxo_clicked()
{
    portkrxo->close();
    ui->pb_closeportkrxo->setEnabled(false);
    ui->pb_openportkrxo->setEnabled(true);
    ui->le_countkrxo->clear();
    ui->le_countkrxo->setEnabled(false);
    ui->spinBox->setEnabled(true);
    ui->statusBar->showMessage("Порт БР АСКРХО закрыт!");
    qDebug()<< "COMPORT KRXO CLOSE!";
}

void MainWindow::on_pushButton_4_clicked()
{
    readkrxo.clear();
    identkrxo();
}


void MainWindow::on_pushButton_7_clicked()
{

    rec.clear(); // очищаем глобальную переменную кол-ва записей в HEX
    readkrxo.clear();
    identkrxo();
    timer->singleShot(4000,this,SLOT(resultkrxo()));

}

void MainWindow::identkrxo()
{
    //char data[23]; //0x54 0x52 0x41 0x50 2 0x00 0x00 0x00 0x01 0x00 0x00 0x00
    QByteArray qbdata, qbdata2, qbdata3;
    qbdata = QByteArray::fromHex("54524150000000006400000003000000"); //23

    //qbdata[16]=0xA1;
    //qbdata[17]=0xA9;
    //qbdata[18]=0xE7;
    //qbdata[19]=0x3F;
    qbdata[20]=0x02;
    qbdata[21]=0x00;
    qbdata[22]=0x00;


    //data[0]=0x54;
    //data[1]=0x52;
    //data[2]=0x41;
    //data[3]=0x50;
    // номер КРХО
    //data[4]=0x0A;
    //data[5]=0x00;
    //data[6]=0x00;
    //data[7]=0x00;
    // _номер КРХО
    //data[8]=0x64;
    //data[9]=0x00;
    //data[10]=0x00;
    //data[11]=0x00;
    // =========
    //data[12]=0x03;
    //data[13]=0x00;
    //data[14]=0x00;
    //data[15]=0x00;
    // CRC
    // инф.
    //data[16]=0xa1;
    //data[17]=0xa9;
    // системн.
    // для 10
    //data[18]=0xe7;
    //data[19]=0x3f;
    // для 5 устр-ва
    //data[18]=0x44;
    //data[19]=0x28;

    // end CRC

    //data[20]=0x08; //команда P
    //data[21]=0x00; // I
    //data[22]=0x00; // I




    QString spbox=ui->spinBox->text();
    switch (spbox.toInt())
    {
    case 1:
        qbdata[4]=0x01;
        break;
    case 2:
        qbdata[4]=0x02;
        break;
    case 3:
        qbdata[4]=0x03;
        break;
    case 4:
        qbdata[4]=0x04;
        break;
    case 5:
        qbdata[4]=0x05;
        break;
    case 6:
        qbdata[4]=0x06;
        break;
    case 7:
        qbdata[4]=0x07;
        break;
    case 8:
        qbdata[4]=0x08;
        break;
    case 9:
        qbdata[4]=0x09;
        break;
    case 10:
        qbdata[4]=0x0A;
        break;
    case 11:
        qbdata[4]=0x0B;
        break;
    case 12:
        qbdata[4]=0x0C;
        break;
    case 13:
        qbdata[4]=0x0D;
        break;
    case 14:
        qbdata[4]=0x0E;
        break;
    case 15:
        qbdata[4]=0x0F;
        break;
    case 16:
        qbdata[4]=0x10;
        break;
    case 17:
        qbdata[4]=0x11;
    case 18:
        qbdata[4]=0x12;
    case 19:
        qbdata[4]=0x13;
    case 20:
        qbdata[4]=0x14;
        break;
    default:
        break;
    }


    QString qtr, tempstr;
    QByteArray qba;

    //================ CRC CSd ===================================

    tempstr = bintohex(CRC(datatobin(qbdata, 20, 22))); //вычисляем CRC по инф. части
    qDebug()<<"tempstr"<<tempstr;


    qtr.clear();
    qba.clear();
    qtr.append(tempstr.at(2));
    qtr.append(tempstr.at(3));
    qDebug()<<"qtr"<<qtr;
    qba=qtr.toLatin1();
    qDebug()<<"qba"<<qba;
    qbdata2=QByteArray::fromHex(qba); //CRC по инф. части


    qtr.clear();
    qba.clear();
    qtr.append(tempstr.at(0));
    qtr.append(tempstr.at(1));
    qDebug()<<"qtr"<<qtr;
    qba=qtr.toLatin1();
    qDebug()<<"qba"<<qba;
    qbdata2.append(QByteArray::fromHex(qba)); //[16-17] //CRC по инф. части




    //================ CRC CSs ===================================

    QByteArray poi;
    poi = qbdata.mid(0,16);
    poi.append(qbdata2.mid(0,2));
    poi.append(qbdata.mid(18,5));

    qDebug()<<"CRC CSs"<<bintohex(CRC(datatobin(poi, 4, 17)));

    tempstr = bintohex(CRC(datatobin(poi, 4, 17))); //вычисляем CRC по системной части


    qtr.clear();
    qba.clear();
    qtr.append(tempstr.at(2));
    qtr.append(tempstr.at(3));
    qDebug()<<"qtr"<<qtr;
    qba=qtr.toLatin1();
    qDebug()<<"qba"<<qba;
    qbdata3=QByteArray::fromHex(qba);


    qtr.clear();
    qba.clear();
    qtr.append(tempstr.at(0));
    qtr.append(tempstr.at(1));
    qDebug()<<"qtr"<<qtr;
    qba=qtr.toLatin1();
    qDebug()<<"qba"<<qba;
    qbdata3.append(QByteArray::fromHex(qba)); //[16-17]



    if (portkrxo->isOpen()) //если порт открыт
    {
    portkrxo->write(qbdata,16);
    portkrxo->write(qbdata2,2);
    portkrxo->write(qbdata3,2);
    qbdata.remove(0,20);
    portkrxo->write(qbdata,3);
    portkrxo->waitForBytesWritten(2000); //ждем пока не запишутся все данные
    }

}

QString MainWindow::datatobin(QByteArray data, int s, int e) //convert HEX to BIN + Polinom^16 (data-hex array, s-start position, e-end position)
{
    QByteArray ba, ba2;
    ba.clear();
    ba2.clear();


    for (s; s<=e; s++)
    {
        if (data.at(s)==0) //если пришла ячейка "0"
        {
            qDebug()<<"data.at(s)"<<data.at(s);
            ba.append("00000000");
        }
        else
        {

           // if (data.at(s)==1 || data.at(s)==2 ||
                //data.at(s)==3 || data.at(s)==5 || data.at(s)==6 ||
               // data.at(s)==7 || data.at(s)==8 || data.at(s)==9)
            //{
                //ba2="0000";
               // ba2.append(QByteArray::number(data[s], 2));
                //qDebug()<<"IXAAAA"<<ba2;
            //}

        //else
        //{

            ba2=QByteArray::number(data[s], 2);
            qDebug()<<"ba2"<<ba2;


            if (ba2.length()>8) //если у нас пришло ffff ffff ffff ffxx т.е. в начале ff
            {
                ba2=ba2.right(8);
                qDebug()<<"ba2RIGHT"<<ba2;
            }


            while (ba2.length()<8) //если не кратно 4-м - добавляем в начало "0"
            {
                qDebug()<<"ROCKKKK";
                ba2.insert(0,'0');
                                qDebug()<<"BA2ROCKKKK"<<ba2;
            }
       // }


            ba.append(ba2);

        }
                    qDebug()<<"ba.append(ba2)"<<ba;
    }

    ba.append("0000000000000000"); //polinom 16 stepen (x^16 + x^12 + x^5 + 1)
        qDebug()<<"BA"<<ba;
        data.clear();
    return ba;

}

QString MainWindow::CRC(QString qs) //подсчет контрольной суммы по CRC-16 CCITT (возвращает bin)
{
    QString poli="10001000000100001"; //x^16 + x^12 + x^5 + 1
    QString qstemp; //для куска из пакета
    QString restemp; // для временного результата при вычитании
    QRegExp one("[1]");
    int pos; //позиция единицы


    qDebug()<<"QS ORIGINAL"<<qs;

    if (qs.startsWith("0")) //если результата начинается с "0"
    {
        pos = one.indexIn(qs);
        qs.remove(0,pos); //удаляем первые нули до "1"
    }

    qstemp = qs.left(poli.length()); //вырезаем кусок из изначального пакета

    qs.remove(0,qstemp.length()); //удаляем из первоначального пакета в соответствии с длиной куска

    qDebug()<<"QS ORIGINAL CUT"<<qs;


    restemp.clear();
    for (int i=0; i<qstemp.length();i++) //делаем вычитание XOR
        {

        restemp.append(XOR(qstemp.at(i), poli.at(i)));
        qDebug()<<"RESTEMP"<<restemp;
        }

    qDebug()<<"RESTEMP"<<restemp;


    if (restemp.startsWith("0")) //если результата начинается с "0"
    {
        pos = one.indexIn(restemp);
        restemp.remove(0,pos); //удаляем первые нули до "1"
    }

    qDebug()<<"RESTEMP AFTER REMOVE"<<restemp;

    qstemp=restemp.insert(restemp.length(),qs.left(pos)); //вставляем кол-во символов равное удаленным нулям из первоначального qs
    qs.remove(0, pos); //удаляем из qs количество вставленных символов

        qDebug()<<"RESTEMP AFTER INSERT"<<restemp;
        qDebug()<<"QSTEMP AFTER INSERT"<<qstemp;

// ===========================================================================================
// ===========================================================================================
// ===========================================================================================


   while (poli.length()<=qs.length()+restemp.length())
   {
        restemp.clear();
        for (int i=0; i<qstemp.length();i++) //делаем вычитание XOR
           {
           restemp.append(XOR(qstemp.at(i), poli.at(i)));
           qDebug()<<"RESTEMP2"<<restemp;
           }

       qDebug()<<"RESTEMP2"<<restemp;


       if (restemp.startsWith("0")) //если результата начинается с "0"
       {
           pos = one.indexIn(restemp);
           restemp.remove(0,pos); //удаляем первые нули до "1"
       }

       qDebug()<<"RESTEMP AFTER REMOVE2"<<restemp;

       qstemp=restemp.insert(restemp.length(),qs.left(pos)); //вставляем кол-во символов равное удаленным нулям из первоначального qs
       qs.remove(0, pos); //удаляем из qs количество вставленных символов


       qDebug()<<"RESTEMP AFTER INSERT2"<<restemp;
       qDebug()<<"QSTEMP AFTER INSERT2"<<qstemp;

   }


while (qstemp.length()%4>0) //если количество не кратно 4, добавляем нули в начало
{
    qstemp.insert(0,"0");
}

qDebug()<<"QSTEMP %"<<qstemp;

qDebug()<<"BIn TO HEX"<<bintohex(qstemp);


       return qstemp;
}

QString MainWindow::bintohex(QString bin)
{
    QString hexStr = "0123456789abcdef";
    QStringList List;
    QString res;
    List << "0000" << "0001" << "0010" << "0011" << "0100" << "0101" << "0110" << "0111" << "1000" << "1001" << "1010" << "1011" << "1100" << "1101" << "1110"<<"1111";


    int binlen = bin.length();

    if ((binlen%4)==0) //если кол-во бит кратно 4
    {
        for (int n=0; n<binlen/4; n++)
        {
            for (int i=0; i<16; i++)
            {
                if (bin.left(4)==List.at(i))
                    {
                        res.append(hexStr.at(i));
                    }
            }
                bin.remove(0,4);
            }
        return res;
    }
    else
    {
        return "-1";
    }

}

QString MainWindow::XOR(QString a, QString b)
{
    if (a=="0" && b=="0")
    {
        return "0";
    }
    if (a=="1" && b=="0")
    {
        return "1";
    }
    if (a=="0" && b=="1")
    {
        return "1";
    }
    if (a=="1" && b=="1")
    {
        return "0";
    }
}

void MainWindow::resultkrxo()
{

//=====================================================================================================
//================================ Для команды 09 =====================================================
//=====================================================================================================


    int dec;
    bool ok;

    dec = rec.toInt(&ok, 16) - 1;

    rec = QString::number(dec, 16);

    while (rec.length()<4)
    {
        rec.insert(0,"0");
    }

    QString temp = rec.mid(2,2);
    QString temp2 = rec.mid(0,2);
    rec.clear();
    rec.append(temp);
    rec.append(temp2);


    qDebug() << "REC" << rec;

    QByteArray inf = QByteArray::fromHex("0000000000") + QByteArray::fromHex(rec.toLatin1()) + QByteArray::fromHex("0000");


    QString number = QString("%1").arg(ui->spinBox->text().toInt(),0,16);

    if (number.length()<2)
    {
        number.insert(0,"0");
    }

    QString qtr, tempstr;
    QByteArray qba, qbdata2, qbdata3;

    //================ CRC CSd ===================================

    tempstr = bintohex(CRC(datatobin(inf, 0, 8))); //вычисляем CRC по инф. части
    qDebug()<<"tempstrres"<<tempstr;


    qtr.clear();
    qba.clear();
    qtr.append(tempstr.at(2));
    qtr.append(tempstr.at(3));
    qDebug()<<"qtr"<<qtr;
    qba=qtr.toLatin1();
    qDebug()<<"qba"<<qba;
    qbdata2=QByteArray::fromHex(qba); //CRC по инф. части


    qtr.clear();
    qba.clear();
    qtr.append(tempstr.at(0));
    qtr.append(tempstr.at(1));
    qDebug()<<"qtr"<<qtr;
    qba=qtr.toLatin1();
    qDebug()<<"qba"<<qba;
    qbdata2.append(QByteArray::fromHex(qba)); //[16-17] //CRC по инф. части


    QByteArray head =  QByteArray::fromHex("54524150");
    QByteArray sys = QByteArray::fromHex(number.toLatin1()) + QByteArray::fromHex("0000006400000009000000");
    sys.append(qbdata2);


    //================ CRC CSs ===================================


    tempstr = bintohex(CRC(datatobin(sys, 0, 13))); //вычисляем CRC по системной части


    qtr.clear();
    qba.clear();
    qtr.append(tempstr.at(2));
    qtr.append(tempstr.at(3));
    qba=qtr.toLatin1();
    qbdata3=QByteArray::fromHex(qba);


    qtr.clear();
    qba.clear();
    qtr.append(tempstr.at(0));
    qtr.append(tempstr.at(1));
    qba=qtr.toLatin1();
    qbdata3.append(QByteArray::fromHex(qba)); //[16-17]



   if (portkrxo->isOpen()) //если порт открыт
    {
       // Для команды 09
       portkrxo->write(head,4);
       portkrxo->write(sys,14); //параметрическая часть + CRCd
       portkrxo->write(qbdata3,2); //CRCs
       portkrxo->write(inf,9); //инф. сообщение

   }

   QTimer* timerkrxo = new QTimer;
   timerkrxo->singleShot(4000,this,SLOT(resultkrxo03()));

   rec.clear();


}

void MainWindow::resultkrxo03()
{
    //=====================================================================================================
    //================================ Для команды 03 =====================================================
    //=====================================================================================================

        QString qtr, tempstr;
        QByteArray qba, qbdata2, qbdata3;

       QByteArray inf = QByteArray::fromHex("040000");
       QByteArray head =  QByteArray::fromHex("54524150");


       QString number = QString("%1").arg(ui->spinBox->text().toInt(),0,16);

       if (number.length()<2)
       {
           number.insert(0,"0");
       }


       //================ CRC CSd ===================================

       tempstr = bintohex(CRC(datatobin(inf, 0, 2))); //вычисляем CRC по инф. части
       qDebug()<<"tempstrres"<<tempstr;


       qtr.clear();
       qba.clear();
       qtr.append(tempstr.at(2));
       qtr.append(tempstr.at(3));
       qDebug()<<"qtr"<<qtr;
       qba=qtr.toLatin1();
       qDebug()<<"qba"<<qba;
       qbdata2=QByteArray::fromHex(qba); //CRC по инф. части


       qtr.clear();
       qba.clear();
       qtr.append(tempstr.at(0));
       qtr.append(tempstr.at(1));
       qDebug()<<"qtr"<<qtr;
       qba=qtr.toLatin1();
       qDebug()<<"qba"<<qba;
       qbdata2.append(QByteArray::fromHex(qba)); //[16-17] //CRC по инф. части


       QByteArray sys = QByteArray::fromHex(number.toLatin1()) + QByteArray::fromHex("0000006400000003000000");
       sys.append(qbdata2);


       //================ CRC CSs ===================================


       tempstr = bintohex(CRC(datatobin(sys, 0, 13))); //вычисляем CRC по системной части


       qtr.clear();
       qba.clear();
       qtr.append(tempstr.at(2));
       qtr.append(tempstr.at(3));
       qba=qtr.toLatin1();
       qbdata3=QByteArray::fromHex(qba);


       qtr.clear();
       qba.clear();
       qtr.append(tempstr.at(0));
       qtr.append(tempstr.at(1));
       qba=qtr.toLatin1();
       qbdata3.append(QByteArray::fromHex(qba)); //[16-17]



      if (portkrxo->isOpen()) //если порт открыт
       {
          // Для команды 03
          portkrxo->write(head,4);
          portkrxo->write(sys,14); //параметрическая часть + CRCd
          portkrxo->write(qbdata3,2); //CRCs
          portkrxo->write(inf,3); //инф. сообщение

      }

}

*/


//======================================================================================================
//======================================================================================================
//================================== GPS БДХК ==========================================================
//======================================================================================================
//======================================================================================================


/*

void MainWindow::on_pb_giveGPSBDHK_clicked()
{
    QByteArray data=QByteArray::fromHex("5452415001с66006");


    listhex.clear();

    if (serial->isOpen())
    {
        serial->write(data,8);
        serial->waitForBytesWritten(2000);
    }

    timer->singleShot(2000, this, SLOT(parse_gpsbdhk()));

}

void MainWindow::parse_gpsbdhk()
{
    str.clear();
    str=listhex.join("");
    qDebug()<<"GIIIIIIIIII"<<str;


}

*/


//======================================================================================================
//======================================================================================================
//================================== AUTO MODE =========================================================
//======================================================================================================
//======================================================================================================


void MainWindow::on_cb_automode_clicked()
{
    //QTimer* timerauto = new QTimer;
    QTimer* tmr = new QTimer;

    tmr->stop();
    disconnect(tmr,SIGNAL(timeout()),this,SLOT(on_cb_automode_clicked()));
    tmr->start(1000*65);
    connect(tmr,SIGNAL(timeout()),this,SLOT(on_cb_automode_clicked()));


    if (ui->cb_automode->isChecked())
    {

        /*
        // GPS
        ui->cb_everysecond->setChecked(true);
        on_cb_everysecond_clicked();
        ui->cb_everysecond->setEnabled(false);
        ui->pb_givegps->setEnabled(false);
        ui->pb_clportgps->setEnabled(false);
        */


        //BDHK
        if (ui->pushButton_getValue->isEnabled())
        {
            on_pushButton_getValue_clicked();
            //timerauto->singleShot(1,this,SLOT(on_pushButton_getValue_clicked()));
            //timerauto->singleShot(1000*, this, SLOT(on_pushButton_getValue_clicked()));

        }
        else
        {
            ui->statusBar->showMessage("Не настроен БДХК. Автоматический сбор данных невозможен");
        }

        // Метео - запускается автоматически после сбора БДХК

/*
        //БК РХО
       if (!ui->pb_openportkrxo->isEnabled())
       {
        on_pushButton_7_clicked();

       }


        else
       {
          ui->statusBar->showMessage("Порт БР АСКХРО. Автоматический сбор данных невозможен");
       }
*/



    }

}




//====================================== TAB 5 =========================================================
//======================================================================================================
//================================== Интеграция ========================================================
//======================================================================================================
//======================================================================================================



void MainWindow::searchASKAV()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    QSqlQuery query;
    db.setDatabaseName("BDHK");
    db.setUserName("cs3");
    db.setPassword("cs3");


    if (!db.open())
    {
        ui->statusBar->showMessage("Не удалось открыть БД для поиска АСКАВ");
        qDebug() << "Не удалось открыть БД для поиска АСКАВ" << db.lastError();
    }
    else
    {
        qDebug() << "COOL";
        query.exec("SET NAMES CP1251");
        query.exec("SELECT Name FROM Device");

        while (query.next())
        {
            qDebug() << "DEVICE ASKAV" << query.value(0).toString();
            ui->label_12->setText(query.value(0).toString());

        }

    }
}






void MainWindow::on_pushButton_3_clicked()
{
    searchASKAV();
}
