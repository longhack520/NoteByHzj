# 原创
：  [Python图像处理] 三十二.傅里叶变换（图像去噪）与霍夫变换（特征识别）万字详细总结

# [Python图像处理] 三十二.傅里叶变换（图像去噪）与霍夫变换（特征识别）万字详细总结

该系列文章是讲解Python OpenCV图像处理知识，前期主要讲解图像入门、OpenCV基础用法，中期讲解图像处理的各种算法，包括图像锐化算子、图像增强技术、图像分割等，后期结合深度学习研究图像识别、图像分类应用。希望文章对您有所帮助，如果有不足之处，还请海涵~

**前面一篇文章介绍了民族服饰及文化图腾识别，详细讲解图像点运算，包括灰度化处理、灰度线性变换、灰度非线性变换、阈值化处理。这篇文章将讲解两个重要的算法——傅里叶变换和霍夫变换，万字长文整理，希望对您有所帮助。** 同时，该部分知识均为作者查阅资料撰写总结，并且开设成了收费专栏，为小宝赚点奶粉钱，感谢您的抬爱。如果有问题随时私聊我，只望您能从这个系列中学到知识，一起加油喔~

PS：写这篇文章另一个重要的原因是Github资源有作者提交了新的贡献，发现提交的是霍夫变换，因此作者也总结这篇文章。CSDN博客专栏9比1分成，真的挺不错的，也希望大家能分享更好的文章。

**前文参考：**<br/> [[Python图像处理] 一.图像处理基础知识及Op](https://blog.csdn.net/Eastmount/article/details/81748802)
