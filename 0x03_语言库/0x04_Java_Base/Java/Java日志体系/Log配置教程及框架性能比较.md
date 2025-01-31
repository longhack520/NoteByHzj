Java 日志
<a name="A8frz"></a>
## 一、摘要
不管是使用何种编程语言，何种框架，日志输出几乎无处不再，也是任何商业软件中必不可少的一部分。<br />总结起来，日志的用途大致可以归纳成以下三种：

- **问题追踪**：通过日志不仅仅包括程序的一些bug，也可以在安装配置时，通过日志可以发现问题。
- **状态监控**：通过实时分析日志，可以监控系统的运行状态，做到早发现问题、早处理问题。
- **安全审计**：审计主要体现在安全上，通过对日志进行分析，可以发现是否存在非授权的操作。

以 Java 编程语言为例，打印日志的方式有很多，例如通过`System.out.print()`方法将关键信息输出到控制台，也可以通过 JDK 自带的日志`Logger`类输出，虽然 JDK 从1.4开始支持日志输出，但是功能单一，无法更好的满足商业要求，于是诞生了很多第三方日志库，像熟悉的主流框架`log4j`、`log4j2`、`logback`等，提供的 API 功能都远胜 JDK 提供的`Logger`。
<a name="0k5fM"></a>
## 二、Log4j
<a name="45aeab7c"></a>
### 2.1、介绍
Log4j 是一种非常流行的日志框架，由`Ceki Gülcü`首创，之后将其开源贡献给 Apache 软件基金会。<br />Log4j 有三个主要的组件：`Loggers`(记录器)，`Appenders` (输出源)和`Layouts`(布局)。这里可简单理解为**日志类别**、**日志要输出的地方**和**日志以何种形式输出**。<br />综合使用这三个组件可以轻松地记录信息的类型和级别，并可以在运行时控制日志输出的样式和位置。<br />Log4j 的架构大致如下：<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1603414885669-49e7aad2-7021-4b0a-b019-b698c9c29e85.webp#align=left&display=inline&height=382&originHeight=382&originWidth=1080&size=0&status=done&style=shadow&width=1080)<br />当使用 Log4j 输出一条日志时，Log4j 自动通过不同的`Appender`（输出源）把同一条日志输出到不同的目的地。例如：

- **console**：输出到屏幕；
- **file**：输出到文件；
- **socket**：通过网络输出到远程计算机；
- **jdbc**：输出到数据库

在输出日志的过程中，通过`Filter`来过滤哪些`log`需要被输出，哪些`log`不需要被输出。<br />在`Loggers`(记录器)组件中，级别分五种：`DEBUG`、`INFO`、`WARN`、`ERROR`和`FATAL`。<br />这五个级别是有顺序的，`DEBUG` < `INFO` < `WARN` < `ERROR` < `FATAL`，分别用来指定这条日志信息的重要程度，明白这一点很重要，`Log4j`有一个规则：**只输出级别不低于设定级别的日志信息**。<br />假设`Loggers`级别设定为`INFO`，则`INFO`、`WARN`、`ERROR`和`FATAL`级别的日志信息都会输出，而级别比`INFO`低的`DEBUG`则不会输出。<br />最后，通过`Layout`来格式化日志信息，例如，自动添加日期、时间、方法名称等信息。<br />具体输出样式配置，可以参考如下内容Log4j2 - Layouts布局介绍
<a name="qDhjP"></a>
### 2.2、项目应用
以 Java 项目为例，在 Maven 的`pom.xml`中添加如下依赖！
<a name="8siV1"></a>
#### 2.2.1、添加 maven 依赖
```xml
<dependencies>
    <dependency>
        <groupId>org.slf4j</groupId>
        <artifactId>slf4j-api</artifactId>
        <version>1.6.6</version>
    </dependency>
    <dependency>
        <groupId>org.slf4j</groupId>
        <artifactId>slf4j-log4j12</artifactId>
        <version>1.6.6</version>
    </dependency>
    <dependency>
        <groupId>log4j</groupId>
        <artifactId>log4j</artifactId>
        <version>1.2.17</version>
    </dependency>
</dependencies>
```
<a name="7lwBK"></a>
#### 2.2.2、创建log4j配置
在实际应用中，要使`Log4j`在系统中运行须事先设定配置文件。<br />配置文件实际上也就是对`Logger`、`Appender`及`Layout`进行相应设定。<br />`Log4j`支持两种配置文件格式，一种是`XML`格式的文件，一种是`properties`属性文件，二选一。<br />创建一个log4j.xml或者log4j.properties，将其放入项目根目录下。
<a name="32LqP"></a>
###### 1、XML格式
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE log4j:configuration PUBLIC "-//APACHE//DTD LOG4J 1.2//EN" "log4j.dtd">
<log4j:configuration xmlns:log4j="http://jakarta.apache.org/log4j/">
    <!-- 控制台输出配置 -->
    <appender name="console" class="org.apache.log4j.ConsoleAppender">
        <!-- 目标为控制台 -->
        <param name="Target" value="System.out" />
        <layout class="org.apache.log4j.PatternLayout">
            <!-- 输出格式 -->
            <param name="ConversionPattern" value="%d{yyyy-MM-dd HH:mm:ss} [%t] %-5p %l %m%n" />
        </layout>
    </appender>
    <!-- 文件输出配置 -->
    <appender name="log_file" class="org.apache.log4j.DailyRollingFileAppender">
        <!-- 目标为文件 -->
        <param name="File" value="/logs/log/file.log" />
        <!-- 向文件追加输出 -->
        <param name="Append" value="true" />
        <!-- 每个小时生成一个log -->
        <param name="DatePattern" value="'.'yyyy-MM-dd-HH" />
        <layout class="org.apache.log4j.PatternLayout">
            <!-- 输出格式 -->
            <param name="ConversionPattern" value="%d{yyyy-MM-dd HH:mm:ss} [%t] %-5p %l %m%n" />
        </layout>
    </appender>
    <!-- Application Loggers -->
    <logger name="org.example">
        <level value="info" />
    </logger>
    <!-- 根目录 -->
    <!-- Root Logger -->
    <root>
        <priority value="info" />
        <appender-ref ref="console" />
        <appender-ref ref="log_file" />
    </root>
