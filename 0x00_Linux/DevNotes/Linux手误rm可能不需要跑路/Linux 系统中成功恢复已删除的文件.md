当用户意外地删除了一个仍然需要的文件时，大多数情况下，是没有简便的方法可以重新找回或重建这个文件。不过，幸运的是文件是可以通过一些方法恢复的。当用户删除了一个文件，该文件并没有消失，只是被隐藏了一段时间。<br />这里将解释它是如何工作的。在一个文件系统中，有一个叫做 文件分配表 的东西，这个表跟踪文件在存储单元（如硬盘， MicroSD 卡，闪存驱动器等等）中的位置。<br />当一个文件被删除，文件系统将会在文件分配表中执行以下两个任务之一：这个文件在文件分配表上的条目被标记为 “自由空间” 或删除文件分配表里这个文件的条目，且将相应的空间被标记为自由空间 。<br />现在，如果有一个新的文件需要被放置在一个存储单元上，操作系统将会把这个文件放置到标记为空位的地方。在新文件被写入到这个空位后，被删除的文件就彻底消失了。<br />当需要恢复一个已经删除的文件时，用户绝对不能再对任何文件进行操作，因为假如该文件对应的“空位”被占用，这个文件就永远也不能恢复了。
<a name="jV7Kf"></a>
### 恢复软件是如何工作的？
大多数的文件系统（在删除文件时）只是标记空间为空白。在这些文件系统下，恢复软件查看文件分配表这个文件，然后复制被删除的文件到另外的存储单元中。假如该文件被复制到其它需要恢复的被删除的存储单元中，那么用户将有可能会失去那个所需的删除文件。<br />文件系统很少会擦除文件分配表中的条目。假如文件系统真的这样做了， 这便是恢复软件在恢复文件了。恢复软件在存储单元中扫描文件头，所有文件都拥有一个特殊的编码字符串，它们位于文件的最前面，也被叫做 魔法数字。例如，一个编译的 JAVA 类文件的魔法数字在十六进制中是“CAFEBABE”。<br />所以，假如要恢复该类型的文件，恢复软件会查找 “CAFEBABE” 然后复制文件到另一个存储单元。一些恢复软件可以查找某种特殊的文件类型。若用户想恢复一个 PDF 文件，则恢复软件将会查找十六进制的魔法数字 “25504446”，这恰恰是 ASCII 编码中的 “%PDF”。恢复软件将会查找所有的魔法数字，然后用户可以选择恢复哪个已删除的文件。<br />假如一个文件的部分被覆写了，则整个文件就会被损坏。通常这个文件可以被恢复，但是其中的内容可能已经没有什么用处。例如，恢复一个已损坏的 JPEG 文件将会是无意义的，因为图片查看器不能从这个损坏的文件产生一幅图片。因此，即使用户拥有了这个文件，该文件也将毫无用处。
<a name="cGcab"></a>
### 设备的位置
在继续之前，下面的一些信息将会对指引恢复软件找到正确的存储单元起到一定的帮助。所有的设备均挂载在 /dev/ 目录下。操作系统赋予每个设备的名称（并不是管理员给予每个分区或设备的名称）遵循一定的命名规律。<br />第一个 SATA 硬盘的第二个分区的名称将会是 sda2。名称的第一个字母暗示了存储类型，在这里指的是 SATA，但字母 “s” 也可能指的是 SCSI、 FireWire（火线端口）或 USB。第二个字母 “d” 指的是 disk(硬盘)。第三个字母指的是设备序数，即字母 “a” 指的是第一个 SATA 而 “b” 指的是第二个。最后的数字代表分区。没有分区数字的设备名代表该设置的所有分区。对于上面的例子，对应的名称为 sda 。作为命名的第一个字母还可能是 “h” ，这对应 PATA 硬盘（IDE）。<br />以下为命名规律的一些例子。假如一个用户有一个 SATA 硬盘（sda），这个设备有 4 个分区- sda1、 sda2、 sda3 和 sda4 。该用户删除了第三个分区，但直到格式化第四个分区之前，第四个分区名 sda4 都将保留不变。然后该用户插入了一个带有一个分区 - 即sdb1- 的 usb 存储卡（sdb），又增加了一个带有一个分区 -hda1- 的 IDE 硬盘 ，接着该用户又增加了一个 SCSI 硬盘 - sdc1 。接着用户移除了 USB 存储卡（sdb）。<br />现在，SCSI 硬盘的名称仍然为 sdc，但如果这个 SCSI 被移除接着再被插入，则它的名称将变为 sdb。虽然还有其他的存储设备存在， 那个 IDE 硬盘的名称仍会有一个 “a”， 因为它是第一个 IDE 硬盘，IDE 设备的命名与 SCSI、 SATA、 FireWire 和 USB 设备要分开计数。
<a name="Ex78W"></a>
### 使用 TestDisk 进行恢复：
每个恢复软件有其不同的功能，特征及支持的不同文件系统。下面是一些关于 使用 TestDisk 在各种文件系统中恢复文件的指南。
<a name="kiH3e"></a>
#### FAT16、 FAT32、 exFAT (FAT64)、 NTFS 以及 ext2/3/4：
TestDisk 是一个运行在 Linux、 *BSD、 SunOS、 Mac OS X、 DOS 和 Windows 等操作系统下的开源的自由软件。TestDisk 可以从下面的链接中找到 ：[http://www.cgsecurity.org/wiki/TestDisk](http://www.cgsecurity.org/wiki/TestDisk)。<br />TestDisk 也可以通过键入 `sudo apt-get install testdisk` 来安装。TestDisk 有着许多的功能，但这篇文章将只关注恢复文件这个功能。<br />使用 root 权限从终端中打开 TestDisk 可以通过键入 sudo testdisk 命令。<br />现在， TestDisk 命令行应用将会被执行。终端的显示将会改变。TestDisk 询问用户它是否可以保留日志，这完全由用户决定。假如一个用户正从系统存储中恢复文件，则不必保留日志。可选择的选项有“生成”、 “追加” 和 “无日志”。假如用户想保留日志，则日志将会保留在该用户的主目录。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1676528215898-07dd4e7f-352f-49c8-a2ff-b23fb14c20d4.jpeg#averageHue=%23373b3e&clientId=u793ab247-1ed7-4&from=paste&id=sVAy6&originHeight=386&originWidth=699&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5a9844fb-787e-4f65-882e-e25660a93e3&title=)<br />在接着的屏幕中，存储设备以 /dev/*的方式被罗列出来。对于系统，系统的存储单元为 /dev/sda，这意味着存储单元为 一个 SATA硬盘（sd）且它是第一个硬盘（a）。每个存储单元的容量以 Gigabyte（千兆字节）为单位显示的。使用上下键来选择一个存储设备然后点击进入。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1676528215903-0dd6e41a-7c42-4bed-8aa8-f5b0b532cabe.jpeg#averageHue=%23363a3d&clientId=u793ab247-1ed7-4&from=paste&id=sMlhC&originHeight=296&originWidth=674&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufda9da95-1652-4ce8-a823-ec4741f5b55&title=)<br />下一屏显示出一个列有分区表（也叫做分区映射表）的清单。正如文件有文件配置表，分区有着分区表。分区是存储设备上的分段。例如在几乎所有的 Linux 系统中，至少存在两种分区类型 - EXT3/4 和 Swap 。每一个分区表将会在下面被简要地描述。TestDisk 并不支持所有类型的分区表，所以这并不是完整的列表。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1676528215849-bf7b7af3-6efc-49b5-975e-9dde5c7bc075.jpeg#averageHue=%2334383b&clientId=u793ab247-1ed7-4&from=paste&id=fDIg2&originHeight=419&originWidth=705&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5f88b2e8-244e-45fc-b1bf-db726f9123e&title=)

- **Intel** - 这类分区表在 Windows 系统和许多的 Linux 系统中非常普遍，它也常常称作 MBR 分区表。
- **EFI GPT** - 这种类型的分区表通常用在 Linux 系统中。对于 Linux 系统，这种分区表是最为推荐的， 因为逻辑分区或扩展分区的概念并不适用于 GPT (GUID Partition Table) 分区表。这意味着，如果每个分区中有一个 Linux 系统，一个 Linux 用户可以从多种类型的 Linux 系统中进行多重启动。当然使用 GPT 分区表还有其他的优势，但那些已超出了本文的讨论范围。
- **Humax** - Humax 分区映射表适用于韩国公司 Humax 生产的设备。
- **Mac** - Apple 分区映射表 (APM) 适用于 Apple 的设备。
- **None** - 某些设备并没有分区表。例如，许多 Subor 游戏控制台不使用分区映射表。如果一个用户试图以其它分区表类型从这类设备中恢复文件，用户就会困扰 TestDisk 为何找卟到任何的文件系统或者文件。
- **Sun** - Sun 分区表适用于 Sun 系统。
- **Xbox** -Xbox 适用于使用 Xbox 分区映射表的存储设备。

假如用户选择了 “Xbox” ，尽管他的系统使用了 GPT 分区表， 那么 TestDisk 将不能找到任何分区或文件系统。假如 TestDisk 按照用户的选择执行，则它可能猜测错误。（下面的图片显示的是当分区表类型错误时的输出）<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1676528215830-06300bd2-2ab3-4b77-942e-2e833c0d7e38.jpeg#averageHue=%2335393c&clientId=u793ab247-1ed7-4&from=paste&id=isTE6&originHeight=224&originWidth=470&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u94f32691-c0c8-4ca1-adaf-78c39540bf2&title=)<br />当用户为他们的设备选择了正确的选项，则在下一屏中，选择 “高级” 选项。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1676528215967-7f710dc5-1a4e-466f-be71-9c70fa73db83.jpeg#averageHue=%23353a3c&clientId=u793ab247-1ed7-4&from=paste&id=oGpDa&originHeight=278&originWidth=731&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u069591ba-0d91-4af3-9c30-2faa8ca40eb&title=)<br />现在，用户将看到一个列有用户存储设备中所有的文件系统或分区的列表。假如用户选择了错误的分区映射表，则在这一步中用户就将会知道他们做出了错误的选择。假如没有错误，通过移动文字光标来高亮选择含有被删除文件的分区。使用 左右键来高亮位于终端底部的 “列表”。接着，按下回车确认。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1676528216318-6e9c2346-6f57-4713-8791-b2ee1177ee36.jpeg#averageHue=%23383d3f&clientId=u793ab247-1ed7-4&from=paste&id=Fmg0w&originHeight=323&originWidth=651&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf64df500-1600-4338-98bb-c9b3dabe61a&title=)<br />新的一屏便会呈现出列有文件和目录的列表。那些白色的文件名就是未被删除的文件，而红色的文件名是那些已被删除的文件。最右边的一列是文件的名称，从右到左方向的接着一列是文件的创建日期，再往左的一列是文件的大小（以 byte/ 比特为单位），最左边带有“-”，“d” ,“r”, “w” 和"x"的一列则代表的是文件的权限情况。“d” 表示该文件为一个目录，其他的权限术语与本文关系不大。在列表的最顶端以“.”代表的一项表示当前目录，第二行以".."代表的一项表示当前目录的上级目录，所以用户可以通过选择目录所在行到达该目录。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1676528216336-4cac0663-e077-47ba-ab7e-cb4476cab5c2.jpeg#averageHue=%23393e40&clientId=u793ab247-1ed7-4&from=paste&id=sXcB7&originHeight=743&originWidth=662&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub8f1bd44-1b33-4130-bee1-5adb915a0ce&title=)<br />举个例子，比如想进入"Xaiml_Dataset" 目录，该目录基本上由被删除的文件组成。通过按键盘上的 "c"键，将恢复文件 "computers.xaiml"，接着被询问选择一个目标目录，当然，应该放置该文件到另一个分区中。现在，当在家目录时，按下了“c”键。（选择目标目录时）哪个目录被高亮并没有什么影响，当前目录就是目标目录，在屏幕的上方，将会显示“复制完成”的消息。在家目录中便会有一个名为"Xaiml_Dataset"的目录，里面里有一个 Xaiml 文件。假如在更多的已删除文件上按“c” 键，则这些文件将会被放置到新的文件夹中而无需再询问目标目录。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1676528216265-0c2ff27f-0de4-4bbf-b461-bdb3ba7e899b.jpeg#averageHue=%23373c3e&clientId=u793ab247-1ed7-4&from=paste&id=znOgk&originHeight=458&originWidth=743&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u85098264-393d-451e-b68d-fb32e9aef62&title=)<br />当这些步骤完成后，重复按“q”键直到看到正常的终端模样。目录"Xaiml_Dataset" 只能被 root 用户访问。为了解决这个问题，使用 root 权限改变该目录及其子目录的权限。做完这些后，文件便被恢复了且用户可以访问它们。
<a name="LHd36"></a>
### 特别的 ReiserFS
为了从 ReiserFS 文件系统中恢复一个文件，首先需将分区中的所有文件做一个备份。因为如果发生某些错误， 这个方法可能会引起文件丢失。接着执行下面的命令，其中 DEVICE指的是那些以 sda2 形式命名的设备。一些文件将被放入 lost+found 目录而其他则会保存到原先被删除的位置。
```bash
reiserfsck --rebuild-tree --scan-whole-partition /dev/DEVICE
```
<a name="iwi6I"></a>
### 恢复被某个程序打开的删除文件
假设用户意外地删除了一个文件，且该文件被某个程序打开。虽然在硬盘中该文件被删除了，但这个程序正使用着位于 RAM 中的该文件的副本。幸好，有两种简单的解决方法来恢复该文件。<br />假如这个软件有保存功能，如文本编辑器，则用户可以重新保存该文件，这样，文本编辑器可以将该文件写入硬盘中。<br />假设在音乐播放器中有一个 MP3 文件，而该音乐播放器并不能保存该 MP3 文件，则这种情形下需要比先前花更多的时间来恢复文件。不幸的是，这种方法并不能保证在所有的系统和应用中有效。首先，键入下面的命令。
```bash
lsof -c smplayer | grep mp3
```
上面的命令会列出所有由 smplayer 使用的文件，这个列表由 grep 命令通过管道搜索 mp3 。命令的输入类似于下面：
```bash
smplayer  10037 collier  mp3    169r      8,1  676376  1704294 /usr/bin/smplayer
```
现在，键入下面的命令来直接从 RAM（在 Linux 系统中，/proc/映射到 RAM）中恢复文件，并复制该文件到选定的文件夹中。其中 cp 指的是复制命令，输出中的数字 10037 来自于进程数，输出中的数字 169 指的是文件描述符，"~/Music/"为目标目录，最后的 "music.mp3" 为用户想恢复的文件的名称。
```bash
cp /proc/10037/fd/169 ~/Music/music.mp3
```
<a name="xVBfL"></a>
### 真正的删除
为确保一个文件不能被恢复，可以使用一个命令来 “擦除” 硬盘。擦除硬盘实际上是向硬盘中写入无意义的数据。例如，许多擦除程序向硬盘中写入零，随机字母或随机数据。不会有空间被占用或丢失，擦除程序只是对空位进行重写覆盖。假如存储单元被文件占满而没有空余空间，则所有先前被删除的文件将会消失而不能恢复。<br />擦除硬盘的目的是确保隐私数据不被他人看见。举个例子，一个公司可能预订了一些新的电脑，总经理决定将旧的电脑卖掉，然而，新的电脑拥有者可能会看到公司的一些机密或诸如信用卡号码，地址等顾客信息。幸好，公司的电脑技术人员可以在卖掉这些旧电脑之前，擦除这些硬盘。<br />为了安装擦除程序 secure-delete，键入 `sudo apt-get install secure-delete`，这个命令将会安装一个包含 4 个程序的程序集，用以确保被删除的文件不能被恢复。

- srm - 永久删除一个文件。使用方法：`srm -f ./secret_file.txt`
- sfill - 擦除空白空间。使用方法：`sfill -f /mount/point/of/partition`
- sswap - 擦除 swap 空间。使用方法：`sswap -f /dev/SWAP_DEVICE`

假如电脑实际去清除那些删除的文件，那么就需要花费更长的时间去执行删除任务。将某些空间标记为空位是快速且容易的，但使得文件永远消失需要花费一定的时间。例如，擦除一个存储单元，可能需要花费几个小时的时间（根据磁盘容量大小）。<br />总之，现在的系统工作的就挺好，因为即便用户清空了垃圾箱，他们仍然有另一次机会来改变他们当初的想法（或错误）。
