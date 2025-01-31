Linux Vim 
<a name="i0uVi"></a>
### Vim教程
```bash
vimtutor// vim 教程
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1602829478456-707f5c11-fa5e-4951-babc-802e2dcba34b.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2162834&status=done&style=none&width=1107.6666666666667)
<a name="3UF5X"></a>
### Vim的工作模式
基本上 vi/vim 共分为三种模式，分别是命令模式（Command mode），输入模式（Insert mode）和底线命令模式（Last line mode）。这三种模式的作用分别是：
<a name="pvKwb"></a>
#### 命令模式
当使用 vi/vim 打开一个文件就进入了命令模式（也可称为一般模式），这是默认的模式。在这个模式，你可以采用『上下左右』按键来移动光标，你可以使用『删除字符』或『删除整行』来处理档案内容，也可以使用『复制、贴上』来处理你的文件数据。
<a name="vealv"></a>
#### 输入模式
在命令模式并不能编辑文件，需要输入如『i, I, o, O, a, A, r,R』等任何一个字母之后才会进入输入模式（也称为编辑模式）。通常在 Linux 中，按下这些按键时，在画面的左下方会出现『 INSERT 或 REPLACE 』的字样，此时才可以进行编辑。而如果要回到命令模式时，则必须要按下『Esc』这个按键即可退出编辑模式。
<a name="SfZEm"></a>
#### 底线命令模式
在命令模式下，按下『:,/,?』中任意一个，就可以将光标移动到最底下那一行，进入底线命令模式（也称为指令列命令模式）。在这个模式当中， 可以提供『搜寻资料』的动作，而读取、存盘、大量取代字符、退出、显示行号等等的动作则是在此模式中达成的！同理，必须按下『Esc』按钮才可以退出该模式，返回命令模式
<a name="bq3FK"></a>
### Vim的快捷键
<a name="DEASC"></a>
#### 移动光标

- 上、下、左、右：方向键或者k、j、h、l
- 跳到文件头部：gg
- 跳到文件尾部：G
- 跳到某行行尾：$
- 跳转到某行开头：行号+G（比如跳到200行开头->200G）
- 跳转到某行末尾：行号+$（比如跳到200行末尾->200$）
- 向下移动n行：行号+回车
- 2w 向前移动两个单词
- 3e 向前移动到第 3 个单词的末尾
- 0 移动到行首
- $ 当前行的末尾
- gg 文件第一行
- G 文件最后一行
- 行号+G 指定行
- <ctrl>+o 跳转回之前的位置
- <ctrl>+i 返回跳转之前的位置
| h 或 向左方向键(←) | 光标向左移动一个字符 |
| --- | --- |
| j 或 向下方向键(↓) | 光标向下移动一个字符 |
| k 或 向上方向键(↑) | 光标向上移动一个字符 |
| l 或 向右方向键(→) | 光标向右移动一个字符 |
| 如果将右手放在键盘上的话，会发现 hjkl 是排列在一起的，因此可以使用这四个按钮来移动光标。如果想要进行多次移动的话，例如向下移动 30 行，可以使用 "30j"或 "30↓" 的组合按键， 亦即加上想要进行的次数(数字)后，按下动作即可！ |  |
| 
| [Ctrl] + [f] | 屏幕『向下』移动一页，相当于 [Page Down]按键 |
| [Ctrl] + [b] | 屏幕『向上』移动一页，相当于 [Page Up]按键 |
| [Ctrl] + [d] | 向下滚动（移动半页） |
| [Ctrl] + [u] | 向上滚动（移动半页） |
| + | 光标移动到非空格符的下一行 |
| - | 光标移动到非空格符的上一行 |
| n | 向右移动 n 个字符，n 是数量 |
| 0 或功能键[Home] | 数字0，移动到当前行最前面字符处 |
| $ 或功能键[End] | 移动到这一行的最后字符处 |
| H | 光标移动到这个屏幕最上方一行的第一个字符处 |
| M | 光标移动到这个屏幕中央一行的第一个字符处 |
| L | 光标移动到这个屏幕最下方一行的第一个字符处 |
| G | 移动到这个档案的最后一行 |
| nG | 移动到这个档案的第 n 行，n是数字（可配合 :set nu) |
| gg | 移动到这个档案的第一行，效果等同于1G  |
| n+【Enter键】 | 向下移动 n 行 |

<a name="lSrWz"></a>
#### 搜索查找
/ 正向查找（n：继续查找，N：相反方向继续查找）<br />？ 逆向查找<br />% 查找配对的 {，[，(<br />输入/或?，然后输入要查找的内容，如：/hello，如果搜到多个：n下一个，N上一个<br />搜索当前光标所在的单词：#

| /word | 向光标之下寻找一个名称为 word 的字符串。例如要在档案内搜寻 vbird 这个字符串，就输入 /vbird 即可！ |
| --- | --- |
| ?word | 向光标之上寻找一个字符串名称为 word 的字符串。 |
| n | 重复前一个搜寻的动作。 |
| N | 与 n 刚好相反，为『反向』进行前一个搜寻动作。 |
| 使用 /word 配合 n 及 N 是非常有帮助的！可以重复的找到一些搜寻的关键词！ |  |

<a name="GYe1r"></a>
#### 替换
替换当前字符（单个）：输入r，然后在输入要替换的字符<br />替换当前字符（多个）：输入R，然后在输入要替换的字符（往后替换）

- s/old/new：如：s/exit/quit，将exit替换为quit（只替换当前行第一处）
- s/old/new/g：文本替换（替换当前行全局）
- 10-20s/old/new/g：范围行内替换
- %s/old/new/g：当前文件内进行替换
| :n1,n2s/word1/word2/g | 在第 n1 与 n2 行之间寻找 word1 这个字符串，并将该字符串取代为 word2 ！ |
| --- | --- |
| :1,$s/word1/word2/g | 从第一行到最后一行寻找 word1 字符串，并将该字符串取代为 word2 |
| :1,$s/word1/word2/gc | 从第一行到最后一行寻找 word1 字符串，并将该字符串取代为 word2 ！且在取代前显示提示字符给用户确认 (confirm) 是否需要取代 |

<a name="rAiah"></a>
#### 删除/剪切
删除光标后面的字符：x，删除光标后面的多个字符：nx<br />删除光标前面的字符：X<br />删除一个单词：dw（注意保证光标在单词的最前面，不然只能删除光标后面的部分）<br />删除至当前单词末尾，包括当前字符 de<br />删除当前行光标前面部分：d0<br />删除当前行光标后面部分：D或者d$<br />删除当前行（整行）：dd<br />删除多行：ndd（如10dd，即从当前位置起，往下删除10行（包括当前行））<br />删除当前位置后面的所有内容：dG（包括当前行）<br />删除当前位置前面的所有内容：dgg（包括当前行）

| x, X | 在一行字当中，x 为向后删除一个字符 (相当于 [del] 按键)， X 为向前删除一个字符(相当于 [backspace] 亦即是退格键) |
| --- | --- |
| nx | 连续向后删除 n 个字符。 |
| dw | 删除一个单词（注意保证光标在单词的最前面，不然只能删除光标后面的部分） |
| dd | 删除当前行（整行） |
| ndd | 删除光标所在的向下 n 行 |
| dgg | 删除当前位置前面的所有内容（包括当前行） |
| d1G | 删除光标所在到第一行的所有数据 |
| dG | 删除当前位置后面的所有内容（包括当前行） |
| d$/D | 删除当前行光标后面部分 |
| d0 | 删除当前行光标前面部分 |

:::info
温馨提示：vim中的删除其实是剪切操作，删除的内容可以用p命令粘贴
:::
<a name="75HBS"></a>
#### 复制
复制当前行：yy<br />粘贴复制内容：p（当前行下一行）；P（当前行上一行）<br />复制多行：nyy（如3yy，即从当前位置起，往下复制3行（包括当前行））<br />复制选中：按v进入可视模式，选择要复制的内容，按一个y即可复制完成

| y1G | 复制游标所在行到第一行的所有数据 |
| --- | --- |
| yG | 复制游标所在行到最后一行的所有数据 |
| y0 | 复制光标所在的那个字符到该行行首的所有数据 |
| y$ | 复制光标所在的那个字符到该行行尾的所有数据 |

<a name="XNTAN"></a>
#### 粘贴
| p, P | p 为将已复制的数据在光标下一行贴上，P 则为贴在游标上一行！ |
| --- | --- |
| J | 将光标所在行与下一行的数据结合成同一行 |

<a name="HMPoz"></a>
#### 其他-【撤销、重复操作】

- 撤销：u
- 反撤销：ctrl+r
| c | 重复删除多个数据，任意方向，并且进入编辑模式 |
| --- | --- |
| u | 复原前一个动作。(常用-撤销) |
| [Ctrl]+r | 重做上一个动作。(常用-反撤销) |
| 这个 u 与 [Ctrl]+r 是很常用的指令！一个是复原，另一个则是重做一次 |  |
| . | 重复前一个动作，比如重复删除、重复贴上等等动作，按下小数点『.』 |

<a name="7ndml"></a>
#### 编辑模式
a：从光标后面开始插入<br />A：会跳到行尾并从光标后面开始插入<br />i：从光标前面开始插入<br />I：会跳到行首并从光标前面开始插入<br />o：会在光标下一行新建空白行插入<br />O：会在光标上一行新建空白行插入<br />s：删除光标盖住的字符开始插入<br />S：删除光标所在行后开始插入

| i, I | 进入输入模式(Insert mode)：i 为『从目前光标所在处输入』， I 为『在目前所在行的第一个非空格符处开始输入』 |
| --- | --- |
| a, A | 进入输入模式(Insert mode)：a 为『从目前光标所在的下一个字符处开始输入』， A 为『从光标所在行的最后一个字符处开始输入』 |
| o, O | 进入输入模式(Insert mode)：<br />这是英文字母 o 的大小写。o 为『在目前光标所在的下一行处输入新的一行』；O 为在目前光标所在处的上一行输入新的一行 |
| r, R | 进入取代模式(Replace mode)：<br />r 只会取代光标所在的那一个字符一次；R会一直取代光标所在的文字，直到按下 ESC 为止 |
| 上面这些按键中，在 vi 画面的左下角处会出现『--INSERT--』或『--REPLACE--』的字样。想要在文件里面输入字符时， 一定要在左下角处看到 INSERT 或 REPLACE 才能输入 |  |
| [Esc] | 退出编辑模式，回到一般模式中 |

<a name="gSsv0"></a>
#### 底线命令模式
| :w | 将编辑的数据写入硬盘档案中 |
| --- | --- |
| :w! | 若文件属性为『只读』时，强制写入该档案。不过，到底能不能写入， 还是和对该文件的操作权限有关 |
| :q | 离开 |
| :q! | 若曾修改过档案，又不想储存，使用 ! 为强制离开不储存档案。 |
| 注意一下，那个惊叹号 (!) 在 vi/vim 当中，常常具有『强制』的意思～ |  |
| :wq/:x | 储存后离开，若为 :wq! 则为强制储存后离开 |
| ZZ | 这是大写的 Z ！若档案没有更动，则不储存离开，若档案已经被更动过，则储存后离开！ |
| :w [filename] | 将编辑的数据储存成另一个档案（类似另存新档） |
| :r [filename] | 在编辑的数据中，读入另一个档案的数据。亦即将 『filename』 这个档案内容加到游标所在行后面 |
| :n1,n2 w [filename] | 将 n1 到 n2 的内容储存成 filename 这个档案。 |
| :! command | 暂时离开 vi 到指令行模式下执行 command 的显示结果！例如『:! ls /home』即可在 vi/vim 当中察看 /home 底下以 ls 输出的档案信息！ |

<a name="scwxf"></a>
#### vim/vi 环境改变/状态修改
:set ic 忽略大小写<br />:set noic 取消忽略大小写<br />:set hls 匹配项高亮显示<br />:set is 显示部分匹配

| :set nu | 显示行号，设定之后，会在每一行的前缀显示该行的行号 |
| --- | --- |
| :set nonu | 与 set nu 相反，为取消行号！ |
| :set tabstop=n | 设置 Tab 键间隔的空格符数量 |

<a name="nZ0ZY"></a>
#### Vim分屏
sp：水平分屏<br />vsp：垂直分屏<br />ctrl+ww：多屏之间切换<br />qall：退出所有屏，以此内推有：wall、wqall<br />vim -o aa.txt bb.txt：进入的时候就进行分屏（o水平；O垂直）
<a name="vJe8S"></a>
#### 折叠
zc 折叠<br />zC 折叠所有嵌套<br />zo 展开折叠<br />zO 展开所有折叠嵌套
<a name="qpwRV"></a>
#### <ctrl>+g 显示当前行以及文件信息
<a name="fPvze"></a>
#### 代码格式化：gg=G（即连续按2个g，再按=，再按G）
<a name="CeUDj"></a>
#### 支持shell命令：!，如:!pwd
<a name="jq5Zo"></a>
### Vim基本配置
<a name="19856425"></a>
#### 取消备份
```bash
set nobackup
set noswapfile
```
<a name="saFBu"></a>
#### 文件编码
```bash
set encoding=utf-8
```
<a name="NIZ7t"></a>
#### 显示行号
```bash
set number
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1602830882238-105e81fc-911d-47d9-ad15-caf1eb33feef.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2187666&status=done&style=none&width=1107.6666666666667)
<a name="GhSxl"></a>
#### 取消换行
```bash
set nowrap
```
<a name="TaOEg"></a>
#### 显示光标当前位置
```bash
set ruler
```
<a name="YBkZv"></a>
#### 设置缩进
```bash
set cindent
set tabstop=2
set shiftwidth=2
```
<a name="uBlIt"></a>
#### 突出显示当前行
```bash
set cursorline
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1602830838094-66ef429d-4132-4b66-896f-7e534d72e271.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2177794&status=done&style=none&width=1107.6666666666667)
<a name="W55RY"></a>
#### 查找
```bash
set ic
set hls
set is
```
<a name="3rUQe"></a>
#### 左下角显示当前vim模式
```bash
set showmode
```
<a name="TRhqb"></a>
#### 代码折叠
```bash
#启动 vim 时关闭折叠代码
set nofoldenable
```
<a name="BVena"></a>
#### 主题
```bash
syntax enable
set background=dark
colorscheme solarized
```
<a name="lXydi"></a>
### 宏录制-（vim recording）
在非其他操作下，按下英文的q键进入宏录制模式
<a name="5JpM6"></a>
#### 操作步骤
第一步：在正常模式下（非insert模式、非visual模式）按下q键盘<br />第二步：选择a-z或0-9中任意一个作为缓冲器的名字，准备开始录制宏<br />第三步：正常的操作，此次所有的操作都会被记录在上一步中定义的缓冲器中<br />第四步：在非insert模式下输入q停止宏的录制<br />第五步：使用@ + 第二步中定义的缓冲器的名字即可使用。
<a name="SxNH6"></a>
#### 使用案例
把下面的文字
```
line1
line-2
line3-1
l4
```
变成如下的文字
```java
System.out.println(line1);
System.out.println(line1);
System.out.println(line-2);
System.out.println(line3-1);
System.out.println(L4);
```
观察可以发现他们的规律，在每行文字的开头添加“System.out.println("，结尾添加");"就变成下面的信息了。下面简单介绍一下如何使用recording来完成这样的操作。<br />首先把光标移动line1上，输入qt，准备开始录制，缓冲器的名字为t，<br />录制的动作为：shift + ^ 回到行首、按下i键进入insert模式、输入“System.out.println(”、按下esc键回到正常模式、shift + $ 回到行尾部、按下i键进入insert模式、输入“);”按下esc键回到正常模式，按下q停止录制。<br />然后把光标移动到下面一行的任意位置输入 @ + t 即可。<br />recording还可以和查询结合起来使用，例如想把一个文件中含有特定字符串的行注释，可以通过这样的宏来实现。<br />在正常模式下输入/search string + enter、shift + ^、i、#、esc、shift + $。<br />让定制的宏自动执行多次的方法是先输入一个数字，然后在输入@ + 缓冲器的名字。 例如 100@t，表示执行100次。
