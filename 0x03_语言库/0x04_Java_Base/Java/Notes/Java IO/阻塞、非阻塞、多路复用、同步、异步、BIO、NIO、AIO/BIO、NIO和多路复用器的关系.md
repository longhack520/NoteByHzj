Java BIO NIO
<a name="kUH5k"></a>
### BIO
BIO即为阻塞IO的意思，通常讲BIO的时候都会和服务器模型配合着讲，在实际应用中讲会更好理解。看下面的代码，初学java网络编程的时候用的都是这个模型：
```java
public static void main(String[] args) throws Exception {
    //建立socket，socket是客户端和服务器沟通的桥梁
    ServerSocket server = new ServerSocket(9090,20);
    
    //通过死循环不断接收客户端请求
    while (true) {
        //线程会阻塞在这行的accep方法
        Socket client = server.accept(); 
        //创建新线程处理新客户端的逻辑
        new Thread(() -> {
                
            //client的读写逻辑

        }).start();
    }
}
```
只要没有客户端连接上服务器，`accept`方法就一直不能返回，这就是阻塞；对应的读写操作道理也一样，想要读取数据，必须等到有数据到达才能返回，这就是阻塞。<br />还可以站在阻塞的基础上思考一下，为什么服务器的模型要设计成来一个客户端就新建一个线程？<br />其实答案很简单，当来了一个客户端创建连接后，如果不给客户端新分配一个线程执行服务器逻辑，那么服务端将很难再和第二个客户端建立连接。<br />就算把客户端连接用集合保存起来，通过单线程遍历集合的方式去执行服务器端逻辑也是不行的。因为如果某个客户端连接因为读写操作阻塞了，那么其他客户端将得不到执行。
<a name="TZ9x5"></a>
### NIO
如果说服务器只有很少的人用，那么上面那段bio的代码其实挺好的，但问题在于互联网蓬勃发展，随着服务器访问人数的增加，这样的服务器模型将会成为瓶颈。<br />以一种C10K的思想去看待上面这段服务器代码。如果客户端的连接数增加了10K倍，那么就意味着要创建10k个线程，单单创建线程就是一项不小的开销了，再加上线程之间要来回切换，单机服务器根本就扛不住这么大的连接数。<br />那既然瓶颈是出在线程上，考虑能不能把服务器的模型变为单线程模型，思路其实和之前说的差不多，用集合保存每个连接的客户端，通过while循环来对每个连接进行操作。<br />之前说了这样的操作瓶颈在于`accept`客户端的时候会阻塞，以及进行读写操作的时候会阻塞，导致单线程执行效率低。为了突破这个瓶颈，操作系统发展出了nio，这里的nio指的是非阻塞io。<br />也就是说在`accept`客户端连接的时候，不需要阻塞，如果没有客户端连接就返回-1（java-NULL），在读写操作的时候，也不阻塞，有数据就读，没数据就直接返回，这样就解决了单线程服务器的瓶颈问题。示例代码如下：
```java

public static void main(String[] args) throws Exception {
    //用于存储客户端的集合
    LinkedList<SocketChannel> clients = new LinkedList<>();
    //nio里概念改成了channel
    ServerSocketChannel ss = ServerSocketChannel.open();
    ss.bind(new InetSocketAddress(9090));
    //设置成非阻塞
    ss.configureBlocking(false);

    while (true) {
        //下面的accept方法不会阻塞
        SocketChannel client = ss.accept();

        if (client == null) {
            System.out.println("null.....");
            
        } else {
            //设置客户端操作也为非阻塞
            client.configureBlocking(false); 
            clients.add(client);
        }

        ByteBuffer buffer = ByteBuffer.allocateDirect(4096);

        //遍历已经链接进来的客户端能不能读写数据
        for (SocketChannel c : clients) {
            int num = c.read(buffer);
            if (num > 0) {
                //其他操作
            }
        }
    }
}
```
<a name="XSpcA"></a>
### 多路复用器
尽管上面的单线程NIO服务器模型比BIO的优良许多，但是仍然有一个大问题。在客户端与服务器建立连接后，后续会进行一系列的读写操作。虽然这些读写操作是非阻塞的，但是每调一次读写操作在操作系统层面都要进行一次用户态和内核态的切换，这个也是一项巨大的开销（读写等系统调用都是在内核态完成的）。<br />在上面的代码中每次循环遍历都进行读写操作，以读操作为例：大部分读操作都是在数据没有准备好的情况下进行读的，相当于执行了一次空操作。要想办法避免这种无效的读取操作，避免内核态和用户态之间的频繁切换。<br />补充：客户端与服务器两端都是通过socket进行连接的，socket在linux操作系统中有对应的文件描述符，读写操作都是以该文件描述符为单位进行操作的。<br />为了避免上述的无效读写，得想办法得知当前的文件描述符是否可读可写。如果逐个文件描述符去询问，那么效率就和直接进行读写操作差不多了，希望有一种方法能够一次性得知哪些文件描述符可读，哪些文件描述符可写，这，就操作系统后来发展出的多路复用器。<br />也就是说，多路复用器的核心功能就是告诉我们哪些文件描述符可读，哪些文件描述符可写。而多路复用器也分为几种，他们也经历了一个演化的过程。最初的多路复用器是select模型，它的模式是这样的：程序端每次把文件描述符集合交给select的系统调用，select遍历每个文件描述符后返回那些可以操作的文件描述符，然后程序对可以操作的文件描述符进行读写。<br />它的弊端是，一次传输的文件描述符集合有限，只能给出1024个文件描述符，poll在此基础上进行了改进，没有了文件描述符数量的限制。<br />但是select和poll在性能上还可以优化，它们共同的弊端在于：

1. 它们需要在内核中对所有传入的文件描述符进行遍历，这也是一项比较耗时的操作
2. （这点是否存在优化空间有待考证）每次要把文件描述符从用户态的内存搬运到内核态的内存，遍历完成后再搬回去，这个来回复制也是一项耗时的操纵。

后来操作系统加入了epoll这个多路复用器，彻底解决了这个问题：<br />epoll多路复用器的模型是这样的：<br />为了在发起系统调用的时候不遍历所有的文件描述符，epoll的优化在于：当数据到达网卡的时候，会触发中断，正常情况下cpu会把相应的数据复制到内存中，和相关的文件描述符进行绑定。epoll在这个基础之上做了延伸，epoll首先是在内核中维护了一个红黑树，以及一些链表结构，当数据到达网卡拷贝到内存时会把相应的文件描述符从红黑树中拷贝到链表中，这样链表存储的就是已经有数据到达的文件描述符，这样当程序调用epoll_wait的时候就能直接把能读的文件描述符返回给应用程序。<br />除了epoll_wait之外，epoll还有两个系统调用，分别是epoll_create和epoll_ctl，分别用于初始化epoll和把文件描述符添加到红黑树中。<br />以上就是多路复用器与常见io模型的关系了，网上常常有文章把多路复用器说成是nio的一部分，也是合理的，因为在具体编程的时候两个概念往往会融为一体。
