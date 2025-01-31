Java SpringBoot 监听器
<a name="5iz8G"></a>
## 1、监听器介绍
Web监听器是一种 Servlet 中特殊的类，它们能帮助开发者监听 web 中特定的事件，比如 ServletContext, HttpSession, ServletRequest 的创建和销毁；变量的创建、销毁和修改等。可以在某些动作前后增加处理，实现监控。
<a name="hSu8C"></a>
## 2、Spring Boot中监听器的使用
Web 监听器的使用场景很多，比如监听 servlet 上下文用来初始化一些数据、监听 http session 用来获取当前在线的人数、监听客户端请求的 servlet request 对象来获取用户的访问信息等等。
<a name="Hjse6"></a>
### 2.1 监听Servlet上下文对象
监听 servlet 上下文对象可以用来初始化数据，用于缓存。举一个很常见的场景，比如用户在点击某个站点的首页时，一般都会展现出首页的一些信息，而这些信息基本上或者大部分时间都保持不变的，但是这些信息都是来自数据库。如果用户的每次点击，都要从数据库中去获取数据的话，用户量少还可以接受，如果用户量非常大的话，这对数据库也是一笔很大的开销。<br />针对这种首页数据，大部分都不常更新的话，完全可以把它们缓存起来，每次用户点击的时候，都直接从缓存中拿，这样既可以提高首页的访问速度，又可以降低服务器的压力。如果做的更加灵活一点，可以再加个定时器，定期的来更新这个首页缓存。就类似与 CSDN 个人博客首页中排名的变化一样。<br />下面针对这个功能写一个示例Demo，在实际中，可以完全套用该代码，来实现自己项目中的相关逻辑。首先写一个 Service，模拟一下从数据库查询数据：
```java
@Service
public class UserService {
    /**
     * 获取用户信息
     * @return
     */
    public User getUser() {
        // 实际中会根据具体的业务场景，从数据库中查询对应的信息
        return new User(1L, "Fcant", "123456");
    }
}
```
然后写一个监听器，实现 `ApplicationListener<ContextRefreshedEvent>` 接口，重写 `onApplicationEvent` 方法，将 ContextRefreshedEvent 对象传进去。如果想在加载或刷新应用上下文时，也重新刷新下预加载的资源，就可以通过监听 ContextRefreshedEvent 来做这样的事情。如下：
```java
/**
 * 使用ApplicationListener来初始化一些数据到application域中的监听器
 * @author Fcant
 * @date 2018/07/05
 */
@Component
public class MyServletContextListener implements ApplicationListener<ContextRefreshedEvent> {
    @Override
    public void onApplicationEvent(ContextRefreshedEvent contextRefreshedEvent) {
        // 先获取到application上下文
        ApplicationContext applicationContext = contextRefreshedEvent.getApplicationContext();
        // 获取对应的service
        UserService userService = applicationContext.getBean(UserService.class);
        User user = userService.getUser();
        // 获取application域对象，将查到的信息放到application域中
        ServletContext application = applicationContext.getBean(ServletContext.class);
        application.setAttribute("user", user);
    }
}
```
正如注释中描述的一样，首先通过 contextRefreshedEvent 来获取 application 上下文，再通过 application 上下文来获取 UserService 这个 bean，项目中可以根据实际业务场景，也可以获取其他的 bean，然后再调用自己的业务代码获取相应的数据，最后存储到 application 域中，这样前端在请求相应数据的时候，就可以直接从 application 域中获取信息，减少数据库的压力。下面写一个 Controller 直接从 application 域中获取 user 信息来测试一下。
```java
@RestController
@RequestMapping("/listener")
public class TestController {
    @GetMapping("/user")
    public User getUser(HttpServletRequest request) {
        ServletContext application = request.getServletContext();
        return (User) application.getAttribute("user");
    }
}
```
启动项目，在浏览器中输入 http://localhost:8080/listener/user 测试一下即可，如果正常返回 user 信息，那么说明数据已经缓存成功。不过 application 这种是缓存在内存中，对内存会有消耗，一般是通过 Redis 进行缓存。
<a name="kvKA8"></a>
### 2.2 监听HTTP会话 Session对象
监听器还有一个比较常用的地方就是用来监听 session 对象，来获取在线用户数量，现在有很多开发者都有自己的网站，监听 session 来获取当前在下用户数量是个很常见的使用场景，下面来介绍一下如何来使用。
```java
/**
 * 使用HttpSessionListener统计在线用户数的监听器
 * @author Fcant
 * @date 2018/07/05
 */
@Component
public class MyHttpSessionListener implements HttpSessionListener {
    private static final Logger logger = LoggerFactory.getLogger(MyHttpSessionListener.class);
    /**
     * 记录在线的用户数量
     */
    public Integer count = 0;
    @Override
    public synchronized void sessionCreated(HttpSessionEvent httpSessionEvent) {
        logger.info("新用户上线了");
        count++;
        httpSessionEvent.getSession().getServletContext().setAttribute("count", count);
    }
    @Override
    public synchronized void sessionDestroyed(HttpSessionEvent httpSessionEvent) {
        logger.info("用户下线了");
        count--;
        httpSessionEvent.getSession().getServletContext().setAttribute("count", count);
    }
}
```
可以看出，首先该监听器需要实现 HttpSessionListener 接口，然后重写 `sessionCreated` 和 `sessionDestroyed` 方法，在 `sessionCreated` 方法中传递一个 HttpSessionEvent 对象，然后将当前 session 中的用户数量加1，`sessionDestroyed` 方法刚好相反，不再赘述。然后写一个 Controller 来测试一下。
```java
@RestController
@RequestMapping("/listener")
public class TestController {
    /**
     * 获取当前在线人数，该方法有bug
     * @param request
     * @return
     */
    @GetMapping("/total")
    public String getTotalUser(HttpServletRequest request) {
        Integer count = (Integer) request.getSession().getServletContext().getAttribute("count");
        return "当前在线人数：" + count;
    }
}
```
该 Controller 中是直接获取当前 session 中的用户数量，启动服务器，在浏览器中输入 localhost:8080/listener/total 可以看到返回的结果是1，再打开一个浏览器，请求相同的地址可以看到 count 是 2 ，这没有问题。但是如果关闭一个浏览器再打开，理论上应该还是2，但是实际测试却是 3。原因是 session 销毁的方法没有执行（可以在后台控制台观察日志打印情况），当重新打开时，服务器找不到用户原来的 session，于是又重新创建了一个 session，那怎么解决该问题呢？可以将上面的 Controller 方法改造一下：
```java
@GetMapping("/total2")
public String getTotalUser(HttpServletRequest request, HttpServletResponse response) {
    Cookie cookie;
    try {
        // 把sessionId记录在浏览器中
        cookie = new Cookie("JSESSIONID", URLEncoder.encode(request.getSession().getId(), "utf-8"));
        cookie.setPath("/");
        //设置cookie有效期为2天，设置长一点
        cookie.setMaxAge( 48*60 * 60);
        response.addCookie(cookie);
    } catch (UnsupportedEncodingException e) {
        e.printStackTrace();
    }
    Integer count = (Integer) request.getSession().getServletContext().getAttribute("count");
    return "当前在线人数：" + count;
}
```
可以看出，该处理逻辑是让服务器记得原来那个 session，即把原来的 sessionId 记录在浏览器中，下次再打开时，把这个 sessionId 传过去，这样服务器就不会重新再创建了。重启一下服务器，在浏览器中再次测试一下，即可避免上面的问题。
<a name="q0asA"></a>
### 2.3 监听客户端请求Servlet Request对象
使用监听器获取用户的访问信息比较简单，实现 ServletRequestListener 接口即可，然后通过 request 对象获取一些信息。如下：
```java
/**
 * 使用ServletRequestListener获取访问信息
 * @author Fcant
 * @date 2018/07/05
 */
@Component
public class MyServletRequestListener implements ServletRequestListener {
    private static final Logger logger = LoggerFactory.getLogger(MyServletRequestListener.class);
    @Override
    public void requestInitialized(ServletRequestEvent servletRequestEvent) {
        HttpServletRequest request = (HttpServletRequest) servletRequestEvent.getServletRequest();
        logger.info("session id为：{}", request.getRequestedSessionId());
        logger.info("request url为：{}", request.getRequestURL());
        request.setAttribute("name", "Fcant");
    }
    @Override
    public void requestDestroyed(ServletRequestEvent servletRequestEvent) {
        logger.info("request end");
        HttpServletRequest request = (HttpServletRequest) servletRequestEvent.getServletRequest();
        logger.info("request域中保存的name值为：{}", request.getAttribute("name"));
    }
}
```
这个比较简单，不再赘述，接下来写一个 Controller 测试一下即可。
```java
@GetMapping("/request")
public String getRequestInfo(HttpServletRequest request) {
    System.out.println("requestListener中的初始化的name数据：" + request.getAttribute("name"));
    return "success";
}
```
<a name="G4E21"></a>
## 3、Spring Boot中自定义事件监听
在实际项目中，往往需要自定义一些事件和监听器来满足业务场景，比如在微服务中会有这样的场景：微服务 A 在处理完某个逻辑之后，需要通知微服务 B 去处理另一个逻辑，或者微服务 A 处理完某个逻辑之后，需要将数据同步到微服务 B，这种场景非常普遍，这个时候，可以自定义事件以及监听器来监听，一旦监听到微服务 A 中的某事件发生，就去通知微服务 B 处理对应的逻辑。
<a name="dUnmt"></a>
### 3.1 自定义事件
自定义事件需要继承 ApplicationEvent 对象，在事件中定义一个 User 对象来模拟数据，构造方法中将 User 对象传进来初始化。如下：
```java
/**
 * 自定义事件
 * @author Fcant
 * @date 2018/07/05
 */
public class MyEvent extends ApplicationEvent {
    private User user;
    public MyEvent(Object source, User user) {
        super(source);
        this.user = user;
    }
    // 省去get、set方法
}
```
<a name="Nomzp"></a>
### 3.2 自定义监听器
接下来，自定义一个监听器来监听上面定义的 MyEvent 事件，自定义监听器需要实现 `ApplicationListener` 接口即可。如下：
```java
/**
 * 自定义监听器，监听MyEvent事件
 * @author Fcant
 * @date 2018/07/05
 */
@Component
public class MyEventListener implements ApplicationListener<MyEvent> {
    @Override
    public void onApplicationEvent(MyEvent myEvent) {
        // 把事件中的信息获取到
        User user = myEvent.getUser();
        // 处理事件，实际项目中可以通知别的微服务或者处理其他逻辑等等
        System.out.println("用户名：" + user.getUsername());
        System.out.println("密码：" + user.getPassword());
    }
}
```
然后重写 `onApplicationEvent` 方法，将自定义的 MyEvent 事件传进来，因为该事件中，定义了 User 对象（该对象在实际中就是需要处理的数据，在下文来模拟），然后就可以使用该对象的信息了。<br />定义好了事件和监听器之后，需要手动发布事件，这样监听器才能监听到，这需要根据实际业务场景来触发，针对本文的例子，写个触发逻辑，如下：
```java
/**
 * UserService
 * @author Fcant
 */
@Service
public class UserService {
    @Resource
    private ApplicationContext applicationContext;
    /**
     * 发布事件
     * @return
     */
    public User getUser2() {
        User user = new User(1L, "Fcant", "123456");
        // 发布事件
        MyEvent event = new MyEvent(this, user);
        applicationContext.publishEvent(event);
        return user;
    }
}
```
在 service 中注入 ApplicationContext，在业务代码处理完之后，通过 ApplicationContext 对象手动发布 MyEvent 事件，这样自定义的监听器就能监听到，然后处理监听器中的业务逻辑。<br />最后，在 Controller 中写一个接口来测试一下：
```java
@GetMapping("/request")
public String getRequestInfo(HttpServletRequest request) {
    System.out.println("requestListener中的初始化的name数据：" + request.getAttribute("name"));
    return "success";
}
```
在浏览器中输入 http://localhost:8080/listener/publish，然后观察一下控制台打印的用户名和密码，即可说明自定义监听器已经生效。
