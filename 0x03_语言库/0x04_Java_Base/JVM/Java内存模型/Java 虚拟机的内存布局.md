Java<br />通常所说的 Java 虚拟机（JVM）的内存布局，一般是指 Java 虚拟机的运行时数据区（Runtime Data Area），也就是当字节码被类加载器加载之后的执行区域划分。当然它通常是 JVM 模块的第一个面试问题，所以，接下来一起来看它里面包含了哪些内容。
<a name="lecY3"></a>
## 官方定义
《Java虚拟机规范》中将 JVM 运行时数据区域划分为以下 5 部分：

1. 程序计数器（Program Counter Register）
2. Java虚拟机栈（Java Virtual Machine Stacks）
3. 本地方法栈（Native Method Stack）
4. Java 堆（Java Heap）
5. 方法区（Methed Area）

如下图所示：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675775256489-8705c410-6704-4810-8620-6f9aebbd3fe8.png#averageHue=%23f5f5f5&clientId=ude1da6eb-12c1-4&from=paste&id=u38dd20cc&originHeight=574&originWidth=1013&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6d16b81a-c875-45bc-94ff-b5b950fa42e&title=)接下来，分别来看每个模块的作用及详细介绍。
<a name="jOoUz"></a>
## 1、程序计数器
《Java虚拟机规范》中对程序计数器的定义如下：
> The Java Virtual Machine can support many threads of execution at once (JLS §17). Each Java Virtual Machine thread has its own pc (program counter) register. At any point, each Java Virtual Machine thread is executing the code of a single method, namely the current method (§2.6) for that thread. If that method is not native, the pc register contains the address of the Java Virtual Machine instruction currently being executed. If the method currently being executed by the thread is native, the value of the Java Virtual Machine's pc register is undefined. The Java Virtual Machine's pc register is wide enough to hold a returnAddress or a native pointer on the specific platform.

以上内容翻译成中文，简单来说它的含义是：JVM 中可以有多个执行线程，每个线程都有自己的程序计数器，在程序计数器中包含的是正在执行线程的指令地址。<br />也就是说，程序计数器（Program Counter Register）是线程独有一块很小的内存区域，保存当前线程所执行字节码的位置，包括正在执行的指令、跳转、分支、循环、异常处理等。
<a name="Hv2la"></a>
### 1.1 作用
**CPU 核数是比较少的，而任务（线程）是比较多的，所以真实的情况是，CPU 会不停的切换线程以执行所有的程序**，当然因为（CPU）切换的速度比较快，所以是感知不到的，感觉好像所有的程序都是一直在执行，其实从微观的层面来看，所有的程序都是切换执行的。<br />那么问题来了，CPU 一直在切换线程执行任务，那 CPU 再次切换到某个线程时，它是怎么知道当前的线程上次知道到哪了？<br />这就是程序计数器的作用，**程序计数器里面保存了当前线程执行的行号，这样当 CPU 切换到当前线程时，才能接着上次执行的位置，继续执行**。<br />PS：程序计数器中真实记录的是下一行任务的执行指令。程序计数器也是 JVM 运行时数据区中执行最快的一块区域。
<a name="wIiz5"></a>
### 1.2 线程共享
程序计数器记录的是每个线程的执行行号，所以每个线程都拥有自己的程序计数器，所以**此区域不是线程共享的，而是线程私有的**。
<a name="Q9Y0f"></a>
### 1.3 GC
GC 是 Garbage Collection 的缩写，译为垃圾收集。**此区域不存在 GC。**
<a name="jL5SM"></a>
### 1.4 OOM
OOM 是 Out of Memory 的缩写，译为内存溢出。**此区域不存在 OOM 的问题。**
<a name="vphhA"></a>
## 2、Java 虚拟机栈
Java 虚拟机栈（Java Virtual Machine Stack）也叫做 JVM 栈，《Java虚拟机规范》对此区域的说明如下：
> Each Java Virtual Machine thread has a private _Java Virtual Machine stack_, created at the same time as the thread. A Java Virtual Machine stack stores frames (§2.6). A Java Virtual Machine stack is analogous to the stack of a conventional language such as C: it holds local variables and partial results, and plays a part in method invocation and return. Because the Java Virtual Machine stack is never manipulated directly except to push and pop frames, frames may be heap allocated. The memory for a Java Virtual Machine stack does not need to be contiguous._In the First Edition of The Java® Virtual Machine Specification, the Java Virtual Machine stack was known as the Java stack._This specification permits Java Virtual Machine stacks either to be of a fixed size or to dynamically expand and contract as required by the computation. If the Java Virtual Machine stacks are of a fixed size, the size of each Java Virtual Machine stack may be chosen independently when that stack is created._A Java Virtual Machine implementation may provide the programmer or the user control over the initial size of Java Virtual Machine stacks, as well as, in the case of dynamically expanding or contracting Java Virtual Machine stacks, control over the maximum and minimum sizes._The following exceptional conditions are associated with Java Virtual Machine stacks:
> - **If the computation in a thread requires a larger Java Virtual Machine stack than is permitted, the Java Virtual Machine throws a StackOverflowError.**
> - **If Java Virtual Machine stacks can be dynamically expanded, and expansion is attempted but insufficient memory can be made available to effect the expansion, or if insufficient memory can be made available to create the initial Java Virtual Machine stack for a new thread, the Java Virtual Machine throws an OutOfMemoryError.**

