# 原创
：  [Python图像处理] 三十八.OpenCV图像增强和图像去雾万字详解（直方图均衡化、局部直方图均衡化、自动色彩均衡化）

# [Python图像处理] 三十八.OpenCV图像增强和图像去雾万字详解（直方图均衡化、局部直方图均衡化、自动色彩均衡化）

该系列文章是讲解Python OpenCV图像处理知识，前期主要讲解图像入门、OpenCV基础用法，中期讲解图像处理的各种算法，包括图像锐化算子、图像增强技术、图像分割等，后期结合深度学习研究图像识别、图像分类应用。希望文章对您有所帮助，如果有不足之处，还请海涵~

**前面一篇文章介绍了直方图统计，包括Matplotlib和OpenCV绘制直方图、掩膜直方图、灰度直方图对比及通过直方图预测黑夜白天。这篇文章将详细讲解图像增强知识，从而改善图像质量，增强图像识别效果，核心内容分为直方图均衡化、局部直方图均衡化和自动色彩均衡三部分。万字长文整理，希望对您有所帮助。** 同时，该部分知识均为作者查阅资料撰写总结，并且开设成了收费专栏，为小宝赚点奶粉钱，感谢您的抬爱。如果有问题随时私聊我，只望您能从这个系列中学到知识，一起加油喔~

<mark>代码下载地址（如果喜欢记得star）：</mark>

#### 文章目录
