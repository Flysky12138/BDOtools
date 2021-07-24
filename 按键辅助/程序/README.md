<h2 align="center">运行</h2>

> ### `a.exe`
>
> 快捷键 `` ALT+` `` 获取鼠标坐标或该点颜色并复制到剪切板

> ### `a.exe a.txt`
>
> 运行脚本 `a.txt`

<br>

<h2 align="center">脚本说明</h2>

1. 脚本首行

```
BDO <run_times> <runEnd_wait(s)> [key]
BDO 3 1 End
#key：停止热键
```

2. 打开 / 关闭游戏

```
OPEN [wait(ms)]
OPEN 800
#正数：HWND_TOPMOST => 将窗口置于最上方
#负数：HWND_BOTTOM  => 将窗口置于最下方

CLOSE
```

3. 按键

```
K <key> [wait(ms)]
K Q 800

K2 <key> <key> <keyUp_delay(ms)> [wait(ms)]
K2 ALT Q 50 800
```

4. 鼠标左击

```
L <keyUp_delay(ms)> [wait(ms)]
L 50 800

L <x,y>/<color(r,g,b)(r,g,b)> [wait(ms)]
L 0,0 800
```

```
<x,y>           =>  0,0 / x,y
<color(r,g,b)(r,g,b)>  =>  color(255,255,255)(0,0,0)
```

5. 鼠标右击

```
R <keyUp_delay(ms)> [wait(ms)]
R 50 800

R <x,y>/<color(r,g,b)(r,g,b)> [wait(ms)]
R 0,0 800
```

6. 鼠标滚轮

```
W <x,y>/<color(r,g,b)(r,g,b)> <downMove_times> [wait(ms)]
W 0,0 3 800
```

7. 鼠标左击拖动再左击

```
M <x,y>/<color(r,g,b)(r,g,b)> <x,y>/<color(r,g,b)(r,g,b)> [wait(ms)]
M 0,0 100,100 800
```

8. 等待时间

```
S <wait(ms)>
S 500
```

9. 启动热键

```
KD <key>
KD /+
#按键按下执行

KU <key>
KU /+
#按键抬起执行
```

10. 粘贴内容

```
CV <"text"> [wait(ms)]
CV "AAAA" 800
```

11. 等待找到坐标后才继续执行

```
WHILE <color(r,g,b)(r,g,b)>
WHILE color(144,142,144)(132,128,129)
```

-   `run_times`：运行次数
-   `runEnd_wait(s)`：单次脚本运行完后的等待时间，当倒计时到达 `3` 秒内且游戏最小化时将弹出窗口，`0` 秒时再隐藏
-   `wait(ms)`：运行单行命令后的等待时间
-   `key`：可填按键 `0-9` `A-Z` `F1-F10` `Esc` `Tab` `Enter` `Ctrl` `LShift` `RShift` `Alt` `Space` `Scroll` `End` `Down` `PgDn` `Insert` `Delete` `PrtSc` `Caps` `Num` `Home` `Up` `PgUp` `Bs` `Left` `Center` `Right` `-` `=` `[` `]` `;` `'` `` ` `` `\` `,` `.` `/` `/-` `/+`
-   `keyUp_delay(ms)`：按键按下到抬起之间的时间
-   `x,y`：可以填写坐标如 `100,200`，也可以直接填写 `x,y`。当填写 `x,y` 时，程序第一次运行到这行代码时，你需要按快捷键 `` ALT+` `` 获取坐标，之后的运行就自动使用这次得到的坐标
-   `color(r,g,b)(r,g,b)`：获取屏幕中第一个满足条件的颜色的坐标（两点是 3 像素矩形左上与右下的顶点，匹配成功后取左上角一点为匹配到的坐标）
-   `downMove_times`：向下移动，以一个背包格子为单位

> 单行命令最后为 `0` 时可不填写  
> 所有功能均需游戏没最小化才会执行，否则直接跳过  
> 非匹配功能不会执行（例：`#K` 用以注释该命令）

<br>

<h2 align="center">判断语法</h2>

```
IF color(r,g,b)(r,g,b)
···
FI
```

```
IF color(93,67,121)(93,68,120)
    K I 100
    IF color(93,67,121)(93,68,120)
        K I 100
    FI
FI
```

<br>

<h2 align="center">循环语法</h2>

```
FOR <run_times>
···
END
```

```
FOR 180
    K R 800
    FOR 180
        K R 800
        K Space 40
        K2 Alt Q 50 800
    END
    S 800
    K R
END
```

<br>

<h2 align="center">脚本实列</h2>

```
BDO 1000 160 End
OPEN -800
K 1 3000
FOR 80
    K R 500
END
K Tab 800
CLOSE
```

<br>

<h2 align="center">依赖</h2>

```
dumpbin.exe /dependents BDO.exe

KERNEL32.dll
USER32.dll
GDI32.dll
SHELL32.dll
gdiplus.dll
MSVCP140.dll
SHLWAPI.dll
VCRUNTIME140_1.dll
VCRUNTIME140.dll
api-ms-win-crt-runtime-l1-1-0.dll
api-ms-win-crt-string-l1-1-0.dll
api-ms-win-crt-stdio-l1-1-0.dll
api-ms-win-crt-heap-l1-1-0.dll
api-ms-win-crt-utility-l1-1-0.dll
api-ms-win-crt-filesystem-l1-1-0.dll
api-ms-win-crt-convert-l1-1-0.dll
api-ms-win-crt-math-l1-1-0.dll
api-ms-win-crt-locale-l1-1-0.dll
```
