Linux<br />Linux 命令是对 Linux 系统进行管理的命令。对于 Linux 系统来说，无论是中央处理器、内存、磁盘驱动器、键盘、鼠标，还是用户等都是文件， Linux 系统管理的命令是它正常运行的核心，与之前的 DOS 命令类似。<br />Linux 命令在系统中有两种类型：内置 Shell 命令和 Linux 命令。

| 命令 | 功能说明 |
| --- | --- |
| **线上查询及帮助命令 (2 个)** | <br /> |
| **man** | 查看命令帮助，命令的词典，更复杂的还有 info，但不常用。 |
| **help** | 查看 Linux 内置命令的帮助，比如 cd 命令。 |
| **文件和目录操作命令 (18 个)** | 

 |
| **ls** | 全拼 list，功能是列出目录的内容及其内容属性信息。 |
| **cd** | 全拼 change directory，功能是从当前工作目录切换到指定的工作目录。 |
| **cp** | 全拼 copy，其功能为复制文件或目录。 |
| **find** | 查找的意思，用于查找目录及目录下的文件。 |
| **mkdir** | 全拼 make directories，其功能是创建目录。 |
| **mv** | 全拼 move，其功能是移动或重命名文件。 |
| **pwd** | 全拼 print working directory，其功能是显示当前工作目录的绝对路径。 |
| **rename** | 用于重命名文件。 |
| **rm** | 全拼 remove，其功能是删除一个或多个文件或目录。 |
| **rmdir** | 全拼 remove empty directories，功能是删除空目录。 |
| **touch** | 创建新的空文件，改变已有文件的时间戳属性。 |
| **tree** | 功能是以树形结构显示目录下的内容。 |
| **basename** | 显示文件名或目录名。 |
| **dirname** | 显示文件或目录路径。 |
| **chattr** | 改变文件的扩展属性。 |
| **lsattr** | 查看文件扩展属性。 |
| **file** | 显示文件的类型。 |
| **md5sum** | 计算和校验文件的 MD5 值。 |
| **查看文件及内容处理命令（21 个）** | 

 |
| **cat** | 全拼 concatenate，功能是用于连接多个文件并且打印到屏幕输出或重定向到指定文件中。 |
| **tac** | tac 是 cat 的反向拼写，因此命令的功能为反向显示文件内容。 |
| **more** | 分页显示文件内容。 |
| **less** | 分页显示文件内容，more 命令的相反用法。 |
| **head** | 显示文件内容的头部。 |
| **tail** | 显示文件内容的尾部。 |
| **cut** | 将文件的每一行按指定分隔符分割并输出。 |
| **split** | 分割文件为不同的小片段。 |
| **paste** | 按行合并文件内容。 |
| **sort** | 对文件的文本内容排序。 |
| **uniq** | 去除重复行。oldboy |
| **wc** | 统计文件的行数、单词数或字节数。 |
| **iconv** | 转换文件的编码格式。 |
| **dos2unix** | 将 DOS 格式文件转换成 UNIX 格式。 |
| **diff** | 全拼 difference，比较文件的差异，常用于文本文件。 |
| **vimdiff** | 命令行可视化文件比较工具，常用于文本文件。 |
| **rev** | 反向输出文件内容。 |
| **grep/egrep** | 过滤字符串，三剑客老三。 |
| **join** | 按两个文件的相同字段合并。 |
| **tr** | 替换或删除字符。 |
| **vi/vim** | 命令行文本编辑器。 |
| **文件压缩及解压缩命令（4 个）** | 

 |
| **tar** | 打包压缩。oldboy |
| **unzip** | 解压文件。 |
| **gzip** | gzip 压缩工具。 |
| **zip** | 压缩工具。 |
| **信息显示命令（11 个）** | 

 |
