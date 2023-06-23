#include "MainWindow.h"
#include <QDebug>
#include <QDesktopServices>
#include <QIcon>
#include <QKeyEvent>
#include <QMenu>
#include <QMenuBar>
#include <QObject>
#include <QProcess>
#include <QString>
#include <QSystemTrayIcon>
#include <QUrl>
#include <comdef.h>
#include <commctrl.h>
#include <cstdlib>
#include <psapi.h>
#include <string>
#include <thread>
#include <tlhelp32.h>
#include <userenv.h>
#include <windows.h>
#include <winternl.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui_MainWindow)
{
    ui->setupUi(this);
    setFixedSize(343, 409);
    setWindowTitle(Title + Version);
    QIcon icon(":///res/logo.png");
    setWindowIcon(icon);
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(icon);
    trayIcon->setToolTip(Title + Version);
    settingAction = new QAction("配置", this);
    connect(settingAction, SIGNAL(triggered()), this, SLOT(Tray_settingAction_Clicked()));
    restoreAction = new QAction("还原", this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(Tray_restoreAction_Clicked()));
    quitAction = new QAction("退出", this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(Tray_quitAction_Clicked()));
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(settingAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayIconMenu);
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
            SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    MainHWND = (HWND)winId();
    Font.setFamily("Microsoft YaHei UI Light");
    State = new QLabel("等待操作...");
    State->setAlignment(Qt::AlignCenter);
    State->setMinimumSize(State->sizeHint());
    statusBar()->setFont(Font);
    statusBar()->setStyleSheet(QString("QStatusBar::item{border: 0px}"));
    statusBar()->addWidget(State);
    QAction *About_Action = new QAction(QStringLiteral("关于"), this);
    QAction *Feedback_Action = new QAction(QStringLiteral("反馈"), this);
    QAction *Help_Action = new QAction(QStringLiteral("帮助"), this);
    QAction *CMD_Action = new QAction(QStringLiteral("启动CMD"), this);
    QAction *TSK_Action = new QAction(QStringLiteral("启动TSK"), this);
    ui->menubar->addAction(About_Action);
    ui->menubar->addAction(Feedback_Action);
    ui->menubar->addAction(Help_Action);
    ui->menubar->addAction(CMD_Action);
    ui->menubar->addAction(TSK_Action);
    connect(About_Action, SIGNAL(triggered()), this, SLOT(Other_About_Clicked()));
    connect(Help_Action, SIGNAL(triggered()), this, SLOT(Other_Help_Clicked()));
    connect(Feedback_Action, SIGNAL(triggered()), this, SLOT(Other_Feedback_Clicked()));
    connect(CMD_Action, SIGNAL(triggered()), this, SLOT(Start_CMD_Clicked()));
    connect(TSK_Action, SIGNAL(triggered()), this, SLOT(Start_TSK_Clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 获取进程的状态
// 返回-1，表示发生异常
// 返回0，表示进程没有被挂起
// 返回1，表示进程处于挂起状态
int MainWindow::GetProcessState(DWORD dwProcessID)
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, dwProcessID);

    if (hSnapshot != INVALID_HANDLE_VALUE)
    {
        DWORD state = 1; // 先置1，一旦有线程还在运行就置0
        THREADENTRY32 te = {sizeof(te)};
        BOOL fOk = Thread32First(hSnapshot, &te);
        for (; fOk; fOk = Thread32Next(hSnapshot, &te))
        {
            if (te.th32OwnerProcessID == dwProcessID)
            {
                HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te.th32ThreadID);
                DWORD suspendCount = SuspendThread(hThread); // 返回之前的挂起数，大于0表示已挂起
                ResumeThread(hThread);                       // 马上恢复，这样不会对目标程序造成影响
                CloseHandle(hThread);
                if (suspendCount == 0)
                    state = 0; // 是个判断所有线程都挂起的好方法
            }
        }
        CloseHandle(hSnapshot);
        return state;
    }
    return -1;
}

