Python
<a name="UAehL"></a>
### argparse
Python当中的argparse模块主要用于命令行的参数解析，可以帮助用户轻松地编写命令行接口，先来看一个例子
```python
import argparse

# 解析参数
parser = argparse.ArgumentParser()
parser.add_argument("name")
args = parser.parse_args()

# 打印结果
print(f'Hello {args.name}!')
```
然后在终端当中运行以下的代码
```python
python python_package.py Fcant
```
就会出现以下的结果
```python
Hello Fcant!
```
要是忘记带上参数了，会自动出现如下的提示
```python
usage: python_package.py [-h] name
python_package.py: error: the following arguments are required: name
```
当然也可以通过如下的命令行来查看需要添加什么样的参数
```python
python python_package.py -h
```
出来的结果如下所示
```python
usage: python_package.py [-h] name

positional arguments:
  name

optional arguments:
  -h, --help  show this help message and exit
```
当然并不知道这个name的参数到底指的是什么，可以更改一下写的程序
```python
import argparse

# 解析参数
parser = argparse.ArgumentParser()
parser.add_argument("name", help="Enter your name")
args = parser.parse_args()

print(f'Hello {args.name}!')
```
这样的话，再来运行一下如下的命令行
```python
python python_package.py -h
```
output
```python
usage: python_package.py [-h] name

positional arguments:
  name        Enter your name

optional arguments:
  -h, --help  show this help message and exit
```
有时候想要输入的不止一个参数，可以这样来做，
```python
import argparse

# 解析参数
parser = argparse.ArgumentParser()
parser.add_argument("name", help="Enter your name")
parser.add_argument("age", help='Enter your age', type=int)
args = parser.parse_args()

born_year = 2022 - args.age
print(f'Hello {args.name}! You were borned in {born_year}.')
```
通过终端输入如下的程序
```python
python python_package.py Fcant 24
```
output
```python
Hello Fcant! You were borned in 1998.
```
<a name="nrT2U"></a>
### shutil
`shutil`模块提供了大量的文件高级操作。特别是针对文件的拷贝、删除、移动、压缩和解压缩等操作，先来看一个例子
```python
import shutil
print(shutil.which("python"))
```
output
```python
路径......
```
上面返回的是Python可执行程序的路径，文件移动的代码是`shutil.move(src, dst)`
```python
shutil.move("源路径", "目标路径")
```
除此之外主要会用到的还有：

