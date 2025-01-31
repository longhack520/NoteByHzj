<a name="rGXDT"></a>
## 1、HTTP 的问题
HTTP 协议是超文本传输协议（Hyper Text Transfer Protocol）的缩写，它是从 WEB 服务器传输超文本标记语言 HTML 到本地浏览器的传送协议。HTTP 设计之初是为了提供一种发布和接收 HTML 页面的方法，时至今日，它的作用已经不仅仅于此了。<br />对于 Java 工程师而言，HTTP 应该算是再熟悉不过的东西了，目前 HTTP 有多个版本，使用较多的是 HTTP/1.1 版本。<br />然而 HTTP 协议有一个缺陷那就是它是通过明文传输数据的，用户通过 HTTP 协议传输的内容很容易被恶意拦截，并且黑客可以伪装成服务端，向用户传送错误的信息，并且能轻易获取用户的隐私信息，而这些操作用户是完全无感知的。<br />由于存在这样的安全隐患，现在小伙伴们见到的大部分网站都在逐步转为 HTTPS，HTTP 网站会越来越少了。
<a name="FlSU0"></a>
## 2、HTTPS
HTTPS（HyperText Transfer Protocol Secure）中文译作超文本传输安全协议，这是一种通过计算机网络进行安全通讯的传输协议。<br />HTTPS 本质上还是由 HTTP 进行通信，只是在 HTTP 协议和 TCP 层之间增加了一个 SSL 的安全传输协议。整个传输的加密过程都在新的安全层 SSL/TLS 中实现，而原来的 HTTP 层的传输流程保持不变，这样就很好地兼容了旧的 HTTP 协议，也沿袭了 TCP/IP 协议族的分层思想。<br />通过 HTTPS，客户端可以确认服务端的身份，保证数据在传输过程中不被篡改，当在自己的浏览器上与某一个网站建立 HTTPS 连接的时候，满足如下情况可以表示这个服务端可以被信任：

1. 首先操作系统中安装了正确且受信任的证书。在 cmd 命令行中执行 certmgr.msc 命令，可以查看操作系统已经安装的证书列表。

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678928712257-e650ee92-981d-483d-a427-935df5386f9e.png#averageHue=%23f8f6f4&clientId=u96695a92-aad8-4&from=paste&id=u59eb88b4&originHeight=414&originWidth=622&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub9565e7a-1c5e-49be-baa6-56b9dbb6fe2&title=)

1. 浏览器本身正确实现了 HTTPS。
2. 被访问的网站提供了一个证书，这个证书是由一个操作系统所信任的证书颁发机构签发的，操作系统所信任的证书颁发机构一般都预装在操作系统中，通过第一步的方式可以查看。
3. 被访问的网站所提供的证书被成功认证。

这里边涉及到一些证书和协议的概念，接下来把整个过程捋一捋。
<a name="ocHv4"></a>
## 3、TLS/SSL
前面提到，HTTPS 就是在 HTTP 的基础之上增加了 TLS/SSL，那么这两个东西该如何理解呢？<br />SSL/TLS 是一种密码通信方案，算是目前使用最广泛的密码通信方案了。SSL 全称是 Secure Socket Layer，中文译作安全套接层，是 1994 年由 Netscape 公司设计的一套协议，并与 1995 年发布了 3.0 版本；TLS 全称是 Transport Layer Security，中文译作传输层安全，则是 IETF 在 SSL3.0 基础上设计的协议，实际上相当于 SSL 的后续版本，目前 TLS 先后迭代了 TLS 1.0、TLS 1.1、TLS 1.2 和 TLS 1.3，目前被广泛使用的是 TLS 1.2 版本。<br />SSL/TLS 涉及到了密码学中的对称加密、非对称加密、数字签名等等，算是密码学领域里的集大成者了。
<a name="cOQ5R"></a>
### 3.1 TLS
接下来就来看看 TLS 如何确保 HTTP 安全。<br />为了确保客户端和服务端之间的数据安全，很容易想到一种方案就是对传输的数据进行加密，没错，这是一个办法，事实上也是这么做的。<br />加密又分为两种：

1. 对称加密
2. 非对称加密