以上内容翻译成中文的含义如下：<br />Java 虚拟机栈是线程私有的区域，它随着线程的创建而创建。它里面保存的是局部变量表（基础数据类型和对象引用地址）和计算过程中的中间结果。Java 虚拟机的内存不需要连续，它只有两个操作：入栈和出栈。<br />Java 虚拟机栈要么大小固定，要么根据计算动态的扩展和收缩。程序员可以对 Java 虚拟机栈进行初始值的大小设置和最大值的设置。<br />Java 虚拟机栈出现的异常有两种：

- 当 Java 虚拟机栈大小固定时，如果程序中的栈分配超过了最大虚拟机栈就会出现 StackOverflowError 异常。
- 如果 Java 虚拟机栈是动态扩展的，那么当内存不足时，就会引发 OutOfMemoryError 的异常。**Java 虚拟机栈主要是管 Java 程序运行的，它保存的是方法的局部变量、方法执行中的部分结果，并参与方法的调用和返回。**简单来说，栈是运行时单位，而堆是存储单位。也就是说：栈解决的是程序运行的问题，即程序如何执行？或者说如何处理数据。堆解决的是数据存储的问题，即数据怎么放？放在哪儿。
<a name="DbXZW"></a>
### 2.1 作用
<a name="c0L6v"></a>
### 2.2 线程共享
**Java 虚拟机栈是线程私有的**，它的生命周期和线程的生命周期一致。
<a name="Ap12m"></a>
### 2.3 GC
Java 虚拟机栈因为只有入栈和出栈两个操作，所以它是**不涉及垃圾回收**的。
<a name="cmG8R"></a>
### 2.4 OOM
此区域虽然没有 GC，但存在两种异常：

- 当 Java 虚拟机栈大小固定时，如果程序中的栈分配超过了最大虚拟机栈就会出现 StackOverflowError 异常。
- 如果 Java 虚拟机栈是动态扩展的，那么当内存不足时，就会引发 OutOfMemoryError 的异常。

