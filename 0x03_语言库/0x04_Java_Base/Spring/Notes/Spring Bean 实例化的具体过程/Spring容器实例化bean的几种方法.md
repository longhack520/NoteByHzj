JavaSpring
<a name="D8X8X"></a>
## 源码分析
本篇文章的核心逻辑入口是在`AbstractAutowireCapableBeanFactory#createBeanInstance()`方法里。 跟进`createBeanInstance(beanName, mbd, args)`方法。这个方法干了哪几件事？

- 首先尝试调用`obtainFromSupplier()`实例化bean
- 尝试调用`instantiateUsingFactoryMethod()`实例化bean
- 根据给定参数推断构造函数实例化bean
- 以上均无，则使用默认构造函数实例化bean

先贴一下代码，然后逐个分析。
```java
/**
  * Create a new instance for the specified bean, using an appropriate instantiation strategy:
  * factory method, constructor autowiring, or simple instantiation.
  *
  * 使用适当的实例化策略为指定的 bean 创建一个新实例：工厂方法、构造函数自动装配或简单实例化。
  *
  * @param beanName the name of the bean
  * @param mbd the bean definition for the bean
  * @param args explicit arguments to use for constructor or factory method invocation
  * @return a BeanWrapper for the new instance
  * @see #obtainFromSupplier
  * @see #instantiateUsingFactoryMethod
  * @see #autowireConstructor
  * @see #instantiateBean
  */
protected BeanWrapper createBeanInstance(String beanName, RootBeanDefinition mbd, @Nullable Object[] args) {
    // Make sure bean class is actually resolved at this point.
    // 确保此时实际解析了 bean 类。
    Class<?> beanClass = resolveBeanClass(mbd, beanName);

    if (beanClass != null && !Modifier.isPublic(beanClass.getModifiers()) && !mbd.isNonPublicAccessAllowed()) {
        throw new BeanCreationException(mbd.getResourceDescription(), beanName,
                                        "Bean class isn't public, and non-public access not allowed: " + beanClass.getName());
    }
    // 通过bd中提供的instanceSupplier来获取一个对象
    // 正常bd中都不会有这个instanceSupplier属性，这里也是Spring提供的一个扩展点，但实际上不常用
    Supplier<?> instanceSupplier = mbd.getInstanceSupplier();
    if (instanceSupplier != null) {
        return obtainFromSupplier(instanceSupplier, beanName);
    }

    //如果工厂方法不为null，则使用工厂方法初始化策略
    // bd中提供了factoryMethodName属性，那么要使用工厂方法的方式来创建对象，
    // 工厂方法又会区分静态工厂方法跟实例工厂方法
    if (mbd.getFactoryMethodName() != null) {
        return instantiateUsingFactoryMethod(beanName, mbd, args);
    }

    // Shortcut when re-creating the same bean...
    // 在原型模式下，如果已经创建过一次这个Bean了，那么就不需要再次推断构造函数了
    // 是否推断过构造函数
    boolean resolved = false;
    // 构造函数是否需要进行注入
    boolean autowireNecessary = false;
    if (args == null) {
        synchronized (mbd.constructorArgumentLock) {
            //一个类里面有多个构造函数，每个构造函数都有不同的参数，所以调用前需要根据参数锁定要调用的构造函数或工厂方法
            if (mbd.resolvedConstructorOrFactoryMethod != null) {
                resolved = true;
                autowireNecessary = mbd.constructorArgumentsResolved;
            }
        }
    }
    //如果已经解析过则使用解析好的构造函数方法，不需要再次锁定
    if (resolved) {
        if (autowireNecessary) {
            //构造函数自动注入
            return autowireConstructor(beanName, mbd, null, null);
        }
        else {
            //使用默认构造函数进行构造
            return instantiateBean(beanName, mbd);
        }
    }

    // Candidate constructors for autowiring?
    //需要根据参数解析构造函数
    Constructor<?>[] ctors = determineConstructorsFromBeanPostProcessors(beanClass, beanName);
    if (ctors != null || mbd.getResolvedAutowireMode() == AUTOWIRE_CONSTRUCTOR ||
        mbd.hasConstructorArgumentValues() || !ObjectUtils.isEmpty(args)) {
        //构造函数自动注入
        return autowireConstructor(beanName, mbd, ctors, args);
    }

    // Preferred constructors for default construction?
    // 默认构造的首选构造函数？
    ctors = mbd.getPreferredConstructors();
    if (ctors != null) {
        return autowireConstructor(beanName, mbd, ctors, null);
    }

    // No special handling: simply use no-arg constructor.
    //使用默认构造函数
    return instantiateBean(beanName, mbd);
}
```
<a name="jKjmI"></a>
### 使用`Supplier`实例化bean
这一块的逻辑对应如下
```java
// 通过bd中提供的instanceSupplier来获取一个对象
// 正常bd中都不会有这个instanceSupplier属性，这里也是Spring提供的一个扩展点，但实际上不常用
Supplier<?> instanceSupplier = mbd.getInstanceSupplier();
if (instanceSupplier != null) {
    return obtainFromSupplier(instanceSupplier, beanName);
}
```
Supplier在Spring里面的使用是相对比较新的，看了一个代码的commit记录，这一块的代码是在2016年12月19号加上的，相对来说还是比较新的。<br />Supplier就是一个Java 8提供的函数式编程接口，里面提供一个get()方法，可以通过这个方法实例化bean对象。
```
@FunctionalInterface
public interface Supplier<T> {

    /**
     * Gets a result.
     *
     * @return a result
     */
    T get();
}
```
跟进`obtainFromSupplier()`方法，可以看到逻辑是比较简单的，通过一个`NamedThreadLocal`设置依赖关系`beanName`，然后调用`instanceSupplier.get()`获取对象，随后包装成一个`BeanWrapper`返回。
```java
/**
  * Obtain a bean instance from the given supplier.
  *
  * 从给定的 supplier 处获取一个 bean 实例。
  *
  * @param instanceSupplier the configured supplier
  * @param beanName the corresponding bean name
  * @return a BeanWrapper for the new instance
  * @since 5.0
  * @see #getObjectForBeanInstance
  */
protected BeanWrapper obtainFromSupplier(Supplier<?> instanceSupplier, String beanName) {
    Object instance;

    // 这里是处理 Supplier 创建的 bean 的内外部名称依赖关系
    String outerBean = this.currentlyCreatedBean.get();
    this.currentlyCreatedBean.set(beanName);
    try {
        // 调用 get() 方法获取对象
        instance = instanceSupplier.get();
    }
    finally {
        if (outerBean != null) {
            this.currentlyCreatedBean.set(outerBean);
        }
        else {
            this.currentlyCreatedBean.remove();
        }
    }

    if (instance == null) {
        instance = new NullBean();
    }
    BeanWrapper bw = new BeanWrapperImpl(instance);
    initBeanWrapper(bw);
    return bw;
}
```
<a name="d1PTE"></a>
### 使用工厂方法实例化bean
<a name="SDYge"></a>
#### 简介
这一块的代码逻辑对应如下
```java
//如果工厂方法不为null，则使用工厂方法初始化策略
// bd中提供了factoryMethodName属性，那么要使用工厂方法的方式来创建对象，
// 工厂方法又会区分静态工厂方法跟实例工厂方法
if (mbd.getFactoryMethodName() != null) {
    return instantiateUsingFactoryMethod(beanName, mbd, args);
}
```
跟进`instantiateUsingFactoryMethod()`方法，可以看到这里先`new`了一个`ConstructorResolver`，这个`ConstructorResolver`非常重要，它会去推断合适的构造函数。实例化bean另起这篇文章来写，很大程度就是因为这个`ConstructorResolver`。
```java
/**
  * Instantiate the bean using a named factory method. The method may be static, if the
  * mbd parameter specifies a class, rather than a factoryBean, or an instance variable
  * on a factory object itself configured using Dependency Injection.
  *
  * 使用命名工厂方法实例化 bean。
  * 如果 mbd 参数指定一个类，而不是 factoryBean，或者使用依赖注入配置的工厂对象本身的实例变量，则该方法可能是静态的。
  *
  * @param beanName the name of the bean
  * @param mbd the bean definition for the bean
  * @param explicitArgs argument values passed in programmatically via the getBean method,
  * or {@code null} if none (-> use constructor argument values from bean definition)
  * @return a BeanWrapper for the new instance
  * @see #getBean(String, Object[])
  */
protected BeanWrapper instantiateUsingFactoryMethod(
    String beanName, RootBeanDefinition mbd, @Nullable Object[] explicitArgs) {

    return new ConstructorResolver(this).instantiateUsingFactoryMethod(beanName, mbd, explicitArgs);
}
```
首先来看一下`ConstructorResolver`类上的定义，翻译一下就是：**用于解析构造函数和工厂方法的委托，通过参数匹配执行构造函数解析。**<br />Delegate for resolving constructors and factory methods.Performs constructor resolution through argument matching.<br />这个类的作用非常强大，可以解析实例化对象的所需的构造器，如果有多个，会根据构造器的参数类型和给定的参数类型通过计算权重的方式去匹配一个最佳的构造器。<br />跟进方法`instantiateUsingFactoryMethod()`代码，这个方法整整三百行，可以说非常离谱。<br />这里先贴个图，看下基本的逻辑。其实逻辑也跟写业务差不多，先做些基础准备，这里就是初始化个`BeanWrapperImpl`，给工厂bean属性赋赋值等，然后就去缓存去工厂方法和参数，取不到就去解析获取方法和参数，然后利用工厂方法和参数，实例化一个对象返回，逻辑清晰。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1681383297392-010f495d-1396-481e-b273-42c29cac865a.png#averageHue=%23fdf7f5&clientId=u71c4be25-e264-4&from=paste&id=ubf19b281&originHeight=635&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8c538e06-afb6-47e8-ba01-5f308fe5b4e&title=)<br />下面分段来看一下代码吧。
<a name="exDtS"></a>
#### 基础属性赋值
这一块代码比较简单，就是初始化一个`BeanWrapperImpl`，然后给一些属性赋值。
```java
// 创建并初始化一个 BeanWrapperImpl
BeanWrapperImpl bw = new BeanWrapperImpl();
this.beanFactory.initBeanWrapper(bw);

// 实例化这个Bean的工厂Bean
Object factoryBean;
// 工厂Bean的Class
Class<?> factoryClass;
// 静态工厂方法或者是实例化工厂方法
boolean isStatic;

/*下面这段代码就是为上面申明的这三个属性赋值*/
String factoryBeanName = mbd.getFactoryBeanName();
if (factoryBeanName != null) {
    // 如果创建这个Bean的工厂就是这个Bean本身的话，那么直接抛出异常
    if (factoryBeanName.equals(beanName)) {
        throw new BeanDefinitionStoreException(mbd.getResourceDescription(), beanName,
                                               "factory-bean reference points back to the same bean definition");
    }
    // 得到创建这个Bean的工厂Bean
    factoryBean = this.beanFactory.getBean(factoryBeanName);
    if (mbd.isSingleton() && this.beanFactory.containsSingleton(beanName)) {
        throw new ImplicitlyAppearedSingletonException();
    }
    factoryClass = factoryBean.getClass();
    isStatic = false;
} else {
    // It's a static factory method on the bean class.
    // factoryBeanName为null,说明是通过静态工厂方法来实例化Bean的
    // 静态工厂进行实例化Bean，beanClass属性必须要是工厂的class,如果为空，直接报错
    if (!mbd.hasBeanClass()) {
        throw new BeanDefinitionStoreException(mbd.getResourceDescription(), beanName,
                                               "bean definition declares neither a bean class nor a factory-bean reference");
    }
    factoryBean = null;
    factoryClass = mbd.getBeanClass();
    isStatic = true;
}
```
<a name="KV7KY"></a>
#### 从缓存获取参数
完成准备后，接下来是从缓存中获取工厂方法和参数，这里也比较简单，跟着注释看看。这里需要注意`resolvedConstructorArguments`和`preparedConstructorArguments`这两个参数缓存，`resolvedConstructorArguments` 用于缓存**完全解析**的构造函数参数的包可见字段，`preparedConstructorArguments` 用于缓存**部分准备**好的构造函数参数的包可见字段。其中如果是`preparedConstructorArguments` 中存在参数的话，需要调用`resolvePreparedArguments()`方法再次进行解析。
```java
// 到这里已经得到了一个BeanWrapper,明确了实例化当前这个Bean到底是静态工厂还是实例工厂
// 并且已经确定了工厂Bean
//================//
// 最终确定的要用来创建对象的方法
Method factoryMethodToUse = null;
ArgumentsHolder argsHolderToUse = null;
Object[] argsToUse = null;

// 从缓存中解析获取参数
// 参数分析时已经说过，explicitArgs就是null
if (explicitArgs != null) {
    argsToUse = explicitArgs;
} else {
    // 下面这段代码是什么意思呢？
    // 在原型模式下，我们会多次创建一个Bean,所以Spring对参数以及所使用的方法做了缓存
    // 在第二次创建原型对象的时候会进入这段缓存的逻辑
    // 但是这里有个问题，为什么Spring对参数有两个缓存呢？
    // 一：resolvedConstructorArguments 用于缓存完全解析的构造函数参数的包可见字段
    // 二：preparedConstructorArguments 用于缓存部分准备好的构造函数参数的包可见字段
    Object[] argsToResolve = null;
    synchronized (mbd.constructorArgumentLock) {
        factoryMethodToUse = (Method) mbd.resolvedConstructorOrFactoryMethod;
        // 缓存已经解析过的工厂方法或者构造方法
        if (factoryMethodToUse != null && mbd.constructorArgumentsResolved) {
            // Found a cached factory method...
            // 找到一个缓存的工厂方法...resolvedConstructorArguments 跟 preparedConstructorArguments都是对参数的缓存
            argsToUse = mbd.resolvedConstructorArguments;
            if (argsToUse == null) {
                argsToResolve = mbd.preparedConstructorArguments;
            }
        }
    }
    if (argsToResolve != null) {
        // preparedConstructorArguments需要再次进行解析，其中主要完成了获取依赖以及类型转换等工作
        argsToUse = resolvePreparedArguments(beanName, mbd, bw, factoryMethodToUse, argsToResolve, true);
    }
}
```
跟进resolvePreparedArguments()方法，简单来说就是根据参数的类型去解析获取出需要的参数，同时这里也会完成类型转换。
```java
/**
  * Resolve the prepared arguments stored in the given bean definition.
  *
  * 解析存储在给定 bean 定义中的准备好的参数。
  *
  */
private Object[] resolvePreparedArguments(String beanName, RootBeanDefinition mbd, BeanWrapper bw,
                                          Executable executable, Object[] argsToResolve, boolean fallback) {

    // 获取类型转换器
    TypeConverter customConverter = this.beanFactory.getCustomTypeConverter();
    TypeConverter converter = (customConverter != null ? customConverter : bw);
    // 获取占位符解析器
    BeanDefinitionValueResolver valueResolver =
    new BeanDefinitionValueResolver(this.beanFactory, beanName, mbd, converter);
    Class<?>[] paramTypes = executable.getParameterTypes();

    Object[] resolvedArgs = new Object[argsToResolve.length];
    // 逐个遍历参数
    for (int argIndex = 0; argIndex < argsToResolve.length; argIndex++) {
        Object argValue = argsToResolve[argIndex];
        MethodParameter methodParam = MethodParameter.forExecutable(executable, argIndex);
        // 如果是自动装配标志，则进行依赖解析
        if (argValue == autowiredArgumentMarker) {
            argValue = resolveAutowiredArgument(methodParam, beanName, null, converter, fallback);
        }
            // 如果是 BeanMetadataElement 类型，则进行各种BeanDefinition的解析，因为 BeanMetadataElement 的实现基本上是各种BeanMetadataElement
        else if (argValue instanceof BeanMetadataElement) {
            argValue = valueResolver.resolveValueIfNecessary("constructor argument", argValue);
        }
            // String 类型
        else if (argValue instanceof String) {
            argValue = this.beanFactory.evaluateBeanDefinitionString((String) argValue, mbd);
        }
        Class<?> paramType = paramTypes[argIndex];
        try {
            // 进行类型转换
            resolvedArgs[argIndex] = converter.convertIfNecessary(argValue, paramType, methodParam);
        }
        catch (TypeMismatchException ex) {
            throw new UnsatisfiedDependencyException(
                mbd.getResourceDescription(), beanName, new InjectionPoint(methodParam),
                "Could not convert argument value of type [" + ObjectUtils.nullSafeClassName(argValue) +
                "] to required type [" + paramType.getName() + "]: " + ex.getMessage());
        }
    }
    return resolvedArgs;
}
```
<a name="Tqtyo"></a>
#### 解析获取工厂方法和参数
解析一下这段逻辑，这里会先去获取该工厂bean的所有方法作为候选方法，然后按照先前赋值的属性进行简单筛选。**如果非常巧合，只找到了一个，同时参数又为空，这时候就可以利用这个唯一的方法去实例化对象，省略了后面参数解析的复杂逻辑**。如果不巧，那就只好老老实实去解析参数匹配。

