Java GC
<a name="Pxswq"></a>
## JVM的运行数据区
首先简单来画一张 JVM的结构原理图，如下。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642076146409-1d299801-7d90-4a12-a9f2-8b016c691fd9.webp#clientId=u0d278dcc-6829-4&from=paste&id=uc27b78c7&originHeight=664&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u231d888d-3ad8-4e7e-ad9b-7d6efc6342d&title=)<br />重点关注 JVM在运行时的数据区，可以看到在程序运行时，大致有5个部分。
<a name="V9bUo"></a>
### 1、方法区
不止是存“方法”，而是存储整个 class文件的信息，JVM运行时，类加载器子系统将会提取 class文件里面的类信息，并将其存放在方法区中。例如类的名称、类的类型（枚举、类、接口）、字段、方法等等。
<a name="EJBTr"></a>
### 2、堆（ Heap）
熟悉 c/c++编程的同学们应该相当熟悉 Heap了，而对于Java而言，每个应用都唯一对应一个JVM实例，而每一个JVM实例唯一对应一个堆。堆主要包括关键字 new的对象实例、 this指针，或者数组都放在堆中，并由应用所有的线程共享。堆由JVM的自动内存管理机制所管理，名为垃圾回收—— GC（garbage collection）。
<a name="ZBhdR"></a>
### 3、栈（ Stack）
操作系统内核为某个进程或者线程建立的存储区域,它保存着一个线程中的方法的调用状态，它具有先进后出的特性。在栈中的数据大小与生命周期严格来说都是确定的，例如在一个函数中声明的int变量便是存储在 stack中，它的大小是固定的，在函数退出后它的生命周期也从此结束。在栈中，每一个方法对应一个栈帧，JVM会对Java栈执行两种操作：压栈和出栈。这两种操作在执行时都是以栈帧为单位的。还有一些即时编译器编译后的代码等数据。
<a name="z0cp4"></a>
### 4、PC寄存器
pc寄存器用于存放一条指令的地址，每一个线程都有一个PC寄存器。
<a name="HuQXi"></a>
### 5、本地方法栈
用来调用其他语言的本地方法，例如 C/C++写的本地代码， 这些方法在本地方法栈中执行，而不会在Java栈中执行。
<a name="qieCc"></a>
## 初识GC
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642076146430-597529d7-771e-4405-8226-767be5d4465c.webp#clientId=u0d278dcc-6829-4&from=paste&id=GvktW&originHeight=442&originWidth=765&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uae083794-74ea-4682-8a29-9235be4acc6&title=)<br />自动垃圾回收机制，简单来说就是寻找 Java堆中的无用对象。打个比方：房间是JVM的内存，在房间里生活会制造垃圾和脏乱，而你妈就是 GC（听起来有点像骂人）。你妈每时每刻都觉得你房间很脏乱，不时要把你赶出门打扫房间，如果你妈一直在房间打扫，那么这个过程你无法继续在房间打游戏吃泡面。但如果你一直在房间，你的房间早晚要变成一个无法居住的猪窝。<br />那么，怎么样回收垃圾比较好呢？大致可以想出下面的思路。
<a name="qcpPa"></a>
### Marking
首先，所有堆中的对象都会被扫描一遍：总得知道哪些是垃圾，哪些是有用的物品吧。因为垃圾实在太多了，所以，你妈会把所有的要扔掉的东西都找出来并打上一个标签，到了时机成熟时回头来一起处理，这样她就能处理你不需要的废物、旧家具，而不是把你喜欢的衣服或者身份证之类的东西扔掉。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642076146576-a868dd55-c33b-41c3-86cd-39d15d79c597.webp#clientId=u0d278dcc-6829-4&from=paste&id=u123a0020&originHeight=499&originWidth=835&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub7fdbcfd-04b1-4605-9d77-a8c13319e0e&title=)
<a name="y6grK"></a>
### Normal Deletion
垃圾收集器将清除掉标记的对象：你妈已经整理了一部分杂物（或者已全部整理完），然后会将他们直接拎出去倒掉。你很开心房间又可以继续接受蹂躏了。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642076146314-be865462-9e00-4388-8e2b-10e3e138377a.webp#clientId=u0d278dcc-6829-4&from=paste&id=DArwG&originHeight=493&originWidth=725&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u18a3c25d-9796-4552-8681-66968d72a63&title=)
<a name="dUfK4"></a>
### Deletion with Compacting
压缩清除的方法：内存有空闲，并不代表着就能使用它，例如要分配数组这种一段连续空间，假如内存中碎片较多，肯定是行不通的。正如房间可能需要再放一个新的床，但是扔掉旧衣柜后，原来的位置并不能放得下新床，所以需要进行空间压缩，把剩下的家具和物品位置并到一起，这样就能腾出更多的空间啦。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642076146394-5d1490d2-9aa0-4172-a1e9-9d6735e5f6e3.webp#clientId=u0d278dcc-6829-4&from=paste&id=ue3fc2f23&originHeight=467&originWidth=727&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucd404cf8-269e-43b1-88b3-32c0a948b70&title=)<br />有趣的是，JVM并不是使用类似于 objective-c的 ARC（AutomaticReferenceCounting）的方式来引用计数对象，而是使用了叫根搜索算法( GC Root)的方法，基本思想就是选定一些对象作为 GC Roots，并组成根对象集合，然后从这些作为 GC Roots的对象作为起始点，搜索所走过的引用链（ ReferenceChain）。如果目标对象到 GC Roots是连接着的，则称该目标对象是可达的，如果目标对象不可达，则说明目标对象是可以被回收的对象。<br />GC Root使用的算法是相当复杂的，不必记住里面的所有细节。但是要知道的一点就是，可以作为 GC Root的对象可以主要分为四种。

