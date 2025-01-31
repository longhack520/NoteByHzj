<a name="MxIWg"></a>
## 一、摘要
在上一篇文章中，详细的介绍了**随着下单流量逐渐上升，为了降低数据库的访问压力，通过请求唯一ID+redis分布式锁来防止接口重复提交**，流程图如下！<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668645020483-ad68ab49-3c52-4cc7-9324-2ae869327d65.png#averageHue=%23f8f7f7&clientId=u55b9ebda-265c-4&from=paste&id=u273f7143&originHeight=248&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u07ca525f-741d-480c-967f-817493a2720&title=)<br />每次提交的时候，需要先调用后端服务获取请求唯一ID，然后才能提交。<br />对于这样的流程，不少的同学可能会感觉到非常鸡肋，尤其是单元测试，需要每次先获取submitToken值，然后才能提交！<br />**能不能不用这么麻烦，直接服务端通过一些规则组合，生成本次请求唯一ID呢**？<br />**答案是可以的！**<br />下面就一起来看看，如何通过服务端来完成请求唯一 ID 的生成？
<a name="wyoSE"></a>
## 二、方案实践
先来看一张图，这张图就是本次方案的核心流程图。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668645020466-e1f6771e-4962-45ac-91b7-9d9169217baa.png#averageHue=%23f8f8f8&clientId=u55b9ebda-265c-4&from=paste&id=u0659eb4f&originHeight=319&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=udf5ac30c-4ee2-4c12-af31-813ca04303e&title=)<br />实现的逻辑，流程如下：

1. 用户点击提交按钮，服务端接受到请求后，通过规则计算出本次请求唯一ID值
2. 使用redis的分布式锁服务，对请求 ID 在限定的时间内尝试进行加锁，如果加锁成功，继续后续流程；如果加锁失败，说明服务正在处理，请勿重复提交
3. 最后一步，如果加锁成功后，需要将锁手动释放掉，以免再次请求时，提示同样的信息

