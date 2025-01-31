Java
<a name="EOltY"></a>
## 1、类加载
下面说一下在Java中类加载的概念及它在整个Java程序得以运行的过程中所处的位置：<br />![2021-06-01-22-47-22-798311.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622558906599-b0824ca5-3169-479b-b8ec-5da7140492f8.png#clientId=ud0328403-58c7-4&from=ui&id=ufbfdebd8&originHeight=763&originWidth=1042&originalType=binary&size=2389837&status=done&style=shadow&taskId=ubb53104d-b84a-41ea-90f2-acb6dab749e)<br />类的加载指的是将类的字节码文件（.class文件）中数据读入到内存中，将其放在运行时数据区的方法区内，然后在堆区创建一个java.lang.Class对象（关于这部分可以看之前的一篇关于Java反射的内容：入口），用来封装类在方法区内的数据结构。类的加载的最终产品是位于堆区中的Class对象，Class对象封装了类在方法区内的数据结构，并且向Java程序员提供了访问方法区内的数据结构的接口。<br />类加载器并不需要等到某个类被“首次主动使用”时再加载它，JVM规范允许类加载器在预料某个类将要被使用时就预先加载它，如果在预先加载的过程中遇到了.class文件缺失或存在错误，类加载器必须在程序首次主动使用该类时才报告错误（LinkageError错误）如果这个类一直没有被程序主动使用，那么类加载器就不会报告错误。<br />上面的话感觉很懵？没事，我给你翻译翻译，和那些编译时需要进行连接工作的语言不同（那些语言都是完成全部代码的编译连接全部放到内存中才开始运行），在Java里，类的加载、连接和初始化过程都是在程序运行起来以后进行的，或者说是在运行期间完成的（懵逼？没事，先保留困惑，详细的解释会在后面类的加载时机那块做出解释）。它的这种设计，会在类加载时增加一定的性能开销，但是这样是为了满足Java的高度灵活性，Java是天生地可以动态扩展地语言，这一特性就是依赖运行期动态加载和动态连接实现的。
<a name="hARlS"></a>
## 2、类的生命周期
说类加载的过程之前，先来了解一下，类的整个生命周期要经历什么<br />![2021-06-01-22-47-22-877527.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622558920065-c803c701-9b84-4809-872b-d9e216e7bf91.png#clientId=ud0328403-58c7-4&from=ui&id=u6832f870&originHeight=703&originWidth=1080&originalType=binary&size=2282188&status=done&style=shadow&taskId=u6b420d7b-c696-4a84-8609-bd3a01c5078)<br />类从被加载到虚拟机的内存中开始，到卸载出内存（整个程序\系统运行结束虚拟机关闭）为止，它的整个生命周期包括：加载、链接、初始化、使用、卸载。<br />因为这里着重说类的加载这一过程，所以类的使用和卸载就不介绍了，后面就默认类的加载这个过程包含：加载、链接、初始化
<a name="Jzhe5"></a>
### 加载（Load）
这里叫做加载，很容易让人误会，会觉得类的加载就是指这里，其实不是这个样子，这里的加载二字和类的加载不是一回事，可以这么理解，加载是类加载过程的一个阶段，这一阶段，虚拟机主要是做三件事：<br />1、根据类的全路径获取类的二进制字节流<br />2、将这个字节流对应的结构转化为方法区的运行时数据结构（把编码的组织方式变成虚拟机运行时所能解读的结构，存放于方法区）<br />3、在内存中生成一个Class对象（java.lang.Class），由这个对象来关联方法区中的数据<br />这里特别注意一下，以上的三点，只是虚拟机规范定义的，至于具体如何实现，是依赖具体的虚拟机来的；例如，第一件事的获取二进制流，并不一定是从字节码文件（Class文件）从进行获取，它可以是从ZIP中获取，从网络中获取，利用代理在计算过程中生成等等；<br />还有第三件事中生成的Class对象，也并不一定是在堆区的，例如HostSpot虚拟机的实现上，Class对象就是放在方法区的。
<a name="H9Jnp"></a>
### 链接（Link）
链接阶段又细分为验证、准备、解析三个步骤：
<a name="gfpy6"></a>
### 验证
作为链接的第一步，它的职责就是确保Class文件的字节流中包含的信息是符合规定的，并且不会对虚拟机进行破坏；其实说白了就是它主要责任就是保证写的代码是符合Java语法的，是合理可行的。如果不合理，编译器是拒绝的。验证主要是针对 文件格式的验证、元数据的验证，字节码的验证，符号引用的验证；<br />文件格式的验证是对字节流进行是否符合Class文件格式的验证，元数据的验证主要是语义语法的验证，即验是否符合Java语言规范，例如：一个类是否有父类（Java中处理Object，所有的类都应该有个父类），字节码的验证主要是对数据流和控制流进行验证，确保程序语义是合法、合逻辑的，例如：在操作栈先放了一个Int型的数据，后面某个地方使用的时候却用Long型来接它。符号引用的验证是确保解析动作能够正常执行。<br />整个验证过程，保证了Java语言的安全性，不会出现不可控的情况。（这里补充一下，这里说的验证、不可控，包括上面举的例子，并不是编程中写的类似于`a != null`这种，它是在编写的程序更下一层的字节码的解析上来说的），对于加载的过程来说，验证阶段很重要，但并不一定是必须的，因为它对程序运行期并没有影响，仅仅旨在保证语言的安全性，如果所运行的全部代码都已经被反复使用和验证过，那么在实施阶段，可以考虑使用-Xverify:none参数来关闭大部分的验证过程，以达到缩短虚拟机加载的时间。
<a name="DVIKl"></a>
### 准备
准备阶段主要作用是正式为类变量分配内存并设置类变量初始值的阶段，即这些变量所使用的内存，都在方法区中进行分配。这里需要注意，这时候进行内存分配的仅仅是类变量，换句话说也就是静态变量（static修饰的），并不包括实例变量，实例变量会在实例化时分配在堆内存中。初始值也并不是我们的赋值，<br />例如：
```java
public Class A{
  public String name；
  public static int value = 987；
}
```
就像刚刚讲的，这里在准备阶段，只会对value变量进行内存分配，并不会对name进行分配，其次，在准备阶段，对value分配完内存，会同时赋予初始值，但是并不会赋给它987，在准备阶段，value的值是0。而赋值为987的指令，是在程序被编译后，存放于类构造器<clinit>()方法中，所以把value赋值987的操作，会在初始化阶段才会进行。（这里补充个特殊情况，如果写成 `public static final int  value = 987`，那么变量value 在准备阶段就会被赋值为987，这就是为什么很多书在讲final字段的时候说它一般用来定义常量，且一经使用，就不可以被更改的原因）
<a name="wLSM1"></a>
### 解析
解析阶段的任务是将常量池中的符号引用替换为直接引用<br />常量池可以理解为存放代码符号的地方，例如代码中声明的变量，它仅仅是个符号，并不具备实际内存，所有这些符号，都会放在常量池中。例如，一个类的方法为`test()`，则符号引用即为test，这个方法存在于内存中的地址假设为0x123456，则这个地址则为直接引用。<br />符号引用：<br />符号引用更多的是以一组符号来描述所引用的内存目标，符号和内存空间实际并没有关系，引用的目标也不一定在内存里，只是在代码中自己写的时候区分的，例如一句 Persion one；其中one就是个'o'，'n'，'e'三个符号的组合，它啥也不是。<br />直接引用：<br />直接引用可以是直接指向内存空间的指针、相对便宜量或是一个能够简洁定位到内存目标的句柄。<br />解析动作主要是针对 类、接口、字段、类方法、方法类型、方法句柄和调用点限定符号的引用进行。
<a name="NDTOk"></a>
### 初始化（Initialize）
在类的加载过程中，加载、连接完全由虚拟机来主导和控制，到了初始化这一阶段，才是真正开始执行类中定义的Java代码。初始化其实就是该阶段是为类的类变量初始化值的，在准备阶段变量已经进了一次赋值，只不过那是系统要求的初始值，而在初始化阶段的赋值，则是根据研发人员编写的主观程序去初始化变量和其他资源。在初始化这步，进行赋值的方式有两种：<br />1、在声明类变量时，直接给变量赋值<br />2、在静态初始化块为类变量赋值
<a name="hInV1"></a>
### 使用
就是对象之间的调用通信等等
<a name="kQFRy"></a>
### 卸载（死亡）
遇到如下几种情况，即类结束生命周期：

- 执行了System.exit()方法
- 程序正常执行结束
- 程序在执行过程中遇到了异常或错误而异常终止
- 由于操作系统出现错误而导致Java虚拟机进程终止
<a name="itZmK"></a>
## 3、类加载器
之前说了那么多一个类的声明周期，更多的是一种理论基础，映射到具体的代码层面，到底是什么来完成类加载这个过程的就是这里要说的——类加载器。<br />虚拟机在设计时，把类加载阶段的 “通过一个类的全路径名来获取该类字节码二进制流” 这个动作放到了 Java虚拟机之外去完成，而负责实现这个动作的模块就叫做类加载器。
<a name="gTB2z"></a>
### 类加载器分类
![2021-05-31-23-29-06-480184.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622475130602-e6423b41-2a25-437e-b330-3a5f4c82dcab.png#clientId=uab56894e-137a-4&from=ui&id=u39791b13&originHeight=781&originWidth=1080&originalType=binary&size=88894&status=done&style=none&taskId=u6a561ed1-f0f1-4a87-af00-1ece229dbb9)
<a name="xjbAr"></a>
### 启动类加载器
1、它用来加载 Java 的核心库(JAVA_HOME/jre/lib/rt.jar,sun.boot.class.path路径下的内容)，并不是Java代码完成，而是用原生代码（C语言或C++）来实现的，并不继承自 java.lang.ClassLoader。<br />2、加载扩展类和应用程序类加载器。并指定他们的父类加载器。
<a name="J49IY"></a>
### 扩展类加载器
这一类加载器由sun.misc.Launcher$ExtClassLoader实现，用来加载 Java 的扩展库(JAVA_HOME/jre/ext/*.jar，或java.ext.dirs路径下的内容) 。Java 虚拟机的实现会提供一个扩展库目录。该类加载器在此目录里面查找并加载 Java类。
<a name="CoT2g"></a>
### 应用类加载器
这个类加载器由sun.misc.Launcher$AppClassLoader实现，由于这个类加载器是ClassLoader中getSystemClassLoader（）方法的返回值，所以它也成为系统类加载器。它负责加载用户类路径下所指定的类库，开发者可以直接使用这个类加载器，如果应用程序中没有自定义过自己的类加载器，一般情况下这个就是系统默认的类加载器。
<a name="FSDBL"></a>
### 自定义加载器
开发人员可以通过继承 java.lang.ClassLoader类的方式实现自己的类加载器，以满足一些特殊的需求。
<a name="JucO9"></a>
### 类加载的代理（双亲委派模式）
![2021-05-31-23-29-06-623800.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622475161415-cdfecc91-e204-40f9-997e-cf8bcf363c32.png#clientId=uab56894e-137a-4&from=ui&id=ua81083cb&originHeight=781&originWidth=1080&originalType=binary&size=88276&status=done&style=shadow&taskId=ud8cd5aa3-fc7b-463c-bff2-252d9db1637)<br />如果一个类加载器收到了类加载器的请求，它首先不会自己去尝试加载这个类，而是把这个请求委派给父加载器去完成。每个层次的类加载器都是如此。因此所有的加载请求最终都会传送到Bootstrap类加载器(启动类加载器)中，只有父类加载反馈自己无法加载这个请求(它的搜索范围中没有找到所需的类)时子加载器才会尝试自己去加载。<br />例如类java.lang.Object,它存放在rt.jart之中，无论哪一个类加载器都要加载这个类.最终都是双亲委派模型最顶端的Bootstrap类加载器去加载，因此Object类在程序的各种类加载器环境中都是同一个类。相反，如果没有使用双亲委派模型.由各个类加载器自行去加载的话，如果用户编写了一个称为“java.lang.Object”的类，并存放在程序的ClassPath中，那系统中将会出现多个不同的Object类.java类型体系中最基础的行为也就无法保证。应用程序也将会一片混乱。<br />当然也并不是所有的加载机制都是双亲委派的方式，例如tomcat作为一个web服务器，它本身实现了类加载，该类加载器也使用代理模式(不同于前面说的双亲委托机制)，所不同的是它是首先尝试去加载某个类，如果找不到再代理给父类加载器。这与一般类加载器的顺序是相反的。但也是为了保证安全，这样核心库就不在查询范围之内。
<a name="vKV0h"></a>
### 类的加载时机
最后说一个比较重要也是诸多困惑的地方，就是什么时候才会加载类。<br />加载、验证、准备、初始化、卸载这五个步骤是确定的，类的加载过程必须按部就班地开始，但是解析阶段就不一定了，它在某些情况下是可以在初始化阶段之后再开始，它这是为了满足Java语言地动态时绑定（泛型、多态的本质）这个特性来的，它是按部就班的开始，而不是按部就班的 “进行”或者“结束”，这些阶段其实是相互交叉混合进行的，通常会在一个阶段执行的过程中调用、激活另外一个阶段。<br />其实上面的话有些绕，从类的使用上来看这个问题，类的使用分为主动引用和被动引用：
<a name="lzSHT"></a>
#### 1、主动引用类（肯定会初始化）

- new一个类的对象。
- 调用类的静态成员(除了final常量)和静态方法。
- 使用java.lang.reflect包的方法对类进行反射调用。
- 当虚拟机启动，java Hello，则一定会初始化Hello类。说白了就是先启动main方法所在的类。
- 当初始化一个类，如果其父类没有被初始化，则先会初始化他的父类
<a name="KpDS4"></a>
#### 2、被动引用

- 当访问一个静态域时，只有真正声明这个域的类才会被初始化。例如：通过子类引用父类的静态变量，不会导致子类初始化。
- 通过数组定义类引用，不会触发此类的初始化。
- 引用常量不会触发此类的初始化（常量在编译阶段就存入调用类的常量池中了）。

首先，Java的编译不是像其他语言一样，都加载到内存中才开始运行，而且动态的，也就会出现：先运行了一部分，初始化了一些类，但是在这一部分运行的代码里被动引用了未被初始化的类（例如static变量），这时候就会出现了这种违背顺序的情况。总的来说就是，

- 先加载并连接当前类
- 父类没有被加载，则去加载、连接、初始化父类，依旧是先加载并连接，然后再判断有无父类，如此循环（所以JVM先将Object加载）
- 如果类中有初始化语句，包括声明时赋值与静态初始化块，则按顺序进行初始化
