Linux 
<a name="W0RwF"></a>
### 谨慎删除文件
如果要谨慎使用 rm 命令，可以为它设置一个别名，在删除文件之前需要进行确认才能删除。有些系统管理员会默认使用这个别名，对于这种情况，可能需要看看下一个技巧。
```bash
$ rm -i    <== 请求确认
```
<a name="dy0JL"></a>
### 关闭别名
可以使用 `unalias` 命令以交互方式禁用别名。它不会更改别名的配置，而仅仅是暂时禁用，直到下次登录或重新设置了这一个别名才会重新生效。
```bash
$ unalias rm
```
如果已经将 `rm -i` 默认设置为 `rm` 的别名，希望在删除文件之前不必进行确认，则可以将 `unalias` 命令放在一个启动文件（例如 ~/.bashrc）中。
<a name="NG2uK"></a>
### 使用 sudo
如果经常在只有 root 用户才能执行的命令前忘记使用 sudo，这里有两个方法可以解决。一是利用命令历史记录，可以使用 `sudo !!`（使用 `!!` 来运行最近的命令，并在前面添加 sudo）来重复执行，二是设置一些附加了所需 sudo 的命令别名。
```bash
$ alias update='sudo apt update'
```
<a name="T2rSx"></a>
### 更复杂的技巧
有时命令行技巧并不仅仅是一个别名。毕竟，别名能做的只有替换命令以及增加一些命令参数，节省了输入的时间。但如果需要比别名更复杂功能，可以通过编写脚本、向 `.bashrc` 或其他启动文件添加函数来实现。例如，下面这个函数会在创建一个目录后进入到这个目录下。在设置完毕后，执行 `source .bashrc`，就可以使用 md temp 这样的命令来创建目录立即进入这个目录下。
```bash
md () { mkdir -p "$@" && cd "$1"; }
```
<a name="FDOny"></a>
### 命令编辑及光标移动
这里有很多快捷键可以修正自己的命令。接下来使用光标二字代替光标的位置。<br />删除从开头到光标处的命令文本<br />ctrl + u，例如：
```bash
$ cd /proc/tty;ls -al光标
```
如果此时使用ctrl+u快捷键，那么该条命令都会被清除，而不需要长按backspace键。<br />删除从光标到结尾处的命令文本<br />ctrl+k，例如：
```bash
$ cd /proc/tty光标;ls -al
```
如果此时使用ctrl + k快捷键，那么从光标开始处到结尾的命令文本将会被删除。<br />还有其他的操作，不再举例，例如：<br />ctrl + a:光标移动到命令开头<br />ctrl + e：光标移动到命令结尾<br />alt  f:光标向前移动一个单词<br />alt  b：光标向前移动一个单词<br />ctrl w：删除一个词（以空格隔开的字符串）
<a name="NlWAn"></a>
### 历史命令快速执行
history记录了执行的历史命令，而使用`!＋历史命令前的数字`，可快速执行历史命令。<br />部分历史命令查看<br />history会显示大量的历史命令，而`fs -l`只会显示部分。
<a name="IgVPN"></a>
### 实时查看日志
```bash
$ tail -f filename.log
```
`tail -f` 加文件名，可以实时显示日志文件内容。当然，使用`less`命令查看文件内容，并且使用shift+f键，也可达到类似的效果。
<a name="govId"></a>
### 磁盘或内存情况查看
怎么知道当前磁盘是否满了呢？
```bash
$ df -h
/dev/sda14      4.6G   10M  4.4G   1% /tmp
/dev/sda11      454M  366M   61M  86% /boot
/dev/sda15       55G   18G   35G  35% /home
/dev/sda1       256M   31M  226M  12% /boot/efi
tmpfs           786M   64K  786M   1% /run/user/10
```
使用`df`命令可以快速查看各挂载路径磁盘占用情况。<br />当前目录各个子目录占用空间大小<br />如果已经知道home目录占用空间较大了，想知道home目录下各个目录占用情况：<br />这里指定了目录深度，否则的话，它会递归统计子目录占用空间大小，可自行尝试。
<a name="Nd5fA"></a>
### 当前内存使用情况
```bash
$ free -h
              total        used        free      shared  buff/cache   available
Mem:           7.7G        3.5G        452M        345M        3.7G        3.5G
```
通过`free`的结果，很容易看到当前总共内存多少，剩余可用内存多少等等。
<a name="vvi5w"></a>
### 使用`-h`参数
是否注意到，在前面几个命令中，都使用了`-h`参数，它的作用是使得结果以人类可读的方式呈现，所以看到它呈现的单位是G,M等，如果不使用-h参数，可以自己尝试一下会是什么样的结果呈现。<br />根据名称查找进程id<br />想快速直接查找进程id，可以使用：
```bash
$ pgrep hello
22692
```
或者：
```bash
$ pidof hello
22692
```
其中，hello是进程名称。<br />根据名称杀死进程<br />一般可以使用`kill -9 pid`方式杀死一个进程，但是这样就需要先找到这个进程的进程id，实际上也可以直接根据名称杀死进程，例如：
```bash
$ killall hello
```
或者：
```bash
$ pkill hello
```
<a name="xm41t"></a>
### 查看进程运行时间
可以使用下面的命令查看进程已运行时间：
```bash
$ ps -p 24525 -o lstart,etime 
                 STARTED     ELAPSED
Sat Mar 23 20:52:08 2019       02:45
```
其中24525是要查看进程的进程id。<br />快速目录切换<br />`cd -` 回到上一个目录<br />`cd`  回到用户家目录
<a name="kETaD"></a>
### 多条命令执行
使用分号隔开可以执行多条命令，例如：
```bash
$ cd /temp/log/;rm -rf *
```
但是如果当前目录是/目录，并且/temp/log目录不存在，那么就会发生激动人心的一幕：
```bash
bash: cd: /temp/log: No such file or directory
```
（突然陷入沉默）<br />因为`;`可以执行多条命令，但是不会因为前一条命令失败，而导致后面的不会执行，因此，cd执行失败后，仍然会继续执行`rm -rf *`，由于处于/目录下，结果可想而知。<br />所以你还以为这种事故是对`rm -rf *`的力量一无所知的情况下产生的吗？<br />如果解决呢？很简单，使用&&，例如:
```bash
$ cd /temp/log/&&rm -rf *
```
这样就会确保前一条命令执行成功，才会执行后面一条。
<a name="HZgxO"></a>
### 查看压缩日志文件
有时候日志文件是压缩的，那么能不能偷懒一下，不解压查看呢？当然可以啦。<br />例如：
```bash
$ zcat test.gz
test log
```
或者：
```bash
$ zless test.gz
test log
```
<a name="jBUEL"></a>
### 清空文件内容
比如有一个大文件，想快速删除，或者不想删除，但是想清空内容：
```bash
>filename
```
将日志同时记录文件并打印到控制台<br />在执行shell脚本，常常会将日志重定向，但是这样的话，控制台就没有打印了，如何使得既能记录日志文件，又能将日志输出到控制台呢？
```bash
$ ./test.sh |tee test.log
```
终止并恢复进程执行<br />使用ctrl+z 暂停一个进程的执行，也可以使用`fg`恢复执行。例如使用
```bash
$ cat filename
```
当发现文件内容可能很多时，使用ctrl+z暂停程序，而如果又想要从刚才的地方继续执行，则只需要使用`fg`命令即可恢复执行。或者使用`bg`使得进程继续在后台执行。
<a name="cGBwQ"></a>
### 计算程序运行时间
可能会进程写一些小程序，并且想要知道它的运行时间，实际上可以很好的利用time命令计算，例如：
```bash
$ time ./fibo 30
the 30 result is 832040
real    0m0.088s
user    0m0.084s
sys    0m0.004s
```
它会显示系统时间，用户时间以及实际使用的总时间。<br />查看内存占用前10的进程
```bash
$ ps -aux|sort -k4nr |head -n 10
```
<a name="TbJia"></a>
### 快速查找需要的命令
man可以查看命令的帮助手册，但是如果想要某个功能却不知道使用哪个命令呢？别着急，还是可以使用man：
```bash
$ man -k "copy files"
cp (1)               - copy files and directories
cpio (1)             - copy files to and from archives
git-checkout-index (1) - Copy files from the index to the working tree
gvfs-copy (1)        - Copy files
gvfs-move (1)        - Copy files
install (1)          - copy files and set attributes
```
使用-k参数，使得与copy files相关的帮助手册都显示出来了。
<a name="wqA3z"></a>
### 命令行下的复制粘贴
在命令行下，复制不能再是ctrl + c了，因为它表示终止当前进程，而控制台下的复制粘贴需要使用下面的快捷键：
```bash
ctrl +  insert
shift + insert
```
<a name="XUAPD"></a>
### 搜索包含某个字符串的文件
例如，要在当前目录下查找包含test字符串的文件：
```bash
$ grep -rn "test"
test2.txt:1:test
```
它便可以找到该字符串在哪个文件的第几行。
<a name="xCQyM"></a>
### 屏幕冻结
程序运行时，终端可能输出大量的日志，想简单查看一下，又不想记录日志文件，此时可以使用ctrl+s键，冻结屏幕，使得日志不再继续输出，而如果想要恢复，可使用ctrl+q退出冻结。
<a name="NFXEj"></a>
### 无编辑器情况下编辑文本文件
如果在某些系统上连基本的vi编辑器都没有，那么可以使用下面的方式进行编辑内容：
```bash
$ cat >file.txt
some words
(ctrl+d)
```
编辑完成后，ctrl+d即可保存。
<a name="Fy31l"></a>
### 查看elf文件
查看elf文件头信息<br />例如：
```bash
$ readelf -h filename
```
在显示结果中，可以看到运行的平台，elf文件类型，大小端情况等。<br />查看库中是否包含某个接口
```bash
$ nm filename |grep interface
```
这里是从文件filename中查看是否包含interface接口，前提是该文件包含符号表。
<a name="pE2Ju"></a>
### 命令编辑
如果要对一个已输入的命令进行修改，可以使用 ^a（ctrl + a）或 ^e（ctrl + e）将光标快速移动到命令的开头或命令的末尾。<br />还可以使用 ^ 字符实现对上一个命令的文本替换并重新执行命令，例如 ^before^after^ 相当于把上一个命令中的 before 替换为 after 然后重新执行一次。
```bash
$ eho hello world  <== 错误的命令
Command 'eho' not found, did you mean:
command 'echo' from deb coreutils
command 'who' from deb coreutils
Try: sudo apt install <deb name>
$ ^e^ec^        <== 替换
echo hello world
hello world
```
<a name="YVQyb"></a>
### 使用远程机器的名称登录到机器上
如果使用命令行登录其它机器上，可以考虑添加别名。在别名中，可以填入需要登录的用户名（与本地系统上的用户名可能相同，也可能不同）以及远程机器的登录信息。例如使用 `server_name ='ssh -v -l username IP-address'` 这样的别名命令：
```bash
$ alias butterfly="ssh -v -l jdoe 192.168.0.11"
```
也可以通过在 /etc/hosts 文件中添加记录或者在 DNS 服务器中加入解析记录来把 IP 地址替换成易记的机器名称。<br />执行 alias 命令可以列出机器上已有的别名。
```bash
$ alias
alias butterfly='ssh -v -l jdoe 192.168.0.11'
alias c='clear'
alias egrep='egrep --color=auto'
alias fgrep='fgrep --color=auto'
alias grep='grep --color=auto'
alias l='ls -CF'
alias la='ls -A'
alias list_repos='grep ^[^#] /etc/apt/sources.list /etc/apt/sources.list.d/*'
alias ll='ls -alF'
alias ls='ls --color=auto'
alias show_dimensions='xdpyinfo | grep '''dimensions:''''
```
只要将新的别名添加到 ~/.bashrc 或类似的文件中，就可以让别名在每次登录后都能立即生效。
<a name="WbsHJ"></a>
### 冻结、解冻终端界面
^s（ctrl + s）将通过执行流量控制命令 XOFF 来停止终端输出内容，这会对 PuTTY 会话和桌面终端窗口产生影响。如果误输入了这个命令，可以使用 ^q（ctrl + q）让终端重新响应。所以只需要记住 ^q 这个组合键就可以了，毕竟这种情况并不多见。
<a name="mWKy5"></a>
### 复用命令
Linux 提供了很多让用户复用命令的方法，其核心是通过历史缓冲区收集执行过的命令。复用命令的最简单方法是输入 ! 然后接最近使用过的命令的开头字母；当然也可以按键盘上的向上箭头，直到看到要复用的命令，然后按回车键。还可以先使用 history 显示命令历史，然后输入 ! 后面再接命令历史记录中需要复用的命令旁边的数字。
```bash
!! <== 复用上一条命令
!ec <== 复用上一条以 “ec” 开头的命令
!76 <== 复用命令历史中的 76 号命令
```
<a name="xPX4d"></a>
### 查看日志文件并动态显示更新内容
使用形如 tail -f /var/log/syslog 的命令可以查看指定的日志文件，并动态显示文件中增加的内容，需要监控向日志文件中追加内容的的事件时相当有用。这个命令会输出文件内容的末尾部分，并逐渐显示新增的内容。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623596004180-a5f9c88a-1e33-4706-ae3d-59a055f10a71.webp#clientId=ufeeb9c41-07a2-4&from=paste&id=ua4d4ab88&originHeight=235&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=uefa465c2-c13a-45b2-b69f-3a45d7978ef)
<a name="Wz2ML"></a>
### 寻求帮助
对于大多数 Linux 命令，都可以通过在输入命令后加上选项 `--help` 来获得这个命令的作用、用法以及它的一些相关信息。除了 `man` 命令之外， `--help` 选项可以在不使用所有扩展选项的情况下获取到所需要的内容。<br />![](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1623596004056-88e6f63a-feef-44e0-938b-1ebc72793cea.jpeg#clientId=ufeeb9c41-07a2-4&from=paste&id=u79416f68&originHeight=506&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=ueeb032ca-14c9-400d-b343-31463b4f1bf)
