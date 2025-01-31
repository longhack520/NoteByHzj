GCN是一类非常强大的用于图数据的神经网络架构。事实上，它非常强大，即使是随机初始化的两层 GCN 也可以生成图网络中节点的有用特征表征。下图展示了这种两层 GCN 生成的每个节点的二维表征。请注意，即使没有经过任何训练，这些二维表征也能够保存图中节点的相对邻近性。<br />![GCN1.png](./img/1603334676995-3ac95d94-e0b5-4b02-abe5-3d601aa029de.png)<br />更形式化地说，图卷积网络（GCN）是一个对图数据进行操作的神经网络。给定图![](./img/c311905eab60237306c75dfad5d0ca29.svg)，GCN 的输入：

- 一个输入维度为![](./img/d332ce0d3f13c01150bd13c7391d86dd.svg)的特征矩阵![](./img/02129bb861061d1a052c592e2dc6b383.svg)，其中![](./img/8d9c307cb7f3c4a32822a51922d1ceaa.svg)是图网络中的节点数而![](./img/64fe1bc01a6e4856a0bf3596094d5a6e.svg)是每个节点的输入特征数
- 一个图结构的维度为![](./img/395a9af17f8642e02a32af8637542947.svg)的矩阵表征，例如图![](./img/dfcf28d0734569a6a693bc8194de62bf.svg)的邻接矩阵![](./img/7fc56270e7a70fa81a5935b72eacbe29.svg)

因此，GCN中的隐藏层可以写作![](./img/989267f7e07981032d8561f086731067.svg)，其中![](./img/9938c90049b6b3aa643c9e17726914b0.svg)，![](./img/8fa14cdd754f91cc6554c9e71929cce7.svg)是一种传播规则。每一个隐藏层![](./img/a008c859cf07727ccfd8359183cde87e.svg)都对应一个维度为![](./img/8c47361ab86ae09c45845ed0453ce154.svg)的特征矩阵，该矩阵中的每一行都是某个节点的特征表征。在每一层中，GCN 会使用传播规则![](./img/8fa14cdd754f91cc6554c9e71929cce7.svg)将这些信息聚合起来，从而形成下一层的特征。这样一来，在每个连续的层中特征就会变得越来越抽象。在该框架下，GCN 的各种变体只不过是在传播规则![](./img/8fa14cdd754f91cc6554c9e71929cce7.svg)的选择上有所不同。
<a name="wAVF1"></a>
# 传播规则
这里给出最简单的传播规则示例：

![](./img/9b47f4129d6b6d232fffcee052eac53f.svg)

其中，![](./img/b0e0273c1caa597ede00c58a94473ace.svg)是第![](./img/865c0c0b4ab0e063e5caa3387c1a8741.svg)层的权重矩阵，![](./img/a2ab7d71a0f07f388ff823293c147d21.svg)是非线性激活函数(如 ReLU 函数)。权重矩阵的维度为![](./img/ec3c6ec85e58d649de0218aba3bafc96.svg)，即权重矩阵第二个维度的大小决定了下一层的特征数。如果你对卷积神经网络很熟悉，那么你会发现由于这些权重在图中的节点间共享，该操作与卷积核滤波操作类似。

接下来我们在最简单的层次上研究传播规则。令![](./img/9a041ce63f6c28100344427c6d71837b.svg)(约束条件![](./img/8fa14cdd754f91cc6554c9e71929cce7.svg)是作用于输入特征矩阵的函数)，![](./img/a2ab7d71a0f07f388ff823293c147d21.svg)为恒等函数，选择权重(约束条件：![](./img/6f64217af3dfb65fbd774a91e5c838c4.svg))。换言之，![](./img/ac7cd6d9b98834785e2fa27b1def03e1.svg)。该传播规则可能过于简单，只是为了帮助理解，后面会逐步补充缺失部分以方便解释每一部分的作用。此外，![](./img/58e69a293e3d65294bd972b467c40996.svg)等价于多层感知机的输入层。<br />![GCN2.png](./img/1603337051146-fc0c0aa0-1092-4824-9dea-b5af2b1237f7.png)<br />我们以上图为简单的示例，使用numpy编写的上述又想吐的邻接矩阵表征如下
```python
A = np.matrix([
    [0, 1, 0, 0],
    [0, 0, 1, 1], 
    [0, 1, 0, 0],
    [1, 0, 1, 0]],
    dtype=float
)
```
接下来，我们需要抽取出特征！我们基于每个节点的索引为其生成两个整数特征，这简化了本文后面手动验证矩阵运算的过程。
```python
In [3]: X = np.matrix([
            [i, -i]
            for i in range(A.shape[0])
        ], dtype=float)
        X
Out[3]: matrix([
           [ 0.,  0.],
           [ 1., -1.],
           [ 2., -2.],
           [ 3., -3.]
        ])
```
我们现在已经建立了一个图，其邻接矩阵为![](./img/7fc56270e7a70fa81a5935b72eacbe29.svg)，输入特征的集合为![](./img/02129bb861061d1a052c592e2dc6b383.svg)。下面让我们来看看，当我们对其应用传播规则后会发生什么：
```python
In [6]: A * X
Out[6]: matrix([
            [ 1., -1.],
            [ 5., -5.],
            [ 1., -1.],
            [ 2., -2.]]
```
每个节点的表征(每一行)现在是其相邻节点特征的和。换句话说，图卷积层将每个节点表示为其相邻节点的聚合。大家可以自己动手验证这个计算过程。请注意，在这种情况下，如果存在从![](./img/9e3669d19b675bd57058fd4664205d2a.svg)到![](./img/7b8b965ad4bca0e41ab51de7b31363a1.svg)的边，则节点![](./img/7b8b965ad4bca0e41ab51de7b31363a1.svg)是节点![](./img/9e3669d19b675bd57058fd4664205d2a.svg)的邻居

