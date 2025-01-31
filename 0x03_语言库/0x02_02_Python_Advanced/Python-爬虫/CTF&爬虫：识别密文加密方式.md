Python 爬虫
<a name="k6M9c"></a>
## 前言
爬虫工程师在做加密参数逆向的时候，经常会遇到各种各样的加密算法、编码、混淆，每个算法都有其对应的特征，对于一些较小的网站，往往直接引用这些官方算法，没有进行魔改等其他操作，这种情况下，如果能熟悉常见算法的特征，通过密文就能猜测出使用的哪种算法、编码、混淆，将会大大提高工作效率！在 CTF 中通常也会有密码类的题目，掌握一些常见密文特征也是 CTFer 们必备的技能！<br />本文将介绍以下编码和加密算法的特征：

- **编码**：Base 系列、Unicode、Escape、URL、Hex；
- **算法**：MD5、SHA 系列、HMAC 系列、RSA、AES、DES、3DES、RC4、Rabbit、SM 系列；
- **混淆**：Obfuscator、JJEncode、AAEncode、JSFuck、Jother、Brainfuck、Ook!、Trivial brainfuck substitution；
- **其他**：恺撒密码、栅栏密码、猪圈密码、摩斯密码、培根密码、维吉尼亚密码、与佛论禅、当铺密码。
<a name="CyoVI"></a>
## 编码系列
<a name="SMNfo"></a>
### Base 系列编码
Base64 是最常见的编码，除此之外，其实还有 Base16、Base32、Base58、Base85、Base100 等，他们之间最明显的区别就是使用了不同数量的可打印字符对任意字节数据进行编码，比如 Base64 使用了 64 个可打印字符（A-Z、a-z、0-9、+、/），Base16 使用了 16 个可打印字符（A-F、0-9），这里主要讲怎么快速识别，其具体原理可自行百度，Base 系列主要特征如下：

- **Base16**：结尾没有等号，数字要多于字母； 
- **Base32**：字母要多于数字，明文数量超过 10 个，结尾可能会有很多等号；
- **Base58**：结尾没有等号，字母要多于数字；
- **Base64**：一般情况下结尾都会有 1 个或者 2 个等号，明文很少的时候可能没有；
- **Base85**：等号一般出现在字符串中间，含有一些奇怪的字符；
- **Base100**：密文由 Emoji 表情组成。

示例：

| 编码类型 | 示例一 | 示例二 |
| --- | --- | --- |
| 明文 | 01234567890 | administrators |
| Base16 | 3031323334353637383930 | 61646D696E6973747261746F7273 |
| Base32 | GAYTEMZUGU3DOOBZGA====== | MFSG22LONFZXI4TBORXXE4Y= |
| Base58 | cX8j8pvGzppMKVb | BNF5dFLUTN5XwM1yLoF |
| Base64 | MDEyMzQ1Njc4OTA= | YWRtaW5pc3RyYXRvcnM= |
| Base85 | 0JP==1c70M3&rY | @:X4hDJ=06Eaa'.EcV |
| Base100 | 🐧🐨🐩🐪🐫🐬🐭🐮🐯🐰🐧 | 👘👛👤👠👥👠👪👫👩👘👫👦👩👪 |

<a name="AmGor"></a>
### Unicode 编码
Unicode 又称为统一码、万国码、单一码，是一种在计算机上使用的字符编码。Unicode 是为了解决传统的字符编码方案的局限而产生的，它为每种语言中的每个字符设定了统一并且唯一的二进制编码，以满足跨语言、跨平台进行文本转换、处理的要求。其主要特征如下：

- 以 `\u`、`&#` 或 `&#x` 开头，后面是数字加字母组合

PS：`\u` 开头和 `&#x` 开头是一样的，都是 16 进制 Unicode 字符的不同写法，`&#` 则是 Unicode 字符 10 进制的写法，此外，`&#` 和 `&#x` 开头的，也称为 HTML 字符实体转换，字符实体是用一个编号写入 HTML 代码中来代替一个字符，在 HTML 中，某些字符是预留的，如果希望正确地显示预留字符，就必须在 HTML 源代码中使用字符实体。<br />示例：

| 编码类型 | 示例一 | 示例二 |
| --- | --- | --- |
| 明文 | 12345 | admin |
| Unicode | \\u0031\\u0032\\u0033\\u0034\\u0035 | \\u0061\\u0064\\u006d\\u0069\\u006e |

<a name="ZVXSp"></a>
### Escape 编码
Escape 编码又叫 %u 编码，Escape 编码就是字符对应 UTF-16BE 表示方式前面加 %u，Escape 不会对 ASCII 字母和数字进行编码，也不会对下面这些 ASCII 标点符号进行编码： `*` `@` `-` `_` `+` `.` `/` ，其他所有的字符都会被转义序列替换。其主要特征如下：

- 以 `%u` 开头，后面是数字加字母组合

示例：

| 编码类型 | 示例一 | 示例二 |
| --- | --- | --- |
| 明文 | 爬虫 | 我爱 Python |
| Escape | %u722C%u866B | %u6211%u7231Python |

<a name="daH1E"></a>
### URL / Hex 编码
URL 和 Hex 编码的结果是一样的，不同的是当用 URL 编码网址时是不会把 http、https 关键字和 `/`、`?`、`&`、`=` 等连接符进行编码的，而 Hex 编码则全部转化了，其主要特征如下：

- 以 `%` 开头，后面是数字加字母组合
| 编码类型 | 示例 |
| --- | --- |
| 明文 | https://www.kuaidaili.com/ |
| Unicode | https://%77%77%77%2E%6B%75%61%69%64%61%69%6C%69%2E%63%6F%6D/ |
| Hex | %68%74%74%70%73%3a%2f%2f%77%77%77%2e%6b%75%61%69%64%61%69%6c%69%2e%63%6f%6d%2f |

