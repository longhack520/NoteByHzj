Java
<a name="NC4co"></a>
## 引言
在Java世界中，如果有任何大家都熟悉的文件格式，那就是JAR文件。JAR文件是一个归档文件或一组文件，作为一个具有.JAR文件扩展名的单元分发。归档就像将所有Java和其他资源文件放在一个单独的案例中，并准备将它们作为.jar文件（其他归档扩展名也存在）发布。这里介绍了JAR文件格式背后的概念，并简要介绍了如何在Java中使用.JAR文件。
<a name="QQdL6"></a>
## What is JAR?
JAR代表Java Archive。它是一种独立于平台的文件格式，专门用于将多个文件压缩和捆绑到一个称为JAR文件的归档文件中。所使用的压缩技术基于流行的ZIP文件格式。尽管JAR可以用作通用归档工具，但它的主要开发目的是在单个HTTP事务中下载浏览器中的文件集合。这是Java小程序流行时的场景，类、图像和声音文件作为HTTP请求下载并由浏览器托管。<br />在过去，它对性能产生了更好的影响，网页很快就对下载的小程序做出了响应。由于文件压缩被吸收到JAR文件中，它减少了文件大小，因此下载时间更短。另一个方面是，可以对每个JAR文件进行数字签名，以验证其来源。
<a name="smbjr"></a>
## 使用JAR在Java中归档文件
JAR仍然是一种流行的文件归档格式，至少在Java领域是如此，它被广泛用于许多不同的目的。JAR文件的一些优点包括：

- JAR文件是一种跨平台归档格式
- JAR文件可以归档各种文件类型，无论是类、音频文件、图像还是基于文本的
- JAR文件向后兼容
- 几乎所有的开发人员都喜欢JAR文件，这使得它成为Java世界中大多数涉及归档文件的场景的明显选择

在典型的场景中，用Java开发的应用程序由许多源文件组成。编译后，将为每个公共类或接口创建对象代码或.class文件。这些文件在通过网络传输时，例如在HTTP协议请求中传输，每个文件传输都需要单独的套接字连接，可能非常大。例如，类文件的大小可能只有几百字节。因此，分别为每个文件进行每个套接字连接和断开连接所需的时间只是浪费时间。<br />现在考虑这个场景：所有文件都是JAR归档的，使用PKZIP算法压缩，并作为一个单元分发。此传输的性能将与之前的场景完全不同。这将显著提高应用程序的整体性能，因为JAR文件现在作为单个单元接收，然后可以根据接收端的程序要求将其解压缩为原始形式。这实际上是Java Applet时代存在JAR文件的经典原因。
<a name="aeTui"></a>
## Java应用程序中JAR文件的用例
Java小程序可能已经过时了，但它们的配套库仍然健在。JAR文件就是其中之一。将库打包在JAR归档文件中是很方便的，正如看到的，大多数Java库都是打包在JAR文件中的。开发人员可以通过将所有类文件打包到一个归档文件中，以便于分发，从而制作一个fat-jar。然而，这是不鼓励的。相反，建议将一个更精简、更有凝聚力的文件编译成一个单独的归档文件。这种将文件分离为较小的关注单元的做法不仅利用了存储，而且还利用了库的一些小升级，从而使其他无关文件不受干扰。
<a name="dxKs1"></a>
## 什么是可执行JAR文件
程序员可以将可执行的Java程序与它使用的库、图像和其他文件一起打包到JAR文件中。开发人员可以简单地以单击并运行的方式执行JAR文件。可执行JAR文件保存了一个清单文件，指定了类路径和应用程序的入口点，它只是包含主方法的类：main class:App.MainClass。一些操作系统允许它在单击时运行；其他人使用简单的命令行调用：
```bash
$ java -jar app.jar
```
<a name="QIZVc"></a>
## 如何在Java中创建JAR文件
Java开发工具包（JDK）提供了一个.jar工具，用于将Java代码打包成jar文件格式。从JDK9开始，JAR也得到了增强，可以使用模块，但在本教程中，将重点介绍使用JAR工具的基本功能。注意，一旦安装了JDK，程序员就可以使用JAR文件了。创建JAR文件的基本命令如下：
```bash
$ jar cf jar-file input-file(s)
```
这里参数c表示希望创建一个JAR文件，而`f`表示希望输出到一个文件。现在，假设有三个文件：a.txt、b.class和c.jpg。如果想创建一个名为app的JAR文件，可以使用以下命令：
```bash
jar cf app a.txt b.class c.jpg
```
使用此命令将创建一个演示app的jar文件。请注意，JAR文件实际上可以有任何扩展名，也可以没有扩展名。如果需要一个特定的.jar扩展名，可以按如下方式重写上述命令：
```bash
jar cf app.jar a.txt b.class c.jpg
```
一旦创建了JAR文件，输入文件就会被压缩，并且可以作为一个单元分发：在本例中是app.JAR。
<a name="bSUOB"></a>
## 如何查看JAR内容
现在，在创建JAR文件之后，可能需要查看JAR文件的内容。执行此操作的基本命令如下：
```bash
$ jar tf app.jar
```
这显示了一个类似于以下内容的列表，取决于JAR中的文件名：
```bash
META-INF/
META-INF/MANIFEST.MF
a.txt
b.class
c.jpg
```
注意，除了归档的三个文件外，JAR文件还包含META-INF文件夹/目录中的MANIFEST.MF。这是由jar命令自动生成的。该文件包含一个名称-值对列表，用冒号分隔并分组为部分。<br />如果JAR文件只用于归档，那么这个文件没有多大用处。如果要将应用程序绑定到JAR文件中，则该文件必须包含Java虚拟机（JVM）运行程序的入口点。<br />入口点指的是包含主方法的类。用于下载的JAR文件包含文件列表及其类路径信息。创建的JAR文件非常基本，仅包含以下信息。MANIFEST.MF文件是一个简单的文本文件，可以由任何文本编辑器打开：
```
Manifest-Version: 1.0
Created-By: 19.0.1 (Oracle Corporation)
```
<a name="PbHGP"></a>
## 如何提取JAR文件
可以使用以下命令提取JAR文件：
```bash
$ jar xf app.jar
```
当提取JAR工具时，它会在当前目录中创建文件的副本；原始JAR文件保持不变。提取将覆盖当前目录和路径名中具有相同名称的所有文件。
<a name="r7yst"></a>
## 如何更新JAR文件
开发人员可以使用以下命令更新或向现有JAR文件添加新文件：
```bash
$ jar uf app.jar d.class
```
在将新文件添加到现有存档中时应小心，因为存档中具有相同名称的任何文件都将被自动覆盖。
<a name="QckzE"></a>
## 关于使用JavaJAR归档文件的最后思考
使用JAR工具时有很多可用选项。一个简单的`jar –help`命令可以快速概述这些参数。作为一名Java程序员，不可能没有直接或间接处理过JAR工具。<br />还有另一种名为WAR（Web Archive）的文件格式，用于捆绑Java Web应用程序和EAR（Enterprise Archive），用于归档由多个模块组成的企业应用程序。这些是JAR格式的特殊扩展，但与JAR不同，EAR和WAR文件不能作为独立应用程序运行。
