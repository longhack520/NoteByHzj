Linux<br />当发现 Linux 服务器上的系统性能问题，在最开始的 1 分钟时间里，会查看哪些系统指标呢?<br />Netflix 在 AWS 上有着大规模的 EC2 集群，以及各种各样的性能分析和监控工具。比如使用 Atlas 来监控整个平台，用 Vector 实时分析 EC2 实例的性能。这些工具已经能够解决大部分的问题，但是有时候还是要登录进机器内部，用一些标准的 Linux 性能分析工具来定位问题。<br />在这篇文章里，Netflix 性能工程团队会介绍一些使用的标准的 Linux 命令行工具，在发现问题的前 60 秒内去分析和定位问题。<br />在这 60 秒内，可以使用**下面这 10 个命令行了解系统整体的运行情况**，以及当前运行的进程对资源的使用情况。<br />在这些指标里面，先关注和错误、以及和资源饱和率相关的指标，然后再看资源使用率。<br />相对来讲，**错误和资源饱和率比较容易理解**。<br />饱和的意思是指一个资源(CPU，内存，磁盘)上的负载超过了它能够处理的能力，这时候观察到的现象就是请求队列开始堆积，或者请求等待的时间变长。
```
uptime 
dmesg | tail 
vmstat 1 
mpstat -P ALL 1 
pidstat 1 
iostat -xz 1 
free -m 
sar -n DEV 1 
sar -n TCP,ETCP 1 
top
```
有些命令行依赖于 sysstat 包。通过这些命令行的使用，可以熟悉一下分析系统性能问题时常用的一套方法或者流程：USE 。这个方法主要从资源使用率(Utilization)、资源饱和度(Satuation)、错误(Error)，这三个方面对所有的资源进行分析(CPU，内存，磁盘等等)。在这个分析的过程中，也要时刻注意已经排除过的资源问题，以便缩小定位的范围，给下一步的定位提供更明确的方向。<br />下面对每个命令行做了一个说明，并且使用了在生产环境的数据作为例子。对这些命令行更详细的描述，请查看相应的帮助文档。
<a name="ekMCi"></a>
## uptime
```bash
$ uptime  
23:51:26 up 21:31, 1 user, load average: 30.02, 26.43, 19.02
```
这个命令能很快地检查系统平均负载，可以认为这个负载的值显示的是有多少任务在等待运行。在 Linux 系统里，这包含了想要或者正在使用 CPU 的任务，以及在 io 上被阻塞的任务。这个命令能使我们对系统的全局状态有一个大致的了解，但是依然需要使用其它工具获取更多的信息。<br />这三个值是系统计算的 1 分钟、5 分钟、15 分钟的指数加权的动态平均值，可以简单地认为就是这个时间段内的平均值。根据这三个值，可以了解系统负载随时间的变化。比如，假设现在系统出了问题，去查看这三个值，发现 1 分钟的负载值比 15 分钟的负载值要小很多，那么很有可能已经错过了系统出问题的时间点。<br />在上面这个例子里面，负载的平均值显示 1 分钟为 30，比 15 分钟的 19 相比增长较多。有很多原因会导致负载的增加，也许是 CPU 不够用了；vmstat 或者 mpstat 可以进一步确认问题在哪里。
<a name="DyJCG"></a>
## `dmesg | tail`
```bash
$ dmesg | tail 
[1880957.563150] perl invoked oom-killer: gfp_mask=0x280da, order=0, oom_score_adj=0 
[...] 
[1880957.563400] Out of memory: Kill process 18694 (perl) score 246 or sacrifice child 
[1880957.563408] Killed process 18694 (perl) total-vm:1972392kB, anon-rss:1953348kB, file-rss:0kB 
[2320864.954447] TCP: Possible SYN flooding on port 7001. Dropping request.  Check SNMP count
```
这个命令显示了最新的几条系统日志。这里主要找一下有没有一些系统错误会导致性能的问题。上面的例子包含了 oom-killer 以及 TCP 丢包。<br />不要略过这一步!dmesg 永远值得看一看。
<a name="b0399"></a>
## `vmstat 1`
```bash
$ vmstat 1 
procs ---------memory---------- ---swap-- -----io---- -system-- ------cpu----- 
r  b swpd   free   buff  cache   si   so    bi    bo   in   cs us sy id wa st 
34  0    0 200889792  73708 591828    0    0     0     5    6   10 96  1  3  0  0 
32  0    0 200889920  73708 591860    0    0     0   592 13284 4282 98  1  1  0  0 
32  0    0 200890112  73708 591860    0    0     0     0 9501 2154 99  1  0  0  0 
32  0    0 200889568  73712 591856    0    0     0    48 11900 2459 99  0  0  0  0 
32  0    0 200890208  73712 591860    0    0     0     0 15898 4840 98  1  1  0  0 
^C
```
vmstat 展示了虚拟内存、CPU 的一些情况。上面这个例子里命令行的 1 表示每隔 1 秒钟显示一次。在这个版本的 vmstat 里，第一行表示了这一次启动以来的各项指标，可以暂时忽略掉第一行。<br />需要查看的指标：
```
r：#处在 runnable 状态的任务，包括正在运行的任务和等待运行的任务。这个值比平均负载能更好地看出 CPU 是否饱和。这个值不包含等待 io 相关的任务。当 r 的值比当前 CPU 个数要大的时候，系统就处于饱和状态了。
free：#以 KB 计算的空闲内存大小。
si，so：#换入换出的内存页。如果这两个值非零，表示内存不够了。
us，sy，id，wa，st：#CPU 时间的各项指标(对所有 CPU 取均值)，分别表示：用户态时间，内核态时间，空闲时间，等待 io，偷取时间(在虚拟化环境下系统在其它租户上的开销)
```
把用户态 CPU 时间(us)和内核态 CPU 时间(sy)加起来，可以进一步确认 CPU 是否繁忙。等待 IO 的时间 (wa)高的话，表示磁盘是瓶颈；注意，这个也被包含在空闲时间里面(id)， CPU 这个时候也是空闲的，任务此时阻塞在磁盘 IO 上了。可以把等待 IO 的时间(wa)看做另一种形式的 CPU 空闲，**它可以告诉你 CPU 为什么是空闲的**。<br />系统处理 IO 的时候，肯定是会消耗内核态时间(sy)的。如果内核态时间较多的话，比如超过 20%，需要进一步分析，也许内核对 IO 的处理效率不高。<br />在上面这个例子里，CPU 时间大部分都消耗在了用户态，表明主要是应用层的代码在使用 CPU。CPU 利用率 (us + sy)也超过了 90%，这不一定是一个问题；可以通过 r 和 CPU 个数确定 CPU 的饱和度。
<a name="AfzP5"></a>
## `mpstat -P ALL 1`
```bash
$ mpstat -P ALL 1 
Linux 3.13.0-49-generic (titanclusters-xxxxx)  07/14/2015  _x86_64_ (32 CPU)

07:38:49 PM  CPU   %usr  %nice   %sys %iowait   %irq  %soft  %steal  %guest  %gnice  %idle 
07:38:50 PM  all  98.47   0.00   0.75    0.00   0.00   0.00    0.00    0.00    0.00   0.78 
07:38:50 PM    0  96.04   0.00   2.97    0.00   0.00   0.00    0.00    0.00    0.00   0.99 
07:38:50 PM    1  97.00   0.00   1.00    0.00   0.00   0.00    0.00    0.00    0.00   2.00 
07:38:50 PM    2  98.00   0.00   1.00    0.00   0.00   0.00    0.00    0.00    0.00   1.00 
07:38:50 PM    3  96.97   0.00   0.00    0.00   0.00   0.00    0.00    0.00    0.00   3.03 
[...]
```
这个命令把每个 CPU 的时间都打印出来，可以看看 CPU 对任务的处理是否均匀。比如，如果某一单个 CPU 使用率很高的话，说明这是一个单线程应用。
<a name="tm4RD"></a>
## `pidstat 1`
```bash
$ pidstat 1 
Linux 3.13.0-49-generic (titanclusters-xxxxx)  07/14/2015    _x86_64_    (32 CPU)

07:41:02 PM   UID       PID    %usr %system  %guest    %CPU   CPU  Command 
07:41:03 PM     0         9    0.00    0.94    0.00    0.94     1  rcuos/0 
07:41:03 PM     0      4214    5.66    5.66    0.00   11.32    15  mesos-slave 
07:41:03 PM     0      4354    0.94    0.94    0.00    1.89     8  java 
07:41:03 PM     0      6521 1596.23    1.89    0.00 1598.11    27  java 
07:41:03 PM     0      6564 1571.70    7.55    0.00 1579.25    28  java 
07:41:03 PM 60004     60154    0.94    4.72    0.00    5.66     9  pidstat

07:41:03 PM   UID       PID    %usr %system  %guest    %CPU   CPU  Command 
07:41:04 PM     0      4214    6.00    2.00    0.00    8.00    15  mesos-slave 
07:41:04 PM     0      6521 1590.00    1.00    0.00 1591.00    27  java 
07:41:04 PM     0      6564 1573.00   10.00    0.00 1583.00    28  java 
07:41:04 PM   108      6718    1.00    0.00    0.00    1.00     0  snmp-pass 
07:41:04 PM 60004     60154    1.00    4.00    0.00    5.00     9  pidstat 
^C
```
pidstat 和 top 很像，不同的是它可以每隔一个间隔打印一次，而不是像 top 那样每次都清屏。这个命令可以方便地查看进程可能存在的行为模式，也可以直接 copy past，可以方便地记录随着时间的变化，各个进程运行状况的变化。<br />上面的例子说明有 2 个 Java 进程消耗了大量 CPU。这里的 %CPU 表明的是对所有 CPU 的值，比如 1591% 标识这个 Java 进程几乎消耗了 16 个 CPU。
<a name="up8RW"></a>
## iostat -xz 1
```
$ iostat -xz 1 
Linux 3.13.0-49-generic (titanclusters-xxxxx)  07/14/2015  x86_64 (32 CPU) 
avg-cpu:  %user   %nice %system %iowait  %steal   %idle 
73.96    0.00    3.73    0.03    0.06   22.21 
Device:   rrqm/s   wrqm/s     r/s     w/s    rkB/s    wkB/s avgrq-sz avgqu-sz   await r_await w_await  svctm  %util 
xvda        0.00     0.23    0.21    0.18     4.52     2.08    34.37     0.00    9.98   13.80    5.42   2.44   0.09 
xvdb        0.01     0.00    1.02    8.94   127.97   598.53   145.79     0.00    0.43    1.78    0.28   0.25   0.25 
xvdc        0.01     0.00    1.02    8.86   127.79   595.94   146.50     0.00    0.45    1.82    0.30   0.27   0.26 
dm-0        0.00     0.00    0.69    2.32    10.47    31.69    28.01     0.01    3.23    0.71    3.98   0.13   0.04 
dm-1        0.00     0.00    0.00    0.94     0.01     3.78     8.00     0.33  345.84    0.04  346.81   0.01   0.00 
dm-2        0.00     0.00    0.09    0.07     1.35     0.36    22.50     0.00    2.55    0.23
```
iostat 是理解块设备(磁盘)的当前负载和性能的重要工具。几个指标的含义：
```
r/s，w/s，rkB/s，wkB/s：#系统发往设备的每秒的读次数、每秒写次数、每秒读的数据量、每秒写的数据量。这几个指标反映的是系统的工作负载。系统的性能问题很有可能就是负载太大。
await：#系统发往 IO 设备的请求的平均响应时间。这包括请求排队的时间，以及请求处理的时间。超过经验值的平均响应时间表明设备处于饱和状态，或者设备有问题。
avgqu-sz：#设备请求队列的平均长度。队列长度大于 1 表示设备处于饱和状态。
%util：#设备利用率。设备繁忙的程度，表示每一秒之内，设备处理 IO 的时间占比。大于 60% 的利用率通常会导致性能问题(可以通过 await 看到)，但是每种设备也会有有所不同。接近 100% 的利用率表明磁盘处于饱和状态。
```
如果这个块设备是一个逻辑块设备，这个逻辑快设备后面有很多物理的磁盘的话，100% 利用率只能表明有些 IO 的处理时间达到了 100%；后端的物理磁盘可能远远没有达到饱和状态，可以处理更多的负载。<br />还有一点需要注意的是，较差的磁盘 IO 性能并不一定意味着应用程序会有问题。应用程序可以有许多方法执行异步 IO，而不会阻塞在 IO 上面；应用程序也可以使用诸如预读取，写缓冲等技术降低 IO 延迟对自身的影响。
<a name="oULPW"></a>
## `free -m`
```bash
$ free -m 
         total       used       free     shared    buffers     cached 
Mem:        245998      24545     221453         83         59        541 
-/+ buffers/cache:      23944     222053 
Swap:
```
右边的两列显式：

