Java
<a name="VWCt9"></a>
## Java I/O
流是一组有顺序的，有起点和终点的字节集合。是对设备文件间数据传输的总称和抽象。<br />在IO中涉及的设备文件包括文件、控制台、网络链接等，这其中又根据流的方向可以将两端的设备文件分为数据源对象和接收端对象

- 数据源对象：有能力产出数据
- 接收端对象：有能力接收数据

而IO流实际上屏蔽了在实际设备中的处理数据的细节，这些处理方式也叫通信方式可以包括顺序、随机存取、缓冲、二进制、按字符、按字节、按行等。<br />字节流和字符流。io默认都是直接操作字节的，多用于读取或书写二进制数据，这些类的基类为InputStream或OutputStream。而字符流操作的是为了支持Unicode编码，用于字符国际化，一个字符占用两个字节，这些类的基类为Reader或Writer。java的io在jdk1.1以后添加了字符流的支持，为直接操作字符流提供了方便。
<a name="L75Y2"></a>
### 关于字符类库和字节类库的选择
最明智的做法是尽量尝试使用Reader和Writer，一旦程序代码无法成功编译，就会发现自己不得不使用面向字节的类库。<br />根据数据的流向，流分为输入流和输出流，这里指的都是内存，内存输入为读，输出为写，I读O写。<br />Java提供了针对不同情况的处理流的类，以便于直观地进行数据操作，这些类就在java的io包之中。下面介绍java io包的类，整个io包大量应用了装饰模式。在实际使用过程中，涉及多层套用的构造函数必然都会有自己的一个装饰模式的架构，包括被装饰类（原构建基类），装饰超类，装饰具体类，用的过程中懂得去区分理解会更加灵活地使用它们。

- 节点流：文件(File)，管道(Piped)和数组(Array)（他们每个类都分别包括输入输出和字节字符四种流）
- 处理流：其余的都是处理类，他们都是属于节点流的装饰类，下面整理了一个关于处理流的表格。
| 处理流 | who | 个数 | 装饰功能 |
| --- | --- | --- | --- |
| 缓冲区 | Buffered开头的类 | 4 | 可将流放在缓冲区内操作 |
| 转化流 | InputStreamReader/OutputStreamWriter | 2 | 可将字节流转化为字符流 |
| 基本类型 | DataXXXStream | 2 | 可传输基本类型数据 |
| 对象流 | ObjectXXXStream | 2 | 可传输对象类型数据(序列化) |
| 打印流 | PrintStream/PrintWriter | 2 | 包含print和println的输出流 |
| 合并流 | SequenceInputStream | 1 | 可逻辑串联其他输入流 |
| 行号读入流 | LineNumberReader | 1 | 可得到一个携带行号的字符读入流 |
| 推回输入流 | PushbackInputStream/PushbackReader | 2 | 可将输入流push back或unread一个字节 |
| 字符串读写流 | StringWriter/StringReader | 2 | 可在缓冲区读写字符串 |

注意：