<a name="hyTGb"></a>
## 加密算法
<a name="aXo0q"></a>
### MD5
MD5 实质是一种消息摘要算法，一个数据的 MD5 值是唯一的，同一个数据不可能计算出多个不同的 MD5 值，但是，不同数据计算出来的 MD5 值是有可能一样的，知道一个 MD5 值，理论上是无法还原出它的原始数据的，MD5 是最容易辨别的，主要特征如下：

- 密文一般为 16 位或者 32 位，其中 16 位是取的 32 位第 9~25 位的值；
- 组成方式为字母（a-f）和数字（0-9）混合，字母可以全部是大写或者小写。

除了通过密文特征来判断以外，还可以搜索源代码，标准 MD5 的源码里是包含了一些特定的值的，没有这些特定值，就无法实现 MD5：

- 0123456789ABCDEF、0123456789abcdef
- 1732584193、-271733879、-1732584194、271733878

PS：某些特殊情况下，密文的长度也有可能不止 16 位或者 32 位，有可能是在官方算法上有一些魔改，通常也是在 16 位的基础上，左右填充了一些随机字符串。<br />示例：

| 编码类型 | 示例一 | 示例二 |
| --- | --- | --- |
| 明文 | 123456 | admin |
| MD5（16 位小写） | 49ba59abbe56e057 | 7a57a5a743894a0e |
| MD5（16 位大写） | 49BA59ABBE56E057 | 7A57A5A743894A0E |
| MD5（32 位小写） | e10adc3949ba59abbe56e057f20f883e | 21232f297a57a5a743894a0e4a801fc3 |
| MD5（32 位大写） | E10ADC3949BA59ABBE56E057F20F883E | 21232F297A57A5A743894A0E4A801FC3 |

<a name="Vnp1z"></a>
### SHA 系列
SHA 是比 MD5 更安全一点的摘要算法，SHA 通常指 SHA 家族算法，分别是 SHA-1、SHA-2、SHA-3，其中 SHA-2 是 SHA-224、SHA-256、SHA-384、SHA-512 的并称，SHA-3 是 SHA3-224、SHA3-256、SHA3-384、SHA3-512、SHAKE128、SHAKE256 的并称，其名字的后缀的数字就代表了结果的大小（bit），注意，SHAKE 算法结果的大小并不是固定的，其他算法特征如下：

- **SHA-1**：字母（a-f）和数字（0-9）混合，固定位数 **40** 位；
- **SHA-224/SHA3-224**：字母（a-f）和数字（0-9）混合，固定位数 **56** 位；
- **SHA-256/SHA3-256**：字母（a-f）和数字（0-9）混合，固定位数 **64** 位；
- **SHA-384/SHA3-384**：字母（a-f）和数字（0-9）混合，固定位数 **96** 位；
- **SHA-512/SHA3-512**：字母（a-f）和数字（0-9）混合，固定位数 **128** 位。

示例：

| 编码类型 | 示例 |
| --- | --- |
| 明文 | 123456 |
| SHA-1 | 7c4a8d09ca3762af61e59520943dc26494f8941b |
| SHA-256 | 8d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c92 |
| SHA3-256 | c888c9ce9e098d5864d3ded6ebcc140a12142263bace3a23a36f9905f12bd64a |

<a name="LJOjO"></a>
### HMAC 系列
HMAC 这种算法就是在 MD5、SHA 两种加密的基础上引入了秘钥，其密文也和 MD5、SHA 类似，密文的长度和使用的 MD5、SHA 算法对应密文的长度是一样的。特征如下：

- **HMAC-MD5**：字母（a-f）和数字（0-9）混合，位数一般为 **32** 位；
- **HMAC-SHA-1**：字母（a-f）和数字（0-9）混合，固定位数 **40** 位；
- **HMAC-SHA-224 / HMAC-SHA3-224**：字母（a-f）和数字（0-9）混合，固定位数 **56** 位；
- **HMAC-SHA-256 / HMAC-SHA3-256**：字母（a-f）和数字（0-9）混合，固定位数 **64** 位；
- **HMAC-SHA-384 / HMAC-SHA3-384**：字母（a-f）和数字（0-9）混合，固定位数 **96** 位；
- **HMAC-SHA-512 / HMAC-SHA3-512**：字母（a-f）和数字（0-9）混合，固定位数 **128** 位。

HMAC 和 SHA、MD5 的密文都很像，当无法确定是否为 HMAC 时，可以通过其名称搜索到加密方法，如果传入了密钥 key，说明就是 HMAC，当然也可以直接当做是 SHA 或 MD5 来解，解密失败时就得考虑是否有密钥，是否为 HMAC 了，在 JS 中，通常一个 HMAC 加密方法是这样写的：
```javascript
function HmacSHA1Encrypt(word, key) {
    return CryptoJS.HmacSHA1(word, key).toString();
}
```
示例（密钥 123456abcde）：

| 编码类型 | 示例 |
| --- | --- |
| 明文 | 123456 |
| HMAC-MD5 | 432bb95bb00005ddce4a1c757488ed95 |
| HMAC-SHA-1 | 37a04076b7736c44460d330ee0d00014428b175e |
| HMAC-SHA-256 | 50cb1345366df11140fb91b43caaf69627e3f5529705ddf6b0d0cae67986e585 |
| HMAC-SHA3-256 | b808ed9f66436e89fba527a01d1d6044318fea8599d9f39bfb6bec4843964bf3 |

<a name="IwZ8h"></a>
### RSA
RSA 加密算法是一种非对称加密算法，通过公钥加密结果，必须私钥解密。同样私钥加密结果，公钥可以解密，应用非常广泛，在网站中通常使用 JSEncrypt 库来实现，其最大的特征就是有一个设置公钥的过程，可以通过以下方法来快速初步判断是否为 RSA 算法：

