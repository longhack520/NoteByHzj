Java<br />ThreadLocal是Java中一个非常重要的线程技术。它可以让每个线程都拥有自己的变量副本，避免了线程间的竞争和数据泄露问题。详细介绍ThreadLocal的定义、用法及其优点。<br />ThreadLocal是Java中一个用来实现线程封闭技术的类。它提供了一个本地线程变量，可以在多线程环境下使每个线程都拥有自己的变量副本。每个线程都可以独立地改变自己的副本，而不会影响到其他线程的副本。ThreadLocal的实现是基于ThreadLocalMap的，每个ThreadLocal对象都对应一个ThreadLocalMap，其中存储了线程本地变量的值。
<a name="KVeuI"></a>
## 优缺点
ThreadLocal的主要优点是可以提高并发程序的性能和安全性，同时也存在一些缺点和使用场景需要注意。<br />**优点**：

1. 提高并发性能：使用ThreadLocal可以避免多个线程之间的竞争，从而提高程序的并发性能。
2. 保证线程安全：每个线程有自己独立的变量副本，避免了线程安全问题。
3. 简化代码：使用ThreadLocal可以避免传递参数的繁琐，简化代码。

**缺点**：

1. 内存泄漏：ThreadLocal变量副本的生命周期与线程的生命周期一样长，如果线程长时间存在，而ThreadLocal变量没有及时清理，就会造成内存泄漏。
2. 增加资源开销：每个线程都要创建一个独立的变量副本，如果线程数很多，就会增加资源开销。
3. 不适用于共享变量：ThreadLocal适用于每个线程有独立的变量副本的场景，不适用于共享变量的场景。
<a name="h1eZG"></a>
## 适用场景

1. 线程安全的对象：ThreadLocal适用于需要在多个线程中使用的线程安全对象，例如SimpleDateFormat、Random等。
2. 跨层传递参数：ThreadLocal可以避免在方法之间传递参数的繁琐，尤其在跨层传递参数的场景中，可以大大简化代码。
3. 线程局部变量：ThreadLocal可以用于在当前线程中存储和访问局部变量，例如日志、请求信息等。
<a name="XZ4P6"></a>
## 实现原理
首先通过一张图看下ThreadLocal与线程的关系图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689049199861-a491b756-8e03-4c6d-b3e0-d455d6c89654.png#averageHue=%23fcfcfc&clientId=u61df75d0-5ea3-4&from=paste&id=ud8e30a6e&originHeight=375&originWidth=917&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub44265ce-0cd4-426e-9e9c-8c656efadf3&title=)

