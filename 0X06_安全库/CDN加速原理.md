网络 CDN
<a name="thE4q"></a>
## 一、什么是CDN
CDN 的全称是(Content Delivery Network)，即内容分发网络。其目的是通过在现有的Internet中增加一层新的CACHE(缓存)层，将网站的内容发布到最接近用户的网络”边缘“的节点，使用户可以就近取得所需的内容，提高用户访问网站的响应速度。从技术上全面解决由于网络带宽小、用户访问量大、网点分布不均等原因，提高用户访问网站的响应速度。<br />简单的说，CDN 的工作原理就是将源站的资源缓存到位于全球各地的 CDN 节点上，用户请求资源时，就近返回节点上缓存的资源，而不需要每个用户的请求都回到源站获取，避免网络拥塞、缓解源站压力，保证用户访问资源的速度和体验。<br />![2021-05-12-15-06-58-985547.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620803291842-d57afa5d-75cc-4d02-ad72-863b0eb15818.jpeg#clientId=ubce6622b-22e3-4&from=ui&id=u63e5e3ec&originHeight=433&originWidth=638&originalType=binary&size=23764&status=done&style=shadow&taskId=u13f7c1f8-3ee0-426f-aa8f-1ddea47cc5e)<br />CDN 对网络的优化作用主要体现在如下几个方面

- 解决服务器端的“第一公里”问题
- 缓解甚至消除了不同运营商之间互联的瓶颈造成的影响
- 减轻了各省的出口带宽压力
- 缓解了骨干网的压力
- 优化了网上热点内容的分布
<a name="H3wQ2"></a>
## 二、CDN工作原理
<a name="AVjZd"></a>
### 传统访问过程
![2021-05-12-15-06-59-059662.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620803300514-3509e082-50be-4856-99f0-0fd63fd2ca24.jpeg#clientId=ubce6622b-22e3-4&from=ui&id=uf4e7258a&originHeight=329&originWidth=618&originalType=binary&size=15627&status=done&style=shadow&taskId=u6ba69825-9f93-41ec-9db8-bd0fabb87d8)<br />传统访问过程<br />由上图可见，用户访问未使用CDN缓存网站的过程为：

1. 用户输入访问的域名，操作系统向 LocalDns 查询域名的 ip 地址
2. LocalDns向 ROOT DNS 查询域名的授权服务器（这里假设LocalDns缓存过期）
3. ROOT DNS将域名授权 dns记录回应给 LocalDns
4. LocalDns 得到域名的授权 dns 记录后,继续向域名授权 dns 查询域名的 ip 地址
5. 域名授权 dns 查询域名记录后，回应给 LocalDns
6. LocalDns 将得到的域名ip地址，回应给 用户端
7. 用户得到域名 ip 地址后，访问站点服务器
8. 站点服务器应答请求，将内容返回给客户端
<a name="k11sh"></a>
### CDN 访问过程
![2021-05-12-15-06-59-205848.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620803310450-1873edb2-13a4-44ca-bf61-a83e8b1a25e7.jpeg#clientId=ubce6622b-22e3-4&from=ui&id=u032e50f5&originHeight=361&originWidth=640&originalType=binary&size=17754&status=done&style=shadow&taskId=u2b7a5a8c-4ce1-4e40-88ce-08bbf309d34)<br />通过上图，可以了解到，使用了CDN缓存后的网站的访问过程变为：

1. 用户输入访问的域名,操作系统向 LocalDns 查询域名的ip地址.
2. LocalDns向 ROOT DNS 查询域名的授权服务器（这里假设LocalDns缓存过期）
3. ROOT DNS将域名授权dns记录回应给 LocalDns
4. LocalDns得到域名的授权dns记录后,继续向域名授权dns查询域名的ip地址
5. 域名授权dns 查询域名记录后（一般是CNAME），回应给 LocalDns
6. LocalDns 得到域名记录后，向智能调度DNS查询域名的ip地址
7. 智能调度DNS 根据一定的算法和策略（比如静态拓扑，容量等），将最适合的CDN节点ip地址回应给 LocalDns
8. LocalDns 将得到的域名ip地址，回应给 用户端
9. 用户得到域名ip地址后，访问站点服务器
10. CDN 节点服务器应答请求，将内容返回给客户端。（缓存服务器一方面在本地进行保存，以备以后使用，二方面把获取的数据返回给客户端，完成数据服务过程）

通过以上的分析可以得到，为了实现对普通用户透明（使用缓存后用户客户端无需进行任何设置）访问，需要使用 DNS（域名解析）来引导用户来访问 Cache 服务器，以实现透明的加速服务。由于用户访问网站的第一步就是域名解析，所以通过修改dns来引导用户访问是最简单有效的方式。
<a name="Tjxat"></a>
### CDN网络的组成要素
对于普通的 Internet 用户，每个 CDN 节点就相当于一个放置在它周围的网站服务器。<br />通过对 DNS 的接管，用户的请求被透明地指向离他最近的节点，节点中 CDN 服务器会像网站的原始服务器一样，响应用户的请求。 由于它离用户更近，因而响应时间必然更快。<br />从上面图中虚线圈起来的那块，就是 CDN 层，这层是位于用户端和站点服务器之间。<br />智能调度 DNS（比如 f5 的 3DNS）

- 智能调度DNS是CDN服务中的关键系统.当用户访问加入CDN服务的网站时，域名解析请求将最终由 “智能调度DNS”负责处理。
- 它通过一组预先定义好的策略，将当时最接近用户的节点地址提供给用户，使用户可以得到快速的服务。
- 同时它需要与分布在各地的CDN节点保持通信，跟踪各节点的健康状态、容量等信息，确保将用户的请求分配到就近可用的节点上.

缓存功能服务

- 负载均衡设备(如lvs,F5的BIG/IP)
- 内容Cache服务器(如squid）
- 共享存储
<a name="Y7hci"></a>
## 三、名词解释
<a name="ybd47"></a>
### CNAME记录（CNAME record）
CNAME即别名( Canonical Name )；可以用来把一个域名解析到另一个域名，当 DNS 系统在查询 CNAME 左面的名称的时候，都会转向 CNAME 右面的名称再进行查询，一直追踪到最后的 PTR 或 A 名称，成功查询后才会做出回应，否则失败。<br />例如，有一台服务器上存放了很多资料，使用docs.example.com去访问这些资源，但又希望通过documents.example.com也能访问到这些资源，那么就可以在DNS解析服务商添加一条CNAME记录，将documents.example.com指向docs.example.com，添加该条CNAME记录后，所有访问documents.example.com的请求都会被转到docs.example.com，获得相同的内容。
<a name="oV7Wv"></a>
### CNAME域名
接入CDN时，在CDN提供商控制台添加完加速域名后，会得到一个CDN分配的CNAME域名， 需要在DNS解析服务商添加CNAME记录，将自己的加速域名指向这个CNAME域名，这样该域名所有的请求才会都将转向CDN的节点，达到加速效果。
<a name="HDCw3"></a>
### DNS
DNS 即 Domain Name System，是域名解析服务的意思。它在互联网的作用是：把域名转换成为网络可以识别的 IP 地址。人们习惯记忆域名，但机器间互相只认IP地址，域名与IP地址之间是一一对应的，它们之间的转换工作称为域名解析，域名解析需要由专门的域名解析服务器来完成，整个过程是自动进行的。比如：上网时输入的www.baidu.com 会自动转换成为 220.181.112.143。<br />常见的DNS解析服务商有：阿里云解析，万网解析，DNSPod，新网解析，Route53（AWS），Dyn，Cloudflare等。
<a name="UWhV1"></a>
### 回源 host
回源host：回源 host 决定回源请求访问到源站上的具体某个站点。
> 例子1：源站是域名源站为www.a.com，回源host为www.b.com,那么实际回源是请求到`www.a.com解析到的IP,对应的主机上的站点www.b.com

> 例子2：源站是IP源站为1.1.1.1, 回源host为www.b.com,那么实际回源的是1.1.1.1对应的主机上的站点www.b.com

<a name="DcZHS"></a>
### 协议回源
指回源时使用的协议和客户端访问资源时的协议保持一致，即如果客户端使用 HTTPS 方式请求资源，当CDN节点上未缓存该资源时，节点会使用相同的 HTTPS 方式回源获取资源；同理如果客户端使用 HTTP 协议的请求，CDN节点回源时也使用HTTP协议。
