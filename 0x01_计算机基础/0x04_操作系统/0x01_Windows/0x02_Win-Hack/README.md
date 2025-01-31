# Windows-Hack-Programming

Detailed explanation of Windows hacking programming technology, Include Windows User Programming(Ring3) And Windows Kernel Rootkit Programming(Ring0)

**书籍内容简介**

《WINDOWS黑客编程技术详解》，作者甘迪文，2018年12月由人民邮电出版社出版，是一本面向黑客编程初学者的书，较为全面的地总结黑客编程技术。其内容重在实践，着重剖析技术实现原理，向读者讲解黑客编程技术的实现方法。

本书介绍的是些黑客编程的基础技术，涉及用户层下的Windows编程和内核层下的Rootkit编程。全书分为用户篇和内核篇两部分，用户篇包括11章，配套49个示例程序源码；内核篇包括7章，配套28个示例程序源码。本书每个技术都有详细的实现原理分析，以及对应的示例代码（配套代码均支持32位和64位Windows 7、Windows 8.1及Windows 10系统），帮助初学者建立起黑客编程技术的基础技能。

本书面向对计算机系统安全开发感兴趣，或者希望提升安全开发水平的读者，以及恶意代码分析研究方面的安全人员。

**勘误收集及相关开发软件下载**

https://www.writebug.com/explore/QA/CTYBTMQK

**书籍封面（正面）**

![book](./book.jpg)

**目录**

- 第1篇 用户篇

  - 第1章 开发环境
    - 1.1 环境安装
    - 1.2 工程项目设置
    - 1.3 关于Debug模式和Release模式的小提示

  - 第2章 基础技术
    - 2.1 运行单一实例
    - 2.2 DLL延时加载
    - 2.3 资源释放

  - 第3章 注入技术
    - 3.1 全局钩子注入
    - 3.2 远线程注入
    - 3.3 突破SESSION 0隔离的远线程注入
    - 3.4 APC注入

  - 第4章 启动技术
    - 4.1 创建进程API
    - 4.2 突破SESSION 0隔离创建用户进程
    - 4.3内存直接加载运行

  - 第5章 自启动技术
    - 5.1 注册表
    - 5.2 快速启动目录
    - 5.3 计划任务
    - 5.4 系统服务

  - 第6章 提权技术
    - 6.1 进程访问令牌权限提升
    - 6.2 Bypass UAC

  - 第7章 隐藏技术
    - 7.1 进程伪装
    - 7.2傀儡进程
    - 7.3 进程隐藏
    - 7.4 DLL劫持

  - 第8章 压缩技术
    - 8.1 数据压缩API
    - 8.2 ZLIB压缩库

  - 第9章 加密技术
    - 9.1 Windows自带的加密库
    - 9.2 Crypto++密码库

  - 第10章 传输技术
    - 10.1 Socket通信
    - 10.2 FTP通信
    - 10.3 HTTP通信
    - 10.4 HTTPS通信

  - 第11章 功能技术
    - 11.1 进程遍历
    - 11.2 文件遍历
    - 11.3 桌面截屏
    - 11.4 按键记录
    - 11.5 远程CMD
    - 11.6 U盘监控
    - 11.7 文件监控
    - 11.8 自删除

- 第2篇 内核篇

  - 第12章 开发环境
    - 12.1 环境安装
    - 12.2 驱动程序开发与调试
    - 12.3 驱动无源码调试
    - 12.4 32位和64位驱动开发

  - 第13章 文件管理技术
    - 13.1 文件管理之内核API
    - 13.2 文件管理之IRP
    - 13.3 文件管理之NTFS解析

  - 第14章 注册表管理技术
    - 14.1 注册表管理之内核API
    - 14.2 注册表管理之HIVE文件解析

  - 第15章 HOOK技术
    - 15.1 SSDT Hook
    - 15.2过滤驱动

  - 第16章 监控技术
    - 16.1 进程创建监控
    - 16.2 模块加载监控
    - 16.3 注册表监控
    - 16.4 对象监控
    - 16.5 Minifilter文件监控
    - 16.6 WFP网络监控

  - 第17章 反监控技术
    - 17.1 反进程创建监控
    - 17.2 反线程创建监控
    - 17.3 反模块加载监控
    - 17.4 反注册表监控
    - 17.5 反对象监控
    - 17.6 反Minifilter文件监控

  - 第18章 功能技术
    - 18.1 过PatchGuard的驱动隐藏
    - 18.2 过PatchGuard的进程隐藏
    - 18.3 TDI网络通信
    - 18.4 强制结束进程
    - 18.5 文件保护
    - 18.6 文件强删

- 附录 函数一览表



