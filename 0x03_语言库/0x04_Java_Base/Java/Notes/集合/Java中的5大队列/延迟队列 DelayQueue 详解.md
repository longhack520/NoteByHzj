Java
<a name="cUa6g"></a>
## 1、前言
在项目中有这样一个场景：页面链接是同一个，但是可以有多个子页面，不同的时间要展示不同子页面，类似一个页面排期功能。也许大家觉得要实现这个功能比较简单，实现过程为：获取所有子页面的生效时间，对每个生效时间点创建一个定时器，每个定时器执行内容为使用新的子页面进行渲染。对于单个或者少量页面这样做完全没有问题，但是在项目中每天都有上万个这样的页面需要进行排期。如果采用这种方式，势必会创建上万个定时器放到jvm内存，这显然是不科学的。<br />做法是把每个子页面排期看成是一个任务放到任务表，任务执行时间即为子页面的开始时间，再通过一个分布式任务调度器，每次获取将来5分钟内即将执行的任务列表。把这些任务放到一个`DelayQueue`中，每个子页面开始时间到达时，从`DelayQueue`中取出，执行页面渲染，这时用户浏览到的页面就是最新的内容。<br />这里通过分布式任务调度器，可以把任务均分到各个服务器上，并且每次获取任务是指取将来5分钟内即将执行的任务列表，这个列表一般不会太多，可以直接放到队列中。当然如果很多也没关系，可以指定获取最大任务条数。通过上述处理可以控制放入`DelayQueue`的任务数，减少不必要的内存消耗。如下图：假如5分钟内即将执行的任务列表有9个，通过分布式调度分配到每台机器上的任务数即为3个：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1691904465245-a7c725f2-1fd4-4dd4-93cf-1800cca35563.jpeg)<br />关于分布式任务调度，可以使用淘宝的tbschedule等类似的框架支持，也可以自己实现。这里不详细讲解分布式任务调度怎么实现，今天的主角是`DelayQueue`—延迟队列。<br />现在已经获取到每台机器上的即将执行的任务列表，接下来就是把这些任务放到`DelayQueue`，通过其`take`方法获取到期的任务定时执行。
<a name="u8sVZ"></a>
## 2、`DelayQueue`基本特征
`DelayQueue`延迟队列同时具备：无界队列、阻塞队列、优先队列的特征。分别来看下：<br />无界队列：通过调用`DelayQueue`的offer方法（或add方法），把待执行的任务对象放入队列，该方法是非阻塞的。这个队列是无界队列，内存足够的情况下，理论上存放的任务对象数是无限的。<br />阻塞队列：`DelayQueue`实现了`BlockingQueue`接口，是一个阻塞队列。但该队列只是在取对象时阻塞，对应两个方法：1、`take()`方法，获取并移除队列头的对象，如果时间还未到，就阻塞等待。<br />2、`poll(long timeout, TimeUnit unit)` 方法，阻塞时间长度为timeout，然后获取并移除队列头的对象，如果对象延迟时间还未到，就返回null。<br />优先队列：`DelayQueue`的一个重要的成员是一个优先队列`PriorityQueue`，`PriorityQueue`内部是一个二叉小顶堆实现，其特点就是头部元素对应的权值是队列中最小的，也就是通过`poll()`方法获取到的对象是最优先的。
<a name="Ipa0S"></a>
## 3、`Delayed`接口
`DelayQueue`延迟队列中存放的对象，必须是实现`Delayed`接口的类对象。`Delayed`接口，是`Comparable`的子类：
```java
public interface Delayed extends Comparable<Delayed>
```
所有要实现Delayed接口必须重写其`getDelay`、`compareTo`方法。<br />看一个实现例子：
```java
public class TaskInfo implements Delayed {  

    //任务id  
    private int id;  

    //业务类型  
    private int type;  

    //业务数据  
    private String data;  

    //执行时间  
    private long excuteTime;  

    public TaskInfo(int id, int type, String data, long excuteTime) {  
        this.id = id;  
        this.type = type;  
        this.data = data;  
        this.excuteTime = TimeUnit.NANOSECONDS.convert(excuteTime, TimeUnit.MILLISECONDS)+System.nanoTime();  
    }  

    public int getId() {  
        return id;  
    }  

    public void setId(int id) {  
        this.id = id;  
    }  

    public int getType() {  
        return type;  
    }  

    public void setType(int type) {  
        this.type = type;  
    }  

    public String getData() {  
        return data;  
    }  

    public void setData(String data) {  
        this.data = data;  
    }  

    public long getExcuteTime() {  
        return excuteTime;  
    }  

    public void setExcuteTime(long excuteTime) {  
        this.excuteTime = excuteTime;  
    }  

    @Override  
    public long getDelay(TimeUnit unit) {  
        return unit.convert(this.excuteTime- System.nanoTime() , TimeUnit.NANOSECONDS);  
    }  

    @Override  
    public int compareTo(Delayed o) {  
        TaskInfo msg = (TaskInfo)o;  
        return this.excuteTime>msg.excuteTime?1:( this.excuteTime<msg.excuteTime?-1:0);  
    }  

}
```
通过`DelayQueue`的`offer`方法加入对象是，会根据对象`compareTo`方法把对象放到优先队列`PriorityQueue`中的指定位置；通过`DelayQueue`的`take`方法获取对象时，会调用对象的`getDelay`方法，确定延迟获取时间，需要注意的是这里的时间单位为纳秒，示例代码中通过`unit.convert(this.excuteTime- System.nanoTime() , TimeUnit.NANOSECONDS)`进行转换。
<a name="g3PXQ"></a>
## 4、`DelayQueue`使用示例
```java
public class DelayTest {  
    private static DelayQueue<TaskInfo> queue = new DelayQueue<>();//延迟队列  
    private static ExecutorService es =  Executors.newFixedThreadPool(3);//3个线程的线程池  

    public static void main(String[] args){  
        while (true) {  
            try {  
                if (queue.size() <=0){  
                    //获取任务放入队列  
                    getTask();  
                    if(queue.size() <= 0){  
                        System.out.println("没有任务睡眠10秒");  
                        //没有任务睡眠10秒  
                        Thread.sleep(10*1000);  
                    }  
                }else{  
                    TaskInfo task = queue.take();  
                    es.submit(()->{  
                        System.out.println("执行任务：" + task.getId() + ":" + task.getData());  
                    });  
                }  
            }catch (Exception e){  
                e.printStackTrace();  
            }  
        }  
    }  

    //模拟从数据库获取 将来10秒中内即将执行的任务  
    public static void getTask(){  
        Random r = new Random();  
        int t = r.nextInt(2);  
        if(t==0){  
            return;  
        }  
        TaskInfo t1 = new TaskInfo(1,1,"任务1",1000);  
        TaskInfo t2 = new TaskInfo(2,2,"任务2",2000);  
        TaskInfo t3 = new TaskInfo(3,3,"任务3",3000);  
        TaskInfo t4 = new TaskInfo(4,4,"任务4",4000);  
        TaskInfo t5 = new TaskInfo(5,5,"任务5",5000);  
        TaskInfo t6 = new TaskInfo(6,6,"任务6",6000);  
        TaskInfo t7 = new TaskInfo(7,7,"任务7",7000);  
        TaskInfo t8 = new TaskInfo(8,8,"任务8",8000);  
        queue.offer(t1);  
        queue.offer(t2);  
        queue.offer(t3);  
        queue.offer(t4);  
        queue.offer(t5);  
        queue.offer(t6);  
        queue.offer(t7);  
        queue.offer(t8);  
    }  
}
```
示例代码讲解：<br />1、首先创建了一个`DelayQueue`的延迟队列；并通过`Executors.newFixedThreadPool(3)`创建了一个3个线程数的线程池。<br />2、main方法循环体中判断如果队列中没有对象，就模拟从数据库中获取10秒内即将执行的任务，并放入`DelayQueue`。如果数据库中没有10秒内即将执行的任务，程序睡眠10秒。<br />3、如果队列中有对象，调用`DelayQueue`的`take()`方法，获取到期的任务信息，并把任务信息交给线程池进行处理。<br />实例中，模拟创建了8个任务，每个任务的延迟执行时间分别为1到8秒。<br />执行main方法，每隔1秒打印一条信息，打印完整信息如下：
```
执行任务：1:任务1  
执行任务：2:任务2  
执行任务：3:任务3  
执行任务：4:任务4  
执行任务：5:任务5  
执行任务：6:任务6  
执行任务：7:任务7  
执行任务：8:任务8
```
测试结果符合预期，这个测试示例其实就是开头业务场景的简化版实现。
<a name="ZnheS"></a>
## 5、`DelayQueue`源码解析
首先看下`DelayQueue`的成员变量：
```java
//为了保证线程安全：对队列中每次存取操作，都需要进行加锁，采用的重入锁  
private final transient ReentrantLock lock = new ReentrantLock();  

//优先队列，延迟对象最终放到改队列中，保证每次从头部取出的对象，是应该最先被执行的  
private final PriorityQueue<E> q = new PriorityQueue<E>();  

//leader线程，其等待延迟时间为优先队列中，最优先对象的延迟时间。其他线程无限期等待  
private Thread leader = null;  

//配合重入锁使用，对线程进行等待，唤醒等操作  
private final Condition available = lock.newCondition();
```
<a name="XvpQD"></a>
### 5.1. 三个加入队列方法
`add`、`offer`、`put`三个加入队列方法，其中`add`和`put`都是直接调用`offer`方法，所以调用三个方法中的任意一个都是等效的。首先看下`offer`方法：
```java
public boolean offer(E e) {  
    final ReentrantLock lock = this.lock;  
    lock.lock();//加锁  
    try {  
        q.offer(e);//调用PriorityQueue的offer方法，放入队列  
        if (q.peek() == e) {//判断刚加入的对象，是不是头节点  
            leader = null;  
            available.signal();//唤醒take()或poll(..)方法中的等待  
        }  
        return true;  
    } finally {  
        lock.unlock();//释放锁  
    }  
}
```
这个方法最值得关注的地方是，放入队列后，判断刚放入队列的对象是不是`PriorityQueue`队列的头节点，如果是需要唤醒`take()`或`poll(..)`方法中的等待阻塞，重新获取头节点对象的延迟等待时间。<br />`add`、`put`方法都是直接调用`offer`方法，源码为：
```java
public boolean add(E e) {  
    return offer(e);  
}  
public void put(E e) {  
    offer(e);  
}
```
<a name="ZkaPi"></a>
### 5.2. 四个获取对象方法
`poll()`、`poll(..)`、`take()`、`peek()`这四个方法都可以实现从队列头获取一个对象，但每个方法实现都不相同。
<a name="IUnT2"></a>
#### （1）`peek()`方法：非阻塞方法
```java
public E peek() {  
    final ReentrantLock lock = this.lock;  
    lock.lock();//加锁  
    try {  
        return q.peek();  
    } finally {  
        lock.unlock();//释放锁  
    }  
}
```
`DelayQueue`的`peek`方法，本质上调用的是`PriorityQueue`的`peek`方法，只是多了一个加锁操作。该方法会返回头部节点对象，但不会从队列中移除。peek的含义为：瞟一眼。
<a name="jQZ5n"></a>
#### （2）`poll()`方法：从队列头部获取并移除一个对象，非阻塞方法
源码实现为：
```java
public E poll() {  
    final ReentrantLock lock = this.lock;  
    lock.lock();//加锁  
    try {  
        E first = q.peek();//获取队列中的头节点对象（不会移除）  
        if (first == null || first.getDelay(NANOSECONDS) > 0)  
            return null;//调用对象的getDelay方法，如果延迟时间还未到，直接返回空  
        else  
            return q.poll();//如果延迟时间已经到达，直接调用PriorityQueue队列的取出并移除的poll方法  
    } finally {  
        lock.unlock();  
    }  
}
```
`poll()`方法 首先调用`peek`方法获取到头节点对象，通过调用对象的`getDelay`方法判断延迟时间是否到达，如果没有到达返回null，否则调用`PriorityQueue`的`poll`方法 取出并移除头节点对象 并返回。
<a name="AcGAB"></a>
#### （3）`take()`方法：`DelayQueue`的核心方法，常用于任务延迟执行，是阻塞方法
源码为：
```java
public E take() throws InterruptedException {  
    final ReentrantLock lock = this.lock;  
    lock.lockInterruptibly();//加中断锁  
    try {  
        for (;;) {  
            E first = q.peek();//获取头节点  
            if (first == null)  
                available.await();//头结点为空，释放锁无限期等待，等待offer方法放入对象，再次获得锁  
            else {  
                long delay = first.getDelay(NANOSECONDS);//获取头节点对象延迟时间  
                if (delay <= 0)  
                    return q.poll();//延迟时间已过，直接从队列中移除并取出返回  
                first = null; // don't retain ref while waiting  
                if (leader != null)  
                    available.await();//如果不是leader线程，无限期等待  
                else {  
                    Thread thisThread = Thread.currentThread();  
                    leader = thisThread;//设置当前线程为leader线程  
                    try {  
                        available.awaitNanos(delay);//释放锁，等待头结点延迟时间到来，再获得锁。  
                    } finally {  
                        if (leader == thisThread)  
                            leader = null;//释放leader线程引用  
                    }  
                }  
            }  
        }  
    } finally {  
        if (leader == null && q.peek() != null)  
            available.signal();//唤醒某一个线程，获得锁，设置leader线程  
        lock.unlock();//释放锁  
    }  
}
```
`take`方法主要实现逻辑为（for循环体）：<br />1、获取头节点对象，如果为空，线程释放锁，并进入无限期等待。等待offer方式，放入对象后，通过`signal()`方法唤醒。<br />2、如果头节点对象不为空，获取该对象的延迟时间，如果小于0，直接从队列中取出并移除该对象，返回。<br />3、如果头节点对象延迟时间大于0，判断是否“leader线程”是否已经存在，如果存在说明当前线程为“追随者线程”，进入无限期等待（等待leader线程take方法完成后，唤醒）。<br />4、如果“leader线程”不存在，把当前线程设置为“leader线程”，释放锁并等待头节点对象的延迟时间后，重新获得锁，下次循环获取头节点对象返回。<br />5、`finally`代码块，每次leader线程执行完成take方法后，需要唤醒其他线程获得锁成为新的leader线程。<br />take方法实现了一个“领导者-追随者模式”的线程处理方式，只有leader线程会等待指定时间后获得锁，其他线程都会进入无限期等待。这也是为什么在`DelayQueue`中都是使用`signal`唤醒，而不使用`signalAll`的原因（只需要一个线程成为leader线程）。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1691904072258-71b08f20-f4af-4467-b729-b30ed680560f.jpeg)<br />这个图，展示有3个线程调用`DelayQueue`的take方法，只会有一个线程成为”leader线程”，这里假设为线程1。其他两个线程为“追随者”，无限期等待，在”leader线程”执行完成之后调用signal方法随机唤醒一个线程成为新的”leader线程”。
<a name="km7oZ"></a>
#### （4）`poll(..)`方法：带延迟参数的`poll`方法，是阻塞方法
源码为：
```java
public E poll(long timeout, TimeUnit unit) throws InterruptedException {  
    long nanos = unit.toNanos(timeout);//把 指定延迟时间 转换成纳秒  
    final ReentrantLock lock = this.lock;  
    lock.lockInterruptibly();//加中断锁  
    try {  
        for (;;) {  
            E first = q.peek();  
            if (first == null) {//如果头节点为空  
                if (nanos <= 0)  
                    return null;//指定延迟时间 小于0直接返回null  
                else  
                    nanos = available.awaitNanos(nanos);//等待 指定延迟时间后，再重新获得锁  
            } else {  
                long delay = first.getDelay(NANOSECONDS);//获取头节点对象的延迟时间  
                if (delay <= 0)  
                    return q.poll();//如果对象延迟时间已过期，直接取出并移除该对象，返回  
                if (nanos <= 0)  
                    return null;//如果对象延迟时间还未到，但指定延迟时间已到，返回null  
                first = null; // don't retain ref while waiting  
                if (nanos < delay || leader != null)  
                    nanos = available.awaitNanos(nanos);//如果“指定延迟时间”小于“对象延迟时间”或者不是leader线程，等待指定时间后 再次被唤醒。  
                else {//如果“指定延迟时间”大于等于“对象延迟时间”并且 leader线程为空  
                    Thread thisThread = Thread.currentThread();  
                    leader = thisThread;//指定当前线程为leader线程  
                    try {  
                        long timeLeft = available.awaitNanos(delay);  
                        nanos -= delay - timeLeft;//重新计算最新的 “指定延迟时间"  
                    } finally {  
                        if (leader == thisThread)  
                            leader = null;  
                    }  
                }  
            }  
        }  
    } finally {  
        if (leader == null && q.peek() != null)  
            available.signal();//leader线程执行结束后，唤醒某个“追随者”线程  
        lock.unlock();  
    }  
}
```
`poll(..)`方法：如果指定的延迟时间，小于头结点对象的延迟时间，返回为空，非阻塞。<br />如果指定的延迟时间，大于头结点对象的延迟时间，会阻塞，阻塞长度为头结点对象的延迟时间。这样说会比较抽象，看一个例子：
```java
public class DelayTest2 {  
    private static DelayQueue<TaskInfo> queue = new DelayQueue<>();//延迟队列  

    public static void main(String[] args) throws Exception{  
        getTask();  
        for(int i=0;i<3;i++){//启动线程数  
            new Thread(new Runnable() {  
                @Override  
                public void run() {  
                    try {  
                        TaskInfo task = queue.poll(10000, TimeUnit.MILLISECONDS);//延迟时间  
                        if(task == null){  
                            System.out.println("任务为空");  
                        }else {  
                            System.out.println("执行任务：" + task.getId() + ":" + task.getData());  
                        }  

                    } catch (InterruptedException e) {  
                        e.printStackTrace();  
                    }  
                }  
            }).start();  
        }  

    }  

    //模拟从数据库获取 将来10秒中内即将执行的任务  
    public static void getTask(){  
        TaskInfo t1 = new TaskInfo(1,1,"任务1",1000);  
        TaskInfo t2 = new TaskInfo(2,2,"任务2",2000);  
        TaskInfo t3 = new TaskInfo(3,3,"任务3",3000);  
        TaskInfo t4 = new TaskInfo(4,4,"任务4",4000);  
        TaskInfo t5 = new TaskInfo(5,5,"任务5",5000);  
        TaskInfo t6 = new TaskInfo(6,6,"任务6",6000);  
        TaskInfo t7 = new TaskInfo(7,7,"任务7",7000);  
        TaskInfo t8 = new TaskInfo(8,8,"任务8",8000);  
        queue.offer(t1);  
        queue.offer(t2);  
        queue.offer(t3);  
        queue.offer(t4);  
        queue.offer(t5);  
        queue.offer(t6);  
        queue.offer(t7);  
        queue.offer(t8);  
    }  
}
```
该实例会启动3个线程同时调用`queue.poll(10000, TimeUnit.MILLISECONDS)`方法，其中一个线程会被设置为“leader线程”，等待时间为头结点的延迟时间，其他线程的等待时间都为10000ms。当“leader线程”执行完成后，会选择另外某个现在做为“leader线程”等待时间改为当前头结点的延迟时间。<br />执行这段代码的main方法，会每隔1秒打印一条信息，完整打印信息如下：
```
执行任务：1:任务1  
执行任务：2:任务2  
执行任务：3:任务3
```
如果把指定延迟时间改为500，即：`queue.poll(500, TimeUnit.MILLISECONDS)`，重新执行`main()`方法，该方法返回为空，这时不会阻塞，并立即打印三条消息：
```
任务为空  
任务为空  
任务为空
```
`poll(..)`方法的使用场景为：按指定时间段，分批次执行延迟队列中的任务。从源码上看，在指定延迟时间大于头节点对象延迟时间时的实现 跟`take()`方法很像，只是“追随者线程”的等待时间有区别：`poll(..)`方法是等待指定延迟时间，`take()`方法是无限期等待。<br />`DelayQueue`的其他方法都比较简单（remove，clear等），这里不再一一列举。