</log4j:configuration>
```
<a name="re7or"></a>
###### 2、properties格式
```
log4j.rootLogger=INFO,M,C,E
log4j.additivity.monitorLogger=false
# INFO级别文件输出配置
log4j.appender.M=org.apache.log4j.DailyRollingFileAppender
log4j.appender.M.File=/logs/info.log
log4j.appender.M.ImmediateFlush=false
log4j.appender.M.BufferedIO=true
log4j.appender.M.BufferSize=16384
log4j.appender.M.Append=true
log4j.appender.M.Threshold=INFO
log4j.appender.M.DatePattern='.'yyyy-MM-dd
log4j.appender.M.layout=org.apache.log4j.PatternLayout
log4j.appender.M.layout.ConversionPattern=%-d{yyyy-MM-dd HH:mm:ss} %p %l %m %n
# ERROR级别文件输出配置
log4j.appender.E=org.apache.log4j.DailyRollingFileAppender
log4j.appender.E.File=/logs/error.log
log4j.appender.E.ImmediateFlush=true
log4j.appender.E.Append=true
log4j.appender.E.Threshold=ERROR
log4j.appender.E.DatePattern='.'yyyy-MM-dd
log4j.appender.E.layout=org.apache.log4j.PatternLayout
log4j.appender.E.layout.ConversionPattern=%-d{yyyy-MM-dd HH:mm:ss} %p %l %m %n
# 控制台输出配置
log4j.appender.C=org.apache.log4j.ConsoleAppender
log4j.appender.C.Threshold=INFO
log4j.appender.C.layout=org.apache.log4j.PatternLayout
log4j.appender.C.layout.ConversionPattern=%d{yyyy-MM-dd HH:mm:ss} [%t] %-5p %l %m %n
```
<a name="q4JZM"></a>
#### 2.2.3、log4j使用
在需要打印日志的类中，引入`Logger`类，在需要的地方打印即可！
```java
package org.example.log4j.service;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
public class LogPrintUtil {
    /**log静态常量*/
    private static final Logger logger = LoggerFactory.getLogger(LogPrintUtil.class);
    public static void main(String[] args){
            logger.info("info信息");
            logger.warn("warn信息");
            logger.error("error信息");
    }
}
```
当然还可以这样写
```java
if(logger.isInfoEnabled()) {
    logger.info("info信息");
}
if(logger.isWarnEnabled()) {
    logger.warn("warn信息");
}
```
<a name="KBNrt"></a>
#### 2.2.4、isInfoEnabled()的作用
简单来说，**在某些场景下**，用`isInfoEnabled()`方法判断下是能提升性能的！<br />例如打印这段内容`logger.info("User:" + userId + appId)`，程序在打印这行代码时，先对内容`("User:" + userId + appId)`进行字符串拼接，然后再输出。<br />如果当前配置文件中日志输出级别是`info`，是直接输出的，当日志输出级别是`error`时，`logger.info()`的内容时不输出的，但是却进行了字符串拼接，如果加上`if(logger.isInfoEnabled())`进行一次判定，`logger.info()`就不会执行，从而更好的提升性能，这个尤其是在高并发和复杂`log`打印情况下提升非常显著。<br />另外，`ERROR`及其以上级别的log信息是一定会被输出的，所以只有`logger.isDebugEnabled`、`logger.isInfoEnabled`和`logger.isWarnEnabled()`方法，而没有`logger.isErrorEnabled`方法。
<a name="e0Nks"></a>
## 三、Log4j2
<a name="bEA6U"></a>
### 3.1、介绍
log4j2 是 log4j 1.x 的升级版，参考了 logback 的一些优秀的设计，并且修复了一些问题，因此带来了一些重大的提升，主要特点有：<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1603414885799-c223b745-43da-4035-8ec5-a5358ca28ed0.webp#align=left&display=inline&height=267&originHeight=267&originWidth=761&size=0&status=done&style=shadow&width=761)

- **异常处理**：在logback中，Appender中的异常不会被应用感知到，但是在log4j2中，提供了一些异常处理机制。
- **性能提升**， log4j2相较于log4j 1和logback都具有很明显的性能提升，后面会有官方测试的数据。
- **自动重载配置**：参考了logback的设计，当然会提供自动刷新参数配置，最实用的就是在生产上可以动态的修改日志的级别而不需要重启应用——那对监控来说，是非常敏感的。
- **无垃圾机制**：log4j2在大部分情况下，都可以使用其设计的一套无垃圾机制，避免频繁的日志收集导致的jvm gc。
<a name="vL6ql"></a>
### 3.2、项目应用
<a name="d4jBs"></a>
#### 3.2.1、添加 maven 依赖
```xml
<dependencies>
    <!-- slf4j核心包 -->
    <dependency>
        <groupId>org.slf4j</groupId>
        <artifactId>slf4j-api</artifactId>
        <version>1.7.13</version>
    </dependency>
    <!--用于与common-log保持桥接 -->
    <dependency>
        <groupId>org.slf4j</groupId>
        <artifactId>jcl-over-slf4j</artifactId>
        <version>1.7.13</version>
        <scope>runtime</scope>
    </dependency>
    <!--核心log4j2jar包 -->
    <dependency>
        <groupId>org.apache.logging.log4j</groupId>
        <artifactId>log4j-api</artifactId>
        <version>2.4.1</version>
    </dependency>
    <dependency>
        <groupId>org.apache.logging.log4j</groupId>
        <artifactId>log4j-core</artifactId>
        <version>2.4.1</version>
    </dependency>
    <!--用于与slf4j保持桥接 -->
    <dependency>
        <groupId>org.apache.logging.log4j</groupId>
        <artifactId>log4j-slf4j-impl</artifactId>
        <version>2.4.1</version>
    </dependency>
    <!--需要使用log4j2的AsyncLogger需要包含disruptor -->
    <dependency>
        <groupId>com.lmax</groupId>
        <artifactId>disruptor</artifactId>
        <version>3.2.0</version>
    </dependency>