1. 每个Thread对象都有一个ThreadLocalMap类型的成员变量threadLocals，这个变量是一个键值对集合，用于存储每个ThreadLocal对象对应的值。
2. 每个ThreadLocal对象都有一个唯一的ID，用于在ThreadLocalMap中作为键来存储值。
3. 当一个线程第一次调用ThreadLocal对象的`get()`方法时，它会先获取当前线程的ThreadLocalMap对象，然后以ThreadLocal对象的ID作为键，从ThreadLocalMap中获取对应的值。
4. 如果ThreadLocalMap中不存在对应的键值对，则调用ThreadLocal对象的`initialValue()`方法来初始化一个值，并将其存储到ThreadLocalMap中。
5. 如果ThreadLocalMap对象的引用不再需要，那么需要手动将其置为null，这样可以避免内存泄漏。
<a name="RFaCZ"></a>
## 内存泄漏
ThreadLocal变量副本的生命周期与线程的生命周期一样长，如果线程长时间存在，而ThreadLocal变量没有及时清理，就会造成内存泄漏。为了避免内存泄漏，可以在使用ThreadLocal的地方及时清理ThreadLocal变量，例如在线程池中使用ThreadLocal时，需要在线程结束时手动清理ThreadLocal变量。<br />内存泄漏出现的原因：
:::warning
ThreadLocalMap中的Entry对象持有ThreadLocal对象的弱引用，但是ThreadLocalMap中的Entry对象是由ThreadLocal对象强引用的。<br />如果ThreadLocal对象没有及时清理，在ThreadLocal对象被垃圾回收时，ThreadLocalMap中的Entry对象仍然存在，从而导致内存泄漏。
:::
解决内存泄漏的方法：
:::warning
在使用ThreadLocal的代码中及时清理ThreadLocal变量。通常情况下，可以使用ThreadLocal的`remove()`方法手动清理ThreadLocal变量，或者在使用完ThreadLocal变量后将其设置为null。
:::
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689049199899-8ec6cbad-7373-47b7-bb75-e116e40f0736.png#averageHue=%23fdfdfd&clientId=u61df75d0-5ea3-4&from=paste&id=u54962a53&originHeight=460&originWidth=1039&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0e1a6785-2ab1-498b-ab32-6d365a10f6d&title=)<br />通过上图可以看到，在线程方法执行过程中，ThreadLocal、ThreadLocalMap以及Thread之间的引用关系; Thread中存在一个属性threadLocals指向了ThreadLocalMap，ThreadLocal实现线程级别的数据隔离主要是基于该对象；在ThreadLocal中是没有存储任何数据，其更像一个线程与ThreadLocalMap间的协调器，数据存储在ThreadLocalMap中，但是该Map的Key却是ThreadLocal的弱引用；<br />一般情况下，线程执行完成后，待线程销毁，那么线程对应的属性threadLocals也会被销毁；但是真实环境中对线程的使用大部分都是线程池，这样在整个系统生命周期中， 线程都是有效的，直至线程池关闭。而将ThreadLocalMap的Key设置成弱引用时，经过GC后该Map的Key则变成了null，但是其Value却一直存在，因此需要手动将key为null 的数据进行清理。<br />下面是一个示例演示如何避免ThreadLocal内存泄漏：
```java
public class MyThreadLocal {

    private static ThreadLocal<String> threadLocal = new ThreadLocal<>();

    public static void set(String value) {
        threadLocal.set(value);
    }

    public static String get() {
        return threadLocal.get();
    }

    public static void remove() {
        threadLocal.remove();
    }
}

public class MyRunnable implements Runnable {

    @Override
    public void run() {
        MyThreadLocal.set("hello");
        System.out.println(MyThreadLocal.get());
        // 在使用完ThreadLocal变量后，调用remove()方法清理ThreadLocal变量
        MyThreadLocal.remove();
    }
}
```
在上面的代码中，MyThreadLocal类封装了ThreadLocal变量的操作，MyRunnable类实现了Runnable接口，使用MyThreadLocal类来存储和访问 ThreadLocal变量。在MyRunnable的run()方法中，使用完ThreadLocal变量后，调用`remove()`方法清理ThreadLocal变量，避免了内存泄漏的问题。<br />**ThreadLocal一般会设置成**`**static**`<br />主要是为了避免重复创建TSO（thread specific object，即与线程相关的变量。）一个ThreadLocal实例对应当前线程中的一个TSO实例。如果把ThreadLocal声明为某个类的实例变量（而不是静态变量），那么每创建一个该类的实例就会导致一个新的TSO实例被创建。而这些被创建的TSO实例是同一个类的实例。同一个线程可能会访问到同一个TSO（指类）的不同实例，这即便不会导致错误，也会导致浪费！<br />简单的说就是在ThreadLocalMap中，同一个线程是否有必要设置多个ThreadLocal来存储线程变量？
<a name="RLDky"></a>
## 示例
下面是一个简单的例子，演示了如何使用ThreadLocal来实现线程数据隔离：
```java
public class ThreadLocalTest {

    private static ThreadLocal<String> threadLocal = new ThreadLocal<>();

    public static void main(String[] args) throws InterruptedException {
        new Thread(() -> {
            threadLocal.set("Thread A");
            System.out.println("Thread A: " + threadLocal.get());
        }).start();

        new Thread(() -> {
            threadLocal.set("Thread B");
            System.out.println("Thread B: " + threadLocal.get());
        }).start();

        Thread.sleep(1000);

        System.out.println("Main: " + threadLocal.get());
    }
}
```
运行结果如下：
```
Thread A: Thread A
Thread B: Thread B
Main: null
```
从输出结果可以看出，每个线程都拥有自己的变量副本，互不影响。而在主线程中，由于没有设置过变量副本，所以返回null。
<a name="uSklG"></a>
## 结束语
ThreadLocal是在多个线程间实现线程对数据独享，并不是用来解决线程间的数据共享问题。
