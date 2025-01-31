Java Spring @Transactional
<a name="nu0RN"></a>
## `@Transactional`注解简介
`@Transactional`是spring中声明式事务管理的注解配置方式，相信这个注解的作用大家都很清楚。`@Transactional`注解可以把事务开启、提交或者回滚的操作，通过aop的方式进行管理。<br />通过`@Transactional`注解就能让Spring管理事务，免去了重复的事务管理逻辑，减少对业务代码的侵入，使开发人员能够专注于业务层面开发。<br />![2021-05-31-17-53-01-028737.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622455090984-d12efde8-fa99-46d3-bca6-93540e2568af.png#averageHue=%23c5f4e7&clientId=uea5099d8-1753-4&from=ui&id=u9d23d749&originHeight=236&originWidth=568&originalType=binary&ratio=1&rotation=0&showTitle=false&size=403112&status=done&style=shadow&taskId=u13195aa8-2131-4d7e-a4f0-049962a8ce5&title=)<br />实现`@Transactional`原理是基于Spring aop，aop又是动态代理模式的实现，通过对源码的阅读，总结出下面的步骤来了解实际中，在Spring 是如何利用aop来实现`@Transactional`的功能的。
<a name="uwNuk"></a>
## Spring中声明式事务实现原理猜想
首先，对于Spring中aop实现原理有了解的话，应该知道想要对一个方法进行代理的话，肯定需要定义切点。在`@Transactional`的实现中，同样如此，Spring定义了以 `@Transactional` 注解为植入点的切点，这样才能知道`@Transactional`注解标注的方法需要被代理。<br />有了切面定义之后，在Spring的bean的初始化过程中，就需要对实例化的bean进行代理，并且生成代理对象。<br />生成代理对象的代理逻辑中，进行方法调用时，需要先获取切面逻辑，`@Transactional`注解的切面逻辑类似于`@Around`，在Spring中是实现一种类似代理逻辑。<br />![2021-05-31-17-53-01-149436.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622455100977-3261cb22-d2f2-4570-9b20-4935bfc44321.png#averageHue=%23f6f6f6&clientId=uea5099d8-1753-4&from=ui&id=u9fa714be&originHeight=365&originWidth=1051&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1153149&status=done&style=shadow&taskId=ue930b1f5-7b7e-4fa2-97d3-2596d30ad73&title=)
<a name="Ip5KB"></a>
## `@Transactional`作用
根据上面的原理猜想，下面简单介绍每个步骤的源码以进行验证。<br />首先是`@Transactional`，作用是定义代理植入点。代理对象创建的通过`BeanPostProcessor`的实现类`AnnotationAwareAspectJAutoProxyCreator`的`postProcessAfterInstantiation`方法来实现个，如果需要进行代理，那么在这个方法就会返回一个代理对象给容器，同时判断植入点也是在这个方法中。<br />那么下面开始分析，在配置好注解驱动方式的事务管理之后，Spring会在ioc容器创建一个`BeanFactoryTransactionAttributeSourceAdvisor`实例，这个实例可以看作是一个切点，在判断一个bean在初始化过程中是否需要创建代理对象，都需要验证一次`BeanFactoryTransactionAttributeSourceAdvisor`是否是适用这个bean的切点。如果是，就需要创建代理对象，并且把`BeanFactoryTransactionAttributeSourceAdvisor`实例注入到代理对象中。<br />在`AopUtils#findAdvisorsThatCanApply`中判断切面是否适用当前bean，可以在这个地方断点分析调用堆栈，`AopUtils#findAdvisorsThatCanApply`一致调用，最终通过以下代码判断是否适用切点。

- `AbstractFallbackTransactionAttributeSource#computeTransactionAttribute(Method method, Class<?> targetClass)` 这里可以根据参数打上条件断点进行调试分析调用栈，targetClass就是目标class …一系列调用
- 最终`SpringTransactionAnnotationParser#parseTransactionAnnotation(java.lang.reflect.AnnotatedElement)`
```java
@Override
public TransactionAttribute parseTransactionAnnotation(AnnotatedElement ae) {
    //这里就是分析Method是否被@Transactional注解标注，有的话，不用说BeanFactoryTransactionAttributeSourceAdvisor适配当前bean，进行代理，并且注入切点
    //BeanFactoryTransactionAttributeSourceAdvisor
    AnnotationAttributes attributes = AnnotatedElementUtils.getMergedAnnotationAttributes(ae, Transactional.class);
    if (attributes != null) {
        return parseTransactionAnnotation(attributes);
    }
    else {
        return null;
    }
}
```
上面就是判断是否需要根据`@Transactional`进行代理对象创建的判断过程。`@Transactional`的作用一个就是标识方法需要被代理，一个就是携带事务管理需要的一些属性信息。
<a name="r890K"></a>
## 动态代理逻辑实现
【aop实现原理分析】中知道，aop最终的代理对象的代理方法是

- `DynamicAdvisedInterceptor#intercept`

所以可以在这个方法断点分析代理逻辑。
```java
@Override
public Object intercept(Object proxy, Method method, Object[] args, MethodProxy methodProxy) throws Throwable {
   Object oldProxy = null;
   boolean setProxyContext = false;
   Class<?> targetClass = null;
   Object target = null;
   try {
      if (this.advised.exposeProxy) {
         // Make invocation available if necessary.
         oldProxy = AopContext.setCurrentProxy(proxy);
         setProxyContext = true;
      }
      // May be null. Get as late as possible to minimize the time we
      // "own" the target, in case it comes from a pool...
      target = getTarget();
      if (target != null) {
         targetClass = target.getClass();
      }
       //follow
      List<Object> chain = this.advised.getInterceptorsAndDynamicInterceptionAdvice(method, targetClass);
      Object retVal;
      // Check whether we only have one InvokerInterceptor: that is,
      // no real advice, but just reflective invocation of the target.
      if (chain.isEmpty() && Modifier.isPublic(method.getModifiers())) {
         // We can skip creating a MethodInvocation: just invoke the target directly.
         // Note that the final invoker must be an InvokerInterceptor, so we know
         // it does nothing but a reflective operation on the target, and no hot
         // swapping or fancy proxying.
         Object[] argsToUse = AopProxyUtils.adaptArgumentsIfNecessary(method, args);
         retVal = methodProxy.invoke(target, argsToUse);
      }
      else {
         // We need to create a method invocation...
         retVal = new CglibMethodInvocation(proxy, target, method, args, targetClass, chain, methodProxy).proceed();
      }
      retVal = processReturnType(proxy, target, method, retVal);
      return retVal;
   }
   finally {
      if (target != null) {
         releaseTarget(target);
      }
      if (setProxyContext) {
         // Restore old proxy.
         AopContext.setCurrentProxy(oldProxy);
      }
   }
}
```
通过分析 `List<Object> chain = this.advised.getInterceptorsAndDynamicInterceptionAdvice(method, targetClass)`返回的是`TransactionInterceptor`,利用`TransactionInterceptor`是如何实现代理逻辑调用的？<br />跟踪`new CglibMethodInvocation(proxy, target, method, args, targetClass, chain, methodProxy).proceed();`<br />发现最终是调用`TransactionInterceptor#invoke`方法，并且把`CglibMethodInvocation`注入到`invoke`方法中，从上面可以看到`CglibMethodInvocation`是包装了目标对象的方法调用的所有必须信息，因此，在`TransactionInterceptor#invoke`里面也是可以调用目标方法的，并且还可以实现类似`@Around`的逻辑，在目标方法调用前后继续注入一些其他逻辑，比如事务管理逻辑。
<a name="JMIxi"></a>
## TransactionInterceptor–最终事务管理者
下面看代码。

- `TransactionInterceptor#invoke`
```java
@Override
public Object invoke(final MethodInvocation invocation) throws Throwable {
    // Work out the target class: may be {@code null}.
    // The TransactionAttributeSource should be passed the target class
    // as well as the method, which may be from an interface.
    Class<?> targetClass = (invocation.getThis() != null ? AopUtils.getTargetClass(invocation.getThis()) : null);

    // Adapt to TransactionAspectSupport's invokeWithinTransaction...
    return invokeWithinTransaction(invocation.getMethod(), targetClass, new InvocationCallback() {
        @Override
        public Object proceedWithInvocation() throws Throwable {
            return invocation.proceed();
        }
    });
}
```
继续跟踪`invokeWithinTransaction`，下面的代码中其实就可以看出一些逻辑端倪，就是猜想的实现方式，事务管理。
```java
protected Object invokeWithinTransaction(Method method, Class<?> targetClass, final InvocationCallback invocation)
      throws Throwable {

   // If the transaction attribute is null, the method is non-transactional.
   final TransactionAttribute txAttr = getTransactionAttributeSource().getTransactionAttribute(method, targetClass);
   final PlatformTransactionManager tm = determineTransactionManager(txAttr);
   final String joinpointIdentification = methodIdentification(method, targetClass);

   if (txAttr == null || !(tm instanceof CallbackPreferringPlatformTransactionManager)) {
      // Standard transaction demarcation with getTransaction and commit/rollback calls.
       //开启事务
      TransactionInfo txInfo = createTransactionIfNecessary(tm, txAttr, joinpointIdentification);
      Object retVal = null;
      try {
         // This is an around advice: Invoke the next interceptor in the chain.
         // This will normally result in a target object being invoked.
          //方法调用
         retVal = invocation.proceedWithInvocation();
      }
      catch (Throwable ex) {
         // target invocation exception
     //回滚事务
         completeTransactionAfterThrowing(txInfo, ex);
         throw ex;
      }
      finally {
         cleanupTransactionInfo(txInfo);
      }
       //提交事务
      commitTransactionAfterReturning(txInfo);
      return retVal;
   }

   else {
      // It's a CallbackPreferringPlatformTransactionManager: pass a TransactionCallback in.
      try {
         Object result = ((CallbackPreferringPlatformTransactionManager) tm).execute(txAttr,
               new TransactionCallback<Object>() {
                  @Override
                  public Object doInTransaction(TransactionStatus status) {
                     TransactionInfo txInfo = prepareTransactionInfo(tm, txAttr, joinpointIdentification, status);
                     try {
                        return invocation.proceedWithInvocation();
                     }
                     catch (Throwable ex) {
                        if (txAttr.rollbackOn(ex)) {
                           // A RuntimeException: will lead to a rollback.
                           if (ex instanceof RuntimeException) {
                              throw (RuntimeException) ex;
                           }
                           else {
                              throw new ThrowableHolderException(ex);
                           }
                        }
                        else {
                           // A normal return value: will lead to a commit.
                           return new ThrowableHolder(ex);
                        }
                     }
                     finally {
                        cleanupTransactionInfo(txInfo);
                     }
                  }
               });

         // Check result: It might indicate a Throwable to rethrow.
         if (result instanceof ThrowableHolder) {
            throw ((ThrowableHolder) result).getThrowable();
         }
         else {
            return result;
         }
      }
      catch (ThrowableHolderException ex) {
         throw ex.getCause();
      }
   }
}
```
<a name="JDBCN"></a>
## 总结
最终可以总结一下整个流程，跟开始的猜想对照。<br />![2021-05-31-17-53-01-277152.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622455136014-6d8d8060-f7f4-47a7-860d-1a24414f2ff8.png#averageHue=%23f6f6f6&clientId=uea5099d8-1753-4&from=ui&id=uc45ad5a1&originHeight=365&originWidth=1051&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1153149&status=done&style=none&taskId=uc3b34916-15cb-4b37-8154-3e6173b26e1&title=)<br />分析源码后对照<br />![2021-05-31-17-53-01-446663.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622455144001-ac4d66f9-9645-4ab5-9a76-815ebbcfa3dd.png#averageHue=%23f6f6f6&clientId=uea5099d8-1753-4&from=ui&id=uc18b3684&originHeight=277&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=899284&status=done&style=shadow&taskId=ubceccb41-24a1-4a04-8f79-29394901224&title=)
