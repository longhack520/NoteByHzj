# 原创
：  [数字媒体] Photoshop基础之图像校正、抠图（证件照）和融合

# [数字媒体] Photoshop基础之图像校正、抠图（证件照）和融合

这篇博客是数字媒体系列的笔记，仅作为在线笔记供大家学习，后续有时间会深入分享PS、视频制作、动画制作等内容。希望您喜欢~

#### 文章目录

---


## 一.Excel证件照换背景

大家在找工作或面试过程中，通常会遇到证件照替换背景的问题，下面介绍Excel替换背景的一种简单方法。

> 
大家试试Python编写代码，也挺有意思的。


假设存在一张图像，如下所示：

**第一步，新建WPS Excel文件，然后导入图片。**

如下图所示：

**第二步，使用鼠标选中证件照，选择上方的“图片工具”=&gt;“抠除背景”=&gt;“智能抠除背景”。**

处理效果如下图所示：

**第三步，选中图片右键“设置对象格式”。**

**第四步，在“填充与线条”中选择合适的背景颜色即可。**

最终效果如下图所示：

---


## 二.PS图像校正

假设存在如下所示的一张倾斜图片，我们需要对其进行校正。

**第一步，文件打开图像或直接拖动图像至PS。**

**第二步，选择“透视裁剪工具”。**

**第三步，选中要校正的图像，如下图所示的奖状。**

**第四步，分别移动四个角，然后点击上面的√。按住Alt键可以缩放图像大小。**

最终实现了图像校正，再去掉背景保存图片即可。

---


## 三.PS抠图

抠图和替换背景颜色是常见的PS用法，如下图所示，我们需要扣取中间的奖章并抠图，再与其它图片进行融合。怎么实现呢？

**第一步，按照第一部分图像校正，提取图中的奖章。**

显示效果如下图所示：

**第二步，选择“魔术橡皮擦工具”，然后点击需要驱除背景颜色的区域。**

处理后的效果如下图所示：

**第三步，选择“背景橡皮擦工具”，然后处理图像的细节部分。**

注意，右键我们可以设置橡皮擦处理图像的大小。

**第四步，按住ALT键，然后将有污渍的背景进行祛除，Ctrl+Z可以撤销错误操作。**

输出结果如下图所示：

**第五步，如果边缘存在噪声，可以使用模糊工具处理。**

**第六步，保存透明背景图像。**

最终结果如下图所示：

---


## 四.PS图像融合

接着我们想将两张图像融合，比如将上面的奖章融合到另一张图像中。

**第一步，导入第一张图像（直接拖动）。**

**第二步，将第二张图像导入如“徽章”（背景透明），可以看到右下角它在另外一个图层。**

**第三步，调整图像大小和位置，按下回车键即可显示效果。**

显示效果如下图所示：

**第四步，调整图像的对比度。**

调整亮度和对比度，让融合的图像色调尽量一致。

最终实现图像融合并保存图像。

---


## 五.总结

希望这篇文章对您有所帮助，如果喜欢，后面也会分享更多“数字媒体”相关的博客。最后祝大家在读博和科研的路上不断前行。项目学习再忙，也要花点时间读论文和思考，加油！这篇文章就写到这里，希望对您有所帮助。由于作者英语实在太差，论文的水平也很低，写得不好的地方还请海涵和批评。同时，也欢迎大家讨论，继续努力！感恩遇见，且看且珍惜。

哈哈，其实我是来秀CSDN奖状的。2022年即将离去，十月份会异常忙碌，加油！秀璋~下图其实空白处越多越忙碌，感恩前行。

(By:Eastmount 2022-09-27 周一夜于武汉 http://blog.csdn.net/eastmount/ )
