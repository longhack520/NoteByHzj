`CompletionService`的实现目标是**任务先完成可优先获取到，即结果按照完成先后顺序排序。**
<a name="lo5l4"></a>
## `ExecutorService`和`CompletionService`对比
假设有 4 个任务(A, B, C, D)用来执行复杂的计算，每个任务的执行时间随着输入参数的不同而不同，如果将任务提交到 `ExecutorService`， 相信你已经可以“信手拈来”
```java
ExecutorService executorService = Executors.newFixedThreadPool(4);
List<Future> futures = new ArrayList<Future<Integer>>();
futures.add(executorService.submit(A));
futures.add(executorService.submit(B));
futures.add(executorService.submit(C));
futures.add(executorService.submit(D));

// 遍历 Future list，通过 get() 方法获取每个 future 结果
for (Future future:futures) {
    Integer result = future.get();
    // 其他业务逻辑
}
```
先直入主题，用 `CompletionService` 实现同样的场景
```java
ExecutorService executorService = Executors.newFixedThreadPool(4);

// ExecutorCompletionService 是 CompletionService 唯一实现类
CompletionService executorCompletionService= new ExecutorCompletionService<>(executorService );

List<Future> futures = new ArrayList<Future<Integer>>();
futures.add(executorCompletionService.submit(A));
futures.add(executorCompletionService.submit(B));
futures.add(executorCompletionService.submit(C));
futures.add(executorCompletionService.submit(D));

// 遍历 Future list，通过 get() 方法获取每个 future 结果
for (int i=0; i<futures.size(); i++) {
    Integer result = executorCompletionService. take().get();
    // 其他业务逻辑
}
```
两种方式在代码实现上几乎一毛一样，JDK 中不会重复造轮子，如果要造一个新轮子，必定是原有的轮子在某些场景的使用上有致命缺陷<br />既然新轮子出来了，二者到底有啥不同呢？`Future get()` 方法的致命缺陷：<br />如果 Future 结果没有完成，调用 `get()` 方法，程序会阻塞在那里，直至获取返回结果<br />先来看第一种实现方式，假设任务 A 由于参数原因，执行时间相对任务 B,C,D 都要长很多，但是按照程序的执行顺序，程序在 get() 任务 A 的执行结果会阻塞在那里，导致任务 B,C,D 的后续任务没办法执行。又因为每个任务执行时间是不固定的，所以无论怎样调整将任务放到 List 的顺序，都不合适，这就是致命弊端<br />新轮子自然要解决这个问题，它的设计理念就是哪个任务先执行完成，`get()` 方法就会获取到相应的任务结果，这么做的好处是什么呢？来看个图就瞬间理解了<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1700531424778-8520d63a-89f8-4898-b3d6-fe1a935e1e0f.png#averageHue=%23171615&clientId=u241888a8-bd8e-4&from=paste&id=u85085399&originHeight=573&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u68dee8c9-9898-4156-bd20-1d0e81dc62b&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1700531424738-5df82841-1421-411b-9dd2-6aae02e87e1c.png#averageHue=%23171615&clientId=u241888a8-bd8e-4&from=paste&id=u5cc92d9d&originHeight=564&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u28dc07f3-f18b-44b6-ae05-4803d514b01&title=)<br />两张图一对比，执行时长高下立判了，在当今高并发的时代，这点时间差，在吞吐量上起到的效果可能不是一点半点了<br />那 `CompletionService` 是怎么做到获取最先执行完的任务结果的呢？
<a name="rnNIE"></a>
## 远看`CompletionService` 轮廓
如果使用过消息队列，你应该秒懂我要说什么了，`CompletionService` 实现原理很简单<br />就是一个将异步任务的生产和任务完成结果的消费解耦的服务<br />用人话解释一下上面的抽象概念只能再画一张图了<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1700531424743-697fc801-2f8b-41be-b98d-dde89ecaf9d9.png#averageHue=%23141313&clientId=u241888a8-bd8e-4&from=paste&id=u84cdfd5a&originHeight=425&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2d169667-ab03-4cde-9bd1-e7a9eebb31c&title=)<br />说白了，哪个任务执行的完，就直接将执行结果放到队列中，这样消费者拿到的结果自然就是最早拿到的那个了<br />从上图中看到，有任务，有结果队列，那 `CompletionService` 自然也要围绕着几个关键字做文章了

- 既然是异步任务，那自然可能用到 Runnable 或 Callable
- 既然能获取到结果，自然也会用到 Future 了

