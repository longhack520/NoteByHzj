Java<br />如果喜欢做封装、做框架的话，这个内容可能很有用。
<a name="qzHSE"></a>
## 什么是隐藏类
隐藏类，是一种不能被其他类直接使用的类。引入隐藏类的主要目的是给框架来使用，使得框架可以在运行时生成类，并通过反射间接使用它们。<br />可能有点抽象，下面通过一个例子来直观的认识它！
<a name="Jvz7D"></a>
## 隐藏类案例
**第一步**：先创建一个普通的Java类
```java
public class JEP371HiddenClasses {

    public static String hello() {
        return "https://www.yuque.com";
    }

}
```
**第二步**：编译一下，或得编译后的class文件。然后使用Base64对文件内容Encode，可以用各种工具，也可以用下面代码来获取：
```java
String filePath = "JEP371HiddenClasses.class";
byte[] b = Files.readAllBytes(Paths.get(filePath));
log.info(Base64.getEncoder().encodeToString(b));
```
执行一下，获取到内容如下：
```java
yv66vgAAAD0AFAoAAgADBwAEDAAFAAYBABBqYXZhL2xhbmcvT2JqZWN0AQAGPGluaXQ+AQADKClWCAAIAQAZaHR0cHM6Ly93d3cuZGlkaXNwYWNlLmNvbQcACgEALmNvbS9kaWRpc3BhY2UvZGVidWcvamF2YTE1L0pFUDM3MUhpZGRlbkNsYXNzZXMBAARDb2RlAQAPTGluZU51bWJlclRhYmxlAQASTG9jYWxWYXJpYWJsZVRhYmxlAQAEdGhpcwEAMExjb20vZGlkaXNwYWNlL2RlYnVnL2phdmExNS9KRVAzNzFIaWRkZW5DbGFzc2VzOwEABWhlbGxvAQAUKClMamF2YS9sYW5nL1N0cmluZzsBAApTb3VyY2VGaWxlAQAYSkVQMzcxSGlkZGVuQ2xhc3Nlcy5qYXZhACEACQACAAAAAAACAAEABQAGAAEACwAAAC8AAQABAAAABSq3AAGxAAAAAgAMAAAABgABAAAAAwANAAAADAABAAAABQAOAA8AAAAJABAAEQABAAsAAAAbAAEAAAAAAAMSB7AAAAABAAwAAAAGAAEAAAAGAAEAEgAAAAIAEw==
```
这个内容就是第一步写的类。<br />**第三步**：通过反射加载上面生成的类，并调用隐藏类中的hello函数，代码如下：
```java
/**
 * 程序猿DD
 * <a href="https://www.didispace.com/java-features/">Java新特性解读</a>
 */
@Test
void testHiddenClasses() throws Throwable {
	// 1. 加载encode之后的隐藏类
	String CLASS_INFO = "yv66vgAAAD0AFAoAAgADBwAEDAAFAAYBABBqYXZhL2xhbmcvT2JqZWN0AQAGPGluaXQ+AQADKClWCAAIAQAZaHR0cHM6Ly93d3cuZGlkaXNwYWNlLmNvbQcACgEALmNvbS9kaWRpc3BhY2UvZGVidWcvamF2YTE1L0pFUDM3MUhpZGRlbkNsYXNzZXMBAARDb2RlAQAPTGluZU51bWJlclRhYmxlAQASTG9jYWxWYXJpYWJsZVRhYmxlAQAEdGhpcwEAMExjb20vZGlkaXNwYWNlL2RlYnVnL2phdmExNS9KRVAzNzFIaWRkZW5DbGFzc2VzOwEABWhlbGxvAQAUKClMamF2YS9sYW5nL1N0cmluZzsBAApTb3VyY2VGaWxlAQAYSkVQMzcxSGlkZGVuQ2xhc3Nlcy5qYXZhACEACQACAAAAAAACAAEABQAGAAEACwAAAC8AAQABAAAABSq3AAGxAAAAAgAMAAAABgABAAAAAwANAAAADAABAAAABQAOAA8AAAAJABAAEQABAAsAAAAbAAEAAAAAAAMSB7AAAAABAAwAAAAGAAEAAAAGAAEAEgAAAAIAEw==";
	byte[] classInBytes = getDecoder().decode(CLASS_INFO);
	Class<?> proxy = MethodHandles.lookup()
		.defineHiddenClass(classInBytes, true, MethodHandles.Lookup.ClassOption.NESTMATE)
		.lookupClass();

	// 输出类名
	log.info(proxy.getName());
	// 输出类有哪些函数
	for(Method method : proxy.getDeclaredMethods()) {
		log.info(method.getName());
	}
	// 2. 调用hello函数
	MethodHandle mh = MethodHandles.lookup().findStatic(proxy, "hello", MethodType.methodType(String.class));
	String result = (String) mh.invokeExact();
	log.info(result);
}
```
可以根据注释来理解。最后执行这个测试内容，可以获得如下输出：
```java
17:20:50.360 [main] INFO com.didispace.debug.java15.JEP371Test - com.didispace.debug.java15.JEP371HiddenClasses/0x0000000800cb0c00
17:20:50.361 [main] INFO com.didispace.debug.java15.JEP371Test - hello
17:20:50.361 [main] INFO com.didispace.debug.java15.JEP371Test - https://www.yuque.com
```

- 第一行：输出了这个隐藏类的类名
- 第二行：输出了这个隐藏类下的方法名称
- 第三行：调用隐藏类下的hello方法获得的返回内容

是不是还挺简单？平时会参与一些基础框架和平台的开发工作的话，一定觉得这个功能还挺不错的吧，毕竟又多了一种动态功能的实现手段！
