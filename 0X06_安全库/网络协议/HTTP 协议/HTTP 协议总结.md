网络协议<br />本文全面介绍了 HTTP 协议相关知识，包括 HTTP 请求报文、响应报文、持久连接、缓存、Cookie 以及 HTTP 版本升级等！<br />HTTP 协议全称为 **H**yper**T**ext **T**ransfer **P**rotocol，即超文本传输协议。

- 超文本：指文字、图片、音频、视频、文件等的混合体，比如最常见的 HTML。
- 传输：指数据从一方转移到另一方，二者之间可能相距数千里。
- 协议：指通信双方所做的一些约定，比如怎么开始通信、信息的格式与顺序、怎么结束通信等。

HTTP 协议是干啥的呢？答案是**用于客户端与服务器端之间的通信**。日常上网过程中最常见的就是 HTTP 协议了，浏览器是最常见的 HTTP 客户端。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679316724084-bf3455fd-cd50-4897-ac02-38165bbb4fc7.png#averageHue=%23fafaf9&clientId=u51d53653-de0b-4&from=paste&id=u70a1a6c6&originHeight=435&originWidth=931&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u232332b1-d3e0-4a50-b714-4350cae749a&title=)<br />比如使用浏览器访问淘宝时，浏览器就会发送一个遵循 HTTP 协议的**请求报文**到淘宝服务器，告诉淘宝服务器自己想要获取淘宝首页信息。<br />淘宝服务器收到此报文后，则会发送一个同样遵循 HTTP 协议的**响应报文**到浏览器，此响应报文中包含淘宝首页的内容。<br />浏览器收到响应报文后解析其内容并展示在界面上。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679316724069-bd0143d6-39c5-4cf4-ad26-57232f1ba9b0.png#averageHue=%23727b6e&clientId=u51d53653-de0b-4&from=paste&id=ub44006cc&originHeight=486&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uaebaf638-dd36-4b0d-ae0a-8267438fac0&title=)
<a name="bLdjV"></a>
## 1、HTTP 请求
客户端向服务器端发送的信息称为**请求报文**，一般结构如下：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679316724030-dec2600c-f577-4fd9-878f-39edb6f14ef3.png#averageHue=%23f5f4f3&clientId=u51d53653-de0b-4&from=paste&id=u57893776&originHeight=396&originWidth=740&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9629ed0f-2245-4bdf-a041-d87fbb06146&title=)
<a name="ZJAWF"></a>
### （1）请求行
请求行用于说明要做些什么，包含三部分内容，中间用空格分割。

- **方法**，指定要对请求资源做什么样的操作（比如查询、修改等）。常见的方法有：GET、POST、PUT、DELETE、HEAD 等。在前后端分离开发中，经常会遵循 RESTful 设计风格，其使用 POST、DELETE、PUT、GET 分别表示对数据的增、删、改、查。
- **资源路径**，指定所要请求资源在服务器中的位置。比如 /index.html，表示访问服务器根目录下名字为 index 的 html 文件。
- **HTTP 版本**，指定所使用的 HTTP 版本。目前使用最多的版本为 HTTP/1.1。

举个栗子：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679316724444-c3ad4d58-8347-4947-aa59-60a0fa8ee01f.png#averageHue=%23f7f6f5&clientId=u51d53653-de0b-4&from=paste&id=u2fb2e161&originHeight=341&originWidth=707&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7cd40b3f-2d86-4d9d-86f9-f9dc0c743a9&title=)<br />面试中常见的一个问题：** GET 和 POST 的区别是什么？**，在这里做一下解答。

- **首先，一般 GET 请求参数存放在 URL 中，而 POST 请求参数存储在请求体中**。因为参数放在 URL 中可以直接被看到，则 GET 请求相对 POST 请求更不安全。但并不是说 POST 请求安全，因为参数放在请求体中，如果不采取加密手段的话，技术人员抓包就能看到明文。同时各个浏览器对 URL 长度做了限制，比如 IE 浏览器限制 URL 的长度最大为 2KB，这就使得了 GET 请求传输的数据长度有了限制，而 POST 请求传输数据长度无限制。
- **其次，一般 GET 请求用于获取数据，POST 请求用于新增数据**。这里需要提一下幂等性的概念。幂等性是指对于同一个系统，在同样条件下，一次请求和重复多次请求对资源的影响是一致的，不会因为多次请求而产生了副作用。GET 请求用于获取资源，不会对系统资源进行改变，因此是幂等的。POST 用于新增资源，这意味着多次请求将创建多个资源，因此不是幂等的。基于这个特点，GET 请求可被缓存、可保留在浏览器历史记录中、浏览器回退不会产生副作用，而 POST 请求反之。
- **最后，GET 和 POST 在本质上并无区别**。HTTP 的底层是 TCP，所以无论是 GET 还是 POST 底层都是通过 TCP 进行连接通信。可以给 GET 加请求体，给 POST 带上 URL 参数，可以用 GET 请求新增数据，POST 请求查询数据，实际上也是完全可行的。

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679316724930-2e60f2de-5f56-4825-bf10-f3890f44ca22.png#averageHue=%23bed8ae&clientId=u51d53653-de0b-4&from=paste&id=u513f05c0&originHeight=544&originWidth=1047&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6d1f0578-0662-419c-80bf-c6bdd4dcfe0&title=)
<a name="IBtxw"></a>
### （2）请求头
请求头用于向服务器传递一些额外的重要信息，比如所能接收的语言等。<br />请求头由字段名和字段值构成，二者之间用冒号进行分隔。常见的一些请求头有：