- 首先这里会对所有方法进行排序，这里会对给定的工厂方法进行排序，优先选择公共方法和具有最多参数的“贪婪”方法。结果将首先包含公共方法，参数数量减少，然后是非公共方法，参数数量再次减少。
- 然后会调用`resolveConstructorArguments()`去获取可执行方法的最少参数个数`minNrOfArgs`。
- 随后遍历所有的候选方法，方法的参数个数必须满足大于或等于可执行方法的最少参数个数`minNrOfArgs`，尝试通过方法参数名称，方法参数类型和给定的`resolvedValues`属性里面去查找或者通过自动注入获取到参数值，然后封装成`ArgumentsHolder`对象。
- 如果存在多个，通过权重计算获取最合适的工厂方法。如果最后没有找到工厂方法和参数，直接报错。如果最合适的有多个，那么也直接报错。
- 最后对工厂方法和参数进行缓存，然后调用`instantiate()`方法，使用前面解析出来的工厂方法和参数进行对象创建。

具体实现如下，可以跟着注释查看。这里就不全部再去解析，下面就简单分析一下确定方法执行所需的最小参数和封装`ArgumentsHolder`对象的逻辑。
```java
// 缓存中找不到方法获取参数，执行到这段代码说明是第一次实例化这个对象
if (factoryMethodToUse == null || argsToUse == null) {
    // Need to determine the factory method...
    // Try all methods with this name to see if they match the given arguments.
    // 需要确定工厂方法...尝试所有具有此名称的方法，看看它们是否与给定的参数匹配。
    // 如果被cglib代理的话，获取父类的class
    factoryClass = ClassUtils.getUserClass(factoryClass);

    // 获取到工厂类中的所有方法
    List<Method> candidateList = null;
    if (mbd.isFactoryMethodUnique) {
        if (factoryMethodToUse == null) {
            factoryMethodToUse = mbd.getResolvedFactoryMethod();
        }
        if (factoryMethodToUse != null) {
            candidateList = Collections.singletonList(factoryMethodToUse);
        }
    }
    if (candidateList == null) {
        candidateList = new ArrayList<>();
        // 获取到工厂类中的所有方法，接下来要一步步从这些方法中筛选出来符合要求的方法
        Method[] rawCandidates = getCandidateMethods(factoryClass, mbd);
        // 第一步筛选：之前 在第二段代码中已经推断了方法是静态或者非静态的
        // 所以这里第一个要求就是要满足静态/非静态这个条件
        // 第二个要求就是必须符合bd中定义的factoryMethodName的名称
        // 其中第二个要求请注意，如果bd是一个configurationClassBeanDefinition,
        // 也就是说是通过扫描@Bean注解产生的，那么在判断时还会添加是否标注了@Bean注解
        for (Method candidate : rawCandidates) {
            if (Modifier.isStatic(candidate.getModifiers()) == isStatic && mbd.isFactoryMethod(candidate)) {
                candidateList.add(candidate);
            }
        }
    }

    // 如果只有一个，则没有重载方法，不需要查找，这里就会省略后续复杂的推断了，可以直接确定方法
    if (candidateList.size() == 1 && explicitArgs == null && !mbd.hasConstructorArgumentValues()) {
        Method uniqueCandidate = candidateList.get(0);
        // 如果没有参数
        if (uniqueCandidate.getParameterCount() == 0) {
            mbd.factoryMethodToIntrospect = uniqueCandidate;
            synchronized (mbd.constructorArgumentLock) {
                mbd.resolvedConstructorOrFactoryMethod = uniqueCandidate;
                mbd.constructorArgumentsResolved = true;
                mbd.resolvedConstructorArguments = EMPTY_ARGS;
            }
            bw.setBeanInstance(instantiate(beanName, mbd, factoryBean, uniqueCandidate, EMPTY_ARGS));
            return bw;
        }
    }

    // 将之前得到的方法集合转换成数组
    // 到这一步得到的其实就是某一个方法的所有重载方法
    // 比如 codegitz(),codegitz(String name),codegitz(String name,int age)
    Method[] candidates = candidateList.toArray(new Method[0]);
    // 排序，public跟参数多的优先级越高
    AutowireUtils.sortFactoryMethods(candidates);

    // 用来保存从配置文件中解析出来的参数
    ConstructorArgumentValues resolvedValues = null;
    // 是否使用了自动注入，本段代码中没有使用到这个属性，但是在后面用到了
    boolean autowiring = (mbd.getResolvedAutowireMode() == AutowireCapableBeanFactory.AUTOWIRE_CONSTRUCTOR);
    int minTypeDiffWeight = Integer.MAX_VALUE;
    // 可能出现多个符合要求的方法，用这个集合保存，实际上如果这个集合有值，就会抛出异常了
    Set<Method> ambiguousFactoryMethods = null;

    int minNrOfArgs;
    // 必定为null,不考虑了
    if (explicitArgs != null) {
        minNrOfArgs = explicitArgs.length;
    } else {
        // We don't have arguments passed in programmatically, so we need to resolve the
        // arguments specified in the constructor arguments held in the bean definition.
        // 就是说配置文件中指定了要使用的参数，那么需要对其进行解析，解析后的值就存储在resolvedValues这个集合中
        if (mbd.hasConstructorArgumentValues()) {
            // 通过解析constructor-arg标签，将参数封装成了ConstructorArgumentValues
            // ConstructorArgumentValues这个类在下文我们专门分析
            ConstructorArgumentValues cargs = mbd.getConstructorArgumentValues();
            resolvedValues = new ConstructorArgumentValues();
            // 解析标签中的属性，类似进行类型转换，后文进行详细分析
            minNrOfArgs = resolveConstructorArguments(beanName, mbd, bw, cargs, resolvedValues);
        }
        else {
            // 配置文件中没有指定要使用的参数，所以执行方法的最小参数个数就是0
            minNrOfArgs = 0;
        }
    }

    LinkedList<UnsatisfiedDependencyException> causes = null;

    for (Method candidate : candidates) {
        Class<?>[] paramTypes = candidate.getParameterTypes();

        if (paramTypes.length >= minNrOfArgs) {
            ArgumentsHolder argsHolder;

            if (explicitArgs != null) {
                // Explicit arguments given -> arguments length must match exactly.
                // 给定的显式参数 -> 参数长度必须完全匹配。
                if (paramTypes.length != explicitArgs.length) {
                    continue;
                }
                argsHolder = new ArgumentsHolder(explicitArgs);
            }
            else {
                // Resolved constructor arguments: type conversion and/or autowiring necessary.
                // 已解决的构造函数参数：需要类型转换和自动装配。
                try {
                    String[] paramNames = null;
                    ParameterNameDiscoverer pnd = this.beanFactory.getParameterNameDiscoverer();
                    if (pnd != null) {
                        paramNames = pnd.getParameterNames(candidate);
                    }
                    // 给定解析的构造函数参数值，创建一个参数数组以调用构造函数或工厂方法
                    argsHolder = createArgumentArray(beanName, mbd, resolvedValues, bw,
                                                     paramTypes, paramNames, candidate, autowiring, candidates.length == 1);
                }
                catch (UnsatisfiedDependencyException ex) {
                    if (logger.isTraceEnabled()) {
                        logger.trace("Ignoring factory method [" + candidate + "] of bean '" + beanName + "': " + ex);
                    }
                    // Swallow and try next overloaded factory method.
                    if (causes == null) {
                        causes = new LinkedList<>();
                    }
                    causes.add(ex);
                    continue;
                }
            }

            // 计算给定参数和方法定义参数的权重，选择一个最合适的方法
            int typeDiffWeight = (mbd.isLenientConstructorResolution() ?
                                  argsHolder.getTypeDifferenceWeight(paramTypes) : argsHolder.getAssignabilityWeight(paramTypes));
            // Choose this factory method if it represents the closest match.
            // 如果它代表最接近的匹配，则选择此工厂方法。
            if (typeDiffWeight < minTypeDiffWeight) {
                factoryMethodToUse = candidate;
                argsHolderToUse = argsHolder;
                argsToUse = argsHolder.arguments;
                minTypeDiffWeight = typeDiffWeight;
                ambiguousFactoryMethods = null;
            }
                // Find out about ambiguity: In case of the same type difference weight
                // for methods with the same number of parameters, collect such candidates
                // and eventually raise an ambiguity exception.
                // However, only perform that check in non-lenient constructor resolution mode,
                // and explicitly ignore overridden methods (with the same parameter signature).
                // 找出歧义：如果具有相同数量的参数的方法的类型差异权重相同，则收集此类候选并最终引发歧义异常。
                // 但是，仅在非宽松构造函数解析模式下执行该检查，并显式忽略重写的方法（具有相同的参数签名）
                // 可以理解为，这里就是收集参数类型和数量一样，方法名一样，如果存在这种情况，最终会抛出异常
                // 为啥会出现这种情况，我理解可能是同名方法参数的顺序不一样导致的，例如 sayHi(String name,String age) 和 sayHi(String age,String name)
            else if (factoryMethodToUse != null && typeDiffWeight == minTypeDiffWeight &&
                     !mbd.isLenientConstructorResolution() &&
                     paramTypes.length == factoryMethodToUse.getParameterCount() &&
                     !Arrays.equals(paramTypes, factoryMethodToUse.getParameterTypes())) {
                if (ambiguousFactoryMethods == null) {
                    ambiguousFactoryMethods = new LinkedHashSet<>();
                    ambiguousFactoryMethods.add(factoryMethodToUse);
                }
                ambiguousFactoryMethods.add(candidate);
            }
        }
    }

    // 最终没有找到可用的工厂方法或者参数，或者有多个符合要求的方法等情况，进行异常处理
    if (factoryMethodToUse == null || argsToUse == null) {
        // 省略一些异常处理...
    }

    // 缓存参数
    if (explicitArgs == null && argsHolderToUse != null) {
        mbd.factoryMethodToIntrospect = factoryMethodToUse;
        argsHolderToUse.storeCache(mbd, factoryMethodToUse);
    }
}
```
简单的逻辑就不做过多解释了，这里重点来看下是怎么确定方法最小使用参数个数的。跟进代码`resolveConstructorArguments()`，该方法会先获取constructor-arg标签指定的参数个数，然后去获取参数的下标值，如果下标值更大，那么最小的参数个数就取下标值加一。
```java
/**
  * Resolve the constructor arguments for this bean into the resolvedValues object.
  * This may involve looking up other beans.
  * <p>This method is also used for handling invocations of static factory methods.
  *
  * 将此 bean 的构造函数参数解析为 resolvedValues 对象。这可能涉及查找其他 bean。
  * <p>此方法也用于处理静态工厂方法的调用。
  *
  * 方法目的：解析配置文件中指定的方法参数
  * beanName：bean名称
  * mbd：beanName对应的beanDefinition
  * bw：通过它进行类型转换
  * ConstructorArgumentValues cargs：解析标签得到的属性，还没有经过解析（类型转换）
  * ConstructorArgumentValues resolvedValues：已经经过解析的参数
  * 返回值：返回方法需要的最小参数个数
  *
  */
private int resolveConstructorArguments(String beanName, RootBeanDefinition mbd, BeanWrapper bw,
                                        ConstructorArgumentValues cargs, ConstructorArgumentValues resolvedValues) {

    // 是否有定制的类型转换器，没有的话直接使用BeanWrapper进行类型转换
    TypeConverter customConverter = this.beanFactory.getCustomTypeConverter();
    TypeConverter converter = (customConverter != null ? customConverter : bw);
    // 构造一个BeanDefinitionValueResolver，专门用于解析constructor-arg中的value属性，实际上还包括ref属性，内嵌bean标签等等
    BeanDefinitionValueResolver valueResolver =
    new BeanDefinitionValueResolver(this.beanFactory, beanName, mbd, converter);

    // minNrOfArgs 记录执行方法要求的最小参数个数，一般情况下就是等于constructor-arg标签指定的参数数量
    int minNrOfArgs = cargs.getArgumentCount();

    for (Map.Entry<Integer, ConstructorArgumentValues.ValueHolder> entry : cargs.getIndexedArgumentValues().entrySet()) {
        int index = entry.getKey();
        if (index < 0) {
            throw new BeanCreationException(mbd.getResourceDescription(), beanName,
                                            "Invalid constructor argument index: " + index);
        }
        // 这是啥意思呢？
        // 暂且你先这样理解
        // 假设A方法直接在配置文件中指定了index=3上要使用的参数，那么这个时候A方法至少需要4个参数
        // 但是其余的3个参数可能不是通过constructor-arg标签指定的，而是直接自动注入进来的，那么在配置文件中我们就只配置了index=3上的参数，也就是说 int minNrOfArgs = cargs.getArgumentCount()=1，这个时候 index=3,minNrOfArgs=1, 所以 minNrOfArgs = 3+1
        if (index > minNrOfArgs) {
            minNrOfArgs = index + 1;
        }
        ConstructorArgumentValues.ValueHolder valueHolder = entry.getValue();
        if (valueHolder.isConverted()) {
            // 如果已经转换过了，直接添加到resolvedValues集合中
            resolvedValues.addIndexedArgumentValue(index, valueHolder);
        }
        else {
            // 解析value/ref/内嵌bean标签等
            Object resolvedValue =
            valueResolver.resolveValueIfNecessary("constructor argument", valueHolder.getValue());
            // 将解析后的resolvedValue封装成一个新的ValueHolder，
            // 并将其source设置为解析constructor-arg得到的那个ValueHolder，
            // 后期会用到这个属性进行判断
            ConstructorArgumentValues.ValueHolder resolvedValueHolder =
            new ConstructorArgumentValues.ValueHolder(resolvedValue, valueHolder.getType(), valueHolder.getName());
            resolvedValueHolder.setSource(valueHolder);
            resolvedValues.addIndexedArgumentValue(index, resolvedValueHolder);
        }
    }
    // 对getGenericArgumentValues进行解析，代码基本一样，不再赘述
    for (ConstructorArgumentValues.ValueHolder valueHolder : cargs.getGenericArgumentValues()) {
        if (valueHolder.isConverted()) {
            resolvedValues.addGenericArgumentValue(valueHolder);
        }else {
            Object resolvedValue =
            valueResolver.resolveValueIfNecessary("constructor argument", valueHolder.getValue());
            ConstructorArgumentValues.ValueHolder resolvedValueHolder = new ConstructorArgumentValues.ValueHolder(
            resolvedValue, valueHolder.getType(), valueHolder.getName());
            resolvedValueHolder.setSource(valueHolder);
            resolvedValues.addGenericArgumentValue(resolvedValueHolder);
        }
    }

    return minNrOfArgs;
}
```
看完怎么确定最小方法参数个数的逻辑，接下来需要按照给定解析的构造函数参数值，创建一个参数数组以调用构造函数或工厂方法，就看一下封装`ArgumentsHolder`对象的过程，跟进代码`createArgumentArray()`方法查看。<br />这方法也没啥好说，就是逐个去获取，如果给定的参数没有，那就从容器中获取，这部分即为自动注入的参数。
```java
/**
  * Create an array of arguments to invoke a constructor or factory method,
  * given the resolved constructor argument values.
  *
  * 给定解析的构造函数参数值，创建一个参数数组以调用构造函数或工厂方法。
  */
private ArgumentsHolder createArgumentArray(
    String beanName, RootBeanDefinition mbd, @Nullable ConstructorArgumentValues resolvedValues,
    BeanWrapper bw, Class<?>[] paramTypes, @Nullable String[] paramNames, Executable executable,
    boolean autowiring, boolean fallback) throws UnsatisfiedDependencyException {

    TypeConverter customConverter = this.beanFactory.getCustomTypeConverter();
    TypeConverter converter = (customConverter != null ? customConverter : bw);

    ArgumentsHolder args = new ArgumentsHolder(paramTypes.length);
    Set<ConstructorArgumentValues.ValueHolder> usedValueHolders = new HashSet<>(paramTypes.length);
    Set<String> autowiredBeanNames = new LinkedHashSet<>(4);

    for (int paramIndex = 0; paramIndex < paramTypes.length; paramIndex++) {
        Class<?> paramType = paramTypes[paramIndex];
        String paramName = (paramNames != null ? paramNames[paramIndex] : "");
        // Try to find matching constructor argument value, either indexed or generic.
        // 尝试找到匹配的构造函数参数值，无论是索引的还是泛型的。
        ConstructorArgumentValues.ValueHolder valueHolder = null;
        if (resolvedValues != null) {
            valueHolder = resolvedValues.getArgumentValue(paramIndex, paramType, paramName, usedValueHolders);
            // If we couldn't find a direct match and are not supposed to autowire,
            // let's try the next generic, untyped argument value as fallback:
            // it could match after type conversion (for example, String -> int).
            // 如果我们找不到直接匹配并且不应该自动装配，让我们尝试下一个通用的、无类型的参数值作为后备：它可以在类型转换后匹配（例如，String -> int）。
            if (valueHolder == null && (!autowiring || paramTypes.length == resolvedValues.getArgumentCount())) {
                valueHolder = resolvedValues.getGenericArgumentValue(null, null, usedValueHolders);
            }
        }
        if (valueHolder != null) {
            // We found a potential match - let's give it a try.
            // Do not consider the same value definition multiple times!
            // 我们找到了一个潜在的匹配 - 让我们试一试。不要多次考虑相同的值定义！
            usedValueHolders.add(valueHolder);
            Object originalValue = valueHolder.getValue();
            Object convertedValue;
            if (valueHolder.isConverted()) {
                convertedValue = valueHolder.getConvertedValue();
                args.preparedArguments[paramIndex] = convertedValue;
            }
            else {
                MethodParameter methodParam = MethodParameter.forExecutable(executable, paramIndex);
                try {
                    convertedValue = converter.convertIfNecessary(originalValue, paramType, methodParam);
                }
                catch (TypeMismatchException ex) {
                    throw new UnsatisfiedDependencyException(
                        mbd.getResourceDescription(), beanName, new InjectionPoint(methodParam),
                        "Could not convert argument value of type [" +
                        ObjectUtils.nullSafeClassName(valueHolder.getValue()) +
                        "] to required type [" + paramType.getName() + "]: " + ex.getMessage());
                }
                Object sourceHolder = valueHolder.getSource();
                if (sourceHolder instanceof ConstructorArgumentValues.ValueHolder) {
                    Object sourceValue = ((ConstructorArgumentValues.ValueHolder) sourceHolder).getValue();
                    args.resolveNecessary = true;
                    args.preparedArguments[paramIndex] = sourceValue;
                }
            }
            args.arguments[paramIndex] = convertedValue;
            args.rawArguments[paramIndex] = originalValue;
        }
        else {
            // 这部分就是超出了参数定义，需要自动注入参数的处理
            MethodParameter methodParam = MethodParameter.forExecutable(executable, paramIndex);
            // No explicit match found: we're either supposed to autowire or
            // have to fail creating an argument array for the given constructor.
            if (!autowiring) {
                throw new UnsatisfiedDependencyException(
                    mbd.getResourceDescription(), beanName, new InjectionPoint(methodParam),
                    "Ambiguous argument values for parameter of type [" + paramType.getName() +
                    "] - did you specify the correct bean references as arguments?");
            }
            try {
                Object autowiredArgument = resolveAutowiredArgument(
                    methodParam, beanName, autowiredBeanNames, converter, fallback);
                args.rawArguments[paramIndex] = autowiredArgument;
                args.arguments[paramIndex] = autowiredArgument;
                args.preparedArguments[paramIndex] = autowiredArgumentMarker;
                args.resolveNecessary = true;
            }
            catch (BeansException ex) {
                throw new UnsatisfiedDependencyException(
                    mbd.getResourceDescription(), beanName, new InjectionPoint(methodParam), ex);
            }
        }
    }

    for (String autowiredBeanName : autowiredBeanNames) {
        this.beanFactory.registerDependentBean(autowiredBeanName, beanName);
        if (logger.isDebugEnabled()) {
            logger.debug("Autowiring by type from bean name '" + beanName +
                         "' via " + (executable instanceof Constructor ? "constructor" : "factory method") +
                         " to bean named '" + autowiredBeanName + "'");
        }
    }

    return args;
}
```
<a name="vVSka"></a>
#### 实例化bean
工厂方法和参数都已经获取完成了，到最后就是调用`instantiate()`方法去实例化一个bean对象，跟进`instantiate()`方法查看代码。可以看到这里就是确定一个构造策略，然后调用其`instantiate()`方法。
```java
private Object instantiate(String beanName, RootBeanDefinition mbd,
                           @Nullable Object factoryBean, Method factoryMethod, Object[] args) {

    try {
        if (System.getSecurityManager() != null) {
            return AccessController.doPrivileged((PrivilegedAction<Object>) () ->
                                                  this.beanFactory.getInstantiationStrategy().instantiate(
                                                      mbd, beanName, this.beanFactory, factoryBean, factoryMethod, args),
                                                 this.beanFactory.getAccessControlContext());
        }
        else {
            // 确定实例化策略，调用其 instantiate() 方法，
            // 该方法有两种实现，
            // 一种是普通的实现 SimpleInstantiationStrategy，
            // 一种是需要使用到代理的 CglibSubclassingInstantiationStrategy 实现
            return this.beanFactory.getInstantiationStrategy().instantiate(
                mbd, beanName, this.beanFactory, factoryBean, factoryMethod, args);
        }
    }
    catch (Throwable ex) {
        throw new BeanCreationException(mbd.getResourceDescription(), beanName,
                                        "Bean instantiation via factory method failed", ex);
    }
}
```
看一下普通的实现`SimpleInstantiationStrategy#instantiate()`，跟进代码查看。可以看到逻辑还是比较简单的，就是完成一些基础设置，然后直接调用方法获取对象返回。**至此，使用工厂方法实例化bean的分析已经完成了。**
```java
@Override
public Object instantiate(RootBeanDefinition bd, @Nullable String beanName, BeanFactory owner,
                          @Nullable Object factoryBean, final Method factoryMethod, Object... args) {

    try {
        // 权限相关，暂时忽略...
        if (System.getSecurityManager() != null) {
            AccessController.doPrivileged((PrivilegedAction<Object>) () -> {
                ReflectionUtils.makeAccessible(factoryMethod);
                return null;
            });
        }
        else {
            // 设置访问标识
            ReflectionUtils.makeAccessible(factoryMethod);
        }

        // 记录上一个调用的工厂方法
        Method priorInvokedFactoryMethod = currentlyInvokedFactoryMethod.get();
        try {
            // 设置当前调用的工厂方法
            currentlyInvokedFactoryMethod.set(factoryMethod);
            // 直接调用工厂方法获取对象
            Object result = factoryMethod.invoke(factoryBean, args);
            if (result == null) {
                result = new NullBean();
            }
            // 返回对象
            return result;
        }
        finally {
            // 后续状态的设置
            if (priorInvokedFactoryMethod != null) {
                currentlyInvokedFactoryMethod.set(priorInvokedFactoryMethod);
            }
            else {
                currentlyInvokedFactoryMethod.remove();
            }
        }
    }
    catch (IllegalArgumentException ex) {
        // 省略部分异常处理...
    }
}
```
<a name="hSTjk"></a>
### 使用有参构造函数实例化bean
还是回到`AbstractAutowireCapableBeanFactory#createBeanInstance()`方法里，如果经过了前面两种处理都没有获得一个实例化的对象的话，那么接下来就要使用构造函数去实例化对象了。<br />构造函数分为有参构造函数和无参构造函数。在这里会先匹配有参构造函数，然后再去使用默认的无参构造函数。<br />这里先讨论使用有参构造函数的情况，啥时候使用有参构造函数呢？在这里看来无非是两种：

