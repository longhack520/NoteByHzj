Python Print 函数<br />学过Python的人应该对print函数再熟悉不过了，几乎每一个学Python的人写下的第一行代码都是：
```python
print('hello world')
```
通过对上面这行代码的学习，其实就掌握了print函数的关键用法，在print后面的括号里面写明要打印的内容就好，然后运行代码就会在电脑上打印出来要打印的内容。<br />除了这个关键的用法以外，print函数还有一些其他的参数，虽然不是很常用，但是有的时候也会用到。
<a name="AhqK8"></a>
### 打印多个内容时的分隔符
上面打印的hello world时一次只打印一个字符，如果要一次性打印多个字符时，可以用逗号把要打印的多个字符隔开就可以，比如：
```python
print('hello world','i am python')
---
hello world i am python
```
运行上面的代码会得到多个字符之间都是用空格来区分的，看起来很混乱，这个时候就可以通过指定分割符的方式来对多个字符进行分割，实现方式是通过sep参数，具体如下：
```python
print('hello world','i am python',sep = ',')
---
hello world, i am python
```
除了可以用逗号分割以外，还可以用其他任何想要的符号在多个字符之间进行分割。
<a name="BdRQ8"></a>
### 结尾符
结尾符是用来指明要打印出出来的print字符以什么结尾，默认是以'\n'结尾的，也就是换行符，什么意思呢？就是每运行完一个print以后，都会重新换一行。比如下面这样：
```python
print(1)
print(2)
print(3)
```
运行上面的代码会得到如下结果：
```
1
2
3
```
这是因为使用了默认的换行结尾符，如果不想让他使用默认的结尾符，那就可以用end参数来指明想要的结尾符，比如使用空的结尾符：
```python
print(1,end = '')
print(2,end = '')
print(3,end = '')
```
运行上面的代码会得到如下结果：
```
123
```
这是因为使用了空结尾符，所以不同print的结果就打印在了一起，也可以使用其他结尾符，比如逗号：
```python
print(1,end = ',')
print(2,end = ',')
print(3,end = ',')
```
运行上面的代码得到如下结果：
```
1,2,3,
```
这样多个print之间就用逗号分割开了。<br />其实除了分割符和结尾符以外，还有两个参数file和flush，但是这两个参数更不常用了，所以就不展开了。