| **uname** | 显示操作系统相关信息的命令。 |
| **hostname** | 显示或者设置当前系统的主机名。 |
| **dmesg** | 显示开机信息，用于诊断系统故障。 |
| **uptime** | 显示系统运行时间及负载。 |
| **stat** | 显示文件或文件系统的状态。 |
| **du** | 计算磁盘空间使用情况。 |
| **df** | 报告文件系统磁盘空间的使用情况。 |
| **top** | 实时显示系统资源使用情况。 |
| **free** | 查看系统内存。 |
| **date** | 显示与设置系统时间。 |
| **cal** | 查看日历等时间信息。 |
| **搜索文件命令（4 个）** | 

 |
| **which** | 查找二进制命令，按环境变量 PATH 路径查找。 |
| **find** | 从磁盘遍历查找文件或目录。 |
| **whereis** | 查找二进制命令，按环境变量 PATH 路径查找。 |
| **locate** | 从数据库 (/var/lib/mlocate/mlocate.db) 查找命令，使用 updatedb 更新库。 |
| **用户管理命令（10 个）** | 

 |
| **useradd** | 添加用户。 |
| **usermod** | 修改系统已经存在的用户属性。 |
| **userdel** | 删除用户。 |
| **groupadd** | 添加用户组。 |
| **passwd** | 修改用户密码。 |
| **chage** | 修改用户密码有效期限。 |
| **id** | 查看用户的 uid,gid 及归属的用户组。 |
| **su** | 切换用户身份。 |
| **visudo** | 编辑 / etc/sudoers 文件的专属命令。 |
| **sudo** | 以另外一个用户身份（默认 root 用户）执行事先在 sudoers 文件允许的命令。 |
| **基础网络操作命令（11 个）** | 

 |
| **telnet** | 使用 TELNET 协议远程登录。 |
| **ssh** | 使用 SSH 加密协议远程登录。 |
| **scp** | 全拼 secure copy，用于不同主机之间复制文件。 |
| **wget** | 命令行下载文件。 |
| **ping** | 测试主机之间网络的连通性。 |
| **route** | 显示和设置 linux 系统的路由表。 |
| **ifconfig** | 查看、配置、启用或禁用网络接口的命令。 |
| **ifup** | 启动网卡。 |
| **ifdown** | 关闭网卡。 |
| **netstat** | 查看网络状态。 |
| **ss** | 查看网络状态。 |
| **深入网络操作命令（9 个）** | 

 |
| **nmap** | 网络扫描命令。 |
| **lsof** | 全名 list open files，也就是列举系统中已经被打开的文件。 |
| **mail** | 发送和接收邮件。 |
| **mutt** | 邮件管理命令。 |
| **nslookup** | 交互式查询互联网 DNS 服务器的命令。 |
| **dig** | 查找 DNS 解析过程。 |
| **host** | 查询 DNS 的命令。 |
| **traceroute** | 追踪数据传输路由状况。 |
| **tcpdump** | 命令行的抓包工具。 |
| **有关磁盘与文件系统的命令（16 个）** | 

 |
| **mount** | 挂载文件系统。 |
| **umount** | 卸载文件系统。 |
| **fsck** | 检查并修复 Linux 文件系统。 |
| **dd** | 转换或复制文件。 |
| **dumpe2fs** | 导出 ext2/ext3/ext4 文件系统信息。 |
| **dump** | ext2/3/4 文件系统备份工具。 |
| **fdisk** | 磁盘分区命令，适用于 2TB 以下磁盘分区。 |
| **parted** | 磁盘分区命令，没有磁盘大小限制，常用于 2TB 以下磁盘分区。 |
| **mkfs** | 格式化创建 Linux 文件系统。 |
| **partprobe** | 更新内核的硬盘分区表信息。 |
| **e2fsck** | 检查 ext2/ext3/ext4 类型文件系统。 |
| **mkswap** | 创建 Linux 交换分区。 |
| **swapon** | 启用交换分区。 |
| **swapoff** | 关闭交换分区。 |
| **sync** | 将内存缓冲区内的数据写入磁盘。 |
| **resize2fs** | 调整 ext2/ext3/ext4 文件系统大小。 |
| **系统权限及用户授权相关命令（4 个）** | 

 |
| **chmod** | 改变文件或目录权限。 |
| **chown** | 改变文件或目录的属主和属组。 |
| **chgrp** | 更改文件用户组。 |
| **umask** | 显示或设置权限掩码。 |
| **查看系统用户登陆信息的命令（7 个）** | 

 |
