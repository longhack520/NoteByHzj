Linux SSH<br />SSH（Secure Shell，安全外壳）是一种网络安全协议，通过加密和认证机制实现安全的访问和文件传输等业务。传统远程登录或文件传输方式，例如Telnet、FTP，使用明文传输数据，存在很多的安全隐患。随着人们对网络安全的重视，这些方式已经慢慢不被接受。SSH协议通过对网络数据进行加密和验证，在不安全的网络环境中提供了安全的登录和其他安全网络服务。作为Telnet和其他不安全远程shell协议的安全替代方案，目前SSH协议已经被全世界广泛使用，大多数设备都支持SSH功能。
<a name="cUbX5"></a>
## SSH端口号是什么？
当SSH应用于STelnet，SFTP以及SCP时，使用的默认SSH端口都是22。当SSH应用于NETCONF时，可以指定SSH端口是22或者830。SSH端口支持修改，更改后当前所有的连接都会断开，SSH服务器开始侦听新的端口。
<a name="wFW4z"></a>
## SSH是如何工作的？
SSH由服务器和客户端组成，在整个通信过程中，为建立安全的SSH通道，会经历如下几个阶段：
<a name="pBIHj"></a>
### 连接建立
SSH服务器在指定的端口侦听客户端的连接请求，在客户端向服务器发起连接请求后，双方建立一个TCP连接。
<a name="c9fj4"></a>
### 版本协商
SSH协议目前存在SSH1.X（SSH2.0之前的版本）和SSH2.0版本。SSH2.0协议相比SSH1.X协议来说，在结构上做了扩展，可以支持更多的认证方法和密钥交换方法，同时提高了服务能力。SSH服务器和客户端通过协商确定最终使用的SSH版本号。
<a name="q3sMy"></a>
### 算法协商
SSH支持多种加密算法，双方根据各自支持的算法，协商出最终用于产生会话密钥的密钥交换算法、用于数据信息加密的加密算法、用于进行数字签名和认证的公钥算法以及用于数据完整性保护的HMAC算法。
<a name="uuoYD"></a>
### 密钥交换
服务器和客户端通过密钥交换算法，动态生成共享的会话密钥和会话ID，建立加密通道。会话密钥主要用于后续数据传输的加密，会话ID用于在认证过程中标识该SSH连接。
<a name="WtKk5"></a>
### 用户认证
SSH客户端向服务器端发起认证请求，服务器端对客户端进行认证。SSH支持以下几种认证方式：

