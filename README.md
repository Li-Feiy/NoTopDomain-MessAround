## MessAround  帮助  

欢迎使用MessAround！希望你在机房快乐摸鱼，轻松学习，不被老师制裁和批斗，不因老师控制耽搁时间。

本软件已测试过：极域2016 v6.0 极域2021 v6.0。

2010及以下版本不做可用性保证。

这是这个软件的帮助信息，这对您顺利使用此软件提供很大的帮助。

### 快捷键

使用 `Alt+M` 唤起本软件窗口，再按即自动隐藏，隐藏后老师不会在极域的列表里面看到这个软件。

使用 `Alt+F4` 杀掉当前鼠标所在的焦点软件。

使用 `Ctrl+Shift+Delete` 快捷启动任务管理器，并且启动后软件会自动勾选置顶选项，它的置顶层级比极域的功能更高。

### 首页项

#### 常用功能·杀掉/启动极域

这个就不用说了，会结束极域进程。（注意老师可能会发现你断线，然后批斗你）

#### 常用功能·挂起/恢复极域

这会使极域进入挂起状态而不响应系统的任何命令，老师看起来也没有掉线，画面会冻结，但是大概一段时间后可能会自动掉线，只能恢复才能连上。

#### 常用功能·监视图片替换（待完善）

这将会将教师端看到您的屏幕替换成一张图片，你可以选择截图或一张图片，启动后再按可以取消。

#### 常用功能·复制极域链接

按下后会将极域的路径复制到剪贴板。

#### 常用功能·禁用/恢复全屏

这将会使全屏广播的极域变成窗口化状态，再次点击可恢复。

#### 常用功能·杀掉输入进程

输入一个进程名或其PID，然后按回车，确认后可以杀掉该进程（注意如果使用了ntsd杀进程方案可能失败）。

#### 限制解除·置顶窗口

切换软件本体窗口置顶状态，常用于截图等操作。

#### 限制解除·解除键盘限制

在黑屏安静等情况下解除极域键盘锁，默认开启（注意对 `Ctrl+Alt+Del` 无效），开启后键盘会卡顿一段时间。

#### 限制解除·解除鼠标限制

在黑屏安静等情况下解除鼠标锁，注意开启后鼠标会卡顿一段时间。

#### 限制解除·U盘/软件限制

解除U盘访问和软件运行限制，实际上是运行了：

```bash
sc stop TDFileFilter
```

注意恢复可能无效。

#### 限制解除·解除网站限制

解除访问网页时的“该网页已被阻止”，注意恢复可能无效。

#### 限制解除·解除极域防杀（待完善）

因为部分机房极域有防杀钩子，该功能可以尝试解掉钩子。

如果仍然杀不了极域的话，可以尝试用其它杀进程方案。

#### 限制解除·禁用教师截屏

通过调用 `user32.dll` 中的 `SetWindowDisplayAffinity` 来绕过截图，不过在 `Windows 10 2004` 以下的版本中会显示为黑框。

#### 限制解除·屏蔽黑屏安静

该功能可以在黑屏安静来临时自动屏蔽掉，注意会有一点延迟。

#### 限制解除·脱离远程关机

该功能通过替换极域目录的 `Shutdown.exe` 改为空程序来实现，再按可以恢复。

当老师（或同学）发现其它的电脑都关了，你的却没关时，请注意您可能会被当作走资派给批斗、打倒（

#### 限制解除·拦截教师控制（待完善）

可以让老师无法控制你的电脑，并且不会导致选择“控制”时本地不可以操作的现象。

#### 限制解除·拦截杀掉进程（待完善）

开启此功能后教师端将无法远程杀掉你的任何进程，请注意您的生命安全。

#### 限制解除·解工具/限制

这会解除老师在组策略中设置的禁用任务管理器、运行等等功能限制。

#### 限制解除·重启资源管理器

这个都不知道的话，还是好好上信息技术课吧...

#### 密码获取

首页底部左侧的文本框显示获取到的密码，如果获取失败，会显示为极域超级密码 `mythware_super_password`

#### 卸载极域

这个功能会自动调用极域卸载器，输入密码卸载极域。

## 设置

#### 常规·摸鱼网站

这会打开洛谷的一个小游戏合集云剪切板。

#### 常规·摸鱼游戏（待完善）

一个简易的贪吃蛇小游戏。

#### 常规·扩展路径设置

请到“关于”窗口下载（密码：XDFZ）扩展包后解压文件夹，并点击小图标浏览或输入扩展文件夹地址（默认：C:/Users/Administrator/AppData/Local/Temp/CyberpunkAddon）。

#### UI·随机标题

开启后，标题栏的文字会随机显示。

#### UI·隐藏托盘图标

这会隐藏软件的托盘图标，你依然可以通过快捷键来显示窗口。

#### UI·性能模式

这会禁用程序的样式表，恢复为系统默认样式。建议在出现样式错乱、性能问题时开启，注意需要重启才能应用。

#### UI·主题

MessAround内置了两款主题！！Cyberpunk主题请下载对应扩展包。

#### UI·窗口不透明度

此拖动条会改变窗口的不透明度，推荐值：60~80.