那么该使用哪一种呢？<br />对称加密，也就是加密密钥和解密密钥是同一个，当浏览器和服务端需要进行通信的时候，约定好一个密钥，然后使用这个密钥对发送的消息进行加密，对方收到消息之后再使用相同的密钥对消息进行解密。但是，在 B/S 架构的项目中，这种方案显然不合适，一个网站把自己的密钥告诉全世界所有的浏览器，那加密和不加密还有区别吗？<br />有小伙伴可能又想到了不对称加密，不对称加密倒是个办法，因为不对称加密是有一个密钥对公钥和私钥，公钥可以公布出来告诉所有人，私钥只有自己知道。通信的时候，客户端首先使用公钥对消息进行加密，服务端收到之后，再通过私钥对消息进行解密，这看起来似乎挺完美的。但是！！！非对称加密存在一个问题，就是非对称加密和解密相当耗时，通过这种方式处理加解密效率太低。<br />那怎么办？可以将两者结合起来。<br />具体来说，就是这样：首先服务端会生成一个非对称加密的密钥对，私钥自己保存，公钥发送给客户端，客户端拿到这个公钥之后，再生成一个对称加密的密钥，然后把对称加密的密钥通过公钥进行加密，加密之后发送给服务端，服务端通过私钥进行解密，这样客户端和服务端就可以通过对称加密进行通信了。<br />事实上，TLS 大致上的思路就是这样的。<br />不过上面这个方案还是有一个漏洞，那就是服务端要通过明文传输的方式把公钥发送给客户端，这个过程还是不安全的，可能被人恶意截胡，那么这个问题该如何解决呢？<br />这就涉及到另外一个概念叫做数字证书了。
<a name="TAyQ8"></a>
### 3.2 CA
数字证书是一个包含了目标网站各种信息如网站域名、证书有效期、签发机构、用于生成对称密钥的公钥、上级证书签发的签名等的文件，通过数字证书可以确认一个用户或者服务站点的身份。<br />实际场景中的数字证书是一系列的，形成了一个信任链，信任链的最顶端是 CA。<br />CA 是 Certificate Authority 的简写，它是一个负责发放和管理数字的证书的第三方权威机构。CA 的工作流程是这样的：

1. CA 自己给自己颁发的用自己的私钥签名的证书称为根证书，根证书的私钥安全性至关重要，根证书的私钥都是被保存在离线计算机中，有严格的操作规章，每次需要使用时，会有专人将数据通过 USB 拷贝过去，操作完了以后，再将数据带出来（这个专指 CA 根证书的私钥）。
2. 一个用户想要获取一个证书，首先自己得有一个密钥对，私钥自己留着，公钥以及其他信息发送给 CA，向 CA 提出申请，CA 判明用户的身份之后，会将这个公钥和用户的身份信息绑定，并且为绑定后的信息进行签名（签名是通过 CA 根证书的私钥进行的），最后将签名后的证书发给申请者。
3. 一个用户想要鉴定一个证书的真伪，就通过 CA 的公钥对证书上的数字签名进行验证，验证通过，就认为这个这个证书是有效的。

上面这个流程中有一个重要前提，那就是 CA 受到大家所有人的信任。<br />然而在实际操作中，并不能直接去跟 CA 申请一个数字证书，因为全世界要认证的内容太多了，CA 搞不过来，而且频繁的找 CA 申请，还有可能导致私钥泄漏，这可就是一个大的灾难了。<br />那怎么办呢？实际操作中，可以基于 CA 来构建一个信任链。具体来说，步骤是这样：

1. 首先手机、笔记本等操作系统中都预装了 CA 颁发的根证书，他们是所有信任构建的基石，前面已经截图给大家看了 Windows 中预装的根证书了。
2. 假设 CA 签发了一个证书 A，在这个过程中 CA 称为 Issuer，A 称为 Subject，假设 A 是一个受信任的中间证书，已经预装在操作系统中了。现在由 A 利用它自己的私钥给某一个网站签发了一个证书 B。
3. 现在当的电脑需要访问该网站的时候，该网站就会发来一个证书 B，由于浏览器并不知道 B 证书是否合法，但是电脑上已经预装了 A 证书，可以从 A 证书中提取出 A 的公钥，然后利用 A 的公钥对 B 证书的签名进行验证（因为 B 证书的签名是 A 的私钥签的），如果验证通过了，就说明 B 是合法的。
4. 相同的道理，B 也可以继续签发 C 证书，C 继续签发 D 证书，这样就形成了一个信任链。
5. 如果服务端的签名是 D 证书，那么一般来说，服务器返回给浏览器的就会包含 B、C、D 三个证书（因为 A 证书已经在电脑上了），即使只返回 D 证书，浏览器也可以根据 D 书中的信息，自动下载到 B、C 两个证书然后进行验证。
> 以前上学的时候，在 12306 网站上买火车票，第一次访问的时候必须要自己先手动安装一个根证书到系统中，然后才能访问。这就是因为当时 12306 所使用的证书的签发机构不被浏览器认可，类似于上面的第 3 步，12306 给我发了一个数字证书 B 回来，但是浏览器上没有合适的公钥对这个 B 证书进行验证，当往自己的系统上安装了 12306 给的证书之后，相当于电脑上有了一个证书 A，现在就可以对 B 证书进行验证了。

总结一下：

1. CA 是一个权威的机构，是一个发证机关，CA 发出来的证书可以证明一个人或者组织的身份。
2. 任何人都可以得到 CA 的证书（含公钥），用以验证 CA 所签发的证书。
3. 每一个数字证书都是由上级证书的私钥来签发的，处于最顶层的就是 CA 签发的根证书了，这个根证书没有上级证书了，所以这个根证书实际上是由 CA 自己的私钥来签发的，这也叫做自签名，即 Self-Signed。

当有了数字签名之后，就可以解决 3.1 小节最后提出的问题了。服务端将数字签名发给浏览器，浏览器利用系统已经内置的公钥验签，确认签名没问题，然后就提取出来数字签名中的公钥，开始协商对称加密的私钥了～