- 密码（password）认证：客户端通过用户名和密码的方式进行认证，将加密后的用户名和密码发送给服务器，服务器解密后与本地保存的用户名和密码进行对比，并向客户端返回认证成功或失败的消息。
- 密钥（publickey）认证：客户端通过用户名，公钥以及公钥算法等信息来与服务器进行认证。
- password-publickey认证：指用户需要同时满足密码认证和密钥认证才能登录。
- all认证：只要满足密码认证和密钥认证其中一种即可。
<a name="H8FHw"></a>
### 会话请求
认证通过后，SSH客户端向服务器端发送会话请求，请求服务器提供某种类型的服务，即请求与服务器建立相应的会话。
<a name="FaJVD"></a>
### 会话交互
会话建立后，SSH服务器端和客户端在该会话上进行数据信息的交互。
<a name="ZF3NO"></a>
## 使用PuTTY和OpenSSH
PuTTY是Windows上经典的免费SSH连接工具，通常用于使用SSH协议远程登录设备，最新版本可以在PuTTY官网下载。<br />OpenSSH是SSH协议的开源实现，支持在Unix操作系统上运行，最新版本可以在OpenSSH官网下载。目前Windows10已经包含OpenSSH客户端和服务器软件，可在“设置—应用—应用与功能—可选功能”中搜索安装。
<a name="zknAV"></a>
## SSH密钥
<a name="jHN7Q"></a>
### 对称加密和非对称加密
提高安全性的基本方式就是加密，加密算法通过密钥将明文转换为密文进行安全传输。SSH在工作过程中结合使用了对称加密和非对称加密两种类型的算法，通过事先生成的SSH密钥来保证信息传输的安全性。两种加密算法的加解密过程见下图。<br />![对称加密算法](https://cdn.nlark.com/yuque/0/2022/png/396745/1650009725178-6a689473-bea0-4f97-9d1d-63373342b187.png#clientId=u62536a31-9f63-4&from=paste&id=uc4ad3d80&originHeight=209&originWidth=375&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ude4e66e2-c714-4ea7-bb79-25a46babcc3&title=%E5%AF%B9%E7%A7%B0%E5%8A%A0%E5%AF%86%E7%AE%97%E6%B3%95 "对称加密算法")<br />![非对称加密算法](https://cdn.nlark.com/yuque/0/2022/png/396745/1650009725068-df1d5899-ddfb-4afa-854d-1585dc3bf0ac.png#clientId=u62536a31-9f63-4&from=paste&id=u4bab0765&originHeight=190&originWidth=375&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u96a1cf88-d441-47fc-9812-ac8af3de3df&title=%E9%9D%9E%E5%AF%B9%E7%A7%B0%E5%8A%A0%E5%AF%86%E7%AE%97%E6%B3%95 "非对称加密算法")<br />对称加密算法使用同一个密钥对数据进行加密和解密。SSH连接建立过程中生成的会话密钥就是对称密钥，该对称密钥是由客户端和服务器端基于共享的部分信息和各自的私有数据使用密钥交换算法分别生成的。因为对称加密算法加解密的速度很快，所以适用于传输大量数据的场景。<br />非对称加密的发送和接收需要使用一对关联的SSH密钥，公钥和私钥。私钥由生成的一方自己保管，公钥可以发送给任何请求通信的其他人。发送方用收到的公钥对自己的通信内容进行加密，只有接收方可以使用私钥进行解密获取通信内容。非对称加密的私钥不需要暴露在网络中，安全性大大增加，但是加解密的速度比对称密钥慢得多。<br />SSH连接过程中的两个阶段使用了非对称加密。一个是在密钥交换阶段，服务器和客户端都生成了自己临时的公钥和私钥，用于计算出同一个用于后续加密通信内容的会话密钥。另外一个就是在用户认证阶段，利用只有匹配的私钥可以唯一解密公钥加密的内容这一特点，通过客户端的公钥私钥对验证客户端的身份。
<a name="Argxv"></a>
### 密钥认证
SSH用户认证最基本的两种方式是密码认证和密钥认证。密码认证是将自己的用户名和密码发送给服务器进行认证，这种方式比较简单，且每次登录都需要输入用户名和密码。密钥认证使用公钥私钥对进行身份验证，实现安全的免密登录，是一种广泛使用且推荐的登录方式。密钥认证的基本原理是服务器端使用客户端的公钥对随机内容加密，客户端使用自己的私钥解密并发送给服务器以证实自己的身份，具体的过程见下图。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650009725126-21cec2c7-a296-4d2e-82ea-0c8f53705016.png#clientId=u62536a31-9f63-4&from=paste&id=ucd314f4a&originHeight=283&originWidth=474&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4353d234-2528-4edf-b274-c921cb4554b&title=)
<a name="aTjz7"></a>
### SSH密钥认证登录流程

- 在进行SSH连接之前，SSH客户端需要先生成自己的公钥私钥对，并将自己的公钥存放在SSH服务器上。
- SSH客户端发送登录请求，SSH服务器就会根据请求中的用户名等信息在本地搜索客户端的公钥，并用这个公钥加密一个随机数发送给客户端。
- 客户端使用自己的私钥对返回信息进行解密，并发送给服务器。
- 服务器验证客户端解密的信息是否正确，如果正确则认证通过。