也就是，**Java 虚拟机栈是可能存在 OOM 的**。
<a name="rYixH"></a>
### 2.5 常见参数设置
设置 Java 虚拟机栈大小：-Xss。如设置：-Xss128k，表示设置每个线程的栈大小为 128k，此设置等价于 -XX:ThreadStackSize=128k。
<a name="zJRzC"></a>
### 2.6 常见问题演示
最简单的错误示例就是死循环，方法自己调自己，这样 Java 虚拟机栈就会只入栈不出栈，当到达 Java 虚拟机栈的最大数之后就会出现 StackOverflowError 异常，如下图所示：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675775256484-01e9840d-0e80-425f-9ddf-a4f4f83ffa60.png#averageHue=%23f9f6f4&clientId=ude1da6eb-12c1-4&from=paste&id=uc9c25cf7&originHeight=400&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uabaf27da-a354-42e5-ad4a-5718033b193&title=)
<a name="vKekP"></a>
## 3、本地方法栈
本地方法栈（Native Method Stacks），《Java 虚拟机规范》对此区域的说明如下：
> An implementation of the Java Virtual Machine may use conventional stacks, colloquially called "C stacks," to support native methods (methods written in a language other than the Java programming language). Native method stacks may also be used by the implementation of an interpreter for the Java Virtual Machine's instruction set in a language such as C. Java Virtual Machine implementations that cannot load native methods and that do not themselves rely on conventional stacks need not supply native method stacks. If supplied, native method stacks are typically allocated per thread when each thread is created. This specification permits native method stacks either to be of a fixed size or to dynamically expand and contract as required by the computation. If the native method stacks are of a fixed size, the size of each native method stack may be chosen independently when that stack is created._A Java Virtual Machine implementation may provide the programmer or the user control over the initial size of the native method stacks, as well as, in the case of varying-size native method stacks, control over the maximum and minimum method stack sizes._The following exceptional conditions are associated with native method stacks:
> - **If the computation in a thread requires a larger native method stack than is permitted, the Java Virtual Machine throws a StackOverflowError.**
> - **If native method stacks can be dynamically expanded and native method stack expansion is attempted but insufficient memory can be made available, or if insufficient memory can be made available to create the initial native method stack for a new thread, the Java Virtual Machine throws an OutOfMemoryError.**

以上内容，挑重点简单翻译一下：**本地方法栈俗称“C栈”，它是 Native（本地）方法（用 Java 编程语言以外的语言编写的方法），此区域和 Java 虚拟机栈类似，这不过诸如 C 语言等使用的栈空间。它也是存在两种异常：StackOverflowError 和 OutOfMemoryError**。<br />PS：因为此区域是非 Java 语言实现和使用的，所以本文就不做过多的赘述，总之，记得一句话：**此区域和 Java 虚拟机栈类似，不过是给 C/C++ 语言使用的**。
<a name="aYaYb"></a>
## 4、堆
堆（Heap）《Java 虚拟机规范》对此区域的说明如下：
> The Java Virtual Machine has a _heap_ that is shared among all Java Virtual Machine threads. The heap is the run-time data area from which memory for all class instances and arrays is allocated. The heap is created on virtual machine start-up. Heap storage for objects is reclaimed by an automatic storage management system (known as a _garbage collector_); objects are never explicitly deallocated. The Java Virtual Machine assumes no particular type of automatic storage management system, and the storage management technique may be chosen according to the implementor's system requirements. The heap may be of a fixed size or may be expanded as required by the computation and may be contracted if a larger heap becomes unnecessary. The memory for the heap does not need to be contiguous._A Java Virtual Machine implementation may provide the programmer or the user control over the initial size of the heap, as well as, if the heap can be dynamically expanded or contracted, control over the maximum and minimum heap size._The following exceptional condition is associated with the heap:
> - **If a computation requires more heap than can be made available by the automatic storage management system, the Java Virtual Machine throws an OutOfMemoryError.**