</dependencies>
```
<a name="8a7480d2"></a>
#### 3.2.2、创建log4j2配置
在项目的根目录下创建一个`log4j2.xml`的文件，与`log4j`相比，`log4j2`的异步输出日志性能非常强劲，配置如下：
<a name="nNXiB"></a>
##### 1、同步输出日志
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!-- status : 这个用于设置log4j2自身内部的信息输出,可以不设置,当设置成trace时。
 注：本配置文件的目标是将不同级别的日志输出到不同文件，最大1MB一个文件， 
    文件数据达到最大值时，旧数据会被压缩并放进指定文件夹 ，最多存放20个文件-->
<Configuration status="error">
    <!-- 配置日志文件输出目录，此配置将日志输出到根目录下的指定文件夹 -->
    <Properties>
        <Property name="fileDir">/logs/log4j2</Property>
        <Property name="fileHistory">/logs/log4j2/history</Property>
    </Properties>
    <Appenders>
        <!-- 优先级从高到低分别是 OFF、FATAL、ERROR、WARN、INFO、DEBUG、ALL -->
        <!-- 单词解释：Match：匹配 DENY：拒绝 Mismatch：不匹配 ACCEPT：接受 -->
        <!-- DENY，日志将立即被抛弃不再经过其他过滤器；NEUTRAL，有序列表里的下个过滤器过接着处理日志；ACCEPT，日志会被立即处理，不再经过剩余过滤器。 -->
        <!--输出日志的格式
        %d{yyyy-MM-dd HH:mm:ss, SSS} : 日志生产时间
        %t 输出当前线程名称
        %-5level 输出日志级别，-5表示左对齐并且固定输出5个字符，如果不足在右边补0
        %logger 输出logger名称，因为Root Logger没有名称，所以没有输出
        %msg 日志文本
        %n 换行
        其他常用的占位符有：
        %F 输出所在的类文件名，如Client.java
        %L 输出行号
        %M 输出所在方法名
        %l  输出语句所在的行数, 包括类名、方法名、文件名、行数
         -->
        <!--这个输出控制台的配置，这里输出all信息到System.out -->
        <console name="Console" target="SYSTEM_OUT">
            <!-- 输出日志的格式 -->
            <PatternLayout charset="UTF-8" pattern="%d{yyyy-MM-dd HH:mm:ss} [%t] %-5level %l %msg%n" />
        </console>
        <!--这个输出文件的配置，这里输出info信息到junbao_info.log -->
        <RollingFile name="RollingFileInfo" fileName="${fileDir}/info.log" filePattern="${fileHistory}/info/%d{yyyy-MM-dd}-%i.log">
            <!-- 此Filter意思是，只输出info级别的数据 DENY，日志将立即被抛弃不再经过其他过滤器；NEUTRAL，有序列表里的下个过滤器过接着处理日志； 
                    ACCEPT，日志会被立即处理，不再经过剩余过滤器。 -->
            <ThresholdFilter level="info" onMatch="ACCEPT" onMismatch="DENY" />
            <PatternLayout charset="UTF-8" pattern="%d{yyyy-MM-dd HH:mm:ss} [%t] %-5level %l %msg%n" />
            <Policies>
            <!-- 如果启用此配置，则日志会按文件名生成新文件， 即如果filePattern配置的日期格式为 %d{yyyy-MM-dd HH} 
                    ，则每小时生成一个压缩文件， 如果filePattern配置的日期格式为 %d{yyyy-MM-dd} ，则天生成一个压缩文件,默认为1 -->
                <TimeBasedTriggeringPolicy />
                <!-- 每个日志文件最大1MB,超过1MB生产新的文件 ; -->
                <SizeBasedTriggeringPolicy size="100MB" />
            </Policies>
             <!--文件夹下最多的文件个数-->  
            <DefaultRolloverStrategy max="20" />
        </RollingFile>
        <RollingFile name="RollingFileWarn" fileName="${fileDir}/warn.log" filePattern="${fileHistory}/warn/%d{yyyy-MM-dd}-%i.log">
            <ThresholdFilter level="warn" onMatch="ACCEPT" onMismatch="DENY" />
            <PatternLayout charset="UTF-8" pattern="%d{yyyy-MM-dd HH:mm:ss} [%t] %-5level %l %msg%n" />
            <Policies>
                <TimeBasedTriggeringPolicy />
                <SizeBasedTriggeringPolicy size="100MB" />
            </Policies>
            <DefaultRolloverStrategy max="20" />
        </RollingFile>
        <RollingFile name="RollingFileError" fileName="${fileDir}/error.log" filePattern="${fileHistory}/error/%d{yyyy-MM-dd}-%i.log">
            <ThresholdFilter level="error" onMatch="ACCEPT" onMismatch="DENY" />
            <PatternLayout charset="UTF-8" pattern="%d{yyyy-MM-dd HH:mm:ss} [%t] %-5level %l %msg%n" />
            <Policies>
                <TimeBasedTriggeringPolicy />
                <SizeBasedTriggeringPolicy size="100MB" />
            </Policies>
            <DefaultRolloverStrategy max="20" />
        </RollingFile>
    </Appenders>
    <!--然后定义logger，只有定义了logger并引入的appender，appender才会生效 -->
    <Loggers>
        <!--全异步输出info级以上的日志信息--> 
        <!-- <asyncRoot level="info" includeLocation="true">
            <appender-ref ref="Console" />
            <appender-ref ref="RollingFileInfo" />
        </asyncRoot> -->
        <!--同步输出info级以上的日志信息--> 
        <root level="info" includeLocation="true">
            <appender-ref ref="Console" />
        </root>
    </Loggers>
</Configuration>
```
<a name="BPsiz"></a>
##### 2、异步输出日志
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!-- status : 这个用于设置log4j2自身内部的信息输出,可以不设置,当设置成trace时。
 注：本配置文件的目标是将不同级别的日志输出到不同文件，最大1MB一个文件， 
    文件数据达到最大值时，旧数据会被压缩并放进指定文件夹 ，最多存放20个文件-->
