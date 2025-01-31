Linux 虚拟内存
<a name="GZ8NN"></a>
## 由来
<a name="MWRpr"></a>
### 虚拟内存
虚拟内存绝对是操作系统中最重要的概念之一。主要是由于内存的重要“战略地位”。CPU太快，但容量小且功能单一，其他 I/O 硬件支持各种花式功能，可是相对于 CPU，它们又太慢。于是它们之间就需要一种润滑剂来作为缓冲，这就是内存大显身手的地方。<br />而在现代操作系统中，多任务已是标配。多任务并行，大大提升了 CPU 利用率，但却引出了多个进程对内存操作的冲突问题，虚拟内存概念的提出就是为了解决这个问题。<br />![2021-05-03-20-32-04-350072.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620045230588-9725c335-37c3-43f5-87bd-4965b33a6a14.jpeg#clientId=uefa89a5e-2b4b-4&from=ui&id=uc2b76a76&originHeight=367&originWidth=599&originalType=binary&size=14471&status=done&style=shadow&taskId=u8ecb523a-e4b7-4c63-870f-f9c901f39fe)<br />上图是虚拟内存最简单也是最直观的解释。<br />操作系统有一块物理内存（中间的部分），有两个进程（实际会更多）P1 和 P2，操作系统偷偷地分别告诉 P1 和 P2，我的整个内存都是你的，随便用，管够。可事实上呢，操作系统只是给它们画了个大饼，这些内存说是都给了 P1 和 P2，实际上只给了它们一个序号而已。只有当 P1 和 P2 真正开始使用这些内存时，系统才开始使用辗转挪移，拼凑出各个块给进程用，P2 以为自己在用 A 内存，实际上已经被系统悄悄重定向到真正的 B 去了，甚至，当 P1 和 P2 共用了 C 内存，他们也不知道。<br />操作系统的这种欺骗进程的手段，就是虚拟内存。对 P1 和 P2 等进程来说，它们都以为自己占用了整个内存，而自己使用的物理内存的哪段地址，它们并不知道也无需关心。
<a name="AevgE"></a>
### 分页和页表
虚拟内存是操作系统里的概念，对操作系统来说，虚拟内存就是一张张的对照表，P1 获取 A 内存里的数据时应该去物理内存的 A 地址找，而找 B 内存里的数据应该去物理内存的 C 地址。<br />系统里的基本单位都是 Byte 字节，如果将每一个虚拟内存的 Byte 都对应到物理内存的地址，每个条目最少需要 8字节（32位虚拟地址->32位物理地址），在 4G 内存的情况下，就需要 32GB 的空间来存放对照表，那么这张表就大得真正的物理地址也放不下了，于是操作系统引入了 页（Page）的概念。<br />在系统启动时，操作系统将整个物理内存以 4K 为单位，划分为各个页。之后进行内存分配时，都以页为单位，那么虚拟内存页对应物理内存页的映射表就大大减小了，4G 内存，只需要 8M 的映射表即可，一些进程没有使用到的虚拟内存，也并不需要保存映射关系，而且Linux 还为大内存设计了多级页表，可以进一页减少了内存消耗。操作系统虚拟内存到物理内存的映射表，就被称为页表。
<a name="q8bVd"></a>
### 内存寻址和分配
通过虚拟内存机制，每个进程都以为自己占用了全部内存，进程访问内存时，操作系统都会把进程提供的虚拟内存地址转换为物理地址，再去对应的物理地址上获取数据。CPU 中有一种硬件，内存管理单元 MMU（Memory Management Unit）专门用来将翻译虚拟内存地址。CPU 还为页表寻址设置了缓存策略，由于程序的局部性，其缓存命中率能达到 98%。<br />以上情况是页表内存在虚拟地址到物理地址的映射，而如果进程访问的物理地址还没有被分配，系统则会产生一个缺页中断，在中断处理时，系统切到内核态为进程虚拟地址分配物理地址。
<a name="b1bUq"></a>
## 功能
虚拟内存不仅通过内存地址转换解决了多个进程访问内存冲突的问题，还带来更多的益处。
<a name="LoypI"></a>
### 进程内存管理
它有助于进程进行内存管理，主要体现在：

- 内存完整性：由于虚拟内存对进程的”欺骗”，每个进程都认为自己获取的内存是一块连续的地址。在编写应用程序时，就不用考虑大块地址的分配，总是认为系统有足够的大块内存即可。
- 安全：由于进程访问内存时，都要通过页表来寻址，操作系统在页表的各个项目上添加各种访问权限标识位，就可以实现内存的权限控制。
<a name="GxNpt"></a>
### 数据共享
通过虚拟内存更容易实现内存和数据的共享。<br />在进程加载系统库时，总是先分配一块内存，将磁盘中的库文件加载到这块内存中，在直接使用物理内存时，由于物理内存地址唯一，即使系统发现同一个库在系统内加载了两次，但每个进程指定的加载内存不一样，系统也无能为力。<br />而在使用虚拟内存时，系统只需要将进程的虚拟内存地址指向库文件所在的物理内存地址即可。如上文图中所示，进程 P1 和 P2 的 B 地址都指向了物理地址 C。<br />而通过使用虚拟内存使用共享内存也很简单，系统只需要将各个进程的虚拟内存地址指向系统分配的共享内存地址即可。
<a name="GZV0o"></a>
### SWAP
虚拟内存可以让帮进程”扩充”内存。<br />前文提到了虚拟内存通过缺页中断为进程分配物理内存，内存总是有限的，如果所有的物理内存都被占用了怎么办呢？<br />Linux 提出 SWAP 的概念，Linux 中可以使用 SWAP 分区，在分配物理内存，但可用内存不足时，将暂时不用的内存数据先放到磁盘上，让有需要的进程先使用，等进程再需要使用这些数据时，再将这些数据加载到内存中，通过这种”交换”技术，Linux 可以让进程使用更多的内存。
<a name="w7i80"></a>
## 常见问题
<a name="XJLOu"></a>
### 32位和64位
最常见的就是 32位和64位的问题了。<br />CPU 通过物理总线访问内存，那么访问地址的范围就受限于机器总线的数量，在32位机器上，有32条总线，每条总线有高低两种电位分别代表 bit 的 1 和 0，那么可访问的最大地址就是 2^32bit = 4GB，所以说 32 位机器上插入大于 4G 的内存是无效的，CPU 访问不到多于 4G 的内存。<br />但 64位机器并没有 64位总线，而且其最大内存还要受限于操作系统，Linux 目前支持最大 256G 内存。<br />根据虚拟内存的概念，在 32 位系统上运行 64 位软件也并无不可，但由于系统对虚拟内存地址的结构设计，64位的虚拟地址在32位系统内并不能使用。
<a name="U82rG"></a>
### 直接操作物理内存
操作系统使用了虚拟内存，想要直接操作内存该怎么办呢？<br />Linux 会将各个设备都映射到 /dev/ 目录下的文件，可以通过这些设备文件直接操作硬件，内存也不例外。在 Linux 中，内存设置被映射为 /dev/mem，root 用户通过对这个文件读写，可以直接操作内存。
<a name="wZrZh"></a>
### JVM 进程占用虚拟内存过多
使用 TOP 查看系统性能时，可以发现在 VIRT 这一列，Java 进程会占用大量的虚拟内存。<br />![2021-05-03-20-32-04-573283.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620045313321-5695705c-03b9-42bf-87d0-fb3c9478853b.png#clientId=uefa89a5e-2b4b-4&from=ui&id=u484b353b&originHeight=80&originWidth=576&originalType=binary&size=16187&status=done&style=none&taskId=ub70b8778-a975-44af-a50b-31ea84f7455)<br />导致这种问题的原因是 Java 使用 Glibc 的 Arena 内存池分配了大量的虚拟内存并没有使用。此外，Java 读取的文件也会被映射为虚拟内存，在虚拟机默认配置下 Java 每个线程栈会占用 1M 的虚拟内存。具体可以查看 为什么linux下多线程程序如此消耗虚拟内存。<br />而真实占用的物理内存要看 RES (resident) 列，这一列的值才是真正被映射到物理内存的大小。
<a name="NeBgz"></a>
## 常用管理命令
也可以自己来管理 Linux 的虚拟内存。
<a name="SzcKN"></a>
### 查看系统内存状态
查看系统内存情况的方式有很多，`free`、 `vmstat`等命令都可输出当前系统的内存状态，需要注意的是可用内存并不只是 free 这一列，由于操作系统的 lazy 特性，大量的 buffer/cache 在进程不再使用后，不会被立即清理，如果之前使用它们的进程再次运行还可以继续使用，它们在必要时也是可以被利用的。<br />此外，通过 `cat /proc/meminfo` 可以查看系统内存被使用的详细情况，包括脏页状态等。
<a name="m5le8"></a>
### `pmap`
如果想单独查看某一进程的虚拟内存分布情况，可以使用 `pmap pid` 命令，它会把虚拟内存各段的占用情况从低地址到高地址都列出来。<br />可以添加 `-XX` 参数来输出更详细的信息。
<a name="nCNgj"></a>
### 修改内存配置
也可以修改 Linux 的系统配置，使用 `sysctl vm [-options] CONFIG` 或 直接读写 `/proc/sys/vm/` 目录下的文件来查看和修改配置。
<a name="rxr1J"></a>
### `SWAP` 操作
虚拟内存的 SWAP 特性并不总是有益，放任进程不停地将数据在内存与磁盘之间大量交换会极大地占用 CPU，降低系统运行效率，所以有时候并不希望使用 `swap`。<br />可以修改 `vm.swappiness=0` 来设置内存尽量少使用 `swap`，或者干脆使用 `swapoff` 命令禁用掉 `SWAP`。
