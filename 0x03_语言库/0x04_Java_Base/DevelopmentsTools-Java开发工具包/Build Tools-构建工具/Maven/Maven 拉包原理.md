JavaMaven<br />业务需求开发的时候，总是会遇到拉不到依赖包的情况。此时如果不清楚 Maven 拉取依赖包的原理，那么很可能找不到问题所在。来了解下 Maven 拉包的原理，在遇到问题的时候能快速解决！
<a name="xJUrK"></a>
## 三种仓库
在 Maven 中，仓库指的是存放代码构建的一个位置。从分类上来说，Maven 仓库有两种类型，分别是：

- 本地仓库（Local）
- 远程仓库（Remote）

本地仓库，是存在于本地的一个仓库，它用来缓存下载的依赖包。这样就不用每次都通过网络去拉取依赖包了，提高了依赖拉取速度，减轻了仓库服务器的压力。<br />远程仓库，如其名字所述，其实位于远程服务器的一个仓库。例如有些公司自己对外开放了一些 API，需要将这些  API 的依赖开放出去，这时候就可以将 API 的 Jar 包放到公司自己的远程仓库中。<br />公司的客户可以通过连接该仓库下载 Jar 包。例如你们公司开发了自己的基础工具类，并将其打成了一个 Jar 包。此时可以将该 Jar 包部署到公司自己的远程仓库中，公司其他开发伙伴配置该远程仓库，从而可以拉取到该 Jar 包依赖。<br />**在网上其他资料中，还会提到 Maven 仓库有另一个类型 —— 中央仓库（Central）。中央仓库其实是一个特殊的远程仓库。** <br />它的特殊之处在于，它是 Maven 官方提供的，其中包括了大量常用的库，基本上大多数的依赖包都可以在这里找到。另外一个特殊之处在于，中央仓库的地址是内置在 Maven 源码中的，即默认会向中央仓库拉取依赖，这个在后续的依赖搜索顺序中会讲到。<br />而经常说的私服，其实也是一个特殊的远程仓库，其特殊之处在于：它只对公司内部开放，方便存放一些本团队创建的开发库。经常说的阿里云 Maven 库，其实就是一个远程仓库，只不过其是对所有人开放罢了。
<a name="tPpBc"></a>
## 依赖搜索顺序
弄懂了仓库的类型，继续来了解下依赖的搜索顺序。<br />在开发过程中找不到依赖包，有多种原因，例如：

- 依赖包确实没有放到远程仓库
- 仓库配置有问题
- 等等

为了弄清楚到底是什么原因导致依赖找不到，需要了解 Maven 是如何找一个依赖的。简单来说，Maven 搜索依赖的算法如下：

1. 首先，在本地仓库搜索，如果找不到则继续下一步。
2. 接着，在中央仓库搜索，如果找不到则继续下一步。
3. 最后，在远程仓库中搜索，如果找不到则抛出错误。如果没有设置远程仓库，那么抛出错误。如果找到了依赖，那么就下载到本地仓库缓存。

简单地说，Maven 搜索遵循简单的顺序 —— 本地仓库 -> 中央仓库 -> 远程仓库。弄明白了这个依赖搜索顺序，可以更好地排查问题。但这可能还不够，还需要搞清楚 Maven  setting.xml 中的一些配置信息，从而可以排查是否是配置出现了问题。例如：有时候配置了 mirror 镜像，会把对某个仓库的请求转发到另一个仓库，这时候要是不懂 mirror 配置，就找不到问题所在了。
<a name="cmf4s"></a>
## 配置信息
关于仓库的一些常见的配置项有：

- repositories
- mirror
- server
- 等等

