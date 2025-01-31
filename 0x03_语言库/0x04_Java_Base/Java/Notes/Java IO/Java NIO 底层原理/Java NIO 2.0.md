Java NIO<br />在早期Java版本当中，输入输出（I/O）流并不那么令开发者感到愉快：<br />1）JDK 1.4 之前的 I/O 没有缓冲区的概念、不支持正则表达式、支持的字符集编码有限等等；<br />2）JDK 1.4 的时候引入了非阻塞 I/O，也就是 NIO 1.0，但遍历目录很困难，不支持文件系统的非阻塞操作等等。<br />为了突破这些限制，JDK 1.7 的时候引入了新的 NIO，也就是本篇文章的主角——NIO 2.0。
<a name="vhpnX"></a>
## 1、基石：Path
Path 既可以表示一个目录，也可以表示一个文件，就像 File 那样——当然了，Path 就是用来取代 File 的。<br />1）可以通过 `Paths.get()` 创建一个 Path 对象，此时 Path 并没有真正在物理磁盘上创建；参数既可以是一个文件名，也可以是一个目录名；绝对路径或者相对路径均可。<br />2）可以通过 `Files.notExists()` 确认 Path（目录或者文件） 是否已经存在。<br />3）可以通过 `Files.createDirectory()` 创建目录，此时目录已经在物理磁盘上创建成功，可通过资源管理器查看到。<br />4）可以通过 `Files.createFile()` 创建文件，此时文件已经在物理磁盘上创建成功，可通过资源管理器查看到。<br />5）可以通过 `toAbsolutePath()` 查看 Path 的绝对路径。<br />6）可以通过 `resolve()` 将 Path 连接起来，参数可以是一个新的 Path 对象，也可以是对应的字符串。<br />具体的代码如下：
```java

public class Wanger {

    public static void main(String[] args) {
        // 相对路径
        Path dir = Paths.get("chenmo");

        // 输出 dir 的绝对路径
        System.out.println(dir.toAbsolutePath()); // 输出：D:\program\java.git\java_demo\chenmo

        if (Files.notExists(dir)) {
            try {
                // 如果目录不存在，则创建目录
                Files.createDirectory(dir);
            } catch (IOException e1) {
                e1.printStackTrace();
            }
        }

        // 这时候 chenmo.txt 文件并未创建
        // 通过 resolve 方法把 dir 和 chenmo.txt 链接起来
        Path file = dir.resolve("chenmo.txt");

        // 输出 file 的绝对路径
        System.out.println(file.toAbsolutePath()); // 输出：D:\program\java.git\java_demo\chenmo\chenmo.txt

        if (Files.notExists(file)) {
            try {
                // 如果文件不存在，则创建文件
                Files.createFile(file);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

    }

}
```
如果要将 File 转换为 Path，可以通过 File 类的 `toPath()` 方法完成。代码示例如下：
```java
File file = new File("1.txt");
Path path = file.toPath();
```
如果要将 Path 转换为 File，可以通过 Path 类的 `toFile()` 方法完成。代码示例如下：
```java
Path path = Paths.get("1.txt");
File file = path.toFile();
```
<a name="prQpn"></a>
## 2、处理目录
NIO 2.0 新增的 `java.nio.file.DirectoryStream<T>` 接口可以非常方便地查找目录中的（符合某种规则的）文件，比如说要查找 chenmo 目录下的 txt 后缀的文件，代码示例如下：
```java
// 相对路径
Path dir = Paths.get("chenmo");

try (DirectoryStream<Path> stream = Files.newDirectoryStream(dir, "*.txt")) {
    for (Path entry : stream) {
        System.out.println(entry.getFileName());
    }
} catch (IOException e) {
    e.printStackTrace();
}
```
1）`Files.newDirectoryStream(Path dir, String glob)` 会返回一个过滤后的 `DirectoryStream`（ 目录流，），第一个参数为目录，第二个参数为 glob 表达式，比如 `*.txt` 表示所有 txt 后缀的文件。<br />2）由于 `DirectoryStream` 继承了 `Closeable` 接口，所以它可以配合 `try-with-resources` 语法写出更安全的代码，目录流会自动调用 close 方法关闭流，释放与流相关的资源，不需要再通过 finally 进行主动关闭。<br />3）`DirectoryStream` 被称为目录流，允许方便地使用 for-each 结构来遍历目录。
<a name="nBm1Y"></a>
## 3、处理目录树
目录树意味着一个目录里既有文件也有子目录，也可能都没有，也可能有其一。NIO 2.0 可以很方便地遍历一颗目录树，并操作符合条件的文件；这其中关键的一个方法就是 Files 类的 walkFileTree，其定义如下：
```java
public static Path walkFileTree(Path start, FileVisitor<? super Path> visitor)
    throws IOException
{
        return walkFileTree(start,
                            EnumSet.noneOf(FileVisitOption.class),
                            Integer.MAX_VALUE,
                            visitor);
    }
```
第二个参数 FileVisitor 被称为文件访问器接口，它实现起来非常复杂，要实现 5 个方法呢，但幸好 JDK 的设计者提供了一个默认的实现类 SimpleFileVisitor，如果只想从目录树中找到 txt 后缀的文件，可以这样做：
```java
// 相对路径
Path dir = Paths.get("chenmo");

try {
  Files.walkFileTree(dir, new SimpleFileVisitor<Path>() {
    @Override
    public FileVisitResult visitFile(Path file, BasicFileAttributes attrs) {
      if (file.toString().endsWith(".txt")) {
        System.out.println(file.getFileName());
      }
      return FileVisitResult.CONTINUE;
    }
  });
} catch (IOException e) {
  e.printStackTrace();
}
```
通过创建匿名内部类来重写 `SimpleFileVisitor` 的 `visitFile` 方法，如果后缀名为 txt 就打印出来。
<a name="iNtM2"></a>
## 4、文件的删除、复制、移动
创建一个文件非常的简单，之前已经体验过了，那么删除一个文件也同样的简单，代码示例如下：
```java
Files.delete(file);
Files.deleteIfExists(file);
```
使用 `Files.delete()` 删除文件之前最好使用 `Files.exists()` 判断文件是否存在，否则会抛出 NoSuchFileException；`Files.deleteIfExists()` 则不用。<br />复制文件也不复杂，代码示例如下：
```java
Path source = Paths.get("1.txt");
Path target = Paths.get("2.txt");
Files.copy(source, target);
```
移动文件和复制文件非常相似，代码示例如下：
```java
Path source = Paths.get("1.txt");
Path target = Paths.get("2.txt");
Files.move(source, target);
```
<a name="X6iJW"></a>
## 5、快速地读写文件
NIO 2.0 提供了带有缓冲区的读写辅助方法，使用起来也非常的简单。可以通过 `Files.newBufferedWriter()` 获取一个文件缓冲输入流，并通过 `write()` 方法写入数据；然后通过 `Files.newBufferedReader()` 获取一个文件缓冲输出流，通过 `readLine()` 方法读出数据。代码示例如下。
```java
Path file = Paths.get("2.txt");

try (BufferedWriter writer = Files.newBufferedWriter(file, StandardCharsets.UTF_8)) {
    writer.write("Hello");
} catch (Exception e) {
    e.printStackTrace();
}

try (BufferedReader reader = Files.newBufferedReader(file, StandardCharsets.UTF_8)) {

    String line;
    while ((line = reader.readLine()) != null) {
        System.out.println(line);
    }
} catch (Exception e) {
    e.printStackTrace();
}
```
<a name="Goep2"></a>
## 6、重要：异步 I/O 操作
上面提到的那些都算是 NIO 2.0 的甜点，而异步 I/O 操作（也称 AIO）才算是真正重要的内容。异步 I/O 操作可以充分利用多核 CPU 的特点，不需要再像以前那样启动一个线程来对 I/O 进行处理，免得阻塞了主线程的其他操作。<br />异步 I/O 操作的核心概念是发起非阻塞方式的 I/O 操作，当 I/O 操作完成时通知。可以分为两种形式：Future 和 Callback。如果希望主线程发起 I/O 操作并轮循等待结果时，一般使用 Future 的形式；而 Callback 的基本思想是主线程派出一个侦查员（CompletionHandler）到独立的线程中执行 I/O 操作，操作完成后，会触发侦查员的 completed 或者 failed 方法。
<a name="qGlB1"></a>
### 1）Future 
先来看一个示例，代码如下：
```java
public static void main(String[] args) throws IOException, InterruptedException, ExecutionException {
    Path file = Paths.get("1.txt");
    AsynchronousFileChannel channel = AsynchronousFileChannel.open(file);
    Future<Integer> result = channel.read(ByteBuffer.allocate(100_000), 0);
    while (!result.isDone()) {
        System.out.println("主线程继续做事情");
    }

    Integer bytesRead = result.get();
    System.out.println(bytesRead);
}
```
1）通过 `AsynchronousFileChannel.open()` 打开一个异步文件通道 channel。<br />2）用 Future 来保存从通道中读取的结果。<br />3）通过 `isDone()` 轮循判断异步 I/O 操作是否完成，如果没有完成的话，主线程可以继续做自己的事情。
<a name="jHfsq"></a>
### 2）Callback
先来看一个示例，代码如下：
```java
public static void main(String[] args) throws IOException, InterruptedException, ExecutionException {
    Path file = Paths.get("1.txt");
    AsynchronousFileChannel channel = AsynchronousFileChannel.open(file);
    channel.read(ByteBuffer.allocate(100_000), 0, null, new CompletionHandler<Integer, ByteBuffer>() {
        public void completed(Integer result, ByteBuffer attachment) {
            System.out.println(result);
        }
        public void failed(Throwable exc, ByteBuffer attachment) {
            System.out.println(exc.getMessage());
        }
    });

    System.out.println("主线程继续做事情");
}
```
1）通过 `AsynchronousFileChannel.open()` 打开一个异步文件通道 `channel`。<br />2）在 read 方法中使用匿名内部类的形式启用 `CompletionHandler`，然后实现 `CompletionHandler` 的两个监听方法，completed 的时候打印结果，failed 的时候打印异常信息。<br />不管是 Future 形式还是 `Callback` 形式，总之异步 I/O 是一个强大的特性，可以保证在处理大文件时性能不受到显著的影响。