<Configuration status="error">
    <!-- 配置日志文件输出目录，此配置将日志输出到根目录下的指定文件夹 -->
    <Properties>
        <Property name="fileDir">/logs/log4j2</Property>
        <Property name="fileHistory">/logs/log4j2/history</Property>
    </Properties>
    <Appenders>
        <!-- 优先级从高到低分别是 OFF、FATAL、ERROR、WARN、INFO、DEBUG、ALL -->
        <!-- 单词解释：Match：匹配 DENY：拒绝 Mismatch：不匹配 ACCEPT：接受 -->
        <!-- DENY，日志将立即被抛弃不再经过其他过滤器；NEUTRAL，有序列表里的下个过滤器过接着处理日志；ACCEPT，日志会被立即处理，不再经过剩余过滤器。 -->
        <!--输出日志的格式
        %d{yyyy-MM-dd HH:mm:ss, SSS} : 日志生产时间
        %t 输出当前线程名称
        %-5level 输出日志级别，-5表示左对齐并且固定输出5个字符，如果不足在右边补0
        %logger 输出logger名称，因为Root Logger没有名称，所以没有输出
        %msg 日志文本
        %n 换行
        其他常用的占位符有：
        %F 输出所在的类文件名，如Client.java
        %L 输出行号
        %M 输出所在方法名
        %l  输出语句所在的行数, 包括类名、方法名、文件名、行数
         -->
        <!--这个输出控制台的配置，这里输出all信息到System.out -->
        <console name="Console" target="SYSTEM_OUT">
            <!-- 输出日志的格式 -->
            <PatternLayout charset="UTF-8" pattern="%d{yyyy-MM-dd HH:mm:ss} [%t] %-5level %l %msg%n" />
        </console>
        <!--这个输出文件的配置，这里输出info信息到junbao_info.log -->
        <RollingFile name="RollingFileInfo" fileName="${fileDir}/info.log" filePattern="${fileHistory}/info/%d{yyyy-MM-dd}-%i.log">
            <!-- 此Filter意思是，只输出info级别的数据 DENY，日志将立即被抛弃不再经过其他过滤器；NEUTRAL，有序列表里的下个过滤器过接着处理日志； 
                    ACCEPT，日志会被立即处理，不再经过剩余过滤器。 -->
            <ThresholdFilter level="info" onMatch="ACCEPT" onMismatch="DENY" />
            <PatternLayout charset="UTF-8" pattern="%d{yyyy-MM-dd HH:mm:ss} [%t] %-5level %l %msg%n" />
            <Policies>
            <!-- 如果启用此配置，则日志会按文件名生成新文件， 即如果filePattern配置的日期格式为 %d{yyyy-MM-dd HH} 
                    ，则每小时生成一个压缩文件， 如果filePattern配置的日期格式为 %d{yyyy-MM-dd} ，则天生成一个压缩文件,默认为1 -->
                <TimeBasedTriggeringPolicy />
                <!-- 每个日志文件最大1MB,超过1MB生产新的文件 ; -->
                <SizeBasedTriggeringPolicy size="100MB" />
            </Policies>
             <!--文件夹下最多的文件个数-->  
            <DefaultRolloverStrategy max="20" />
        </RollingFile>
        <RollingFile name="RollingFileWarn" fileName="${fileDir}/warn.log" filePattern="${fileHistory}/warn/%d{yyyy-MM-dd}-%i.log">
            <ThresholdFilter level="warn" onMatch="ACCEPT" onMismatch="DENY" />
            <PatternLayout charset="UTF-8" pattern="%d{yyyy-MM-dd HH:mm:ss} [%t] %-5level %l %msg%n" />
            <Policies>
                <TimeBasedTriggeringPolicy />
                <SizeBasedTriggeringPolicy size="100MB" />
            </Policies>
            <DefaultRolloverStrategy max="20" />
        </RollingFile>
        <RollingFile name="RollingFileError" fileName="${fileDir}/error.log" filePattern="${fileHistory}/error/%d{yyyy-MM-dd}-%i.log">
            <ThresholdFilter level="error" onMatch="ACCEPT" onMismatch="DENY" />
            <PatternLayout charset="UTF-8" pattern="%d{yyyy-MM-dd HH:mm:ss} [%t] %-5level %l %msg%n" />
            <Policies>
                <TimeBasedTriggeringPolicy />
                <SizeBasedTriggeringPolicy size="100MB" />
            </Policies>
            <DefaultRolloverStrategy max="20" />
        </RollingFile>
    </Appenders>
    <!--然后定义logger，只有定义了logger并引入的appender，appender才会生效 -->
    <Loggers>
        <!--全异步输出info级以上的日志信息--> 
        <asyncRoot level="info" includeLocation="true">
            <appender-ref ref="Console" />
            <appender-ref ref="RollingFileInfo" />
        </asyncRoot>
        <!--同步输出info级以上的日志信息--> 
        <!-- <root level="info" includeLocation="true">
            <appender-ref ref="Console" />
        </root> -->
    </Loggers>