| 请求头 | 含义 |
| --- | --- |
| Host | 接收请求的域名 |
| User-Agent | 客户端软件的名称和版本号等相关信息 |
| Connection | 设置发送响应之后 TCP 连接是否继续保持的通信选项 |
| Cache-Control | 控制缓存的相关信息 |
| Referer | 记录请求的来源（当通过点击超级链接进入下一个页面时，会记录上一个页面的 URI） |
| Accept | 客户端可支持的数据类型， 以 MIME 类型来表示 |
| Accept-Encoding | 客户端可支持的编码格式 |
| Accept-Language | 客户端可支持的语言 |
| If-Modified-Since | 用于判断资源的缓存是否有效（客户端通知服务器，本地缓存的最后变更时间） |
| If-None-Match | 用于判断资源的缓存是否有效 |
| Range | 用于断点续传，指定第一个字节的位置和最后一个字节的位置。 |
| Cookie | 表示请求者的身份，用于保存状态信息 |

<a name="UMjHp"></a>
### （3）请求空行
请求空行用于表明请求头已经结束。
<a name="SlbCJ"></a>
### （4）请求体
请求体用于传送客户端要发给服务器的数据，比如请求参数，通常出现在 POST 请求方法中，而 GET 方法无请求体，它的请求参数直接会显示在网址上面。<br />请求行和请求头的数据都是文本形式且格式化的，而请求体不同，其可以包含任意的二进制数据，比如文本、图片、视频等等。
<a name="rTnJ0"></a>
## 2、HTTP 响应
服务器向客户端发送的信息称为**响应报文**，响应报文的结构一般如下：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679316724896-e7be54db-8961-4eda-b201-33df63ceda4a.png#averageHue=%23f4f4f3&clientId=u51d53653-de0b-4&from=paste&id=u406815c6&originHeight=379&originWidth=693&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u12d2784d-5b23-4e81-a849-0983f2a809e&title=)
<a name="wBc3h"></a>
### （1）响应行
响应行用于说明对请求的处理情况，包含三部分内容，中间用空格分割。

- **HTTP 版本**，指定所使用的 HTTP 版本。比如 HTTP/1.1 表示使用的 HTTP 版本是 1.1。
- **状态码**，以三位数字形式描述服务器对请求的处理结果。比如 200 表示成功。
- **消息短语**，以文本形式描述服务器对请求的处理结果。比如 OK 表示成功。

举个栗子：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679316724898-dcaa0cc1-20e0-41c6-9968-aa6b043e7ba1.png#averageHue=%23f8f8f8&clientId=u51d53653-de0b-4&from=paste&id=u542bb439&originHeight=361&originWidth=759&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua5df7f7a-2e8c-40d7-88ea-45a65e802ec&title=)<br />面试中常见的一个问题：** HTTP 有哪些常见状态码？**，在这里做一下解答。![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679316725170-9b23ad3c-c1e4-4b83-91a0-8fe56b2bd8f4.png#averageHue=%23cddae8&clientId=u51d53653-de0b-4&from=paste&id=u43753163&originHeight=422&originWidth=1019&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u814b9b52-ba2c-45be-8a12-eae4e2563b3&title=)

