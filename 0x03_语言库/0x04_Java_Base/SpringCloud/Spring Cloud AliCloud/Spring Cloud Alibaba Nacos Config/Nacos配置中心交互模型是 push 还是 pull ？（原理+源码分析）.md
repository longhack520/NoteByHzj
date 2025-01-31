JavaSpringCloud Alibaba Nacos<br />Nacos在做配置中心的时候，配置数据的交互模式是服务端推过来还是客户端主动拉的？<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658368254524-1be69271-9059-4265-895f-a6ff6585df9c.png#clientId=uba7ad15e-6301-4&from=paste&id=u37d15a7e&originHeight=512&originWidth=832&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5f15b38f-1484-4372-a989-eb3eaecc10b&title=)<br />这里我先抛出答案：客户端主动拉的！<br />接下来咱们扒一扒Nacos的源码，来看看它具体是如何实现的？
<a name="wlYHs"></a>
## 配置中心
聊Nacos之前简单回顾下配置中心的由来。<br />简单理解配置中心的作用就是对配置统一管理，修改配置后应用可以动态感知，而无需重启。<br />因为在传统项目中，大多都采用静态配置的方式，也就是把配置信息都写在应用内的yml或properties这类文件中，如果要想修改某个配置，通常要重启应用才可以生效。<br />但有些场景下，比如想要在应用运行时，通过修改某个配置项，实时的控制某一个功能的开闭，频繁的重启应用肯定是不能接受的。<br />尤其是在微服务架构下，我们的应用服务拆分的粒度很细，少则几十多则上百个服务，每个服务都会有一些自己特有或通用的配置。假如此时要改变通用配置，难道要挨个改几百个服务配置？很显然这不可能。所以为了解决此类问题配置中心应运而生。<br />![配置中心](https://cdn.nlark.com/yuque/0/2022/png/396745/1658368254677-8856099b-61cb-4256-991a-f75b460b14c1.png#clientId=uba7ad15e-6301-4&from=paste&id=uf5caa208&originHeight=497&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u2a8e2d8a-7a2e-4b3c-923b-740ce7fa9cc&title=%E9%85%8D%E7%BD%AE%E4%B8%AD%E5%BF%83 "配置中心")
<a name="xc30s"></a>
## 推与拉模型
客户端与配置中心的数据交互方式其实无非就两种，要么推push，要么拉pull。
<a name="peCfg"></a>
### 推模型
客户端与服务端建立TCP长连接，当服务端配置数据有变动，立刻通过建立的长连接将数据推送给客户端。<br />优势：长链接的优点是实时性，一旦数据变动，立即推送变更数据给客户端，而且对于客户端而言，这种方式更为简单，只建立连接接收数据，并不需要关心是否有数据变更这类逻辑的处理。<br />弊端：长连接可能会因为网络问题，导致不可用，也就是俗称的假死。连接状态正常，但实际上已无法通信，所以要有的心跳机制KeepAlive来保证连接的可用性，才可以保证配置数据的成功推送。
<a name="duVj5"></a>
### 拉模型
客户端主动的向服务端发请求拉配置数据，常见的方式就是轮询，比如每3s向服务端请求一次配置数据。<br />轮询的优点是实现比较简单。但弊端也显而易见，轮询无法保证数据的实时性，什么时候请求？间隔多长时间请求一次？都是不得不考虑的问题，而且轮询方式对服务端还会产生不小的压力。
<a name="kCaeg"></a>
## 长轮询
开篇就给出了答案，nacos采用的是客户端主动拉pull模型，应用长轮询（Long Polling）的方式来获取配置数据。<br />额？以前只听过轮询，长轮询又是什么鬼？它和传统意义上的轮询（暂且叫短轮询吧，方便比较）有什么不同呢？
<a name="XvqfA"></a>
### 短轮询
不管服务端配置数据是否有变化，不停的发起请求获取配置，比如支付场景中前段JS轮询订单支付状态。<br />这样的坏处显而易见，由于配置数据并不会频繁变更，若是一直发请求，势必会对服务端造成很大压力。还会造成推送数据的延迟，比如：每10s请求一次配置，如果在第11s时配置更新了，那么推送将会延迟9s，等待下一次请求。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658368254584-50f853aa-b8b9-448f-9f6b-798cd6287de0.png#clientId=uba7ad15e-6301-4&from=paste&id=uc7cfe885&originHeight=168&originWidth=528&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4f569ab0-0a74-401e-bffd-67977bad5f6&title=)<br />为了解决短轮询的问题，有了长轮询方案。
<a name="OGFa7"></a>
### 长轮询
长轮询可不是什么新技术，它不过是由服务端控制响应客户端请求的返回时间，来减少客户端无效请求的一种优化手段，其实对于客户端来说与短轮询的使用并没有本质上的区别。<br />客户端发起请求后，服务端不会立即返回请求结果，而是将请求挂起等待一段时间，如果此段时间内服务端数据变更，立即响应客户端请求，若是一直无变化则等到指定的超时时间后响应请求，客户端重新发起长链接。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658368254563-d35b95c5-fd2e-41df-8554-3a4821141e02.png#clientId=uba7ad15e-6301-4&from=paste&id=u1435c62b&originHeight=363&originWidth=754&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc135b34b-2c61-4ab1-bfd4-98a51e800c4&title=)
<a name="j3BjV"></a>
## Nacos初识
为了后续演示操作方便在本地搭了个Nacos。**注意：** 运行时遇到个小坑，由于Nacos默认是以cluster集群的方式启动，而本地搭建通常是单机模式standalone，这里需手动改一下启动脚本startup.X中的启动模式。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658368254581-f1581638-70b5-4a02-a0e4-6e187af3b84b.png#clientId=uba7ad15e-6301-4&from=paste&id=u4821e586&originHeight=224&originWidth=782&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u95344917-d8be-4439-83c5-964e7e31b51&title=)<br />直接执行/bin/startup.X就可以了，默认用户密码均是nacos。![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658368255071-53f8b171-9e82-45ed-9c7e-3b501cef4a0b.png#clientId=uba7ad15e-6301-4&from=paste&id=uf991c28e&originHeight=523&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ufcfb6cac-7d11-4d05-a1d3-8d5ed482bd7&title=)
<a name="a23po"></a>
## 几个概念
Nacos配置中心的几个核心概念：dataId、group、namespace，它们的层级关系如下图：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658368255152-05ba875c-ac0e-4d34-95c9-890f62fe27c4.png#clientId=uba7ad15e-6301-4&from=paste&id=u869a7778&originHeight=632&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u554b4674-51bf-4741-adcc-ec92f649525&title=)<br />dataId：是配置中心里最基础的单元，它是一种key-value结构，key通常是的配置文件名称，比如：application.yml、mybatis.xml，而value是整个文件下的内容。<br />目前支持JSON、XML、YAML等多种配置格式。![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658368255258-738e68fc-8dff-4eca-9670-26e2d470c143.png#clientId=uba7ad15e-6301-4&from=paste&id=u2a05f0c1&originHeight=793&originWidth=970&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1ef05160-4276-4db9-bca7-95ffc1978fa&title=)<br />group：dataId配置的分组管理，比如同在dev环境下开发，但同环境不同分支需要不同的配置数据，这时就可以用分组隔离，默认分组DEFAULT_GROUP。<br />namespace：项目开发过程中肯定会有dev、test、pro等多个不同环境，namespace则是对不同环境进行隔离，默认所有配置都在public里。
<a name="xhdeu"></a>
## 架构设计
下图简要描述了nacos配置中心的架构流程。<br />客户端、控制台通过发送Http请求将配置数据注册到服务端，服务端持久化数据到Mysql。<br />客户端拉取配置数据，并批量设置对dataId的监听发起长轮询请求，如服务端配置项变更立即响应请求，如无数据变更则将请求挂起一段时间，直到达到超时时间。为减少对服务端压力以及保证配置中心可用性，拉取到配置数据客户端会保存一份快照在本地文件中，优先读取。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658368255418-880ea88e-8524-4f29-ad26-f8c1154bc1d0.png#clientId=uba7ad15e-6301-4&from=paste&id=uacfcf5ea&originHeight=707&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u706aac46-b963-4c3f-913f-0937dd62847&title=)<br />这里我省略了比较多的细节，如鉴权、负载均衡、高可用方面的设计（其实这部分才是真正值得学的，后边另出文讲吧），主要弄清客户端与服务端的数据交互模式。<br />下边以Nacos 2.0.1版本源码分析，2.0以后的版本改动较多，和网上的很多资料略有些不同 地址：[https://github.com/alibaba/nacos/releases/tag/2.0.1](https://github.com/alibaba/nacos/releases/tag/2.0.1)
<a name="bSJko"></a>
## 客户端源码分析
Nacos配置中心的客户端源码在nacos-client项目，其中`NacosConfigService`实现类是所有操作的核心入口。<br />说之前先了解个客户端数据结构cacheMap，这里大家重点记住它，因为它几乎贯穿了Nacos客户端的所有操作，由于存在多线程场景为保证数据一致性，cacheMap采用了`AtomicReference`原子变量实现。
```java
/**
 * groupKey -> cacheData.
 */
private final AtomicReference<Map<String, CacheData>> cacheMap = new AtomicReference<Map<String, CacheData>>(new HashMap<>());
```
`cacheMap`是个Map结构，key为`groupKey`，是由dataId, group, tenant(租户)拼接的字符串；value为`CacheData`对象，每个dataId都会持有一个CacheData对象。
<a name="Emutg"></a>
### 获取配置
Nacos获取配置数据的逻辑比较简单，先取本地快照文件中的配置，如果本地文件不存在或者内容为空，则再通过HTTP请求从远端拉取对应dataId配置数据，并保存到本地快照中，请求默认重试3次，超时时间3s。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658368255457-b2032e5e-51dc-4e0b-9a7d-b30d5edb45b4.png#clientId=uba7ad15e-6301-4&from=paste&id=u66f64a78&originHeight=535&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7412eb1f-b3b3-4310-9d5e-fb1c7bd7773&title=)<br />获取配置有`getConfig()`和`getConfigAndSignListener()`这两个接口，但`getConfig()`只是发送普通的HTTP请求，而`getConfigAndSignListener()`则多了发起长轮询和对dataId数据变更注册监听的操作`addTenantListenersWithContent()`。
```java
@Override
public String getConfig(String dataId, String group, long timeoutMs) throws NacosException {
    return getConfigInner(namespace, dataId, group, timeoutMs);
}

@Override
public String getConfigAndSignListener(String dataId, String group, long timeoutMs, Listener listener)
        throws NacosException {
    String content = getConfig(dataId, group, timeoutMs);
    worker.addTenantListenersWithContent(dataId, group, content, Arrays.asList(listener));
    return content;
}
```
<a name="FGSga"></a>
### 注册监听
客户端注册监听，先从`cacheMap`中拿到dataId对应的`CacheData`对象。
```java
public void addTenantListenersWithContent(String dataId, String group, String content,
                                          List<? extends Listener> listeners) throws NacosException {
    group = blank2defaultGroup(group);
    String tenant = agent.getTenant();
    // 1、获取dataId对应的CacheData，如没有则向服务端发起长轮询请求获取配置
    CacheData cache = addCacheDataIfAbsent(dataId, group, tenant);
    synchronized (cache) {
        // 2、注册对dataId的数据变更监听
        cache.setContent(content);
        for (Listener listener : listeners) {
            cache.addListener(listener);
        }
        cache.setSyncWithServer(false);
        agent.notifyListenConfig();
    }
}
```
如没有则向服务端发起长轮询请求获取配置，默认的`Timeout`时间为30s，并把返回的配置数据回填至`CacheData`对象的content字段，同时用content生成MD5值；再通过`addListener()`注册监听器。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658368255486-97839f39-4f02-4068-bdf1-21ac42a907ab.png#clientId=uba7ad15e-6301-4&from=paste&id=u9f23c672&originHeight=639&originWidth=915&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u60d3cc68-3b08-4dde-b20d-3612edb5819&title=)<br />`CacheData`也是个出场频率非常高的一个类，看到除了dataId、group、tenant、content这些相关的基础属性，还有几个比较重要的属性如：listeners、md5(content真实配置数据计算出来的md5值)，以及注册监听、数据比对、服务端数据变更通知操作都在这里。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658368255572-df05fb53-206d-4431-89a6-2a490aa7cf0c.png#clientId=uba7ad15e-6301-4&from=paste&id=u3d7724c5&originHeight=864&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u10e79fe3-a131-4be0-a45d-346255dd843&title=)<br />其中listeners是对dataId所注册的所有监听器集合，其中的`ManagerListenerWrap`对象除了持有`Listener`监听类，还有一个`lastCallMd5`字段，这个属性很关键，它是判断服务端数据是否更变的重要条件。<br />在添加监听的同时会将`CacheData`对象当前最新的md5值赋值给`ManagerListenerWrap`对象的`lastCallMd5`属性。
```java
public void addListener(Listener listener) {
	ManagerListenerWrap wrap =
		(listener instanceof AbstractConfigChangeListener) ? new ManagerListenerWrap(listener, md5, content)
		: new ManagerListenerWrap(listener, md5);
}
```
看到这对dataId监听设置就完事了？可以发现所有操作都围着`cacheMap`结构中的`CacheData`对象，那么大胆猜测下一定会有专门的任务来处理这个数据结构。
<a name="qMlzZ"></a>
### 变更通知
客户端又是如何感知服务端数据已变更呢？<br />还是从头看，`NacosConfigService`类的构造器中初始化了一个`ClientWorker`，而在`ClientWorker`类的构造器中又启动了一个线程池来轮询cacheMap。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658368256010-aa69c956-bbee-4195-b4e5-3617c7e55736.png#clientId=uba7ad15e-6301-4&from=paste&id=u6a5d0144&originHeight=433&originWidth=858&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u81f146bd-c570-444a-9bc6-7cc513636be&title=)<br />而在`executeConfigListen()`方法中有这么一段逻辑，检查cacheMap中dataId的`CacheData`对象内，MD5字段与注册的监听listener内的lastCallMd5值，不相同表示配置数据变更则触发`safeNotifyListener`方法，发送数据变更通知。
```java
void checkListenerMd5() {
	for (ManagerListenerWrap wrap : listeners) {
		if (!md5.equals(wrap.lastCallMd5)) {
			safeNotifyListener(dataId, group, content, type, md5, encryptedDataKey, wrap);
		}
	}
}
```
`safeNotifyListener()`方法单独起线程，向所有对dataId注册过监听的客户端推送变更后的数据内容。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658368256052-f8d235a6-d557-44bd-b3d2-545ff7d4c3a3.png#clientId=uba7ad15e-6301-4&from=paste&id=u1bb8fb06&originHeight=762&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucdffe137-bdc8-42b9-9b0c-ff91e960676&title=)<br />客户端接收通知，直接实现`receiveConfigInfo()`方法接收回调数据，处理自身业务就可以了。
```java
configService.addListener(dataId, group, new Listener() {
	@Override
	public void receiveConfigInfo(String configInfo) {
		System.out.println("receive:" + configInfo);
	}

	@Override
	public Executor getExecutor() {
		return null;
	}
});
```
为了理解更直观用测试demo演示下，获取服务端配置并设置监听，每当服务端配置数据变化，客户端监听都会收到通知，一起看下效果。
```java
public static void main(String[] args) throws NacosException, InterruptedException {
    String serverAddr = "localhost";
    String dataId = "test";
    String group = "DEFAULT_GROUP";
    Properties properties = new Properties();
    properties.put("serverAddr", serverAddr);
    ConfigService configService = NacosFactory.createConfigService(properties);
    String content = configService.getConfig(dataId, group, 5000);
    System.out.println(content);
    configService.addListener(dataId, group, new Listener() {
        @Override
        public void receiveConfigInfo(String configInfo) {
            System.out.println("数据变更 receive:" + configInfo);
        }
        @Override
        public Executor getExecutor() {
            return null;
        }
    });

    boolean isPublishOk = configService.publishConfig(dataId, group, "我是新配置内容～");
    System.out.println(isPublishOk);

    Thread.sleep(3000);
    content = configService.getConfig(dataId, group, 5000);
    System.out.println(content);
}
```
结果和预想的一样，当向服务端`publishConfig`数据变化后，客户端可以立即感知，愣是用主动拉pull模式做出了服务端实时推送的效果。
```java
数据变更 receive:我是新配置内容～
true
我是新配置内容～
```
<a name="mA6D6"></a>
## 服务端源码分析
Nacos配置中心的服务端源码主要在nacos-config项目的`ConfigController`类，服务端的逻辑要比客户端稍复杂一些，这里重点看下。
<a name="CfyEj"></a>
### 处理长轮询
服务端对外提供的监听接口地址/v1/cs/configs/listener，这个方法内容不多，顺着`doPollingConfig`往下看。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658368256138-cad163d5-5260-4ed1-89cb-9c72d35e2981.png#clientId=uba7ad15e-6301-4&from=paste&id=ue5ccd30c&originHeight=521&originWidth=854&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u56e5effa-3645-42c3-b4e1-f3db18c43b6&title=)<br />服务端根据请求header中的Long-Pulling-Timeout属性来区分请求是长轮询还是短轮询，这里只关注长轮询部分，接着看`LongPollingService`（记住这个service很关键）类中的`addLongPollingClient()`方法是如何处理客户端的长轮询请求的。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658368256250-0f8af5a0-fe0e-419f-b3eb-8eac62877003.png#clientId=uba7ad15e-6301-4&from=paste&id=u734aa584&originHeight=318&originWidth=880&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u89cd954c-6f40-4a50-bb3e-23d4d85d4b8&title=)<br />正常客户端默认设置的请求超时时间是30s，但这里发现服务端“偷偷”的给减掉了500ms，现在超时时间只剩下了29.5s，那为什么要这样做呢？<br />用官方的解释之所以要提前500ms响应请求，为了最大程度上保证客户端不会因为网络延时造成超时，考虑到请求可能在负载均衡时会耗费一些时间，毕竟Nacos最初就是按照阿里自身业务体量设计的！<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658368258017-292138a7-4339-4626-80a7-c25b852c08dd.png#clientId=uba7ad15e-6301-4&from=paste&id=uc80dd86b&originHeight=995&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc106b079-81b7-47b7-a077-078a8067ef2&title=)<br />此时对客户端提交上来的groupkey的MD5与服务端当前的MD5比对，如md5值不同，则说明服务端的配置项发生过变更，直接将该groupkey放入`changedGroupKeys`集合并返回给客户端。
```java
MD5Util.compareMd5(req, rsp, clientMd5Map)
```
如未发生变更，则将客户端请求挂起，这个过程先创建一个名为`ClientLongPolling`的调度任务`Runnable`，并提交给`scheduler`定时线程池延后29.5s执行。
```java
ConfigExecutor.executeLongPolling(
                new ClientLongPolling(asyncContext, clientMd5Map, ip, probeRequestSize, timeout, appName, tag));
```
这里每个长轮询任务携带了一个`asyncContext`对象，使得每个请求可以延迟响应，等延时到达或者配置有变更之后,调用`asyncContext.complete()`响应完成。<br />asyncContext 为 Servlet 3.0新增的特性，异步处理，使Servlet线程不再需要一直阻塞，等待业务处理完毕才输出响应；可以先释放容器分配给请求的线程与相关资源，减轻系统负担，其响应将被延后，在处理完业务或者运算后再对客户端进行响应。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658368256590-4869632d-c631-46ce-a8cd-d0afb3a0f69f.png#clientId=uba7ad15e-6301-4&from=paste&id=ub1234282&originHeight=524&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2378b6ab-f50d-4ef8-9a62-bb6f1cb2ce7&title=)<br />`ClientLongPolling`任务被提交进入延迟线程池执行的同时，服务端会通过一个`allSubs`队列保存所有正在被挂起的客户端长轮询请求任务，这个是客户端注册监听的过程。<br />如延时期间客户端据数一直未变化，延时时间到达后将本次长轮询任务从`allSubs`队列剔除，并响应请求response，这是取消监听。收到响应后客户端再次发起长轮询，循环往复。<br />![处理长轮询](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658368256691-4dc3e581-0821-41c2-a018-9ed07d828791.jpeg#clientId=uba7ad15e-6301-4&from=paste&id=ufcc83ebe&originHeight=428&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u5ed652ef-c2b4-4621-a140-90237bb0082&title=%E5%A4%84%E7%90%86%E9%95%BF%E8%BD%AE%E8%AF%A2 "处理长轮询")<br />到这知道服务端是如何挂起客户端长轮询请求的，一旦请求在挂起期间，用户通过管理平台操作了配置项，或者服务端收到了来自其他客户端节点修改配置的请求。<br />怎么能让对应已挂起的任务立即取消，并且及时通知客户端数据发生了变更呢？
<a name="nMRr1"></a>
### 数据变更
管理平台或者客户端更改配置项接位置`ConfigController`中的`publishConfig`方法。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658368257057-4f61e6c3-bfc2-4291-b243-7bb5af26b8b2.png#clientId=uba7ad15e-6301-4&from=paste&id=u0cebc642&originHeight=338&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua28634c0-b370-400d-8fc3-f210c44b431&title=)<br />值得注意得是，在`publishConfig`接口中有这么一段逻辑，某个dataId配置数据被修改时会触发一个数据变更事件Event。
```java
ConfigChangePublisher.notifyConfigChange(new ConfigDataChangeEvent(false, dataId, group, tenant, time.getTime()));
```
仔细看`LongPollingService`会发现在它的构造方法中，正好订阅了数据变更事件，并在事件触发时执行一个数据变更调度任务`DataChangeTask`。<br />![订阅数据变更事件](https://cdn.nlark.com/yuque/0/2022/png/396745/1658368257170-041c21bb-7661-48a4-8df4-14f7d35e9f44.png#clientId=uba7ad15e-6301-4&from=paste&id=u59acb667&originHeight=546&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u46fe79f9-5a60-4e85-8bc7-8afed59f424&title=%E8%AE%A2%E9%98%85%E6%95%B0%E6%8D%AE%E5%8F%98%E6%9B%B4%E4%BA%8B%E4%BB%B6 "订阅数据变更事件")<br />`DataChangeTask`内的主要逻辑就是遍历`allSubs`队列，上边我们知道，这个队列中维护的是所有客户端的长轮询请求任务，从这些任务中找到包含当前发生变更的`groupkey`的`ClientLongPolling`任务，以此实现数据更变推送给客户端，并从`allSubs`队列中剔除此长轮询任务。<br />![DataChangeTask](https://cdn.nlark.com/yuque/0/2022/png/396745/1658368257240-e830da73-f69a-48ac-a442-667c0cff455c.png#clientId=uba7ad15e-6301-4&from=paste&id=u997746f8&originHeight=648&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u9838892f-72f0-4c0d-83f3-8c1de625c97&title=DataChangeTask "DataChangeTask")<br />而在看给客户端响应response时，调用`asyncContext.complete()`结束了异步请求。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658368257342-2b5d5256-5739-4129-a631-a4eedbb7e239.png#clientId=uba7ad15e-6301-4&from=paste&id=u9075ba3d&originHeight=449&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua2f14f79-91ba-4051-86e8-b476bbc0711&title=)
<a name="NvJMJ"></a>
## 结束语
上边只揭开了nacos配置中心的冰山一角，实际上还有非常多重要的技术细节都没提及到，建议大家没事看看源码，源码不需要通篇的看，只要抓住核心部分就够了。就比如今天这个题目以前真没太在意，突然被问一下子吃不准了，果断看下源码，而且这样记忆比较深刻。<br />nacos的源码还是比较**朴素**的，代码并没有过多炫技，看起来相对轻松。
