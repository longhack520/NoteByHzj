<a name="ibfvs"></a>
# 性能对比
当前的很多文献都表明，从理论的角度，传输明文信息，如模型参数、参数梯度等，也是不安全的，攻击者可以通过窃取这些梯度信息来还原或部分还原原始数据信息，从而导致数据隐私的泄漏。这也是隐私保护机器学习提出的初衷。

同态加密、差分隐私和安全多方计算，它们是密码学领域常用的安全策略，在与联邦学习结合使用的过程中，各自有各自的优缺点。这里从计算性能、通信性能、安全性三个维度进行比较。

- 计算性能：从计算的角度看，计算主要耗时在求梯度上。对于同态加密，计算在密文的状态下进行，密文的计算要比明文的计算耗时更长；而差分隐私主要通过添加噪声数据进行计算，其效率与直接明文计算几乎没有区别；同理，秘密共享是在明文状态下进行的，计算性能基本不受影响。
- 通信性能：从通信的角度看，同态加密传输的是明文数据，密文数据比明文数据占用的比特数要更大，因此传输效率要比明文慢；差分隐私传输的是带噪声数据的明文数据，其传输效率与直接明文传输几乎没有区别；秘密共享为了保护数据隐私，通常会将数据进行拆分并向多方传输，完成相同功能的迭代。同态加密和差分隐私需要一次，而秘密共享需要多次传输才能完成。
- 安全性：这里我们特指在联邦学习场景中本地数据隐私的安全。虽然在联邦学习的训练过程中，我们是通过模型参数的交互来进行预训练的，而不是原始数据，但当前越来越多的研究都表明，即使只有模型的参数或者梯度，也能反向破解原始的输入数据。结合当前的三种安全机制来保护联邦学习训练时的模型参数传输：同态加密由于传输的是密文数据，因此其安全性是最可靠的；秘密共享通过将模型数据进行拆分，只有当恶意客户端超过一定的数目并且相互串通合谋时，才有信息泄漏的风险，总体上安全性较高；差分隐私对模型参数添加噪声数据，但添加的噪声会直接影响模型的性能(当噪声比较小时，模型的性能损失较小，但安全性变差；相反，当噪声比较大时，模型的性能损失较大，但安全性变强)。
| 性能 | 同态加密 | 秘密共享 | 差分隐私 |
| --- | --- | --- | --- |
| 计算性能 | 耗时高 | 耗时低 | 耗时低 |
| 通信性能 | 耗时高 | 耗时较高 | 耗时低 |
| 安全性 | 安全性高 | 安全性较高 | 安全性有一定损失 |

<a name="GdtBg"></a>
# Python安全计算库
前面介绍了联邦学习中常见的安全机制及实现。事实上，这些安全机制并非联邦学习所特有，它们各自在密码学领域中已经被广泛研究和使用，因此，当前也有很多开源的实现方案可供使用。本节简要介绍一些常用的基于Python实现的安全计算库。
<a name="Hh7p4"></a>
### 加/解密：[pycrypto](https://github.com/pycrypto/pycrypto)
pycrypto提供了常用的加/解密算法的实现，不仅包括AES、DES、RSA、ElGamal等常用算法，还包括常用的散列函数，例如SHA256和RIPEMD160等。Python开源包[pycryptodome](https://github.com/Legrandin/pycryptodome)是pycrypto项目的一个分支(fork)，提供了更加丰富的加/解密算法的实现，包括秘密共享算法。在pycryptodome包里，对计算量大的操作还提供了高效的C语言拓展。
<a name="VIR5A"></a>
### 同态加密：[python-paillier](https://github.com/data61/python-paillier)
当前联邦学习系统里最常用的隐私保护机制是同态加密。开源的python-paillier程序包提供了支持部分同态加密(例如加法和标量乘法同态加密)的Python3实现。python-paillier程序包主要实现了基于Paillier算法的加法和标量乘法同态加密算法，支持对浮点数的加密运算。
<a name="sqaAe"></a>
### 差分隐私：[differential-privacy](https://github.com/google/differential-privacy)
差分隐私的主要实现机制是在输入或输出上加入随机化的噪声数据，例如拉普拉斯噪声，高斯噪声、指数噪声等。Google提供的differential-privacy开源包提供了常用的差分隐私方法的实现。
<a name="XpTUL"></a>
### 安全多方计算：[MPyC](https://github.com/lschoe/mpyc)
开源的MPyC程序包提供了基于Python的安全多方计算的实现。MPyC程序包主要提供了基于有限域上的阈值秘密共享，即使用Shamir的门限阈值密码共享方案以及伪随机秘密共享。

常用的基于Python的安全计算程序包还有散列算法、ECC(Elliptic Curve cryptography)加密算法、ECDSA(Elliptic Curve Digital Signature Algorithm)加密算法和eth-keys密钥生成算法等。