- 搜索关键词 `new JSEncrypt()`，`JSEncrypt` 等，一般会使用 JSEncrypt 库，会有 new 一个实例对象的操作；
- 搜索关键词 `setPublicKey`、`setKey`、`setPrivateKey`、`getPublicKey` 等，一般实现的代码里都含有设置密钥的过程。

RSA 的私钥、公钥、明文、密文长度也有一定对应关系，也可以从这方面初步判断：

| 私钥长度<br />（Base64） | 公钥长度<br />（Base64） | 明文长度 | 密文长度 |
| --- | --- | --- | --- |
| 428 | 128 | 1~53 | 88 |
| 812 | 216 | 1~117 | 172 |
| 1588 | 392 | 1~245 | 344 |

<a name="CbMaJ"></a>
### AES、DES、3DES、RC4、Rabbit 等
AES、DES、3DES、RC4、Rabbit 等加密算法的密文通常没有固定的长度，他们通常使用 crypto-js 来实现，比如 AES 加解密示例如下：
```javascript
CryptoJS = require("crypto-js")

var key = CryptoJS.enc.Utf8.parse("0123456789abcdef");
var iv = CryptoJS.enc.Utf8.parse("0123456789abcdef");

function AESEncrypt(word) {
    var srcs = CryptoJS.enc.Utf8.parse(word);
    var encrypted = CryptoJS.AES.encrypt(srcs, key, {
        iv: iv,
        mode: CryptoJS.mode.CBC,
        padding: CryptoJS.pad.Pkcs7
    });
    return encrypted.toString();
}

function AESDecrypt(word) {
    var srcs = word;
    var decrypt = CryptoJS.AES.decrypt(srcs, key, {
        iv: iv,
        mode: CryptoJS.mode.CBC,
        padding: CryptoJS.pad.Pkcs7
    });
    return decrypt.toString(CryptoJS.enc.Utf8);
}

console.log(AESEncrypt("爬虫"))
console.log(AESDecrypt("nSk3wCd92s08sQ9N+VHNvA=="))
```
在 crypto-js 中，也有一些特定的关键字，可以通过搜索这些关键字来快速定位到 crypto-js：

- CryptoJS、crypto-js、iv、mode、padding、createEncryptor、createDecryptor
- ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=、0xffffffff、0xffff

定位到 CryptoJS 后，观察加密方法，比如 AES 就是 `CryptoJS.AES.encrypt`，DES 就是 `CryptoJS.DES.encrypt`，3DES 就是 `CryptoJS.TripleDES.encrypt`，注意他的 iv、mode、padding，拿下来就可以本地复现了。
<a name="pl2x0"></a>
### SM 系列
SM 代表商密，即商业密码，是我国发布的一系列国产加密算法，SM 系列包括：SM1、SM2、SM3 、SM4、SM7、SM9，其中 SM1 和 SM7 的算法不公开，SM 系列算法在我国一些 gov 网站上有应用。<br />在 SM 的 JavaScript 代码中一般会存在以下关键字，可以通过搜索关键字定位：

