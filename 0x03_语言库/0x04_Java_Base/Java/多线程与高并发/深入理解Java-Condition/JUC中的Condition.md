Java
<a name="Qtqql"></a>
## 主要内容

1. synchronized中实现线程等待和唤醒
2. Condition简介及常用方法介绍及相关示例
3. 使用Condition实现生产者消费者
4. 使用Condition实现同步阻塞队列

Object对象中的`wait()`，`notify()`方法，用于线程等待和唤醒等待中的线程。
<a name="V1g3g"></a>
## synchronized中等待和唤醒线程示例
```java
import java.util.concurrent.TimeUnit;

public class Demo1 {
    static Object lock = new Object();

    public static class T1 extends Thread {
        @Override
        public void run() {
            System.out.println(System.currentTimeMillis() + "," + this.getName() + "准备获取锁!");
            synchronized (lock) {
                System.out.println(System.currentTimeMillis() + "," + this.getName() + "获取锁成功!");
                try {
                    lock.wait();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            System.out.println(System.currentTimeMillis() + "," + this.getName() + "释放锁成功!");
        }
    }

    public static class T2 extends Thread {
        @Override
        public void run() {
            System.out.println(System.currentTimeMillis() + "," + this.getName() + "准备获取锁!");
            synchronized (lock) {
                System.out.println(System.currentTimeMillis() + "," + this.getName() + "获取锁成功!");
                lock.notify();
                System.out.println(System.currentTimeMillis() + "," + this.getName() + " notify!");
                try {
                    TimeUnit.SECONDS.sleep(5);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                System.out.println(System.currentTimeMillis() + "," + this.getName() + "准备释放锁!");
            }
            System.out.println(System.currentTimeMillis() + "," + this.getName() + "释放锁成功!");
        }
    }

    public static void main(String[] args) throws InterruptedException {
        T1 t1 = new T1();
        t1.setName("t1");
        t1.start();
        TimeUnit.SECONDS.sleep(5);
        T2 t2 = new T2();
        t2.setName("t2");
        t2.start();
    }
}
```
输出：
```
1：1563530109234,t1准备获取锁!
2：1563530109234,t1获取锁成功!
3：1563530114236,t2准备获取锁!
4：1563530114236,t2获取锁成功!
5：1563530114236,t2 notify!
6：1563530119237,t2准备释放锁!
7：1563530119237,t2释放锁成功!
8：1563530119237,t1释放锁成功!
```
代码结合输出的结果分析一下：

1. 线程t1先获取锁，然后调用了`wait()`方法将线程置为等待状态，然后会释放lock的锁
2. 主线程等待5秒之后，启动线程t2，t2获取到了锁，结果中1、3行时间相差5秒左右
3. t2调用`lock.notify()`方法，准备将等待在lock上的线程t1唤醒，`notify()`方法之后又休眠了5秒，看一下输出的5、8可知，`notify()`方法之后，t1并不能立即被唤醒，需要等到t2将synchronized块执行完毕，释放锁之后，t1才被唤醒
4. `wait()`方法和`notify()`方法必须放在同步块内调用（`synchronized`块内），否则会报错
<a name="u3pXT"></a>
## Condition使用简介
在了解Condition之前，需要先了解一下重入锁ReentrantLock。<br />任何一个java对象都天然继承于Object类，在线程间实现通信的往往会应用到Object的几个方法，比如`wait()`、`wait(long timeout)`、`wait(long timeout, int nanos)`与`notify()`、`notifyAll()`几个方法实现等待/通知机制，同样的， 在java Lock体系下依然会有同样的方法实现等待/通知机制。<br />从整体上来看**Object的wait和notify/notify是与对象监视器配合完成线程间的等待/通知机制，而Condition与Lock配合完成等待通知机制，前者是java底层级别的，后者是语言级别的，具有更高的可控制性和扩展性**。两者除了在使用方式上不同外，在**功能特性**上还是有很多的不同：

1. Condition能够支持不响应中断，而通过使用Object方式不支持
2. Condition能够支持多个等待队列（new 多个Condition对象），而Object方式只能支持一个
3. Condition能够支持超时时间的设置，而Object不支持