以上内容，挑重点简单翻译一下：<br />堆是线程共享的，程序中所有类实例和数组的内存都存储在此区域，它在 Java 虚拟机启动时就会创建。对象不会被显式释放，只会在垃圾收集时释放。堆的大小可以是固定的，也可以动态扩展或收缩。堆的内存在物理层面不需要是连续的。<br />程序员可以对堆进行初始大小控制，或者设置最大、最小堆的容量。堆可能会出现 OutOfMemoryError 异常。
<a name="suL8T"></a>
### 4.1 作用
堆是 Java 虚拟机的主要存储单位，Java 中所有的对象和数组都是保存在此区域的。
<a name="PBprL"></a>
### 4.2 线程共享
堆是线程共享的，堆上的对象可能被多个线程同时访问。
<a name="SmSGp"></a>
### 4.3 GC
堆是 JVM 最大的一块区域，也是垃圾回收器进行垃圾回收最频繁的一块区域。
<a name="qJUGZ"></a>
### 4.4 OOM
当堆空间不足时，会发生 OutOfMemoryError 异常。
<a name="q50a2"></a>
### 4.5 常见参数设置

- -Xms：设置初始 Java 堆大小，比如：-Xms10m，表示设置堆的初始大小为 10MB。
- -Xmx：设置最大 Java 堆大小，比如：-Xmx10m，表示设置堆的最大空间为 10MB。接下来演示一下堆空间 OOM 的问题，先使用“-Xmx50m”的参数来设置一下 Idea，它表示将程序运行的最大内存设置为 50m，如果程序的运行超过这个值就会出现内存溢出的问题，设置方法如下：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675775256476-d513d1e7-01a6-4ab1-aac7-f105f56b7a2a.png#averageHue=%233e4244&clientId=ude1da6eb-12c1-4&from=paste&id=u705f4591&originHeight=677&originWidth=1012&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0343a1b7-e396-495d-8c68-8c3251acef2&title=)设置后的最终效果这样的：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675775256472-3ee2227c-8da5-422c-ae41-d06129265bac.png#averageHue=%233f4245&clientId=ude1da6eb-12c1-4&from=paste&id=u85dc52d9&originHeight=722&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u30bc5b5a-81e6-4b30-af5c-c1c7e6f6d19&title=)PS：因为使用的 Idea 是社区版，所以可能界面不一样，只需要点击“Edit Configurations...”找到“VM options”选项，设置上“-Xmx50m”参数就可以了。
<a name="oNnRl"></a>
### 4.6 常见问题演示
配置完 Idea 之后，接下来实现一下业务代码。在代码中会创建一个大对象，这个对象中会有一个 10m 大的数组，然后将这个大对象存储在 ThreadLocal 中，再使用线程池执行大于 5 次添加任务，因为设置了最大运行内存是 50m，所以理想的情况是执行 5 次添加操作之后，就会出现内存溢出的问题，实现代码如下：
```java
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

public class ThreadLocalOOMExample {

    /**
     * 定义一个 10m 大的类
     */
    static class MyTask {
        // 创建一个 10m 的数组（单位转换是 1M -> 1024KB -> 1024*1024B）
        private byte[] bytes = new byte[10 * 1024 * 1024];
    }

    // 定义 ThreadLocal
    private static ThreadLocal<MyTask> taskThreadLocal = new ThreadLocal<>();

    // 主测试代码
    public static void main(String[] args) throws InterruptedException {
        // 创建线程池
        ThreadPoolExecutor threadPoolExecutor =
                new ThreadPoolExecutor(5, 5, 60,
                        TimeUnit.SECONDS, new LinkedBlockingQueue<>(100));
        // 执行 10 次调用
        for (int i = 0; i < 10; i++) {
            // 执行任务
            executeTask(threadPoolExecutor);
            Thread.sleep(1000);
        }
    }

    /**
     * 线程池执行任务
     * @param threadPoolExecutor 线程池
     */
    private static void executeTask(ThreadPoolExecutor threadPoolExecutor) {
        // 执行任务
        threadPoolExecutor.execute(new Runnable() {
            @Override
            public void run() {
                System.out.println("创建对象");
                // 创建对象（10M）
                MyTask myTask = new MyTask();
                // 存储 ThreadLocal
                taskThreadLocal.set(myTask);
                // 将对象设置为 null，表示此对象不在使用了
                myTask = null;
            }
        });
    }
}
```
以上程序的执行结果如下：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675775256464-d643ced7-c9c7-4f5b-84e6-7287b4e5ee46.png#averageHue=%23343030&clientId=ude1da6eb-12c1-4&from=paste&id=ud8b134e3&originHeight=212&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf57b49f6-be31-410a-a0ab-7386b8dbd0d&title=)从上述图片可看出，当程序执行到第 5 次添加对象时就出现内存溢出的问题了，这是因为设置了最大的运行内存是 50m，每次循环会占用 10m 的内存，加上程序启动会占用一定的内存，因此在执行到第 5 次添加任务时，就会出现内存溢出的问题。
<a name="rQOri"></a>
## 5、方法区
方法区（Method Area）《Java 虚拟机规范》对此区域的说明如下：
> The Java Virtual Machine has a _method area_ that is shared among all Java Virtual Machine threads. The method area is analogous to the storage area for compiled code of a conventional language or analogous to the "text" segment in an operating system process. It stores per-class structures such as the run-time constant pool, field and method data, and the code for methods and constructors, including the special methods (§2.9) used in class and instance initialization and interface initialization. The method area is created on virtual machine start-up. Although the method area is logically part of the heap, simple implementations may choose not to either garbage collect or compact it. This specification does not mandate the location of the method area or the policies used to manage compiled code. The method area may be of a fixed size or may be expanded as required by the computation and may be contracted if a larger method area becomes unnecessary. The memory for the method area does not need to be contiguous._A Java Virtual Machine implementation may provide the programmer or the user control over the initial size of the method area, as well as, in the case of a varying-size method area, control over the maximum and minimum method area size._The following exceptional condition is associated with the method area:

- **If memory in the method area cannot be made available to satisfy an allocation request, the Java Virtual Machine throws an OutOfMemoryError.**

以上内容，挑重点简单翻译一下：方法区是线程共享的，方法区类似于传统语言的编译代码的存储区，或者类似于操作系统进程中的“文本”段。它存储每个类的结构，如运行时常量池、字段和方法数据，以及方法和构造函数的代码。<br />方法区域是在 Java 虚拟机启动时创建的，尽管方法区域在逻辑上是堆的一部分，但简单的实现可能选择不进行垃圾收集或压缩。方法区域可以是固定的大小，也可以动态扩展。方法区的（物理）内存不需要连续。<br />Java 虚拟机实现可以为程序员或用户提供对方法区域初始大小的控制，以及在可变大小的方法区域的情况下，对最大和最小方法区域大小的控制。<br />如果方法区中的内存无法满足分配请求，Java 虚拟机将抛出一个 OutOfMemoryError。
<a name="hTEor"></a>
### 5.1 作用
用于存储每个类的结构，包括**运行时常量池、静态变量、字段和方法数据**。
<a name="F5iX4"></a>
### 5.2 HotSpot 方法区实现
HotSpot 虚拟机是 Sun JDK 和 Open JDK 中自带的虚拟机，也是目前使用范围最广的 Java 虚拟机。作为官方 Java 虚拟机的化身，目前所讲的所有知识，几乎都是针对此虚拟机的，所以要看 HotSpot 虚拟机对《Java 虚拟机规范》中方法区的实现。<br />**对于 HotSpot 虚拟机来说，不同的 JDK 方法区的实现是不同的，在 JDK 1.7 之前，HotSpot 技术团队使用的是永久代来实现方法区的**，但这种实现有一个致命的问题，这样设计更容易造成内存溢出。因为永久代有 -XX：MaxPermSize（方法区分配的最大内存）的上限，即使不设置也会有默认的大小。例如，32 位操作系统中的 4GB 内存限制等，并且这样设计导致了部分的方法在不同类型的 Java 虚拟机下的表现也不同，比如 String::intern() 方法。所以在 JDK 1.7 时 HotSpot 虚拟机已经把原本放在永久代的字符串常量池和静态变量等移出了方法区，并且在 JDK 1.8 中完全废弃了永久代的概念。<br />**JDK 1.8 之后，HotSpot 虚拟机开始使用元空间（Meta Space）来实现方法区了。**
<a name="X5S2E"></a>
### 5.3 线程共享
方法区是线程共享的。
<a name="dqb9W"></a>
### 5.4 GC
《Java 虚拟机规范》中规定方法区可以没有 GC（垃圾回收），但 HotSpot 中对此区域实现了 GC 操作。
<a name="t7aqK"></a>
### 5.5 OOM
方法区是存在 OOM 情况的，比如在 JDK 1.8 中，如果元空间设置空间过小，而类信息产生的过多就会产生 OOM，如下示例所示：
```java
import net.sf.cglib.proxy.Enhancer;
import net.sf.cglib.proxy.MethodInterceptor;
import net.sf.cglib.proxy.MethodProxy;
import java.lang.reflect.Method;

/**
 * 方法区 OOM 演示（JDK 1.8+）
 * 设置 -XX:MaxMetaspaceSize=10m 元空间最大内存为 10MB。
 */
public class MethodAreaOOMExample {
    public static void main(String[] args) {
        while (true) {
            Enhancer enhancer = new Enhancer();
            enhancer.setSuperclass(MethodAreaOOMExample.class);
            enhancer.setUseCache(false);
            enhancer.setCallback(new MethodInterceptor() {
                @Override
                public Object intercept(Object object, Method method,
                                        Object[] args,
                                        MethodProxy methodProxy) throws Throwable {
                    return methodProxy.invokeSuper(object, args);
                }
            });
            enhancer.create();
        }
    }
}
```
以上程序的执行结果如下图所示：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675775256970-f6072660-5713-465c-8a1e-4bde4568e33c.png#averageHue=%23cfbca7&clientId=ude1da6eb-12c1-4&from=paste&id=u13918ce6&originHeight=148&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u16cd6fdf-2a97-4d95-9f07-15b3966e5d0&title=)以上代码是通过 CGLIB 不断的产生动态代理类将方法区填满，从而就导致 OOM 的问题。<br />PS：在使用 CGLIB 之前，需要现在当前项目中导入 CGLIB 才可以正常使用。
<a name="hIUqP"></a>
### 5.6 常用参数设置
**永久代（HotSpot 虚拟机，JDK 1.7 之前设置有效）：**