</Configuration>
```
详细 API 可以参考官方网站！
<a name="bEPhj"></a>
#### 3.2.3、log4j2使用
与 log4j 类似，直接在需要位置打印日志即可
```java
package org.example.log4j.service;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
public class LogPrintUtil {
    /**log静态常量*/
    private static final Logger logger = LoggerFactory.getLogger(LogPrintUtil.class);
    public static void main(String[] args){
            logger.info("info信息");
            logger.warn("warn信息");
            logger.error("error信息");
    }
}
```
<a name="L73QV"></a>
## 四、Logback
<a name="KOIPE"></a>
### 4.1、介绍
Logback 也是用 java 编写一款非常热门的日志开源框架，由 log4j 创始人写的，性能比 log4j 要好!<br />logback 主要分为3个模块：

- logback-core：核心代码模块
- logback-classic：log4j的一个改良版本，同时实现了slf4j的接口，这样如果之后要切换其他日志组件也是一件很容易的事
- logback-access：访问模块与Servlet容器集成提供通过Http来访问日志的功能
<a name="F3ti4"></a>
### 4.2、项目应用
<a name="g97ow"></a>
#### 4.2.1、添加 maven 依赖
```xml
<!--这个依赖直接包含了 logback-core 以及 slf4j-api的依赖-->
<dependency>
    <groupId>ch.qos.logback</groupId>
    <artifactId>logback-classic</artifactId>
    <version>1.2.3</version>