<a name="CmlCe"></a>
# 自环与归一化
你可能已经发现了其中的问题，即第一版GCN出现的问题：

- 节点的聚合表征不包含它自己的特征！该表征是相邻节点的特征聚合，因此只有具有自环（self-loop）的节点才会在该聚合中包含自己的特征。
- 度大的节点在其特征表征中将具有较大的值，度小的节点将具有较小的值。这可能会导致梯度消失或梯度爆炸，也会影响随机梯度下降算法（随机梯度下降算法通常被用于训练这类网络，且对每个输入特征的规模（或值的范围）都很敏感）。

主流GCN采用了Symmetric normalized Laplacian，即![](./img/3aa75b4acae59f84e7ab0541a671b6ab.svg)来解决上面提到的两个问题。这里我们暂不采用复杂的拉普拉斯矩阵，还是以简单举例，帮助理解。<br />
<a name="2qjBP"></a>
## 增加自环
为了解决第一个问题，我们可以直接为每个节点添加一个自环。具体而言，这可以通过在应用传播规则之前将邻接矩阵![](./img/7fc56270e7a70fa81a5935b72eacbe29.svg)与单位矩阵![](./img/dd7536794b63bf90eccfd37f9b147d7f.svg)相加来实现。
```python
In [4]: I = np.matrix(np.eye(A.shape[0]))
        I
Out[4]: matrix([
            [1., 0., 0., 0.],
            [0., 1., 0., 0.],
            [0., 0., 1., 0.],
            [0., 0., 0., 1.]
        ])
In [8]: A_hat = A + I
        A_hat * X
Out[8]: matrix([
            [ 1., -1.],
            [ 6., -6.],
            [ 3., -3.],
            [ 5., -5.]])
```
现在，由于每个节点都是自己的邻居，每个节点在对相邻节点的特征求和过程中也会囊括自己的特征！
<a name="BXI5Q"></a>
## 特征表征归一化
通过将邻接矩阵![](./img/7fc56270e7a70fa81a5935b72eacbe29.svg)与度矩阵![](./img/f623e75af30e62bbd73d6df5b50bb7b5.svg)的逆相乘，对其进行变换，从而通过节点的度对特征表征进行归一化。因此，我们简化后的传播规则如下：

![](./img/a9d6cbaf26ca2f0cf15e68406b2bfca9.svg)

