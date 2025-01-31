JavaNetty
<a name="ZPVDZ"></a>
## 对 Reactor 的理解
Reactor 是个啥呢？其实它算是一个编程模式或者说一个架构模式，常用在**服务端**网络通信相关的模块。<br />Reactor 直译到中文是反应堆，虽然直译不贴切，但确实跟“反应”有关。<br />反应啥呢？**对触发的事件进行反应**。<br />在大学的时候，相信很多人都做过图书管理系统等类似的大作业，这种都是基于 Java GUI 做的，当然具体如何实现的不重要，重要的是完成之后的成品是不是点击一个按钮就会弹出一个对应的弹框？<br />点击按钮就会弹框，不同的按钮弹不同的框，这就是反应。<br />计算机不就讲究抽象，所以把这上面说的这些抽象一下，**就是针对不同的事件需要有不同的处理逻辑（方法调用）**。<br />到这里得到了两个概念：事件（点击按钮）和处理逻辑（弹对应的框）。<br />再回到弹框这个场景，思考下，到底是谁在监听着按钮被点击的动作来弹出不同的框呢？<br />这里，很容易想到两种情况：

1. 一个按钮派一个线程在守着，只要按钮被点击了，这个线程就执行弹框
2. 一个线程轮询所有按钮的情况，死循环查看只要有一个按钮被点击了，就找出按钮绑定的框，然后弹

如果按钮很少，其实多派几个线程守着影响不大，假设有一千个、一万个按钮呢？这种场景下第二种实现更优，毕竟按钮也不是一直会被点击着，对吧。<br />把“人”、“点击按钮”、“弹框”对应到网络编程中，就是线程、事件、处理。<br />第一种情况翻译过来就是一个线程接待一个连接，每当连接上有事件产生，则线程会对应事件作出响应。<br />这对应着很早之前的网络编程模型，那时候网上没那么多用户，并且也就只有阻塞 I/O，如果连接没有反应，那么线程就得阻塞等待着 I/O 事件的发生。<br />第二种情况翻译过来就是，由一个线程接待多个连接，不论哪个连接上有事件产生，这个线程都会根据事件找到对应的处理逻辑来作出响应。<br />这就是对应现在流行的基于非阻塞 I/O 的 I/O 多路复用，这种场景就适合海量用户的情况，服务端可用很少线程来处理数量庞大的连接，就像上面说的，毕竟连接（按钮）也不是一直会有请求过来（被点击）。<br />至于前头提到的事件，**基础的** I/O 上的事件也就这三种：连接事件、读事件、写事件。<br />现在回到网络 I/O 上，再来看 Reactor：也就是有一个 selector（select/poll/epoll） 线程，它管理着很多连接，只要某个连接上有事件产生，就会唤醒 selector 线程，这个线程就会根据发生的事件类型做不同的处理。<br />这就是 Reactor，对应的线程也叫 Reactor 线程（就靠它起反应啦）。<br />那没 Reactor 的场景是怎样的？<br />就是上面提的第一种情况，一个线程接待一条连接。不像第二种情况这样，由一个线程来接待许多连接，由一个“负责人”来接待多个客户，这种什么事都找一个人，是不是感受起来那个人就像一个 Reactor？<br />**能一对多的根据不同请求做出不同响应实现**，是个人认为的 Reactor 核心。至于事件、调度器、Acceptor等玩意，是为了写论文或者文章必须要搞的一些概念，反正理解了之后，也就这么回事儿，因为要从流程上走通必须得有这么些个东西。<br />就像 SpringMVC，要想一个请求能找到对应的 Controller，那不就得有个统一入口判断路由，所以必须要有这么个东西，不然请求到不了 Controller 了啊，那这个玩意叫啥？不就是 `DispatcherServlet` 吗， 中文名不就是前置控制器吗。<br />所以，不是说 SpringMVC 需要有 `DispatcherServlet` 这么个玩意，而是 SpringMVC 需要有个统一入口判断路由，所以就弄了个实现类，这个实现类呢就叫 `DispatcherServlet`。<br />同样看 Reactor 也是一样的，不是说 Reactor 需要有个叫 `Demultiplexer`、 `Dispatcher`、`Handle`、Event Handler 这么些个玩意才叫 Reactor 。<br />而是在网络 I/O 中基于非阻塞 I/O ，且需要少量的线程接待大量的连接，这样一场景下，就必须要个玩意作为一个监听者，监听底层这么多连接是否有请求，然后根据请求的类型（抽象的事件），指派调用不同的处理逻辑，才对应衍生出上面这么些个名词。<br />在具体的实现上， `Demultiplexer` 和 `Dispatcher` 当然可以是同一个线程，同在一个死循环里面，这都是 ok 的，不要看着名词就理解着，代码里必须分这么两个玩意，不是的哈。<br />基于上面的理解，然后再看看来自 Reactor 论文的这张图把，是不是好理解了。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679112664699-f4aff4c3-9975-485c-86df-cb9edf996e0f.png#averageHue=%23efe0de&clientId=u11daef29-2853-4&from=paste&id=uc7862112&originHeight=358&originWidth=560&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc8dccc76-a5e3-408c-a67a-5d8e0ecd025&title=)<br />最后再说个官方点的回答吧：
> Reactor 是服务端在网络编程时的一个编程模式，主要由一个基于 Selector （底层是 select/poll/epoll）的死循环线程，也称为 Reactor 线程。将 I/O 操作抽象成不同的事件，每个事件都配置对应的回调函数，由 Selector 监听连接上事件的发生，再进行分发调用相应的回调函数进行事件的处理。