- 默认命名规则：字节流是输入输出(Input/Output)，字符流是读写(Writer/Reader)，当又有输入输出又有读写的时候，那一定是转化流(`InputStreamReader`/`OutputStreamWriter`)。
- 默认都是操作字节，所有操作字符的类都需要先经过转化流将字节流转为字符流再使用。
- `LineNumberInputStream`已过时，因为它是基于字节输入流的，而错误假定字节能充分表示字符，现已被`LineNumberReader`取代。
- `StringBufferInputStream`已过时，因为此类未能正确地将字符转换为字节，现已被e取代。
- 这里面还有一个过滤流的概念，它也包括输入输出字节字符四种流，在上面没有表示出来。它是一个抽象类，作为“装饰器”的接口，其中，“装饰器”为其他输入输出字符字节类提供有用功能。
<a name="iCIAz"></a>
### Java IO 转化流的适配器模式
在字符流处理类加入java io类库之前，所有的类都是面向字节流的，在jdk1.1以后，添加了字符流的支持，根据“开闭原则”，所以在不改变原有类的基础上，有了转化流：e和`OutputStreamWriter`，这两个类正是所谓的“适配器类”，e可以吧e转换为Reader，而e可以将e转换为Writer。字节流和字符流都有各自独立的一整套继承层次结构，而通过适配器类，可以在不改变原有类的前提下有效将他们结合起来。
<a name="b3Adi"></a>
### Java IO 装饰器模式的研究：
Java I/O类库需要多种不同功能的组合，存在filter类的原因就是抽象类filter是所有装饰器类的基类，装饰器必须具有和它所装饰对象相同的接口。`FilterInputStream`和`FilterOutputStream`是用来提供装饰器类接口以控制特定输入流（`InputStream`）和输出流（`OutputStream`）的两个类，他们的名字并不是很直观，包括`DataInput`/`OutputStream`, `BufferedInput`/`OutputStream`,`LineNumberInputStream`,`PushbackInputStream`,`PrintStream`等，这些过滤流类在下面都会有详细介绍。`FilterInputStream`和`FilterOutputStream`分别自I/O类库中的基类`InputStream`和`OutputStream`派生而立，这两个类是装饰器的必要条件（以便能为所有正在被修饰的对象提供通用接口）。
<a name="asc99"></a>
## 一、File / 文件流 / `RandomAccessFile`
节点流是直接与数据源相连，进行IO操作，文件是数据源中最常见的，下面介绍文件相关的内容。
<a name="feH8z"></a>
### 1、File
File类可以表示文件也可以表示文件夹目录，直接展示一段代码，就懂了。
```java
package javaS.IO;
import java.io.File;
/**
 * 基于磁盘IO操作的类 java.io.File
 *
 * 可以表示文件，也可以表示文件夹目录
 *
 */
public class FileS extends IOBaseS {
    @Test
    public void testFileMethods() throws IOException {
        logger.info("Start testing file methods.");
        File file = new File(root);
        if (!file.exists())
            /**
             * 创建目录 mkdir();
             */
            file.mkdir();
        if (file.isDirectory()) {
            File file1 = new File(root+"/UME.txt");
            File file2 = new File(root+"/HongXing.txt");
            /**
             * 创建文件 createNewFile();
             */
            file1.createNewFile();
            file2.createNewFile();
            File file3 = new File(root+"/Cinema");
            file3.mkdir();
            /**
             * 列出文件路径下的所有文件（包括文件和目录）
             */
            File[] files = file.listFiles();
            for (File f : files) {
                /**
                 * 判断该文件路径是否为目录
                 */
                if (f.isDirectory()) {
                    logger.info("The directory in 'Files' is: " + f.getName());
                } else {
                    logger.info("The file in 'Files' is: " + f.getName());
                }
                logger.info("Whose path is: " + f.getAbsolutePath());
            }
        } else {
            logger.info("FileS is not a directory!");
        }
        logger.info("Complete testing file methods.");
        /**
         * 输出：
         * 15:12:56[testFileMethods]: Start testing file methods.
         * 15:12:56[testFileMethods]: The file in 'Files' is: HongXing.txt
         * 15:12:56[testFileMethods]: Whose path is: /home/work/github/mainbase/resource/StudyFile/HongXing.txt
         * 15:12:56[testFileMethods]: The directory in 'Files' is: Cinema
         * 15:12:56[testFileMethods]: Whose path is: /home/work/github/mainbase/resource/StudyFile/Cinema
         * 15:12:56[testFileMethods]: The file in 'Files' is: UME.txt
         * 15:12:56[testFileMethods]: Whose path is: /home/work/github/mainbase/resource/StudyFile/UME.txt
         * 15:12:56[testFileMethods]: Complete testing file methods.
         */
    }
}
```
为了下面测试方便，在FileS类中加入一个静态方法，用来初始化目录：
```java
/**
     * 清空一个文件，以便于我们测试使用
     * 
     * @param filePath
     * @throws IOException
     */
public static void initEV(String filePath) throws IOException {
	File f = new File(filePath);
	if (f.exists())
		f.delete();
	f.createNewFile();
}
```
然后，File还支持过滤器的功能，例如想要列出某目录下的所有文本文件的名字。
```java
/**
     * 测试文件目录过滤器，例如列出目录下所有"*.txt"
     */
public void testFileFilter() {
	logger.info("start testing file filter.");
	String filterStr = "(./*)+(txt)$";
	File file = new File(root);
	String list[] = file.list(new FilenameFilter() {
		@Override
		public boolean accept(File dir, String name) {
			return Pattern.matches(filterStr, name);
		}
	});
	for (String s : list)
		logger.info(s);
}
/**
     * 输出：
     * 
     * 12:57:17[testFileFilter]: start testing file filter.
     * 
     * 12:57:17[testFileFilter]: HuaYi.txt
     * 
     * 12:57:17[testFileFilter]: HongXing.txt
     * 
     * 12:57:17[testFileFilter]: UME.txt
     */
```
正常的`String list[] = file.list();` 都能用到，它的意思是列出该目录下的所有文件名，如果想过滤这些文件名列出只想要的文本文件名，则需要通过匿名内部类创建一个`FilenameFilter`接口的实现类对象，创建时必须实现其方法`accept`方法，该方法是一个回调函数，每一个查找出来的文件名都要回调该方法，是否能够通过全靠`accept`的布尔返回值所决定，这也是策略模式的体现，因为在`accept`方法体的具体实现留给了开发者去自定义，所以这个函数更加有了一个“漏斗”的作用。只想得到文本文件名，那么定义一个正则表达式去过滤即可。<br />File其他方法的测试：
```java
public void testFileTool() {
	File file = new File(root);
	PPrint.pprint(file.listFiles());
	// 转为List
	@SuppressWarnings("unused")
	List<File> fl = Arrays.asList(file.listFiles());
	logger.info("file.length(): " + file.length());
	logger.info("file.getName(): " + file.getName());
	logger.info("file.getParent(): " + file.getParent());
	// file.renameTo(new File("resource/S"));// 重命名文件
	logger.info("file.canRead(): " + file.canRead());
	logger.info("file.canWrite(): " + file.canWrite());
	SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:SS:SSS");
	Date a = new Date(file.lastModified());
	logger.info("file.lastModified(): " + sdf.format(a));
}
```
输出：
```java
[
resource/StudyFile/access
resource/StudyFile/HuaYi.txt
resource/StudyFile/HongXing.txt
resource/StudyFile/Cinema
resource/StudyFile/UME.txt
]
13:59:20[testFileTool]: file.length(): 4096
13:59:20[testFileTool]: file.getName(): StudyFile
13:59:20[testFileTool]: file.getParent(): resource
13:59:20[testFileTool]: file.canRead(): true
13:59:20[testFileTool]: file.canWrite(): true
13:59:20[testFileTool]: file.lastModified(): 2017-11-30 13:55:00:000
```
一个用于操作数组或列表的实用打印输出类，其实这种写法自己也会经常在业务代码中写出。
```java
package javaS.IO;
import java.util.Arrays;
import java.util.Collection;
public class PPrint {
	public static String pFormat(Collection<?> c) {// 泛型方法
		if (c.size() == 0)
			return "[]";
		StringBuilder result = new StringBuilder("[");
		for (Object elem : c) {
			if (c.size() != 1) {
				result.append("\n");
			}
			result.append(elem);
		}
		if (c.size() != 1) {
			result.append("\n");
		}
		result.append("]");
		return result.toString();
	}
	/**
     * 打印一个可视化的集合
     * 
     * @param c
     */
	public static void pprint(Collection<?> c) {
		System.out.println(pFormat(c));
	}
	/**
     * 打印一个可视化的数组
     * 
     * @param c
     */
	public static void pprint(Object[] c) {
		System.out.println(pFormat(Arrays.asList(c)));
	}
}
```
<a name="rdz0p"></a>
### 2、文件流
文件流包括输入输出字符字节四种类，首先来看文件字节流处理。
```java
package javaS.IO;
import java.io.BufferedOutputStream;
/**
 * 字节流的学习
 * 
 * 基于字节I/O操作的基类：InputStream和OutputStream
 * 
 * 对应的缓存类：BufferedInputStream和BufferedOutputStream
 * 
 * 出入的主语是“内存”，出内存就是写入文件，入内存就是读取文件
 *
 */
public class ByteStreamS extends IOBaseS {
	@Test
	/**
     * 使用输出流OutputStream.write，将内存中的内容写入设备文件（这里的设备文件为File：磁盘文件）
     */
	public void testWrite2OutputStream() throws IOException {
		OutputStream fos = new FileOutputStream(root+"/UME.txt");//找不到该文件会自动创建（包括路径）
		/**
         * 内容中的字符串内容content
         */
		String content = "哈哈哈\n嘿嘿";
		fos.write(content.getBytes());// 直接写入字节
		fos.close();// 操作完注意将流关闭
		/**
         * 文件后面追加内容，构造函数加第二个参数true
         */
		OutputStream fosadd = new FileOutputStream(root+"/UME.txt", true);
		fosadd.write(" 你好".getBytes());
		fosadd.close();
	}
	@Test
	/**
     * 使用输入流读取InputStream.read，将设备文件（这里的磁盘文件是File）读到内存buffer中去。
     */
	public void testRead2InputStream() throws IOException {
		int bufferSize = 200;
		FileInputStream fis = new FileInputStream(root+"/UME.txt");
		byte buffer[] = new byte[bufferSize];
		int length;
		while ((length = fis.read(buffer, 0, bufferSize)) > -1) {
			String str = new String(buffer, 0, length);
			logger.info(str);
		}
		fis.close();// 操作完注意将流关闭
		/**
         * 输出：
         * 13:41:02[testInputStreamS]: 举杯邀明月床前明月光
         */
	}
}
```
<a name="JrjPu"></a>
#### `Buffered` 缓冲区的作用
可以将数据流从数据源中处理完毕都存入内存缓冲区，然后统一一次性与底层IO进行操作，可以有效降低程序直接操作IO的频率，提高io执行速度。以下为缓冲区处理流的使用。
```java
/**
     * 缓冲区处理流：BufferedInputStream，BufferedOutputStream，BufferedReader,BufferedWriter,
     * 一次性写入，降低占用IO的频率
     * 避免每次和硬盘打交道，提高数据访问的效率。
     */
@Test
public void testWrite2BufferedOutputStream() throws IOException {
	// OutputStream为基类
	OutputStream fosaddOnce = new FileOutputStream(root+"/UME.txt");
	OutputStream bs = new BufferedOutputStream(fosaddOnce);
	bs.write("举杯邀明月".getBytes());
	bs.flush();// 每次flush会将内存中数据一齐刷入到外部文件中，但不会close该流。
	bs.write("床前明月光".getBytes());
	/**
         * close方法除了有关闭流的作用，在其关闭流之前也会执行一次flush。
         * 注意一定要先关闭BufferedOutputStream，再关闭FileOutputStream，从外到内打开，要从内到外关闭。
         */
	bs.close();
	fosaddOnce.close();// 两个流都要关闭
}
```
<a name="k3NDL"></a>
#### 使用构造函数套接的方式优化代码
很少使用单一的类来创建流对象，而是通过叠合多个对象来提供所期望的功能。(这就是装饰器模式)
```java
public void testWrite2BufferedOutputStream() throws IOException {
	OutputStream bs = new BufferedOutputStream(new FileOutputStream(root+"/UME.txt"));
	bs.write("举杯邀明月".getBytes());
	bs.close();//只关闭一次即可。
}
```
下面再看字符流的处理，这里也包括了文件字符读写流（`FileWriter`/`FileReader`）的应用。
```java
package javaS.IO;
+import java.io.BufferedReader;
/**
 * 字符流的学习
 * 
 * 基于字符I/O操作的基类: Reader和Writer
 * 
 * 对应的缓存类：BufferedReader和BufferedWriter
 *
 */
public class CharacterStreamS extends IOBaseS {
    @Test
    /**
     * OutputStreamWriter,字节到字符的转化桥梁，转化过程中需指定编码字符集，否则采用默认字符集。
     */
    public void testWriter() throws IOException {
        // 文件输出流不变
        FileOutputStream fos = new FileOutputStream(root + "HongXing.txt");
        /**
         * 输出流写入类（这是比起字节流多出来的类）专门用来写入字符流，注意字符编码的参数
         * 
         * 如果只保留fos一个参数，编码默认为工作区默认编码，这里是“UTF-8"，
         * 
         * 字节编码为字符
         * 
         * 为了保证写入和读取的编码统一，请每次都要指定编码
         * 
         * 输出体系中提供的两个转换流，用于实现将字节流转换成字符流。
         */
        OutputStreamWriter osw = new OutputStreamWriter(fos);
        // 缓存写入类，对应BufferedOutputStream
        BufferedWriter bw = new BufferedWriter(osw);
        bw.write("感时花溅泪，恨别鸟惊心");
        bw.close();
        osw.close();
        fos.close();
        /**
         * 终版：将close部分缩短
         */
        BufferedWriter bwA = new BufferedWriter(
                new OutputStreamWriter(new FileOutputStream(root + "HongXing.txt", true), "UTF-8"));// 注意层级，加入指定编码的参数
        bwA.write("\n烽火连三月，家书抵万金");
        bwA.close();
    }
    @Test
    /**
     * InputStreamReader,字节到字符的转化桥梁，转化过程中需指定编码字符集，否则采用默认字符集。
     */
    public void testReader() throws IOException {
        FileInputStream fis = new FileInputStream(root + "HongXing.txt");
        /**
         * 输出流读取类（这是比起字节流多出来的类）专门用来读取字符流，注意字符编码的参数要与写入时的编码相同，否则会乱码
         * 
         * 输入体系中提供的两个转换流，用于实现将字节流转换成字符流。
         */
        InputStreamReader isr = new InputStreamReader(fis, "UTF-8");
        BufferedReader br = new BufferedReader(isr);
        String str;// 这里也可以用StringBuilder来累计文件的全部内容，最后输出。
        while ((str = br.readLine()) != null) {
            logger.info(str);
        }
        br.close();// 显示调用close方法关闭流
        isr.close();
        fis.close();
        /**
         * 终版：将close部分缩短
         */
        BufferedReader brA = new BufferedReader(
                new InputStreamReader(new FileInputStream(root + "HongXing.txt"), "UTF-8"));
        String strA;
        while ((strA = brA.readLine()) != null) {
            logger.info(strA);
        }
        brA.close();
        /**
         * 输出： 15:04:07[testReader]: 感时花溅泪，恨别鸟惊心 15:04:07[testReader]:
         * 烽火连三月，家书抵万金
         */
    }
    /**
     * File提供了支持字符读写的封装类：FileWriter和FileReader
     * 所以不必每次都使用InputStreamReader和OutputStreamWriter来转换。
     */
    @Test
    public void testFileWriter() throws IOException {
        FileWriter fw = new FileWriter(root + "HongXing.txt", true);
        fw.write("\n杜甫《春望》");
        fw.close();
    }
    @Test
    public void testFileReader() throws IOException {
        FileReader fr = new FileReader(root + "HongXing.txt");
        // FileReader直接read方法没有readLine方便，所以套上装饰类BufferedReader借它的readLine用一用
        BufferedReader br = new BufferedReader(fr);
        String str;
        while ((str = br.readLine()) != null) {
            logger.info(str);
        }
        br.close();
        fr.close();
    }
}
```
注意：File提供了支持字符读写的封装类：`FileWriter`和`FileReader`，所以不必每次都使用`InputStreamReader`和`OutputStreamWriter`来转换。
<a name="HpsPE"></a>
### 3、`RandomAccessFile`
`RandomAccessFile` 是任意位置进入文件的意思，适用于由大小已知的记录组成的文件，它有一个`seek`方法定义了文件的位置，所以要注意在对文件进行`RandomAccessFile`操作时，要记住文件的内容的位置和大小，否则会发生内容复写更改的后果。
```java
package javaS.IO;
import java.io.File;
/**
 * RandomAccessFile：有一个指针seek，对文件任意位置操作的类
 *
 */
public class RandomAccessFileS extends IOBaseS {
    @Before
    public void testWrite2RAFile() throws IOException {
        FileS.initEV(root + "/access");// 首先清空access文件。
        RandomAccessFile raf = new RandomAccessFile(root + "/access", "rw");// rw是采用读写的方式打开文件
        logger.info(raf.length());
        Student Jhon = new Student(1001, "Jhon", 26, 1.85d);
        Student Jack = new Student(1002, "Jack", 25, 1.75d);
        Jhon.write(raf);// 写入文件以后，指针到当前文本结尾
        // 当前seek是从seek(raf.length)开始的
        logger.info(raf.length());
        Jack.write(raf);// 继续写入，指针继续移动到末尾，相当于追加
        logger.info(raf.length());
        raf.close();
    }
    @After
    public void testReadRAFile() throws IOException {
        RandomAccessFile raf = new RandomAccessFile(root + "/access", "r");
        // 获取raf时，seek就是在文件开始位置
        logger.info(raf.length());
        Student Lily = new Student();
        Lily.read(raf);
        logger.info(Lily);
        Lily.read(raf);
        logger.info(Lily);
        // 读入次数是有限的，一定要预先知道最多读几次，否则会报EOFException。
        Lily.read(raf);
        logger.info(Lily);
        raf.close();
        /**
         * 输出： 16:14:30[testReadRAFile]: id:1001 name:Jhon age:26 height:1.85
         */
    }
}
```
`RandomAccessFile`就像数据库一样，是将数据一条一条的写入的，而这些数据必须是类对象，不能是基本类型数据，因为该类对象要写两个方法`write`和`read`用来将对象写入`RandomAccessFile`，这两个方法也不是继承于谁，可以起别的名字，调用的时候也是自己调用。
```java
public class Student extends IOBaseS{
    private long id;
    private String name;
    private int age;
    private double height;
    public Student() {
    }
    public Student(long id, String name, int age, double height) {
        super();
        this.id = id;
        this.name = name;
        this.age = age;
        this.height = height;
    }
    public void write(RandomAccessFile raf) throws IOException {
        raf.writeLong(id);
        raf.writeUTF(name);// 采用UTF的编码方式写入字符串
        raf.writeInt(age);
        raf.writeDouble(height);
    }
    /**
     * 要严格按照写入顺序读取，这也是ORM的意义
     * 
     * @param raf
     * @throws IOException
     */
    public void read(RandomAccessFile raf) throws IOException {
        this.id = raf.readLong();
        this.name = raf.readUTF();
        this.age = raf.readInt();
        this.height = raf.readDouble();
    }
}
```
按照类的结构，将字段按顺序写入文件的任意位置，读取的时候也要按照相同的顺序读出。
<a name="pyB3c"></a>
#### `RandomAccessFile` 追加内容
追加的时候要使用`seek`方法，将光标定位到文件末尾，如果不重新定位，默认从起点开始写入，就会覆盖原有数据。
```java
/**
 * 追加内容
 */
public void testWriteAppend2RAFile() throws IOException {
	RandomAccessFile raf = new RandomAccessFile(root + "/access", "rw");// rw是采用读写的方式打开文件
	Student Mason = new Student(1003, "Mason", 26, 1.82d);// 这里的“Mason”比上面的两条数据多一位字符
	// 追加内容要先调整seek的位置到raf.length，然后开始追加内容
	raf.seek(raf.length());
	Mason.write(raf);
	logger.info(raf.length());
	raf.close();
}
```
<a name="fGhlV"></a>
#### `RandomAccessFile` 在任意位置插入数据
```java
@Test
public void insert() {
	Student Hudson = new Student(1005, "Hudson", 45, 1.76d);
	try {
		insert(root + "/access", 26, Hudson);
	} catch (IOException e) {
		e.printStackTrace();
	}
}
/**
     * 在RandomAccessFile指定位置插入数据，先将位置后面的数据放入缓冲区，插入数据以后再将其写回来。
     * 
     * @param file
     *            能找到该文件的路径，是字符串类型
     * @param position
     *            其实外部调用的时候能找到这个位置比较难，因为不确定数据长度是多少，弄不好就会将数据拆分引起混乱。
     * @param content
     */
public void insert(String file, long position, Student s) throws IOException {
	/**
         * 创建一个临时文件
         * 
         * 在使用完以后就将其删除
         */
	File tempFile = File.createTempFile("temp", null);
	tempFile.deleteOnExit();
	FileOutputStream fos = new FileOutputStream("temp");
	/**
         * 将插入位置后面的数据缓存到临时文件
         */
	RandomAccessFile raf = new RandomAccessFile(file, "rw");
	raf.seek(position);
	byte[] buffer = new byte[20];
	while (raf.read(buffer) > -1) {
		fos.write(buffer);
	}
	raf.seek(position);
	/**
         * 向RandomAccessFile写入插入内容
         */
	s.write(raf);
	/**
         * 从临时文件中写回缓存数据到RandomAccessFile
         */
	FileInputStream fis = new FileInputStream("temp");
	while (fis.read(buffer) > -1) {
		raf.write(buffer);
	}
	fos.close();
	fis.close();
	raf.close();
	tempFile.delete();//删除临时文件tempFile
}
```
插入数据的时候，`RandomAccessFile`并没有提供相关的方法，由于在旧的位置写入数据会覆盖原有数据，所以要将插入位置后面的数据缓存到一个临时文件中，插入数据以后再将临时文件的内容接着被插入的数据后面。<br />下面定义一个完整测试流程：先初始化一个空白文件，然后添加两行数据Jhon和Jack，然后在他俩中间插入Hudson，最后读出该文件数据，验证结果输出为：
```java
    /**
     * 输出： 
     * 17:10:31[testWrite2RAFile]: 0 17:10:31[testWrite2RAFile]: 26
     * 17:10:31[testWrite2RAFile]: 52 17:10:31[testReadRAFile]: 94
     * 17:10:31[testReadRAFile]: id:1001 name:Jhon age:26 height:1.85
     * 17:10:31[testReadRAFile]: id:1005 name:Hudson age:45 height:1.76
     * 17:10:31[testReadRAFile]: id:1002 name:Jack age:25 height:1.75
     */
```
<a name="XnlQY"></a>
## 二、`DataOutputStream` / `DataInputStream`
这一对类可以直接写入java基本类型数据（没有String），但写入以后是一个二进制文件的形式，不可以直接查看。`DataOutputStream` / `DataInputStream`是常用的过滤流类，如果对象的序列化是整个对象转换为一个字节序列的话，`DataOutputStream `/ `DataInputStream`就是将字段序列化，转为二进制数据。下面看代码。
```java
package javaS.IO;
import java.io.BufferedInputStream;
public class DataStream extends IOBaseS {
    @Test
    /**
     * DataOutputStream，可以直接写入java基本类型数据（没有String），但写入以后是一个二进制文件的形式，不可以直接查看。
     * 
     * 文本文件是二进制文件的特殊形式，这是通过转储实现的
     */
    public void testWrite2DataOutputStream() throws IOException {
        OutputStream fosaddOnce = new FileOutputStream(root + "/UME.txt");
        OutputStream bs = new BufferedOutputStream(fosaddOnce);
        DataOutputStream dos = new DataOutputStream(bs);
        dos.writeInt(22);
        dos.writeShort(1222222222);
        dos.writeLong(20L);
        dos.writeByte(3);
        dos.writeChar(42);
        dos.close();
        bs.close();
        fosaddOnce.close();
        /**
         * 终版：上面的close阶段要从内向外关闭三次，比较麻烦，下面直接采用装饰模式标准写法，套接对象。
         * 套接对象：最里面的一定是节点流，它之外的无论几层都是处理流
         * FileOutputStream:属于节点流，其他节点流还包括管道和数组，剩下的都是处理流
         * BufferedOutputStream:缓冲技术（也属于处理流） DataOutputStream:处理流
         */
        DataOutputStream dosA = new DataOutputStream(new BufferedOutputStream(new FileOutputStream(root + "/UME.txt")));
        dosA.writeInt(22);
        dosA.writeShort(65538);// DataOutputStream并不会检查数据是否越界，越界的数据按照二进制方式截取，只保留界限以内的数据。
        dosA.writeLong(20L);
        dosA.writeByte(3);
        dosA.writeChar(42);
        dosA.writeDouble(3.1415926);
        dosA.close();// 只关闭一次。
    }
    @Test
    /**
     * 通过DataInputStream处理流读取二进制文件，一定要按照写入的顺序去读取java基本类型的文件内容，否则会出现乱码或者不准确的信息
     */
    public void testRead2DataInputStream() throws IOException {
        DataInputStream dis = new DataInputStream(new BufferedInputStream(new FileInputStream(root + "/UME.txt")));
        logger.info(dis.readInt());
        /**
         * 即使存入越界的树65538，也不会报错，因为超出部分不会被存入，存入的只是超出的部分。
         * short类型占据16位的空间，因此将65538转为二进制数，超出16位的部分自动截掉，只保留16为以内的数据，所以就变成了2。
         */
        logger.info(dis.readShort());
        logger.info(dis.readLong());
        logger.info(dis.readByte());
        logger.info(dis.readChar());
        logger.info(dis.readDouble());
        dis.close();
        /**
         * 输出： 
         * 13:39:03[testDataInputStream]: 22 
         * 13:39:03[testDataInputStream]: 2 
         * 13:39:03[testDataInputStream]: 20 
         * 13:39:03[testDataInputStream]: 3
         * 13:39:03[testDataInputStream]: *
         * 13:39:03[testDataInputStream]: 3.1415926
         */
    }
}
```
<a name="PNhQU"></a>
## 三、对象流 / 序列化
数据传输过程中，都会默认采用二进制文件的方式，因为计算机的底层识别方式就是二进制，不依赖任何运行环境或是程序设计语言，所以这是实现数据传输跨平台跨网络的基础。序列化可以直接将java对象转化为一个字节序列，并能够在以后将这个字节序列完全恢复为原来的对象，这一过程甚至可以通过网络进行，这意味着序列化机制能自动弥补不同操作系统之间的差异。<br />如果要让一个类的对象可以成功序列化，那么它必须实现`Serializable`接口，这个接口就像`Cloneable`接口一样，他们只是一个空的标识接口。<br />下面这段话用来描述对象序列化真的非常受启发。<br />当创建对象时，只要需要，它就会一直存在，但是在程序终止时，无论如何它都不会继续存在。尽管这么做肯定是有意义的，但是仍旧存在某些情况，如果对象能够在程序不运行的情况下仍能存在并保存其信息，那将非常有用。这样，在下次运行程序时，该对象将被重建并且拥有的信息与在程序上次运行时它所拥有的信息相同。当然，可以通过将信息写入文件或数据库来达到相同的效果，但是在使万物都成为对象的面向对象的精神中，如果能够将一个对象声明是“持久性”的，并处理掉所有细节，那将会显得十分方便。<br />相对于数据库存储那种重量级持久性来说，对象的序列化可以实现轻量级持久性。<br />持久性意味着一个对象的生命周期并不取决于程序是否正在执行，它可以生存于程序的调用之间。<br />轻量级是因为不能用某个关键字来简单定义一个对象，并让系统自动维护其他细节问题，相反，对象必须在程序中显示地序列化和反序列化。<br />如果需要一个更严格的持久性机制，可以考虑像Hibernate之类的工具。TODO: 会有一篇文章深入介绍Hibernate。<br />对象序列化的意义：

