Java<br />在 Java 中有这样的一个类，平时可能都用不到，但是需要的时候还是会很方便的，它就是 Java 中的javafx.util.Pair。在介绍 Pair 的使用之前先看看下面的例子。<br />如果对 Python 或者 GO 这两个语言有了解的，应该知道这两种语言是有下面这样的特性，那就是一个函数可以返回多个值，如下所示：<br />Python 示例：
```python
#! /usr/bin/env python3
# -*- coding: utf-8 -*-

"""
日期：2022-07-09 12:14
描述：a test module

"""

def mul_result(a, b):
	key = "key"
	value = a + b
	return key, value


if __name__ == '__main__':
	key, mul = mul_result(3, 4)
	print(key)
	print(mul)
```
GO 示例：
```go
package main

import "fmt"

func mul_result(a, b int) (key string, value int) {
	key = "key"
	value = a + b
	return key, value
}

func main() {
	key, value := mul_result(3, 4)
	fmt.Println(key)
	fmt.Println(value)
}
```
mul_result 函数接收两个入参，并且同时也返回两个结果，在调用的地方可以直接获取到两个返回值。<br />那么在 Java 语言中能否实现这样的效果呢？<br />有的小伙伴就说了，那不是 so easy 吗？可以定义一个实体类，想返回多少个字段就返回多少个字段，还有的小伙伴就说了，可以利用集合不管是 List 还是 Map 都可以实现，如下所示：
```java
public static void main(String[] args) {
	Map<String, Object> result = mul_result(3, 4);
	System.out.println(result.get("key"));
	System.out.println(result.get("value"));
}

public static Map<String, Object> mul_result(int a, int b) {
	Map<String, Object> result  = new HashMap<>();
	result.put("key", "key");
	result.put("value", a + b);
	return result;
}
```
这种做法确实可以，而且很多时候就是这么使用的，对于确定的类型可以使用确定类型的集合，对于不确定类型可以用 Object 类型。不过当对于有时候只有两个固定的 key-value 需要返回的时候，就可以用一种更优雅的 Pair 形式来进行处理。
```java
public static void main(String[] args) {
	Pair<String, Integer> pair = testPair(3, 4);
	System.out.println(pair.getKey());
	System.out.println(pair.getValue());
}

public static Pair<String, Integer> testPair(int a, int b) {
	Pair<String, Integer> pair = new Pair<>("key", a + b);
	return pair;
}
```
这里使用的 Pair 是 JDK 自带的类javafx.util.Pair，Pair 对象通过构造函数创建过后，是不能进行修改的，除了 Pair 之外，JDK 还有另外两个一样功能的类可以使用，如下所示：
```java
public static void main(String[] args) {

    AbstractMap.SimpleEntry<String, Integer> simpleEntry = new AbstractMap.SimpleEntry<>("Java", 666);
    System.out.println(simpleEntry.getKey());
    System.out.println(simpleEntry.getValue());
    simpleEntry.setValue(6666);

    AbstractMap.SimpleImmutableEntry<String, Integer> immutableEntry = new AbstractMap.SimpleImmutableEntry<>("Java", 666);
    System.out.println(immutableEntry.getKey());
    System.out.println(immutableEntry.getValue());
    immutableEntry.setValue(6666);

  }
```
在抽象类 `AbstractMap` 中有 `SimpleEntry` 和 `SimpleImmutableEntry` 这两个子类也有同样的效果，另外从名字上也可以看出来 `SimpleImmutableEntry` 是不支持修改，如果进行了修改会报错 `UnsupportedOperationException`。<br />Pair 类的使用场景主要就是在一些需要返回固定配对的地方，真正遇到的场景可能也不太多，所以在日常的工作中也并不常用，而且可替代的方案也有很多。<br />其实通过 JDK 的源码可以看到 Pair 的实现也特别简单，底层也就是包含两个字段 key，value 的实体类，跟自己写的没太大的区别，不过既然 JDK 已经提供的具体的实现为何不采用拿来主义呢？<br />写到这里有的小伙伴就要问了，那要想返回一个三元组怎么办呢？Python 和 GO 直接加一个返回值就行了，在 Java 这里不行了吧。确实JDK 本身并没有提供三元组的实现，如果真的需要可以使用 Pair 自己来构造一个三元组，不过这个世界上总有一些好事之者已经做好了。<br />Javatuples 是一个第三方的 Java 元组库，这个库目前实现了最多十个元组，如果要实现常用的三元组或者四元组可以使用这个组件，不过更多元组的使用场景并不多，而且也不是很灵活，还是乖乖自定义实体吧。<br />感兴趣的朋友可以自己试试这个组件，在 pom 文件中加入如下依赖
```xml
<dependency>
	<groupId>org.javatuples</groupId>
	<artifactId>javatuples</artifactId>
	<version>1.2</version>
</dependency>
```
其中各个数量的元组类名称如下，不得不说十元组的数量已经不算少了，一般这种情况还是使用实体来的方便。
```java
Unit<A> (1 element)
Pair<A,B> (2 elements)
Triplet<A,B,C> (3 elements)
Quartet<A,B,C,D> (4 elements)
Quintet<A,B,C,D,E> (5 elements)
Sextet<A,B,C,D,E,F> (6 elements)
Septet<A,B,C,D,E,F,G> (7 elements)
Octet<A,B,C,D,E,F,G,H> (8 elements)
Ennead<A,B,C,D,E,F,G,H,I> (9 elements)
Decade<A,B,C,D,E,F,G,H,I,J> (10 elements)
```