Condition由ReentrantLock对象创建，并且可以同时创建多个，Condition接口在使用前必须先调用ReentrantLock的`lock()`方法获得锁，之后调用Condition接口的`await()`将释放锁，并且在该Condition上等待，直到有其他线程调用Condition的`signal()`方法唤醒线程，使用方式和`wait()`、`notify()`类似。<br />示例代码：
```java
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

public class Demo2 {
    static ReentrantLock lock = new ReentrantLock();
    static Condition condition = lock.newCondition();

    public static class T1 extends Thread {
        @Override
        public void run() {
            System.out.println(System.currentTimeMillis() + "," + this.getName() + "准备获取锁!");
            lock.lock();
            try {
                System.out.println(System.currentTimeMillis() + "," + this.getName() + "获取锁成功!");
                condition.await();
            } catch (InterruptedException e) {
                e.printStackTrace();
            } finally {
                lock.unlock();
            }
            System.out.println(System.currentTimeMillis() + "," + this.getName() + "释放锁成功!");
        }
    }

    public static class T2 extends Thread {
        @Override
        public void run() {
            System.out.println(System.currentTimeMillis() + "," + this.getName() + "准备获取锁!");
            lock.lock();
            try {
                System.out.println(System.currentTimeMillis() + "," + this.getName() + "获取锁成功!");
                condition.signal();
                System.out.println(System.currentTimeMillis() + "," + this.getName() + " signal!");
                try {
                    TimeUnit.SECONDS.sleep(5);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                System.out.println(System.currentTimeMillis() + "," + this.getName() + "准备释放锁!");
            } finally {
                lock.unlock();
            }
            System.out.println(System.currentTimeMillis() + "," + this.getName() + "释放锁成功!");
        }
    }

    public static void main(String[] args) throws InterruptedException {
        T1 t1 = new T1();
        t1.setName("t1");
        t1.start();
        TimeUnit.SECONDS.sleep(5);
        T2 t2 = new T2();
        t2.setName("t2");
        t2.start();
    }
}
```
输出：
```
1563532185827,t1准备获取锁!
1563532185827,t1获取锁成功!
1563532190829,t2准备获取锁!
1563532190829,t2获取锁成功!
1563532190829,t2 signal!
1563532195829,t2准备释放锁!
1563532195829,t2释放锁成功!
1563532195829,t1释放锁成功!
```
输出的结果和使用`synchronized`关键字的实例类似。<br />`Condition.await()`方法和`Object.wait()`方法类似，当使用`Condition.await()`方法时，需要先获取`Condition`对象关联的`ReentrantLock`的锁，在`Condition.await()`方法被调用时，当前线程会释放这个锁，并且当前线程会进行等待（处于阻塞状态）。在`signal()`方法被调用后，系统会从`Condition`对象的等待队列中唤醒一个线程，一旦线程被唤醒，被唤醒的线程会尝试重新获取锁，一旦获取成功，就可以继续执行了。因此，在signal被调用后，一般需要释放相关的锁，让给其他被唤醒的线程，让他可以继续执行。
<a name="sXfZj"></a>
## `Condition`常用方法
`Condition`接口提供的常用方法有：<br />**和**`**Object**`**中**`**wait**`**类似的方法**

1. `void await() throws InterruptedException`：当前线程进入等待状态，如果其他线程调用condition的`signal`或者`signalAll`方法并且当前线程获取Lock从await方法返回，如果在等待状态中被中断会抛出被中断异常；
2. `long awaitNanos(long nanosTimeout)`：当前线程进入等待状态直到被通知，中断或者**超时**；
3. `boolean await(long time, TimeUnit unit) throws InterruptedException`：同第二种，支持自定义时间单位，false：表示方法超时之后自动返回的，true：表示等待还未超时时，await方法就返回了（超时之前，被其他线程唤醒了）
4. `boolean awaitUntil(Date deadline) throws InterruptedException`：当前线程进入等待状态直到被通知，中断或者**到了某个时间**
5. `void awaitUninterruptibly();`：当前线程进入等待状态，不会响应线程中断操作，只能通过唤醒的方式让线程继续

