IDEA 快捷键<br />Mac 键盘符号和修饰键说明：<br />⌘ ——> Command<br />⇧ ——> Shift<br />⌥ ——> Option<br />⌃ ——> Control<br />↩︎ ——> Return/Enter<br />⌫ ——> Delete<br />⌦ ——> 向前删除键(Fn + Delete)<br />↑ ——> 上箭头<br />↓ ——> 下箭头<br />← ——> 左箭头<br />→ ——> 右箭头<br />⇞ ——> Page Up(Fn + ↑)<br />⇟ ——> Page Down(Fn + ↓)<br />⇥ ——> 右制表符(Tab键)<br />⇤ ——> 左制表符(Shift + Tab)<br />⎋ ——> Escape(Esc)<br />End ——> Fn + →<br />Home ——> Fn + ←
<a name="zXPRX"></a>
## 1、Editing（编辑）
Control + Space    基本的代码补全（补全任何类、方法、变量）<br />Control + Shift + Space    智能代码补全（过滤器方法列表和变量的预期类型）<br />Command + Shift + Enter    自动结束代码，行末自动添加分号<br />Command + P    显示方法的参数信息<br />Control + J    快速查看文档<br />Shift + F1    查看外部文档（在某些代码上会触发打开浏览器显示相关文档）<br />Command + 鼠标放在代码上    显示代码简要信息<br />Command + F1    在错误或警告处显示具体描述信息<br />Command + N, Control + Enter, Control + N    生成代码（getter、setter、hashCode、equals、toString、构造函数等）<br />Control + O    覆盖方法（重写父类方法）<br />Control + I    实现方法（实现接口中的方法）<br />Command + Option + T    包围代码（使用if...else、try...catch、for、synchronized等包围选中的代码）<br />Command + /    注释 / 取消注释与行注释<br />Command + Option + /    注释 / 取消注释与块注释<br />Option + 方向键上    连续选中代码块<br />Option + 方向键下    减少当前选中的代码块<br />Control + Shift + Q    显示上下文信息<br />Option + Enter    显示意向动作和快速修复代码<br />Command + Option + L    格式化代码<br />Control + Option + O    优化 import<br />Control + Option + I    自动缩进线<br />Tab / Shift + Tab    缩进代码 / 反缩进代码<br />Command + X    剪切当前行或选定的块到剪贴板<br />Command + C    复制当前行或选定的块到剪贴板<br />Command + V    从剪贴板粘贴<br />Command + Shift + V    从最近的缓冲区粘贴<br />Command + D    复制当前行或选定的块<br />Command + Delete    删除当前行或选定的块的行<br />Control + Shift + J    智能的将代码拼接成一行<br />Command + Enter    智能的拆分拼接的行<br />Shift + Enter    开始新的一行<br />Command + Shift + U    大小写切换<br />Command + Shift + ] / Command + Shift + [    选择直到代码块结束 / 开始<br />Option + Fn + Delete    删除到单词的末尾<br />Option + Delete    删除到单词的开头<br />Command + 加号 / Command + 减号    展开 / 折叠代码块<br />Command + Shift + 加号    展开所以代码块<br />Command + Shift + 减号    折叠所有代码块<br />Command + W    关闭活动的编辑器选项卡
<a name="ejo0G"></a>
## 2、Search / Replace（查询/替换）
Double Shift    查询任何东西<br />Command + F    文件内查找<br />Command + G    查找模式下，向下查找<br />Command + Shift + G    查找模式下，向上查找<br />Command + R    文件内替换<br />Command + Shift + F    全局查找（根据路径）<br />Command + Shift + R    全局替换（根据路径）<br />Command + Shift + S    查询结构（Ultimate Edition 版专用，需要在 Keymap 中设置）<br />Command + Shift + M    替换结构（Ultimate Edition 版专用，需要在 Keymap 中设置）
<a name="DwmL1"></a>
## 3、Usage Search（使用查询）
Option + F7 / Command + F7    在文件中查找用法 / 在类中查找用法<br />Command + Shift + F7    在文件中突出显示的用法<br />Command + Option + F7    显示用法
<a name="giMAD"></a>
## 4、Compile and Run（编译和运行）
Command + F9    编译 Project<br />Command + Shift + F9    编译选择的文件、包或模块<br />Control + Option + R    弹出 Run 的可选择菜单<br />Control + Option + D    弹出 Debug 的可选择菜单<br />Control + R    运行<br />Control + D    调试<br />Control + Shift + R, Control + Shift + D    从编辑器运行上下文环境配置
<a name="cW0UP"></a>
## 5、Debugging（调试）
F8    进入下一步，如果当前行断点是一个方法，则不进入当前方法体内<br />F7    进入下一步，如果当前行断点是一个方法，则进入当前方法体内，如果该方法体还有方法，则不会进入该内嵌的方法中<br />Shift + F7    智能步入，断点所在行上有多个方法调用，会弹出进入哪个方法<br />Shift + F8    跳出<br />Option + F9    运行到光标处，如果光标前有其他断点会进入到该断点<br />Option + F8    计算表达式（可以更改变量值使其生效）<br />Command + Option + R    恢复程序运行，如果该断点下面代码还有断点则停在下一个断点上<br />Command + F8    切换断点（若光标当前行有断点则取消断点，没有则加上断点）<br />Command + Shift + F8    查看断点信息
<a name="BtNRz"></a>
## 6、Navigation（导航）
Command + O    查找类文件<br />Command + Shift + O    查找所有类型文件、打开文件、打开目录，打开目录需要在输入的内容前面或后面加一个反斜杠/<br />Command + Option + O    前往指定的变量 / 方法<br />Control + 方向键左 / Control + 方向键右    左右切换打开的编辑 tab 页<br />F12    返回到前一个工具窗口<br />Esc    从工具窗口进入代码文件窗口<br />Shift + Esc    隐藏当前或最后一个活动的窗口，且光标进入代码文件窗口<br />Command + Shift + F4    关闭活动 run/messages/find/... tab<br />Command + L    在当前文件跳转到某一行的指定处<br />Command + E    显示最近打开的文件记录列表<br />Option + 方向键左 / Option + 方向键右    光标跳转到当前单词 / 中文句的左 / 右侧开头位置<br />Command + Option + 方向键左 / Command + Option + 方向键右    退回 / 前进到上一个操作的地方<br />Command + Shift + Delete    跳转到最后一个编辑的地方<br />Option + F1    显示当前文件选择目标弹出层，弹出层中有很多目标可以进行选择(如在代码编辑窗口可以选择显示该文件的 Finder)<br />Command + B / Command + 鼠标点击    进入光标所在的方法/变量的接口或是定义处<br />Command + Option + B    跳转到实现处，在某个调用的方法名上使用会跳到具体的实现处，可以跳过接口<br />Option + Space, Command + Y    快速打开光标所在方法、类的定义<br />Control + Shift + B    跳转到类型声明处<br />Command + U    前往当前光标所在方法的父类的方法 / 接口定义<br />Control + 方向键下 / Control + 方向键上    当前光标跳转到当前文件的前一个 / 后一个方法名位置<br />Command + ] / Command + [    移动光标到当前所在代码的花括号开始 / 结束位置<br />Command + F12    弹出当前文件结构层，可以在弹出的层上直接输入进行筛选（可用于搜索类中的方法）<br />Control + H    显示当前类的层次结构<br />Command + Shift + H    显示方法层次结构<br />Control + Option + H    显示调用层次结构<br />F2 / Shift + F2    跳转到下一个 / 上一个突出错误或警告的位置<br />F4 / Command + 方向键下    编辑 / 查看代码源<br />Option + Home    显示到当前文件的导航条<br />F3    选中文件 / 文件夹 / 代码行，添加 / 取消书签<br />Option + F3    选中文件 / 文件夹/代码行，使用助记符添加 / 取消书签<br />Control + 0…Control + 9    定位到对应数值的书签位置<br />Command + F3    显示所有书签
<a name="BC70K"></a>
## 7、Refactoring（重构）
F5    复制文件到指定目录<br />F6    移动文件到指定目录<br />Command + Delete    在文件上为安全删除文件，弹出确认框<br />Shift + F6    重命名文件<br />Command + F6    更改签名<br />Command + Option + N    一致性<br />Command + Option + M    将选中的代码提取为方法<br />Command + Option + V    提取变量<br />Command + Option + F    提取字段<br />Command + Option + C    提取常量<br />Command + Option + P    提取参数
<a name="ZQ39b"></a>
## 8、VCS / Local History（版本控制 / 本地历史记录）
Command + K    提交代码到版本控制器<br />Command + T    从版本控制器更新代码<br />Option + Shift + C    查看最近的变更记录<br />Control + C    快速弹出版本控制器操作面板
<a name="W8s8H"></a>
## 9、Live Templates（动态代码模板）
Command + Option + J    弹出模板选择窗口，将选定的代码使用动态模板包住<br />Command + J    插入自定义动态代码模板
<a name="sYjeT"></a>
## 10、General（通用）
Command + 1…Command + 9    打开相应编号的工具窗口<br />Command + S    保存所有<br />Command + Option + Y    同步、刷新<br />Control + Command + F    切换全屏模式<br />Command + Shift + F12    切换最大化编辑器<br />Option + Shift + F    添加到收藏夹<br />Option + Shift + I    检查当前文件与当前的配置文件<br />Control + `    快速切换当前的 scheme（切换主题、代码样式等）<br />Command + ,    打开 IDEA 系统设置<br />Command + ;    打开项目结构对话框<br />Shift + Command + A    查找动作（可设置相关选项）<br />Control + Shift + Tab    编辑窗口标签和工具窗口之间切换（如果在切换的过程加按上 delete，则是关闭对应选中的窗口）