带着这些线索，走进 CompletionService 源码看一看
<a name="BW3BM"></a>
## 近看 `CompletionService` 源码
`CompletionService` 是一个接口，它简单的只有 5 个方法：
```java
Future<V> submit(Callable<V> task);
Future<V> submit(Runnable task, V result);
Future<V> take() throws InterruptedException;
Future<V> poll();
Future<V> poll(long timeout, TimeUnit unit) throws InterruptedException;
```
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1700532009903-54141dc2-349f-4a0e-a985-3f262be11685.png#averageHue=%23f6f6f6&clientId=u241888a8-bd8e-4&from=paste&height=666&id=uc84e559a&originHeight=1665&originWidth=2099&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=236560&status=done&style=none&taskId=u7ea856b6-b3d3-4f56-afee-fd5defdab7d&title=&width=839.6)<br />另外 3 个方法都是从阻塞队列中获取并移除阻塞队列第一个元素，只不过他们的功能略有不同

- **Take**：如果队列为空，那么调用 `take()` 方法的线程会被阻塞
- **Poll**：如果队列为空，那么调用 `poll()` 方法的线程会返回 null
- **Poll-timeout**：以超时的方式获取并移除阻塞队列中的第一个元素，如果超时时间到，队列还是空，那么该方法会返回 null

所以说，按大类划分上面5个方法，其实就是两个功能

- 提交异步任务 （submit）
- 从队列中拿取并移除第一个元素 (take/poll)