**和Object的notify/notifyAll类似的方法**

1. `void signal()`：唤醒一个等待在condition上的线程，将该线程从**等待队列**中转移到**同步队列**中，如果在同步队列中能够竞争到Lock则可以从等待方法中返回。
2. `void signalAll()`：与1的区别在于能够唤醒所有等待在condition上的线程
<a name="dFyov"></a>
## `Condition.await()`过程中被打断
```java
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

public class Demo4 {
    static ReentrantLock lock = new ReentrantLock();
    static Condition condition = lock.newCondition();

    public static class T1 extends Thread {
        @Override
        public void run() {
            lock.lock();
            try {
                condition.await();
            } catch (InterruptedException e) {
                System.out.println("中断标志：" + this.isInterrupted());
                e.printStackTrace();
            } finally {
                lock.unlock();
            }
        }
    }

    public static void main(String[] args) throws InterruptedException {
        T1 t1 = new T1();
        t1.setName("t1");
        t1.start();
        TimeUnit.SECONDS.sleep(2);
        //给t1线程发送中断信号
        System.out.println("1、t1中断标志：" + t1.isInterrupted());
        t1.interrupt();
        System.out.println("2、t1中断标志：" + t1.isInterrupted());
    }
}
```
输出：
```
1、t1中断标志：false
2、t1中断标志：true
中断标志：false
java.lang.InterruptedException
    at java.util.concurrent.locks.AbstractQueuedSynchronizer$ConditionObject.reportInterruptAfterWait(AbstractQueuedSynchronizer.java:2014)
    at java.util.concurrent.locks.AbstractQueuedSynchronizer$ConditionObject.await(AbstractQueuedSynchronizer.java:2048)
    at com.example.chat09.Demo4$T1.run(Demo4.java:19)
```
调用`condition.await()`之后，线程进入阻塞中，调用`t1.interrupt()`，给t1线程发送中断信号，`await()`方法内部会检测到线程中断信号，然后触发`InterruptedException`异常，线程中断标志被清除。从输出结果中可以看出，线程t1中断标志的变换过程：false->true->false
<a name="Nq9ZG"></a>
## `await(long time, TimeUnit unit)`超时之后自动返回
```java
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

public class Demo5 {
    static ReentrantLock lock = new ReentrantLock();
    static Condition condition = lock.newCondition();

    public static class T1 extends Thread {
        @Override
        public void run() {
            lock.lock();
            try {
                System.out.println(System.currentTimeMillis() + "," + this.getName() + ",start");
                boolean r = condition.await(2, TimeUnit.SECONDS);
                System.out.println(r);
                System.out.println(System.currentTimeMillis() + "," + this.getName() + ",end");
            } catch (InterruptedException e) {
                e.printStackTrace();
            } finally {
                lock.unlock();
            }
        }
    }

    public static void main(String[] args) throws InterruptedException {
        T1 t1 = new T1();
        t1.setName("t1");
        t1.start();
    }
}
```
输出：
```
1563541624082,t1,start
false
1563541626085,t1,end
```
t1线程等待2秒之后，自动返回继续执行，最后`await`方法返回false，`**await**`**返回false表示超时之后自动返回**
<a name="L3R3r"></a>
## `await(long time, TimeUnit unit)`超时之前被唤醒
```java
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

public class Demo6 {
    static ReentrantLock lock = new ReentrantLock();
    static Condition condition = lock.newCondition();

    public static class T1 extends Thread {
        @Override
        public void run() {
            lock.lock();
            try {
                System.out.println(System.currentTimeMillis() + "," + this.getName() + ",start");
                boolean r = condition.await(5, TimeUnit.SECONDS);
                System.out.println(r);
                System.out.println(System.currentTimeMillis() + "," + this.getName() + ",end");
            } catch (InterruptedException e) {
                e.printStackTrace();
            } finally {
                lock.unlock();
            }
        }
    }

    public static void main(String[] args) throws InterruptedException {
        T1 t1 = new T1();
        t1.setName("t1");
        t1.start();
        //休眠1秒之后，唤醒t1线程
        TimeUnit.SECONDS.sleep(1);
        lock.lock();
        try {
            condition.signal();
        } finally {
            lock.unlock();
        }
    }
}
```
输出：
```
1563542046046,t1,start
true
1563542047048,t1,end
```
t1线程中调用`condition.await(5, TimeUnit.SECONDS);`方法会释放锁，等待5秒，主线程休眠1秒，然后获取锁，之后调用`signal()`方法唤醒t1，输出结果中发现await后过了1秒（1、3行输出结果的时间差），await方法就返回了，并且返回值是true。**true表示await方法超时之前被其他线程唤醒了。**
<a name="o0uBA"></a>
## `long awaitNanos(long nanosTimeout)`超时返回
```java
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

public class Demo7 {
    static ReentrantLock lock = new ReentrantLock();
    static Condition condition = lock.newCondition();

    public static class T1 extends Thread {
        @Override
        public void run() {
            lock.lock();
            try {
                System.out.println(System.currentTimeMillis() + "," + this.getName() + ",start");
                long r = condition.awaitNanos(TimeUnit.SECONDS.toNanos(5));
                System.out.println(r);
                System.out.println(System.currentTimeMillis() + "," + this.getName() + ",end");
            } catch (InterruptedException e) {
                e.printStackTrace();
            } finally {
                lock.unlock();
            }
        }
    }

    public static void main(String[] args) throws InterruptedException {
        T1 t1 = new T1();
        t1.setName("t1");
        t1.start();
    }
}
```
输出：
```
1563542547302,t1,start
-258200
1563542552304,t1,end
```
`**awaitNanos**`**参数为纳秒，可以调用**`**TimeUnit**`**中的一些方法将时间转换为纳秒。**<br />t1调用await方法等待5秒超时返回，返回结果为负数，表示超时之后返回的。
<a name="XqJhm"></a>
## `waitNanos(long nanosTimeout)`超时之前被唤醒
```java
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

public class Demo8 {
    static ReentrantLock lock = new ReentrantLock();
    static Condition condition = lock.newCondition();

    public static class T1 extends Thread {
        @Override
        public void run() {
            lock.lock();
            try {
                System.out.println(System.currentTimeMillis() + "," + this.getName() + ",start");
                long r = condition.awaitNanos(TimeUnit.SECONDS.toNanos(5));
                System.out.println(r);
                System.out.println(System.currentTimeMillis() + "," + this.getName() + ",end");
            } catch (InterruptedException e) {
                e.printStackTrace();
            } finally {
                lock.unlock();
            }
        }
    }

    public static void main(String[] args) throws InterruptedException {
        T1 t1 = new T1();
        t1.setName("t1");
        t1.start();
        //休眠1秒之后，唤醒t1线程
        TimeUnit.SECONDS.sleep(1);
        lock.lock();
        try {
            condition.signal();
        } finally {
            lock.unlock();
        }
    }
}
```
输出：
```
1563542915991,t1,start
3999988500
1563542916992,t1,end
```
t1中调用await休眠5秒，主线程休眠1秒之后，调用`signal()`唤醒线程t1，`await`方法返回正数，表示返回时距离超时时间还有多久，将近4秒，返回正数表示，线程在超时之前被唤醒了。<br />**其他几个有参的**`**await**`**方法和无参的**`**await**`**方法一样，线程调用**`**interrupt()**`**方法时，这些方法都会触发InterruptedException异常，并且线程的中断标志会被清除。**
<a name="KjJL4"></a>
## 同一个锁支持创建多个`Condition`
使用两个`Condition`来实现一个阻塞队列的例子：
```java
import java.util.LinkedList;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

public class BlockingQueueDemo<E> {
    int size;//阻塞队列最大容量

    ReentrantLock lock = new ReentrantLock();

    LinkedList<E> list = new LinkedList<>();//队列底层实现

    Condition notFull = lock.newCondition();//队列满时的等待条件
    Condition notEmpty = lock.newCondition();//队列空时的等待条件

    public BlockingQueueDemo(int size) {
        this.size = size;
    }

    public void enqueue(E e) throws InterruptedException {
        lock.lock();
        try {
            while (list.size() == size)//队列已满,在notFull条件上等待
            notFull.await();
            list.add(e);//入队:加入链表末尾
            System.out.println("入队：" + e);
            notEmpty.signal(); //通知在notEmpty条件上等待的线程
        } finally {
            lock.unlock();
        }
    }

    public E dequeue() throws InterruptedException {
        E e;
        lock.lock();
        try {
            while (list.size() == 0)//队列为空,在notEmpty条件上等待
            notEmpty.await();
            e = list.removeFirst();//出队:移除链表首元素
            System.out.println("出队：" + e);
            notFull.signal();//通知在notFull条件上等待的线程
            return e;
        } finally {
            lock.unlock();
        }
    }

    public static void main(String[] args) {
        BlockingQueueDemo<Integer> queue = new BlockingQueueDemo<>(2);
        for (int i = 0; i < 10; i++) {
            int data = i;
            new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        queue.enqueue(data);
                    } catch (InterruptedException e) {

                    }
                }
            }).start();
        }
        for (int i = 0; i < 10; i++) {
            new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        Integer data = queue.dequeue();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }).start();
        }
    }
}
```
代码非常容易理解，创建了一个阻塞队列，大小为3，队列满的时候，会被阻塞，等待其他线程去消费，队列中的元素被消费之后，会唤醒生产者，生产数据进入队列。上面代码将队列大小置为1，可以实现同步阻塞队列，生产1个元素之后，生产者会被阻塞，待消费者消费队列中的元素之后，生产者才能继续工作。
<a name="B8YC8"></a>
## Object的监视器方法与Condition接口的对比
| **对比项** | **Object 监视器方法** | **Condition** |
| --- | --- | --- |
| 前置条件 | 获取对象的锁 | 调用`Lock.lock`获取锁，调用`Lock.newCondition()`获取`Condition`对象 |
| 调用方式 | 直接调用，如：`object.wait()` | 直接调用，如：`condition.await()` |
| 等待队列个数 | 一个 | 多个，使用多个condition实现 |
| 当前线程释放锁并进入等待状态 | 支持 | 支持 |
| 当前线程释放锁进入等待状态中不响应中断 | 不支持 | 支持 |
| 当前线程释放锁并进入超时等待状态 | 支持 | 支持 |
| 当前线程释放锁并进入等待状态到将来某个时间 | 不支持 | 支持 |
| 唤醒等待队列中的一个线程 | 支持 | 支持 |
| 唤醒等待队列中的全部线程 | 支持 | 支持 |

