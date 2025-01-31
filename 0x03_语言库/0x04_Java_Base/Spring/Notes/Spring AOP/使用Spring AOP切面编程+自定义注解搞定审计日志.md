JavaSpring
<a name="KO3mt"></a>
## 一、简介
任何一个软件系统，都不可避免的会碰到【信息安全】这个词，尤其是对于刚入行的新手。<br />实际上，随着工作阅历的增加，越来越能感觉到，实现业务方提的需求，只是完成了软件系统研发中的【能用】要求；服务是否【可靠】可能需要从架构层和运维方面去着手解决；至于是否【安全】、更多的需要从【信息安全】这个角度来思考，尤其是当软件系统面对外界的恶意干扰和攻击时，是否依然能保障用户正常使用，对于大公司，这个可能是头等大事，因为可能一个很小很小的漏洞，一不小心可能会给公司带来几千万的损失！<br />最常见的就是电商系统和支付系统，尤其是需求旺季的时候，经常有黑客专门攻击这些电商系统，导致大量服务宕机，影响用户正常下单。<br />像这样的攻击案例每天都有，有的公司甚至直接向黑客气妥，给钱消灾！<br />但是这种做法肯定不是长久之计，最重要的还是主动提升系统的【安全】防御系数。<br />由于信息安全所涉及的要求内容众多，这里介绍其中关于【审计日志】的要求和具体应用。<br />【审计日志】，简单的说就是系统需要记录谁，在什么时间，对什么数据，做了什么样的更改！这个日志数据是极其珍贵的，后面如果因业务操作上出了问题，可以很方便进行操作回查。<br />同时，任何一个 IT 系统，如果要过审，这项任务基本上也是必审项！
<a name="ILqME"></a>
## 二、实践
实现【审计日志】这个需求，有一个很好的技术解决方案，就是使用 Spring 的切面编程，创建一个代理类，利用`afterReturning`和`afterThrowing`方法来实现日志的记录。<br />具体实现步骤如下
<a name="bbjtU"></a>
### 创建审计日志表
```sql
CREATE TABLE `tb_audit_log` (
  `id` bigint(20) NOT NULL  COMMENT '审计日志，主键ID',
  `table_name` varchar(500) DEFAULT '' COMMENT '操作的表名，多个用逗号隔开',
  `operate_desc` varchar(200) DEFAULT '' COMMENT '操作描述',
  `request_param` varchar(200) DEFAULT '' COMMENT '请求参数',
  `result` int(10) COMMENT '执行结果，0:成功，1:失败',
  `ex_msg` varchar(200) DEFAULT '' COMMENT '异常信息',
  `user_agent` text COLLATE utf8mb4_unicode_ci COMMENT '用户代理信息',
  `ip_address` varchar(32) NOT NULL DEFAULT '' COMMENT '操作时设备IP',
  `ip_address_name` varchar(32) DEFAULT '' COMMENT '操作时设备IP所在地址',
  `operate_time` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '操作时间',
  `operate_user_id` varchar(32) DEFAULT '' COMMENT '操作人ID',
  `operate_user_name` varchar(32) DEFAULT '' COMMENT '操作人',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='审计日志表';
```
<a name="KaD9g"></a>
### 编写一个注解类
```java
@Retention(RetentionPolicy.RUNTIME)
@Target({ElementType.TYPE,ElementType.METHOD})
@Documented
public @interface SystemAuditLog {

    /**
     * 操作了的表名
     * @return
     */
    String tableName() default "";

    /**
     * 日志描述
     * @return
     */
    String description() default "";

}
```
<a name="XtWIB"></a>
### 编写一个代理类
```java
@Component
@Aspect
public class SystemAuditLogAspect {

    @Autowired
    private SystemAuditLogService systemAuditLogService;

    /**
     * 定义切入点，切入所有标注此注解的类和方法
     */
    @Pointcut("@within(com.example.demo.core.annotation.SystemAuditLog)|| @annotation(com.example.demo.core.annotation.SystemAuditLog)")
    public void methodAspect() {
    }

    /**
     * 方法调用前拦截
     */
    @Before("methodAspect()")
    public void before(){
        System.out.println("SystemAuditLog代理 -> 调用方法执行之前......");
    }

    /**
     * 方法调用后拦截
     */
    @After("methodAspect()")
    public void after(){
        System.out.println("SystemAuditLog代理 -> 调用方法执行之后......");
    }

    /**
     * 调用方法结束拦截
     */
    @AfterReturning(value = "methodAspect()")
    public void afterReturning(JoinPoint joinPoint) throws Exception {
        System.out.println("SystemAuditLog代理 -> 调用方法结束拦截......");
        //封装数据
        AuditLog entity = warpAuditLog(joinPoint);
        entity.setResult(0);

        //插入到数据库
        systemAuditLogService.add(entity);
    }


    /**
     * 抛出异常拦截
     */
    @AfterThrowing(value="methodAspect()", throwing="ex")
    public void afterThrowing(JoinPoint joinPoint, Exception ex) throws Exception {
        System.out.println("SystemAuditLog代理 -> 抛出异常拦截......");
        //封装数据
        AuditLog entity = warpAuditLog(joinPoint);
        entity.setResult(1);
        //封装错误信息
        entity.setExMsg(ex.getMessage());

        //插入到数据库
        systemAuditLogService.add(entity);
    }


    /**
     * 封装插入实体
     * @param joinPoint
     * @return
     * @throws Exception
     */
    private AuditLog warpAuditLog(JoinPoint joinPoint) throws Exception {
        //获取请求上下文
        HttpServletRequest request = getHttpServletRequest();
        //获取注解上的参数值
        SystemAuditLog systemAuditLog = getServiceMethodDescription(joinPoint);
        //获取请求参数
        Object requestObj = getServiceMethodParams(joinPoint);
        //封装数据
        AuditLog auditLog = new AuditLog();
        auditLog.setId(SnowflakeIdWorker.getInstance().nextId());
        //从请求上下文对象获取相应的数据
        if(Objects.nonNull(request)){
            auditLog.setUserAgent(request.getHeader("User-Agent"));
            //获取登录时的ip地址
            auditLog.setIpAddress(IpAddressUtil.getIpAddress(request));
            //调用外部接口，获取IP所在地
            auditLog.setIpAddressName(IpAddressUtil.getLoginAddress(auditLog.getIpAddress()));
        }
        //封装操作的表和描述
        if(Objects.nonNull(systemAuditLog)){
            auditLog.setTableName(systemAuditLog.tableName());
            auditLog.setOperateDesc(systemAuditLog.description());
        }
        //封装请求参数
        auditLog.setRequestParam(JSON.toJSONString(requestObj));
        //封装请求人
        if(Objects.nonNull(requestObj) && requestObj instanceof BaseRequest){
            auditLog.setOperateUserId(((BaseRequest) requestObj).getLoginUserId());
            auditLog.setOperateUserName(((BaseRequest) requestObj).getLoginUserName());
        }
        auditLog.setOperateTime(new Date());
        return auditLog;
    }


    /**
     * 获取当前的request
     * 这里如果报空指针异常是因为单独使用spring获取request
     * 需要在配置文件里添加监听
     *
     * 如果是spring项目，通过下面方式注入
     * <listener>
     * <listener-class>
     * org.springframework.web.context.request.RequestContextListener
     * </listener-class>
     * </listener>
     *
     * 如果是springboot项目，在配置类里面，通过下面方式注入
     * @Bean
     * public RequestContextListener requestContextListener(){
     *     return new RequestContextListener();
     * }
     * @return
     */
    private HttpServletRequest getHttpServletRequest(){
        RequestAttributes ra = RequestContextHolder.getRequestAttributes();
        ServletRequestAttributes sra = (ServletRequestAttributes)ra;
        HttpServletRequest request = sra.getRequest();
        return request;
    }

    /**
     * 获取请求对象
     * @param joinPoint
     * @return
     * @throws Exception
     */
    private Object getServiceMethodParams(JoinPoint joinPoint) {
        Object[] arguments = joinPoint.getArgs();
        if(Objects.nonNull(arguments) && arguments.length > 0){
            return arguments[0];
        }
        return null;
    }


    /**
     * 获取自定义注解里的参数
     * @param joinPoint
     * @return 返回注解里面的日志描述
     * @throws Exception
     */
    private SystemAuditLog getServiceMethodDescription(JoinPoint joinPoint) throws Exception {
        //类名
        String targetName = joinPoint.getTarget().getClass().getName();
        //方法名
        String methodName = joinPoint.getSignature().getName();
        //参数
        Object[] arguments = joinPoint.getArgs();
        //通过反射获取示例对象
        Class targetClass = Class.forName(targetName);
        //通过实例对象方法数组
        Method[] methods = targetClass.getMethods();
        for(Method method : methods) {
            //判断方法名是不是一样
            if(method.getName().equals(methodName)) {
                //对比参数数组的长度
                Class[] clazzs = method.getParameterTypes();
                if(clazzs.length == arguments.length) {
                    //获取注解里的日志信息
                    return method.getAnnotation(SystemAuditLog.class);
                }
            }
        }
        return null;
    }
}
```
<a name="racfm"></a>
### 在对应的接口或者方法上添加审计日志注解
```java
@RestController
@RequestMapping("api")
public class LoginController {

    /**
     * 用户登录，添加审计日志注解
     * @param request
     */
    @SystemAuditLog(tableName = "tb_user", description = "用户登录")
    @PostMapping("login")
    public void login(UserLoginDTO request){
        //登录逻辑处理
    }
}
```
<a name="c5JmD"></a>
### 相关的实体类
```java
@Data
public class AuditLog {

    /**
     * 审计日志，主键ID
     */
    private Long id;

    /**
     * 操作的表名，多个用逗号隔开
     */
    private String tableName;

    /**
     * 操作描述
     */
    private String operateDesc;

    /**
     * 请求参数
     */
    private String requestParam;

    /**
     * 执行结果，0:成功，1:失败
     */
    private Integer result;

    /**
     * 异常信息
     */
    private String exMsg;

    /**
     * 请求代理信息
     */
    private String userAgent;

    /**
     * 操作时设备IP
     */
    private String ipAddress;

    /**
     * 操作时设备IP所在地址
     */
    private String ipAddressName;

    /**
     * 操作时间
     */
    private Date operateTime;


    /**
     * 操作人ID
     */
    private String operateUserId;

    /**
     * 操作人
     */
    private String operateUserName;
}
```
```java
public class BaseRequest implements Serializable {

    /**
     * 请求token
     */
    private String token;

    /**
     * 登录人ID
     */
    private String loginUserId;

    /**
     * 登录人姓名
     */
    private String loginUserName;

    public String getToken() {
        return token;
    }

    public void setToken(String token) {
        this.token = token;
    }

    public String getLoginUserId() {
        return loginUserId;
    }

    public void setLoginUserId(String loginUserId) {
        this.loginUserId = loginUserId;
    }

    public String getLoginUserName() {
        return loginUserName;
    }

    public void setLoginUserName(String loginUserName) {
        this.loginUserName = loginUserName;
    }
}
```
```java
@Data
public class UserLoginDTO extends BaseRequest {

    /**
     * 用户名
     */
    private String userName;

    /**
     * 密码
     */
    private String password;
}
```
<a name="gBdgx"></a>
## 三、小结
整个程序的实现过程，主要使用了 Spring AOP 特性，对特定方法进行前、后拦截，从而实现业务方的需求。