- -XX:PermSize=100m：设置永久代初始值为 100MB。
- -XX:MaxPermSize=100m：设置永久代最大值为 100MB。

**元空间（HotSpot 虚拟机，JDK 1.8 之后设置有效）：**

- -XX:MetaspaceSize=100m：设置元空间初始大小为 100MB。
- -XX:MaxMetaspaceSize=100m：设置元空间最大容量为 100MB，默认不设置，则没有限制。
- -XX:CompressedClassSpaceSize=100m：设置 Class Metaspace 的大小为 100MB，默认值为 1G。

**直接内存（HotSpot 虚拟机，JDK 1.8 之后设置有效）：**-XX:MaxDirectMemorySize=100m：指定直接内存的最大容量为 100MB。
<a name="nVPGK"></a>
## 总结
《Java虚拟机规范》中将 JVM 运行时数据区域划分为以下 5 部分：

1. 程序计数器（Program Counter Register）
2. Java 虚拟机栈（Java Virtual Machine Stacks）
3. 本地方法栈（Native Method Stack）
4. Java 堆（Java Heap）
5. 方法区（Methed Area）

其中线程私有的区域是：程序计数器、Java 虚拟机栈、本地方法栈；而线程共享的是：Java 堆和方法区。<br />而除了程序计数器，其他区域都是可以会出现 OOM 的。
