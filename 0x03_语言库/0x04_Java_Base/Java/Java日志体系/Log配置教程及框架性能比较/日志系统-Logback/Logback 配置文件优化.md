Java日志Logback
<a name="bhN4I"></a>
### 1、目标

1. 日志输出到文件并根据`LEVEL`级别将日志分类保存到不同文件
2. 通过异步输出日志减少磁盘IO提高性能
3. 异步输出日志的原理
<a name="cidWI"></a>
### 2、配置文件logback-spring.xml
SpringBoot工程自带logback和slf4j的依赖，所以重点放在编写配置文件上，需要引入什么依赖，日志依赖冲突统都不需要管了。logback框架会默认加载classpath下命名为logback-spring或logback的配置文件。将所有日志都存储在一个文件中文件大小也随着应用的运行越来越大并且不好排查问题，正确的做法应该是将error日志和其他日志分开，并且不同级别的日志根据时间段进行记录存储。
```xml
<?xml version="1.0" encoding="utf-8"?>
<configuration>
    <property resource="logback.properties"/>
    <appender name="CONSOLE-LOG" class="ch.qos.logback.core.ConsoleAppender">
        <layout class="ch.qos.logback.classic.PatternLayout">
            <pattern>[%d{yyyy-MM-dd' 'HH:mm:ss.sss}] [%C] [%t] [%L] [%-5p] %m%n</pattern>
        </layout>
    </appender>
    <!--获取比info级别高(包括info级别)但除error级别的日志-->
    <appender name="INFO-LOG" class="ch.qos.logback.core.rolling.RollingFileAppender">
        <filter class="ch.qos.logback.classic.filter.LevelFilter">
            <level>ERROR</level>
            <onMatch>DENY</onMatch>
            <onMismatch>ACCEPT</onMismatch>
        </filter>
        <encoder>
            <pattern>[%d{yyyy-MM-dd' 'HH:mm:ss.sss}] [%C] [%t] [%L] [%-5p] %m%n</pattern>
        </encoder>

        <!--滚动策略-->
        <rollingPolicy class="ch.qos.logback.core.rolling.TimeBasedRollingPolicy">
            <!--路径-->
            <fileNamePattern>${LOG_INFO_HOME}//%d.log</fileNamePattern>
            <maxHistory>30</maxHistory>
        </rollingPolicy>
    </appender>
    <appender name="ERROR-LOG" class="ch.qos.logback.core.rolling.RollingFileAppender">
        <filter class="ch.qos.logback.classic.filter.ThresholdFilter">
            <level>ERROR</level>
        </filter>
        <encoder>
            <pattern>[%d{yyyy-MM-dd' 'HH:mm:ss.sss}] [%C] [%t] [%L] [%-5p] %m%n</pattern>
        </encoder>
        <!--滚动策略-->
        <rollingPolicy class="ch.qos.logback.core.rolling.TimeBasedRollingPolicy">
            <!--路径-->
            <fileNamePattern>${LOG_ERROR_HOME}//%d.log</fileNamePattern>
            <maxHistory>30</maxHistory>
        </rollingPolicy>
    </appender>

    <root level="info">
        <appender-ref ref="CONSOLE-LOG" />
        <appender-ref ref="INFO-LOG" />
        <appender-ref ref="ERROR-LOG" />
    </root>
</configuration>
```
部分标签说明<br />`<root>`标签，必填标签，用来指定最基础的日志输出级别

- `<appender-ref>`标签，添加`append`

`<append>`标签，通过使用该标签指定日志的收集策略

- `name`属性指定`appender`命名
- `class`属性指定输出策略，通常有两种，控制台输出和文件输出，文件输出就是将日志进行一个持久化。`ConsoleAppender`将日志输出到控制台

`<filter>`标签，通过使用该标签指定过滤策略

- `<level>`标签指定过滤的类型

`<encoder>`标签，使用该标签下的`<pattern>`标签指定日志输出格式。<br />`<rollingPolicy>`标签指定收集策略，比如基于时间进行收集

