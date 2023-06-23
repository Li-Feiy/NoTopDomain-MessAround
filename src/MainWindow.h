#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include "About.h"
#include "Feedback.h"
#include "Game.h"
#include "Help.h"
#include "ui_MainWindow.h"
#include <QFile>
#include <QApplication>
#include <QUrl>  
#include <QEventLoop>
#include <QTimer>
#include <QFont>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QPainter>
#include <QString>
#include <QSystemTrayIcon>
#include <QThread>
#include <fstream>
#include <comdef.h>
#include <commctrl.h>
#include <tlhelp32.h>
#include <userenv.h>
#include <windows.h>
#include <winternl.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString Version = "b1.5 by Li_Feiy";
    QString Title="MessAround ";
    QLabel *State;
    QFont Font;
    HWND MainHWND;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QAction *settingAction;
    QAction *restoreAction;
    QAction *quitAction;
    bool is_Top = true;
    bool is_Show = true;
    bool is_Rand = false;
    int GetProcessState(DWORD dwProcessID);
    bool KillProcess(DWORD dwProcessID, int way);
    int GetProcessIDFromName(const char *name);
    void SuspendProcess(DWORD dwProcessID, BOOL fSuspend);
    bool Check_Running(QString &processName);
    void changeEvent(QEvent *event);

  private slots:
    void Start_CMD_Clicked();
    void Start_PWSH_Clicked();
    void Start_TSK_Clicked();
    void Other_About_Clicked();
    void Other_Help_Clicked();
    void Other_Feedback_Clicked();
    void Tray_settingAction_Clicked();
    void Tray_restoreAction_Clicked();
    void Tray_quitAction_Clicked();
    void on_Luogu_Button_clicked();
    void on_CF_Button_clicked();
    void on_Github_Button_clicked();
    void on_FZQOJ_Button_clicked();
    void on_Gua_Button_clicked();
    void on_Main_Button_clicked();
    void on_Kill_Button_clicked();
    void on_Game_Button_clicked();
    void on_Restart_Button_clicked();
    void on_Top_CheckBox_stateChanged(int arg1);
    void on_RandTitle_CheckBox_stateChanged(int arg1);
    void on_Light_CheckBox_stateChanged(int arg1);
    void on_Dark_CheckBox_stateChanged(int arg1);
    void on_Cyberpunk_CheckBox_stateChanged(int arg1);
    void on_Transparent_HorizontalSlider_valueChanged(int value);
  public:
    Ui_MainWindow *ui;
};

#endif