- JVM栈中引用的对象；
- 方法区中，静态属性引用的对象；
- 方法区中，常量引用的对象；
- 本地方法栈中，JNI（即Native方法）引用的对象；

在 JDK1.2之后，Java将引用分为强引用、软引用、弱引用、虚引用4种，这4种引用强度依次减弱。
<a name="SWryY"></a>
## 分代与GC机制
嗯，听起来这样就可以了？但是实际情况下，很不幸，在JVM中绝大部分对象都是英年早逝的，在编码时大部分堆中的内存都是短暂临时分配的，所以无论是效率还是开销方面，按上面那样进行 GC往往是无法满足需求的。而且，实际上随着分配的对象增多， GC的时间与开销将会放大。所以，JVM的内存被分为了三个主要部分：新生代，老年代和永久代。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642076146935-8d4429cc-8401-4be1-a705-058edf6c9218.webp#clientId=u0d278dcc-6829-4&from=paste&id=ufd3ce182&originHeight=223&originWidth=1024&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1bcc5c25-c1be-4a3f-a4be-93b57206940&title=)
<a name="b3qga"></a>
### 新生代
所有新产生的对象全部都在新生代中， Eden区保存最新的对象，有两个 SurvivorSpace—— S1和 S0，三个区域的比例大致为 8:1:1。当新生代的 Eden区满了，将触发一次 GC，把新生代中的 GC称为 minor garbage collections。minor garbage collections是一种 Stopthe world事件，比如你妈在打扫时，会把你赶出去，而不是你一边扔垃圾她一边打扫。<br />来看下对象在堆中的分配过程，首先有新的对象进入时，默认放入新生代的 Eden区， S区都是默认为空的。下面对象的数字代表经历了多少次 GC，也就是对象的年龄。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642076147032-6ed35bcc-80a2-434c-b7e0-f0be40eaedc1.webp#clientId=u0d278dcc-6829-4&from=paste&id=ud342f572&originHeight=459&originWidth=753&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7efa7334-fb83-41f0-83c9-0762dfbdda8&title=)<br />当 eden区满了，触发 minor garbage collections，这时还有被引用的对象，就会被分配到 S0区域，剩下没有被引用的对象就都会被清除。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642076146795-d3ec687a-aa5d-499d-b1da-e92dee7f0b42.webp#clientId=u0d278dcc-6829-4&from=paste&id=u78e5a1f5&originHeight=453&originWidth=783&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0e7a4d7d-92cf-4db5-a34c-2dcbb96842c&title=)<br />再一次 GC时， S0区的部分对象很可能会出现没有引用的，被引用的对象以及 S0中的存活对象，会被一起移动到 S1中。eden和 S0中的未引用对象会被全部清除。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642076147086-fc56c6eb-cd62-47f4-a413-04bcf39a22e4.webp#clientId=u0d278dcc-6829-4&from=paste&id=u3820e4be&originHeight=496&originWidth=763&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4502b452-602c-4080-8829-1b9d9de9373&title=)<br />接下来就是无限循环上面的步骤了，当新生代中存活的对象超过了一定的【年龄】，会被分配至老年代的 Tenured区中。这个年龄可以通过参数 MaxTenuringThreshold设定，默认值为 15，图中的例子为 8次。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642076147194-cece9d09-5059-4ea0-96d1-677d97f6ad1d.webp#clientId=u0d278dcc-6829-4&from=paste&id=ubbc1ed91&originHeight=605&originWidth=854&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4a9449f2-0089-4bb4-a2ce-f07f65d35d0&title=)<br />新生代管理内存采用的算法为 GC复制算法( CopyingGC)，也叫标记-复制法，原理是把内存分为两个空间:一个 From空间，一个 To空间，对象一开始只在 From空间分配， To空间是空闲的。GC时把存活的对象从 From空间复制粘贴到 To空间，之后把 To空间变成新的 From空间，原来的 From空间变成 To空间。
<a name="IPrCY"></a>
#### 首先标记不可达对象。
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642076147163-f6bc384e-95d6-47d4-b4c2-fe28f2857b73.webp#clientId=u0d278dcc-6829-4&from=paste&id=u88840d95&originHeight=362&originWidth=875&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8d2b25e2-4b3c-4103-86e7-d665b15b034&title=)
<a name="DmfsF"></a>
#### 然后移动存活的对象到 to区，并保证他们在内存中连续。
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642076147434-a580fa9e-350b-4dca-b0c4-474bc32506f3.webp#clientId=u0d278dcc-6829-4&from=paste&id=u87eb2fec&originHeight=362&originWidth=870&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf2858bdf-278f-48df-94c1-6cb1820b648&title=)
<a name="mLcQB"></a>
#### 清扫垃圾。
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642076147696-b5cd2c38-e08f-4aeb-9163-3f91a4e500a6.webp#clientId=u0d278dcc-6829-4&from=paste&id=ufd91c5ee&originHeight=358&originWidth=875&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u73b4bee3-4411-46dd-88bc-b7484a602ea&title=)<br />可以看到上图操作后内存几乎都是连续的，所以它的效率是非常高的，但是相对的吞吐量会较大。并且，把内存一分为二，占用了将近一半的可用内存。用一段伪代码来实现大致为下。
```java
void copying(){
        $free = $to_start // $free表示To区占用偏移量，每复制成功一个对象obj, 
                          // $free向前移动size(obj)
        for(r : $roots)
            *r = copy(*r) // 复制成功后返回新的引用

        swap($from_start, $to_start) // GC完成后交互From区与To区的指针
 }
```
<a name="fSKpm"></a>
### 老年代
老年代用来存储活时间较长的对象，老年代区域的 GC是 major garbage collection，老年代中的内存不够时，就会触发一次。这也是一个 Stopthe world事件，但是看名字就知道，这个回收过程会相当慢，因为这包括了对新生代和老年代所有对象的回收，也叫 FullGC。<br />老年代管理内存最早采用的算法为标记-清理算法，这个算法很好理解，结合 GC Root的定义，把所有不可达的对象全部标记进行清除。
<a name="iep4c"></a>
#### 在清除前，黄色的为不可达对象。
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642076147713-65a318f0-e04a-414c-a574-bf7bd8bb2b2b.webp#clientId=u0d278dcc-6829-4&from=paste&id=u14fa898f&originHeight=372&originWidth=565&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u23d747b1-07e1-40c1-96d8-62761a67ab7&title=)
<a name="zhOrl"></a>
#### 在清除后，全部都变成可达对象。
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642076147566-60af10a1-3b50-4cc6-9afc-0eaa491bab23.webp#clientId=u0d278dcc-6829-4&from=paste&id=u8c0d05bf&originHeight=371&originWidth=567&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2d764348-02f2-40be-8cde-c5cec34ae7b&title=)<br />那么，这个算法的劣势很好理解：对，会在标记清除的过程中产生大量的内存碎片，Java在分配内存时通常是按连续内存分配，这样会浪费很多内存。所以，现在的 JVM GC在老年代都是使用标记-压缩清除方法，将上图在清除后的内存进行整理和压缩，以保证内存连续，虽然这个算法的效率是三种算法里最低的。
<a name="Hgzce"></a>
### 永久代
永久代位于方法区，主要存放元数据，例如 Class、 Method的元信息，与 GC要回收的对象其实关系并不是很大，可以几乎忽略其对 GC的影响。除了 JavaHotSpot这种较新的虚拟机技术，会回收无用的常量和的类，以免大量运用反射这类频繁自定义 ClassLoader的操作时方法区溢出。
<a name="hm8ZU"></a>
## GC收集器与优化
一般而言， GC不应该成为影响系统性能的瓶颈，在评估 GC收集器的优劣时一般考虑以下几点：