#define KILL_FORCE 1
#define KILL_DEFAULT 2
bool MainWindow::KillProcess(DWORD dwProcessID, int way)
{
    if (way == KILL_FORCE)
    {
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, dwProcessID);

        if (hSnapshot != INVALID_HANDLE_VALUE)
        {
            bool rtn = false;
            THREADENTRY32 te = {sizeof(te)};
            BOOL fOk = Thread32First(hSnapshot, &te);
            for (; fOk; fOk = Thread32Next(hSnapshot, &te))
            {
                if (te.th32OwnerProcessID == dwProcessID)
                {
                    HANDLE hThread = OpenThread(THREAD_TERMINATE, FALSE, te.th32ThreadID);
                    if (TerminateThread(hThread, 0))
                        rtn = true;
                    CloseHandle(hThread);
                }
            }
            CloseHandle(hSnapshot);
            return rtn;
        }
        return false;
    }
    else if (way == KILL_DEFAULT)
    {
        HANDLE handle = OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessID);
        WINBOOL sta = TerminateProcess(handle, 0);
        CloseHandle(handle);
        return sta;
    }
    return false;
}

int MainWindow::GetProcessIDFromName(const char *name)
{
    int id = 0;
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (Process32First(hSnapshot, &pe))
    {
        do
        {
            if (strcmp(pe.szExeFile, name) == 0)
            {
                id = pe.th32ProcessID;
                break;
            }
        } while (Process32Next(hSnapshot, &pe));
    }
    CloseHandle(hSnapshot);
    return id;
}

void MainWindow::SuspendProcess(DWORD dwProcessID, BOOL fSuspend)
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, dwProcessID);

    if (hSnapshot != INVALID_HANDLE_VALUE)
    {

        THREADENTRY32 te = {sizeof(te)};
        BOOL fOk = Thread32First(hSnapshot, &te);
        for (; fOk; fOk = Thread32Next(hSnapshot, &te))
        {
            if (te.th32OwnerProcessID == dwProcessID)
            {
                HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te.th32ThreadID);

                if (hThread != NULL)
                {
                    if (fSuspend)
                    {
                        SuspendThread(hThread);
                    }
                    else
                    {
                        ResumeThread(hThread);
                    }
                }
                CloseHandle(hThread);
            }
        }
        CloseHandle(hSnapshot);
    }
}

bool MainWindow::Check_Running(QString &processName)
{
    QProcess process;
    QString cmd = "tasklist";
    process.start(cmd);
    process.waitForFinished();
    QString str = process.readAllStandardOutput();
    if (str.contains(processName))
    {
        return true;
    }
    else
    {
        return false;
    }
}

static void SetStyle(const QString &styleName)
{
    QFile file(QString("%1").arg(styleName));
    file.open(QFile::ReadOnly);
    QString css = QLatin1String(file.readAll());
    qApp->setStyleSheet(css);
}

void MainWindow::changeEvent(QEvent *event)
{
    if ((event->type() == QEvent::WindowStateChange) && isMinimized())
    {
        hide();
        trayIcon->show();
        event->ignore();
    }
}

void MainWindow::Start_CMD_Clicked()
{
    QProcess p(0);
    QString cmd = "cmd.exe";
    p.start("cmd", QStringList() << "/c"
                                 << "start cmd");
    p.waitForStarted();
    p.waitForFinished();
    if (Check_Running(cmd))
    {
        State->setText("CMD运行成功");
    }
    else
    {
        State->setText("CMD运行失败");
    }
}

void MainWindow::Start_PWSH_Clicked()
{
    QProcess p(0);
    QString powershell = "powershell.exe";
    p.start("cmd", QStringList() << "/c"
                                 << "start powershell");
    p.waitForStarted();
    p.waitForFinished();
    if (Check_Running(powershell))
    {
        State->setText("PowerShell运行成功");
    }
    else
    {
        State->setText("PowerShell运行失败");
    }
}

void MainWindow::Start_TSK_Clicked()
{
    QProcess p(0);
    QString TSK = "Taskmgr.exe";
    p.start("cmd", QStringList() << "/c"
                                 << "start taskmgr");
    p.waitForStarted();
    p.waitForFinished();
    if (Check_Running(TSK))
    {
        State->setText("任务管理器运行成功");
    }
    else
    {
        State->setText("任务管理器运行失败");
    }
}

void MainWindow::Other_About_Clicked()
{
    About *m_About = new About;
    m_About->show();
}

void MainWindow::Other_Help_Clicked()
{
    Help *m_Help = new Help;
    m_Help->show();
}

void MainWindow::Other_Feedback_Clicked()
{
    Feedback *m_Feedback = new Feedback;
    m_Feedback->show();
}

void MainWindow::Tray_settingAction_Clicked()
{
    show();
    ui->tabWidget->setCurrentIndex(1);
}

void MainWindow::Tray_restoreAction_Clicked()
{
    show();
}