| **whoami** | 显示当前有效的用户名称，相当于执行 id -un 命令。 |
| **who** | 显示目前登录系统的用户信息。 |
| **w** | 显示已经登陆系统的用户列表，并显示用户正在执行的指令。 |
| **last** | 显示登入系统的用户。 |
| **lastlog** | 显示系统中所有用户最近一次登录信息。 |
| **users** | 显示当前登录系统的所有用户的用户列表。 |
| **finger** | 查找并显示用户信息。 |
| **内置命令及其它（19 个）** | 

 |
| **echo** | 打印变量，或直接输出指定的字符串 |
| **printf** | 将结果格式化输出到标准输出。 |
| **rpm** | 管理 rpm 包的命令。 |
| **yum** | 自动化简单化地管理 rpm 包的命令。 |
| **watch** | 周期性的执行给定的命令，并将命令的输出以全屏方式显示。 |
| **alias** | 设置系统别名。 |
| **unalias** | 取消系统别名。 |
| **date** | 查看或设置系统时间。 |
| **clear** | 清除屏幕，简称清屏。 |
| **history** | 查看命令执行的历史纪录。 |
| **eject** | 弹出光驱。 |
| **time** | 计算命令执行时间。 |
| **nc** | 功能强大的网络工具。 |
| **xargs** | 将标准输入转换成命令行参数。 |
| **exec** | 调用并执行指令的命令。 |
| **export** | 设置或者显示环境变量。 |
| **unset** | 删除变量或函数。 |
| **type** | 用于判断另外一个命令是否是内置命令。 |
| **bc** | 命令行科学计算器 |
| **系统管理与性能监视命令 (9 个)** | 

 |
| **chkconfig** | 管理 Linux 系统开机启动项。 |
| **vmstat** | 虚拟内存统计。 |
| **mpstat** | 显示各个可用 CPU 的状态统计。 |
| **iostat** | 统计系统 IO。 |
| **sar** | 全面地获取系统的 CPU、运行队列、磁盘 I/O、分页（交换区）、内存、 CPU 中断和网络等性能数据。 |
| **ipcs** | 用于报告 Linux 中进程间通信设施的状态，显示的信息包括消息列表、共享内存和信号量的信息。 |
| **ipcrm** | 用来删除一个或更多的消息队列、信号量集或者共享内存标识。 |
| **strace** | 用于诊断、调试 Linux 用户空间跟踪器。我们用它来监控用户空间进程和内核的交互，比如系统调用、信号传递、进程状态变更等。 |
| **ltrace** | 命令会跟踪进程的库函数调用, 它会显现出哪个库函数被调用。 |
| **关机 / 重启 / 注销和查看系统信息的命令（6 个）** | 

 |
| **shutdown** | 关机。 |
| **halt** | 关机。 |
| **poweroff** | 关闭电源。 |
| **logout** | 退出当前登录的 Shell。 |
| **exit** | 退出当前登录的 Shell。 |
| **Ctrl+d** | 退出当前登录的 Shell 的快捷键。 |
| **进程管理相关命令（15 个）** | 

 |
| **bg** | 将一个在后台暂停的命令，变成继续执行 （在后台执行）。 |
| **fg** | 将后台中的命令调至前台继续运行。 |
| **jobs** | 查看当前有多少在后台运行的命令。 |
| **kill** | 终止进程。 |
| **killall** | 通过进程名终止进程。 |
| **pkill** | 通过进程名终止进程。 |
| **crontab** | 定时任务命令。 |
| **ps** | 显示进程的快照。 |
| **pstree** | 树形显示进程。 |
| **nice/renice** | 调整程序运行的优先级。 |
| **nohup** | 忽略挂起信号运行指定的命令。 |
| **pgrep** | 查找匹配条件的进程。 |
| **runlevel** | 查看系统当前运行级别。 |
| **init** | 切换运行级别。 |
| **service** | 启动、停止、重新启动和关闭系统服务，还可以显示所有系统服务的当前状态。 |

