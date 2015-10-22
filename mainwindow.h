#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString readkrxo;

signals:
    void read(QString);

private slots:

    void serialReceived();
    void ComConnect();
    bool getType();
    void portCloseStyleSheet();
    void portOpenStyleSheet();
    void monitorOff();
    void monitorOn();
    void on_pushButton_2_clicked();
    void on_pushButton_opport_clicked();
    void on_pushButton_clport_clicked();
    void on_pbtn_type_clicked();
    void getlist();
    bool compare(QString a, QStringList b, int c);
    bool comparestr(QString a, int c);
    void on_pushButton_getValue_clicked();
    bool Value();
    void TimerTick();
    void Danger();
    void text_all_lineedit(QString text);
    void on_pushButton_clicked();
    bool SQLBDHK();
    void on_pb_giveMeteo_clicked();
    void parse_meteo();
    QString HexToAscii(QString String);
    void SetPDK();
    void on_cb_automode_clicked();
    bool SQLGPS();
    void on_radioButton_clicked();
    bool SQLMeteo();
    void deviceOff();





    //void on_pb_openBD_clicked();
    //void testconnect();
    //bool SQLKRXO6();
    //bool SQLKRXO7();
    //bool SQLKRXO9();
    //void ReadAll();
    //void on_pb_opportgps_clicked();
    //void parse_gps(QString gps);
    //void on_pb_openportkrxo_clicked();
    //void on_pb_closeportkrxo_clicked();
    //void on_pushButton_4_clicked();
    //void Readkrxo();
    //void on_pushButton_7_clicked();
    //void identkrxo();
    //void resultkrxo();
    //void resultkrxo03();
    //QString CRC(QString qs);
    //QString datatobin(QByteArray data, int s, int e);
    //QString XOR(QString a, QString b);
    //QString bintohex(QString bin);
    //void parse_rxo(QString data);
    //void on_pb_giveGPSBDHK_clicked();
    //void parse_gpsbdhk();
    //void Readyread();
    //void on_pb_clportgps_clicked();
    //void on_pb_givegps_clicked();
    //void on_cb_everysecond_clicked();
    //void portCloseStyleGPS();
    //void portOpenStyleGPS();
    //void on_cb_visibleTeGPS_clicked();
    //QString IEEE754forKRXO(QString str, int b);


    //для Tab 2

    void portCloseStyleSheet_3();
    //void portCloseStyleGPS_3();
    void groupbox_3(bool stat);
    void deviceOff_3();
    void portOpenStyleSheet_3();
    void serialReceived_3();
    void ComConnect_3();
    void monitorOff_3();
    void monitorOn_3();
    bool getType_3();
    void all_lineedit_enable_3(bool en);
    void text_all_lineedit_3(QString text);
    void getlist_3();
    bool comparestr_3(QString a, int c);
    void TimerTick_3();
    bool Value_3();
    QString FormatText_3(QString IEEE754);
    QString IEEE754_3(int b);
    int realint_3(QString string);
    int bintodec_3(QString bin);
    QString hextobin_3(QString hex);
    bool SQLBDHK_3();


    void on_pushButton_opport_3_clicked();
    void on_pushButton_clport_3_clicked();
    void on_pushButton_getValue_3_clicked();
    void on_pbtn_type_3_clicked();


    //для Tab 3

    void portCloseStyleSheet_4();
    void groupbox_4(bool stat);
    void deviceOff_4();
    void portOpenStyleSheet_4();
    void serialReceived_4();
    void ComConnect_4();
    void monitorOff_4();
    void monitorOn_4();
    bool getType_4();
    void all_lineedit_enable_4(bool en);
    void text_all_lineedit_4(QString text);
    void getlist_4();
    bool comparestr_4(QString a, int c);
    void TimerTick_4();
    bool Value_4();
    QString FormatText_4(QString IEEE754);
    QString IEEE754_4(int b);
    int realint_4(QString string);
    int bintodec_4(QString bin);
    QString hextobin_4(QString hex);
    bool SQLBDHK_4();


    void on_pushButton_opport_4_clicked();
    void on_pushButton_clport_4_clicked();
    void on_pushButton_getValue_4_clicked();
    void on_pbtn_type_4_clicked();



    //для Tab 4

    void portCloseStyleSheet_5();
    void groupbox_5(bool stat);
    void deviceOff_5();
    void portOpenStyleSheet_5();
    void serialReceived_5();
    void ComConnect_5();
    void monitorOff_5();
    void monitorOn_5();
    bool getType_5();
    void all_lineedit_enable_5(bool en);
    void text_all_lineedit_5(QString text);
    void getlist_5();
    bool comparestr_5(QString a, int c);
    void TimerTick_5();
    bool Value_5();
    QString FormatText_5(QString IEEE754);
    QString IEEE754_5(int b);
    int realint_5(QString string);
    int bintodec_5(QString bin);
    QString hextobin_5(QString hex);
    bool SQLBDHK_5();


    void on_pushButton_opport_5_clicked();
    void on_pushButton_clport_5_clicked();
    void on_pushButton_getValue_5_clicked();
    void on_pbtn_type_5_clicked();


    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;

    QString FormatText(QString IEEE754);
    QString IEEE754(int b);
    QString hextobin(QString hex);
    int realint(QString string);
    int bintodec(QString bin);
    void all_lineedit_enable(bool en);
    void groupbox(bool stat);

    void searchASKAV();



};

struct MyThread : public QThread { using QThread::msleep;};




#endif // MAINWINDOW_H