</dependency>
<!-- 支持在xml中写判断标签 -->
<dependency>
    <groupId>org.codehaus.janino</groupId>
    <artifactId>janino</artifactId>
    <version>2.7.8</version>
</dependency>
```
<a name="UH1Rb"></a>
#### 4.2.2、创建logback配置文件
<a name="H1RIh"></a>
##### 1、配置说明
logback在启动的时候，会按照下面的顺序加载配置文件：

- 如果java程序启动时指定了`logback.configurationFile`属性，就用该属性指定的配置文件。如j`ava -Dlogback.configurationFile=/path/to/mylogback.xml Test`，这样执行`Test`类的时候就会加载`/path/to/mylogback.xml`配置
- 在`classpath`中查找`logback.groovy`文件
- 在`classpath`中查找`logback-test.xml`文件
- 在`classpath`中查找`logback.xml`文件
- 如果是`jdk6+`,那么会调用S`erviceLoader`查找 `com.qos.logback.classic.spi.Configurator`接口的第一个实现类
- 自动使用`ch.qos.logback.classic.BasicConfigurator`，在控制台输出日志

上面的顺序表示优先级，使用`java -D`配置的优先级最高，只要获取到配置后就不会再执行下面的流程。相关代码可以看`ContextInitializer#autoConfig()`方法。
<a name="8MQ7q"></a>
##### 2、同步输出日志
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!-- scan:当此属性设置为true时，配置文件如果发生改变，将会被重新加载，默认值为true。scanPeriod:设置监测配置文件是否有修改的时间间隔，如果没有给出时间单位，默认单位是毫秒。当scan为true时，此属性生效。默认的时间间隔为1分钟。 
    debug:当此属性设置为true时，将打印出logback内部日志信息，实时查看logback运行状态。默认值为false。 -->