- 吞吐量
- GC开销
- 暂停时间
- GC频率
- 堆空间
- 对象生命周期

所以针对不同的 GC收集器，要对应的应用场景来进行选择和调优，回顾 GC的历史，主要有 4种 GC收集器: Serial、 Parallel、 CMS和 G1。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642076147759-a14c84e8-61d9-4908-9983-f08d6c918219.webp#clientId=u0d278dcc-6829-4&from=paste&id=u277aadbd&originHeight=768&originWidth=1024&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8df75971-5ed4-4cf3-bd88-eaf56a85190&title=)
<a name="SMclD"></a>
### Serial
Serial收集器使用了标记-复制的算法，可以用`-XX:+UseSerialGC`使用单线程的串行收集器。但是在 GC进行时，程序会进入长时间的暂停时间，一般不太建议使用。
<a name="SmaXp"></a>
### Parallel
`-XX:+UseParallelGC-XX:+UseParallelOldGCParallel`也使用了标记-复制的算法，但是称之为吞吐量优先的收集器，因为 Parallel最主要的优势在于并行使用多线程去完成垃圾清理工作，这样可以充分利用多核的特性，大幅降低 gc时间。当程序场景吞吐量较大，例如消息队列这种应用，需要保证有效利用 CPU资源，可以忍受一定的停顿时间，可以优先考虑这种方式。
<a name="VtOFx"></a>
### CMS ( ConcurrentMarkSweep)
`-XX:+UseParNewGC-XX:+UseConcMarkSweepGCCMS`使用了标记-清除的算法，当应用尤其重视服务器的响应速度（比如 Apiserver），希望系统停顿时间最短，以给用户带来较好的体验，那么可以选择 CMS。CMS收集器在 MinorGC时会暂停所有的应用线程，并以多线程的方式进行垃圾回收。在 FullGC时不暂停应用线程，而是使用若干个后台线程定期的对老年代空间进行扫描，及时回收其中不再使用的对象。
<a name="c0quL"></a>
### G1（ GarbageFirst）
-XX:+UseG1GC 在堆比较大的时候，如果 full gc频繁，会导致停顿，并且调用方阻塞、超时、甚至雪崩的情况出现，所以降低 full gc的发生频率和需要时间，非常有必要。G1的诞生正是为了降低 FullGC的次数，而相较于 CMS， G1使用了标记-压缩清除算法，这可以大大降低较大内存（ 4GB以上） GC时产生的内存碎片。<br />G1提供了两种 GC模式， YoungGC和 MixedGC，两种都是 StopTheWorld(STW)的。YoungGC主要是对 Eden区进行 GC， MixGC不仅进行正常的新生代垃圾收集，同时也回收部分后台扫描线程标记的老年代分区。<br />另外有趣的一点， G1将新生代、老年代的物理空间划分取消了，而是将堆划分为若干个区域（ region），每个大小都为 2的倍数且大小全部一致，最多有 2000个。除此之外， G1专门划分了一个 Humongous区，它用来专门存放超过一个 region 50%大小的巨型对象。在正常的处理过程中，对象从一个区域复制到另外一个区域，同时也完成了堆的压缩。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642076147793-56ed62dd-61ef-41d7-b1a3-31ca227b8b2c.webp#clientId=u0d278dcc-6829-4&from=paste&id=ubf5850a9&originHeight=280&originWidth=681&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubc0dec75-81b4-4e21-bdb5-f29924cb519&title=)<br />常用参数<br />`-XX:+UseSerialGC`：在新生代和老年代使用串行收集器<br />`-XX:+UseParNewGC`：在新生代使用并行收集器<br />`-XX:+UseParallelGC` ：新生代使用并行回收收集器，更加关注吞吐量<br />`-XX:+UseParallelOldGC`：老年代使用并行回收收集器<br />`-XX:ParallelGCThreads`：设置用于垃圾回收的线程数<br />`-XX:+UseConcMarkSweepGC`：新生代使用并行收集器，老年代使用CMS+串行收集器<br />`-XX:ParallelCMSThreads`：设定CMS的线程数量<br />`-XX:+UseG1GC`：启用G1垃圾回收器