- `shutil.copyfile(src, dst)`：复制文件
- `shutil.copytree(olddir, newdir, True/False)`：复制整个文件夹目录
- `shutil.rmtree(src)`：递归删除一整个目录以及目录文件夹下的所有内容
<a name="hZRUE"></a>
### glob
glob模块主要是用来查找符合特定规则的目录和文件，并将查找出来的结果返回到一个列表当中来。它还可以和正则通配符一起来使用，例如
```python
def choose_numbered_files(root="."):
    return glob.glob(f"{root}/[0-9].*")
    
choose_numbered_files("images")
```
返回的是在images路径下的带有数字的文件，结果如下
```python
['images\\1.gif',
 'images\\1.png',
 'images\\2.gif',
 'images\\2.png',
 'images\\3.png',]
```
上面用到的`glob.glob()`返回的是符合匹配条件的所有文件的路径，而`glob.iglob()`返回的是一个迭代对象，需要循环遍历获取每个元素之后得到符合匹配条件的所有文件的路径。
<a name="YrFUh"></a>
### pprint
`pprint`模块提供了“美化打印”任意Python数据结构的功能，方便使用者阅读，要是用普通的`print`来打印的话，如下
```python
nested = [list("abcs"), list("sdff"), [1, 45, 4, 6, 7, 8], list(range(12))]
print(nested)
```
output
```python
[['a', 'b', 'c', 's'], ['s', 'd', 'f', 'f'], [1, 45, 4, 6, 7, 8], [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11]]
```
而用`pprint`模块来打印的话，如下
```python
from pprint import pprint
pprint(nested)
```
output
```python
[['a', 'b', 'c', 's'],
 ['s', 'd', 'f', 'f'],
 [1, 45, 4, 6, 7, 8],
 [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11]]
```
还能够对键值对类型的数据进行格式化的输出，代码如下
```python
import json
import pprint
from urllib.request import urlopen

with urlopen('https://pypi.org/pypi/sampleproject/json') as resp:
    project_info = json.load(resp)['info']

pprint.pprint(project_info)
```
output
```python
{'author': 'A. Random Developer',
 'author_email': 'author@example.com',
 'bugtrack_url': None,
 'classifiers': [.......],
 'description': [.......],
 'description_content_type': 'text/markdown',
 'docs_url': None,
 'download_url': '',
 'downloads': {.........},
 'home_page': 'https://github.com/pypa/sampleproject',
 'keywords': 'sample setuptools development',
 'license': '',
 'maintainer': '',
 'maintainer_email': '',
 'name': 'sampleproject',
 'package_url': 'https://pypi.org/project/sampleproject/',
 'platform': '',
 'project_url': 'https://pypi.org/project/sampleproject/',
 'project_urls': ..........,
 'requires_dist': [..........],
 'requires_python': '>=3.5, <4',
 'summary': 'A sample Python project',
 'version': '2.0.0',
 'yanked': False,
 'yanked_reason': None}
```
<a name="mFMAm"></a>
### statistics
Python当中的`statistics`模块提供了更加完善的数据统计操作，例如对中位数的计算就提供了`median_low()`、`median_high()`两种方法，分别来计算数据的低中位数(偶数个样本时取中间两个数的较小者)，代码如下
```python
statistics.median_low([1,3,5,7])
```
output
```python
3
```
和高中位数(偶数个样本时取中间两个数的较大者)，代码如下
```python
statistics.median_high([1,3,5,7])
```
output
```python
5
```
除此之外，例如平均数、众数、标准差、方差等等都能够计算，例如
```python
x1 = statistics.mode([1,1,2,3,4,3,3,3,3])
print(x1)

x2 = statistics.mode(["a","b","c","d","d","a","a",])
print(x2)
```
output
```python
3
a
```
<a name="pgGHA"></a>
### calendar
Python当中的日历模块提供了对日期的一系列操作方法，并且可以生成日历，代码如下
```python
import calendar
print(calendar.calendar(2022))
```
output
```python
                                  2022

      January                   February                   March
Mo Tu We Th Fr Sa Su      Mo Tu We Th Fr Sa Su      Mo Tu We Th Fr Sa Su
                1  2          1  2  3  4  5  6          1  2  3  4  5  6
 3  4  5  6  7  8  9       7  8  9 10 11 12 13       7  8  9 10 11 12 13
10 11 12 13 14 15 16      14 15 16 17 18 19 20      14 15 16 17 18 19 20
17 18 19 20 21 22 23      21 22 23 24 25 26 27      21 22 23 24 25 26 27
24 25 26 27 28 29 30      28                        28 29 30 31
31

       April                      May                       June
Mo Tu We Th Fr Sa Su      Mo Tu We Th Fr Sa Su      Mo Tu We Th Fr Sa Su
             1  2  3                         1             1  2  3  4  5
 4  5  6  7  8  9 10       2  3  4  5  6  7  8       6  7  8  9 10 11 12
11 12 13 14 15 16 17       9 10 11 12 13 14 15      13 14 15 16 17 18 19
18 19 20 21 22 23 24      16 17 18 19 20 21 22      20 21 22 23 24 25 26
25 26 27 28 29 30         23 24 25 26 27 28 29      27 28 29 30
                          30 31
......
```
当然也可以打印出某一个月份的日历，代码如下
```python
import calendar
print(calendar.month(2022, 3))
```
output
```python
     March 2022
Mo Tu We Th Fr Sa Su
    1  2  3  4  5  6
 7  8  9 10 11 12 13
14 15 16 17 18 19 20
21 22 23 24 25 26 27
28 29 30 31
```
`calendar.isleap(year)`是闰年则返回`True`，否则返回`False`，例如
```python
import calendar
print(calendar.isleap(2022))
```
output
```python
False
```