<configuration scan="true" scanPeriod="60 seconds" debug="false">
    <!-- 运行环境，dev:开发，test:测试，pre:预生产，pro:生产 -->
    <property name="system_host" value="dev" />
    <property file="system.properties" />
    <!-- 上下文变量设置,用来定义变量值,其中name的值是变量的名称，value的值时变量定义的值。 通过<property>定义的值会被插入到logger上下文中。定义变量后，可以使“${}”来使用变量。 -->
    <property name="CONTEXT_NAME" value="logback-test" />
    <!-- 日志文件存放路径设置，绝对路径 -->
    <property name="logs.dir" value="/opt/logs" />
    <!-- 日志文件存放路径设置，tomcat路径 -->
    <property name="logs.dir" value="${catalina.base}/logs" />
    <!-- 定义日志文件 相对输入位置 -->  
    <property name="log_dir" value="log" />
    <!-- 日志输出格式设置 -->
    <!-- 
    %d{yyyy-MM-dd HH:mm:ss} [%level] - %msg%n
      Logger: %logger
      Class: %class
      File: %file
      Caller: %caller
      Line: %line
      Message: %m
      Method: %M
      Relative: %relative
      Thread: %thread
      Exception: %ex
      xException: %xEx
      nopException: %nopex
      rException: %rEx
      Marker: %marker
      newline:%n
    -->
    <property name="CUSTOM_LOG_PATTERN"
        value="%d{yyyy-MM-dd HH:mm:ss} [%thread] %-5level %logger{90} - %msg%n" />
    <!-- 上下文名称：<contextName>, 每个logger都关联到logger上下文， 默认上下文名称为“default”。但可以使用<contextName>设置成其他名字，用于区分不同应用程序的记录。 
        一旦设置，不能修改。 -->
    <contextName>${CONTEXT_NAME}</contextName>
    <!-- <appender>是<configuration>的子节点，是负责写日志的组件。 有两个必要属性name和class。name指定appender名称， 
        class指定appender的实现类。 -->
    <appender name="console" class="ch.qos.logback.core.ConsoleAppender">
        <!-- 对日志进行格式化。 -->
        <encoder>
            <pattern>${CUSTOM_LOG_PATTERN}</pattern>
            <charset>UTF-8</charset>
        </encoder>
    </appender>
    <appender name="file"
        class="ch.qos.logback.core.rolling.RollingFileAppender">
        <!-- 按天来回滚，如果需要按小时来回滚，则设置为{yyyy-MM-dd_HH} -->
        <rollingPolicy class="ch.qos.logback.core.rolling.TimeBasedRollingPolicy">
            <fileNamePattern>log/testC.%d{yyyy-MM-dd}.%i.log</fileNamePattern>
            <!-- 如果按天来回滚，则最大保存时间为30天，30天之前的都将被清理掉 -->
            <maxHistory>30</maxHistory>
            <!-- 按时间回滚的同时，按文件大小来回滚 -->
            <timeBasedFileNamingAndTriggeringPolicy
                class="ch.qos.logback.core.rolling.SizeAndTimeBasedFNATP">
                <maxFileSize>100MB</maxFileSize>
            </timeBasedFileNamingAndTriggeringPolicy>
        </rollingPolicy>
        <!-- 过滤器，只记录WARN级别的日志 -->
        <!-- 果日志级别等于配置级别，过滤器会根据onMath 和 onMismatch接收或拒绝日志。 -->
        <filter class="ch.qos.logback.classic.filter.LevelFilter">
            <!-- 设置过滤级别 -->
            <level>WARN</level>
            <!-- 用于配置符合过滤条件的操作 -->
            <onMatch>ACCEPT</onMatch>
            <!-- 用于配置不符合过滤条件的操作 -->
            <onMismatch>DENY</onMismatch>
        </filter>
        <!-- 日志输出格式 -->
        <encoder>
            <pattern>${CUSTOM_LOG_PATTERN}</pattern>
            <charset>UTF-8</charset>
        </encoder>
    </appender>
 
    <appender name="log_file"
        class="ch.qos.logback.core.rolling.RollingFileAppender">
        <!-- 被写入的文件名，可以是相对目录，也可以是绝对目录，如果上级目录不存在会自动创建，没有默认值。 -->
        <file>${logs.dir}/logback-test.log</file>
        <!-- 按照固定窗口模式生成日志文件，当文件大于20MB时，生成新的日志文件。窗口大小是1到3，当保存了3个归档文件后，将覆盖最早的日志 -->
        <rollingPolicy class="ch.qos.logback.core.rolling.FixedWindowRollingPolicy">
            <!-- 必须包含“%i”例如，假设最小值和最大值分别为1和2，命名模式为 mylog%i.log,会产生归档文件mylog1.log和mylog2.log。还可以指定文件压缩选项，例如，mylog%i.log.gz 
                或者 没有log%i.log.zip -->
            <FileNamePattern>${logs.dir}/logback-test.%i.log</FileNamePattern>
            <!-- 窗口索引最小值 -->
            <minIndex>1</minIndex>
            <!-- 窗口索引最大值 -->
            <maxIndex>3</maxIndex>
        </rollingPolicy>
        <!-- 日志级别过滤器 -->
        <filter class="ch.qos.logback.classic.filter.LevelFilter">
            <!-- 日志级别过滤器 -->
            <level>INFO</level>
            <!-- 符合要求的日志级别，过滤，ACCEPT:接受 -->
            <onMatch>ACCEPT</onMatch>
            <!-- 不符合要求的日志级别，过滤，DENY:拒绝 -->
            <onMismatch>DENY</onMismatch>
        </filter>
        <!-- 激活滚动的条件。 -->
        <triggeringPolicy
            class="ch.qos.logback.core.rolling.SizeBasedTriggeringPolicy">
            <!-- 活动文件的大小，默认值是10MB -->
            <maxFileSize>30MB</maxFileSize>
        </triggeringPolicy>
        <!-- 对记录事件进行格式化。 -->
        <encoder>
            <pattern>${CUSTOM_LOG_PATTERN}</pattern>
            <charset>UTF-8</charset>
        </encoder>
    </appender>
 
    <!-- 异步输出 -->
    <appender name="ASYNC_logback" class="ch.qos.logback.classic.AsyncAppender">
        <!-- 不丢失日志.默认的,如果队列的80%已满,则会丢弃TRACT、DEBUG、INFO级别的日志 -->
        <!-- <discardingThreshold>0</discardingThreshold> -->
        <!-- 更改默认的队列的深度,该值会影响性能.默认值为256 -->
        <!-- <queueSize>256</queueSize> -->
        <!-- 添加附加的appender,最多只能添加一个 -->
        <appender-ref ref="log_file" />
    </appender>
    <!-- 指定包输出路径 -->
    <!-- 用来设置某一个 包 或者具体的某一个 类 的日志打印级别、以及指定<appender>, name:用来指定受此logger约束的某一个包或者具体的某一个类。 
        level:用来设置打印级别，大小写无关：TRACE, DEBUG, INFO, WARN, ERROR, ALL 和 OFF，还有一个特俗值INHERITED或者同义词NULL，代表强制执行上级的级别。如果未设置此属性，那么当前loger将会继承上级的级别。 
        additivity:是否向上级logger传递打印信息。默认是true。(这个logger的上级就是上面的root) <logger>可以包含零个或多个<appender-ref>元素，标识这个appender将会添加到这个logger。 -->
    <logger name="org.logback.test" level="DEBUG" additivity="true">
        <appender-ref ref="stdout" />
    </logger>
    <!-- 特殊的<logger>元素，是根logger。只有一个level属性，应为已经被命名为"root". level:设置打印级别，大小写无关：TRACE, 
        DEBUG, INFO, WARN, ERROR, ALL 和 OFF，不能设置为INHERITED或者同义词NULL。默认是DEBUG。 <root>可以包含零个或多个<appender-ref>元素，标识这个appender将会添加到这个loger。 -->
    <root>
        <level value="WARN" />
        <!-- if表达式，需要Janino jar -->
        <!-- Janino 2.6.0版本开始，除了janino.jar之外， commons-compiler.jar也需要在类路径中 -->
        <if condition='property("system_host").contains("dev")'>
            <then>
                <appender-ref ref="stdout" />
            </then>
        </if>
        <appender-ref ref="file" />
    </root>