- 200 OK：表示请求被正常处理，这是最常见的状态码。
- 204 No Content：表示请求被正常处理，但在返回的响应报文中不含响应体内容。
- 301 Moved Permanently ：永久重定向，表示请求的资源已经被永久转移了，新的 URL 定义在响应报文的 **Location** 字段中，浏览器将自动获取新的 URL 发出新的请求。场景：比如建设一个网站后，将网站的 url 变换了，重新申请一个域名，但是希望之前 url 仍然可以访问到，就可以做一个重定向到新的 url 下面。比如京东最早网址 http://www.360buy.com 重定向到 http://www.jd.com。
- 302 Found ：临时重定向（即以后还可能有变化），表示请求的资源已被临时分配了新的 URL，新的 URL 会在响应报文中的 **Location** 字段中返回，浏览器将会自动使用新的 URL 发出新的请求。比如用户在未登录时访问个人中心页面，这时可以临时重定向到登录的 url；或者协议发生变化，比如京东 http://www.jd.com 重定向到 https://www.jd.com；再比如，今天夜里网站后台要系统维护，服务暂时不可用，这就属于『临时』的，可以配置成 302 跳转，把流量临时切换到一个静态通知页面，浏览器看到这个 302 就知道这只是暂时的情况，不会做缓存优化，第二天还会访问原来的地址。
- 304 Not Modified：代表上次的文档已经被缓存了，还可以继续使用，即访问缓存。
- 400 Bad Request：一个通用差错状态码，表示请求报文中存在语法错误，客户端发生的错误。
- 401 Unauthorized ：用户未认证。
- 403 Forbidden：表示服务器虽然收到了请求，但是拒绝提供服务，常见的原因是为没有访问权限（即用户未授权）。
- 404 Not Found ：表示请求资源不存在。
- 500 Internal Server Error：表示服务器出现错误，可能是出现了一些 Bug 或故障。
- 502 Bad Gateway：通常是服务器作为网关或代理时返回的错误码，表示服务器自身工作正常，访问后端服务器发生了错误（可能后端服务器宕机了）。
- 503 Service Unavailable：表示服务器暂时处于**超负载**或者正在进行停机维护，暂时无法处理请求，可以稍后再试。Web 服务器如果限流，就可以给超载的流量直接响应 503 状态码。
<a name="saTrM"></a>
### （2）响应头
响应头用于向客户端传递一些额外的重要信息，比如响应内容的长度等。<br />响应头由字段名和字段值构成，二者之间用冒号进行分隔。常见的一些响应头有：

| 响应头 | 含义 |
| --- | --- |
| Date | 日期时间信息，表示服务器产生并发送响应报文的日期和时间。 |
| Server | 表示HTTP服务器应用程序的信息，类似于请求报文中的 User-Agent |
| Location | 此字段会配合重定向使用，用于提供重定向后新的 URI。 |
| **Connection** | 设置发送响应之后 TCP 连接是否继续保持的通信选项 |
| **Cache-Control** | 控制缓存的相关信息 |
| Content-Type | 服务器返回的响应类型 |
| **Content-length** | 服务器返回的响应长度 |
| Content-Encoding | 服务器返回的响应编码 |
| Content-Language | 服务器返回的响应语言 |
| Last-Modified | 指定响应内容最后的修改时间 |
| Expires | 表示资源失效的时间，浏览器会在指定过期时间内使用本地缓存 |
| **Etag** | 用于协商缓存，返回一个摘要值 |
| Accept-Ranges | 用于断点续传，指定服务器所支持的内容范围 |
| **Set-Cookie** | 设置状态信息 |