- 指定了一个构造函数，spring提供了一个`determineConstructorsFromBeanPostProcessors()`方法来提供一个扩展口返回一个构造函数。
- 传入的参数不为空，这里会进入到`autowireConstructor()`方法里进行推断构造函数，这里的推断过程跟上面工厂方法推断过程非常类似，可以参考着看看。

先来看看指定一个构造函数是怎么处理，跟进`determineConstructorsFromBeanPostProcessors()`方法，这里可以看到就是调用了`SmartInstantiationAwareBeanPostProcessor#determineCandidateConstructors()`方法，没啥特别的。
```java
/**
  * Determine candidate constructors to use for the given bean, checking all registered
  * {@link SmartInstantiationAwareBeanPostProcessor SmartInstantiationAwareBeanPostProcessors}.
  *
  * 确定用于给定 bean 的候选构造函数，检查所有已注册的 {@link SmartInstantiationAwareBeanPostProcessor SmartInstantiationAwareBeanPostProcessors}。
  *
  * @param beanClass the raw class of the bean
  * @param beanName the name of the bean
  * @return the candidate constructors, or {@code null} if none specified
  * @throws org.springframework.beans.BeansException in case of errors
  * @see org.springframework.beans.factory.config.SmartInstantiationAwareBeanPostProcessor#determineCandidateConstructors
  */
@Nullable
protected Constructor<?>[] determineConstructorsFromBeanPostProcessors(@Nullable Class<?> beanClass, String beanName)
throws BeansException {

    if (beanClass != null && hasInstantiationAwareBeanPostProcessors()) {
        for (BeanPostProcessor bp : getBeanPostProcessors()) {
            if (bp instanceof SmartInstantiationAwareBeanPostProcessor) {
                SmartInstantiationAwareBeanPostProcessor ibp = (SmartInstantiationAwareBeanPostProcessor) bp;
                Constructor<?>[] ctors = ibp.determineCandidateConstructors(beanClass, beanName);
                if (ctors != null) {
                    return ctors;
                }
            }
        }
    }
    return null;
}
```
接下来看重点`autowireConstructor()`方法，这里的实现逻辑也是委派给了`ConstructorResolver`去实现。
```java
/**
  * "autowire constructor" (with constructor arguments by type) behavior.
  * Also applied if explicit constructor argument values are specified,
  * matching all remaining arguments with beans from the bean factory.
  * <p>This corresponds to constructor injection: In this mode, a Spring
  * bean factory is able to host components that expect constructor-based
  * dependency resolution.
  *
  * “自动装配构造函数”（按类型使用构造函数参数）行为。
  * 如果指定了显式构造函数参数值，也适用，将所有剩余参数与 bean 工厂中的 bean 匹配。
  * <p>这对应于构造函数注入：在这种模式下，Spring bean 工厂能够托管期望基于构造函数的依赖解析的组件。
  *
  * @param beanName the name of the bean
  * @param mbd the bean definition for the bean
  * @param ctors the chosen candidate constructors
  * @param explicitArgs argument values passed in programmatically via the getBean method,
  * or {@code null} if none (-> use constructor argument values from bean definition)
  * @return a BeanWrapper for the new instance
  */
protected BeanWrapper autowireConstructor(
    String beanName, RootBeanDefinition mbd, @Nullable Constructor<?>[] ctors, @Nullable Object[] explicitArgs) {

    return new ConstructorResolver(this).autowireConstructor(beanName, mbd, ctors, explicitArgs);
}
```
跟进`autowireConstructor()`方法，可以看到这里的逻辑跟工厂方法的解析是非常类似的，根据参数去推断合适的方法，然后获取参数的依赖，最后利用获取到的构造方法和参数实例化对象，详细可见上一节，这里不再赘述。
```java
/**
  * "autowire constructor" (with constructor arguments by type) behavior.
  * Also applied if explicit constructor argument values are specified,
  * matching all remaining arguments with beans from the bean factory.
  * <p>This corresponds to constructor injection: In this mode, a Spring
  * bean factory is able to host components that expect constructor-based
  * dependency resolution.
  *
  * “自动装配构造函数”（按类型使用构造函数参数）行为。
  * 如果指定了显式构造函数参数值，也适用，将所有剩余参数与 bean 工厂中的 bean 匹配。
  * <p>这对应于构造函数注入：在这种模式下，Spring bean 工厂能够托管期望基于构造函数的依赖解析的组件。
  *
  * @param beanName the name of the bean
  * @param mbd the merged bean definition for the bean
  * @param chosenCtors chosen candidate constructors (or {@code null} if none)
  * @param explicitArgs argument values passed in programmatically via the getBean method,
  * or {@code null} if none (-> use constructor argument values from bean definition)
  * 通过 getBean 方法以编程方式传入的参数值，如果没有则 {@code null}（-> 使用 bean 定义中的构造函数参数值）
  * @return a BeanWrapper for the new instance
  */
public BeanWrapper autowireConstructor(String beanName, RootBeanDefinition mbd,
                                       @Nullable Constructor<?>[] chosenCtors, @Nullable Object[] explicitArgs) {

    BeanWrapperImpl bw = new BeanWrapperImpl();
    this.beanFactory.initBeanWrapper(bw);

    Constructor<?> constructorToUse = null;
    ArgumentsHolder argsHolderToUse = null;
    Object[] argsToUse = null;

    if (explicitArgs != null) {
        argsToUse = explicitArgs;
    }
    else {
        Object[] argsToResolve = null;
        // 先到缓存中尝试获取构造器和参数
        synchronized (mbd.constructorArgumentLock) {
            constructorToUse = (Constructor<?>) mbd.resolvedConstructorOrFactoryMethod;
            if (constructorToUse != null && mbd.constructorArgumentsResolved) {
                // Found a cached constructor...
                // 如果已经解析过了构造器，则直接使用缓存的构造器
                argsToUse = mbd.resolvedConstructorArguments;
                if (argsToUse == null) {
                    argsToResolve = mbd.preparedConstructorArguments;
                }
            }
        }
        // 如果 mbd 存在待解析的参数，直接进行解析
        if (argsToResolve != null) {
            argsToUse = resolvePreparedArguments(beanName, mbd, bw, constructorToUse, argsToResolve, true);
        }
    }

    // 缓存中没有同时存在构造器和参数，老老实实去创建获取
    if (constructorToUse == null || argsToUse == null) {
        // Take specified constructors, if any.
        // 采用指定的构造函数，如果有的话。
        Constructor<?>[] candidates = chosenCtors;
        if (candidates == null) {
            Class<?> beanClass = mbd.getBeanClass();
            try {
                // 获取所有的构造器
                candidates = (mbd.isNonPublicAccessAllowed() ?
                              beanClass.getDeclaredConstructors() : beanClass.getConstructors());
            }
            catch (Throwable ex) {
                throw new BeanCreationException(mbd.getResourceDescription(), beanName,
                                                "Resolution of declared constructors on bean Class [" + beanClass.getName() +
                                                "] from ClassLoader [" + beanClass.getClassLoader() + "] failed", ex);
            }
        }

        // 如果只有一个构造器，且无指定的参数并且没有已解析的参数，直接使用该构造函数实例化对象返回
        if (candidates.length == 1 && explicitArgs == null && !mbd.hasConstructorArgumentValues()) {
            Constructor<?> uniqueCandidate = candidates[0];
            // 如果这个唯一的构造函数是无参构造函数，设置 mbd 属性，实例化后返回
            if (uniqueCandidate.getParameterCount() == 0) {
                synchronized (mbd.constructorArgumentLock) {
                    mbd.resolvedConstructorOrFactoryMethod = uniqueCandidate;
                    mbd.constructorArgumentsResolved = true;
                    mbd.resolvedConstructorArguments = EMPTY_ARGS;
                }
                // 实例化返回
                bw.setBeanInstance(instantiate(beanName, mbd, uniqueCandidate, EMPTY_ARGS));
                return bw;
            }
        }

        // Need to resolve the constructor.
        // 不是无参构造函数，需要解析构造函数。
        boolean autowiring = (chosenCtors != null ||
                              mbd.getResolvedAutowireMode() == AutowireCapableBeanFactory.AUTOWIRE_CONSTRUCTOR);
        ConstructorArgumentValues resolvedValues = null;

        int minNrOfArgs;
        // 确定最少参数的个数
        if (explicitArgs != null) {
            minNrOfArgs = explicitArgs.length;
        }
        else {
            ConstructorArgumentValues cargs = mbd.getConstructorArgumentValues();
            resolvedValues = new ConstructorArgumentValues();
            minNrOfArgs = resolveConstructorArguments(beanName, mbd, bw, cargs, resolvedValues);
        }

        AutowireUtils.sortConstructors(candidates);
        int minTypeDiffWeight = Integer.MAX_VALUE;
        Set<Constructor<?>> ambiguousConstructors = null;
        LinkedList<UnsatisfiedDependencyException> causes = null;

        for (Constructor<?> candidate : candidates) {
            Class<?>[] paramTypes = candidate.getParameterTypes();

            if (constructorToUse != null && argsToUse != null && argsToUse.length > paramTypes.length) {
                // Already found greedy constructor that can be satisfied ->
                // do not look any further, there are only less greedy constructors left.
                // 已经找到可以满足的贪心构造器了——>别再看了，贪心构造器就少了。
                break;
            }
            if (paramTypes.length < minNrOfArgs) {
                continue;
            }

            ArgumentsHolder argsHolder;
            if (resolvedValues != null) {
                try {
                    String[] paramNames = ConstructorPropertiesChecker.evaluate(candidate, paramTypes.length);
                    if (paramNames == null) {
                        ParameterNameDiscoverer pnd = this.beanFactory.getParameterNameDiscoverer();
                        if (pnd != null) {
                            paramNames = pnd.getParameterNames(candidate);
                        }
                    }
                    argsHolder = createArgumentArray(beanName, mbd, resolvedValues, bw, paramTypes, paramNames,
                                                     getUserDeclaredConstructor(candidate), autowiring, candidates.length == 1);
                }
                catch (UnsatisfiedDependencyException ex) {
                    if (logger.isTraceEnabled()) {
                        logger.trace("Ignoring constructor [" + candidate + "] of bean '" + beanName + "': " + ex);
                    }
                    // Swallow and try next constructor.
                    if (causes == null) {
                        causes = new LinkedList<>();
                    }
                    causes.add(ex);
                    continue;
                }
            }
            else {
                // Explicit arguments given -> arguments length must match exactly.
                if (paramTypes.length != explicitArgs.length) {
                    continue;
                }
                argsHolder = new ArgumentsHolder(explicitArgs);
            }

            // 获取构造函数和给定参数间的类型权重
            int typeDiffWeight = (mbd.isLenientConstructorResolution() ?
                                  argsHolder.getTypeDifferenceWeight(paramTypes) : argsHolder.getAssignabilityWeight(paramTypes));
            // Choose this constructor if it represents the closest match.
            // 如果它代表最接近的匹配，则选择此构造函数。
            if (typeDiffWeight < minTypeDiffWeight) {
                constructorToUse = candidate;
                argsHolderToUse = argsHolder;
                argsToUse = argsHolder.arguments;
                minTypeDiffWeight = typeDiffWeight;
                ambiguousConstructors = null;
            }
            else if (constructorToUse != null && typeDiffWeight == minTypeDiffWeight) {
                if (ambiguousConstructors == null) {
                    ambiguousConstructors = new LinkedHashSet<>();
                    ambiguousConstructors.add(constructorToUse);
                }
                ambiguousConstructors.add(candidate);
            }
        }

        if (constructorToUse == null) {
            if (causes != null) {
                UnsatisfiedDependencyException ex = causes.removeLast();
                for (Exception cause : causes) {
                    this.beanFactory.onSuppressedException(cause);
                }
                throw ex;
            }
            throw new BeanCreationException(mbd.getResourceDescription(), beanName,
                                            "Could not resolve matching constructor " +
                                            "(hint: specify index/type/name arguments for simple parameters to avoid type ambiguities)");
        }
        else if (ambiguousConstructors != null && !mbd.isLenientConstructorResolution()) {
            throw new BeanCreationException(mbd.getResourceDescription(), beanName,
                                            "Ambiguous constructor matches found in bean '" + beanName + "' " +
                                            "(hint: specify index/type/name arguments for simple parameters to avoid type ambiguities): " +
                                            ambiguousConstructors);
        }

        if (explicitArgs == null && argsHolderToUse != null) {
            argsHolderToUse.storeCache(mbd, constructorToUse);
        }
    }

    Assert.state(argsToUse != null, "Unresolved constructor arguments");
    bw.setBeanInstance(instantiate(beanName, mbd, constructorToUse, argsToUse));
    return bw;
}
```
<a name="EI7CG"></a>
### 使用默认构造函数实例化bean
如果前面的逻辑都没有获取到一个实例化bean，那么就会走到最后的默认逻辑`instantiateBean()`里，这里会使用默认的构造函数去实例化一个bean返回。<br />跟进`instantiateBean()`方法代码查看。
```java
/**
  * Instantiate the given bean using its default constructor.
  *
  * 使用其默认构造函数实例化给定的 bean。
  *
  * @param beanName the name of the bean
  * @param mbd the bean definition for the bean
  * @return a BeanWrapper for the new instance
  */
protected BeanWrapper instantiateBean(final String beanName, final RootBeanDefinition mbd) {
    try {
        Object beanInstance;
        final BeanFactory parent = this;
        if (System.getSecurityManager() != null) {
            beanInstance = AccessController.doPrivileged((PrivilegedAction<Object>) () ->
                                                          getInstantiationStrategy().instantiate(mbd, beanName, parent),
                                                         getAccessControlContext());
        }
        else {
            // 使用默认的实例化策略来实例化对象，默认为 CglibSubclassingInstantiationStrategy 实现，但是instantiate()方法只在SimpleInstantiationStrategy里有实现逻辑
            beanInstance = getInstantiationStrategy().instantiate(mbd, beanName, parent);
        }
        BeanWrapper bw = new BeanWrapperImpl(beanInstance);
        initBeanWrapper(bw);
        return bw;
    }
    catch (Throwable ex) {
        throw new BeanCreationException(
            mbd.getResourceDescription(), beanName, "Instantiation of bean failed", ex);
    }
}
```
进入`SimpleInstantiationStrategy#instantiate()`方法，这里获取了默认的构造函数，然后调用`BeanUtils.instantiateClass(constructorToUse)`实例化对象，**注意这里没有传入构造参数。**
```java
@Override
public Object instantiate(RootBeanDefinition bd, @Nullable String beanName, BeanFactory owner) {
    // Don't override the class with CGLIB if no overrides.
    /**
     * 如果需要覆盖或者动态替换方法，则使用cglib进行动态代理
     * 因为可以在创建动态代理的同时将动态方法织入类中
     * 如果没有需要改变的方法，为了方便直接反射即可
     */
    if (!bd.hasMethodOverrides()) {
        Constructor<?> constructorToUse;
        synchronized (bd.constructorArgumentLock) {
            constructorToUse = (Constructor<?>) bd.resolvedConstructorOrFactoryMethod;
            if (constructorToUse == null) {
                final Class<?> clazz = bd.getBeanClass();
                if (clazz.isInterface()) {
                    throw new BeanInstantiationException(clazz, "Specified class is an interface");
                }
                try {
                    if (System.getSecurityManager() != null) {
                        constructorToUse = AccessController.doPrivileged(
                            (PrivilegedExceptionAction<Constructor<?>>) clazz::getDeclaredConstructor);
                    }
                    else {
                        constructorToUse = clazz.getDeclaredConstructor();
                    }
                    bd.resolvedConstructorOrFactoryMethod = constructorToUse;
                }
                catch (Throwable ex) {
                    throw new BeanInstantiationException(clazz, "No default constructor found", ex);
                }
            }
        }
        return BeanUtils.instantiateClass(constructorToUse);
    }
    else {
        // Must generate CGLIB subclass.
        return instantiateWithMethodInjection(bd, beanName, owner);
    }
}
```
跟进`BeanUtils.instantiateClass(constructorToUse)`的实现，**这里注意，上一步并没有传入参数，这里使用的是无参构造函数。**最后是调用了`ctor.newInstance(argsWithDefaultValues)`实例化一个对象返回。至此，所有实例化对象的方法都已经分析完成。
```java
/**
  * Convenience method to instantiate a class using the given constructor.
  * <p>Note that this method tries to set the constructor accessible if given a
  * non-accessible (that is, non-public) constructor, and supports Kotlin classes
  * with optional parameters and default values.
  *
  * 使用给定构造函数实例化类的便捷方法。
  * <p>请注意，如果给定一个不可访问（即非公共）构造函数，
  * 此方法会尝试将构造函数设置为可访问，并且支持带有可选参数和默认值的 Kotlin 类。
  *
  * @param ctor the constructor to instantiate
  * @param args the constructor arguments to apply (use {@code null} for an unspecified
  * parameter, Kotlin optional parameters and Java primitive types are supported)
  * @return the new instance
  * @throws BeanInstantiationException if the bean cannot be instantiated
  * @see Constructor#newInstance
  */
public static <T> T instantiateClass(Constructor<T> ctor, Object... args) throws BeanInstantiationException {
    Assert.notNull(ctor, "Constructor must not be null");
    try {
        ReflectionUtils.makeAccessible(ctor);
        if (KotlinDetector.isKotlinReflectPresent() && KotlinDetector.isKotlinType(ctor.getDeclaringClass())) {
            return KotlinDelegate.instantiateClass(ctor, args);
        }
        else {
            // 获取构造器的参数
            Class<?>[] parameterTypes = ctor.getParameterTypes();
            Assert.isTrue(args.length <= parameterTypes.length, "Can't specify more arguments than constructor parameters");
            Object[] argsWithDefaultValues = new Object[args.length];
            // 遍历获取传入的 args 参数
            for (int i = 0 ; i < args.length; i++) {
                if (args[i] == null) {
                    Class<?> parameterType = parameterTypes[i];
                    argsWithDefaultValues[i] = (parameterType.isPrimitive() ? DEFAULT_TYPE_VALUES.get(parameterType) : null);
                }
                else {
                    argsWithDefaultValues[i] = args[i];
                }
            }
            return ctor.newInstance(argsWithDefaultValues);
        }
    }
    catch (InstantiationException ex) {
        // 省略部分异常处理...
    }
}
```
<a name="q0Jo1"></a>
## 总结
主要是分析了实例化bean的几种方法，有哪几种？纸面上来看是四种，但是从实际来看，后面的三种都是确定一个函数以及函数的参数来实例化一个对象，有异曲同工之妙，可以细细品味。实例化对象如果不指定的话，默认用的都是无参构造函数。<br />**总的来说这篇文章比较简单，就讲了实例化对象这一件事情**，相比较之前兜兜转转的逻辑来说可能相对层次浅一点。难点在于理解方法和函数之间的匹配，怎么通过类型权重去确定一个最佳方法，这里需要花点时间琢磨一下。
