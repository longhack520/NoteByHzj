Linux<br />Linux系统一般有4个主要部分：内核、shell、文件系统和应用程序。内核、shell和文件系统一起形成了基本的操作系统结构，它们使得用户可以运行程序、管理文件并使用系统。<br />![2021-07-08-22-10-14-266456.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625753478966-7492fe44-0896-4893-9be3-a3b460fb8c68.png#averageHue=%23f7f7f5&clientId=u5b1b95be-ce8f-4&from=ui&id=ueb6662a4&originHeight=298&originWidth=300&originalType=binary&ratio=1&rotation=0&showTitle=false&size=70792&status=done&style=shadow&taskId=u90c28632-4f4b-4a92-926f-0a1f5957996&title=)
<a name="RnAc8"></a>
## 1、Linux内核
内核是操作系统的核心，具有很多最基本功能，它负责管理系统的进程、内存、设备驱动程序、文件和网络系统，决定着系统的性能和稳定性。<br />Linux 内核由如下几部分组成：内存管理、进程管理、设备驱动程序、文件系统和网络管理等。如图：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1625753396533-c977a1ae-c176-4730-b48f-661d7b23f97a.png#averageHue=%23f1f1f1&clientId=u5b1b95be-ce8f-4&from=paste&id=ue5d252a9&originHeight=481&originWidth=550&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud1b5ec71-8c9b-476f-8c32-7eacabe7135&title=)<br />系统调用接口：SCI 层提供了某些机制执行从用户空间到内核的函数调用。这个接口依赖于体系结构，甚至在相同的处理器家族内也是如此。SCI 实际上是一个非常有用的函数调用多路复用和多路分解服务。在 ./linux/kernel 中可以找到 SCI 的实现，并在 ./linux/arch 中找到依赖于体系结构的部分。
<a name="ucAaZ"></a>
### 1.1 内存管理
对任何一台计算机而言，其内存以及其它资源都是有限的。为了让有限的物理内存满足应用程序对内存的大需求量，Linux 采用了称为“虚拟内存”的内存管理方式。Linux 将内存划分为容易处理的“内存页”（对于大部分体系结构来说都是 4KB）。Linux 包括了管理可用内存的方式，以及物理和虚拟映射所使用的硬件机制。<br />不过内存管理要管理的可不止 4KB 缓冲区。Linux 提供了对 4KB 缓冲区的抽象，例如 slab 分配器。这种内存管理模式使用 4KB 缓冲区为基数，然后从中分配结构，并跟踪内存页使用情况，比如哪些内存页是满的，哪些页面没有完全使用，哪些页面为空。这样就允许该模式根据系统需要来动态调整内存使用。<br />为了支持多个用户使用内存，有时会出现可用内存被消耗光的情况。由于这个原因，页面可以移出内存并放入磁盘中。这个过程称为交换，因为页面会被从内存交换到硬盘上。内存管理的源代码可以在 ./linux/mm 中找到。
<a name="PGdpJ"></a>
### 1.2 进程管理
进程实际是某特定应用程序的一个运行实体。在 Linux 系统中，能够同时运行多个进程，Linux 通过在短的时间间隔内轮流运行这些进程而实现“多任务”。这一短的时间间隔称为“时间片”，让进程轮流运行的方法称为“进程调度” ，完成调度的程序称为调度程序。<br />进程调度控制进程对CPU的访问。当需要选择下一个进程运行时，由调度程序选择最值得运行的进程。可运行进程实际上是仅等待CPU资源的进程，如果某个进程在等待其它资源，则该进程是不可运行进程。Linux使用了比较简单的基于优先级的进程调度算法选择新的进程。<br />通过多任务机制，每个进程可认为只有自己独占计算机，从而简化程序的编写。每个进程有自己单独的地址空间，并且只能由这一进程访问，这样，操作系统避免了进程之间的互相干扰以及“坏”程序对系统可能造成的危害。为了完成某特定任务，有时需要综合两个程序的功能，例如一个程序输出文本，而另一个程序对文本进行排序。为此，操作系统还提供进程间的通讯机制来帮助完成这样的任务。Linux 中常见的进程间通讯机制有信号、管道、共享内存、信号量和套接字等。<br />内核通过 SCI 提供了一个应用程序编程接口（API）来创建一个新进程（fork、exec 或 Portable Operating System Interface [POSⅨ] 函数），停止进程（kill、exit），并在它们之间进行通信和同步（signal 或者 POSⅨ 机制）。
<a name="aZn4O"></a>
### 1.3 文件系统
和 DOS 等操作系统不同，Linux 操作系统中单独的文件系统并不是由驱动器号或驱动器名称（如 A: 或 C: 等）来标识的。相反，和 UNIX 操作系统一样，Linux 操作系统将独立的文件系统组合成了一个层次化的树形结构，并且由一个单独的实体代表这一文件系统。Linux 将新的文件系统通过一个称为“挂装”或“挂上”的操作将其挂装到某个目录上，从而让不同的文件系统结合成为一个整体。Linux 操作系统的一个重要特点是它支持许多不同类型的文件系统。Linux 中最普遍使用的文件系统是 Ext2，它也是 Linux 土生土长的文件系统。但 Linux 也能够支持 FAT、VFAT、FAT32、MINIX 等不同类型的文件系统，从而可以方便地和其它操作系统交换数据。由于 Linux 支持许多不同的文件系统，并且将它们组织成了一个统一的虚拟文件系统.<br />虚拟文件系统（VirtualFileSystem,VFS）：隐藏了各种硬件的具体细节，把文件系统操作和不同文件系统的具体实现细节分离了开来，为所有的设备提供了统一的接口，VFS提供了多达数十种不同的文件系统。虚拟文件系统可以分为逻辑文件系统和设备驱动程序。逻辑文件系统指Linux所支持的文件系统，如ext2,fat等，设备驱动程序指为每一种硬件控制器所编写的设备驱动程序模块。<br />虚拟文件系统（VFS）是 Linux 内核中非常有用的一个方面，因为它为文件系统提供了一个通用的接口抽象。VFS 在 SCI 和内核所支持的文件系统之间提供了一个交换层。即VFS在用户和文件系统之间提供了一个交换层。<br />VFS在用户和文件系统之间提供了一个交换层：<br />![2021-07-08-22-10-15-099448.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625753507728-81bf890f-13aa-4d8f-a5fb-0271a190c39d.png#averageHue=%23cedfbb&clientId=u5b1b95be-ce8f-4&from=ui&id=u107f52b1&originHeight=245&originWidth=300&originalType=binary&ratio=1&rotation=0&showTitle=false&size=60807&status=done&style=shadow&taskId=u83c6fb97-f4ff-4c8e-b295-1291dd7684c&title=)<br />在 VFS 上面，是对诸如 open、close、read 和 write 之类的函数的一个通用 API 抽象。在 VFS 下面是文件系统抽象，它定义了上层函数的实现方式。它们是给定文件系统（超过 50 个）的插件。文件系统的源代码可以在 ./linux/fs 中找到。<br />文件系统层之下是缓冲区缓存，它为文件系统层提供了一个通用函数集（与具体文件系统无关）。这个缓存层通过将数据保留一段时间（或者随即预先读取数据以便在需要是就可用）优化了对物理设备的访问。缓冲区缓存之下是设备驱动程序，它实现了特定物理设备的接口。<br />因此，用户和进程不需要知道文件所在的文件系统类型，而只需要象使用 Ext2 文件系统中的文件一样使用它们。
<a name="IBXlU"></a>
### 1.4 设备驱动程序
设备驱动程序是 Linux 内核的主要部分。和操作系统的其它部分类似，设备驱动程序运行在高特权级的处理器环境中，从而可以直接对硬件进行操作，但正因为如此，任何一个设备驱动程序的错误都可能导致操作系统的崩溃。设备驱动程序实际控制操作系统和硬件设备之间的交互。<br />设备驱动程序提供一组操作系统可理解的抽象接口完成和操作系统之间的交互，而与硬件相关的具体操作细节由设备驱动程序完成。一般而言，设备驱动程序和设备的控制芯片有关，例如，如果计算机硬盘是 SCSI 硬盘，则需要使用 SCSI 驱动程序，而不是 IDE 驱动程序。
<a name="ngy3n"></a>
### 1.5 网络接口（NET）
提供了对各种网络标准的存取和各种网络硬件的支持。网络接口可分为网络协议和网络驱动程序。网络协议部分负责实现每一种可能的网络传输协议。众所周知，TCP/IP 协议是 Internet 的标准协议，同时也是事实上的工业标准。<br />Linux 的网络实现支持 BSD 套接字，支持全部的TCP/IP协议。Linux内核的网络部分由BSD套接字、网络协议层和网络设备驱动程序组成。网络设备驱动程序负责与硬件设备通讯，每一种可能的硬件设备都有相应的设备驱动程序。
<a name="TzwoB"></a>
## 2、Linux Shell
shell是系统的用户界面，提供了用户与内核进行交互操作的一种接口。它接收用户输入的命令并把它送入内核去执行，是一个命令解释器。另外，shell编程语言具有普通编程语言的很多特点，用这种编程语言编写的shell程序与其他应用程序具有同样的效果。<br />目前主要有下列版本的shell。

1. Bourne Shell：是贝尔实验室开发的。
2. BASH：是GNU的Bourne Again Shell，是GNU操作系统上默认的shell，大部分Linux的发行套件使用的都是这种shell。
3. Korn Shell：是对Bourne SHell的发展，在大部分内容上与Bourne Shell兼容。
4. C Shell：是SUN公司Shell的BSD版本。
<a name="RQAbx"></a>
## 3、Linux 文件系统
文件系统是文件存放在磁盘等存储设备上的组织方法。Linux系统能支持多种目前流行的文件系统，如EXT2、 EXT3、 FAT、 FAT32、 VFAT和ISO9660。
<a name="nrUtW"></a>
### 3.1 文件类型
Linux下面的文件类型主要有：

1. 普通文件：C语言元代码、SHELL脚本、二进制的可执行文件等。分为纯文本和二进制。
2. 目录文件：目录，存储文件的唯一地方。
3. 链接文件：指向同一个文件或目录的的文件。
4. 设备文件：与系统外设相关的，通常在/dev下面。分为块设备和字符设备。
5. 管道(FIFO)文件: 提供进程之间通信的一种方式
6. 套接字(socket) 文件：该文件类型与网络通信有关

可以通过ls –l, file, stat几个命令来查看文件的类型等相关信息。
<a name="aAZOX"></a>
### 3.2 Linux目录
文件结构是文件存放在磁盘等存贮设备上的组织方法。主要体现在对文件和目录的组织上；<br />目录提供了管理文件的一个方便而有效的途径。<br />Linux使用标准的目录结构，在安装的时候，安装程序就已经为用户创建了文件系统和完整而固定的目录组成形式，并指定了每个目录的作用和其中的文件类型。<br />完整的目录树可划分为小的部分，这些小部分又可以单独存放在自己的磁盘或分区上。这样，相对稳定的部分和经常变化的部分可单独存放在不同的分区中，从而方便备份或系统管理。目录树的主要部分有 root、/usr、/var、/home 等。这样的布局可方便在 Linux 计算机之间共享文件系统的某些部分。<br />![2021-07-08-22-10-15-796457.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625753553345-10bc2d73-8a0b-4567-8ca4-f6c036e287ad.png#averageHue=%23f2f2f2&clientId=u5b1b95be-ce8f-4&from=ui&id=u483239f3&originHeight=224&originWidth=500&originalType=binary&ratio=1&rotation=0&showTitle=false&size=36662&status=done&style=shadow&taskId=ufdcd93c4-c148-4392-b2ec-9aa5a8d167f&title=)<br />Linux采用的是树型结构。最上层是根目录，其他的所有目录都是从根目录出发而生成的。<br />微软的DOS和windows也是采用树型结构，但是在DOS和windows中这样的树型结构的根是磁盘分区的盘符，有几个分区就有几个树型结构，他们之间的关系是并列的。最顶部的是不同的磁盘（分区），如：C，D，E，F等。<br />但是在linux中，无论操作系统管理几个磁盘分区，这样的目录树只有一个。从结构上讲，各个磁盘分区上的树型目录不一定是并列的。
<a name="AlIAz"></a>
### 3.3 Linux磁盘分区
主分区，扩展分区和逻辑分区：<br />linux分区不同于windows,硬盘和硬盘分区在Linux都表示为设备.<br />硬盘分区一共有三种：主分区，扩展分区和逻辑分区。<br />硬盘的分区主要分为主分区(Primary Partion)和扩展分区(Extension Partion)两种，主分区和扩展分区的数目之和不能大于四个。<br />主分区(Primary Partion)：可以马上被使用但不能再分区。<br />扩展分区(Extension Partion)：必须再进行分区后才能使用，也就是说它必须还要进行二次分区。<br />逻辑分区((Logical Partion))：由扩展分区建立起来的分区，逻辑分区没有数量上限制。<br />扩展分区只不过是逻辑分区的“容器”，实际上只有主分区和逻辑分区进行数据存储。<br />Linux下硬盘分区的标识<br />硬盘分区的标识一般使用/dev/hd[a-z]X或者/dev/sd[a-z]X来标识，其中[a-z]代表硬盘号，X代表硬盘内的分区号。<br />整块硬盘分区的块号标识:Linux下用hda、hdb、sda、sdb 等来标识不同的硬盘;<br />其中：<br />IDE接口硬盘：表示为/dev/hda1、/dev/hdb …；<br />SCSI 接口的硬盘、SATA接口的硬盘表示为/dev/sda、/dev/sdb … … ；<br />硬盘内的分区：如果X的值是1到4,表示硬盘的主分区（包含扩展分区）；逻辑分区从是从5开始的，比如/dev/hda5肯定是逻辑分区了；<br />例如：<br />用hda1、hda2、 hda5、hda6 来标识不同的分区。其中，字母a代表第一块硬盘，b代表第二块硬盘，依次类推。而数字1 代表一块硬盘的第一个分区、2 代表第二个分区，依次类推。1 到4 对应的是主分区(Primary Partition)或扩展分区(Extension Partition)。从5开始，对应的都是硬盘的逻辑分区(Logical Partition)。一块硬盘即使只有一个主分区，逻辑分区也是从5开始编号的，这点应特别注意。<br />总结：一个硬盘分区首先要确认在哪个硬盘，然后再确认它所在硬盘内的哪个分区。<br />对于/dev/hda 类似的表示方法，也并不陌生吧；在Linux通`过fdisk -l` 就可以查到硬盘是/dev/hda还是/dev/hdb；
```bash
[root@localhost ~]# fdisk -l
Disk /dev/hda: 80.0 GB, 80026361856 bytes
255 heads, 63 sectors/track, 9729 cylinders
Units = cylinders of 16065 * 512 = 8225280 bytes
Device Boot Start End Blocks Id System
/dev/hda1 * 1 970 7791493+ 7 HPFS/NTFS
/dev/hda2 971 9729 70356667+ 5 Extended
/dev/hda5 971 2915 15623181 b W95 FAT32
/dev/hda6 2916 4131 9767488+ 83 linux
/dev/hda7 4132 5590 11719386 83 linux
/dev/hda8 5591 6806 9767488+ 83 linux
/dev/hda9 6807 9657 22900626 83 linux
/dev/hda10 9658 9729 578308+ 82 linux swap / Solaris
```
注意第一行， Disk /dev/hda: 80.0 GB, 80026361856 bytes ，这个就是表示机器中只有一个硬盘设备/dev/hda ，体积大小为 80.0G；下面的就是硬盘的分区，每个分区都有详细的信息，在这里不详细说了；<br />Linux下磁盘分区和目录的关系如下：<br />– 任何一个分区都必须挂载到某个目录上。<br />– 目录是逻辑上的区分。分区是物理上的区分。<br />– 磁盘Linux分区都必须挂载到目录树中的某个具体的目录上才能进行读写操作。<br />– 根目录是所有Linux的文件和目录所在的地方，需要挂载上一个磁盘分区。
<a name="CLPTv"></a>
### 3.4 Linux主要目录的功用。
/bin 二进制可执行命令<br />/dev 设备特殊文件<br />/etc 系统管理和配置文件<br />/etc/rc.d 启动的配置文件和脚本<br />/home 用户主目录的基点，比如用户user的主目录就是/home/user，可以用~user表示<br />/lib 标准程序设计库，又叫动态链接共享库，作用类似windows里的.dll文件<br />/sbin 系统管理命令，这里存放的是系统管理员使用的管理程序<br />/tmp 公用的临时文件存储点<br />/root 系统管理员的主目录（呵呵，特权阶级）<br />/mnt 系统提供这个目录是让用户临时挂载其他的文件系统。<br />/lost+found 这个目录平时是空的，系统非正常关机而留下“无家可归”的文件（windows下叫什么.chk）就在这里<br />/proc 虚拟的目录，是系统内存的映射。可直接访问这个目录来获取系统信息。<br />/var 某些大文件的溢出区，比方说各种服务的日志文件<br />/usr 最庞大的目录，要用到的应用程序和文件几乎都在这个目录。其中包含：<br />/usr/X11R6 存放X window的目录<br />/usr/bin 众多的应用程序<br />/usr/sbin 超级用户的一些管理程序<br />/usr/doc linux文档<br />/usr/include linux下开发和编译应用程序所需要的头文件<br />/usr/lib 常用的动态链接库和软件包的配置文件<br />/usr/man 帮助文档<br />/usr/src 源代码，linux内核的源代码就放在/usr/src/linux里<br />/usr/local/bin 本地增加的命令<br />/usr/local/lib 本地增加的库
<a name="gNVmc"></a>
### 3.5 linux文件系统
文件系统指文件存在的物理空间，linux系统中每个分区都是一个文件系统，都有自己的目录层次结构。linux会将这些分属不同分区的、单独的文件系统按一定的方式形成一个系统的总的目录层次结构。一个操作系统的运行离不开对文件的操作，因此必然要拥有并维护自己的文件系统。

1. 文件系统类型：
```bash
ext2 ： 早期linux中常用的文件系统
ext3 ：ext2的升级版，带日志功能
RAMFS ： 内存文件系统，速度很快
NFS ： 网络文件系统，由SUN发明，主要用于远程文件共享
MS-DOS ：MS-DOS文件系统
VFAT ：Windows 95/98 操作系统采用的文件系统
FAT ：Windows XP 操作系统采用的文件系统
NTFS：Windows NT/XP 操作系统采用的文件系统
HPFS ：OS/2 操作系统采用的文件系统
PROC : 虚拟的进程文件系统
ISO9660 ： 大部分光盘所采用的文件系统
ufsSun : OS 所采用的文件系统
NCPFS ：Novell 服务器所采用的文件系统
SMBFS ：Samba 的共享文件系统
XFS ： 由SGI开发的先进的日志文件系统，支持超大容量文件
JFS ：IBM的AIX使用的日志文件系统
ReiserFS : 基于平衡树结构的文件系统
udf: 可擦写的数据光盘文件系统
```

2. 文件系统特性：

磁盘分区完毕后还需要进行格式化(format)，之后操作系统才能够使用这个分区。格式化的目的是能使操作系统可以使用的文件系统格式（即上面提到文件系统类型）.<br />每种操作系统能够使用的文件系统并不相同. 如windows 98 以前的微软操作系统主要利用的文件系统是 FAT (或 FAT16)，windows 2000 以后的版本有所谓的 NTFS 文件系统，至于 Linux 的正统文件系统则为 Ext2 (Linux second extended file system, ext2fs)这一个。此外，在默认的情况下，windows 操作系统是不会认识 Linux 的 Ext2 的。<br />传统的磁盘与文件系统之应用中，一个分区就是只能够被格式化成为一个文件系统，所以可以说一个 filesystem 就是一个 partition。但是由于新技术的利用，例如常听到的LVM与软件磁盘阵列(software raid)， 这些技术可以将一个分区格式化为多个文件系统(例如LVM)，也能够将多个分区合成一个文件系统(LVM, RAID)！所以说，目前在格式化时已经不再说成针对 partition 来格式化了， 通常可以称呼一个可被挂载的数据为一个文件系统而不是一个分区喔！<br />那么文件系统是如何运行的呢？这与操作系统的文件数据有关。较新的操作系统的文件数据除了文件实际内容外， 通常含有非常多的属性，例如 Linux 操作系统的文件权限(rwx)与文件属性(拥有者、群组、时间参数等)。文件系统通常会将这两部份的数据分别存放在不同的区块，权限与属性放置到 inode 中，至于实际数据则放置到 data block 区块中。另外，还有一个超级区块 (superblock) 会记录整个文件系统的整体信息，包括 inode 与 block 的总量、使用量、剩余量等。<br />对于一个磁盘分区来说，在被指定为相应的文件系统后，整个分区被分为 1024，2048 和 4096 字节大小的块。根据块使用的不同，可分为：<br />超级块(Superblock)：这是整个文件系统的第一块空间。包括整个文件系统的基本信息，如块大小，inode/block的总量、使用量、剩余量，指向空间 inode 和数据块的指针等相关信息。<br />inode块(文件索引节点)：文件系统索引，记录文件的属性。它是文件系统的最基本单元，是文件系统连接任何子目录、任何文件的桥梁。每个子目录和文件只有唯一的一个 inode 块。它包含了文件系统中文件的基本属性(文件的长度、创建及修改时间、权限、所属关系)、存放数据的位置等相关信息. 在 Linux 下可以通过 “ls -li” 命令查看文件的 inode 信息。硬连接和源文件具有相同的 inode 。<br />数据块(Block) ：实际记录文件的内容，若文件太大时，会占用多个block。为了提高目录访问效率，Linux还提供了表达路径与inode对应关系的dentry结构。它描述了路径信息并连接到节点inode，它包括各种目录信息，还指向了inode和超级块。<br />就像一本书有封面、目录和正文一样。在文件系统中，超级块就相当于封面，从封面可以得知这本书的基本信息；inode 块相当于目录，从目录可以得知各章节内容的位置；而数据块则相当于书的正文，记录着具体内容。<br />Linux正统的文件系统(如ext2、3等)将硬盘分区时会划分出超级块、inode Table区块和data block数据区域。一个文件由一个超级块、inode和数据区域块组成。Inode包含文件的属性(如读写属性、owner等，以及指向数据块的指针)，数据区域块则是文件内容。当查看某个文件时，会先从inode table中查出文件属性及数据存放点，再从数据块中读取数据。<br />![2021-07-08-22-10-16-032455.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625754188692-7b8a40df-5bd5-428a-9d4e-22fc78195f24.png#averageHue=%23eeeeee&clientId=u5b1b95be-ce8f-4&from=ui&id=ub1addd18&originHeight=190&originWidth=362&originalType=binary&ratio=1&rotation=0&showTitle=false&size=47024&status=done&style=shadow&taskId=ub4624875-56c4-480e-9a02-f8ae8a1734e&title=)<br />将 inode与block区块用图解来说明一下，如下图所示，文件系统先格式化出inode与block的区块，假设某一个文件的属性与权限数据是放置到 inode 4号(下图较小方格内)，而这个inode记录了文件数据的实际放置点为 2, 7, 13, 15 这四个 block 号码，此时操作系统就能够据此来排列磁盘的阅读顺序，可以一口气将四个 block 内容读出来！那么数据的读取就如同下图中的箭头所指定的模样了。<br />![2021-07-08-22-10-16-299451.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625754200684-3449f847-9d65-4a08-a830-1381b2060453.png#averageHue=%2356e7e7&clientId=u5b1b95be-ce8f-4&from=ui&id=uc84d216c&originHeight=190&originWidth=379&originalType=binary&ratio=1&rotation=0&showTitle=false&size=80713&status=done&style=shadow&taskId=u75b6bded-f447-4812-8529-91834e7c10b&title=)<br />这种数据存取的方法称为索引式文件系统(indexed allocation)。那有没有其他的惯用文件系统可以比较一下啊？有的，那就是惯用的闪盘(闪存)，闪盘使用的文件系统一般为 FAT 格式。FAT 这种格式的文件系统并没有 inode 存在，所以 FAT 没有办法将这个文件的所有 block 在一开始就读取出来。每个 block 号码都记录在前一个 block 当中， 其读取方式有点像下图所示：<br />![2021-07-08-22-10-16-526461.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625754210177-531bc40d-efaa-43dd-a6f2-d55dd334059f.png#averageHue=%233be9e9&clientId=u5b1b95be-ce8f-4&from=ui&id=u3db9383b&originHeight=134&originWidth=360&originalType=binary&ratio=1&rotation=0&showTitle=false&size=54322&status=done&style=shadow&taskId=uffc9489c-499a-405d-a75b-45354b4fffe&title=)<br />上图中假设文件的数据依序写入1->7->4->15号这四个 block 号码中， 但这个文件系统没有办法一口气就知道四个 block 的号码，他得要一个一个的将 block 读出后，才会知道下一个 block 在何处。如果同一个文件数据写入的 block 分散的太过厉害时，则磁盘读取头将无法在磁盘转一圈就读到所有的数据， 因此磁盘就会多转好几圈才能完整的读取到这个文件的内容！<br />**常常会听到所谓的“碎片整理”吧？**需要碎片整理的原因就是文件写入的 block 太过于离散了，此时文件读取的效能将会变的很差所致。这个时候可以透过碎片整理将同一个文件所属的 blocks 汇整在一起，这样数据的读取会比较容易啊！想当然，FAT 的文件系统需要经常的碎片整理一下，那么 Ext2 是否需要磁盘重整呢？<br />由于 Ext2 是索引式文件系统，基本上不太需要常常进行碎片整理的。但是如果文件系统使用太久， 常常删除/编辑/新增文件时，那么还是可能会造成文件数据太过于离散的问题，此时或许会需要进行重整一下的。<br />可以用ln命令对一个已经存在的文件再建立一个新的连接，而不复制文件的内容。连接有软连接和硬连接之分，软连接又叫符号连接。它们各自的特点是：<br />硬连接：原文件名和连接文件名都指向相同的物理地址。目录不能有硬连接；硬连接不能跨越文件系统（不能跨越不同的分区）文件在磁盘中只有一个拷贝，节省硬盘空间；<br />由于删除文件要在同一个索引节点属于唯一的连接时才能成功，因此可以防止不必要的误删除。<br />符号连接：用ln -s命令建立文件的符号连接符号连接是linux特殊文件的一种，作为一个文件，它的数据是它所连接的文件的路径名。类似windows下的快捷方式。<br />可以删除原有的文件而保存连接文件，没有防止误删除功能。
<a name="qd8aT"></a>
### 3.6 文件系统在内核中的表示
内核数据结构<br />Linux内核的VFS子系统可以图示如下：<br />![2021-07-08-22-10-17-050458.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625754877748-af2e82b8-8e68-439c-a959-aee44c095ff7.png#averageHue=%23e9e9e9&clientId=u5b1b95be-ce8f-4&from=ui&id=u30b00a61&originHeight=606&originWidth=600&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1093251&status=done&style=shadow&taskId=uee856b6a-3162-4638-9a84-3394d9c6c1e&title=)<br />文件与IO：每个进程在PCB（Process Control Block）中都保存着一份文件描述符表，文件描述符就是这个表的索引，每个表项都有一个指向已打开文件的指针，现在明确一下：已打开的文件在内核中用file结构体表示，文件描述符表中的指针指向file结构体。<br />在file结构体中维护File Status Flag（file结构体的成员f_flags）和当前读写位置（file结构体的成员f_pos）。在上图中，进程1和进程2都打开同一文件，但是对应不同的file结构体，因此可以有不同的File Status Flag和读写位置。file结构体中比较重要的成员还有f_count，表示引用计数（Reference Count），后面会讲到，dup、fork等系统调用会导致多个文件描述符指向同一个file结构体，例如有fd1和fd2都引用同一个file结构体，那么它的引用计数就是2，当close(fd1)时并不会释放file结构体，而只是把引用计数减到1，如果再close(fd2)，引用计数就会减到0同时释放file结构体，这才真的关闭了文件。<br />每个file结构体都指向一个file_operations结构体，这个结构体的成员都是函数指针，指向实现各种文件操作的内核函数。比如在用户程序中read一个文件描述符，read通过系统调用进入内核，然后找到这个文件描述符所指向的file结构体，找到file结构体所指向的file_operations结构体，调用它的read成员所指向的内核函数以完成用户请求。在用户程序中调用lseek、read、write、ioctl、open等函数，最终都由内核调用file_operations的各成员所指向的内核函数完成用户请求。<br />file_operations结构体中的release成员用于完成用户程序的close请求，之所以叫release而不叫close是因为它不一定真的关闭文件，而是减少引用计数，只有引用计数减到0才关闭文件。对于同一个文件系统上打开的常规文件来说，read、write等文件操作的步骤和方法应该是一样的，调用的函数应该是相同的，所以图中的三个打开文件的file结构体指向同一个file_operations结构体。如果打开一个字符设备文件，那么它的read、write操作肯定和常规文件不一样，不是读写磁盘的数据块而是读写硬件设备，所以file结构体应该指向不同的file_operations结构体，其中的各种文件操作函数由该设备的驱动程序实现。<br />每个file结构体都有一个指向dentry结构体的指针，“dentry”是directory entry（目录项）的缩写。传给open、stat等函数的参数的是一个路径，例如/home/akaedu/a，需要根据路径找到文件的inode。为了减少读盘次数，内核缓存了目录的树状结构，称为dentry cache，其中每个节点是一个dentry结构体，只要沿着路径各部分的dentry搜索即可，从根目录/找到home目录，然后找到akaedu目录，然后找到文件a。dentry cache只保存最近访问过的目录项，如果要找的目录项在cache中没有，就要从磁盘读到内存中。<br />每个dentry结构体都有一个指针指向inode结构体。inode结构体保存着从磁盘inode读上来的信息。在上图的例子中，有两个dentry，分别表示/home/akaedu/a和/home/akaedu/b，它们都指向同一个inode，说明这两个文件互为硬链接。inode结构体中保存着从磁盘分区的inode读上来信息，例如所有者、文件大小、文件类型和权限位等。每个inode结构体都有一个指向inode_operations结构体的指针，后者也是一组函数指针指向一些完成文件目录操作的内核函数。<br />和file_operations不同，inode_operations所指向的不是针对某一个文件进行操作的函数，而是影响文件和目录布局的函数，例如添加删除文件和目录、跟踪符号链接等等，属于同一文件系统的各inode结构体可以指向同一个inode_operations结构体。<br />inode结构体有一个指向super_block结构体的指针。super_block结构体保存着从磁盘分区的超级块读上来的信息，例如文件系统类型、块大小等。super_block结构体的s_root成员是一个指向dentry的指针，表示这个文件系统的根目录被mount到哪里，在上图的例子中这个分区被mount到/home目录下。<br />file、dentry、inode、super_block这几个结构体组成了VFS的核心概念。对于ext2文件系统来说，在磁盘存储布局上也有inode和超级块的概念，所以很容易和VFS中的概念建立对应关系。而另外一些文件系统格式来自非UNIX系统（例如Windows的FAT32、NTFS），可能没有inode或超级块这样的概念，但为了能mount到Linux系统，也只好在驱动程序中硬凑一下，在Linux下看FAT32和NTFS分区会发现权限位是错的，所有文件都是rwxrwxrwx，因为它们本来就没有inode和权限位的概念，这是硬凑出来的。
<a name="TQvWk"></a>
### 3.7 挂载文件系统
linux系统中每个分区都是一个文件系统，都有自己的目录层次结构。linux会将这些分属不同分区的、单独的文件系统按一定的方式形成一个系统的总的目录层次结构。这里所说的“按一定方式”就是指的挂载。<br />将一个文件系统的顶层目录挂到另一个文件系统的子目录上，使它们成为一个整体，称为挂载。把该子目录称为挂载点.<br />例如要读取硬盘中的一个格式化好的分区、光盘或软件等设备时，必须先把这些设备对应到某个目录上，而这个目录就称为“挂载点（mount point）”，这样才可以读取这些设备。挂载后将物理分区细节屏蔽掉，用户只有统一的逻辑概念。所有的东西都是文件。<br />注意：<br />1、挂载点必须是一个目录。<br />2、一个分区挂载在一个已存在的目录上，这个目录可以不为空，但挂载后这个目录下以前的内容将不可用。<br />对于其他操作系统建立的文件系统的挂载也是这样。但是需要理解的是：光盘、软盘、其他操作系统使用的文件系统的格式与linux使用的文件系统格式是不一样的。光盘是ISO9660；软盘是fat16或ext2；windows NT是fat16、NTFS；windows98是fat16、fat32；windows2000和windowsXP是fat16、fat32、 NTFS。挂载前要了解linux是否支持所要挂载的文件系统格式。<br />挂载时使用mount命令，其格式：`mount [-参数] [设备名称] [挂载点]`<br />其中常用的参数有：<br />-t 指定设备的文件系统类型（什么提到的文件类型）<br />-o 指定挂载文件系统时的选项。有些也可用在/etc/fstab中。常用的有
```bash
codepage=XXX 代码页
iocharset=XXX 字符集
ro 以只读方式挂载
rw 以读写方式挂载
nouser 使一般用户无法挂载
user 可以让一般用户挂载设备
```
例如：

1. 挂载windows的文件系统：

1）首先使用`sudo fdisk -l`查看挂载的设备，例如最下面有：/dev/hda5<br />2）mkdir创建一个目录，这里的目录是作为挂在目录，就是要把E盘挂到这个目录下：`mk /mnt/winc`<br />3）windows和linux使用的不是一个文件系统，一般情况下linux不挂载windows文件系统，所以需要手动mount：
```bash
# mount -t vfat /dev/hda5 /mnt/winc （ -t vfat指出这里的文件系统fat32）
```
现在就可以进入/mnt/winc等目录读写这些文件了。

2. 挂载光盘：`# mk /mnt/cdrom`
```bash
# mount -t iso9660 /dev/cdrom /mnt/cdrom (关盘的名字一般都是cdrom，这条命令一般都通用)
```

3. 虚拟机共享文件夹：例如在VirtualBox下，主机是Windows，Ubuntu是Guest。共分三步：

1). 首先要安装虚拟电脑工具包：<br />在VirtualBox的菜单里选择"设备"->"安装虚拟电脑工具包"，可以发现在Ubuntu桌面上多出一个光盘图标，这张光盘默认被自动加载到了文件夹/media/cdom0，而且/cdrom自动指向这个文件夹。默认设置下文件管理器会自动打开这张光盘，可以看到里面有个"VBoxLinuxAdditions.run"文件。打开一个命令行终端，依次输入"cd /cdrom"和"`sudo sh ./VBoxLinuxAdditions.run`"，不含双引号，开始安装工具包。安装完毕，会用英文提示要重启Ubuntu，建议立刻重启。重启后，比较明显的变化是鼠标是共享模式，并且剪贴板也和Windows共享了。如果有这些变化，说明虚拟电脑工具包已经装成功。<br />2). 下一步设置共享文件夹。<br />在共享文件夹设置窗口中，单击右侧的”添加一个共享文件夹”，路径选择想要共享的Windows文件夹，共享名任取一个自己喜欢的，比如"myshare"，选项read-only是指是否只允许ubuntu读这个文件夹，请根据需要选择这个选项。<br />3). 在ubuntu下挂载这个共享文件夹：`sudo mount -t vboxsf myshare /media/share`<br />其中"myshare"是之前取的共享文件夹的名字，"/media/share"是要挂载到的目标文件.
<a name="w1dU7"></a>
### 3.8 自动挂载windows分区
每次开机访问windows分区都要运行mount命令显然太烦琐，为什么访问其他的linux分区不用使用mount命令呢？<br />其实，每次开机时，linux自动将需要挂载的linux分区挂载上了。那么是不是可以设定让linux在启动的时候也挂载到希望挂载的分区，如windows分区，以实现文件系统的自动挂载呢？<br />这是完全可以的。在/etc目录下有个fstab文件，它里面列出了linux开机时自动挂载的文件系统的列表。/etc/fstab文件如下：
```
/dev/hda2 / ext3 defaults 1 1
/dev/hda1 /boot ext3 defaults 1 2
none /dev/pts devpts gid=5,mode=620 0 0
none /proc proc defaults 0 0
none /dev/shm tmpfs defaults 0 0
/dev/hda3 swap swap defaults 0 0
/dev/cdrom /mnt/cdrom iso9660 noauto,codepage=936,iocharset=gb2312 0 0
/dev/fd0 /mnt/floppy auto noauto,owner,kudzu 0 0
/dev/hdb1 /mnt/winc vfat defaults,codepage=936,iocharset=cp936 0 0
/dev/hda5 /mnt/wind vfat defaults,codepage=936,iocharset=cp936 0 0
```
在/etc/fstab文件里，第一列是挂载的文件系统的设备名，第二列是挂载点，第三列是挂载的文件系统类型，第四列是挂载的选项，选项间用逗号分隔。第五六列不知道是什么意思，还望高手指点。<br />在最后两行是手工添加的windows下的C；D盘，加了codepage=936和iocharset=cp936参数以支持中文文件名。参数defaults实际上包含了一组默认参数：<br />rw 以可读写模式挂载<br />suid 开启用户ID和群组ID设置位<br />dev 可解读文件系统上的字符或区块设备<br />exec 可执行二进制文件<br />auto 自动挂载<br />nouser 使一般用户无法挂载<br />async 以非同步方式执行文件系统的输入输出操作<br />大家可以看到在这个列表里，光驱和软驱是不自动挂载的，参数设置为noauto。（如果非要设成自动挂载，要确保每次开机时光驱和软驱里都要有盘。)
<a name="ju7li"></a>
### 3.9 软连接、硬链接
可以用ln命令对一个已经存在的文件再建立一个新的连接，而不复制文件的内容。连接有软连接和硬连接之分，软连接又叫符号连接。它们各自的特点是：<br />硬连接：是给文件一个副本，原文件名和连接文件名都指向相同的物理地址。目录不能有硬连接；硬连接不能跨越文件系统（不能跨越不同的分区）文件在磁盘中只有一个拷贝，节省硬盘空间；<br />修改其中一个，与其连接的文件同时被修改。如果删除其中任意一个其余的文件将不受影响。<br />由于删除文件要在同一个索引节点属于唯一的连接时才能成功，因此可以防止不必要的误删除。<br />符号连接（软连接）：用ln -s命令建立文件的符号连接符号连接是linux特殊文件的一种，作为一个文件，它的数据是它所连接的文件的路径名。类似windows下的快捷方式。<br />当然删除这个连接，也不会影响到源文件，但对连接文件的使用、引用都是直接调用源文件的。<br />具体关系可以看下图：<br />![2021-07-08-22-10-17-342452.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625754331559-f3fa9798-5427-4afb-a407-a9b40e165c18.png#averageHue=%23caa07d&clientId=u5b1b95be-ce8f-4&from=ui&id=ue6827052&originHeight=364&originWidth=367&originalType=binary&ratio=1&rotation=0&showTitle=false&size=77582&status=done&style=shadow&taskId=u93a59335-6374-48cc-bddb-66ddda2b799&title=)<br />从图上可以看出硬链接和软链接的区别：<br />1：硬链接原文件和新文件的inode编号一致。而软链接不一样。<br />2：对原文件删除，会导致软链接不可用，而硬链接不受影响。<br />3：对原文件的修改，软、硬链接文件内容也一样的修改，因为都是指向同一个文件内容的。
<a name="qwrwl"></a>
### 3.10 文件目录管理命令
磁盘和文件空间 ：fdisk df du<br />文件目录与管理：cd pwd mkdir rmdir ls cp rm mv<br />查看文件内容 cat、tac、more、less、head 、tail<br />文件目录与权限 ：chmod chown chgrp umask<br />文件查找：which、whereis、locate、find、find
<a name="zhsjN"></a>
## 4、Linux 应用
标准的Linux系统一般都有一套都有称为应用程序的程序集，它包括文本编辑器、编程语言、X Window、办公套件、Internet工具和数据库等。
<a name="MHQS0"></a>
## 5、Linux 内核参数优化
内核参数是用户和系统内核之间交互的一个接口，通过这个接口，用户可以在系统运行的同时动态更新内核配置，而这些内核参数是通过Linux Proc文件系统存在的。因此，可以通过调整Proc文件系统达到优化Linux性能的目的。