下面将一个个讲解他们的作用。
<a name="dp9Lh"></a>
### repositories 标签
repositories 标签用于定义远程仓库，其配置范例如下所示：
```xml
<repositories>
  <repository>
    <!--公司镜像的唯一标识，这个配置要注意，不能与mirrorOf配置的相同，不然会被拦截，重定向到外网的镜像仓库 -->
    <id>nexus</id>
    <!--仓库描述，随意写 -->
    <name>xxxx</name>
    <!-- 公司私有仓库地址，这个很重要不能错-->
    <url>http://xxx:8081/nexus/content/groups/public</url>
    <!-- 是否开启 releases 包的下载及更新策略 -->
    <releases>
      <enabled>true</enabled>
      <updatePolicy>daily</updatePolicy>
    </releases>
    <!-- 是否开启 snapshots 包的下载及更新策略 -->
    <snapshots>
      <enabled>false</enabled>
      <checksumPolicy>warn</checksumPolicy>
    </snapshots>
    <layout>default</layout>
  </repository>
</repositories>
```
repositories 标签既可以在 setting.xml 文件配置，也可以在 pom.xml 文件中配置。Maven 在拉取依赖的时候，是按照声明的顺序从上到下去对应的远程仓库拉取依赖的。
<a name="HHB7v"></a>
### mirror 标签
mirror 标签用于定义仓库镜像，其相当于一个拦截器。当 mirror 的 mirrorOf 值与 repository 的 id 相同时，repository 定义的仓库会被拦截，转而使用 mirror 中定义的仓库地址。配置范例如下：
```xml
<!--使用xx公司私有仓库替换Maven默认的中央仓库 -->
<mirrors>
  <mirror>
    <!--自己公司的镜像的唯一标识,在mirror标签中，其实没啥用：如xiaoyaziyun -->
    <id>xiaoyaziyun</id>   
    <!--仓库描述，随意写 -->
    <name>xx公司私有仓库地址</name> 
    <!--xx公司私有仓库地址，这个很重要不能错-->
    <url>http://maven.aliyun.com/nexus/content/groups/public/</url>
    <!--`central`为Maven中央仓库的标识，替换Maven源码内默认的是中央仓库地址-->
    <mirrorOf>central</mirrorOf>
  </mirror>
</mirrors>
```
如上图配置所示，Maven 会用 http://maven.aliyun.com/nexus/content/groups/public/ 这个仓库镜像替换 Maven 中央仓库，其中 central 是 Maven 中央仓库的 ID 标识。经常说用阿里云的 Maven 仓库可以提速，其实就是使用这种方法实现的。
<a name="CUZqE"></a>
### server 标签
大部分远程仓库无须认证就可以访问，但自己搭建的 Maven 仓库，处于安全方面的考虑，会设置访问权限。此时，需要在 setting.xml 文件中配置 server 标签。配置示例如下代码所示：
```xml
<settings>
  <!--配置远程仓库认证信息-->
  <servers>
    <server>
      <id>shuyi-tech-repo</id>
      <username>admin</username>
      <password>admin123</password>
    </server>
  </servers>
</settings>
```
上面的配置为 repository id 为 shuyi-tech-repo 的远程仓库配置了用户名和密码，其中用户名为 admin，密码为 admin123。这里通过 server.id 与 reposiroty.id 标签将认证信息与仓库绑定在一起，因此在配置的时候需要保持这两个信息一致，否则可能导致访问失败。
<a name="WGGI2"></a>
### 其他标签

- proxy 标签：服务器不能直接访问外网时需要设置的代理服务，不常用。
- profiles 标签：上面提到 mirror，配置多个只会一个有效，如果需要多仓库配置，可以在 profiles 节点下配置多个 profile，但是配置需要激活，特别麻烦，也不常用。
<a name="wluTo"></a>
## 配置文件优先级
对于 Maven 来说，会看到有多个配置文件：

- 项目 pom.xml 文件
- .m2/settings.xml 文件
- Maven 安装目录 /conf/settings.xml 文件

如果一个配置同时存在于多个位置，那么到底以哪个为准呢？简单地说，这几个配置文件的优先级是怎样的呢？如果没搞懂这个问题，那么排查问题的时候可能会一团糟。<br />其实三者的级先是  pom.xml >/用户/.m2/settings.xml>/maven安装目录/conf/settings.xml。如果要设置全局 Maven 仓库配置，需要在 Maven 安装目录 /conf 下找到 settings.xml 来修改。
