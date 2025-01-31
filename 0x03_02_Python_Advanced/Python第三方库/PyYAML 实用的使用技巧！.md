PythonYaml<br />YAML 是一个被广泛使用的数据序列化和配置语言，作为一个开发者，总是不免和它打交道。但处理 YAML 文档，尤其是使用 PyYAML 的过程总是非常痛苦。<br />分享在 Python 下使用 PyYAML 的技巧和代码片段，并介绍几个相关的库。<br />注意：本文中的代码仅保证在 Python 3 下正常工作
<a name="WkobK"></a>
## 总是使用 `safe_load`/`safe_dump`
PyYAML 的 load 函数可以构造任意 Python 对象（Pickle 协议），这意味着一次 load 可能导致任意 Python 函数被执行。<br />为了确保应用程序的安全性，尽量在任何情况下使用 `yaml.safe_load` 和 `yaml.safe_dump`。
<a name="Cv95z"></a>
## 保留字段顺序
Python 3.7+ 中，dict keys 具备保留插入顺序的特性，所以通过 `yaml.safe_load` 得到的 dict，其 keys 顺序会与原始文件保持一致。
```python
>>> import yaml
>>> text = """---
... c: 1
... b: 1
... d: 1
... a: 1
... """
>>> d = yaml.safe_load(text)
>>> d
{'c': 1, 'b': 1, 'd': 1, 'a': 1}
>>> list(d)
['c', 'b', 'd', 'a']
```
当把 `dict` 导出为 YAML 字符串时，为 `yaml.safe_dump` 传递 `sort_keys=False` 来保留 keys 的顺序。
```python
>>> print(yaml.safe_dump(d))
a: 1
b: 1
c: 1
d: 1
>>> d['e'] = 1
>>> print(yaml.safe_dump(d, sort_keys=False))
c: 1
b: 1
d: 1
a: 1
e: 1
```
如果 Python 版本较低，或者想确保代码能在更广泛的环境下工作，可以使用 oyaml 库来代替 PyYAML 的 yaml 包。
```python
>>> import oyaml as yaml
>>> d = yaml.safe_load(text)
>>> d
OrderedDict([('c', 1), ('b', 1), ('d', 1), ('a', 1)])
>>> d['e'] = 1
>>> print(yaml.safe_dump(d, sort_keys=False))
c: 1
b: 1
d: 1
a: 1
e: 1
```
<a name="eZVh6"></a>
## 优化列表项的缩进
默认情况下，PyYAML 输出的列表缩进与其父元素一致。
```python
>>> d = {'a': [1, 2, 3]}
>>> print(yaml.safe_dump(d))
a:
- 1
- 2
- 3
```
这并不是很好的格式，根据 Ansible 和 HomeAssistant 等 YAML 书写规范，列表项应该缩进 2 空格。<br />这种格式也会对导致列表项不会被如 VSCode 等编辑器识别，进而无法使用编辑器的折叠功能。<br />要解决这个问题，使用如下代码片段，在代码中定义 IndentDumper class:
```python
class IndentDumper(yaml.Dumper):
    def increase_indent(self, flow=False, indentless=False):
        return super(IndentDumper, self).increase_indent(flow, False)
```
然后将它传递给 `yaml.dump` 的 Dumper 关键字参数。
```python
>>> print(yaml.dump(d, Dumper=IndentDumper))
a:
  - 1
  - 2
  - 3
```
注意，`yaml.safe_dump` 由于有自己的 Dumper class，传递此参数会造成冲突。
<a name="z0ajd"></a>
## 输出可读的 UTF-8 字符
默认情况下，PyYAML 假设希望输出的结果里只有 ASCII 字符。
```python
>>> d = {'a': '你好'}
>>> print(yaml.safe_dump(d))
a: "\u4F60\u597D"
```
这会让输出结果非常难以阅读。<br />在 UTF-8 足够普及的今天，直接输出 UTF-8 字符是非常安全的。因此可以将 `allow_unicode=True` 传入 `yaml.safe_dump` 使 PyYAML 将 Unicode 转换成 UTF-8 字符串。
```python
>>> print(yaml.safe_dump(d, allow_unicode=True))
a: 你好
```
<a name="EWPko"></a>
## 一些 YAML 相关的库
<a name="NtQYv"></a>
### oyaml
Link: [https://github.com/wimglenn/oyaml](https://github.com/wimglenn/oyaml)<br />正如上文中提到的，oyaml 是 yaml 包的替换品，使 dict keys 的顺序在 dump/load 的时候得以保留。<br />oyaml 是一个单文件库，只有 53 行代码，因此使用起来非常灵活，可以直接把它的代码复制到自己的项目中，然后根据自己的需求进行修改。
<a name="twtzS"></a>
### strictyaml
Link: [https://github.com/crdoconnor/strictyaml](https://github.com/crdoconnor/strictyaml)<br />有的人说 YAML 过于复杂和灵活，不是一个好的配置语言。但这不是 YAML 的问题，而是使用方式的问题。如果限制程序只使用 YAML 的部分功能，YAML 其实可以变得像它设计的那般好用。<br />这就是 StrictYAML 的设计意图，它是一个类型安全的 YAML 解析器，实现了 YAML 规范说明中的一个子集 。<br />如果对 YAML 的输入输出有较强的安全考虑，建议使用 StrictYAML 代替 PyYAML。<br />顺带一提的是，StrictYAML 的文档站有很多关于设计细节和配置语言思考的文章，非常值得一看。
<a name="Nlylz"></a>
### ruamel.yaml
Link: [https://yaml.readthedocs.io/en/latest/overview.html](https://yaml.readthedocs.io/en/latest/overview.html)<br />ruamel.yaml 是 PyYAML 的一个分叉，于 2009 年发布并持续维护至今。<br />ruamel.yaml 的文档里详细说明了它和 PyYAML 的差异。总体来说，ruamel.yaml 专注在 YAML 1.2 上，对一些语法进行了优化。<br />ruamel.yaml 最令人感兴趣的特性是输入输出的 “round-trip”，可以最大程度地保留输入源的原始格式。官方文档中的定义是这样的：
> A round-trip is a YAML load-modify-save sequence and ruamel.yaml tries to preserve, among others:
> - comments
> - block style and key ordering are kept, so you can diff the round-tripped source
> - flow style sequences ( ‘a: b, c, d’) (based on request and test by Anthony Sottile)
> - anchor names that are hand-crafted (i.e. not of the formidNNN)
> - merges in dictionaries are preserved

如果有尽可能保留原始格式的需求，建议使用 ruamel.yaml 代替 PyYAML。<br />在使用中注意到 ruamel.yaml 的 safe load 方法 (`YAML(typ='safe').load`) 与 PyYAML 有些不同，它无法解析 flow style 的集合定义 (如 a: {"foo": "bar"})，这点没有在文档中提及，使用时须多加注意。
<a name="EtDFb"></a>
## 总结
YAML 有它好的地方和坏的地方。它易于阅读，初期的学习曲线非常平缓。但 YAML 的规范说明非常复杂，不仅造成了使用中的混乱，也使不同语言的实现在很多细微的地方难以保持一致。