- SM2、SM3、SM4
- FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF
- FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC
- 28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93
- abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789
- getPublicKeyFromPrivateKey、doEncrypt、doDecrypt、doSignature
<a name="mL5yH"></a>
## 混淆系列
<a name="WPEFo"></a>
### Obfuscator
Obfuscator官网：[https://obfuscator.io/](https://obfuscator.io/)<br />Obfuscator 就是混淆的意思，简称 OB 混淆，OB 混淆具有以下特征：

- 一般由一个大数组或者含有大数组的函数、一个自执行函数、解密函数和加密后的函数四部分组成；
- 函数名和变量名通常以 _0x 或者 0x 开头，后接 1~6 位数字或字母组合；
- 自执行函数，进行移位操作，有明显的 push、shift 关键字；

一段正常的代码如下：
```javascript
function hi() {
  console.log("Hello World!");
}
hi();
```
经过 OB 混淆后的结果：
```javascript
function _0x3f26() {
    var _0x2dad75 = ['5881925kTCKCP', 'Hello\x20World!', '600mDvfGa', '699564jYNxbu', '1083271cEvuvT', 'log', '18sKjcFY', '214857eMgFSU', '77856FUKcuE', '736425OzpdFI', '737172JqcGMg'];
    _0x3f26 = function () {
        return _0x2dad75;
    };
    return _0x3f26();
}

(function (_0x307c88, _0x4f8223) {
    var _0x32807d = _0x1fe9, _0x330c58 = _0x307c88();
    while (!![]) {
        try {
            var _0x5d6354 = parseInt(_0x32807d(0x6f)) / 0x1 + parseInt(_0x32807d(0x6e)) / 0x2 + parseInt(_0x32807d(0x70)) / 0x3 + -parseInt(_0x32807d(0x69)) / 0x4 + parseInt(_0x32807d(0x71)) / 0x5 + parseInt(_0x32807d(0x6c)) / 0x6 * (parseInt(_0x32807d(0x6a)) / 0x7) + -parseInt(_0x32807d(0x73)) / 0x8 * (parseInt(_0x32807d(0x6d)) / 0x9);
            if (_0x5d6354 === _0x4f8223) break; else _0x330c58['push'](_0x330c58['shift']());
        } catch (_0x3f18e4) {
            _0x330c58['push'](_0x330c58['shift']());
        }
    }
}(_0x3f26, 0xaa023));

function _0x1fe9(_0xa907e7, _0x410a46) {
    var _0x3f261f = _0x3f26();
    return _0x1fe9 = function (_0x1fe950, _0x5a08da) {
        _0x1fe950 = _0x1fe950 - 0x69;
        var _0x82a06 = _0x3f261f[_0x1fe950];
        return _0x82a06;
    }, _0x1fe9(_0xa907e7, _0x410a46);
}

function hi() {
    var _0x12a222 = _0x1fe9;
    console[_0x12a222(0x6b)](_0x12a222(0x72));
}

hi();
```
<a name="ABTcu"></a>
### JJEncode
JJEncode、AAEncode、JSFuck 都是同一个作者，JJEncode 具有以下特征：

- 大量 `$`、`_` 符号、大量重复的自定义变量；
- 仅由 18 个符号组成：`[]()!+,\"$.:;_{}~=`

正常的一段 JS 代码：
```javascript
alert("Hello, JavaScript" )
```
经过 JJEncode 混淆（自定义变量名为 $）之后的代码：
```javascript
$=~[];$={___:++$,$$$$:(![]+"")[$],__$:++$,$_$_:(![]+"")[$],_$_:++$,$_$$:({}+"")[$],$$_$:($[$]+"")[$],_$$:++$,$$$_:(!""+"")[$],$__:++$,$_$:++$,$$__:({}+"")[$],$$_:++$,$$$:++$,$___:++$,$__$:++$};$.$_=($.$_=$+"")[$.$_$]+($._$=$.$_[$.__$])+($.$$=($.$+"")[$.__$])+((!$)+"")[$._$$]+($.__=$.$_[$.$$_])+($.$=(!""+"")[$.__$])+($._=(!""+"")[$._$_])+$.$_[$.$_$]+$.__+$._$+$.$;$.$$=$.$+(!""+"")[$._$$]+$.__+$._+$.$+$.$$;$.$=($.___)[$.$_][$.$_];$.$($.$($.$$+"\""+$.$_$_+(![]+"")[$._$_]+$.$$$_+"\\"+$.__$+$.$$_+$._$_+$.__+"(\\\"\\"+$.__$+$.__$+$.___+$.$$$_+(![]+"")[$._$_]+(![]+"")[$._$_]+$._$+",\\"+$.$__+$.___+"\\"+$.__$+$.__$+$._$_+$.$_$_+"\\"+$.__$+$.$$_+$.$$_+$.$_$_+"\\"+$.__$+$._$_+$._$$+$.$$__+"\\"+$.__$+$.$$_+$._$_+"\\"+$.__$+$.$_$+$.__$+"\\"+$.__$+$.$$_+$.___+$.__+"\\\"\\"+$.$__+$.___+")"+"\"")())();
```
<a name="F6Dy7"></a>
### AAEncode
JJEncode、AAEncode、JSFuck 都是同一个作者，AAEncode 具有以下特征：

- 仅由日式表情符号组成

正常的一段 JS 代码：
```javascript
alert("Hello, JavaScript" )
```
经过 AAEncode 混淆之后的代码：
```javascript
ﾟωﾟﾉ= /｀ｍ´）ﾉ ~┻━┻   //*´∇｀*/ ['_']; o=(ﾟｰﾟ)  =_=3; c=(ﾟΘﾟ) =(ﾟｰﾟ)-(ﾟｰﾟ); (ﾟДﾟ) =(ﾟΘﾟ)= (o^_^o)/ (o^_^o);(ﾟДﾟ)={ﾟΘﾟ: '_' ,ﾟωﾟﾉ : ((ﾟωﾟﾉ==3) +'_') [ﾟΘﾟ] ,ﾟｰﾟﾉ :(ﾟωﾟﾉ+ '_')[o^_^o -(ﾟΘﾟ)] ,ﾟДﾟﾉ:((ﾟｰﾟ==3) +'_')[ﾟｰﾟ] }; (ﾟДﾟ) [ﾟΘﾟ] =((ﾟωﾟﾉ==3) +'_') [c^_^o];(ﾟДﾟ) ['c'] = ((ﾟДﾟ)+'_') [ (ﾟｰﾟ)+(ﾟｰﾟ)-(ﾟΘﾟ) ];(ﾟДﾟ) ['o'] = ((ﾟДﾟ)+'_') [ﾟΘﾟ];(ﾟoﾟ)=(ﾟДﾟ) ['c']+(ﾟДﾟ) ['o']+(ﾟωﾟﾉ +'_')[ﾟΘﾟ]+ ((ﾟωﾟﾉ==3) +'_') [ﾟｰﾟ] + ((ﾟДﾟ) +'_') [(ﾟｰﾟ)+(ﾟｰﾟ)]+ ((ﾟｰﾟ==3) +'_') [ﾟΘﾟ]+((ﾟｰﾟ==3) +'_') [(ﾟｰﾟ) - (ﾟΘﾟ)]+(ﾟДﾟ) ['c']+((ﾟДﾟ)+'_') [(ﾟｰﾟ)+(ﾟｰﾟ)]+ (ﾟДﾟ) ['o']+((ﾟｰﾟ==3) +'_') [ﾟΘﾟ];(ﾟДﾟ) ['_'] =(o^_^o) [ﾟoﾟ] [ﾟoﾟ];(ﾟεﾟ)=((ﾟｰﾟ==3) +'_') [ﾟΘﾟ]+ (ﾟДﾟ) .ﾟДﾟﾉ+((ﾟДﾟ)+'_') [(ﾟｰﾟ) + (ﾟｰﾟ)]+((ﾟｰﾟ==3) +'_') [o^_^o -ﾟΘﾟ]+((ﾟｰﾟ==3) +'_') [ﾟΘﾟ]+ (ﾟωﾟﾉ +'_') [ﾟΘﾟ]; (ﾟｰﾟ)+=(ﾟΘﾟ); (ﾟДﾟ)[ﾟεﾟ]='\\'; (ﾟДﾟ).ﾟΘﾟﾉ=(ﾟДﾟ+ ﾟｰﾟ)[o^_^o -(ﾟΘﾟ)];(oﾟｰﾟo)=(ﾟωﾟﾉ +'_')[c^_^o];(ﾟДﾟ) [ﾟoﾟ]='\"';(ﾟДﾟ) ['_'] ( (ﾟДﾟ) ['_'] (ﾟεﾟ+(ﾟДﾟ)[ﾟoﾟ]+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ (ﾟｰﾟ)+ (ﾟΘﾟ)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ (ﾟｰﾟ)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ (ﾟｰﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((o^_^o) +(o^_^o))+ ((o^_^o) - (ﾟΘﾟ))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((o^_^o) +(o^_^o))+ (ﾟｰﾟ)+ (ﾟДﾟ)[ﾟεﾟ]+((ﾟｰﾟ) + (ﾟΘﾟ))+ (c^_^o)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟｰﾟ)+ ((o^_^o) - (ﾟΘﾟ))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ (ﾟΘﾟ)+ (c^_^o)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ (ﾟｰﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ (ﾟｰﾟ)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ (ﾟｰﾟ)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ ((ﾟｰﾟ) + (o^_^o))+ (ﾟДﾟ)[ﾟεﾟ]+((ﾟｰﾟ) + (ﾟΘﾟ))+ (ﾟｰﾟ)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟｰﾟ)+ (c^_^o)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ (ﾟΘﾟ)+ ((o^_^o) - (ﾟΘﾟ))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ (ﾟｰﾟ)+ (ﾟΘﾟ)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((o^_^o) +(o^_^o))+ ((o^_^o) +(o^_^o))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ (ﾟｰﾟ)+ (ﾟΘﾟ)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((o^_^o) - (ﾟΘﾟ))+ (o^_^o)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ (ﾟｰﾟ)+ (o^_^o)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((o^_^o) +(o^_^o))+ ((o^_^o) - (ﾟΘﾟ))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ (ﾟΘﾟ)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((o^_^o) +(o^_^o))+ (c^_^o)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((o^_^o) +(o^_^o))+ (ﾟｰﾟ)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟｰﾟ)+ ((o^_^o) - (ﾟΘﾟ))+ (ﾟДﾟ)[ﾟεﾟ]+((ﾟｰﾟ) + (ﾟΘﾟ))+ (ﾟΘﾟ)+ (ﾟДﾟ)[ﾟoﾟ]) (ﾟΘﾟ)) ('_');
```
<a name="RX3Tq"></a>
### JSFuck
JJEncode、AAEncode、JSFuck 都是同一个作者，JSFuck 具有以下特征：

- 仅由 6 个符号组成：`[]()!+`

正常的一段 JS 代码：
```javascript
alert(1)
```
经过 JSFuck 混淆之后的代码类似于：
```javascript
[][(![]+[])[+[]]+(![]+[])[!+[]+!+[]]+(![]+[])[+!+[]]+(!![]+[])[+[]]][([][(![]+[])[+[]]+(![]+[])[!+[]+!+[]]+(![]+[])[+!+[]]+(!![]+[])[+[]]]+[])[!+[]+!+[]+!+[]]+(!![]+[][(![]+[])[+[]]+(![]+[])[!+[]+!+[]]+(![]+[])[+!+[]]+(!![]+[])[+[]]])[+!+[]+[+[]]]+([][[]]+[])[+!+[]]+(![]+[])[!+[]+!+[]+!+[]]+(!![]+[])[+[]]+(!![]+[])[+!+[]]+([][[]]+[])[+[]]+([][(![]+[])[+[]]+(![]+[])[!+[]+!+[]]+(![]+[])[+!+[]]+(!![]+[])[+[]]]+[])[!+[]+!+[]+!+[]]+(!![]+[])[+[]]+(!![]+[][(![]+[])[+[]]+(![]+[])[!+[]+!+[]]+(![]+[])[+!+[]]+(!![]+[])[+[]]])[+!+[]+[+[]]]+(!![]+[])[+!+[]]]((!![]+[])[+!+[]]+(!![]+[])[!+[]+!+[]+!+[]]+(!![]+[])[+[]]+([][[]]+[])[+[]]+(!![]+[])[+!+[]]+([][[]]+[])[+!+[]]+(+[![]]+[][(![]+[])[+[]]+(![]+[])[!+[]+!+[]]+(![]+[])[+!+[]]+(!![]+[])[+[]]])[+!+[]+[+!+[]]]+(!![]+[])[!+[]+!+[]+!+[]]+(+(!+[]+!+[]+!+[]+[+!+[]]))[(!![]+[])[+[]]+(!![]+[][(![]+[])[+[]]+(![]+[])[!+[]+!+[]]+(![]+[])[+!+[]]+(!![]+[])[+[]]])[+!+[]+[+[]]]+([]+[])[([][(![]+[])[+[]]+(![]+[])[!+[]+!+[]]+(![]+[])[+!+[]]+(!![]+[])[+[]]]+[])[!+[]+!+[]+!+[]]+(!![]+[][(![]+[])[+[]]+(![]+[])[!+[]+!+[]]+(![]+[])[+!+[]]+(!![]+[])[+[]]])[+!+[]+[+[]]]+([][[]]+[])[+!+[]]+(![]+[])[!+[]+!+[]+!+[]]+(!![]+[])[+[]]+(!![]+[])[+!+[]]+([][[]]+[])[+[]]+([][(![]+[])[+[]]+(![]+[])[!+[]+!+[]]+(![]+[])[+!+[]]+(!![]+[])[+[]]]+[])[!+[]+!+[]+!+[]]+(!![]+[])[+[]]+(!![]+[][(![]+[])[+[]]+(![]+[])[!+[]+!+[]]+(![]+[])[+!+[]]+(!![]+[])[+[]]])[+!+[]+[+[]]]+(!![]+[])[+!+[]]][([][[]]+[])[+!+[]]+(![]+[])[+!+[]]+((+[])[([][(![]+[])[+[]]+(![]+[])[!+[]+!+[]]+(![]+[])[+!+[]]+(!![]+[])[+[]]]+[])[!+[]+!+[]+!+[]]+(!![]+[][(![]+[])[+[]]+(![]+[])[!+[]+!+[]]+(![]+[])[+!+[]]+(!![]+[])[+[]]])[+!+[]+[+[]]]+([][[]]+[])[+!+[]]+(![]+[])[!+[]+!+[]+!+[]]+(!![]+[])[+[]]+(!![]+[])[+!+[]]+([][[]]+[])[+[]]+([][(![]+[])[+[]]+(![]+[])[!+[]+!+[]]+(![]+[])[+!+[]]+(!![]+[])[+[]]]+[])[!+[]+!+[]+!+[]]+(!![]+[])[+[]]+(!![]+[][(![]+[])[+[]]+(![]+[])[!+[]+!+[]]+(![]+[])[+!+[]]+(!![]+[])[+[]]])[+!+[]+[+[]]]+(!![]+[])[+!+[]]]+[])[+!+[]+[+!+[]]]+(!![]+[])[!+[]+!+[]+!+[]]]](!+[]+!+[]+!+[]+[!+[]+!+[]])+(![]+[])[+!+[]]+(![]+[])[!+[]+!+[]])()((![]+[])[+!+[]]+(![]+[])[!+[]+!+[]]+(!![]+[])[!+[]+!+[]+!+[]]+(!![]+[])[+!+[]]+(!![]+[])[+[]]+([][(![]+[])[+[]]+(![]+[])[!+[]+!+[]]+(![]+[])[+!+[]]+(!![]+[])[+[]]]+[])[+!+[]+[!+[]+!+[]+!+[]]]+[+!+[]]+([+[]]+![]+[][(![]+[])[+[]]+(![]+[])[!+[]+!+[]]+(![]+[])[+!+[]]+(!![]+[])[+[]]])[!+[]+!+[]+[+[]]])
```
<a name="TrUqx"></a>
### Jother
Jother 混淆和 JSFuck 有点儿类似，唯一的区别就是密文比 JSFuck 多了 `{}`，其解密方式和 JSFuck 是一样的，Jother 混淆现在不太常见了，也很难找到在线混淆之类的工具了，原作者有个在线页面也关闭了，不过仍然可以了解一下，Jother 混淆具有以下特征：

- 仅由 8 个符号组成：`[]()!+{}`

正常的一段代码：
```javascript
function anonymous() {
  return location
}
```
经过 Jother 混淆之后的代码类似于：
```javascript
[][(![]+[])[!![]+!![]+!![]]+({}+[])[+!![]]+(!![]+[])[+!![]]+(!![]+[])[+[]]][({}+[])[!![]+!![]+!![]+!![]+!![]]+({}+[])[+!![]]+({}[[]]+[])[+!![]]+(![]+[])[!![]+!![]+!![]]+(!![]+[])[+[]]+(!![]+[])[+!![]]+({}[[]]+[])[+[]]+({}+[])[!![]+!![]+!![]+!![]+!![]]+(!![]+[])[+[]]+({}+[])[+!![]]+(!![]+[])[+!![]]]((!![]+[])[+!![]]+(!![]+[])[!![]+!![]+!![]]+(!![]+[])[+[]]+({}[[]]+[])[+[]]+(!![]+[])[+!![]]+({}[[]]+[])[+!![]]+({}+[])[!![]+!![]+!![]+!![]+!![]+!![]+!![]]+(![]+[])[!![]+!![]]+({}+[])[+!![]]+({}+[])[!![]+!![]+!![]+!![]+!![]]+(![]+[])[+!![]]+(!![]+[])[+[]]+({}[[]]+[])[!![]+!![]+!![]+!![]+!![]]+({}+[])[+!![]]+({}[[]]+[])[+!![]])()
```
<a name="xjSyx"></a>
### Brainfuck
Brainfuck 实际上是一种极小化的计算机语言，又称为 BF 语言，该语言以其极简主义着称，仅包含八个简单的命令、一个数据指针和一个指令指针，这种语言在爬虫领域也可以是一种反爬手段，可以视为一种混淆方式，虽然不常见🤔，这里给一个在线体验的网址：[https://copy.sh/brainfuck/text.html](https://copy.sh/brainfuck/text.html) ，感兴趣的同志可以深入研究一下，Brainfuck 具有以下特征：

- 仅由 `<>+-.[]` 组成，大量的 `+-` 符号。

正常的一段代码：
```javascript
alert("Hello, Brainfuck")
```
经过 Brainfuck 混淆之后的代码类似于：
```javascript
--[----->+<]>-----.+++++++++++.-------.+++++++++++++.++.+[--->+<]>+.------.++[->++<]>.-[->+++++<]>++.+++++++..+++.[->+++++<]>+.------------.+[->++<]>.---[----->+<]>-.+++[->+++<]>++.++++++++.+++++.--------.-[--->+<]>--.+[->+++<]>+.++++++++.+[++>---<]>.+++++++.
```
<a name="Gid2k"></a>
### Ook!
Ook! 和 Brainfuck 的原理都是类似的，只不过符号有差异，同样的，这种语言在爬虫领域也可以是一种反爬手段，可以视为一种混淆方式，虽然不常见🤔，在线体验的网址：[https://www.splitbrain.org/services/ook](https://www.splitbrain.org/services/ook) ，Ook! 具有以下特征：

- 完整 Ook!：仅由 3 种符号组成 `Ook.`、`Ook?`、`Ook!`
- Short Ook!：仅由 3 种符号组成 `.!?`

正常的一段代码：
```javascript
alert("Hello, Ook!")
```
经过 Ook! 混淆之后的代码类似于：
```javascript
Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook! Ook? Ook! Ook! Ook. Ook? Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook? Ook. Ook? Ook! Ook. Ook? Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook! Ook. Ook? Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook! Ook? Ook! Ook! Ook. Ook? Ook. Ook. Ook. Ook. Ook. Ook. Ook? Ook. Ook? Ook! Ook. Ook? Ook. Ook. Ook. Ook. Ook! Ook. Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook. Ook? Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook! Ook? Ook! Ook! Ook. Ook? Ook. Ook. Ook. Ook. Ook. Ook. Ook? Ook. Ook? Ook! Ook. Ook? Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook! Ook. Ook. Ook. Ook. Ook. Ook! Ook. Ook? Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook! Ook? Ook! Ook! Ook. Ook? Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook? Ook. Ook? Ook! Ook. Ook? Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook. Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook. Ook? Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook! Ook? Ook! Ook! Ook. Ook? Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook? Ook. Ook? Ook! Ook. Ook? Ook. Ook. Ook. Ook. Ook! Ook. Ook? Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook! Ook? Ook! Ook! Ook. Ook? Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook? Ook. Ook? Ook! Ook. Ook? Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook! Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook! Ook. Ook! Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook! Ook. Ook? Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook! Ook? Ook! Ook! Ook. Ook? Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook? Ook. Ook? Ook! Ook. Ook? Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook. Ook? Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook! Ook? Ook! Ook! Ook. Ook? Ook! Ook! Ook! Ook! Ook! Ook! Ook? Ook. Ook? Ook! Ook. Ook? Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook. Ook? Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook! Ook? Ook! Ook! Ook. Ook? Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook? Ook. Ook? Ook! Ook. Ook? Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook! Ook. Ook? Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook! Ook? Ook! Ook! Ook. Ook? Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook? Ook. Ook? Ook! Ook. Ook? Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook! Ook. Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook. Ook? Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook! Ook? Ook! Ook! Ook. Ook? Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook? Ook. Ook? Ook! Ook. Ook? Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook. Ook. Ook. Ook! Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook! Ook. Ook? Ook. 
```
<a name="KMiZY"></a>
### Trivial brainfuck substitution
Trivial brainfuck substitution 不是一种单一的编程语言，而是一大类编程语言，成员超过 20 个，前面提到的 Brainfuck、Ook! 都是其中的一员，在爬虫领域中，说实话这种稀奇古怪的混淆其实并不常见，但是在一些 CTF 中有可能会出现，作为爬虫工程师也可以了解了解😎，具体可以参考：[https://esolangs.org/wiki/Trivial_brainfuck_substitution](https://esolangs.org/wiki/Trivial_brainfuck_substitution)
<a name="GTJ8x"></a>
## 其他
<a name="TRPdt"></a>
### 恺撒密码
恺撒密码（Caesar cipher）又称为恺撒加密、恺撒变换、变换加密，它是一种替换加密的技术，明文中的所有字母都在字母表上向后（或向前）按照一个固定数目进行偏移后被替换成密文。例如，当偏移量是 3 的时候，所有的字母 A 将被替换成 D，B 变成 E，以此类推。这个加密方法是以罗马共和时期恺撒的名字命名的，当年恺撒曾用此方法与其将军们进行联系。<br />根据偏移量的不同，还存在若干特定的恺撒密码名称：偏移量为10：Avocat（A→K）；偏移量为13：ROT13；偏移量为-5：Cassis (K 6)；偏移量为-6：Cassette (K 7)<br />示例（偏移量 3）：

- 明文字母表：ABCDEFGHIJKLMNOPQRSTUVWXYZ
- 密文字母表：DEFGHIJKLMNOPQRSTUVWXYZABC
<a name="X3IOB"></a>
### 栅栏密码
栅栏密码就是把要加密的明文分成 N 个一组，然后把每组的第 1 个字连起来，形成一段无规律的话。栅栏密码本身有一个潜规则，就是组成栅栏的字母一般不会太多，一般不超过 30 个。<br />示例：明文为 THE LONGEST DAY MUST HAVE AN END。加密时，把将要传递的信息中的字母交替排成上下两行：<br />T E O G S D Y U T A E N N<br />H L N E T A M S H V A E D<br />将下面一行字母排在上面一行的后边组合成密文：<br />TEOGSDYUTAENN HLNETAMSHVAED<br />栅栏密码还有一种变种，称为栅栏密码 W 型，它会先把明文类似 W 形状进行排列，然后再按栏目顺序 1-N，取每一栏的所有字符值，组成加密后密文，比如字符串 123456789，采用栏目数为 3 时，明文将采用如下排列： <br />1—5—9<br />-2-4-6-8-<br />–3—7–<br />取每一栏所有字符串，组成加密后密文：159246837
<a name="QONXn"></a>
### 猪圈密码
猪圈密码也称为朱高密码、共济会暗号、共济会密码或共济会员密码，是一种以格子为基础的简单替代式密码。只能对字母加解密并且符号无法复制，粘贴后会直接显示明文，即使使用符号，也不会影响密码分析，亦可用在其它替代式的方法。曾经是美国内战时盟军使用的密码，目前仅在密码教学、各种竞赛中使用。<br />![](./img/1642727843143-e6fae285-b029-4c30-811f-a51fd43b6b4b.gif)
<a name="UG8lJ"></a>
### 摩斯密码
摩斯密码（Morse code），又称为摩尔斯电码、摩斯电码，是一种时通时断的信号代码，这种信号代码通过不同的排列顺序来表达不同的英文字母、数字和标点符号等。<br />26个字母的摩斯密码表

| 字符 | 摩斯码 | 字符 | 摩斯码 | 字符 | 摩斯码 |
| --- | --- | --- | --- | --- | --- |
| A | ．━ | B | ━ ．．． | C | ━ ．━ ． |
| D | ━ ．． | E | ． | F | ．．━ ． |
| G | ━ ━ ． | H | ．．．． | I | ．． |
| J | ．━ ━ ━ | K | ━ ．━ | L | ．━ ．． |
| M | ━ ━ | N | ━ ． | O | ━ ━ ━ |
| P | ．━ ━ ． | Q | ━ ━ ．━ | R | ．━ ． |
| S | ．．． | T | ━ | U | ．．━ |
| V | ．．．━ | W | ．━ ━ | X | ━ ．．━ |
| Y | ━ ．━ ━ | Z | ━ ━ ．． | 

 | 

 |

10个数字的摩斯密码表

| 字符 | 摩斯码 | 字符 | 摩斯码 | 字符 | 摩斯码 |
| --- | --- | --- | --- | --- | --- |
| 0 | ━ ━ ━ ━ ━ | 1 | ．━ ━ ━ ━ | 2 | ．．━ ━ ━ |
| 3 | ．．．━ ━ | 4 | ．．．．━ | 5 | ．．．．． |
| 6 | ━ ．．．． | 7 | ━ ━ ．．． | 8 | ━ ━ ━ ．． |
| 9 | ━ ━ ━ ━ ． | 

 | 

 | 

 | 

 |

标点符号的摩斯密码表

| 字符 | 摩斯码 | 字符 | 摩斯码 | 字符 | 摩斯码 |
| --- | --- | --- | --- | --- | --- |
| . | ．━ ．━ ．━ | : | ━ ━ ━ ．．． | , | ━ ━ ．．━ ━ |
| ; | ━ ．━ ．━ ． | ? | ．．━ ━ ．． | = | ━ ．．．━ |
| ' | ．━ ━ ━ ━ ． | / | ━ ．．━ ． | ! | ━ ．━ ．━ ━ |
| ━ | ━ ．．．．━ | _ | ．．━ ━ ．━ | " | ．━ ．．━ ． |
| ( | ━ ．━ ━ ． | ) | ━ ．━ ━ ．━ | $ | ．．．━ ．．━ |
| & | ． ．．． | @ | ．━ ━ ．━ ． | 

 | 

 |

<a name="jLH0j"></a>
### 培根密码
培根密码，又名倍康尼密码（Bacon's cipher）是由法兰西斯·培根发明的一种隐写术，它是一种本质上用二进制数设计的，没有用通常的0和1来表示，而是采用a和b，看到一串的a和b，并且五个一组，那么就是培根加密了。<br />第一种方式：

| 字符 | 培根密码 | 字符 | 培根密码 | 字符 | 培根密码 | 字符 | 培根密码 |
| --- | --- | --- | --- | --- | --- | --- | --- |
| A | aaaaa | H | aabbb | O | abbba | V | babab |
| B | aaaab | I | abaaa | P | abbbb | W | babba |
| C | aaaba | J | abaab | Q | baaaa | X | babbb |
| D | aaabb | K | ababa | R | baaab | Y | bbaaa |
| E | aabaa | L | ababb | S | baaba | Z | bbaab |
| F | aabab | M | abbaa | T | baabb | <br /> | <br /> |
| G | aabba | N | abbab | U | babaa | <br /> | <br /> |

第二种方式：

| 字符 | 培根密码 | 字符 | 培根密码 | 字符 | 培根密码 | 字符 | 培根密码 |
| --- | --- | --- | --- | --- | --- | --- | --- |
| a | AAAAA | h | AABBB | p | ABBBA | x | BABAB |
| b | AAAAB | i-j | ABAAA | q | ABBBB | y | BABBA |
| c | AAABA | k | ABAAB | r | BAAAA | z | BABBB |
| d | AAABB | l | ABABA | s | BAAAB | <br /> | <br /> |
| e | AABAA | m | ABABB | t | BAABA | <br /> | <br /> |
| f | AABAB | n | ABBAA | u-v | BAABB | <br /> | <br /> |
| g | AABBA | o | ABBAB | w | BABAA | <br /> | <br /> |

示例：

- 明文：kuaidaili
- 密文：ABABABABAAAAAAAABAAAAAABBAAAAAABAAAABABBABAAA
<a name="GsaqU"></a>
### 维吉尼亚密码
维吉尼亚密码是在凯撒密码基础上产生的一种加密方法，它将凯撒密码的全部25种位移排序为一张表，与原字母序列共同组成26行及26列的字母表。另外，维吉尼亚密码必须有一个密钥，这个密钥由字母组成，最少一个，最多可与明文字母数量相等。维吉尼亚密码表如下：<br />![](./img/1642727843249-d5c34cef-9a06-466c-a990-1257a98562d1.webp)<br />示例：

- 明文：I've got it.
- 密钥：ok
- 密文：W'fs qcd wd.

首先，密钥长度需要与明文长度相同，如果少于明文长度，则重复拼接直到相同。示例的明文长度为8个字母（非字母均被忽略），密钥会被程序补全为 okokokok，然后根据维吉尼亚密码表进行加密：明文第一个字母是 I，密钥第一个字母是 o，在表格中找到 I 列与 o 行相交点，字母 W 就是密文第一个字母，同理，v 列与 k 行交点字母是 F，e 列与 o 行交点字母是 S，以此类推。注意：维吉尼亚密码只对字母进行加密，不区分大小写，若文本中出现非字母字符会原样保留，如果输入多行文本，每行是单独加密的。
<a name="Abtp9"></a>
### 与佛论禅
字符串转换后，是一些佛语，在线体验：[https://keyfc.net/bbs/tools/tudoucode.aspx](https://keyfc.net/bbs/tools/tudoucode.aspx)<br />示例：

- 明文：Fcant
- 密文：佛曰：俱栗摩孕礙罰迦豆南怯神冥倒侄提栗怯集世栗故哆般
<a name="SonJp"></a>
### 当铺密码
当铺密码在 CTF 比赛题目中出现过。该加密算法是根据当前汉字有多少笔画出头，对应的明文就是数字几。<br />示例：

- 明文：王夫 井工 夫口 由中人 井中 夫夫 由中大
- 密文：67 84 70 123 82 77 125