- `<fileNamePattern>`标签指定生成日志保存地址 通过这样配置已经实现了分类分天日志的目标了
<a name="E0sLf"></a>
### 3、logback 高级特性异步输出日志
之前的日志配置方式是基于同步的，每次日志输出到文件都会进行一次磁盘IO。采用异步写日志的方式而不让此次写日志发生磁盘IO，阻塞线程从而造成不必要的性能损耗。异步输出日志的方式很简单，添加一个基于异步写日志的`appender`，并指向原先配置的`appender`即可
```xml
<!-- 异步输出 -->
<appender name="ASYNC-INFO" class="ch.qos.logback.classic.AsyncAppender">
    <!-- 不丢失日志.默认的,如果队列的80%已满,则会丢弃TRACT、DEBUG、INFO级别的日志 -->
    <discardingThreshold>0</discardingThreshold>
    <!-- 更改默认的队列的深度,该值会影响性能.默认值为256 -->
    <queueSize>256</queueSize>
    <!-- 添加附加的appender,最多只能添加一个 -->
    <appender-ref ref="INFO-LOG"/>
</appender>

<appender name="ASYNC-ERROR" class="ch.qos.logback.classic.AsyncAppender">
    <!-- 不丢失日志.默认的,如果队列的80%已满,则会丢弃TRACT、DEBUG、INFO级别的日志 -->
    <discardingThreshold>0</discardingThreshold>
    <!-- 更改默认的队列的深度,该值会影响性能.默认值为256 -->
    <queueSize>256</queueSize>
    <!-- 添加附加的appender,最多只能添加一个 -->
    <appender-ref ref="ERROR-LOG"/>
</appender>
```
<a name="OGlKK"></a>
### 4、异步输出日志性能测试
既然能提高性能的话，必须进行一次测试比对，同步和异步输出日志性能到底能提升多少倍？
<a name="bDyQD"></a>
### 5、服务器硬件

- CPU 六核
- 内存 8G
<a name="YLZtC"></a>
### 6、测试工具
Apache Jmeter
<a name="kG6BU"></a>
### 7、同步输出日志

- 线程数：100
- Ramp-Up Loop(可以理解为启动线程所用时间) ：0 可以理解为100个线程同时启用
- 测试结果

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641555561772-cfdc5fe9-b549-476b-b68f-8d50c01a7c27.webp#clientId=ua6bf87e6-e2ca-4&from=paste&id=u69991553&originHeight=536&originWidth=1000&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u44b43c71-e93e-4245-ad84-76d81456386&title=)<br />重点关注指标 Throughput【TPS】 吞吐量：系统在单位时间内处理请求的数量，在同步输出日志中 TPS 为 44.2/sec
<a name="HzcPx"></a>
### 8、异步输出日志

- 线程数 100
- Ramp-Up Loop：0
- 测试结果

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641555561977-344f8d1f-4a32-494e-bed2-b327f8a596da.webp#clientId=ua6bf87e6-e2ca-4&from=paste&id=u182986dc&originHeight=548&originWidth=1009&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue31e31fb-361d-4a38-b776-7046f93216f&title=)<br />TPS 为 497.5/sec ， 性能提升了10多倍！！！
<a name="I41TZ"></a>
### 9、异步日志输出原理
从logback框架下的Logger.info方法开始追踪。一路的方法调用路径如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641555562183-f5581712-f50e-4bfa-9f42-411234a6a615.webp#clientId=ua6bf87e6-e2ca-4&from=paste&id=u8f2ccbae&originHeight=578&originWidth=742&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u073445bf-5a18-4540-9e4f-e2fa9e578d4&title=)<br />异步输出日志中最关键的就是配置文件中ch.qos.logback.classic``AsyncAppenderBase``append
```java
protected void append(E eventObject) {
    if(!this.isQueueBelowDiscardingThreshold() || !this.isDiscardable(eventObject)) {
        this.preprocess(eventObject);
        this.put(eventObject);
    }
}
```
通过队列情况判断是否需要丢弃日志，不丢弃的话将它放到阻塞队列中，通过查看代码，这个阻塞队列为`ArrayBlockingQueueu`，默认大小为256，可以通过配置文件进行修改。`Logger.info(...)`到`append(...)`就结束了，只做了将日志塞入到阻塞队列的事，然后继续执行`Logger.info(...)`下面的语句了。在`AsyncAppenderBase`类中定义了一个`Worker`线程，`run`方法中的关键部分代码如下:
```java
E e = parent.blockingQueue.take();
aai.appendLoopOnAppenders(e);
```
从阻塞队列中取出一个日志，并调用`AppenderAttachableImpl`类中的`appendLoopOnAppenders`方法维护一个`Append`列表。`Worker`线程中调用方法过程主要如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641555561712-1c84bfaa-c55f-4790-ac59-00596e8e28ee.webp#clientId=ua6bf87e6-e2ca-4&from=paste&id=u20def597&originHeight=630&originWidth=652&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7f72457f-21ce-4492-a5ec-32975546f4e&title=)<br />最主要的两个方法就是encode``write``encode