- buffers：用于块设备 I/O 的缓冲区缓存。
- cached：用于文件系统的页面缓存。

只是想要检查这些不接近零的大小，其可能会导致更高磁盘 I/O(使用 iostat 确认)，和更糟糕的性能。上面的例子看起来还不错，每一列均有很多 M 个大小。<br />比起第一行，-/+ buffers/cache 提供的内存使用量会更加准确些。Linux 会把暂时用不上的内存用作缓存，一旦应用需要的时候就立刻重新分配给它。所以部分被用作缓存的内存其实也算是空闲的内存。<br />如果使用 ZFS 的话，可能会有点困惑。ZFS 有自己的文件系统缓存，在 `free -m` 里面看不到；系统看起来空闲内存不多了，但是有可能 ZFS 有很多的缓存可用。
<a name="dY07P"></a>
## `sar -n DEV 1`
```bash
$ sar -n DEV 1 
Linux 3.13.0-49-generic (titanclusters-xxxxx)  07/14/2015     _x86_64_    (32 CPU)

12:16:48 AM     IFACE   rxpck/s   txpck/s    rxkB/s    txkB/s   rxcmp/s   txcmp/s  rxmcst/s   %ifutil 
12:16:49 AM      eth0  18763.00   5032.00  20686.42    478.30      0.00      0.00      0.00      0.00 
12:16:49 AM        lo     14.00     14.00      1.36      1.36      0.00      0.00      0.00      0.00 
12:16:49 AM   docker0      0.00      0.00      0.00      0.00      0.00      0.00      0.00      0.00

12:16:49 AM     IFACE   rxpck/s   txpck/s    rxkB/s    txkB/s   rxcmp/s   txcmp/s  rxmcst/s   %ifutil 
12:16:50 AM      eth0  19763.00   5101.00  21999.10    482.56      0.00      0.00      0.00      0.00 
12:16:50 AM        lo     20.00     20.00      3.25      3.25      0.00      0.00      0.00      0.00 
12:16:50 AM   docker0      0.00      0.00      0.00      0.00      0.00      0.00      0.00      0.00 
^C
```
这个工具可以查看网络接口的吞吐量：rxkB/s 和 txkB/s 可以测量负载，也可以看是否达到网络流量限制了。在上面的例子里，eth0 的吞吐量达到了大约 22 Mbytes/s，差不多 176 Mbits/sec ，比 1 Gbit/sec 还要少很多。<br />这个例子里也有 %ifutil 标识设备利用率，也用 Brenan 的 nicstat tool 测量。和 nicstat 一样，这个设备利用率很难测量正确，上面的例子里好像这个值还有点问题。
<a name="KPyyx"></a>
## `sar -n TCP,ETCP 1`
```bash
$ sar -n TCP,ETCP 1 
Linux 3.13.0-49-generic (titanclusters-xxxxx)  07/14/2015    _x86_64_    (32 CPU)

12:17:19 AM  active/s passive/s    iseg/s    oseg/s 
12:17:20 AM      1.00      0.00  10233.00  18846.00

12:17:19 AM  atmptf/s  estres/s retrans/s isegerr/s   orsts/s 
12:17:20 AM      0.00      0.00      0.00      0.00      0.00

12:17:20 AM  active/s passive/s    iseg/s    oseg/s 
12:17:21 AM      1.00      0.00   8359.00   6039.00

12:17:20 AM  atmptf/s  estres/s retrans/s isegerr/s   orsts/s 
12:17:21 AM      0.00      0.00      0.00      0.00      0.00 
^C
```
这是对 TCP 重要指标的一些概括，包括：
```
active/s：#每秒钟本地主动开启的 TCP 连接，也就是本地程序使用 connect() 系统调用
passive/s：#每秒钟从源端发起的 TCP 连接，也就是本地程序使用 accept() 所接受的连接
retrans/s：#每秒钟的 TCP 重传次数
```
atctive 和 passive 的数目通常可以用来衡量服务器的负载：接受连接的个数(passive)，下游连接的个数(active)。可以简单认为 active 为出主机的连接，passive 为入主机的连接；但这个不是很严格的说法，比如 loalhost 和 localhost 之间的连接。重传表示网络或者服务器的问题。也许是网络不稳定了，也许是服务器负载过重开始丢包了。上面这个例子表示每秒只有 1 个新连接建立。
<a name="Ca7r7"></a>
## top
```bash
$ top 
top - 00:15:40 up 21:56,  1 user,  load average: 31.09, 29.87, 29.92 
Tasks: 871 total,   1 running, 868 sleeping,   0 stopped,   2 zombie 
%Cpu(s): 96.8 us,  0.4 sy,  0.0 ni,  2.7 id,  0.1 wa,  0.0 hi,  0.0 si,  0.0 st 
KiB Mem:  25190241+total, 24921688 used, 22698073+free,    60448 buffers 
KiB Swap:        0 total,        0 used,        0 free.   554208 cached Mem

PID USER      PR  NI    VIRT    RES    SHR S  %CPU %MEM     TIME+ COMMAND 
20248 root      20   0  0.227t 0.012t  18748 S  3090  5.2  29812:58 java 
4213 root      20   0 2722544  64640  44232 S  23.5  0.0 233:35.37 mesos-slave 
66128 titancl+  20   0   24344   2332   1172 R   1.0  0.0   0:00.07 top 
5235 root      20   0 38.227g 547004  49996 S   0.7  0.2   2:02.74 java 
4299 root      20   0 20.015g 2.682g  16836 S   0.3  1.1  33:14.42 java 
1 root      20   0   33620   2920   1496 S   0.0  0.0   0:03.82 init 
2 root      20   0       0      0      0 S   0.0  0.0   0:00.02 kthreadd 
3 root      20   0       0      0      0 S   0.0  0.0   0:05.35 ksoftirqd/0 
5 root       0 -20       0      0      0 S   0.0  0.0   0:00.00 kworker/0:0H 
6 root      20   0       0      0      0 S   0.0  0.0   0:06.94 kworker/u256:0 
8 root      20   0       0      0      0 S   0.0  0.0   2:38.05 rcu_sched
```
top 命令涵盖了许多指标。可以用它来看和之前查看的结果有没有很大的不同，如果有的话，那表示系统的负载在变化。<br />top 的缺点就是很难找到这些指标随着时间的一些行为模式，在这种情况下，vmstat 或者 pidstat 这种可以提供滚动输出的命令是更好的方式。如果不以足够快的速度暂停输出(Ctrl-S 暂停，Ctrl-Q 继续)，一些间歇性问题的线索也可能由于被清屏而丢失。