引入缓存服务后，防止重复提交的大体思路如上，实践代码如下！
<a name="QW05Y"></a>
#### 2.1、引入 redis 组件
本次 demo 项目是基于SpringBoot版本进行构建，添加相关的redis依赖环境如下：
```xml
<!-- 引入springboot -->
<parent>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-parent</artifactId>
  <version>2.1.0.RELEASE</version>
</parent>

......

<!-- Redis相关依赖包，采用jedis作为客户端 -->
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-data-redis</artifactId>
  <exclusions>
    <exclusion>
      <groupId>redis.clients</groupId>
      <artifactId>jedis</artifactId>
    </exclusion>
    <exclusion>
      <artifactId>lettuce-core</artifactId>
      <groupId>io.lettuce</groupId>
    </exclusion>
  </exclusions>
</dependency>
<dependency>
  <groupId>redis.clients</groupId>
  <artifactId>jedis</artifactId>
</dependency>
<dependency>
  <groupId>org.apache.commons</groupId>
  <artifactId>commons-pool2</artifactId>
</dependency>
```
<a name="cHLeE"></a>
#### 2.2、添加 redis 环境配置
在全局配置application.properties文件中，添加redis相关服务配置如下
```
# 项目名
spring.application.name=springboot-example-submit

# Redis数据库索引（默认为0）
spring.redis.database=1
# Redis服务器地址
spring.redis.host=127.0.0.1
# Redis服务器连接端口
spring.redis.port=6379
# Redis服务器连接密码（默认为空）
spring.redis.password=
# Redis服务器连接超时配置
spring.redis.timeout=1000

# 连接池配置
spring.redis.jedis.pool.max-active=8
spring.redis.jedis.pool.max-wait=1000
spring.redis.jedis.pool.max-idle=8
spring.redis.jedis.pool.min-idle=0
spring.redis.jedis.pool.time-between-eviction-runs=100
```
<a name="gKggI"></a>
#### 2.3、编写服务验证逻辑，通过 aop 代理方式实现
首先创建一个`@SubmitLimit`注解，通过这个注解来进行方法代理拦截！
```java
@Retention(RetentionPolicy.RUNTIME)
@Target({ElementType.METHOD})
@Documented
public @interface SubmitLimit {

    /**
     * 指定时间内不可重复提交（仅相对上一次发起请求时间差）,单位毫秒
     * @return
     */
    int waitTime() default 1000;

    /**
     * 指定请求头部key，可以组合生成签名
     * @return
     */
    String[] customerHeaders() default {};


    /**
     * 自定义重复提交提示语
     * @return
     */
    String customerTipMsg() default "";
}
```
编写方法代理服务，增加防止重复提交的验证，实现了逻辑如下！
```java
@Order(1)
@Aspect
@Component
public class SubmitLimitAspect {

    private static final Logger LOGGER = LoggerFactory.getLogger(SubmitLimitAspect.class);

    /**
     * redis分割符
     */
    private static final String REDIS_SEPARATOR = ":";

    /**
     * 默认锁对应的值
     */
    private static final String DEFAULT_LOCK_VALUE = "DEFAULT_SUBMIT_LOCK_VALUE";

    /**
     * 默认重复提交提示语
     */
    private static final String DEFAULT_TIP_MSG = "服务正在处理，请勿重复提交！";


    @Value("${spring.application.name}")
    private String applicationName;

    @Autowired
    private RedisLockService redisLockService;


    /**
     * 方法调用环绕拦截
     */
    @Around(value = "@annotation(com.example.submittoken.config.annotation.SubmitLimit)")
    public Object doAround(ProceedingJoinPoint joinPoint){
        HttpServletRequest request = getHttpServletRequest();
        if(Objects.isNull(request)){
            return ResResult.getSysError("请求参数不能为空！");
        }
        //获取注解配置的参数
        SubmitLimit submitLimit = getSubmitLimit(joinPoint);
        //组合生成key，通过key实现加锁和解锁
        String lockKey = buildSubmitLimitKey(joinPoint, request, submitLimit.customerHeaders());
        //尝试在指定的时间内加锁
        boolean lock = redisLockService.tryLock(lockKey, DEFAULT_LOCK_VALUE, Duration.ofMillis(submitLimit.waitTime()));
        if(!lock){
            String tipMsg = StringUtils.isEmpty(submitLimit.customerTipMsg()) ? DEFAULT_TIP_MSG : submitLimit.customerTipMsg();
            return ResResult.getSysError(tipMsg);
        }
        try {
            //继续执行后续流程
            return execute(joinPoint);
        } finally {
            //执行完毕之后，手动将锁释放
            redisLockService.releaseLock(lockKey, DEFAULT_LOCK_VALUE);
        }
    }

    /**
     * 执行任务
     * @param joinPoint
     * @return
     */
    private Object execute(ProceedingJoinPoint joinPoint){
        try {
            return joinPoint.proceed();
        } catch (CommonException e) {
            return ResResult.getSysError(e.getMessage());
        } catch (Throwable e) {
            LOGGER.error("业务处理发生异常，错误信息：",e);
            return ResResult.getSysError(ResResultEnum.DEFAULT_ERROR_MESSAGE);
        }
    }


    /**
     * 获取请求对象
     * @return
     */
    private HttpServletRequest getHttpServletRequest(){
        RequestAttributes ra = RequestContextHolder.getRequestAttributes();
        ServletRequestAttributes sra = (ServletRequestAttributes)ra;
        HttpServletRequest request = sra.getRequest();
        return request;
    }

    /**
     * 获取注解值
     * @param joinPoint
     * @return
     */
    private SubmitLimit getSubmitLimit(JoinPoint joinPoint){
        MethodSignature methodSignature = (MethodSignature) joinPoint.getSignature();
        Method method = methodSignature.getMethod();
        SubmitLimit submitLimit = method.getAnnotation(SubmitLimit.class);
        return submitLimit;
    }

    /**
     * 组合生成lockKey
     * 生成规则：项目名+接口名+方法名+请求参数签名（对请求头部参数+请求body参数，取SHA1值）
     * @param joinPoint
     * @param request
     * @param customerHeaders
     * @return
     */
    private String buildSubmitLimitKey(JoinPoint joinPoint, HttpServletRequest request, String[] customerHeaders){
        //请求参数=请求头部+请求body
        String requestHeader = getRequestHeader(request, customerHeaders);
        String requestBody = getRequestBody(joinPoint.getArgs());
        String requestParamSign = DigestUtils.sha1Hex(requestHeader + requestBody);
        String submitLimitKey = new StringBuilder()
                .append(applicationName)
                .append(REDIS_SEPARATOR)
                .append(joinPoint.getSignature().getDeclaringType().getSimpleName())
                .append(REDIS_SEPARATOR)
                .append(joinPoint.getSignature().getName())
                .append(REDIS_SEPARATOR)
                .append(requestParamSign)
                .toString();
        return submitLimitKey;
    }


    /**
     * 获取指定请求头部参数
     * @param request
     * @param customerHeaders
     * @return
     */
    private String getRequestHeader(HttpServletRequest request, String[] customerHeaders){
        if (Objects.isNull(customerHeaders)) {
            return "";
        }
        StringBuilder sb = new StringBuilder();
        for (String headerKey : customerHeaders) {
            sb.append(request.getHeader(headerKey));
        }
        return sb.toString();
    }


    /**
     * 获取请求body参数
     * @param args
     * @return
     */
    private String getRequestBody(Object[] args){
        if (Objects.isNull(args)) {
            return "";
        }
        StringBuilder sb = new StringBuilder();
        for (Object arg : args) {
            if (arg instanceof HttpServletRequest
                    || arg instanceof HttpServletResponse
                    || arg instanceof MultipartFile
                    || arg instanceof BindResult
                    || arg instanceof MultipartFile[]
                    || arg instanceof ModelMap
                    || arg instanceof Model
                    || arg instanceof ExtendedServletRequestDataBinder
                    || arg instanceof byte[]) {
                continue;
            }
            sb.append(JacksonUtils.toJson(arg));
        }
        return sb.toString();
    }
}
```
部分校验逻辑用到了redis分布式锁，具体实现逻辑如下：
```java
/**
 * redis分布式锁服务类
 * 采用LUA脚本实现，保证加锁、解锁操作原子性
 *
 */
@Component
public class RedisLockService {

    /**
     * 分布式锁过期时间，单位秒
     */
    private static final Long DEFAULT_LOCK_EXPIRE_TIME = 60L;

    @Autowired
    private StringRedisTemplate stringRedisTemplate;

    /**
     * 尝试在指定时间内加锁
     * @param key
     * @param value
     * @param timeout 锁等待时间
     * @return
     */
    public boolean tryLock(String key,String value, Duration timeout){
        long waitMills = timeout.toMillis();
        long currentTimeMillis = System.currentTimeMillis();
        do {
            boolean lock = lock(key, value, DEFAULT_LOCK_EXPIRE_TIME);
            if (lock) {
                return true;
            }
            try {
                Thread.sleep(1L);
            } catch (InterruptedException e) {
                Thread.interrupted();
            }
        } while (System.currentTimeMillis() < currentTimeMillis + waitMills);
        return false;
    }

    /**
     * 直接加锁
     * @param key
     * @param value
     * @param expire
     * @return
     */
    public boolean lock(String key,String value, Long expire){
        String luaScript = "if redis.call('setnx', KEYS[1], ARGV[1]) == 1 then return redis.call('expire', KEYS[1], ARGV[2]) else return 0 end";
        RedisScript<Long> redisScript = new DefaultRedisScript<>(luaScript, Long.class);
        Long result = stringRedisTemplate.execute(redisScript, Collections.singletonList(key), value, String.valueOf(expire));
        return result.equals(Long.valueOf(1));
    }


    /**
     * 释放锁
     * @param key
     * @param value
     * @return
     */
    public boolean releaseLock(String key,String value){
        String luaScript = "if redis.call('get', KEYS[1]) == ARGV[1] then return redis.call('del', KEYS[1]) else return 0 end";
        RedisScript<Long> redisScript = new DefaultRedisScript<>(luaScript, Long.class);
        Long result = stringRedisTemplate.execute(redisScript, Collections.singletonList(key),value);
        return result.equals(Long.valueOf(1));
    }
}
```
部分代码使用到了序列化相关类JacksonUtils，源码如下：
```java
public class JacksonUtils {

    private static final Logger LOGGER = LoggerFactory.getLogger(JacksonUtils.class);


    private static final ObjectMapper objectMapper = new ObjectMapper();

    static {
        // 对象的所有字段全部列入
        objectMapper.setSerializationInclusion(JsonInclude.Include.ALWAYS);
        // 忽略未知的字段
        objectMapper.configure(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES, false);
        // 读取不认识的枚举时，当null值处理
        objectMapper.configure(DeserializationFeature.READ_UNKNOWN_ENUM_VALUES_AS_NULL, true);
        //        序列化忽略未知属性
        objectMapper.configure(SerializationFeature.FAIL_ON_EMPTY_BEANS, false);
        //忽略字段大小写
        objectMapper.configure(MapperFeature.ACCEPT_CASE_INSENSITIVE_PROPERTIES, true);

        objectMapper.configure(JsonParser.Feature.AUTO_CLOSE_SOURCE, true);
        SimpleModule module = new SimpleModule();
        module.addSerializer(Long.class, ToStringSerializer.instance);
        module.addSerializer(Long.TYPE, ToStringSerializer.instance);
        objectMapper.registerModule(module);
    }

    public static String toJson(Object object) {
        if (object == null) {
            return null;
        }
        try {
            return objectMapper.writeValueAsString(object);
        } catch (Exception e) {
            LOGGER.error("序列化失败",e);
        }
        return null;
    }

    public static <T> T fromJson(String json, Class<T> classOfT) {
        if (json == null) {
            return null;
        }
        try {
            return objectMapper.readValue(json, classOfT);
        } catch (Exception e) {
            LOGGER.error("反序列化失败",e);
        }
        return null;
    }

    public static <T> T fromJson(String json, Type typeOfT) {
        if (json == null) {
            return null;
        }
        try {
            return objectMapper.readValue(json, objectMapper.constructType(typeOfT));
        } catch (Exception e) {
            LOGGER.error("反序列化失败",e);
        }
        return null;
    }
}
```
<a name="O4dbt"></a>
#### 2.4、在相关的业务接口上，增加`@SubmitLimit`注解即可
```java
@RestController
@RequestMapping("order")
public class OrderController {

    @Autowired
    private OrderService orderService;

    /**
     * 下单，指定请求头部参与请求唯一值计算
     * @param request
     * @return
     */
    @SubmitLimit(customerHeaders = {"appId", "token"}, customerTipMsg = "正在加紧为您处理，请勿重复下单！")
    @PostMapping(value = "confirm")
    public ResResult confirmOrder(@RequestBody OrderConfirmRequest request){
        //调用订单下单相关逻辑
        orderService.confirm(request);
        return ResResult.getSuccess();
    }
}
```
**其中最关键的一个步就是将唯一请求 ID  的生成，放在服务端通过组合来实现，在保证防止接口重复提交的效果同时，也可以显著的降低接口测试复杂度**！
<a name="Su5J6"></a>
## 三、小结
本次方案相比于上一个方案，最大的改进点在于：将接口请求唯一 ID 的生成逻辑，放在服务端通过规则组合来实现，不需要前端提交接口的时候强制带上这个参数，在满足防止接口重复提交的要求同时，又能减少前端和测试提交接口的复杂度！<br />**需要特别注意的是：使用redis的分布式锁，推荐单机环境，如果redis是集群环境，可能会导致锁短暂无效**！
