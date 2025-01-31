![](https://cdn.nlark.com/yuque/0/2023/png/396745/1688607094458-175e9b5c-a9ae-4426-8685-7a2400dc0010.png#averageHue=%23f0f2d7&clientId=ue1821de9-013c-4&from=paste&id=ua361879d&originHeight=503&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u71fa8b23-70df-491f-ab18-6ed797caa14&title=)<br />在Java中，动态代理是一种机制，允许在运行时动态地创建代理对象来代替某个实际对象，从而在其前后执行额外的逻辑。<br />为什么JDK动态代理只能代理接口实现类，原因是JDK动态代理是基于接口实现的。<br />当使用Proxy类创建代理对象时，需要指定一个接口列表来表示代理对象所应该实现的接口，这些接口就成为代理对象的类型。<br />具体来说，代理对象的方法调用会被转发到实现`InvocationHandler`接口的类中的`invoke()`方法。这个`invoke()`方法接受三个参数：代理对象本身、被调用的方法对象和方法的参数数组。`invoke()`方法需要返回被代理方法调用的结果。<br />由于代理对象的类型是由接口列表决定的，因此只有实现了接口的类才能被代理。如果想代理一个类而不是一个接口，需要使用其他的代理技术，比如CGLIB。
<a name="hEePV"></a>
## 1、JDK动态代理代码实例
下面是一个简单的示例代码，展示了如何使用JDK动态代理来创建代理对象。
```java
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;

public class ProxyDemo {
    public static void main(String[] args) {
        RealObject real = new RealObject();
        InvocationHandler handler = new DynamicProxy(real);

        // 创建代理对象
        MyInterface proxy = (MyInterface) Proxy.newProxyInstance(
            MyInterface.class.getClassLoader(),
            new Class<?>[] { MyInterface.class },
            handler);

        // 调用代理对象的方法
        proxy.doSomething();
    }
}

interface MyInterface {
    void doSomething();
}

class RealObject implements MyInterface {
    public void doSomething() {
        System.out.println("RealObject doSomething");
    }
}

class DynamicProxy implements InvocationHandler {
    private Object target;

    public DynamicProxy(Object target) {
        this.target = target;
    }

    public Object invoke(Object proxy, Method method, Object[] args)
    throws Throwable {
        System.out.println("Before method invocation");
        Object result = method.invoke(target, args);
        System.out.println("After method invocation");
        return result;
    }
}
```
在上面的代码中，`RealObject`实现了`MyInterface`接口，它是要代理的实际对象。`DynamicProxy`类实现了`InvocationHandler`接口，并在`invoke()`方法中添加了额外的逻辑，用于在代理对象方法调用前后执行。<br />在`main()`方法中，使用`Proxy.newProxyInstance()`方法创建代理对象。指定了`MyInterface`接口作为代理对象类型，并将`DynamicProxy`对象作为代理对象的`InvocationHandler`。<br />最后，调用代理对象的`doSomething()`方法，并观察控制台输出的结果。<br />需要注意的是，代理对象的方法调用都会被转发到`DynamicProxy`类的`invoke()`方法中进行处理，因此在这个示例中，实际的`RealObject`对象的`doSomething()`方法的执行是在`invoke()`方法中通过反射进行的。<br />总结一下，JDK动态代理只能代理接口实现类，原因是JDK动态代理是基于接口实现的，代理对象的类型由接口列表决定。如果想代理一个类而不是一个接口，需要使用其他的代理技术，比如CGLIB。
<a name="K8FKn"></a>
## 2、Cglib 代码演示
以下是CGLIB代理的示例代码。
```java
import net.sf.cglib.proxy.Enhancer;
import net.sf.cglib.proxy.MethodInterceptor;
import net.sf.cglib.proxy.MethodProxy;

import java.lang.reflect.Method;

public class CGLIBProxyDemo {
    public static void main(String[] args) {
        RealObject real = new RealObject();
        MethodInterceptor handler = new CGLIBProxy(real);

        // 创建代理对象
        RealObject proxy = (RealObject) Enhancer.create(
            RealObject.class,
            handler);

        // 调用代理对象的方法
        proxy.doSomething();
    }
}

class CGLIBProxy implements MethodInterceptor {
    private Object target;

    public CGLIBProxy(Object target) {
        this.target = target;
    }

    public Object intercept(Object obj, Method method, Object[] args, MethodProxy proxy) throws Throwable {
        System.out.println("Before method invocation");
        Object result = proxy.invoke(target, args);
        System.out.println("After method invocation");
        return result;
    }
}
```
在上面的示例中，使用CGLIB的`Enhancer`类和`MethodInterceptor`接口来创建代理对象。`RealObject`类不再需要实现接口，而是直接作为代理对象的类型。在`CGLIBProxy`类中，实现了`MethodInterceptor`接口，并在`intercept()`方法中添加了额外的逻辑。<br />在`main()`方法中，使用`Enhancer.create()`方法创建代理对象。指定了`RealObject`类作为代理对象类型，并将`CGLIBProxy`对象作为代理对象的`MethodInterceptor`。最后调用代理对象的`doSomething()`方法，并观察控制台输出的结果。<br />需要注意的是，CGLIB代理使用字节码技术来生成代理对象，因此它的效率比JDK动态代理要高，但是它也需要额外的库依赖。
<a name="pBqkp"></a>
## 3、两者优缺点
JDK动态代理和CGLIB代理都有它们自己的优缺点。<br />JDK动态代理的优点：

- JDK动态代理是Java标准库的一部分，因此它不需要引入任何外部依赖。
- JDK动态代理只需要实现接口即可生成代理对象，不需要改变原有类的结构。
- 由于JDK动态代理是基于接口实现的，因此它更适合用于代理接口实现类的场景。

JDK动态代理的缺点：

- JDK动态代理只能代理实现了接口的类，无法代理没有实现接口的类。
- JDK动态代理在生成代理对象时，需要使用反射机制，因此它的效率相对较低。

CGLIB代理的优点：

- CGLIB代理是基于字节码技术实现的，因此它的效率比JDK动态代理更高。
- CGLIB代理可以代理没有实现接口的类。

CGLIB代理的缺点：

- CGLIB代理需要引入外部依赖。
- CGLIB代理在生成代理对象时，需要改变原有类的结构，因此它可能会引起一些问题，例如无法代理`final`类或`final`方法等问题。

综上所述，JDK动态代理适用于代理接口实现类的场景，而CGLIB代理适用于代理没有实现接口的类的场景。如果需要代理接口实现类，而且不想引入额外的依赖，那么JDK动态代理是一个不错的选择；如果需要代理没有实现接口的类，那么CGLIB代理可能更适合需求。