- 支持java的远程方法调用RMI，它使存活于其他计算机上的对象使用起来就像是存活在本机上一样。
- 对于java bean，一定要保存下来它在设计阶段对他的状态信息的配置，在程序启动时进行后期恢复，这中具体工作就是由对象序列化完成的。
```java
package javaS.IO;
/**
 * Serializable为标记接口，表示这个类的对象可以被序列化。
 *
 */
public class Student extends IOBaseS implements Serializable {
    /**
     * 类中的声明
     * 
     * transient和static的变量不会被序列化
     */
    /**
     * 序列号：避免重复序列化
     * 如果serialVersionUID被修改，反序列化会失败。
     * 当程序试图序列化一个对象时，会先检查该对象是否已经被序列化过，只有该对象从未（在本次虚拟机中）被序列化，系统才会将该对象转换成字节序列并输出。
     */
    private static final long serialVersionUID = -6861464712478477441L;
    private long id;
    private String name;
    private int age;
    private transient double height;
    public Student() {
    }
    public Student(long id, String name, int age, double height) {
        super();
        this.id = id;
        this.name = name;
        this.age = age;
        this.height = height;
    }
    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("id:");
        sb.append(this.id);
        sb.append(" ");
        sb.append("name:");
        sb.append(this.name);
        sb.append(" ");
        sb.append("age:");
        sb.append(this.age);
        sb.append(" ");
        sb.append("height:");
        sb.append(this.height);
        return sb.toString();
    }
    /**
     * 相当于重写了ObjectOutputStream.writeObject方法，ObjectOutputStream写入该对象的时候会调用该方法
     * 
     * 作用：可以在序列化过程中，采用自定义的方式对数据进行加密
     * 
     * 参考源码：
     * 
     * public final void writeObject(Object obj) throws IOException {
            if (enableOverride) {// 如果发现参数Object有重写该方法，则去执行重写的方法，否则继续执行本地方法。
                writeObjectOverride(obj);
                return;
            }
            try {
                writeObject0(obj, false);
            } catch (IOException ex) {
                if (depth == 0) {
                    writeFatalException(ex);
                }
                throw ex;
            }
        }
     * 
     * 
     * readObject方法的分析同上。
     * 
     * @param out
     * @throws IOException
     */
    private final void writeObject(ObjectOutputStream out) throws IOException {
        logger.info("Start writing data to Object.");
        out.writeLong(this.id);
        /**
         * 下面的writeObject是StringBuffer源码中的：
         * 
         * readObject is called to restore the state of the StringBuffer from a stream.
            private synchronized void writeObject(java.io.ObjectOutputStream s)
                    throws java.io.IOException {
                java.io.ObjectOutputStream.PutField fields = s.putFields();
                fields.put("value", value);
                fields.put("count", count);
                fields.put("shared", false);
                s.writeFields();
            }
         */
        out.writeObject(new StringBuffer(name));
        out.writeInt(this.age);
        out.writeDouble(this.height);// 这里重写以后，就忽略了transient的设置
    }
    private final void readObject(ObjectInputStream in) throws IOException, ClassNotFoundException {
        logger.info("Start reading data to Object.");
        this.id = in.readLong();
        /**
         * 下面的readObject是StringBuffer源码中的：
         * 
         *  readObject is called to restore the state of the StringBuffer from a stream.
            private void readObject(java.io.ObjectInputStream s)
                    throws java.io.IOException, ClassNotFoundException {
                java.io.ObjectInputStream.GetField fields = s.readFields();
                value = (char[])fields.get("value", null);
                count = fields.get("count", 0);
            }
         */
        this.name = ((StringBuffer) in.readObject()).toString();
        this.age = in.readInt();
        this.height = in.readDouble();
    }
}
```
这个类内容比较多，因为为了更好的去分析，将jdk的源码也粘贴到注释区域了。但是不影响，文末会有源码位置，感兴趣的同学可以去下载源码查看。在这个类中，重写了`readObject`和`writeObject`方法，外部`ObjectXXXStream`在调用的时候会先找对象类中是否有重写的`readObject`和`writeObject`方法，如果有则使用没有才调用自己内部默认的实现方法。
```java
package javaS.IO;
import java.io.FileInputStream;
/**
 * 研究对象序列化（跨平台，跨网络的基础）
 * 
 * 内存 -> 磁盘/网络
 * 
 * Java对象 -> 二进制文件
 *
 */
public class ObjectStreamS extends IOBaseS {
    /**
     * ObjectOutputStream:对象流（对象序列化），不同于DataOutputStream是操作基本类型。
     * 测试序列化对象存储结构
     */
    @Test
    public void testWriteSerialObject() throws IOException {
        FileS.initEV(root + "/access");// 先将access文件清空。
        ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(root + "/access"));
        Student Lu = new Student(2001, "Luxa", 31, 1.81d);
        // 可以写入不同的序列化对象数据，但要记录写入顺序
        oos.writeObject(Lu);
        oos.close();
        /**
         * access内容：由于写入的是一个二进制文件，所以打开是乱码
         * 
         * ¬í^@^Esr^@^PjavaS.IO.Student Ç.2<95>×³^?^B^@^DI^@^CageD^@^FheightJ^@^BidL^@^Dnamet^@^RLjava/lang/String;xp^@^@^@^_?üõÂ<8f>\(ö^@^@^@^@^@^@^GÑt^@^DLuxa
         */
    }
    /**
     * ObjectInputStream:对象反序列化
     * 读取二进制文件（序列号文件）
     * 
     * @throws IOException
     * @throws ClassNotFoundException
     */
    @Test
    public void testReadSerialObject() throws IOException, ClassNotFoundException {
        ObjectInputStream ois = new ObjectInputStream(new FileInputStream(root + "/access"));
        // 可以读取不同的对象的数据，但是要按照写入顺序去读取。
        Student s = (Student) ois.readObject();
        logger.info(s);
        ois.close();
        /**
         * ①输出：
         * 
         * 10:24:08[testReadSerialObject]: id:2001 name:Luxa age:31 height:1.81
         * 
         * ②若height属性变量被声明为transient，则该变量在序列化过程中不会被写入，为初始值。输出为：
         * 
         * 10:29:34[testReadSerialObject]: id:2001 name:Luxa age:31 height:0.0
         */
    }
}
```
transient变量可标识出来不被序列化的字段，这些字段可能携带敏感信息，例如密码。但是这个关键字在重写了`writeObject`和`readObject`方法以后，不好使了。<br />序列化的使用忠告：

