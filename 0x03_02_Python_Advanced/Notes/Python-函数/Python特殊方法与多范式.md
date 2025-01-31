Python 特殊方法与多范式<br />Python一切皆对象，但同时，Python还是一个多范式语言(multi-paradigm)，不仅可以使用面向对象的方式来编写程序，还可以用面向过程的方式来编写相同功能的程序(还有函数式、声明式等)。Python的多范式依赖于Python对象中的特殊方法(special method)。<br />特殊方法名的前后各有两个下划线。特殊方法又被成为魔法方法(magic method)，定义了许多Python语法和表达方式，正如在下面的例子中将要看到的。当对象中定义了特殊方法的时候，Python也会对它们有“特殊优待”。比如定义了`__init__()`方法的类，会在创建对象的时候自动执行`__init__()`方法中的操作。
:::info
可以通过`dir()`来查看对象所拥有的特殊方法，比如`dir(1)`
:::
<a name="HcXOP"></a>
### 运算符
Python的运算符是通过调用对象的特殊方法实现的。比如：
```python
'abc' + 'xyz'               # 连接字符串
```
实际执行了如下操作：
```python
'abc'.__add__('xyz')
```
所以，在Python中，两个对象是否能进行加法运算，首先就要看相应的对象是否有`__add__()`方法。一旦相应的对象有`__add__()`方法，即使这个对象从数学上不可加，都可以用加法的形式，来表达`obj.__add__()`所定义的操作。在Python中，运算符起到简化书写的功能，但它依靠特殊方法实现。<br />Python不强制用户使用面向对象的编程方法。用户可以选择自己喜欢的使用方式(比如选择使用+符号，还是使用更加面向对象的`__add__()`方法)。特殊方法写起来总是要更费事一点。<br />尝试下面的操作，看看效果，再想想它的对应运算符
```python
(1.8).__mul__(2.0)
True.__or__(False)
```
<a name="MFuKN"></a>
### 内置函数
与运算符类似，许多内置函数也都是调用对象的特殊方法。比如
```python
len([1,2,3])      # 返回表中元素的总数
```
实际上做的是
```python
[1,2,3].__len__()
```
相对与`__len__()`，内置函数`len()`也起到了简化书写的作用。<br />尝试下面的操作，想一下它的对应内置函数
```python
(-1).__abs__()
(2.3).__int__()
```
<a name="HQCCt"></a>
### 表(list)元素引用
下面是常见的表元素引用方式
```python
li = [1, 2, 3, 4, 5, 6]print(li[3])
```
上面的程序运行到li[3]的时候，Python发现并理解`[]`符号，然后调用`__getitem__()`方法。
```python
li = [1, 2, 3, 4, 5, 6]print(li.__getitem__(3))
```
尝试看下面的操作，想想它的对应
```python
li.__setitem__(3, 0)
{'a':1, 'b':2}.__delitem__('a')
```
<a name="LQP7j"></a>
### 函数
在Python中，函数也是一种对象。实际上，任何一个有`__call__()`特殊方法的对象都被当作是函数。比如下面的例子:
```python
class SampleMore(object):
    def __call__(self, a):
        return a + 5
add = SampleMore()     # A function object
print(add(2))          # Call function    
map(add, [2, 4, 5])    # Pass around function object
```
`add`为SampleMore类的一个对象，当被调用时，add执行加5的操作。add还可以作为函数对象，被传递给`map()`函数。
<a name="Hijop"></a>
### 总结
对于内置的对象来说(比如整数、表、字符串等)，它们所需要的特殊方法都已经在Python中准备好了。而用户自己定义的对象也可以通过增加特殊方法，来实现自定义的语法。特殊方法比较靠近Python的底层，许多Python功能的实现都要依赖于特殊方法。