`CompletionService` 只是接口，`ExecutorCompletionService` 是该接口的唯一实现类
<a name="JrRnr"></a>
### `ExecutorCompletionService` 源码分析
先来看一下类结构，实现类里面并没有多少内容<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1700532071895-dfb9d232-bc1e-4d80-a160-9afe4b298e68.png#averageHue=%23fefde1&clientId=u241888a8-bd8e-4&from=paste&id=u74693fe2&originHeight=854&originWidth=1283&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=116313&status=done&style=none&taskId=uaeb6f789-11d3-44c8-a3cf-d9edda39260&title=)<br />`ExecutorCompletionService` 有两种构造函数：
```java
private final Executor executor;
private final AbstractExecutorService aes;
private final BlockingQueue<Future<V>> completionQueue;

public ExecutorCompletionService(Executor executor) {
    if (executor == null)
        throw new NullPointerException();
    this.executor = executor;
    this.aes = (executor instanceof AbstractExecutorService) ?
        (AbstractExecutorService) executor : null;
    this.completionQueue = new LinkedBlockingQueue<Future<V>>();
}
public ExecutorCompletionService(Executor executor,
                                 BlockingQueue<Future<V>> completionQueue) {
    if (executor == null || completionQueue == null)
        throw new NullPointerException();
    this.executor = executor;
    this.aes = (executor instanceof AbstractExecutorService) ?
        (AbstractExecutorService) executor : null;
    this.completionQueue = completionQueue;
}
```
两个构造函数都需要传入一个 Executor 线程池，因为是处理异步任务的，是不被允许手动创建线程的，所以这里要使用线程池也就很好理解了<br />另外一个参数是 `BlockingQueue`，如果不传该参数，就会默认队列为 `LinkedBlockingQueue`，任务执行结果就是加入到这个阻塞队列中的<br />所以要彻底理解 `ExecutorCompletionService`，只需要知道一个问题的答案就可以了：<br />它是如何将异步任务结果放到这个阻塞队列中的？<br />想知道这个问题的答案，那只需要看它提交任务之后都做了些什么？
```java
public Future<V> submit(Callable<V> task) {
    if (task == null) throw new NullPointerException();
    RunnableFuture<V> f = newTaskFor(task);
    executor.execute(new QueueingFuture(f));
    return f;
}
```
前面也分析过，`execute` 是提交 Runnable 类型的任务，本身得不到返回值，但又可以将执行结果放到阻塞队列里面，所以肯定是在 `QueueingFuture` 里面做了文章<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1700533087482-91f42210-faf5-4990-95ab-0d92d7122112.png#averageHue=%23f6f6f6&clientId=u241888a8-bd8e-4&from=paste&height=604&id=u7543bd17&originHeight=1510&originWidth=1419&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=127373&status=done&style=none&taskId=ub0dfaba5-8abc-441f-9d00-f5e7f03b159&title=&width=567.6)<br />从上图中看一看出，`QueueingFuture` 实现的接口非常多，所以说也就具备了相应的接口能力。<br />重中之重是，它继承了 `FutureTask` ，`FutureTask` 重写了 `Runnable` 的 `run()` 方法 (方法细节分析可以查看`FutureTask`源码分析 ) 文中详细说明了，无论是`set()` 正常结果，还是`setException()` 结果，都会调用 `finishCompletion()` 方法:
```java
private void finishCompletion() {
    // assert state > COMPLETING;
    for (WaitNode q; (q = waiters) != null;) {
        if (UNSAFE.compareAndSwapObject(this, waitersOffset, q, null)) {
            for (;;) {
                Thread t = q.thread;
                if (t != null) {
                    q.thread = null;
                    LockSupport.unpark(t);
                }
                WaitNode next = q.next;
                if (next == null)
                    break;
                q.next = null; // unlink to help gc
                q = next;
            }
            break;
        }
    }

    // 重点 重点 重点
    done();

    callable = null;        // to reduce footprint
}
```
上述方法会执行 `done()` 方法，而 `QueueingFuture` 恰巧重写了 `FutureTask` 的 `done()` 方法：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1700532426669-a91df6e7-db97-4c3a-a3c9-495dbe6f2dd6.png#averageHue=%23fbfafa&clientId=u241888a8-bd8e-4&from=paste&height=236&id=ue9c69495&originHeight=590&originWidth=2230&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=142712&status=done&style=none&taskId=u2e4c19e9-c0fd-41ae-90f8-809c8ee04bf&title=&width=892)<br />方法实现很简单，就是将 task 放到阻塞队列中
```java
protected void done() { 
    completionQueue.add(task); 
}
```
执行到此的 task 已经是前序步骤 set 过结果的 task，所以就可以通过消费阻塞队列获取相应的结果了<br />相信到这里，`CompletionService` 应该没什么秘密可言了
<a name="gJdat"></a>
## `CompletionService` 的主要用途
在 JDK docs 上明确给了两个例子来说明 `CompletionService` 的用途：<br />假设有一组针对某个问题的solvers，每个都返回一个类型为Result的值，并且想要并发地运行它们，处理每个返回一个非空值的结果，在某些方法使用(Result r)<br />其实就是文中开头的使用方式
```java
void solve(Executor e,
            Collection<Callable<Result>> solvers)
     throws InterruptedException, ExecutionException {
     CompletionService<Result> ecs
         = new ExecutorCompletionService<Result>(e);
     for (Callable<Result> s : solvers)
         ecs.submit(s);
     int n = solvers.size();
     for (int i = 0; i < n; ++i) {
         Result r = ecs.take().get();
         if (r != null)
             use(r);
     }
 }
```
假设想使用任务集的第一个非空结果，忽略任何遇到异常的任务，并在第一个任务准备好时取消所有其他任务
```java
void solve(Executor e,
            Collection<Callable<Result>> solvers)
     throws InterruptedException {
     CompletionService<Result> ecs
         = new ExecutorCompletionService<Result>(e);
     int n = solvers.size();
     List<Future<Result>> futures
         = new ArrayList<Future<Result>>(n);
     Result result = null;
     try {
         for (Callable<Result> s : solvers)
             futures.add(ecs.submit(s));
         for (int i = 0; i < n; ++i) {
             try {
                 Result r = ecs.take().get();
                 if (r != null) {
                     result = r;
                     break;
                 }
             } catch (ExecutionException ignore) {}
         }
     }
     finally {
         for (Future<Result> f : futures)
            // 注意这里的参数给的是 true，详解同样在前序 Future 源码分析文章中
             f.cancel(true);
     }

     if (result != null)
         use(result);
 }
```
这两种方式都是非常经典的 `CompletionService` 使用范式 ，请大家仔细品味每一行代码的用意<br />范式没有说明 Executor 的使用，使用 `ExecutorCompletionService`，需要自己创建线程池，看上去虽然有些麻烦，但好处是可以让多个 `ExecutorCompletionService` 的线程池隔离，这种隔离性能避免几个特别耗时的任务拖垮整个应用的风险 （这也是反复说过多次的，不要所有业务共用一个线程池）
<a name="k5Wdf"></a>
## 总结
`CompletionService` 的应用场景还是非常多的，比如

- Dubbo 中的 Forking Cluster
- 多仓库文件/镜像下载（从最近的服务中心下载后终止其他下载过程）
- 多服务调用（天气预报服务，最先获取到的结果）

`CompletionService` 不但能满足获取最快结果，还能起到一定 "load balancer" 作用，获取可用服务的结果，使用也非常简单， 只需要遵循范式即可。