让我们看看发生了什么。我们首先计算出节点的度矩阵。
```python
In [9]: D = np.array(np.sum(A, axis=0))[0]
        D = np.matrix(np.diag(D))
        D
Out[9]: matrix([
            [1., 0., 0., 0.],
            [0., 2., 0., 0.],
            [0., 0., 2., 0.],
            [0., 0., 0., 1.]
        ])
```
变换之前
```python
A = np.matrix([
    [0, 1, 0, 0],
    [0, 0, 1, 1], 
    [0, 1, 0, 0],
    [1, 0, 1, 0]],
    dtype=float
)
```
变换之后
```python
In [10]: D**-1 * A
Out[10]: matrix([
             [0. , 1. , 0. , 0. ],
             [0. , 0. , 0.5, 0.5],
             [0. , 0.5, 0. , 0. ],
             [0.5, 0. , 0.5, 0. ]
])
```
可以观察到，邻接矩阵中每一行的权重（值）都除以该行对应节点的度。我们接下来对变换后的邻接矩阵应用传播规则：
```python
In [11]: D**-1 * A * X
Out[11]: matrix([
             [ 1. , -1. ],
             [ 2.5, -2.5],
             [ 0.5, -0.5],
             [ 2. , -2. ]
         ])
```
得到与相邻节点的特征均值对应的节点表征。这是因为（变换后）邻接矩阵的权重对应于相邻节点特征加权和的权重。大家可以自己动手验证这个结果。
<a name="EW8y0"></a>
# 整合
现在，我们将把自环和归一化技巧结合起来。此外，我们还将重新介绍之前为了简化讨论而省略的有关权重和激活函数的操作。
<a name="OJULL"></a>
## 添加权重
首先是应用权重。请注意，这里的 D_hat 是 A_hat = A + I 对应的度矩阵，即具有强制自环的矩阵![](./img/7fc56270e7a70fa81a5935b72eacbe29.svg)的度矩阵
```python
In [45]: W = np.matrix([
             [1, -1],
             [-1, 1]
         ])
         D_hat**-1 * A_hat * X * W
Out[45]: matrix([
            [ 1., -1.],
            [ 4., -4.],
            [ 2., -2.],
            [ 5., -5.]
        ])
```
如果我们想要减小输出特征表征的维度，我们可以减小权重矩阵 W 的规模：
```python
In [46]: W = np.matrix([
             [1],
             [-1]
         ])
         D_hat**-1 * A_hat * X * W
Out[46]: matrix([[1.],
        [4.],
        [2.],
        [5.]]
)
```
<a name="0iLEI"></a>
## 激活函数
本文选择保持特征表征的维度，并应用 ReLU 激活函数。
```python
In [51]: W = np.matrix([
             [1, -1],
             [-1, 1]
         ])
         relu(D_hat**-1 * A_hat * X * W)
Out[51]: matrix([[1., 0.],
        [4., 0.],
        [2., 0.],
        [5., 0.]])
```
这就是一个带有邻接矩阵、输入特征、权重和激活函数的完整隐藏层！
<a name="TKpL6"></a>
# 真实场景应用
Zachary 空手道俱乐部是一个被广泛使用的社交网络，其中的节点代表空手道俱乐部的成员，边代表成员之间的相互关系。当年，Zachary 在研究空手道俱乐部的时候，管理员和教员发生了冲突，导致俱乐部一分为二。下图显示了该网络的图表征，其中的节点标注是根据节点属于俱乐部的哪个部分而得到的，「A」和「I」分别表示属于管理员和教员阵营的节点。<br />![Zachary.png](./img/1603347417296-89d8ceda-00ad-46b0-9944-814c6e509657.png)<br />接下来，我们将构建一个图卷积网络。我们并不会真正训练该网络，但是会对其进行简单的随机初始化，从而生成我们在本文开头看到的特征表征。我们将使用 networkx，它有一个可以很容易实现的 Zachary 空手道俱乐部的图表征。然后，我们将计算 A_hat 和 D_hat 矩阵。
```python
from networkx import to_numpy_matrix
zkc = karate_club_graph()
order = sorted(list(zkc.nodes()))
A = to_numpy_matrix(zkc, nodelist=order)
I = np.eye(zkc.number_of_nodes())
A_hat = A + I
D_hat = np.array(np.sum(A_hat, axis=0))[0]
D_hat = np.matrix(np.diag(D_hat))
```
接下来，我们将随机初始化权重。
```python
W_1 = np.random.normal(
    loc=0, scale=1, size=(zkc.number_of_nodes(), 4))
W_2 = np.random.normal(
    loc=0, size=(W_1.shape[1], 2))
```
接着，我们会堆叠 GCN 层。这里，我们只使用单位矩阵作为特征表征，即每个节点被表示为一个 one-hot 编码的类别变量。
```python
def gcn_layer(A_hat, D_hat, X, W):
    return relu(D_hat**-1 * A_hat * X * W)
H_1 = gcn_layer(A_hat, D_hat, I, W_1)
H_2 = gcn_layer(A_hat, D_hat, H_1, W_2)
output = H_2
```
我们进一步抽取出特征表征。
```python
feature_representations = {
    node: np.array(output)[node] 
    for node in zkc.nodes()}
```
这样的特征表征可以很好地将 Zachary 空手道俱乐部的两个社区划分开来。至此，我们甚至都没有开始训练模型<br />![GCN3.png](./img/1603347530274-a2b6d7e5-643d-444a-a4ca-2ebba7655e7c.png)<br />我们应该注意到，在该示例中由于 ReLU 函数的作用，在 x 轴或 y 轴上随机初始化的权重很可能为 0，因此需要反复进行几次随机初始化才能生成上面的图。<br />
<a name="vY1Pj"></a>
# Source
[https://www.jiqizhixin.com/articles/2019-02-20-12?from=synced&keyword=%E6%9E%81%E7%AE%80%E7%9A%84Numpy%E5%AE%9E%E7%8E%B0](https://www.jiqizhixin.com/articles/2019-02-20-12?from=synced&keyword=%E6%9E%81%E7%AE%80%E7%9A%84Numpy%E5%AE%9E%E7%8E%B0)