</configuration>
```
注意：`logback`如果配置要输出行号，性能会明显降低，如果不是必须，建议不要配置！
<a name="2CpaL"></a>
#### 4.2.3、logback使用
```
package org.example.logback.service;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
public class LogPrintUtil {
    /**log静态常量*/
    private static final Logger logger = LoggerFactory.getLogger(LogPrintUtil.class);
    public static void main(String[] args){
         logger.info("info信息");
            logger.warn("warn信息");
            logger.error("error信息");
    }
}
```
<a name="4b167bbb"></a>
## 五、SLF4J桥接
上面代码使用时，都使用的是`private static final Logger logger = LoggerFactory.getLogger(LogPrintUtil.class)`这个，其中都来自`org.slf4j`包，`SLF4J`是啥？有什么作用呢？<br />`SLF4J`本身并不输出日志，最大的特色是：它可以通过适配的方式挂接不同的日志系统，属于一个日志接口。<br />如果项目适配到`log4j`就使用`log4j`日志库进行输出；如果适配到`logback`就使用`logback`日志库进行输出；如果适配到`log4j2`就使用`log4j2`日志库进行输出。<br />这样最大的好处，就是想将项目从`log4j`换成`log4j2`的时候，只需要在项目`pom.xml`中进行桥接适配即可，**不用修改具体需要打印日志的代码**！
<a name="088gn"></a>
## 六、三大主流日志框架性能比较
三个日志框架的日志输出性能比较，本文以**10000**条数据进行打印，比较`log4j`、`log4j2`、`logback`日志的输出时间。<br />每个电脑的配置不一样，测试的结果也不一样，结果是真实的。

- 同步输出

![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1603414885689-c0b48821-6fd4-4590-bc70-e19bd863da6d.webp#align=left&display=inline&height=264&originHeight=264&originWidth=1000&size=0&status=done&style=shadow&width=1000)

- 异步输出

![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1603414885698-d8e49981-dfc9-4880-ac77-058e37c18203.webp#align=left&display=inline&height=270&originHeight=270&originWidth=994&size=0&status=done&style=shadow&width=994)<br />从测试结果上可以看出：

- 不建议生产环境进行控制台输出；
- 在纯文件输出的环境下，`logback`的输出优于`log4j2`，而`log4j2`要优于`log4j`，如果要进行生产环境的部署，建议采用`logback`，如果是使用`log4j2`，建议使用异步方式进行输出，输出结果基本是实时输出；

最后需要注意的地方是：`log`有风险，输出需谨慎！<br />由于输出`log`过程需要进行磁盘操作，且`log4j`为了保证`log`输出过程的线程安全性而使用同步锁，就使得输出`log`成为很耗时的操作，所以`log`信息一定要言简意赅，不要输出一些无用的`log`。