<a name="TA8Zb"></a>
## Netty 的 Reactor
都知道 Netty 可以有两个线程组，一个是 bossGroup，一个是 workerGroup。<br />之前也提到了 bossGroup 主要是接待新连接(老板接活)，workerGroup 是负责新连接后续的一切 I/O （员工干活）<br />对应到 Reactor 模型中，bossGroup 中的 eventLoop 就是主 Reactor。它的任务就是监听等待连接事件的到来，即 OP_ACCEPT。<br />然后创建子 channel ，从 workerGroup 中选择一个 eventLoop ，将子 channel 与这个 eventLoop 绑定，之后这个子 channel 对应的 I/O事件，都由这个 eventLoop 负责。<br />而这个 workerGroup 中的 eventLoop 就是所谓的子 Reactor，它的任务就是负责已经建连完毕的连接之后的所有 I/O 请求。<br />其实从 eventLoop 这个名字就能看出，它的作用就是 loop event，说白了就是一个线程，死循环的等待事件的发生，然后根据不同的事件类型进行不一样的后续处理，仅此而已。<br />正常情况下 bossGroup 只会配置一个 eventLoop，即一个线程，因为一般服务只会暴露一个端口，所以只要一个 eventLoop 监听这个端口，然后 accept 连接。<br />而 workerGroup 在 Netty 中，默认是 cpu 核心数*2，例如 4 核 CPU ，默认会在 workerGroup 建 8 个 eventLoop，所以就有 8 个子 Reactor。<br />所以正常 Netty 服务端的配置是，1个主 Reactor，多个从 Reactor，这就是所谓的主从 Reactor。<br />基本上现在的主流配置都是主从 Reactor。
<a name="WOUH4"></a>
## 关于 Reactor 模型的演进
在深入 Netty 的 Reactor 实现之前，先来看看，为什么会演变成主从 Reactor？<br />最开始的模型是**单 Reactor 单线程** ，可以理解成一个线程来即监听新的连接，又要响应老的连接的请求，如果逻辑处理的很快，那没有问题，看看人家 redis 就够用，但是如果逻辑处理的慢，那就会阻塞其他请求。<br />所以就有了**单 Reactor 多线程**，还是由一个线程来监听所有的底层 Socket，但是一些耗时的操作可以分配给线程池进行业务处理，这样就不会因为逻辑处理慢导致 Reactor 的阻塞。<br />但是这个模型还会有瓶颈，即监听新的连接和响应老的连接的请求都由一个线程处理，积累的老连接多了，有很多事件需要响应，就会影响新连接的接入，这就不太舒服了，况且现在都是多核 CPU，还差这么一个线程吗？<br />所以就又演进成**主从 Reactor**，由一个线程，即主 Reactor 专门等待新连接的建连，然后创建多个线程作为子 Reactor，均匀的负责已经接入的老连接，这样一来既不会影响接待新连接的速度，也能更好的利用多核 CPU 的能力响应老连接的请求。<br />这就是关于 Reactor 模型的演进了。<br />好了，接下来再看看 Netty 实现 Reactor 的核心类，现在一般都是用 NIO ，所以看 `NioEventLoop` 这个类。<br />友情提示，有条件建议在PC端看下面的内容，源码类的手机上看不太舒服
<a name="EnuaR"></a>
## NioEventLoop
前面已经提到一个 NioEventLoop 就是一个线程，那线程的核心肯定就是它的 run 方法。<br />基于我们的理解，知道这个 run 方法的主基调肯定是死循环等待 I/O 事件产生，然后处理事件。<br />事实也是如此， NioEventLoop 主要做了三件事：