<a name="QqIM8"></a>
### （3）响应空行
响应空行用于表明响应头已经结束。
<a name="ljKAP"></a>
### （4）响应体
响应体用于传送服务器要发给浏览器的正文。<br />同请求报文的请求体一样，响应体可包含任意的二进制数据。浏览器收到响应报文后，则会将正文加载到内存，然后解析渲染，最后显示页面内容。
<a name="E13E9"></a>
## 3、HTTP 持久连接
客户端发送一系列请求给服务器，如果服务器与客户端对每个请求/响应对都经过一个单独的 TCP 连接发送，则称为**非持续连接**，也称为短连接；如果经过相同的 TCP 连接发送，则称为**持续连接**，也称为长连接。<br />比如打开一个 Web 页面时，假设该页面含有一个 HTML 基础文件和 2 张图片，如果客户端与服务器通过同一个 TCP 连接来获取这 3 个数据，则为持续连接，如果通过建立 3 次不同的 TCP 连接，则为非持续连接。![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679316725574-e18b305e-6484-438b-a075-5a3c1a800476.png#averageHue=%23fbfafa&clientId=u51d53653-de0b-4&from=paste&id=u3509e35b&originHeight=542&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2603d098-42b0-4db4-b05d-abfe7f71917&title=)<br />非持续连接的缺点：

- 每次建立连接需要三次握手过程，导致总的请求响应时间变长。当然也不是绝对的，如果多个连接可以并行请求，总响应时间可能变短，比如 Chrome 浏览器为了提升加载速度，可以同时打开 6 个并行连接，但多个并行连接会加重 Web 服务器负担。
- 必须为每一个请求的对象建立和维护一个全新的连接，而每一个连接都需要客户和服务器分配 TCP 的缓冲区和保持 TCP 变量，使得 Web 服务器存在严重的负担，因为一台 Web 服务器可能同时服务于数以百计不同的客户的请求。

HTTP（1.1 及之后） 默认采用**持续连接**方式，但也可配置成非持续连接方式。在报文中使用 Connection 字段来表示是否使用持久连接。

- 如果 Connection 字段的值为 keep-alive，则表明此连接为持久连接，HTTP1.1 及以后可默认不写。
- 如果 Connection 字段的值为 close，则表明要关闭连接。

注意：持久连接不是永久连接，一般在一个可配置的超时间隔后，如果此连接仍未被使用，HTTP 服务器就会关闭该连接。
<a name="xwFqy"></a>
## 4、HTTP 缓存
> 对于一些短时间内不会产生变化的资源，客户端（浏览器）可以在一次请求后将服务器响应的资源缓存在本地，之后直接读取本地的数据，而不必再重新发送请求。

经常会接触到『**缓存**』这一概念，比如由于内存和 CPU 之间速度差距较大，为了进一步提升电脑性能，于是设计了 L1 缓存、L2 缓存等，让 CPU 先从缓存中取数据，如果取不到，再去内存取。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679316726130-4bc10457-cb9a-4e88-b776-4ad5be05a796.png#averageHue=%23f4f2f1&clientId=u51d53653-de0b-4&from=paste&id=ud18ca038&originHeight=96&originWidth=204&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7b4624be-8588-44e8-820f-91a36607440&title=)<br />又比如在后端开发中，由于数据库一般存储在硬盘上，读取速度较慢，于是可能会采用 Redis 等内存数据库作为缓存，先去 Redis 中取数据，如果取不到，再去数据库中取。<br />再比如在操作系统中，由于页表进行地址转换的速度较慢，于是有了 TLB 快表，当需要进行逻辑地址到物理地址的转换时，先去查询速度更快的 TLB 快表，如果查不到，再去查询页表，此时 TLB 快表就是一种缓存。<br />缓存的主要目的在于**提升查询速度**，一般逻辑如图所示。![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679316726203-d2a89f49-725f-4246-bd95-d1f75fc0818b.png#averageHue=%23faf9f9&clientId=u51d53653-de0b-4&from=paste&id=ue9fe5c5e&originHeight=465&originWidth=724&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc21a51b2-4bc0-41c0-8dd0-cf5ba28350e&title=)<br />同样，在 HTTP 设计中也有缓存的概念，主要是为了**加快响应速度**，HTTP 缓存的实现依赖于请求报文和响应报文中的一些字段，分为强缓存和协商缓存。
<a name="qhNO5"></a>
### （1）强缓存
**强缓存**指的是在缓存数据未失效的情况下，那么就会直接使用浏览器的缓存数据，不会再向服务器发送任何请求，逻辑类似于前面举的 L1 缓存、Redis、TLB 快表。<br />具体实现主要是通过 Cache-Control字段和 Expires字段。<br />Cache-Control 是一个相对时间（即多长时间后过期，http1.1 规范），Expires 是一个绝对时间（即在某个时间点过期，http1.0 规范），如果两个字段同时存在，Cache-Control 的优先级更高。![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679316726154-b1d2c5b7-7c87-468a-ab8d-699c61c8264e.png#averageHue=%23f9f9f8&clientId=u51d53653-de0b-4&from=paste&id=u17511462&originHeight=483&originWidth=902&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ucafa7a92-6d0a-48a9-9f0f-8f5b0abb577&title=)<br />由于服务器端时间和客户端时间可能不同步，存在偏差，这也就是导致了使用 Expires 可能会存在时间误差，因此**一般更推荐使用 Cache-Control 来实现强缓存**。<br />以 Cache-Control 为例，强缓存的具体的实现流程如下：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679316726308-06cf5900-57cb-489c-94e2-604b9a4b336d.png#averageHue=%23f9f9f9&clientId=u51d53653-de0b-4&from=paste&id=u5125ea50&originHeight=562&originWidth=722&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue2952154-c4bb-466e-aada-959c8772aff&title=)

1. 当浏览器第一次请求访问服务器资源时，**服务器会在响应头中加上 Cache-Control**。Cache-Control 中可以设置以下内容。
   - max-age=秒，表示缓存将于指定毫秒值后过期。比如：cache-control: max-age=31536000，表示缓存将于 365 天后过期。
   - no-store，表示不允许缓存（包括强缓存和协商缓存）。
   - no-cache，表示不使用强缓存，而是**使用协商缓存**，即使用之前必须要先去服务器端验证是否失效，如果没失效，则再使用缓存，如果失效了，则返回最新数据。等价于max-age=0, must-revalidate。
   - must-revalidate，表示允许缓存，并且如果缓存不过期的话，先使用缓存，如果缓存过期的话，再去服务器端进行验证缓存是否还有效。这里很多小伙伴可能会有疑问，即使没有加上 must-revalidate，有了 max-age 后，缓存过期了不也会去服务器验证吗，加不加 must-revalidate 有什么区别呢？在 HTTP 协议规范中，允许客户端在某些特殊情况下直接使用过期缓存，比如校验请求错误时（如无法再次连通服务器），而加上了 must-revalidate 后，在校验请求错误时，会返回 504 错误码，而不是使用过期缓存。
1. 浏览器再次请求访问服务器中的该资源时，根据请求资源的时间与 Cache-Control 中设置的过期时间大小，计算出该资源是否过期，
   1. 如果没有过期（且 Cache-Control 没有设置 no-cache 属性和 no-store 属性），则使用该缓存，结束；
   2. 否则重新请求服务器；
<a name="OIV6E"></a>
### （2）协商缓存
**协商缓存**指的是当第一次请求后，服务器响应头 Cache-Control 字段属性设置为 no-cache 或者缓存时间过期了，那么浏览器再次请求时就会与服务器进行协商，判断缓存资源是否有效，即资源是否进行了修改更新。

- 如果资源没有更新，那么服务器返回 304 状态码，表明缓存仍然可用，而不需要再次发送资源，减少了服务器的数据传输压力，并更新缓存时间。
- 如果数据有更新，服务器返回 200 状态码，新资源存放在请求体中。

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679316726409-220f848e-a1a0-405d-adc0-b0c5dfe4b9b4.png#averageHue=%23fafafa&clientId=u51d53653-de0b-4&from=paste&id=u146b330c&originHeight=676&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uab33a381-bc61-438a-8585-1c03ff47f59&title=)<br />协商缓存可以基于以下两种方式来实现：<br />**第一种（HTTP/1.0 规范）**：请求头部中的 If-Modified-Since 字段与响应头部中的 Last-Modified 字段：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679316726538-8dafcdec-613e-4cfd-80c8-c58584a36759.png#averageHue=%23f7f3ed&clientId=u51d53653-de0b-4&from=paste&id=u37f3be4f&originHeight=598&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4db8b077-76c1-4a6d-8e2e-daab7a8f1e6&title=)

- Last-Modified：标示这个响应资源的最后修改时间。第一次请求资源后，服务器将在响应头中带上此信息。
- If-Modified-Since：当资源过期了，浏览器再次发起请求的时候带上 Last-Modified 的时间（放在请求头 If-Modified-Since 中），服务器将此时间与被请求资源的最后修改时间进行对比，
- 如果最后修改时间较大，说明资源有被修改过，则返回最新资源和 200 状态码；
   - 否则说明资源无新修改，返回 304 状态码。
- 此种方式存在以下问题：
   - 基于时间实现，可能会由于时间误差而出现不可靠问题，并且只能精确到秒级，在同一秒内，Last-Modified 无感知。
   - 如果某些文件被修改了，但是内容并没有任何变化（比如只是修改时间发生了变化），而 Last-Modified 却改变了，导致文件没法使用缓存。

**第二种（HTTP/1.1 规范）**：请求头部中的 If-None-Match 字段与响应头部中的 ETag 字段：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679316757360-9fe2f632-075c-43ea-877c-05acfcb96450.png#averageHue=%23f7f5ef&clientId=u51d53653-de0b-4&from=paste&id=ub248c1d4&originHeight=617&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uea6a65e3-4b84-484f-927f-970817a8156&title=)

- Etag：唯一标识响应资源，是一个 hash 值；第一次请求资源后，服务器将在响应头中带上此信息。
- If-None-Match：当资源过期了，浏览器再次向服务器发起请求时，会将请求头 If-None-Match 值设置为 Etag 中的值。服务器将此值与资源的 hash 值进行比对：
   - 如果二者相等，则资源没有变化，则返回 304 状态码。
   - 如果资源变化了，则返回新资源和 200 状态码。
- 此种方式存在的问题在于计算 Etag 会消耗系统性能，但可以解决第一种方式所存在的问题，推荐使用。

注意 ：

- 如果 HTTP 响应头部同时有 Etag 和 Last-Modified 字段的时候，**Etag 的优先级更高**，也就是先会判断 Etag 是否变化了，如果 Etag 没有变化，然后再看 Last-Modified。
- Ctrl + F5 强制刷新，会直接向服务器提取数据。
- 按 F5 刷新或浏览器的刷新按钮，默认加上 Cache-Control：max-age=0，即会走协商缓存。
<a name="ZNtr7"></a>
## 5、Cookie
HTTP 是一种**无状态**协议，即其本身不会记忆请求和响应之间的通信状态，那么 Web 服务器就无法判断此请求到底来自于哪个用户，HTTP 协议中并不会保存关于用户的任何信息。这样设计的好处是不需要额外资源保存用户状态信息，减少了服务器的 CPU 及内存资源的消耗。<br />但是随着 Web 的发展，很多业务需要保存用户状态。

- 比如电商网站需要在用户跳转到其他商品页面时，仍然可以保存用户的登录状态。不然用户每访问一次网站都要重新登录一下，过于繁琐，体验效果就很差。
- 比如短视频网站希望记录用户以前看过的视频，以便之后向其精准化推荐感兴趣的视频。

**为了实现保持状态的功能，这就出现了 Cookie。**Cookie （服务器给的凭证）类似于逛商场时的会员卡（商家给的凭证），记录着身份信息，只要出示了会员卡，商场工作人员就能确定身份。同样的，只要给服务器发送报文时带上了 Cookie，他就知道我们是谁了。![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679316757659-83386147-f6e9-4b71-afcd-c03c407dccf8.png#averageHue=%23f6f5f5&clientId=u51d53653-de0b-4&from=paste&id=u192e3ce7&originHeight=443&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub4c440e1-4a1a-49e4-b88c-e5e9efde049&title=)<br />Cookie 中可以包含任意信息，最常见的是包含一个服务器为了进行跟踪而产生的**独特的识别码**。<br />举个栗子：<br />张三在发出第一次请求后，服务器将其状态信息记录下来，比如他的名字、年龄、地址、购物历史等，并通过响应头 Set-Cookie字段，给予其一个 id=12345 的独特识别码作为 Cookie，那么其再次向服务器发出请求时，浏览器会自动在请求报文中的 Cookie 字段中带上 id=12345，服务器就可以通过这个查询到张三的具体信息，从而实现了保持状态的功能。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679316757662-c6161af6-e708-4b20-ac76-71944d65b093.png#averageHue=%23f7f1f0&clientId=u51d53653-de0b-4&from=paste&id=u042ef675&originHeight=473&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2a77dbe1-b965-4c0e-9e87-5a6c70a1371&title=)<br />**Cookie 属性：**

- max-age：过期时间有多长（绝对时间，单位：秒）。
   - 负数，表示浏览器关闭即失效。默认即为 -1。
   - 正数：失效时刻= 创建时刻+ max-age。
   - 0：表示 Cookie 立即删除，即 Cookie 直接过期（从而实现使 cookie 失效）。
- expires：过期时间（相对时间）。
- secure：表示这个 Cookie 只会在 https 的时候才会发送。
- HttpOnly：设置后无法通过使用 JavaScript 脚本访问，可以保障安全，防止攻击者盗用用户 Cookie。
- domain：表示该 Cookie 对于哪个域是有效的。（Cookie 默认是不能直接跨域访问的，但是二级域名是可以共享 cookie 的）

Cookie 的缺点是如果传递的状态信息较多，使得包过大，将会降低网络传输效率。<br />一般浏览器限制 Cookie 大小为 4KB。
<a name="fD6jw"></a>
## 6、HTTP 版本
随着互联网的发展，HTTP 也在不断升级打怪，下面分别介绍一下 HTTP/1.1、HTTP/2 以及 HTTP/3 在前一版本基础上的改进之处。
<a name="YdRhm"></a>
### （1）HTTP/1.1 相比 HTTP/1.0 性能上的改进
HTTP/1.1 是目前最常见的 HTTP 版本，其相对于 HTTP/1.0 有以下改进。
<a name="bVunV"></a>
#### ① 持久连接
这个在前文中已经提到过，HTTP/1.0 中一个 TCP 连接只能发送一个请求和响应，而 HTTP/1.1 进行了优化，同一个 TCP 连接可以发送多次 HTTP 请求，减少了建立和关闭连接的性能开销。![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679316758141-37f32897-c16a-403b-aff3-ef76d85babfa.png#averageHue=%23fbfafa&clientId=u51d53653-de0b-4&from=paste&id=uc4ed0ea8&originHeight=542&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u48f68d94-a986-43c6-ad2b-136057f73b3&title=)<br />Web 服务软件一般都会提供 keepalive_timeout 参数，用来指定 HTTP 持久连接的超时时间。比如设置了 HTTP 持久连接的超时时间是 60 秒，Web 服务软件就会启动一个定时器，如果完成某个 HTTP 请求后，在 60 秒内都没有再发起新的请求，就会触发回调函数来释放该连接。
<a name="eyY1R"></a>
#### ② 管道机制
持久连接虽然可以多个请求复用同一个连接，但是每次都需要等到上一个请求响应完成后，才能发送下一个请求。<br />管道机制中，只要第一个请求发出去了，不必等其回来，就可以发第二个请求出去，即相当于同时发出多个请求，因而可以减少整体的响应时间。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679316759092-b2ac1449-3301-4f86-b8e7-d4c8dd334ee5.png#averageHue=%23fbfbfb&clientId=u51d53653-de0b-4&from=paste&id=uc7c168f5&originHeight=510&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u09074040-f730-4c21-90fe-adbfac93b21&title=)<br />**虽然客户端可以同时发出多个 HTTP 请求，不用⼀个个等待响应，但是服务器必须按照接收请求的顺序依次发送对这些管道化请求的响应**，以保证客户端能够区分出每次请求的响应内容。这存在下面问题：<br />如果服务端在处理一个请求时耗时比较长，那么后续请求的处理都会被阻塞住，会导致客户端迟迟收不到数据，这称为「**队头堵塞**」。<br />实际上，虽然管道机制的想法很好，但实现却非常困难，因而很多浏览器根本不支持它。一般为了提升性能，采用并行多个 TCP 连接的形式来实现请求的同时发送。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679316758296-103b2ca9-6caf-4131-b758-424902c77f16.png#averageHue=%23fcfcfc&clientId=u51d53653-de0b-4&from=paste&id=uce004058&originHeight=636&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u39dcd2ac-445a-4134-ac76-634c985888d&title=)
<a name="qzOCt"></a>
#### ③ 缓存控制
前文已经提到过，HTTP/1.1 在 HTTP/1.0 基础之上，增加了一些请求响应头，以更好的实现对缓存的控制。比如

- 新增 Cache-Control 代替原先的 Expires；
- 新增 If-None-Match 和 Etag 代替原先的  If-Modified-Since和 Last-Modified 。
<a name="ICnc5"></a>
#### ④ 断点续传
利⽤ HTTP 消息头使⽤分块传输编码，将实体主体分块传输。
<a name="lfFDO"></a>
### （2）HTTP/2 相比 HTTP/1.1 性能上的改进
HTTP/2 协议本身是基于 HTTPS 的，因此更加安全，其相对于 HTTP/1.1 有以下改进。
<a name="exG62"></a>
#### ① 头部压缩
HTTP/1.1 中的请求头携带大量信息，而且每次都要重复发送，即使是同样的内容，每次请求都需要附带，这会造成性能的损耗。HTTP/2 进行了优化，引入了**头信息压缩机制**。<br />客户端和服务器**同时维护一张头信息表**，高频出现的字段会存入这个表，生成一个索引号。发送报文时直接使用索引号替代字段。另外，索引表中不存在的字段使用哈夫曼编码**压缩**。<br />同时，多个请求中，如果请求头相同，则后续请求只需要发送差异的部分，**重复的部分无需再发送**。![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679316758467-91ce6047-ee2a-42a9-810e-6092e4d1d9bf.png#averageHue=%23b1b57b&clientId=u51d53653-de0b-4&from=paste&id=ub3e610df&originHeight=703&originWidth=1034&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7b9515ab-2532-49a1-9c2b-3d23ef0b925&title=)
<a name="Qer9O"></a>
#### ② 二进制帧
HTTP/1.1 的报文为纯文本格式，而 HTTP/2 的报文全面采用二进制格式，并将原始的报文拆分为头信息帧（Headers Frame）和数据帧（Data Frame）。采用二进制格式有利于提升数据传输效率。
<a name="D5Vp3"></a>
#### ③ 多路复用
在 HTTP/2 中定义了**流（Stream）**的概念，它是二进制帧的双向传输序列，一个数据流对应着一个完整的请求-响应过程，在同一个请求响应过程中，往返的帧会分配一个**唯一的流编号**（Stream ID）。<br />在流的支持下，HTTP/2 可以在**一个 TCP 连接中传输多个请求或响应，而不用按照顺序一一对应（即实现多路复用**），因为它们属于不同的流，所发送的帧头部都会携带 Stream ID，可以通过此 Stream ID 有效区分不同的请求-响应。![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679316758601-32207998-64fd-40bf-82e5-5871ae8b1f6d.png#averageHue=%23f7f5f4&clientId=u51d53653-de0b-4&from=paste&id=u49d4a356&originHeight=393&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4cbf6ac2-9c71-4669-ae66-4fd3820f396&title=)<br />因而 HTTP/2 解决了 HTTP/1.1 的『队头阻塞』问题，多个请求 - 响应之间没有了顺序关系，不需要排队等待，降低了延迟，大幅度提高了连接的利用率。<br />举个栗子：<br />在一个 TCP 连接里面，服务器同时收到了 A 请求和 B 请求，于是先回应 A 请求，结果发现处理过程非常耗时，于是就发送 A 请求已经处理好的部分，接着回应 B 请求，完成后，再发送 A 请求剩下的部分。
<a name="Yb9pE"></a>
#### ④ 服务端推送
在 HTTP/1.1 中，只能客户端发起请求，服务器对请求进行响应。<br />而在 HTTP/2  中，服务端可以**主动**给客户端推送必要的资源，以减少请求延迟时间。<br />比如当客户端向服务器请求一个 HTML 文件后，服务器除了将此 HTML 文件响应给客户端外，还可以提前主动将此 HTML中所依赖的 JS 和 CSS 文件推送给客户端，这样客户端在解析HTML时，无需耗费额外的请求去得到相应的 JS 和 CSS 文件。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679316758664-57997b6c-9f52-4577-b62e-0abf9d243c37.png#averageHue=%23fcfcfc&clientId=u51d53653-de0b-4&from=paste&id=u9ef1d107&originHeight=352&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2fc5dc98-2f2b-41d1-8ca8-f8dfc2cd522&title=)
<a name="MpWEy"></a>
### （3）HTTP/3 相比 HTTP/2 性能上的改进
Google 公司为了解决 HTTP/2 存在的一些问题，提出了 QUIC 协议，而 HTTP-over-QUIC 就是 HTTP/3，其相对于 HTTP/2 有以下改进。
<a name="zKrvI"></a>
#### ① 无队头阻塞
前面提到，HTTP/2 通过**多路复用**解决了 HTTP1.1 的『队头阻塞』问题，但其只是解决了 HTTP 这一层面的『队头阻塞』问题，底层仍然采用的 TCP 连接，HTTP/2 并没有解决 TCP 的『队头阻塞』问题。<br />TCP 是可靠的、面向字节流的协议。HTTP/2 的多个请求虽然可以跑在同一个 TCP 连接中，但如果出现丢包现象，TCP 就需要进行重传，这可能就会导致整个 TCP 连接上的所有流阻塞，直到丢的包重传成功，这就是 TCP 的『队头阻塞』问题。<br />为了解决此问题，**HTTP/3 底层不再使用 TCP，而是采用 UDP**！而 UDP 是无连接的，多个流互相独立，之间不再有依赖，因而即使某个流发生了丢包，只会对该流产生影响，并不会使得其他流阻塞！<br />这时候有的小伙伴可能会问了，HTTP/3 底层不采用 TCP，那怎么保证可靠传输呢？答案就是 HTTP/3 在应用层自己重新实现了可靠性机制。也就是说，**HTTP/3 将原先 TCP 协议提供的部分功能上移至 QUIC，而且进行了改进。**![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679316758692-203769a9-b759-4441-85a2-5474b06ed0cf.png#averageHue=%23e3d7a9&clientId=u51d53653-de0b-4&from=paste&id=ua33de125&originHeight=529&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0fd76e48-f271-4e86-8ec4-93813f2f06c&title=)
<a name="Jdgs4"></a>
#### ② 优化重传机制
TCP 采用**序号+确认号+超时重传**机制来保证消息的可靠性，即如果某条消息超过一定时间还没有得到确认，则重新发送此消息。<br />由于网络拥堵情况不断变化，因而消息的超时时间并不是固定的，而是通过不断采样消息的往返时间不断调整的，但 **TCP 超时采样存在不准确的问题**。<br />举个栗子：<br />客户端发送一个序号为 N 的包，然后超时了（可能丢了，也可能网络堵塞了），于是重新发送一个序号为 N 的包，之后服务器收到后返回一个确认号 ACK 为 N+1 的包。但此时客户端并无法判断这个确定包是对原始报文的确认还是重传报文的确认，那么此时往返时间应该如何计算呢？

- 如果认为确认包是对原始报文的确认，则可能把时间算长了；
- 如果认为确认包是对重传报文的确认，则可能包时间算长了。

因而 TCP 的重传超时时间计算不准确，如果计算偏大，则效率慢，很久才会重传，而如果计算偏小，则可能确认报文已经在路上了，但却重传了！![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679316758893-b1b26b47-32b2-4ec8-b1b0-ec12e1efb8a7.png#averageHue=%23faf9f9&clientId=u51d53653-de0b-4&from=paste&id=u84179de2&originHeight=501&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2981f3c7-eb93-46bc-b1a3-e96c3fc526b&title=)<br />QUIC 是如何解决此问题呢？其定义了一个**递增**的序列号（不再叫 Seq，而是 Packet Number），每个序列号的包只发送一次，**即使重传相同的包，其序列号也不一样**。<br />举个栗子：<br />客户端发送一个序号为 N 的包，然后超时了，于是重新发送一个相同的包，但序号不再是 N，而是 N+1；那么如果返回的确认包 ACK 为 N+1，就是对原始报文的响应，如果 ACK 为 N+2，就是对重传报文的响应，因而采样时间计算相对更加准确！<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679316758970-a7687381-7d4e-415a-9195-711feb878ddf.png#averageHue=%23f9f9f9&clientId=u51d53653-de0b-4&from=paste&id=u6d21c1b7&originHeight=516&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5e4cbecf-7684-47c4-a833-e66ad1fdcdf&title=)<br />那此时怎么知道包 N 和包 N+1 是同一个包呢？**QUIC 定义了一个 Offset 概念**。发送的数据有个偏移量 Offset，可以通过 Offset 知道数据目前发送到了哪里，因而如果某个 Offset 的包没有收到确认，就重发。
<a name="oRY1q"></a>
#### ③ 连接迁移
众所周知，一条 TCP 连接是由四元组标识的，分别是**源 IP、源端口、目的 IP、目的端口**。一旦其中一个元素发生了变化，就需要断开重连。<br />当手机信号不稳定或者在 WIFI 与移动网络切换时，都将会导致重连，而重连就意味着需要重新进行三次握手，将产生一定的时延，用户感到卡顿，体验不友好。<br />而 QUIC 不采用四元组的方式标识连接，而是以一个 **64 位的随机数作为 ID** 来标识，通过此连接 ID 标记通信的两端，之后即使网络发生变化，IP 或端口变了，但只要 ID 不变，则无需重连，只需要复用原先连接即可，时延低，减少了用户的卡顿感，实现连接迁移。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679316759146-032654df-f97f-4e8b-a92b-ecd2cb1e059f.png#averageHue=%23f8f6f5&clientId=u51d53653-de0b-4&from=paste&id=ud3b60097&originHeight=300&originWidth=775&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf94d142d-1e77-48c3-bc53-e1cbcff6ec7&title=)
<a name="nrQCq"></a>
### （4）总结
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679316759154-6ed97756-8365-4fe0-9100-34cdfbc1f230.png#averageHue=%23c4d3e4&clientId=u51d53653-de0b-4&from=paste&id=u1674c4fc&originHeight=569&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4747f82e-55fb-450e-87a6-0ffd17543d6&title=)
