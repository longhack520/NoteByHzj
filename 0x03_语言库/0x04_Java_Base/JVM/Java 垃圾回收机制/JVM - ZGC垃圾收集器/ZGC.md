Java JVM<br />Java长期支持版本Java11发布时推出了ZGC，之后的Java12 -16均有ZGC的一些更新。然而Java17的新特性已经敲定，确定没有关于ZGC的相关更新，也从另一方面说明了至Java17 ZGC已经比较稳定，可以作为比较稳定的GC垃圾回收器。相信未来ZGC必定会成为主流的Java垃圾回收期。
<a name="k6GMq"></a>
## 1、ZGC从何而来，有什么好处
<a name="WeUTu"></a>
### 1.1 ZGC 的命名
根据OpenJDK官方网站的说明ZGC其实并没有什么特殊意义，就是一个名字而已。起初只是为了致敬ZFS 文件系统，表示ZGC与ZFS一样都是革命性的，是一个跨时代的产品。更像是一种崇拜命名法。所以ZGC就是要做革命性的与以往的垃圾回收器性能上有很大提高的GC。
<a name="MhxXK"></a>
### 1.2ZGC的好处及特点
ZGC是一个低延迟的可扩展GC，它有以下的目标
<a name="SyMWi"></a>
#### 1. 下图是SPEC做出的测试对比，Java16中ZGC 已经可做到亚毫秒级别的延迟了
亚毫秒级别的时间延迟 (就是不超过1毫秒。)<br />![2021-08-27-16-54-24-263831.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630054581538-e5c36abd-4a1b-4dc5-a54d-798a6daf6e7c.png#averageHue=%23faece7&clientId=u0e9e5690-5080-4&from=ui&id=ue63c6466&originHeight=448&originWidth=908&originalType=binary&ratio=1&rotation=0&showTitle=false&size=23338&status=done&style=shadow&taskId=u05450c82-aa63-48cd-a82c-179274e2b2a&title=)
<a name="MehFB"></a>
#### 2. 暂停时间不会随着堆的大小、存活集、根集的大小的增加而增加
<a name="dRrEU"></a>
#### 3. 支持8MB-16TB级别的堆大小
同时ZGC有如下标签：

- 并发
- 基于Region
- 堆压缩
- 彩色指针 (对象使用64的指针，有44位表示对象内存地址（如果是8TB ， 4TB堆内存则会用到43,42位）  ，其中有4位用于GC，其余为备用 )，个别详情请看 http://hg.openjdk.java.net/jdk/jdk/rev/24f6b0e413a0

![2021-08-27-16-54-24-392826.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630054581536-bbe45e2b-bb92-4e0a-b070-e04e46460332.png#averageHue=%23ffffff&clientId=u0e9e5690-5080-4&from=ui&id=fpWsI&originHeight=254&originWidth=581&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1705&status=done&style=shadow&taskId=u2fa33a07-fd15-4f14-9f29-2a4176d1b9f&title=)

- 负载屏障
- NUMA支持 （每个CPU单独访问一块内存）
<a name="MKdtB"></a>
## 2、ZGC如何使用及调优
<a name="WzKM2"></a>
### 2.1 ZGC支持那些系统
![2021-08-27-16-54-24-505828.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630054581544-6d926a1e-0ce3-4274-8d36-c0ae23f82533.png#averageHue=%23eac995&clientId=u0e9e5690-5080-4&from=ui&id=zF1Ow&originHeight=190&originWidth=716&originalType=binary&ratio=1&rotation=0&showTitle=false&size=4936&status=done&style=shadow&taskId=u53875652-39aa-4a14-83ee-87987c86621&title=)<br />直至Java17 ZGC基本支持所有的主流系统。
<a name="sWYAi"></a>
### 2.2 ZGC可用的调优参数
尽管ZGC有比较大的改动，但是它仍然可以使用绝大部分的GC参数。当然它也有其特有的参数。
<a name="CqoFn"></a>
#### 2.2.1 ZGC 通用的参数
-XX:MinHeapSize, -Xms 最小堆大小 (default = 8388608 = 8M) <br />-XX:InitialHeapSize, -Xms 初始化堆大小 (default = 134217728 = 128M )<br />-XX:MaxHeapSize, -Xmx 最大堆大小  (default = 2134900736  = 2036M) <br />-XX:SoftMaxHeapSize JVM堆的最大软限制 (default = 2134900736  = 2036M) <br />-XX:ConcGCThreads 并发GC的线程数量（default  -XX:+ConcGCThreads=1 ) <br />-XX:ParallelGCThreads  设置垃圾回收时的并行GC线程数量 (default = 4 ) <br />-XX:UseLargePages 使用大页面内存 (dafault  false) <br />-XX:UseTransparentHugePages 使用Transparent大页面内存 <br />-XX:UseNUMA  使用UNMA内存分配，可以获得更好的性能<br />-XX:SoftRefLRUPolicyMSPerMB 每MB的空闲内存空间允许软引用对象存活时间（default = 1000） <br />-XX:AllocateHeapAt = 堆分配参数，可以使用非DRAM 内存，这个参数将指向文件系统的文件并使用内存映射来达到在备用存储设备上进行堆分配。
<a name="u3xR8"></a>
#### 2.2.2 ZGC 特有的参数
-XX:ZAllocationSpikeTolerance 修正系数，数值越大，越早触发GC (default = 2.000000) <br />-XX:ZCollectionInterval   ZGC发生的最小时间间隔 ，秒 (default = 0.000000) <br />-XX:ZFragmentationLimit  relocation时，当前region碎片化大于此值，则回收region (default = 25.000000) -XX:ZMarkStackSpaceLimit  指定为标记堆栈分配的最大字节数 (default = 8589934592 = 8096M)<br />-XX:ZProactive   是否启用主动回收 (default   true) <br />-XX:ZUncommit  是否归还不使用的内存给OS(default   true)<br />-XX:ZUncommitDelay  不再使用的内存最多延迟多久会归还给OS (default  = 300 s)
<a name="VCX0H"></a>
#### 2.2.3 ZGC的一些诊断参数
-XX:+UnlockDiagnosticVMOptions 使用诊断模式，下面的参数才会起作用<br />-XX:ZStatisticsInterval 指定统计数据输出之间的时间间隔(秒)。<br />-XX:ZVerifyForwarding 检验转发表 -XX:ZVerifyMarking 检验标记集 <br />-XX:ZVerifyObjects 检验对象 -XX:ZVerifyRoots 检验根节点 <br />-XX:ZVerifyViews 检验堆视图访问
<a name="FPc0P"></a>
### 2.3 启用ZGC
-XX:+UseZGC  启用ZGC<br />-XX:+UseZGC -Xmx-Xlog:gc<br />-XX:+UseZGC -Xmx-Xlog:gc*  可以打印更详细的GC日志
<a name="jCvjW"></a>
### 2.4 可优化参数详解
<a name="iASGi"></a>
#### 2.4.1 堆参数的设置
ZGC比较重要的调优参数是设置最大堆内存(`-Xmx<size>`).， ZGC设置一个最大堆内存有两个考量：1.堆可以容纳程序的存活集  2.堆中要有足够的空间允许GC运行时分配.<br />然而多大内存合适这就要根据 内存使用情况以及GC频率来确定。
<a name="JhgXa"></a>
#### 2.4.2 并发GC线程数设置
另一个调优参数就是并发GC线程数量的设置 (`-XX:ConcGCThreads=<number>`) 。<br />一般情况下ZGC可以自动选择一个合适的值，但是也要根据程序的特点来修改。这个参数对GC使用CPU的时间影响比较大，如果数量太大的话，GC线程会占用过多的CPU时间，数量过少的话垃圾回收有会不及时。通常情况下如果程序的低延迟很重要，那么CPU使用率最好永远不要超过70%。
<a name="cUF2j"></a>
#### 2.4.3 将不用的内存返回给OS
默认情况下，ZGC会将不使用的内存还给OS。可以使用XX:-ZUncommit 取消这一功能。然而如果实际堆内存比最小堆内存都小的话，肯定不会将不使用的内存返还给OS，另外最大堆内存(-Xmx)和最小堆内存 (-Xms) 的设置相等的话，也不会起作用。<br />另外可以设置延迟归还对内存的时间 `-XX:ZUncommitDelay=<seconds>`
<a name="EtrkX"></a>
#### 2.4.4  在Linux中开启大页支持
在Linux操作系统中，采用内存映射来管理内存，逻辑页面映射到对应的物理内存，<br />使用大页面这个参数在吞吐量，低延迟，启动时间方面都有很好的性能提升，可以说除了设置起来有点复杂之外，没有任何的缺点。设置这个参数需要Root权限，所以默认没有开启。
<a name="Da169"></a>
#### 2.4.5 Linux中使用大透明页
Linux下的大页分为两种标准，大页（Huge Pages）和透明大页（Transparent Huge Pages）。大页是预分配的方式管理，透明大页是动态分配的方式。可以使用这个参数`-XX:+UseTransparentHugePages` 来使用 transparent huge pages .对延迟要求很高的程序而言，这个参数是不推荐使用的，它可能会造成不必要的延迟峰值。
<a name="zXkyu"></a>
#### 2.4.6 配置NUMA的支持
NUMA（非一致性内存访问）是Intel为了解决SMP多CPU结构中中线访问方式中资源竞争和一致性问题引起的。ZGC支持NUMA，此特性是默认开启的，它可以自主判断系统是否支持NUMA。如果在支持NUMA的机器上运行，那么将有显著的性能提升。
<a name="VOnMD"></a>
## 3、ZGC垃圾回收流程简述
3.1 ZGC同G1类似，也将内存分为Regions， 叫做ZPages。ZPages可以动态的创建和销毁，而且还可以动态的变化大小，无论怎么变化均为2MB 的倍数。<br />ZGC的堆区可以分为Small(2MB) Medium(32MB) Large(N*2MB)等多个Heap Regions. 其中Medium和Large是连续分配的内存。<br />ZGC的垃圾回收一般可以分为以下几个步骤流程。

1. Pause Mark Start这个阶段会有STW，主要对Root集进行扫描，标记Root集中存活对象，即标记根可达对象。
2. Concurrent Mark/Remap并发标记阶段，根据上一步标记的根对象去遍历对象图，并标记存活对象，主要根据上一步的Root存活对象标记其对象树上的对象。
3. Pause Mark End标记结束，此处有STW，算再标记阶段，由于之前标记的时候程序也在运行，此时会有对象引用的变更，此处会对那些变更引用的对象重新标记一下。此时也会清理一些Root对象以及无效空间。
4. Concurrent Prepare for Relocate并发标记Region及处理弱根对象 ，标记需要整理的Region集合，并选择下一步Relocation要用到的Region集合，也会处理不是强根的对象。
5. Pause Relocate Start开始Relocate，此处有STW，处理Root对象的指针，此时会将Roots的对象移动到选好的Region集合中。
6. Concurrent Relocate并发Relocate，此时并发执行，将存活对象移动到上一步选好的新的Region集合中。然后就是清理被释放的分区，等等工作。

可以看出来ZGC整体使用了标记复制加整理的思想，不过ZGC可以根据当前的内存使用情况，选择将存活对象整理到一个全新的空Region集合中或者某个存有对象的Region中。由于整个GC的流程中只有三处需要暂停，然而这三处的STW时间都是亚毫秒级别。整个GC的流程延迟总体会小于1ms。<br />![2021-08-27-16-54-24-636852.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630054612831-7a0c326f-f9ba-458f-a45c-14c26891c5ff.png#averageHue=%23fefefe&clientId=u0e9e5690-5080-4&from=ui&id=ua2e84805&originHeight=445&originWidth=879&originalType=binary&ratio=1&rotation=0&showTitle=false&size=20597&status=done&style=shadow&taskId=uc5d59bcf-9d9a-40ea-8c7b-fdb8dcb6d5b&title=)
<a name="Mez1T"></a>
## 总结
总体概括了介绍了比较成熟的ZGC，介绍了ZGC的一些重要调优参数，讲解了一下ZGC的垃圾回收流程。相信大家也有所收获。目前ZGC的应用比较少，但随着Java17的正式推出，那么ZGC必然会逐渐成为主流GC。
