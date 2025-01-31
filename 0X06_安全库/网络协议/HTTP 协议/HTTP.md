网络协议 HTTP
<a name="ilpyT"></a>
## HTTP
<a name="FTD0i"></a>
### 能否详细解释「超文本传输协议」？
HTTP的名字「超文本协议传输」，它可以拆成三个部分：

- 超文本
- 传输
- 协议

![三个部分](https://cdn.nlark.com/yuque/0/2022/png/396745/1651194975162-bf05329d-af14-4054-8d43-3de541465995.png#averageHue=%23f6f6c5&clientId=u35b0b29f-c957-4&from=paste&id=u3488f81c&originHeight=204&originWidth=531&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u523d3926-3716-4ed3-a342-d3f3cb9ab04&title=%E4%B8%89%E4%B8%AA%E9%83%A8%E5%88%86 "三个部分")
<a name="sgkq2"></a>
#### 1、「协议」
在生活中，也能随处可见「协议」，例如：

- 刚毕业时会签一个「三方协议」；
- 找房子时会签一个「租房协议」；

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651194975176-87ce763a-4003-4dea-a2d3-e2d1bc9b3561.png#averageHue=%23edd7cc&clientId=u35b0b29f-c957-4&from=paste&id=u04c0873a&originHeight=264&originWidth=519&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ufa16ce4a-c670-4191-a6c8-71331364e46&title=)<br />生活中的协议，本质上与计算机中的协议是相同的，协议的特点:

- 「**协**」字，代表的意思是必须有**两个以上的参与者**。例如三方协议里的参与者有三个：你、公司、学校三个；租房协议里的参与者有两个：你和房东。
- 「**议**」字，代表的意思是对参与者的一种**行为约定和规范**。例如三方协议里规定试用期期限、毁约金等；租房协议里规定租期期限、每月租金金额、违约如何处理等。

针对 HTTP **协议**，可以这么理解。<br />HTTP 是一个用在计算机世界里的**协议**。它使用计算机能够理解的语言确立了一种计算机之间交流通信的规范（**两个以上的参与者**），以及相关的各种控制和错误处理方式（**行为约定和规范**）。
<a name="crnTf"></a>
#### 2、「传输」
所谓的「传输」，很好理解，就是把一堆东西从 A 点搬到 B 点，或者从 B 点 搬到 A 点。<br />别轻视了这个简单的动作，它至少包含两项重要的信息。<br />HTTP 协议是一个**双向协议**。<br />在上网冲浪时，浏览器是请求方 A ，百度网站就是应答方 B。双方约定用 HTTP 协议来通信，于是浏览器把请求数据发送给网站，网站再把一些数据返回给浏览器，最后由浏览器渲染在屏幕，就可以看到图片、视频了。<br />![请求 - 应答](https://cdn.nlark.com/yuque/0/2022/png/396745/1651194975180-c0698cb0-3aa0-4829-b51e-d227e0d3298c.png#averageHue=%238bbd5a&clientId=u35b0b29f-c957-4&from=paste&id=uda8363c3&originHeight=271&originWidth=758&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=uf13ed885-6642-4814-96d1-73e030a1803&title=%E8%AF%B7%E6%B1%82%20-%20%E5%BA%94%E7%AD%94 "请求 - 应答")<br />数据虽然是在 A 和 B 之间传输，但允许中间有**中转或接力**。<br />就好像第一排的同学想传递纸条给最后一排的同学，那么传递的过程中就需要经过好多个同学（中间人），这样的传输方式就从「A < --- > B」，变成了「A <-> N <-> M <-> B」。<br />而在 HTTP 里，需要中间人遵从 HTTP 协议，只要不打扰基本的数据传输，就可以添加任意额外的东西。<br />针对**传输**，可以进一步理解了 HTTP。<br />HTTP 是一个在计算机世界里专门用来在**两点之间传输数据**的约定和规范。
<a name="oSAhY"></a>
#### 3、「超文本」
HTTP 传输的内容是「超文本」。<br />先来理解「文本」，在互联网早期的时候只是简单的字符文字，但现在「文本」的涵义已经可以扩展为图片、视频、压缩包等，在 HTTP 眼里这些都算作「文本」。<br />再来理解「超文本」，它就是**超越了普通文本的文本**，它是文字、图片、视频等的混合体，最关键有超链接，能从一个超文本跳转到另外一个超文本。<br />HTML 就是最常见的超文本了，它本身只是纯文字文件，但内部用很多标签定义了图片、视频等的链接，再经过浏览器的解释，呈现的就是一个文字、有画面的网页了。<br />OK，经过了对 HTTP 里这三个名词的详细解释，就可以给出比「超文本传输协议」这七个字更准确更有技术含量的答案：<br />**HTTP 是一个在计算机世界里专门在「两点」之间「传输」文字、图片、音频、视频等「超文本」数据的「约定和规范」。**<br />那「HTTP 是用于从互联网服务器传输超文本到本地浏览器的协议 ，这种说法正确吗？<br />这种说法是**不正确**的。因为也可以是「服务器< -- >服务器」，所以采用**两点之间**的描述会更准确。
<a name="JRfEt"></a>
### HTTP 常见的状态码有哪些？
![五大类 HTTP 状态码](https://cdn.nlark.com/yuque/0/2022/png/396745/1651194975231-e1ff3b4b-4eac-4b5d-817f-ef955f982d05.png#averageHue=%237a837b&clientId=u35b0b29f-c957-4&from=paste&id=udb93eeab&originHeight=542&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u9e368b32-aeb2-4c76-86cd-dd528541c5b&title=%E4%BA%94%E5%A4%A7%E7%B1%BB%20HTTP%20%E7%8A%B6%E6%80%81%E7%A0%81 "五大类 HTTP 状态码")<br />1xx 类状态码属于**提示信息**，是协议处理中的一种中间状态，实际用到的比较少。<br />2xx 类状态码表示服务器**成功**处理了客户端的请求，也是最愿意看到的状态。

- 「**200 OK**」是最常见的成功状态码，表示一切正常。如果是非 HEAD 请求，服务器返回的响应头都会有 body 数据。
- 「**204 No Content**」也是常见的成功状态码，与 200 OK 基本相同，但响应头没有 body 数据。
- 「**206 Partial Content**」是应用于 HTTP 分块下载或断点续传，表示响应返回的 body 数据并不是资源的全部，而是其中的一部分，也是服务器处理成功的状态。

3xx 类状态码表示客户端请求的资源发送了变动，需要客户端用新的 URL 重新发送请求获取资源，也就是**重定向**。

- 「**301 Moved Permanently**」表示永久重定向，说明请求的资源已经不存在了，需改用新的 URL 再次访问。
- 「**302 Found**」表示临时重定向，说明请求的资源还在，但暂时需要用另一个 URL 来访问。

301 和 302 都会在响应头里使用字段 Location，指明后续要跳转的 URL，浏览器会自动重定向新的 URL。

- 「**304 Not Modified**」不具有跳转的含义，表示资源未修改，重定向已存在的缓冲文件，也称缓存重定向，也就是告诉客户端可以继续使用缓存资源，用于缓存控制。

4xx 类状态码表示客户端发送的**报文有误**，服务器无法处理，也就是错误码的含义。

- 「**400 Bad Request**」表示客户端请求的报文有错误，但只是个笼统的错误。
- 「**403 Forbidden**」表示服务器禁止访问资源，并不是客户端的请求出错。
- 「**404 Not Found**」表示请求的资源在服务器上不存在或未找到，所以无法提供给客户端。

5xx 类状态码表示客户端请求报文正确，但是**服务器处理时内部发生了错误**，属于服务器端的错误码。

- 「**500 Internal Server Error**」与 400 类型，是个笼统通用的错误码，服务器发生了什么错误，并不知道。
- 「**501 Not Implemented**」表示客户端请求的功能还不支持，类似“即将开业，敬请期待”的意思。
- 「**502 Bad Gateway**」通常是服务器作为网关或代理时返回的错误码，表示服务器自身工作正常，访问后端服务器发生了错误。
- 「**503 Service Unavailable**」表示服务器当前很忙，暂时无法响应服务器，类似“网络服务正忙，请稍后重试”的意思。
<a name="vKLdA"></a>
### HTTP 常见字段有哪些？
<a name="MauFf"></a>
#### Host 字段
客户端发送请求时，用来指定服务器的域名。<br />![Host: www.A.com](https://cdn.nlark.com/yuque/0/2022/png/396745/1651194975263-ee7ceeac-dc38-49d1-94eb-885c22836fae.png#averageHue=%23f9f8f5&clientId=u35b0b29f-c957-4&from=paste&id=ub8fb7f80&originHeight=444&originWidth=812&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u10f80716-e18d-4c37-8e6a-80cb13e7470&title=Host%3A%20www.A.com "Host: www.A.com")<br />有了 Host 字段，就可以将请求发往「同一台」服务器上的不同网站。
<a name="sziPv"></a>
#### Content-Length 字段
服务器在返回数据时，会有 Content-Length 字段，表明本次回应的数据长度。<br />![Content-Length: 1000](https://cdn.nlark.com/yuque/0/2022/png/396745/1651194975659-1dc70a42-d9ed-439e-b02f-021f801dfe57.png#averageHue=%23fdfef9&clientId=u35b0b29f-c957-4&from=paste&id=u2034c450&originHeight=212&originWidth=422&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u5dc58fd7-cfbb-4a2b-9be6-43606b7f5f0&title=Content-Length%3A%201000 "Content-Length: 1000")<br />如上面则是告诉浏览器，本次服务器回应的数据长度是 1000 个字节，后面的字节就属于下一个回应了。
<a name="SBuno"></a>
#### Connection 字段
Connection 字段最常用于客户端要求服务器使用 TCP 持久连接，以便其他请求复用。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651194975721-39c43c03-5301-4c86-9313-ea4720ad5148.png#averageHue=%23f4e9c8&clientId=u35b0b29f-c957-4&from=paste&id=u2a0bdc4f&originHeight=272&originWidth=512&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u15dd0b4d-6dec-4080-b687-78dba59d656&title=)<br />HTTP/1.1 版本的默认连接都是持久连接，但为了兼容老版本的 HTTP，需要指定 Connection 首部字段的值为 Keep-Alive。
<a name="LimnA"></a>
#### Connection: keep-alive
一个可以复用的 TCP 连接就建立了，直到客户端或服务器主动关闭连接。但是，这不是标准字段。
<a name="pAAW5"></a>
#### Content-Type 字段
Content-Type 字段用于服务器回应时，告诉客户端，本次数据是什么格式。<br />![Content-Type: text/html; charset=utf-8](https://cdn.nlark.com/yuque/0/2022/png/396745/1651194975803-4fa40b0c-5752-46a8-9a9b-e1bea74be6c6.png#averageHue=%23f0e7c8&clientId=u35b0b29f-c957-4&from=paste&id=u495217b2&originHeight=317&originWidth=707&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ua72169bb-c698-493c-99af-7303b03daf4&title=Content-Type%3A%20text%2Fhtml%3B%20charset%3Dutf-8 "Content-Type: text/html; charset=utf-8")<br />上面的类型表明，发送的是网页，而且编码是UTF-8。<br />客户端请求的时候，可以使用 Accept 字段声明自己可以接受哪些数据格式。
```
Accept: */*
```
上面代码中，客户端声明自己可以接受任何格式的数据。
<a name="mHYKo"></a>
#### Content-Encoding 字段
Content-Encoding 字段说明数据的压缩方法。表示服务器返回的数据使用了什么压缩格式<br />![Content-Encoding: gzip](https://cdn.nlark.com/yuque/0/2022/png/396745/1651194975832-a874d95e-c462-4852-a263-e5ce1d60eb1e.png#averageHue=%23f5edd0&clientId=u35b0b29f-c957-4&from=paste&id=u6f6ffc0f&originHeight=302&originWidth=677&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=udefbcaab-b5ee-4667-b6bb-2d15a224c29&title=Content-Encoding%3A%20gzip "Content-Encoding: gzip")<br />上面表示服务器返回的数据采用了 gzip 方式压缩，告知客户端需要用此方式解压。<br />客户端在请求时，用 Accept-Encoding 字段说明自己可以接受哪些压缩方法。
```
Accept-Encoding: gzip, deflate
```

---

<a name="FI2G3"></a>
## GET 与 POST
<a name="c7pvY"></a>
### GET 和 POST 有什么区别？
根据 RFC 规范，**GET 的语义是从服务器获取指定的资源**，这个资源可以是静态的文本、页面、图片视频等。GET 请求的参数位置一般是写在 URL 中，URL 规定只能支持 ASCII，所以 GET 请求的参数只允许 ASCII 字符 ，而且浏览器会对 URL 的长度有限制（HTTP协议本身对 URL长度并没有做任何规定）。<br />比如打开文章，浏览器就会发送 GET 请求给服务器，服务器就会返回文章的所有文字及资源。<br />![GET 请求](https://cdn.nlark.com/yuque/0/2022/png/396745/1651194975847-d7b38c35-8282-44e3-9c89-0aa571fb9a65.png#averageHue=%23eee2cb&clientId=u35b0b29f-c957-4&from=paste&id=ub7111ba7&originHeight=321&originWidth=455&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u16ecfa6d-0528-4980-8b48-00b4a1c2c32&title=GET%20%E8%AF%B7%E6%B1%82 "GET 请求")<br />根据 RFC 规范，**POST 的语义是根据请求负荷（报文body）对指定的资源做出处理**，具体的处理方式视资源类型而不同。POST 请求携带数据的位置一般是写在报文 body 中， body 中的数据可以是任意格式的数据，只要客户端与服务端协商好即可，而且浏览器不会对 body 大小做限制。<br />比如，在文章底部，敲入了留言后点击「提交」，浏览器就会执行一次 POST 请求，把留言文字放进了报文 body 里，然后拼接好 POST 请求头，通过 TCP 协议发送给服务器。<br />![POST 请求](https://cdn.nlark.com/yuque/0/2022/png/396745/1651194976334-e456c4a2-aaa3-4cef-a694-cd394fda4460.png#averageHue=%23ede5ce&clientId=u35b0b29f-c957-4&from=paste&id=u1ad32846&originHeight=231&originWidth=455&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u955c83ae-fe9b-4254-b7e5-b48d57dbd86&title=POST%20%E8%AF%B7%E6%B1%82 "POST 请求")
<a name="h5fkW"></a>
### GET 和 POST 方法都是安全和幂等的吗？
先说明下安全和幂等的概念：

- 在 HTTP 协议里，所谓的「安全」是指请求方法不会「破坏」服务器上的资源。
- 所谓的「幂等」，意思是多次执行相同的操作，结果都是「相同」的。

如果从 RFC 规范定义的语义来看：

- **GET 方法就是安全且幂等的**，因为它是「只读」操作，无论操作多少次，服务器上的数据都是安全的，且每次的结果都是相同的。所以，**可以对 GET 请求的数据做缓存，这个缓存可以做到浏览器本身上（彻底避免浏览器发请求），也可以做到代理上（如nginx），而且在浏览器中 GET 请求可以保存位书签**。
- **POST** 因为是「新增或提交数据」的操作，会修改服务器上的资源，所以是**不安全**的，且多次提交数据就会创建多个资源，所以**不是幂等**的。所以，**浏览器一般不会缓存 POST 请求，也不能把 POST 请求保存为书签**。

做个简要的小结。<br />GET 的语义是请求获取指定的资源。GET 方法是安全、幂等、可被缓存的。<br />POST 的语义是根据请求负荷（报文主体）对指定的资源做出处理，具体的处理方式视资源类型而不同。POST 不安全，不幂等，（大部分实现）不可缓存。<br />注意， 上面是从 RFC 规范定义的语义来分析的。<br />但是实际过程中，开发者不一定会按照 RFC 规范定义的语义来实现 GET 和 POST 方法。比如：

- 可以用 GET 方法实现新增或删除数据的请求，这样实现的 GET 方法自然就不是安全和幂等。
- 可以用 POST 方法实现查询数据的请求，这样实现的  POST 方法自然就是安全和幂等。

曾经有个笑话，有人写了个博客，删除博客用的是GET请求，他觉得没人访问就连鉴权都没做。然后Google服务器爬虫爬了一遍，他所有博文就没了。。。<br />如果「安全」放入概念是指信息是否会被泄漏的话，虽然 POST 用 body 传输数据，而 GET 用 URL 传输，这样数据会在浏览器地址拦容易看到，但是并不能说 GET 不如 POST 安全的。<br />因为 HTTP 传输的内容都是明文的，虽然在浏览器地址拦看不到 POST 提交的 body 数据，但是只要抓个包就都能看到了。<br />所以，要避免传输过程中数据被窃取，就要使用 HTTPS 协议，这样所有 HTTP 的数据都会被加密传输。
<a name="zEqOt"></a>
### GET 请求可以带 body 吗？
RFC 规范并没有规定 GET 请求不能带 body 的。理论上，任何请求都可以带 body 的。只是因为 RFC 规范定义的 GET 请求是获取资源，所以根据这个语义不需要用到 body。<br />另外，URL 中的查询参数也不是 GET 所独有的，POST 请求的 URL 中也可以有参数的。
<a name="jzCKI"></a>
## HTTP 缓存技术
<a name="K4kvE"></a>
### HTTP 缓存有哪些实现方式？
对于一些具有重复性的 HTTP 请求，比如每次请求得到的数据都一样的，可以把这对「请求-响应」的数据都**缓存在本地**，那么下次就直接读取本地的数据，不必在通过网络获取服务器的响应了，这样的话 HTTP/1.1 的性能肯定肉眼可见的提升。<br />所以，避免发送 HTTP 请求的方法就是通过**缓存技术**，HTTP 设计者早在之前就考虑到了这点，因此 HTTP 协议的头部有不少是针对缓存的字段。<br />HTTP 缓存有两种实现方式，分别是**强制缓存和协商缓存**。
<a name="dUMbb"></a>
### 什么是强制缓存？
强缓存指的是只要浏览器判断缓存没有过期，则直接使用浏览器的本地缓存，决定是否使用缓存的主动性在于浏览器这边。<br />如下图中，返回的是 200 状态码，但在 size 项中标识的是 from disk cache，就是使用了强制缓存。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651194976348-b7d72758-38e3-47a8-ad51-2d550e33d9a6.png#averageHue=%23f8f1f0&clientId=u35b0b29f-c957-4&from=paste&id=u9cdc0d0e&originHeight=374&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u427dbe8e-99dc-411d-8bdc-488dc743d15&title=)<br />强缓存是利用下面这两个 HTTP 响应头部（Response Header）字段实现的，它们都用来表示资源在客户端缓存的有效期：

- Cache-Control， 是一个相对时间；
- Expires，是一个绝对时间；

如果 HTTP 响应头部同时有 Cache-Control 和 Expires 字段的话，**Cache-Control的优先级高于 Expires** 。<br />Cache-control 选项更多一些，设置更加精细，所以建议使用 Cache-Control 来实现强缓存。具体的实现流程如下：

- 当浏览器第一次请求访问服务器资源时，服务器会在返回这个资源的同时，在 Response 头部加上 Cache-Control，Cache-Control 中设置了过期时间大小；
- 浏览器再次请求访问服务器中的该资源时，会先**通过请求资源的时间与 Cache-Control 中设置的过期时间大小，来计算出该资源是否过期**，如果没有，则使用该缓存，否则重新请求服务器；
- 服务器再次收到请求后，会再次更新 Response 头部的 Cache-Control。
<a name="rUuMQ"></a>
### 什么是协商缓存？
当在浏览器使用开发者工具的时候，可能会看到过某些请求的响应码是 304，这个是告诉浏览器可以使用本地缓存的资源，通常这种通过服务端告知客户端是否可以使用缓存的方式被称为协商缓存。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651194976468-506b3c17-45d4-4835-84fe-16ceb8e4d9bc.png#averageHue=%23f7f5f3&clientId=u35b0b29f-c957-4&from=paste&id=uacebdd33&originHeight=1127&originWidth=1017&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua4f918e7-88cf-4fd1-8a6d-f5b1883f2f8&title=)<br />上图就是一个协商缓存的过程，所以**协商缓存就是与服务端协商之后，通过协商结果来判断是否使用本地缓存**。<br />协商缓存可以基于两种头部来实现。<br />第一种：请求头部中的 If-Modified-Since 字段与响应头部中的 Last-Modified 字段实现，这两个字段的意思是：

- 响应头部中的 Last-Modified：标示这个响应资源的最后修改时间；
- 请求头部中的 If-Modified-Since：当资源过期了，发现响应头中具有 Last-Modified 声明，则再次发起请求的时候带上 Last-Modified 的时间，服务器收到请求后发现有 If-Modified-Since 则与被请求资源的最后修改时间进行对比（Last-Modified），如果最后修改时间较新（大），说明资源又被改过，则返回最新资源，HTTP 200 OK；如果最后修改时间较旧（小），说明资源无新修改，响应 HTTP 304 走缓存。

第二种：请求头部中的 If-None-Match 字段与响应头部中的 ETag 字段，这两个字段的意思是：

- 响应头部中 Etag：唯一标识响应资源；
- 请求头部中的 If-None-Match：当资源过期时，浏览器发现响应头里有 Etag，则再次向服务器发起请求时，会将请求头If-None-Match 值设置为 Etag 的值。服务器收到请求后进行比对，如果资源没有变化返回 304，如果资源变化了返回 200。

第一种实现方式是基于时间实现的，第二种实现方式是基于一个唯一标识实现的，相对来说后者可以更加准确地判断文件内容是否被修改，避免由于时间篡改导致的不可靠问题。<br />如果 HTTP 响应头部同时有 Etag 和 Last-Modified 字段的时候， Etag 的优先级更高，也就是先会判断 Etag 是否变化了，如果 Etag 没有变化，然后再看  Last-Modified。<br />注意，**协商缓存这两个字段都需要配合强制缓存中 Cache-control 字段来使用，只有在未能命中强制缓存的时候，才能发起带有协商缓存字段的请求**。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651194976503-b5ef1f67-a54b-4502-b238-e61d712b4439.png#averageHue=%23878686&clientId=u35b0b29f-c957-4&from=paste&id=u34c94a55&originHeight=528&originWidth=554&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ueb537560-4be4-4549-b577-c7fb5f9e5d9&title=)<br />使用 ETag 字段实现的协商缓存的过程如下；

- 当浏览器第一次请求访问服务器资源时，服务器会在返回这个资源的同时，在 Response 头部加上 ETag 唯一标识，这个唯一标识的值是根据当前请求的资源生成的；
- 当浏览器再次请求访问服务器中的该资源时，首先会先检查强制缓存是否过期，如果没有过期，则直接使用本地缓存；如果缓存过期了，会在 Request 头部加上 If-None-Match 字段，该字段的值就是 ETag 唯一标识；
- 服务器再次收到请求后，**会根据请求中的 If-None-Match 值与当前请求的资源生成的唯一标识进行比较**：
   - **如果值相等，则返回 304 Not Modified，不会返回资源**；
   - 如果不相等，则返回 200 状态码和返回资源，并在 Response 头部加上新的 ETag 唯一标识；
- 如果浏览器收到 304 的请求响应状态码，则会从本地缓存中加载资源，否则更新资源。
<a name="cuc0I"></a>
## HTTP 特性
<a name="fU89t"></a>
### HTTP（1.1） 的优点有哪些？
HTTP 最凸出的优点是「简单、灵活和易于扩展、应用广泛和跨平台」。
<a name="iBMk5"></a>
#### 1、简单
HTTP 基本的报文格式就是 header + body，头部信息也是 key-value 简单文本的形式，**易于理解**，降低了学习和使用的门槛。
<a name="dWEQe"></a>
#### 2、灵活和易于扩展
HTTP协议里的各类请求方法、URI/URL、状态码、头字段等每个组成要求都没有被固定死，都允许开发人员**自定义和扩充**。<br />同时 HTTP 由于是工作在应用层（ OSI 第七层），则它**下层可以随意变化**。<br />HTTPS 也就是在 HTTP 与 TCP 层之间增加了 SSL/TLS 安全传输层，HTTP/3 甚至把 TCP 层换成了基于 UDP 的 QUIC。
<a name="C7MHY"></a>
#### 3、应用广泛和跨平台
互联网发展至今，HTTP 的应用范围非常的广泛，从台式机的浏览器到手机上的各种 APP，从看新闻、刷贴吧到购物、理财、吃鸡，HTTP 的应用遍地开花，同时天然具有**跨平台**的优越性。
<a name="WgWf9"></a>
### HTTP（1.1） 的缺点有哪些？
HTTP 协议里有优缺点一体的**双刃剑**，分别是「无状态、明文传输」，同时还有一大缺点「不安全」。
<a name="bfWwL"></a>
#### 1、无状态双刃剑
无状态的**好处**，因为服务器不会去记忆 HTTP 的状态，所以不需要额外的资源来记录状态信息，这能减轻服务器的负担，能够把更多的 CPU 和内存用来对外提供服务。<br />无状态的**坏处**，既然服务器没有记忆能力，它在完成有关联性的操作时会非常麻烦。<br />例如登录->添加购物车->下单->结算->支付，这系列操作都要知道用户的身份才行。但服务器不知道这些请求是有关联的，每次都要问一遍身份信息。<br />这样每操作一次，都要验证信息，这样的购物体验还能愉快吗？别问，问就是**酸爽**！<br />对于无状态的问题，解法方案有很多种，其中比较简单的方式用 **Cookie** 技术。<br />Cookie 通过在请求和响应报文中写入 Cookie 信息来控制客户端的状态。<br />相当于，**在客户端第一次请求后，服务器会下发一个装有客户信息的「小贴纸」，后续客户端请求服务器的时候，带上「小贴纸」，服务器就能认得了了**，<br />![Cookie 技术](https://cdn.nlark.com/yuque/0/2022/png/396745/1651194976501-b4820f2c-2fca-4bf4-b7ee-939418ed0677.png#averageHue=%23ddcfb8&clientId=u35b0b29f-c957-4&from=paste&id=ue24aceb1&originHeight=691&originWidth=719&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u3d8dfa3b-e840-47ad-bbd6-ddd9070d7f9&title=Cookie%20%E6%8A%80%E6%9C%AF "Cookie 技术")
<a name="KTOyB"></a>
#### 2、明文传输双刃剑
明文意味着在传输过程中的信息，是可方便阅读的，通过浏览器的 F12 控制台或 Wireshark 抓包都可以直接肉眼查看，为调试工作带了极大的便利性。<br />但是这正是这样，HTTP 的所有信息都暴露在了光天化日下，相当于**信息裸奔**。在传输的漫长的过程中，信息的内容都毫无隐私可言，很容易就能被窃取，如果里面有账号密码信息，那**你号没了**。
<a name="oV3Lu"></a>
#### 3、不安全
HTTP 比较严重的缺点就是不安全：

- 通信使用明文（不加密），内容可能会被窃听。比如，**账号信息容易泄漏，那你号没了。**
- 不验证通信方的身份，因此有可能遭遇伪装。比如，**访问假的淘宝、拼多多，那你钱没了。**
- 无法证明报文的完整性，所以有可能已遭篡改。比如，**网页上植入垃圾广告，视觉污染，眼没了。**

HTTP 的安全问题，可以用 HTTPS 的方式解决，也就是通过引入 SSL/TLS 层，使得在安全上达到了极致。
<a name="zBKY3"></a>
### HTTP/1.1 的性能如何？
HTTP 协议是基于 **TCP/IP**，并且使用了「**请求 - 应答**」的通信模式，所以性能的关键就在这**两点**里。
<a name="zfreR"></a>
#### 1、长连接
早期 HTTP/1.0 性能上的一个很大的问题，那就是每发起一个请求，都要新建一次 TCP 连接（三次握手），而且是串行请求，做了无谓的 TCP 连接建立和断开，增加了通信开销。<br />为了解决上述 TCP 连接问题，HTTP/1.1 提出了**长连接**的通信方式，也叫持久连接。这种方式的好处在于减少了 TCP 连接的重复建立和断开所造成的额外开销，减轻了服务器端的负载。<br />持久连接的特点是，只要任意一端没有明确提出断开连接，则保持 TCP 连接状态。<br />![短连接与长连接](https://cdn.nlark.com/yuque/0/2022/png/396745/1651194976821-b1feb3d7-b98f-42b5-82eb-b78c84c09202.png#averageHue=%23f7f5f4&clientId=u35b0b29f-c957-4&from=paste&id=u69a42e6b&originHeight=472&originWidth=616&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ub9a95d3f-47b4-4c45-8c69-94286b49457&title=%E7%9F%AD%E8%BF%9E%E6%8E%A5%E4%B8%8E%E9%95%BF%E8%BF%9E%E6%8E%A5 "短连接与长连接")<br />当然，如果某个 HTTP 长连接超过一定时间没有任何数据交互，服务端就会主动断开这个连接。
<a name="bsUeD"></a>
#### 2、管道网络传输
HTTP/1.1 采用了长连接的方式，这使得管道（pipeline）网络传输成为了可能。<br />即可在同一个 TCP 连接里面，客户端可以发起多个请求，只要第一个请求发出去了，不必等其回来，就可以发第二个请求出去，可以**减少整体的响应时间。**<br />举例来说，客户端需要请求两个资源。以前的做法是，在同一个 TCP 连接里面，先发送 A 请求，然后等待服务器做出回应，收到后再发出 B 请求。那么，管道机制则是允许浏览器同时发出 A 请求和 B 请求，如下图：<br />![管道网络传输](https://cdn.nlark.com/yuque/0/2022/png/396745/1651194976956-de4a62a1-03bf-4041-ad10-4c23f283e095.png#averageHue=%23faf9f8&clientId=u35b0b29f-c957-4&from=paste&id=u6d31e5a4&originHeight=381&originWidth=351&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ua9ae0d1a-7401-4c79-b866-a8e8b7b2f1c&title=%E7%AE%A1%E9%81%93%E7%BD%91%E7%BB%9C%E4%BC%A0%E8%BE%93 "管道网络传输")<br />但是**服务器必须按照接收请求的顺序发送对这些管道化请求的响应**。<br />注意，是按照服务端收到的请求顺序响应，并不管哪个请求是先发送的，假设客户端先发送 A 请求，后发送 B 请求，如果服务端先收到 B 请求，就先响应 B 请求，然后再响应 A 请求，但是假设处理 B 请求的时候，耗时比较长，那么请求 A 的响应就会被阻塞，这称为「队头堵塞」。<br />所以，**HTTP/1.1 管道解决了请求的队头阻塞，但是没有解决响应的队头阻塞**。
<a name="xuq3o"></a>
#### 3、队头阻塞
「请求 - 应答」的模式加剧了 HTTP 的性能问题。<br />因为当顺序发送的请求序列中的一个请求因为某种原因被阻塞时，在后面排队的所有请求也一同被阻塞了，会招致客户端一直请求不到数据，这也就是「**队头阻塞**」，好比上班的路上塞车。<br />![队头阻塞](https://cdn.nlark.com/yuque/0/2022/png/396745/1651194976958-cd921561-ee5c-4258-bf8f-f88a2518f183.png#averageHue=%23f8f4ee&clientId=u35b0b29f-c957-4&from=paste&id=ua8b88945&originHeight=422&originWidth=387&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u0cd20fdf-250b-4f63-aed0-a324d373adf&title=%E9%98%9F%E5%A4%B4%E9%98%BB%E5%A1%9E "队头阻塞")<br />总之 HTTP/1.1 的性能一般般，后续的 HTTP/2 和 HTTP/3 就是在优化 HTTP 的性能。

---

<a name="GH3E1"></a>
## HTTP 与 HTTPS
<a name="VreSf"></a>
### HTTP 与 HTTPS 有哪些区别？

1. HTTP 是超文本传输协议，信息是明文传输，存在安全风险的问题。HTTPS 则解决 HTTP 不安全的缺陷，在 TCP 和 HTTP 网络层之间加入了 SSL/TLS 安全协议，使得报文能够加密传输。
2. HTTP 连接建立相对简单， TCP 三次握手之后便可进行 HTTP 的报文传输。而 HTTPS 在 TCP 三次握手之后，还需进行 SSL/TLS 的握手过程，才可进入加密报文传输。
3. HTTP 的端口号是 80，HTTPS 的端口号是 443。
4. HTTPS 协议需要向 CA（证书权威机构）申请数字证书，来保证服务器的身份是可信的。
<a name="bA07y"></a>
### HTTPS 解决了 HTTP 的哪些问题？
HTTP 由于是明文传输，所以安全上存在以下三个风险：

- **窃听风险**，比如通信链路上可以获取通信内容，用户号容易没。
- **篡改风险**，比如强制植入垃圾广告，视觉污染，用户眼容易瞎。
- **冒充风险**，比如冒充淘宝网站，用户钱容易没。

![HTTP 与 HTTPS 网络层](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1651194977340-28cb72ef-663f-4fd2-a5ac-1852087331f4.jpeg#averageHue=%23dce1db&clientId=u35b0b29f-c957-4&from=paste&id=u9ecca5c5&originHeight=275&originWidth=596&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u936bb6e5-c6ec-4a85-9720-ffe4d452d00&title=HTTP%20%E4%B8%8E%20HTTPS%20%E7%BD%91%E7%BB%9C%E5%B1%82 "HTTP 与 HTTPS 网络层")<br />HTTP**S** 在 HTTP 与 TCP 层之间加入了 SSL/TLS 协议，可以很好的解决了上述的风险：

- **信息加密**：交互信息无法被窃取，但你的号会因为「自身忘记」账号而没。
- **校验机制**：无法篡改通信内容，篡改了就不能正常显示，但百度「竞价排名」依然可以搜索垃圾广告。
- **身份证书**：证明淘宝是真的淘宝网，但你的钱还是会因为「剁手」而没。

可见，只要自身不做「恶」，SSL/TLS 协议是能保证通信是安全的。
<a name="xGVMm"></a>
### HTTPS 是如何解决上面的三个风险的？

- **混合加密**的方式实现信息的**机密性**，解决了窃听的风险。
- **摘要算法**的方式来实现**完整性**，它能够为数据生成独一无二的「指纹」，指纹用于校验数据的完整性，解决了篡改的风险。
- 将服务器公钥放入到**数字证书**中，解决了冒充的风险。
<a name="yKJH0"></a>
#### 1、混合加密
通过**混合加密**的方式可以保证信息的**机密性**，解决了窃听的风险。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651194977414-39cb04f0-8c1c-4b60-9fef-d42a3a9634ce.png#averageHue=%23f3dd9b&clientId=u35b0b29f-c957-4&from=paste&id=u8ed2e520&originHeight=471&originWidth=613&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc314de94-e35a-4efe-bb3d-964d5e561e4&title=)<br />HTTPS 采用的是**对称加密**和**非对称加密**结合的「混合加密」方式：

- 在通信建立前采用**非对称加密**的方式交换「会话秘钥」，后续就不再使用非对称加密。
- 在通信过程中全部使用**对称加密**的「会话秘钥」的方式加密明文数据。

采用「混合加密」的方式的原因：

- **对称加密**只使用一个密钥，运算速度快，密钥必须保密，无法做到安全的密钥交换。
- **非对称加密**使用两个密钥：公钥和私钥，公钥可以任意分发而私钥保密，解决了密钥交换问题但速度慢。
<a name="shLag"></a>
#### 2、摘要算法
**摘要算法**用来实现**完整性**，能够为数据生成独一无二的「指纹」，用于校验数据的完整性，解决了篡改的风险。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651194977360-3f56f0cb-b742-48ab-a23b-f257ec1403f1.png#averageHue=%23e6e5e0&clientId=u35b0b29f-c957-4&from=paste&id=uc0513148&originHeight=241&originWidth=713&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u81b6552c-9e38-4df6-9cbc-e412543bfaa&title=)<br />校验完整性<br />客户端在发送明文之前会通过摘要算法算出明文的「指纹」，发送的时候把「指纹 + 明文」一同加密成密文后，发送给服务器，服务器解密后，用相同的摘要算法算出发送过来的明文，通过比较客户端携带的「指纹」和当前算出的「指纹」做比较，若「指纹」相同，说明数据是完整的。
<a name="NXXCI"></a>
#### 3、数字证书
客户端先向服务器端索要公钥，然后用公钥加密信息，服务器收到密文后，用自己的私钥解密。<br />这就存在些问题，如何保证公钥不被篡改和信任度？<br />所以这里就需要借助第三方权威机构 CA （数字证书认证机构），将**服务器公钥放在数字证书**（由数字证书认证机构颁发）中，只要证书是可信的，公钥就是可信的。<br />![数子证书工作流程](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1651194977534-02bef203-04b2-4dd1-8c23-8f559f4b7905.jpeg#averageHue=%23dfd49d&clientId=u35b0b29f-c957-4&from=paste&id=u7a8b7111&originHeight=577&originWidth=779&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u7a4756bb-61da-432a-a681-4111c3dbdb7&title=%E6%95%B0%E5%AD%90%E8%AF%81%E4%B9%A6%E5%B7%A5%E4%BD%9C%E6%B5%81%E7%A8%8B "数子证书工作流程")<br />通过数字证书的方式保证服务器公钥的身份，解决冒充的风险。
<a name="C9QHJ"></a>
### HTTPS  是如何建立连接的？其间交互了什么？
SSL/TLS 协议基本流程：

- 客户端向服务器索要并验证服务器的公钥。
- 双方协商生产「会话秘钥」。
- 双方采用「会话秘钥」进行加密通信。

前两步也就是 SSL/TLS 的建立过程，也就是 TLS 握手阶段。<br />SSL/TLS 的「握手阶段」涉及**四次**通信，可见下图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651194977511-24280b79-9827-4fde-802d-bce052cffb64.png#averageHue=%23fcf6e5&clientId=u35b0b29f-c957-4&from=paste&id=ubd963677&originHeight=1962&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucbfe301a-cdb9-47b3-b0c3-300e53c8f23&title=)<br />SSL/TLS 协议建立的详细流程：
<a name="VWV9j"></a>
#### 1、ClientHello
首先，由客户端向服务器发起加密通信请求，也就是 ClientHello 请求。<br />在这一步，客户端主要向服务器发送以下信息：<br />（1）客户端支持的 SSL/TLS 协议版本，如 TLS 1.2 版本。<br />（2）客户端生产的随机数（Client Random），后面用于生成「会话秘钥」条件之一。<br />（3）客户端支持的密码套件列表，如 RSA 加密算法。
<a name="nNnzw"></a>
#### 2、SeverHello
服务器收到客户端请求后，向客户端发出响应，也就是 SeverHello。服务器回应的内容有如下内容：<br />（1）确认 SSL/ TLS 协议版本，如果浏览器不支持，则关闭加密通信。<br />（2）服务器生产的随机数（Server Random），也是后面用于生产「会话秘钥」条件之一。<br />（3）确认的密码套件列表，如 RSA 加密算法。<br />（4）服务器的数字证书。
<a name="P8zWt"></a>
#### 3、客户端回应
客户端收到服务器的回应之后，首先通过浏览器或者操作系统中的 CA 公钥，确认服务器的数字证书的真实性。<br />如果证书没有问题，客户端会**从数字证书中取出服务器的公钥**，然后使用它加密报文，向服务器发送如下信息：<br />（1）一个随机数（pre-master key）。该随机数会被服务器公钥加密。<br />（2）加密通信算法改变通知，表示随后的信息都将用「会话秘钥」加密通信。<br />（3）客户端握手结束通知，表示客户端的握手阶段已经结束。这一项同时把之前所有内容的发生的数据做个摘要，用来供服务端校验。<br />上面第一项的随机数是整个握手阶段的第三个随机数，会发给服务端，所以这个随机数客户端和服务端都是一样的。<br />**服务器和客户端有了这三个随机数（Client Random、Server Random、pre-master key），接着就用双方协商的加密算法，各自生成本次通信的「会话秘钥」**。
<a name="RHWEe"></a>
#### 4、服务器的最后回应
服务器收到客户端的第三个随机数（pre-master key）之后，通过协商的加密算法，计算出本次通信的「会话秘钥」。<br />然后，向客户端发送最后的信息：<br />（1）加密通信算法改变通知，表示随后的信息都将用「会话秘钥」加密通信。<br />（2）服务器握手结束通知，表示服务器的握手阶段已经结束。这一项同时把之前所有内容的发生的数据做个摘要，用来供客户端校验。<br />至此，整个 SSL/TLS 的握手阶段全部结束。接下来，客户端与服务器进入加密通信，就完全是使用普通的 HTTP 协议，只不过用「会话秘钥」加密内容。
<a name="FOLci"></a>
## HTTP/1.1、HTTP/2、HTTP/3 演变
<a name="r8vUR"></a>
### HTTP/1.1 相比 HTTP/1.0 提高了什么性能？
HTTP/1.1 相比 HTTP/1.0 性能上的改进：

- 使用 TCP 长连接的方式改善了 HTTP/1.0 短连接造成的性能开销。
- 支持管道（pipeline）网络传输，只要第一个请求发出去了，不必等其回来，就可以发第二个请求出去，可以减少整体的响应时间。

但 HTTP/1.1 还是有性能瓶颈：

- 请求 / 响应头部（Header）未经压缩就发送，首部信息越多延迟越大。只能压缩 Body 的部分；
- 发送冗长的首部。每次互相发送相同的首部造成的浪费较多；
- 服务器是按请求的顺序响应的，如果服务器响应慢，会招致客户端一直请求不到数据，也就是队头阻塞；
- 没有请求优先级控制；
- 请求只能从客户端开始，服务器只能被动响应。
<a name="bIpjn"></a>
### HTTP/2 做了什么优化？
HTTP/2 协议是基于 HTTPS 的，所以 HTTP/2 的安全性也是有保障的。<br />![HTT/1 ~ HTTP/2](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1651194977902-ec6c0d69-6760-4f93-b672-0208a0d5af70.jpeg#averageHue=%23d5e1cd&clientId=u35b0b29f-c957-4&from=paste&id=u2e49de29&originHeight=366&originWidth=587&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ua907bd67-ed63-4d8b-9874-767dec8aec9&title=HTT%2F1%20~%20HTTP%2F2 "HTT/1 ~ HTTP/2")<br />那 HTTP/2 相比 HTTP/1.1 性能上的改进：
<a name="YlNNU"></a>
#### 1、头部压缩
HTTP/2 会**压缩头**（Header）如果同时发出多个请求，他们的头是一样的或是相似的，那么，协议会**消除重复的部分**。<br />这就是所谓的 HPACK 算法：在客户端和服务器同时维护一张头信息表，所有字段都会存入这个表，生成一个索引号，以后就不发送同样字段了，只发送索引号，这样就**提高速度**了。
<a name="ZXeg5"></a>
#### 2、二进制格式
HTTP/2 不再像 HTTP/1.1 里的纯文本形式的报文，而是全面采用了**二进制格式**，头信息和数据体都是二进制，并且统称为帧（frame）：**头信息帧（Headers Frame）和数据帧（Data Frame）**。<br />![HTTP/1 与 HTTP/2](https://cdn.nlark.com/yuque/0/2022/png/396745/1651194977926-caab13c5-b981-41ae-a5b4-c23d07c61b6c.png#averageHue=%23e3ded9&clientId=u35b0b29f-c957-4&from=paste&id=u247c8541&originHeight=548&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ubdf14243-e3c4-4bf9-9e6d-7479b0316e1&title=HTTP%2F1%20%E4%B8%8E%20HTTP%2F2 "HTTP/1 与 HTTP/2")<br />这样虽然对人不友好，但是对计算机非常友好，因为计算机只懂二进制，那么收到报文后，无需再将明文的报文转成二进制，而是直接解析二进制报文，这**增加了数据传输的效率**。<br />比如状态码 200 ，在 HTTP/1.1 是用 '2''0''0' 三个字符来表示（二进制：110010 110000 110000），如图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651194978005-687b69e6-1e8b-4e72-a32a-c6883da2bff2.png#averageHue=%23e3e2e2&clientId=u35b0b29f-c957-4&from=paste&id=ue9579ec7&originHeight=645&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u649ae07f-93e7-42ff-8661-fff3fc0c6ea&title=)<br />在 HTTP/2 是用数字 200 表示（二进制：11001000），如图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651194978073-ee2da891-5e2a-436d-a48e-e08ab3c79802.png#averageHue=%23e2decb&clientId=u35b0b29f-c957-4&from=paste&id=uf56dadee&originHeight=587&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u01d51713-9c73-4590-a64f-a64a6007e9e&title=)
<a name="Oo0gf"></a>
#### 3、数据流
HTTP/2 的数据包不是按顺序发送的，同一个连接里面连续的数据包，可能属于不同的回应。因此，必须要对数据包做标记，指出它属于哪个回应。<br />在 HTTP/2 中每个请求或相应的所有数据包，称为一个数据流（Stream）。每个数据流都标记着一个独一无二的编号（Stream ID），**不同 Stream 的帧是可以乱序发送的（因此可以并发不同的 Stream ）**，因为每个帧的头部会携带 Stream ID 信息，所以接收端可以通过 Stream ID 有序组装成 HTTP 消息<br />客户端和服务器**双方都可以建立 Stream**， Stream ID 也是有区别的，客户端建立的 Stream 必须是奇数号，而服务器建立的 Stream 必须是偶数号。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651194978144-088b03a7-5a98-491c-bad8-cd1ceab43e86.png#averageHue=%23e9e8e8&clientId=u35b0b29f-c957-4&from=paste&id=u9135edb2&originHeight=436&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud9587daf-cc73-4e58-828a-349b1544c45&title=)<br />客户端还可以**指定数据流的优先级**。优先级高的请求，服务器就先响应该请求。
<a name="kJlg7"></a>
#### 4、多路复用
HTTP/2 是可以在**一个连接中并发多个请求或回应，而不用按照顺序一一对应**。<br />移除了 HTTP/1.1 中的串行请求，不需要排队等待，也就不会再出现「队头阻塞」问题，**降低了延迟，大幅度提高了连接的利用率**。<br />举例来说，在一个 TCP 连接里，服务器收到了客户端 A 和 B 的两个请求，如果发现 A 处理过程非常耗时，于是就回应 A 请求已经处理好的部分，接着回应 B 请求，完成后，再回应 A 请求剩下的部分。<br />![多路复用](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1651194978378-e75c9543-ef86-4c2f-827e-d042111d8a8a.jpeg#averageHue=%23f8f8f7&clientId=u35b0b29f-c957-4&from=paste&id=u7c89de63&originHeight=381&originWidth=371&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ud5b6eac1-da98-4281-b060-03dec1800bd&title=%E5%A4%9A%E8%B7%AF%E5%A4%8D%E7%94%A8 "多路复用")
<a name="kWAwc"></a>
#### 5、服务器推送
HTTP/2 还在一定程度上改善了传统的「请求 - 应答」工作模式，服务不再是被动地响应，也可以**主动**向客户端发送消息。<br />比如，客户端通过 HTTP/1.1 请求从服务器那获取到了 HTML 文件，而 HTML 可能还需要依赖 CSS 来渲染页面，这时客户端还要再发起获取 CSS 文件的请求，需要两次消息往返，如下图左边部分：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651194978576-bee02c90-8dce-461e-a5a2-b8e4148a8d25.png#averageHue=%23f7f7f7&clientId=u35b0b29f-c957-4&from=paste&id=u1131eb6b&originHeight=402&originWidth=800&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u51006f87-b109-4d96-8307-6977b785bfc&title=)<br />如上图右边部分，在 HTTP/2 中，客户端在访问 HTML 时，服务器可以直接主动推送 CSS 文件，减少了消息传递的次数。
<a name="KbHJf"></a>
### HTTP/2 有什么缺陷？
HTTP/2 通过 Stream 的并发能力，解决了 HTTP/1 队头阻塞的问题，看似很完美了，但是 HTTP/2 还是存在“队头阻塞”的问题，只不过问题不是在 HTTP 这一层面，而是在 TCP 这一层。<br />**HTTP/2 是基于 TCP 协议来传输数据的，TCP 是字节流协议，TCP 层必须保证收到的字节数据是完整且连续的，这样内核才会将缓冲区里的数据返回给 HTTP 应用，那么当「前 1 个字节数据」没有到达时，后收到的字节数据只能存放在内核缓冲区里，只有等到这 1 个字节数据到达时，HTTP/2 应用层才能从内核中拿到数据，这就是 HTTP/2 队头阻塞问题。**<br />举个例子，如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1651194978675-c290a9f6-5c45-4cc8-906b-e50b7e90e5db.jpeg#averageHue=%23f5f4f2&clientId=u35b0b29f-c957-4&from=paste&id=u07b0a091&originHeight=502&originWidth=521&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8fc91909-23fd-4c47-ab7c-1d734eac9ef&title=)<br />图中发送方发送了很多个 packet，每个 packet 都有自己的序号，可以认为是 TCP 的序列号，其中 packet 3 在网络中丢失了，即使 packet 4-6 被接收方收到后，由于内核中的 TCP 数据不是连续的，于是接收方的应用层就无法从内核中读取到，只有等到 packet 3 重传后，接收方的应用层才可以从内核中读取到数据，这就是 HTTP/2 的队头阻塞问题，是在 TCP 层面发生的。<br />所以，一旦发生了丢包现象，就会触发 TCP 的重传机制，这样在一个 TCP 连接中的**所有的 HTTP 请求都必须等待这个丢了的包被重传回来**。
<a name="ItO25"></a>
### HTTP/3 做了哪些优化？
前面知道了 HTTP/1.1 和 HTTP/2 都有队头阻塞的问题：

- HTTP/1.1 中的管道（ pipeline）虽然解决了请求的队头阻塞，但是**没有解决响应的队头阻塞**，因为服务端需要按顺序响应收到的请求，如果服务端处理某个请求消耗的时间比较长，那么只能等相应完这个请求后， 才能处理下一个请求，这属于 HTTP 层队头阻塞。
- HTTP/2 虽然通过多个请求复用一个 TCP 连接解决了 HTTP 的队头阻塞 ，但是**一旦发生丢包，就会阻塞住所有的 HTTP 请求**，这属于 TCP 层队头阻塞。

HTTP/2 队头阻塞的问题是因为 TCP，所以 **HTTP/3 把 HTTP 下层的 TCP 协议改成了 UDP！**<br />![HTTP/1 ~ HTTP/3](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1651194978702-e52c00f8-11d5-49e5-9a27-5505d1b1d5ec.jpeg#averageHue=%23d2dec6&clientId=u35b0b29f-c957-4&from=paste&id=u8e706e45&originHeight=366&originWidth=782&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ucd7cc6ef-532f-440a-9db4-6e574e62d12&title=HTTP%2F1%20~%20HTTP%2F3 "HTTP/1 ~ HTTP/3")<br />UDP 发生是不管顺序，也不管丢包的，所以不会出现像 HTTP/2 队头阻塞的问题<br />大家都知道 UDP 是不可靠传输的，但基于 UDP 的 **QUIC 协议** 可以实现类似 TCP 的可靠性传输。<br />QUIC 有以下 3 个特点。
<a name="uK252"></a>
#### 1、无队头阻塞
QUIC 协议也有类似 HTTP/2 Stream 与多路复用的概念，也是可以在同一条连接上并发传输多个 Stream，Stream 可以认为就是一条 HTTP 请求。<br />QUIC 有自己的一套机制可以保证传输的可靠性的。**当某个流发生丢包时，只会阻塞这个流，其他流不会受到影响，因此不存在队头阻塞问题**。这与 HTTP/2 不同，HTTP/2 只要某个流中的数据包丢失了，其他流也会因此受影响。<br />所以，QUIC 连接上的多个 Stream 之间并没有依赖，都是独立的，某个流发生丢包了，只会影响该流，其他流不受影响。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651194978768-3d3f412d-80e7-4af8-a076-d25a46348e52.png#clientId=u35b0b29f-c957-4&from=paste&id=u1836d02b&originHeight=504&originWidth=700&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u298111c0-fa1f-4268-af13-c61fe0b4ebe&title=)
<a name="Hed5R"></a>
#### 2、更快的连接建立
对于 HTTP/1 和 HTTP/2 协议，TCP 和 TLS 是分层的，分别属于内核实现的传输层、openssl 库实现的表示层，因此它们难以合并在一起，需要分批次来握手，先 TCP 握手，再 TLS 握手。<br />HTTP/3 在传输数据前虽然需要 QUIC 协议握手，这个握手过程只需要 1 RTT，握手的目的是为确认双方的「连接 ID」，连接迁移就是基于连接 ID 实现的。<br />但是 HTTP/3 的 QUIC 协议并不是与 TLS 分层，而是QUIC 内部包含了 TLS，它在自己的帧会携带 TLS 里的“记录”，再加上 QUIC 使用的是 TLS/1.3，因此仅需 1 个 RTT 就可以「同时」完成建立连接与密钥协商，如下图：<br />![TCP HTTPS（TLS/1.3） 和 QUIC HTTPS](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1651194978953-a0090ed9-038b-4510-ac2b-f996c0e6311f.jpeg#clientId=u35b0b29f-c957-4&from=paste&id=u2c2df915&originHeight=492&originWidth=742&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u09aa012e-7299-4318-9c76-e603c6d4d81&title=TCP%20HTTPS%EF%BC%88TLS%2F1.3%EF%BC%89%20%E5%92%8C%20QUIC%20HTTPS "TCP HTTPS（TLS/1.3） 和 QUIC HTTPS")<br />甚至，在第二次连接的时候，应用数据包可以和 QUIC 握手信息（连接信息 + TLS 信息）一起发送，达到 0-RTT 的效果。
<a name="Mgrnu"></a>
#### 3、连接迁移
基于 TCP 传输协议的 HTTP 协议，由于是通过四元组（源 IP、源端口、目的 IP、目的端口）确定一条 TCP 连接，那么**当移动设备的网络从 4G 切换到 WIFI 时，意味着 IP 地址变化了，那么就必须要断开连接，然后重新建立连接**。而建立连接的过程包含 TCP 三次握手和 TLS 四次握手的时延，以及 TCP 慢启动的减速过程，给用户的感觉就是网络突然卡顿了一下，因此连接的迁移成本是很高的。<br />而 QUIC 协议没有用四元组的方式来“绑定”连接，而是通过**连接 ID**来标记通信的两个端点，客户端和服务器可以各自选择一组 ID 来标记自己，因此即使移动设备的网络变化后，导致 IP 地址变化了，只要仍保有上下文信息（比如连接 ID、TLS 密钥等），就可以“无缝”地复用原连接，消除重连的成本，没有丝毫卡顿感，达到了**连接迁移**的功能。<br />所以， QUIC 是一个在 UDP 之上的**伪** TCP + TLS + HTTP/2 的多路复用的协议。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651194979108-b2e972d3-b130-4b51-95b8-f07143505a52.png#clientId=u35b0b29f-c957-4&from=paste&id=u42c14406&originHeight=327&originWidth=700&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3cecbb8e-463e-469e-a204-16455b1f01c&title=)<br />QUIC 是新协议，对于很多网络设备，根本不知道什么是 QUIC，只会当做 UDP，这样会出现新的问题，因为有的网络设备是会丢掉 UDP 包的，而 QUIC 是基于UDP 实现的，那么如果网络设备无法识别这个是 QUIC 包，那么就会当作 UDP包，然后被丢弃。<br />所以，HTTP/3 现在普及的进度非常的缓慢，不知道未来 UDP 是否能够逆袭 TCP。

---

参考资料：<br />[1] 上野 宣.图解HTTP.人民邮电出版社.<br />[2] 罗剑锋.透视HTTP协议.极客时间.<br />[3] 陈皓.HTTP的前世今.酷壳CoolShell.https://coolshell.cn/articles/19840.html<br />[4] 阮一峰.HTTP 协议入门.阮一峰的网络日志.http://www.ruanyifeng.com/blog/2016/08/http.html

---

<a name="fCvgD"></a>
## 问答
<a name="LKTDw"></a>
### 问：“https和http相比，就是传输的内容多了对称加密，可以这么理解吗？”

1. 建立连接时候：https 比 http多了 TLS 的握手过程；
2. 传输内容的时候：https 会把数据进行加密，通常是对称加密数据；
<a name="G7ZUP"></a>
### 问：“ 文中 TLS 和 SSL 没有做区分，这两个需要区分吗？”
这两实际上是一个东西。<br />SSL 是洋文 “_Secure Sockets Layer_ 的缩写，中文叫做「安全套接层」。它是在上世纪 90 年代中期，由网景公司设计的。<br />到了1999年，SSL 因为应用广泛，已经成为互联网上的事实标准。IETF 就在那年把 SSL 标准化。标准化之后的名称改为 TLS（是 “_Transport Layer Security_” 的缩写），中文叫做 「传输层安全协议」。<br />很多相关的文章都把这两者并列称呼（SSL/TLS），因为这两者可以视作同一个东西的不同阶段。
<a name="dlJIL"></a>
### 问：“为啥 ssl 的握手是 4 次？”
SSL/TLS 1.2 需要 4 握手，需要 2 个 RTT 的时延，文中的图是把每个交互分开画了，实际上把他们合在一起发送，就是 4 次握手：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651194979152-9c12a425-877e-4c96-84c6-a90ea5336159.png#clientId=u35b0b29f-c957-4&from=paste&id=ue45d27b3&originHeight=304&originWidth=489&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3c4216ab-60c6-47c4-9521-16c3e3a543a&title=)<br />另外， SSL/TLS 1.3 优化了过程，只需要 1 个 RTT 往返时延，也就是只需要 3 次握手：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651194979507-2ae8d203-deca-4b5f-b90c-3f9ea0f91f9b.png#averageHue=%23fafaf9&clientId=u35b0b29f-c957-4&from=paste&id=u58a73af1&originHeight=346&originWidth=544&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucafa969c-8e56-4ce3-a6df-c7b3fb9be9b&title=)