- 谨慎实现`Serializable`接口，需要随时保持uuid的时效性以及一致性，对于结构性框架中要避免不可序列化的类与已序列化的类之间的继承关系。
- 要考虑实现自定义的序列化形式，正如以上代码中所做的那样。
- 保护性的编写`readObject`方法。加入异常处理，让无效的序列化对象在反序列化过程中终端抛出异常。
<a name="qBsgM"></a>
## 四、`PrintStream` / `PrintWriter`
首先请问java的标准输入流是什么？是`InputStream`，正确。那么java的标准输出流是什么？是`OutputSteam`？No！而是`PrintStream`。<br />因为标准输入输出流是`System`类的定义，System中有三个字段，in是`InputStream`类型，对应的是标准输入流，err和out都是`PrintStream`对象，out对应的是标准输出流。常用的`System.out.println(data)`方法的返回值就是`PrintStream`对象，此流默认输出在控制台，也可以重定向输出位置：
```java
System.setOut(new PrintStream(new FileOutputStream(file)));
System.out.println("这些内容只能在file对象的文件中才能看到哦！");
```
`PrintWriter`就是`PrintStream`的字符操作的版本。`PrintStream`都是针对字节流进行操作的，如果要操作字符流，可以使用`PrintWriter`。下面直接看代码的注释吧。
```java
package javaS.IO;
import java.io.FileOutputStream;
public class PrintStreamS extends IOBaseS {
    /**
     * 打印流的使用非常类似于FileWriter，但是它支持更多的方法，同时也有着丰富的构造方法。
     */
    @Test
    public void testPrintStream() throws IOException {
        FileS.initEV(root + "HongXing.txt");
        // PrintStream p = new PrintStream(root + "HongXing.txt");
        // PrintStream p = new PrintStream(new File(root + "HongXing.txt"));
        // PrintStream p = new PrintStream(new FileOutputStream(root +
        // "HongXing.txt"), true, "UTF-8");
        PrintStream p = System.out;// 数据源切换到控制台，标准输出，相当于System.out.xxx();
        p.append("海上升明月");
        p.println("润物细无声");
        p.print("当春乃发生");
        p.write("无敌心头好".getBytes());
        p.flush();// 刷入内存数据到数据源
        System.out.write("asdfas".getBytes());
        p.close();
        /**
         * 输出：
         * 
         * 海上升明月润物细无声
         * 
         * 当春乃发生无敌心头好
         */
    }
    /**
     * PrintWriter与PrintStream的两点区别：
     * 
     * write方法一个是写入字节，一个是写入字符。
     * 
     * 一般来讲，使用PrintStream多一些。
     */
    @Test // 如果忘记写该注解，执行JUnit会报错initializationError
    public void testPrintWriter() throws IOException {
        FileS.initEV(root + "HongXing.txt");
        // PrintWriter p = new PrintWriter(root + "HongXing.txt");
        // PrintWriter p = new PrintWriter(new File(root + "HongXing.txt"));
        // 第二个参数为autoflush，如果为true的话，println、printf和format会自动执行flush。
        // PrintWriter p = new PrintWriter(new FileOutputStream(root + "HongXing.txt"), true);
        System.setOut(new PrintStream(new FileOutputStream(root + "HongXing.txt")));// 输出重定向，从默认的控制台转到文件，这也是日志系统的基本思想。
        PrintWriter p = new PrintWriter(System.out, true);// 将PrintWriter的打印位置改到标准输出
        p.append("海上升明月");
        p.println("润物细无声");
        p.print("当春乃发生");
        p.write("无敌心头好");// 这是与PrintStream唯一区别了
        p.flush();// PrintWriter也支持刷入操作
        p.close();
    }
    /**
     * 测试标准输入输出
     */
    public void testStandardIO() throws IOException {
        BufferedReader stdin = new BufferedReader(new InputStreamReader(System.in));
        String str;
        while ((str = stdin.readLine()) != null && str.length() != 0)
            logger.info(str);
    }
}
```
<a name="LXm2z"></a>
## 五、`StringWriter` / `StringReader`
这没什么太多可说的，就是对一个字符串的读写操作，一般很少单独使用，因为直接使用String就可以将他们代替，然而当需要一个流的时候，可以与其他IO流进行组合使用。
```java
package javaS.IO;
import java.io.IOException;

public class StringIOS extends IOBaseS {
	@Test
	public void testStringWriter() throws IOException {
		StringWriter sw = new StringWriter();
		sw.write("Hello");
		sw.append("A");
		sw.close();
		logger.info(sw);
		StringReader sr = new StringReader("Hello");
		int c;
		StringBuilder sb = new StringBuilder();
		while ((c = sr.read()) != -1) {
			sb.append((char)c);
		}
		logger.info(sb);
		/**
         * Output:
         * 
         * 12:56:47[testStringWriter]: HelloA
         * 
         * 12:56:47[testStringWriter]: Hello
         */
	}
}
```
<a name="Qva0p"></a>
## Java IO基础的总结
关于节点流，以上部分展示了文件字符字节输入输出流和数组各种流的使用。唯独没有管道相关的内容，管道的部分会在NIO中去研究。<br />关于处理流，展示了缓冲区，对象流，基本类型数据流，转化流，打印流，合并流，行号读入流，推回输入流以及字符串读写流。