void MainWindow::Tray_quitAction_Clicked()
{
    close();
}

void MainWindow::on_Luogu_Button_clicked()
{
    QDesktopServices::openUrl(QUrl(QString("https://www.luogu.com.cn/")));
    State->setText("启动洛谷");
}

void MainWindow::on_CF_Button_clicked()
{
    QDesktopServices::openUrl(QUrl(QString("https://codeforces.com/")));
    State->setText("启动CodeForces");
}

void MainWindow::on_Github_Button_clicked()
{
    QDesktopServices::openUrl(QUrl(QString("https://github.com/")));
    State->setText("启动Github");
}

void MainWindow::on_FZQOJ_Button_clicked()
{
    QDesktopServices::openUrl(QUrl(QString("https://qoj.fzoi.top/")));
    State->setText("启动FZQOJ");
}

void MainWindow::on_Gua_Button_clicked()
{
    DWORD pid = GetProcessIDFromName("StudentMain.exe");
    if (GetProcessState(GetProcessIDFromName(("StudentMain.exe"))) == 0)
    {
        SuspendProcess(pid, TRUE);
        MainWindow();
        if (GetProcessState(GetProcessIDFromName(("StudentMain.exe"))) == 1)
        {
            State->setText("挂起极域成功");
        }
        else
        {
            State->setText("挂起极域失败");
        }
    }
    else if (GetProcessState(GetProcessIDFromName(("StudentMain.exe"))) == 1)
    {
        SuspendProcess(pid, FALSE);
        MainWindow();
        if (GetProcessState(GetProcessIDFromName(("StudentMain.exe"))) == 0)
        {
            State->setText("恢复极域成功");
        }
        else
        {
            State->setText("恢复极域失败");
        }
    }
}

void MainWindow::on_Main_Button_clicked()
{
    QString exe = "StudentMain.exe";
    int Pid = GetProcessIDFromName("StudentMain.exe");
    if (Check_Running(exe))
    {
        KillProcess(Pid, 1);
        if (Check_Running(exe))
        {
            KillProcess(Pid, 2);
        }
        if (Check_Running(exe))
        {
            State->setText("杀掉极域失败");
        }
        else
        {
            State->setText("杀掉极域成功");
        }
    }
    else
    {
    }
}

void MainWindow::on_Kill_Button_clicked()
{
    QString name = ui->Kill_LineEdit->text();
    int n = name.size() - 1;
    DWORD pid;
    if (name[n] == 'e' && name[n - 1] == 'x' && name[n - 2] == 'e' && name[n - 3] == '.' || n == -1)
    {
        std::string SName = name.toStdString();
        const char *Name = SName.c_str();
        pid = GetProcessIDFromName(Name);
        if (!Check_Running(name))
        {
            State->setText("未找到进程");
            return;
        }
    }
    else
    {
        pid = name.toInt();
    }
    KillProcess(pid, 1);
    if (Check_Running(name))
    {
        KillProcess(pid, 2);
    }
    if (Check_Running(name))
    {
        State->setText("杀掉进程失败");
    }
    else
    {
        State->setText("杀掉进程成功");
    }
}

void MainWindow::on_Game_Button_clicked()
{
    Game *game = new Game;
    game->show();
}

void MainWindow::on_Restart_Button_clicked()
{
    qApp->exit(773);
}

void MainWindow::on_Top_CheckBox_stateChanged(int arg1)
{
    if (arg1)
    {
        is_Top = true;
        State->setText("置顶成功");
    }
    else
    {
        is_Top = false;
        State->setText("取消置顶成功");
    }
}

void MainWindow::on_RandTitle_CheckBox_stateChanged(int arg1)
{
    if (arg1)
    {
        is_Rand = true;
    }
    else
    {
        is_Rand = false;
    }
}
void MainWindow::on_Light_CheckBox_stateChanged(int arg1)
{
    if (arg1)
    {
        SetStyle(":///res/Light.css");
    }
}

void MainWindow::on_Dark_CheckBox_stateChanged(int arg1)
{
    if (arg1)
    {
        SetStyle(":///res/Dark.css");
    }
}

void MainWindow::on_Cyberpunk_CheckBox_stateChanged(int arg1)
{
    if (arg1)
    {
        SetStyle(":///res/Cyberpunk.css");
    }
}
void MainWindow::on_Transparent_HorizontalSlider_valueChanged(int value)
{
    double tran;
    tran = value / 100.0;
    setWindowOpacity(tran);
}