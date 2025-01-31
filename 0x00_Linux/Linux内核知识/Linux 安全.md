Linux 安全<br />Linux 作为 MINIX 和 UNIX 的衍生操作系统，从一开始就是一个`多用户`系统。这意味着 Linux 从早期开始就建立了安全和信息访问控制机制。下面主要探讨的就是 Linux 安全性的一些内容
<a name="23Btx"></a>
### Linux 安全基本概念
一个 Linux 系统的用户群里由一系列注册用户组成，他们每一个都有一个唯一的 UID (User ID)。一个 UID 是一个位于 0 到 65535 之间的整数。文件（进程或者是其他资源）都标记了它的所有者的 UID。默认情况下，文件的所有者是创建文件的人，文件的所有者是创建文件的用户。<br />用户可以被分成许多组，每个组都会由一个 16 位的整数标记，这个组叫做 `GID(组 ID)`。给用户分组是手动完成的，它由系统管理员执行，分组就是在数据库中添加一条记录指明哪个用户属于哪个组。一个用户可以属于不同组。<br />Linux 中的基本安全机制比较容易理解，每个进程都会记录它所有者的 UID 和 GID。当文件创建后，它会获取创建进程的 UID 和 GID。当一个文件被创建时，它的 UID 和 GID 就会被标记为进程的 UID 和 GID。这个文件同时会获取由该进程决定的一些权限。这些权限会指定所有者、所有者所在组的其他用户及其他用户对文件具有什么样的访问权限。对于这三类用户而言，潜在的访问权限是 **「读、写和执行」**，分别由 r、w 和 x 标记。当然，执行文件的权限仅当文件时可逆二进制程序时才有意义。试图执行一个拥有执行权限的非可执行文件，系统会报错。<br />**「Linux 用户分为三种」**

- `root（超级管理员）`，它的 UID 为 0，这个用户有极大的权限，可以直接无视很多的限制 ，包括读写执行的权限。
- `系统用户`，UID 为 1～499。
- `普通用户`，UID 范围一般是 500～65534。这类用户的权限会受到基本权限的限制，也会受到来自管理员的限制。不过要注意 nobody 这个特殊的帐号，UID 为 65534，这个用户的权限会进一步的受到限制，一般用于实现来宾帐号。

Linux 中的每类用户由 3 个比特为来标记，所以 9 个比特位就能够表示所有的权限。<br />下面来看一下一些基本的用户和权限例子

| 二进制 | 标记 | 准许的文件访问权限 |
| :--- | :--- | :--- |
| 111000000 | rwx------ | 所有者可读、写和执行 |
| 111111000 | rwxrwx--- | 所有者和组可以读、写和执行 |
| 111111111 | rwxrwxrwx | 所有人可以读、写和执行 |
| 000000000 | --------- | 任何人不拥有任何权限 |
| 000000111 | ------rwx | 只有组以外的其他用户拥有所有权 |
| 110100100 | rw-r--r-- | 所有者可以读和写，其他人可以读 |
| 110100100 | rw-r----- | 所有者可以读和写，组可以读 |

上面提到，UID 为 0 的是一个特殊用户，称为 `超级用户(或者根用户)`。超级用户能够读和写系统中的任何文件，不管这个文件由谁所有，也不管这个文件的保护模式如何。UID 为 0 的进程还具有少数调用受保护系统调用的权限，而普通用户是不可能有这些功能的。通常情况下，只有系统管理员知道超级用户的密码。<br />在 Linux 系统下，目录也是一种文件，并且具有和普通文件一样的保护模式。不同的是，目录的 x 比特位表示查找权限而不是执行权限。因此，如果一个目录的保护模式是 `rwxr-xr-x`，那么它允许所有者读、写和查找目录，而其他人只可以读和查找，而不允许从中添加或者删除目录中的文件。<br />与 I/O 有关的特殊文件拥有和普通文件一样的保护位。这种机制可以用来限制对 I/O 设备的访问权限。举个例子，打印机是特殊文件，它的目录是 `/dev/lp`，它可以被根用户或者一个叫守护进程的特殊用户拥有，具有保护模式 rw-------，从而阻止其他所有人对打印机的访问。毕竟每个人都使用打印机的话会发生混乱。<br />当然，如果 /dev/lp 的保护模式是 rw-------，那就意味着其他任何人都不能使用打印机。<br />这个问题通过增加一个保护位 `SETUID` 到之前的 9 个比特位来解决。当一个进程的 SETUID 位打开，它的 `有效 UID` 将变成相应可执行文件的所有者 UID，而不是当前使用该进程的用户的 UID。将访问打印机的程序设置为守护进程所有，同时打开 SETUID 位，这样任何用户都可以执行此程序，而且拥有守护进程的权限。<br />除了 SETUID 之外，还有一个 SETGID 位，SETGID 的工作原理和 SETUID 类似。但是这个位一般很不常用。
<a name="Xrt9E"></a>
### Linux 安全相关的系统调用
Linux 中关于安全的系统调用不是很多，只有几个，如下列表所示

| 系统调用 | 描述 |
| :--- | :--- |
| chmod | 改变文件的保护模式 |
| access | 使用真实的 UID 和 GID 测试访问权限 |
| chown | 改变所有者和组 |
| setuid | 设置 UID |
| setgid | 设置 GID |
| getuid | 获取真实的 UID |
| getgid | 获取真实的 GID |
| geteuid | 获取有效的 UID |
| getegid | 获取有效的 GID |

在日常开发中用到最多的就是 `chmod`了，chmod 之前一直认为是改变权限，现在专业一点是改变文件的保护模式。它的具体函数如下
```c
s = chmod("路径名","值");
```
例如
```c
s = chmod("/usr/local/cxuan", 777);
```
他就是会把 `/usr/local/cxuan` 这个路径的保护模式改为 rwxrwxrwx，任何组和人都可以操作这个路径。只有该文件的所有者和超级用户才有权利更改保护模式。<br />`access` 系统调用用来检验实际的 UID 和 GID 对某文件是否拥有特定的权限。下面就是四个 getxxx 的系统调用，这些用来获取 uid 和 gid 的。
> 注意：其中的 chown、setuid 和 setgid 是超级用户才能使用，用来改变所有者进程的 UID 和 GID。

<a name="EmGEg"></a>
### Linux 安全实现
当用户登录时，登录程序，也被称为 `login`，会要求输入用户名和密码。它会对密码进行哈希处理，然后在 `/etc/passwd` 中进行查找，看看是否有匹配的项。使用哈希的原因是防止密码在系统中以非加密的方式存在。如果密码正确，登录程序会在 /etc/passwd 中读取用户选择的 shell 程序的名称，有可能是 `bash`，有可能是 `shell` 或者其他的 `csh` 或 `ksh`。然后登录程序使用 setuid 和 setgid 这两个系统调用来把自己的 UID 和 GID 变为用户的 UID 和 GID，然后它打开键盘作为标准输入、标准输入的文件描述符是 0 ，屏幕作为标准输出，文件描述符是 1 ，屏幕也作为标准错误输出，文件描述符为 2。最后，执行用户选择的 shell 程序，终止。<br />当任何进程想要打开一个文件，系统首先将文件的 i - node 所记录的保护位与用户有效 UID 和 有效 GID 进行对比，来检查访问是否允许。如果访问允许，就打开文件并返回文件描述符；否则不打开文件，返回 - 1。<br />Linux 安全模型和实现在本质上与大多数传统的 UNIX 系统相同。
