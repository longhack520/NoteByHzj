网络协议 TCP IP<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623663199206-bdafe78e-d70c-44fb-b018-2621fe212bd1.png#clientId=u42391713-44bb-4&from=paste&id=uaf360e7b&originHeight=637&originWidth=482&originalType=url&ratio=3&status=done&style=none&taskId=u4564db11-c4dd-42ac-b304-c967f84053e)
<a name="FzbK3"></a>
## 一、TCP/IP模型
TCP/IP协议模型（Transmission Control Protocol/Internet Protocol），包含了一系列构成互联网基础的网络协议，是Internet的核心协议。<br />基于TCP/IP的参考模型将协议分成四个层次，它们分别是链路层、网络层、传输层和应用层。下图表示TCP/IP模型与OSI模型各层的对照关系。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623663199104-03c94f97-d483-46bd-9f9a-f08b7e684679.webp#clientId=u42391713-44bb-4&from=paste&id=u26347fb6&originHeight=339&originWidth=437&originalType=url&ratio=3&status=done&style=shadow&taskId=ud33e0005-1025-4d81-9482-e120ab20d0f)<br />TCP/IP协议族按照层次由上到下，层层包装。最上面的是应用层，这里面有http，ftp 等等熟悉的协议。而第二层则是传输层，著名的TCP和UDP协议就在这个层次。第三层是网络层，IP协议就在这里，它负责对数据加上IP地址和其他的数据以确定传输的目标。第四层是数据链路层，这个层次为待传送的数据加入一个以太网协议头，并进行CRC编码，为最后的数据传输做准备。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623663199141-b4bd3cd7-754a-461c-9a9a-52c8baf94bef.webp#clientId=u42391713-44bb-4&from=paste&id=u82fe1df8&originHeight=466&originWidth=640&originalType=url&ratio=3&status=done&style=shadow&taskId=u6c758f74-da43-4ed6-b2e9-8757c769dc5)<br />上图清楚地表示了TCP/IP协议中每个层的作用，而TCP/IP协议通信的过程其实就对应着数据入栈与出栈的过程。入栈的过程，数据发送方每层不断地封装首部与尾部，添加一些传输的信息，确保能传输到目的地。出栈的过程，数据接收方每层不断地拆除首部与尾部，得到最终传输的数据。![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623663199156-6aa82ccb-5757-44a8-95dc-a3d625980957.webp#clientId=u42391713-44bb-4&from=paste&id=u9513a171&originHeight=614&originWidth=640&originalType=url&ratio=3&status=done&style=shadow&taskId=u62c62952-e56e-483d-bb2c-5461e9b2670)<br />上图以HTTP协议为例，具体说明。
<a name="Q6XqJ"></a>
## 二、数据链路层
物理层负责0、1比特流与物理设备电压高低、光的闪灭之间的互换。数据链路层负责将0、1序列划分为数据帧从一个节点传输到临近的另一个节点,这些节点是通过MAC来唯一标识的(MAC,物理地址，一个主机会有一个MAC地址)。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623663199227-25c898b1-2141-45e9-b4a5-c429c9ad42aa.webp#clientId=u42391713-44bb-4&from=paste&id=ue0206d75&originHeight=129&originWidth=640&originalType=url&ratio=3&status=done&style=shadow&taskId=u40b2f6ad-acbe-4795-8e9d-2e7a0dd88ee)

