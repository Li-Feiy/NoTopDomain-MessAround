#include "MainWindow.h"

#include <QApplication>
#include <QDebug>
#include <QDesktopServices>
#include <QFontDatabase>
#include <QProcess>
#include <ctime>
#include <tchar.h>
#include <thread>
#include <windows.h>
#include <winuser.h>
#pragma comment(lib, "user32.lib")

int GetProcessState(DWORD dwProcessID);
int GetProcessIDFromName(const char *name);
bool Check_Running(QString &processName);
void is_Running(MainWindow *t);
void KeyState(MainWindow *t);
void SetTop(MainWindow *t);
static void setStyle(const QString &styleName);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    int id1 = QFontDatabase::addApplicationFont(":///res/CyberFont.ttf");
    int id2 = QFontDatabase::addApplicationFont(":///res/XYBER.otf");
    if (id1 == -1 || id2 == -1)
    {
        MessageBox(NULL, TEXT("Can not set font"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
    }
    setStyle(":///res/Cyberpunk.css");
    SetWindowPos(w.MainHWND, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    std::thread C(is_Running, &w);
    C.detach();
    std::thread Key(KeyState, &w);
    Key.detach();
    std::thread Top(SetTop, &w);
    Top.detach();
    w.show();
      int e=a.exec();
    if(e==773)
    {
       QProcess::startDetached(qApp->applicationFilePath());
       return 0;
    }
    return e;
}

int GetProcessState(DWORD dwProcessID)
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

int GetProcessIDFromName(const char *name)
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

bool Check_Running(QString &processName)
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

void is_Running(MainWindow *t)
{
    QString exe = "StudentMain.exe";
    QString Radio = "屏幕广播";
    while (1)
    {
        if (Check_Running(exe))
        {
            QString pid = QString::number(GetProcessIDFromName("StudentMain.exe"));
            QString s = "<span style=color:orange>运行中</span><span style=color:gray> PID:</span>";
            s += pid;
            t->ui->Main_Button->setText("杀掉极域");
            if (GetProcessState(GetProcessIDFromName("StudentMain.exe")) == 1)
            {
                t->ui->DoMainState_Label->setText("<span style=color:red>挂起中</span>");
                t->ui->Gua_Button->setText("恢复极域");
            }
            else
            {
                t->ui->Gua_Button->setText("挂起极域");
                t->ui->Gua_Button->setEnabled(true);
                t->ui->DoMainState_Label->setText(s);
            }
        }
        else
        {
            t->ui->Gua_Button->setEnabled(false);
            t->ui->Gua_Button->setText("挂起极域");
            t->ui->Main_Button->setText("启动极域");
            t->ui->DoMainState_Label->setText("<span style=color:green>未运行</span>");
        }
        HWND RadioHWND = FindWindowW(NULL, (LPCWSTR)Radio.unicode());
        if (RadioHWND == NULL)
        {
            t->ui->Full_Button->setEnabled(false);
            t->ui->RadioState_Label->setText("<span style=color:green>未运行</span>");
        }
        else
        {
            t->ui->Full_Button->setEnabled(true);
            t->ui->RadioState_Label->setText("<span style=color:orange>进行中</span>");
        }
        Sleep(250);
    }
}

void KeyState(MainWindow *t)
{
    while (1)
    {
        if (GetAsyncKeyState(VK_MENU) && GetAsyncKeyState(0x4D))
        {
            if (t->is_Show)
            {
                t->is_Show = false;
                t->hide();
            }
            else
            {
                t->is_Show = true;
                t->show();
            }
            Sleep(100);
            continue;
        }
        Sleep(100);
    }
}

void SetTop(MainWindow *t)
{
    srand((unsigned)time(0));
    QString tTitle = t->Title + t->Version;
    while (1)
    {
        if (t->is_Top)
        {
            SetWindowPos(t->MainHWND, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        }
        else
        {
            SetWindowPos(t->MainHWND, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        }
        tTitle = t->Title + t->Version;
        if (t->is_Rand)
        {
            int num = rand() % 3 + 1;
            for (int i = 1; i <= num; i++)
            {
                int cnt = rand() % tTitle.size() + 1;
                int t = rand() % 126 + 33;
                QChar tchar = (QChar)t;
                if (tTitle[cnt] == ' ')
                    continue;
                tTitle[cnt] = tchar;
            }
        }
        else
        {
            tTitle = t->Title + t->Version;
        }
        t->setWindowTitle(tTitle);
        Sleep(1000);
    }
}

static void setStyle(const QString &styleName)
{
    QFile file(QString("%1").arg(styleName));
    file.open(QFile::ReadOnly);
    QString css = QLatin1String(file.readAll());
    qApp->setStyleSheet(css);
}