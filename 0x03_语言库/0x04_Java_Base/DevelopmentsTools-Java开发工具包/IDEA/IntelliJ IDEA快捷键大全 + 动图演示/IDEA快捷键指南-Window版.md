IDEA 快捷键

- Ctrl + Shift + A
   - 说明：IDEA 把所有的可以执行的操作都定义为action，也就是一个动作。它定义的快捷键相当多，所以使用此快捷键，不需要记住快捷键，只需要输入想要的操作的英文，就可以迅速找到要执行的操作。
   - 说明：勾选 Include non-project items 后，搜索范围将包括依赖的jar包

下文的快捷键全是基于 windows 平台的。
<a name="dkWvs"></a>
### 查看源码

- Ctrl + E
   - 说明：打开当前项目最近浏览的文件列表
- Ctrl + Alt + 左右键
   - 说明：加←键，表示跳转到当前项目上一次浏览的文件
   - 说明：加→键，表示跳转到当前项目下一次浏览的文件
- Ctrl + N
   - 说明：默认输入类名，在当前项目中搜索指定类；
   - 说明：勾选 Include non-project items 后，搜索范围将包括依赖的jar包
- Ctrl + Shift + N
   - 说明：默认输入文件名(包含后缀)，在当前项目中搜索指定文件；
   - 说明：勾选 Include non-project items 后，搜索范围将包括依赖的jar包
- Ctrl + Alt + Shift + N
   - 说明：默认输入函数名/变量名，在当前项目中搜索指定函数/变量；
   - 说明：勾选 Include non-project items 后，搜索范围将包括依赖的jar包
- Ctrl + Shift + F
   - 说明：全局搜索某字符串；可指定是否匹配大小写，是否是单词，是否启用正则表达式，限定搜索的文件类型
   - 使用：在弹出的搜索框输入关键字即可
- Ctrl + F12
   - 说明：列表展示当前类的 变量f 和 方法m
- Ctrl + Alt + Shift + U
   - 此处使用 Ctrl + F 可以搜索定位某个依赖在结构关系图中的位置；
   - 双击该依赖节点可以跳转到该依赖定义的地方；
   - Shift + Del 可以删除该依赖与它上一个节点的连接关系，也就是 <exclusion></exclusion>。
   - 说明：查看当前类所涉及的方法之间的结构关系图
   - 说明：查看 pom 文件中引用的 jar 包之间的结构关系图；
- Alt + F7
   - 说明：查看光标停留的 方法/变量 在何处被调用
   - 使用：选中需要查询的类名或方法名等
- Ctrl + H
   - 说明：查看当前方法被谁调用/它调用的谁
<a name="nFrdZ"></a>
### 做记号

- F11
   - 说明：给当前光标停留的这行代码加上书签
- Ctrl + F11
   - 说明：给当前光标停留的这行代码加上书签，并编号
- Ctrl + 数字/字母
   - 说明：对已经编号的书签使用 Ctrl + 数字/字母 组合键，可以快速跳转到对应标签
- Shift + F11
   - 说明：打开所有书签列表
- Alt + Shift + F
   - 说明：收藏
   - 使用：当光标放在类名时，收藏的是当前类；当光标放在函数方法名时，收藏的是当前函数。默认是收藏到当前项目名的收藏夹下面，当让也可以自己新增收藏夹。
<a name="Yj9dk"></a>
### 写代时，切换相当方便

- 打开多个窗口的项目工程的时候用到
   - Ctrl + Alt + ] 切换到下一个工程
   - Ctrl + Alt + [ 切换到上一个工程
- Ctrl + Shift + E
   - 说明：打开当前项目最近修改的文件列表
- Alt + Shift + C
   - 说明：打开当前项目最近执行的操作列表；并且点击相应操作能查看对哪些文件作了修改
- Ctrl + Shift + Backspace
   - 说明：跳转到当前项目上一次编辑的文件
<a name="CUOWt"></a>
### 编辑代码必备

- Alt + window 编号
   - 说明：跳转到窗口区
- Esc
   - 返回代码编辑区
- Shift + F6
   - 说明：全局修改(重构) 类名/变量名
   - 使用：在原始创建的类中选中所需修改的类名（在引用处不可修改）
- Shift + Alt + u
   - 说明：将不是驼峰格式的命名自动转化驼峰；
   - 使用：需要IDEA安装CamelCase插件
- Ctrl + Shift + u
   - 说明：将选中的单词全部转换成大写或者小写
   - 使用：不需要安装插件
- Alt + Enter
   - 说明：智能提示：导包、自动修正、自动创建函数、实现接口、单词拼写、list replace、字符串format或者build……
   - 使用：根据提示进行选择对应动作操作
- Shift + Enter
   - 说明：在光标停留行的下一行，添加一空白行
- Ctrl + Enter
   - 说明：在光标停留行的当前行，添加一空白行
- Ctrl + Shift + Space
   - 说明：自动补全代码
   - 使用：在未打全的代码后使用快捷键补全代码; 如果上下文没有可以补全的，按两次会出现代码建议提示。
- Ctrl + 左右键
   - 说明：默认以最近的一个单词为准，将光标移动到该单词首或末尾，
- Ctrl + Shift + 左右键
   - 说明：选中最近的一个或多个单词
   - 使用：将光标移动到需要选中的单词前或后，例如：按住Ctrl + Shift + 左键，即向左一个单词一个单词的累加选中
- Alt + Shift + 上下键
   - 说明：将光标所在行代码向上/下移动；或者将光标选中的代码块所在的多行代码向上/下移动
- Ctrl + D
   - 说明：复制当前行到下一行
   - 使用：将光标移动到需要复制的行,即可使用
- 将多次用到的数据抽取出来定义
   - Ctrl + Alt + V
   - 说明：抽取出来定义成一个普通变量
   - Ctrl + Alt + C
   - 说明：抽取出来定义成一个静态变量
   - Ctrl + Alt + F
   - 说明：抽取出来定义成一个成员变量
   - Ctrl + Alt + P
   - 说明：抽取变量出来定义成方法的参数
   - Ctrl + Alt + M
   - 说明：抽取出来定义成一个新的方法
- Ctrl + Alt + L
   - 说明：格式化代码
   - 使用：选中需要格式化的代码部分使用快捷键
- Alt + 左右键
   - 说明：加←键，表示跳转到编辑区打开的文件列表的左边一个文件（相对光标当前停留的文件而言）
   - 说明：加→键，表示跳转到编辑区打开的文件列表的右边一个文件
- Alt + 上下键
   - 说明：加↑键，表示跳转到下一个方法
   - 说明：加↓键，表示跳转到上一个方法
- Ctrl + Alt + Shift + J
   - 说明：选中当前文件中，所有相同的字符串
- F2
   - 说明：直接将光标定位到报红提示的地方