1. select 等待 I/O 事件的发生
2. 处理发生的 I/O 事件
3. 处理提交至线程中的任务，包括提交的异步任务、定时任务、尾部任务。

首先折叠下代码，可以看到妥妥的死循环，这也是 Reactor 线程的标配，这辈子无限只为了等待事件发生且处理事件。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679112413630-12fedd3b-b155-48e3-97ff-b2232e5dafc7.png#averageHue=%232f2d2b&clientId=ubc2e8322-ea5a-4&from=paste&id=ue54fa268&originHeight=103&originWidth=398&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9b047b0a-0eb6-4c07-9dcd-4ae6d57f6f4&title=)<br />在 Netty 的实现里，NioEventLoop 线程不仅要处理 I/O 事件，还需要处理提交的异步任务、定时任务和尾部任务，所以这个线程需要平衡 I/O 事件处理和任务处理的时间。<br />因此有个 selectStrategy 这样的策略，根据判断当前是否有任务在等待被执行，如果有则立即进行一次不会阻塞的 select 来尝试获取 I/O 事件，如果没任务则会选择 SelectStrategy.SELECT 这个策略。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679112413722-a7b79d32-0c2d-451b-b2c1-942df0ac9682.png#averageHue=%232d2c2c&clientId=ubc2e8322-ea5a-4&from=paste&id=u52123f95&originHeight=662&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u290fb0fc-04a9-4683-b7c5-bb65cd31eef&title=)<br />从图中也可以看到，这个策略会根据最近将要发生的定时任务的执行时间来控制 select 最长阻塞的时间。<br />从下面的代码可以看到，根据定时任务即将执行的时间还预留了 5 微秒的时间窗口，如果 5 微秒内就要到了，那就不阻塞了，直接进行一个非阻塞的 select 立刻尝试获取 I/O 事件。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679112413647-716b4af9-045f-4f2e-b802-41cb567a5a98.png#averageHue=%232d2c2b&clientId=ubc2e8322-ea5a-4&from=paste&id=ua863f1c7&originHeight=216&originWidth=998&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u67bef9bf-5e39-4021-88eb-3d4caac9d89&title=)<br />经过上面的这个操作，select 算是完毕了，最终会把就绪的 I/O 事件个数赋值给 strategy，如果没有的话那 strategy 就是 0 ，接着就该处理 I/O 事件和任务了。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679112413598-1092f11c-5106-4d83-ad13-48b063a531e1.png#averageHue=%232c2b2b&clientId=ubc2e8322-ea5a-4&from=paste&id=u2bfa3918&originHeight=681&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uccd6d78f-64e3-48fa-af72-4792cd15ccb&title=)<br />上面代码把重点几个部分都框出来了，这里有个 selectCnt 来统计 select 的次数，这个用于处理 JDK Selector 空轮询的 bug ，下面会提。<br />ioRatio 这个参数用来控制 I/O 事件执行的时间和任务执行时间的占比，毕竟一个线程要做多个事情，要做到**雨露均沾**对吧，不能冷落了谁。<br />可以看到，具体的实现是记录 I/O 事件的执行时间，然后再根据比例算出任务能执行的最长的时间来控制任务的执行。
<a name="nlhee"></a>
### I/O 事件的处理
来看看 I/O 事件具体是如何处理的，也就是 processSelectedKeys 方法。<br />点进去可以看到，实际上会有两种处理的方法，一种是优化版，一种是普通版。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679112413690-354432e7-6b11-4c16-883d-9ed02c158e3d.png#averageHue=%232e2c2b&clientId=ubc2e8322-ea5a-4&from=paste&id=u0efb131e&originHeight=183&originWidth=687&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u29321425-45bf-4ee0-ba27-56964803b7c&title=)<br />这两个版本的逻辑都是一样的，区别就在于优化版会替换 selectedKeys 的类型，JDK 实现的 selectedKeys 是 set 类型，而 Netty 认为这个类型的选择还是有优化的余地的。<br />Netty 用 SelectedSelectionKeySet 类型来替换了 set 类型，其实就是用数组来替换了 set<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679112413920-66a3cf5b-8d2a-43b2-8478-68769a90ec30.png#averageHue=%232e2c2b&clientId=ubc2e8322-ea5a-4&from=paste&id=uadeaccf2&originHeight=129&originWidth=363&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u55467177-5efd-4b6a-bc8f-7ab4ceda742&title=)<br />相比 set 类型而言，数组的遍历更加高效，其次数组尾部添加的效率也高于 set，毕竟 set 还可能会有 hash冲突。当然这是 Netty 为追求底层极致优化所做的，平日的代码没必要这般“斤斤计较”，意义不大。<br />那 Netty 是通过什么办法替换了这个类型呢？<br />**反射**。<br />看下代码哈，不是很复杂：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679112414062-999cd6b7-f99e-4046-bc0b-8fd710359130.png#averageHue=%232d2c2c&clientId=ubc2e8322-ea5a-4&from=paste&id=u169cb7ca&originHeight=652&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u76be07f1-9f5a-418a-b333-7b230f72ee1&title=)<br />这也能给我们提供一些思路，比方调用三方提供的 jar 包，无法修改它的源码，但是又想对它做一些增强，那么就可以仿照 Netty 的做法，通过反射来替换之~<br />打个断点看下替换前后 selectedKey 的类型，之前是 HashSet：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679112414014-b838ef5a-db47-43b6-9d47-d9683f6119ef.png#averageHue=%236f816a&clientId=ubc2e8322-ea5a-4&from=paste&id=u1556317f&originHeight=488&originWidth=932&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uabc9a712-08bd-4c6d-8bc3-7b5ff799525&title=)<br />替换了后就变成了 SelectedSelectionKeySet 了。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679112414019-99ef0503-882d-4c23-9f18-2ca659ef609c.png#averageHue=%236d816c&clientId=ubc2e8322-ea5a-4&from=paste&id=u13c994be&originHeight=504&originWidth=870&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u742317ad-921f-4d1a-a1c1-1b7988a5c73&title=)<br />现在再看下优化版的处理 I/O 事件的遍历方法，和普通版逻辑一样的，只是遍历是利用数组罢了。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679112414186-c576ca6c-0757-4043-8241-f420ad92ca30.png#averageHue=%232d2c2b&clientId=ubc2e8322-ea5a-4&from=paste&id=u1d39d86f&originHeight=500&originWidth=922&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5bbbc959-31f1-4878-a325-38d8bcb81d5&title=)<br />没啥好说的，就那个帮助 GC 可以提一下，如果看过很多开源软件就会发现有很多这样的实现，直接置为 null 的语句，这是为了帮助 GC。<br />紧接着看下真正处理 I/O 事件的方法 `processSelectedKey`<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679112414211-75387668-e24d-4d68-9530-102a6bc82d33.png#averageHue=%232c2c2b&clientId=ubc2e8322-ea5a-4&from=paste&id=u18f9a6a3&originHeight=714&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud8cc9f3b-1a74-4c87-bf5a-ca75c0ccfa1&title=)<br />可以看到，这个方法本质就是根据不同的事件进行不同的处理，实际上会将事件在对应的 channel 的 pipeline 上面传播，并且触发各种相应的自定义事件，拿 `OP_ACCEPT` 事件作为例子分析。<br />针对 `OP_ACCEPT` 事件，`unsafe.read` 实际会调用 `NioMessageUnsafe#read` 方法。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679112414392-6f927a14-01df-4d00-ba1e-ed431647abd0.png#averageHue=%232c2c2b&clientId=ubc2e8322-ea5a-4&from=paste&id=uea40b5fe&originHeight=535&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubd439085-7d35-498f-a138-14c4ca6ffb2&title=)<br />从上面代码来看，逻辑并不复杂，主要就是循环读取新建立的子 channel，并触发 ChannelRead 和 `ChannelReadComplete` 事件，使之在 pipeline 中传播，期间就会触发之前添加的 `ServerBootstrapAcceptor#channelRead`，将其分配给 workerGroup 中的 eventLoop ，即子 Reactor 线程。<br />当然，自定义的 handler 也可以实现这两个事件方法，这样对应的事件到来后，能进行相应的逻辑处理。<br />好了，Netty 的 `OP_ACCEPT` 事件处理分析到此结束，其他事件也是类似的，都会触发相应的事件，然后在 pipeline 中传递，触发不同 `Channelhandler` 的方法，进行逻辑处理。<br />以上，就是 Netty 实现的主从 Reactor 模型。<br />当然，Netty 也支持单 Reactor，无非就是不要 workerGroup，至于线程数也可以自行配置，十分灵活，不过现在一般用的都是主从 Reactor 模型。
<a name="rYVb5"></a>
## 最后
这篇不仅讲了 Netty 的 Reactor 实现，也把 Netty 是如何处理  I/O 操作的部分也囊括了。