<a name="Rn5HI"></a>
## 总结

1. 使用condition的步骤：创建condition对象，获取锁，然后调用condition的方法
2. 一个`ReentrantLock`支持床多个condition对象
3. `void await() throws InterruptedException;`方法会释放锁，让当前线程等待，支持唤醒，支持线程中断
4. `void awaitUninterruptibly();`方法会释放锁，让当前线程等待，支持唤醒，不支持线程中断
5. `long awaitNanos(long nanosTimeout) throws InterruptedException;`参数为纳秒，此方法会释放锁，让当前线程等待，支持唤醒，支持中断。超时之后返回的，结果为负数；超时之前返回的，结果为正数（表示返回时距离超时时间相差的纳秒数）
6. `boolean await(long time, TimeUnit unit) throws InterruptedException;`方法会释放锁，让当前线程等待，支持唤醒，支持中断。超时之后返回的，结果为false；超时之前返回的，结果为true
7. `boolean awaitUntil(Date deadline) throws InterruptedException;`参数表示超时的截止时间点，方法会释放锁，让当前线程等待，支持唤醒，支持中断。超时之后返回的，结果为false；超时之前返回的，结果为true
8. `void signal();`会唤醒一个等待中的线程，然后被唤醒的线程会被加入同步队列，去尝试获取锁
9. `void signalAll();`会唤醒所有等待中的线程，将所有等待中的线程加入同步队列，然后去尝试获取锁