- 封装成帧: 把网络层数据报加头和尾，封装成帧,帧头中包括源MAC地址和目的MAC地址。
- 透明传输:零比特填充、转义字符。
- 可靠传输: 在出错率很低的链路上很少用，但是无线链路WLAN会保证可靠传输。
- 差错检测(CRC):接收者检测错误,如果发现差错，丢弃该帧。
<a name="CW96T"></a>
## 三、网络层
<a name="QS8pU"></a>
### 1、IP协议
IP协议是TCP/IP协议的核心，所有的TCP，UDP，IMCP，IGMP的数据都以IP数据格式传输。要注意的是，IP不是可靠的协议，这是说，IP协议没有提供一种数据未传达以后的处理机制，这被认为是上层协议：TCP或UDP要做的事情。
<a name="H2fYg"></a>
#### 1.1 IP地址
在数据链路层中一般通过MAC地址来识别不同的节点，而在IP层也要有一个类似的地址标识，这就是IP地址。<br />32位IP地址分为网络位和地址位，这样做可以减少路由器中路由表记录的数目，有了网络地址，就可以限定拥有相同网络地址的终端都在同一个范围内，那么路由表只需要维护一条这个网络地址的方向，就可以找到相应的这些终端了。<br />A类IP地址: 0.0.0.0~127.0.0.0<br />B类IP地址:128.0.0.1~191.255.0.0<br />C类IP地址:192.168.0.0~239.255.255.0
<a name="WqlJL"></a>
#### 1.2 IP协议头
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623663199827-532a0fea-c22e-4cfa-93b9-8ba552ab4c9a.webp#clientId=u42391713-44bb-4&from=paste&id=uaed33ff2&originHeight=413&originWidth=640&originalType=url&ratio=3&status=done&style=none&taskId=ubb008988-250e-49ca-af65-48842c63cea)<br />这里只介绍:八位的TTL字段。这个字段规定该数据包在穿过多少个路由之后才会被抛弃。某个IP数据包每穿过一个路由器，该数据包的TTL数值就会减少1，当该数据包的TTL成为零，它就会被自动抛弃。<br />这个字段的最大值也就是255，也就是说一个协议包也就在路由器里面穿行255次就会被抛弃了，根据系统的不同，这个数字也不一样，一般是32或者是64。
<a name="NZkTt"></a>
### 2、ARP及RARP协议
ARP 是根据IP地址获取MAC地址的一种协议。<br />ARP（地址解析）协议是一种解析协议，本来主机是完全不知道这个IP对应的是哪个主机的哪个接口，当主机要发送一个IP包的时候，会首先查一下自己的ARP高速缓存（就是一个IP-MAC地址对应表缓存）。<br />如果查询的IP－MAC值对不存在，那么主机就向网络发送一个ARP协议广播包，这个广播包里面就有待查询的IP地址，而直接收到这份广播的包的所有主机都会查询自己的IP地址，如果收到广播包的某一个主机发现自己符合条件，那么就准备好一个包含自己的MAC地址的ARP包传送给发送ARP广播的主机。<br />而广播主机拿到ARP包后会更新自己的ARP缓存（就是存放IP-MAC对应表的地方）。发送广播的主机就会用新的ARP缓存数据准备好数据链路层的的数据包发送工作。<br />RARP协议的工作与此相反，不做赘述。
<a name="JgjpC"></a>
### 3、ICMP协议
IP协议并不是一个可靠的协议，它不保证数据被送达，那么，自然的，保证数据送达的工作应该由其他的模块来完成。其中一个重要的模块就是ICMP(网络控制报文)协议。ICMP不是高层协议，而是IP层的协议。<br />当传送IP数据包发生错误。比如主机不可达，路由不可达等等，ICMP协议将会把错误信息封包，然后传送回给主机。给主机一个处理错误的机会，这 也就是为什么说建立在IP层以上的协议是可能做到安全的原因。
<a name="swQjN"></a>
## 四、ping
ping可以说是ICMP的最著名的应用，是TCP/IP协议的一部分。利用“ping”命令可以检查网络是否连通，可以很好地帮助分析和判定网络故障。<br />例如：当某一个网站上不去的时候。通常会ping一下这个网站。ping会回显出一些有用的信息。一般的信息如下:<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623663199849-91409a27-25b6-4058-a9ac-72760160eb3c.webp#clientId=u42391713-44bb-4&from=paste&id=u2b4d19d3&originHeight=268&originWidth=640&originalType=url&ratio=3&status=done&style=none&taskId=u9b5cf7e9-1821-4edb-af37-7bd85569df3)<br />ping这个单词源自声纳定位，而这个程序的作用也确实如此，它利用ICMP协议包来侦测另一个主机是否可达。原理是用类型码为0的ICMP发请求，受到请求的主机则用类型码为8的ICMP回应。
<a name="Yq49J"></a>
## 五、Traceroute
Traceroute是用来侦测主机到目的主机之间所经路由情况的重要工具，也是最便利的工具。<br />Traceroute的原理是非常非常的有意思，它收到到目的主机的IP后，首先给目的主机发送一个TTL=1的UDP数据包，而经过的第一个路由器收到这个数据包以后，就自动把TTL减1，而TTL变为0以后，路由器就把这个包给抛弃了，并同时产生 一个主机不可达的ICMP数据报给主机。主机收到这个数据报以后再发一个TTL=2的UDP数据报给目的主机，然后刺激第二个路由器给主机发ICMP数据 报。如此往复直到到达目的主机。这样，traceroute就拿到了所有的路由器IP。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623663200336-86169a4e-9625-4750-bf53-36148bb10f28.webp#clientId=u42391713-44bb-4&from=paste&id=u3e41ebea&originHeight=227&originWidth=388&originalType=url&ratio=3&status=done&style=none&taskId=ue4188697-1108-4a4b-a571-bb066a5ad05)
<a name="GKBiM"></a>
## 六、TCP/UDP
TCP/UDP都是是传输层协议，但是两者具有不同的特性，同时也具有不同的应用场景，下面以图表的形式对比分析。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623663200446-658c13f0-8895-4f49-b707-4c28fa2becae.webp#clientId=u42391713-44bb-4&from=paste&id=u74162dce&originHeight=403&originWidth=640&originalType=url&ratio=3&status=done&style=none&taskId=u36b85ad0-1ecd-4e0c-a4b2-79a8aec1e89)
<a name="zQEW4"></a>
### 面向报文
面向报文的传输方式是应用层交给UDP多长的报文，UDP就照样发送，即一次发送一个报文。因此，应用程序必须选择合适大小的报文。若报文太长，则IP层需要分片，降低效率。若太短，会是IP太小。
<a name="Ak4lI"></a>
### 面向字节流
面向字节流的话，虽然应用程序和TCP的交互是一次一个数据块（大小不等），但TCP把应用程序看成是一连串的无结构的字节流。TCP有一个缓冲，当应用程序传送的数据块太长，TCP就可以把它划分短一些再传送。<br />关于拥塞控制，流量控制，是TCP的重点，后面讲解。<br />TCP和UDP协议的一些应用![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623663200960-b0b76b24-728b-49c4-8fcd-cfb79a3f1e62.webp#clientId=u42391713-44bb-4&from=paste&id=u46c5c0fc&originHeight=498&originWidth=640&originalType=url&ratio=3&status=done&style=shadow&taskId=u8e2c2b1d-f276-4bed-8993-540a5ed6b9c)
<a name="EJ8O6"></a>
### 什么时候应该使用TCP？
当对网络通讯质量有要求的时候，比如：整个数据要准确无误的传递给对方，这往往用于一些要求可靠的应用，比如HTTP、HTTPS、FTP等传输文件的协议，POP、SMTP等邮件传输的协议。
<a name="q7FCV"></a>
### 什么时候应该使用UDP？
当对网络通讯质量要求不高的时候，要求网络通讯速度能尽量的快，这时就可以使用UDP。
<a name="XJ0Ib"></a>
## 七、DNS
DNS（Domain Name System，域名系统），因特网上作为域名和IP地址相互映射的一个分布式数据库，能够使用户更方便的访问互联网，而不用去记住能够被机器直接读取的IP数串。通过主机名，最终得到该主机名对应的IP地址的过程叫做域名解析（或主机名解析）。DNS协议运行在UDP协议之上，使用端口号53。
<a name="DoMXM"></a>
## 八、TCP连接的建立与终止
<a name="K86tL"></a>
### 1、三次握手
TCP是面向连接的，无论哪一方向另一方发送数据之前，都必须先在双方之间建立一条连接。在TCP/IP协议中，TCP协议提供可靠的连接服务，连接是通过三次握手进行初始化的。三次握手的目的是同步连接双方的序列号和确认号并交换 TCP窗口大小信息。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623663201043-17b76701-c958-4182-a1aa-1d7f191a8292.webp#clientId=u42391713-44bb-4&from=paste&id=ua50c2c87&originHeight=439&originWidth=640&originalType=url&ratio=3&status=done&style=shadow&taskId=u3add92e4-5c17-4821-a1cc-c174b3cb3e7)<br />第一次握手：建立连接。客户端发送连接请求报文段，将SYN位置为1，Sequence Number为x；然后，客户端进入SYN_SEND状态，等待服务器的确认；<br />第二次握手：服务器收到SYN报文段。服务器收到客户端的SYN报文段，需要对这个SYN报文段进行确认，设置Acknowledgment Number为x+1(Sequence Number+1)；同时，自己自己还要发送SYN请求信息，将SYN位置为1，Sequence Number为y；服务器端将上述所有信息放到一个报文段（即SYN+ACK报文段）中，一并发送给客户端，此时服务器进入SYN_RECV状态； <br />第三次握手：客户端收到服务器的SYN+ACK报文段。然后将Acknowledgment Number设置为y+1，向服务器发送ACK报文段，这个报文段发送完毕以后，客户端和服务器端都进入ESTABLISHED状态，完成TCP三次握手。
<a name="bGFQW"></a>
#### 为什么要三次握手？
为了防止已失效的连接请求报文段突然又传送到了服务端，因而产生错误。<br />具体例子：“已失效的连接请求报文段”的产生在这样一种情况下：client发出的第一个连接请求报文段并没有丢失，而是在某个网络结点长时间的滞留了，以致延误到连接释放以后的某个时间才到达server。本来这是一个早已失效的报文段。但server收到此失效的连接请求报文段后，就误认为是client再次发出的一个新的连接请求。<br />于是就向client发出确认报文段，同意建立连接。假设不采用“三次握手”，那么只要server发出确认，新的连接就建立了。由于现在client并没有发出建立连接的请求，因此不会理睬server的确认，也不会向server发送数据。但server却以为新的运输连接已经建立，并一直等待client发来数据。这样，server的很多资源就白白浪费掉了。采用“三次握手”的办法可以防止上述现象发生。例如刚才那种情况，client不会向server的确认发出确认。server由于收不到确认，就知道client并没有要求建立连接。”
<a name="RtdQS"></a>
### 2、四次挥手
当客户端和服务器通过三次握手建立了TCP连接以后，当数据传送完毕，肯定是要断开TCP连接的啊。那对于TCP的断开连接，这里就有了神秘的“四次分手”。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623663201018-c711a03c-0cc3-4830-8b15-fd653d633c4d.webp#clientId=u42391713-44bb-4&from=paste&id=u51793db5&originHeight=433&originWidth=640&originalType=url&ratio=3&status=done&style=none&taskId=uc2bf21a0-fa68-4c78-bc14-d306b63c7c8)<br />第一次分手：主机1（可以使客户端，也可以是服务器端），设置Sequence Number，向主机2发送一个FIN报文段；此时，主机1进入FIN_WAIT_1状态；这表示主机1没有数据要发送给主机2了；<br />第二次分手：主机2收到了主机1发送的FIN报文段，向主机1回一个ACK报文段，Acknowledgment Number为Sequence Number加1；主机1进入FIN_WAIT_2状态；主机2告诉主机1，我“同意”你的关闭请求；<br />第三次分手：主机2向主机1发送FIN报文段，请求关闭连接，同时主机2进入LAST_ACK状态；<br />第四次分手：主机1收到主机2发送的FIN报文段，向主机2发送ACK报文段，然后主机1进入TIME_WAIT状态；主机2收到主机1的ACK报文段以后，就关闭连接；此时，主机1等待2MSL后依然没有收到回复，则证明Server端已正常关闭，那好，主机1也可以关闭连接了。
<a name="Bm3r6"></a>
#### 为什么要四次分手？
TCP协议是一种面向连接的、可靠的、基于字节流的运输层通信协议。TCP是全双工模式，这就意味着，当主机1发出FIN报文段时，只是表示主机1已经没有数据要发送了，主机1告诉主机2，它的数据已经全部发送完毕了；但是，这个时候主机1还是可以接受来自主机2的数据；当主机2返回ACK报文段时，表示它已经知道主机1没有数据发送了，但是主机2还是可以发送数据到主机1的；当主机2也发送了FIN报文段时，这个时候就表示主机2也没有数据要发送了，就会告诉主机1，也没有数据要发送了，之后彼此就会愉快的中断这次TCP连接。
<a name="NIovc"></a>
#### 为什么要等待2MSL？
MSL：报文段最大生存时间，它是任何报文段被丢弃前在网络内的最长时间。原因有二：

- 保证TCP协议的全双工连接能够可靠关闭
- 保证这次连接的重复数据段从网络中消失

第一点：如果主机1直接CLOSED了，那么由于IP协议的不可靠性或者是其它网络原因，导致主机2没有收到主机1最后回复的ACK。那么主机2就会在超时之后继续发送FIN，此时由于主机1已经CLOSED了，就找不到与重发的FIN对应的连接。所以，主机1不是直接进入CLOSED，而是要保持TIME_WAIT，当再次收到FIN的时候，能够保证对方收到ACK，最后正确的关闭连接。<br />第二点：如果主机1直接CLOSED，然后又再向主机2发起一个新连接，不能保证这个新连接与刚关闭的连接的端口号是不同的。也就是说有可能新连接和老连接的端口号是相同的。一般来说不会发生什么问题，但是还是有特殊情况出现：假设新连接和已经关闭的老连接端口号是一样的，如果前一次连接的某些数据仍然滞留在网络中，这些延迟数据在建立新连接之后才到达主机2，由于新连接和老连接的端口号是一样的，TCP协议就认为那个延迟的数据是属于新连接的，这样就和真正的新连接的数据包发生混淆了。所以TCP连接还要在TIME_WAIT状态等待2倍MSL，这样可以保证本次连接的所有数据都从网络中消失。
<a name="wdQ84"></a>
## 九、TCP流量控制
如果发送方把数据发送得过快，接收方可能会来不及接收，这就会造成数据的丢失。所谓流量控制就是让发送方的发送速率不要太快，要让接收方来得及接收。<br />利用滑动窗口机制可以很方便地在TCP连接上实现对发送方的流量控制。<br />设A向B发送数据。在连接建立时，B告诉了A：“我的接收窗口是 rwnd = 400 ”(这里的 rwnd 表示 receiver window) 。因此，发送方的发送窗口不能超过接收方给出的接收窗口的数值。请注意，TCP的窗口单位是字节，不是报文段。假设每一个报文段为100字节长，而数据报文段序号的初始值设为1。大写ACK表示首部中的确认位ACK，小写ack表示确认字段的值ack。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623663201595-b72f40a9-0718-461e-b416-223cb9842e98.webp#clientId=u42391713-44bb-4&from=paste&id=u3921ab00&originHeight=333&originWidth=640&originalType=url&ratio=3&status=done&style=shadow&taskId=u51bd6488-1c41-4a21-ba58-bebecb39df2)<br />从图中可以看出，B进行了三次流量控制。第一次把窗口减少到 rwnd = 300 ，第二次又减到了 rwnd = 100 ，最后减到 rwnd = 0 ，即不允许发送方再发送数据了。这种使发送方暂停发送的状态将持续到主机B重新发出一个新的窗口值为止。B向A发送的三个报文段都设置了 ACK = 1 ，只有在ACK=1时确认号字段才有意义。<br />TCP为每一个连接设有一个持续计时器(persistence timer)。只要TCP连接的一方收到对方的零窗口通知，就启动持续计时器。若持续计时器设置的时间到期，就发送一个零窗口控测报文段（携1字节的数据），那么收到这个报文段的一方就重新设置持续计时器。
<a name="UxtwS"></a>
## 十、TCP拥塞控制
发送方维持一个拥塞窗口 cwnd ( congestion window )的状态变量。拥塞窗口的大小取决于网络的拥塞程度，并且动态地在变化。发送方让自己的发送窗口等于拥塞窗口。<br />发送方控制拥塞窗口的原则是：只要网络没有出现拥塞，拥塞窗口就再增大一些，以便把更多的分组发送出去。但只要网络出现拥塞，拥塞窗口就减小一些，以减少注入到网络中的分组数。<br />慢开始算法：<br />当主机开始发送数据时，如果立即所大量数据字节注入到网络，那么就有可能引起网络拥塞，因为现在并不清楚网络的负荷情况。因此，较好的方法是 先探测一下，即由小到大逐渐增大发送窗口，也就是说，由小到大逐渐增大拥塞窗口数值。<br />通常在刚刚开始发送报文段时，先把拥塞窗口 cwnd 设置为一个最大报文段MSS的数值。而在每收到一个对新的报文段的确认后，把拥塞窗口增加至多一个MSS的数值。用这样的方法逐步增大发送方的拥塞窗口 cwnd ，可以使分组注入到网络的速率更加合理。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623663201533-73f15f2a-d896-4a63-970e-d892962c4e21.webp#clientId=u42391713-44bb-4&from=paste&id=udc40ad89&originHeight=284&originWidth=640&originalType=url&ratio=3&status=done&style=shadow&taskId=uc02e5724-3733-4ea3-a84c-9ea89c77a0e)<br />每经过一个传输轮次，拥塞窗口 cwnd 就加倍。一个传输轮次所经历的时间其实就是往返时间RTT。不过“传输轮次”更加强调：把拥塞窗口cwnd所允许发送的报文段都连续发送出去，并收到了对已发送的最后一个字节的确认。<br />另，慢开始的“慢”并不是指cwnd的增长速率慢，而是指在TCP开始发送报文段时先设置cwnd=1，使得发送方在开始时只发送一个报文段（目的是试探一下网络的拥塞情况），然后再逐渐增大cwnd。<br />为了防止拥塞窗口cwnd增长过大引起网络拥塞，还需要设置一个慢开始门限ssthresh状态变量。慢开始门限ssthresh的用法如下：

- 当 cwnd < ssthresh 时，使用上述的慢开始算法。
- 当 cwnd > ssthresh 时，停止使用慢开始算法而改用拥塞避免算法。
- 当 cwnd = ssthresh 时，既可使用慢开始算法，也可使用拥塞控制避免算法。拥塞避免
<a name="s1gZZ"></a>
### 拥塞避免
让拥塞窗口cwnd缓慢地增大，即每经过一个往返时间RTT就把发送方的拥塞窗口cwnd加1，而不是加倍。这样拥塞窗口cwnd按线性规律缓慢增长，比慢开始算法的拥塞窗口增长速率缓慢得多。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623663201555-48c00269-a476-4ec2-af16-1d0778a71cd8.webp#clientId=u42391713-44bb-4&from=paste&id=u2bb663ae&originHeight=284&originWidth=640&originalType=url&ratio=3&status=done&style=none&taskId=u045c513a-1254-4554-9275-272ea6f2ed3)<br />无论在慢开始阶段还是在拥塞避免阶段，只要发送方判断网络出现拥塞（其根据就是没有收到确认），就要把慢开始门限ssthresh设置为出现拥塞时的发送 方窗口值的一半（但不能小于2）。然后把拥塞窗口cwnd重新设置为1，执行慢开始算法。<br />这样做的目的就是要迅速减少主机发送到网络中的分组数，使得发生 拥塞的路由器有足够时间把队列中积压的分组处理完毕。<br />如下图，用具体数值说明了上述拥塞控制的过程。现在发送窗口的大小和拥塞窗口一样大。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623663201958-be9b9ea1-6c6a-4980-87ce-e37afbd0e326.webp#clientId=u42391713-44bb-4&from=paste&id=udfec6737&originHeight=332&originWidth=640&originalType=url&ratio=3&status=done&style=none&taskId=u791ce4c2-5740-448c-8acd-07df36c41d4)
<a name="jRDJb"></a>
### 快重传和快恢复
<a name="OZbQy"></a>
#### 快重传
快重传算法首先要求接收方每收到一个失序的报文段后就立即发出重复确认（为的是使发送方及早知道有报文段没有到达对方）而不要等到自己发送数据时才进行捎带确认。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623663202126-13ac1c82-d656-49a8-8d29-5eeabe3fd1a3.webp#clientId=u42391713-44bb-4&from=paste&id=u7080ce17&originHeight=312&originWidth=490&originalType=url&ratio=3&status=done&style=none&taskId=u68e5be25-9a68-42af-b114-218aaa8aee2)<br />接收方收到了M1和M2后都分别发出了确认。现在假定接收方没有收到M3但接着收到了M4。<br />显然，接收方不能确认M4，因为M4是收到的失序报文段。根据 可靠传输原理，接收方可以什么都不做，也可以在适当时机发送一次对M2的确认。<br />但按照快重传算法的规定，接收方应及时发送对M2的重复确认，这样做可以让 发送方及早知道报文段M3没有到达接收方。发送方接着发送了M5和M6。接收方收到这两个报文后，也还要再次发出对M2的重复确认。这样，发送方共收到了 接收方的四个对M2的确认，其中后三个都是重复确认。<br />快重传算法还规定，发送方只要一连收到三个重复确认就应当立即重传对方尚未收到的报文段M3，而不必 继续等待M3设置的重传计时器到期。<br />由于发送方尽早重传未被确认的报文段，因此采用快重传后可以使整个网络吞吐量提高约20%。
<a name="ONRx0"></a>
#### 快恢复
与快重传配合使用的还有快恢复算法，其过程有以下两个要点：

- 当发送方连续收到三个重复确认，就执行“乘法减小”算法，把慢开始门限ssthresh减半。
- 与慢开始不同之处是现在不执行慢开始算法（即拥塞窗口cwnd现在不设置为1），而是把cwnd值设置为 慢开始门限ssthresh减半后的数值，然后开始执行拥塞避免算法（“加法增大”），使拥塞窗口缓慢地线性增大。
