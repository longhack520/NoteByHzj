Python 函数
<a name="UcDS7"></a>
## 1、函数是对象
![](./img/1634173133453-7d6fd550-c859-47cc-b6b0-753627be669c.webp)<br />这意味着，`yell` 能赋值给 `bark`，如下所示：<br />![](./img/1634173133127-76c9bb8e-7066-47e6-85af-32586c8036c2.webp)<br />而`bark`轻松被赋予`yell`的功能<br />![](./img/1634173133386-1c6963c8-cb88-4794-94be-92c506b9cc47.webp)
<a name="qYvvz"></a>
## 2、删除函数
函数既然是对象，可以删除函数，如下：<br />![](./img/1634173133117-934ee5d5-5945-4ba7-bd66-24543b3d9f59.webp)<br />但是`bark`还是能够正常调用：<br />![](./img/1634173133170-852ac26f-ddda-465c-a67b-a09509fa1866.webp)
<a name="SDrh9"></a>
## 3、这说明：函数名字和函数本身完全是分开的
<a name="TWy5B"></a>
## 4、函数既然是对象，就可以结合数据结构使用
![](./img/1634173133488-fecff865-3d49-4960-9f61-9c4210deedd1.webp)<br />![2021-10-14-09-00-39-990874.png](./img/1634173272489-a2e09371-3885-4adc-a1e9-5a4a74e33864.png)
<a name="iWua5"></a>
## 5、函数既然是对象，就可以作为函数参数
![2021-10-14-09-00-40-152888.png](./img/1634173272488-f800e6e9-173c-46f6-8b33-3b4a96c0851a.png)<br />![2021-10-14-09-00-40-352875.png](./img/1634173272498-aa5a55b0-01af-4ee8-86ce-037d9f3a6cbe.png)
<a name="v9RA6"></a>
## 6、函数既然是对象，就可以在嵌套在函数体内
如下所示，函数内再定义2个函数对象<br />![2021-10-14-09-00-40-583878.png](./img/1634173329108-72d7e8e5-3856-421e-8e5b-c55fe3915754.png)
<a name="SKZ94"></a>
## 7、内层函数对象能够记忆住父函数的变量
官方术语：闭包，记住这个例子：<br />![2021-10-14-09-00-40-789875.png](./img/1634173329090-2734a533-d547-4987-be0e-74eac54e04a7.png)<br />![2021-10-14-09-00-41-026877.png](./img/1634173329094-79466ad5-e8cd-403b-9c17-7ea57787150e.png)<br />![2021-10-14-09-00-41-203876.png](./img/1634173349967-c6fbeafa-aa86-473e-8a34-aec59b313c26.png)
<a name="gsERN"></a>
## 8、所有的函数都是对象，但是所有的对象并不一定都是函数
<a name="wJtJS"></a>
## 9、对象成为函数需要实现`__call__`协议
实现`__call__`协议后，`plus_3`对象便成为函数，可以`plus_3(4)`了！强大。<br />![2021-10-14-09-00-41-448875.png](./img/1634173350001-9469fbc6-1089-4298-8e2f-5636596a66c8.png)
