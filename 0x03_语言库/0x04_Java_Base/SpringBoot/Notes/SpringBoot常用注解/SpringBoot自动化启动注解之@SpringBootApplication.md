Java SpringBoot
<a name="uGzRk"></a>
### 概述
先来了解一下 `@SpringBootApplication` 是什么，以及如何在一个简单的 Spring Boot 应用程序中使用它。先看看 Spring Team 在源码中对它的定义是什么？
> Indicates a configuration class that declares one or more `@Bean` methods and also triggers auto-configuration and component scanning. This is a convenience annotation that is equivalent to declaring `@Configuration`, `@EnableAutoConfiguration` and `@ComponentScan`.

> 表示一个配置类，它声明了一个或多个`@Bean`方法，也触发了自动配置和组件扫描。这是一个方便的注解，相当于声明了`@Configuration`、`@EnableAutoConfiguration`和`@ComponentScan`。

从上面的定义可以看出，`@SpringBootApplication` 注解其实是一个组合注解。使用 `@SpringBootApplication` 相当于同时使用了 `@Configuration`、`@EnableAutoConfiguration` 和 `@ComponentScan` 。`@SpringBootApplication` 是在 Spring Boot 1.2.0 之后才开始有的，如果项目使用的 Spring Boot 1.2.0 之前的版本，那就不能使用这个注解，只能完整的使用那 3 个注解来代替它。<br />接下来看看，通过一个简单的示例来看看怎么使用的。
<a name="WRErN"></a>
### `@SpringBootApplication` 示例
下面是一个简单的例子，说明如何使用 `@SpringBootApplication` 注解来编写 Spring Boot 应用程序。
```java
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
public class Application {
    public static void main(String[] args) {
        SpringApplication.run(Application.class, args);
    }
}
```
Spring Boot 项目的启动类非常的简洁，没有一行多余的代码。`@SpringBootApplication` 放在项目启动类上主要起到了自动化配置的作用。下面看看 `@SpringBootApplication` 具体的代码。
```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Inherited
@SpringBootConfiguration
@EnableAutoConfiguration
@ComponentScan(excludeFilters = { @Filter(type = FilterType.CUSTOM, classes = TypeExcludeFilter.class),
        @Filter(type = FilterType.CUSTOM, classes = AutoConfigurationExcludeFilter.class) })
public @interface SpringBootApplication {
    @AliasFor(annotation = EnableAutoConfiguration.class)
    Class<?>[] exclude() default {};
    
    @AliasFor(annotation = EnableAutoConfiguration.class)
    String[] excludeName() default {};
    @AliasFor(annotation = ComponentScan.class, attribute = "basePackages")
    String[] scanBasePackages() default {};
    @AliasFor(annotation = ComponentScan.class, attribute = "basePackageClasses")
    Class<?>[] scanBasePackageClasses() default {};
    @AliasFor(annotation = ComponentScan.class, attribute = "nameGenerator")
    Class<? extends BeanNameGenerator> nameGenerator() default BeanNameGenerator.class;
    @AliasFor(annotation = Configuration.class)
    boolean proxyBeanMethods() default true;
}
```
从 `@SpringBootApplication` 源码可以看出 `@SpringBootApplication` = `@SpringBootConfiguration` + `@ComponentScan` + `@EnableAutoConfiguration` 。<br />前面已经提过了，`@SpringBootApplication` 是3个注解的组合，下面分别介绍一下每个注解都有什么作用吧。
<a name="ZeSez"></a>
#### `@SpringBootConfiguration`
这个注解将一个类标记为基于 Java Config 的配置类。如果喜欢基于 Java 的配置而不是基于 XML 的配置，这一点就特别重要。
<a name="rH34O"></a>
#### `@ComponentScan`
该注解使组件扫描成为可能，这样创建的 Web 控制器类和其他组件将被自动发现，并在 Spring 应用上下文中注册为 Bean。编写的所有 `@Controller` 类将被该注解发现。
<a name="cJmvl"></a>
#### `@EnableAutoConfiguration`
这个注解可以启用 Spring Boot 自动配置功能。<br />如果仔细的话会发现和前面讲的不一样， `@SpringBootConfiguration` 是从那里冒出来的，不是应该是 `@Configuration` 吗？
<a name="aC3L0"></a>
### `@SpringBootConfiguration`
```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Configuration
@Indexed
public @interface SpringBootConfiguration {
    @AliasFor(annotation = Configuration.class)
    boolean proxyBeanMethods() default true;
}
```
从源码可以看出，`@SpringBootConfiguration` 继承自 `@Configuration`，二者功能也一致，标注当前类是配置类，不过 `@SpringBootConfiguration` 是一个特殊的标记类，在项目中只能使用一次。
<a name="JRotG"></a>
### `@ComponentScan`
```java
@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.TYPE)
@Documented
@Repeatable(ComponentScans.class)
public @interface ComponentScan {
    @AliasFor("basePackages")
    String[] value() default {};
    @AliasFor("value")
    String[] basePackages() default {};
    Class<?>[] basePackageClasses() default {};
    Class<? extends BeanNameGenerator> nameGenerator() default BeanNameGenerator.class;
    Class<? extends ScopeMetadataResolver> scopeResolver() default AnnotationScopeMetadataResolver.class;
    ScopedProxyMode scopedProxy() default ScopedProxyMode.DEFAULT;
    ...
}
```
`@ComponentScan` 可以通过该注解指定扫描某些包下包含如下注解的均自动注册为 Spring beans：`@Component`、`@Service`、 `@Repository`、 `@Controller` 等等注释的类。Spring Boot 除了可以使用 `@ComponentScan` 注解来加载bean，还可以使用 `@Import` 指定该类。
<a name="sU3B9"></a>
### `@EnableAutoConfiguration`
`@EnableAutoConfiguration` 的作用启动自动的配置，意思就是 Spring Boot 根据添加的 jar 包来配置项目的默认配置，比如根据 spring-boot-starter-web ，来判断项目是否需要添加了 web mvc 和 tomcat，就会自动配置 web 项目中所需要的默认配置。简单点说就是它会根据定义在 classpath 下的类，自动的生成一些 Bean，并加载到 Spring 的上下文中。
```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Inherited
@AutoConfigurationPackage
@Import(AutoConfigurationImportSelector.class)
public @interface EnableAutoConfiguration {
    String ENABLED_OVERRIDE_PROPERTY = "spring.boot.enableautoconfiguration";
    Class<?>[] exclude() default {};
    String[] excludeName() default {};
}
```
从上述源码中可以看到 `@Import` 引入了 `AutoConfigurationImportSelector` 类。`AutoConfigurationImportSelector` 使用了 Spring Core 包的 `SpringFactoriesLoader#loadFactoryNames()` 方法。`AutoConfigurationImportSelector` 类实现了 `DeferredImportSelector` 接口，并实现了 `selectImports` 方法，用来导出 `Configuration` 类。
```java
public class AutoConfigurationImportSelector implements DeferredImportSelector, BeanClassLoaderAware,
        ResourceLoaderAware, BeanFactoryAware, EnvironmentAware, Ordered {
    @Override
    public String[] selectImports(AnnotationMetadata annotationMetadata) {
        if (!isEnabled(annotationMetadata)) {
            return NO_IMPORTS;
        }
        AutoConfigurationEntry autoConfigurationEntry = getAutoConfigurationEntry(annotationMetadata);
        return StringUtils.toStringArray(autoConfigurationEntry.getConfigurations());
    }
    protected AutoConfigurationEntry getAutoConfigurationEntry(AnnotationMetadata annotationMetadata) {
        if (!isEnabled(annotationMetadata)) {
            return EMPTY_ENTRY;
        }
        AnnotationAttributes attributes = getAttributes(annotationMetadata);
        List<String> configurations = getCandidateConfigurations(annotationMetadata, attributes);
        ...
        return new AutoConfigurationEntry(configurations, exclusions);
    }
    protected boolean isEnabled(AnnotationMetadata metadata) {
        if (getClass() == AutoConfigurationImportSelector.class) {
            return getEnvironment().getProperty(EnableAutoConfiguration.ENABLED_OVERRIDE_PROPERTY, Boolean.class, true);
        }
        return true;
    }
    protected List<String> getCandidateConfigurations(AnnotationMetadata metadata, AnnotationAttributes attributes) {
        List<String> configurations = SpringFactoriesLoader.loadFactoryNames(getSpringFactoriesLoaderFactoryClass(),
                getBeanClassLoader());
        Assert.notEmpty(configurations, "No auto configuration classes found in META-INF/spring.factories. If you "
                + "are using a custom packaging, make sure that file is correct.");
        return configurations;
    }
}
```
导出的类是通过 `SpringFactoriesLoader#loadFactoryNames()` 读取了 classpath 下面的 `META-INF/spring.factories` 文件。
```java
public final class SpringFactoriesLoader {
    public static final String FACTORIES_RESOURCE_LOCATION = "META-INF/spring.factories";
    public static List<String> loadFactoryNames(Class<?> factoryType, @Nullable ClassLoader classLoader) {
        ClassLoader classLoaderToUse = classLoader;
        if (classLoaderToUse == null) {
            classLoaderToUse = SpringFactoriesLoader.class.getClassLoader();
        }
        String factoryTypeName = factoryType.getName();
        return loadSpringFactories(classLoaderToUse).getOrDefault(factoryTypeName, Collections.emptyList());
    }
    private static Map<String, List<String>> loadSpringFactories(ClassLoader classLoader) {
        Map<String, List<String>> result = cache.get(classLoader);
        if (result != null) {
            return result;
        }
        result = new HashMap<>();
        try {
            Enumeration<URL> urls = classLoader.getResources(FACTORIES_RESOURCE_LOCATION);
            while (urls.hasMoreElements()) {
                URL url = urls.nextElement();
                UrlResource resource = new UrlResource(url);
                Properties properties = PropertiesLoaderUtils.loadProperties(resource);
                for (Map.Entry<?, ?> entry : properties.entrySet()) {
                    String factoryTypeName = ((String) entry.getKey()).trim();
                    String[] factoryImplementationNames =
                            StringUtils.commaDelimitedListToStringArray((String) entry.getValue());
                    for (String factoryImplementationName : factoryImplementationNames) {
                        result.computeIfAbsent(factoryTypeName, key -> new ArrayList<>())
                                .add(factoryImplementationName.trim());
                    }
                }
            }
            // Replace all lists with unmodifiable lists containing unique elements
            result.replaceAll((factoryType, implementations) -> implementations.stream().distinct()
                    .collect(Collectors.collectingAndThen(Collectors.toList(), Collections::unmodifiableList)));
            cache.put(classLoader, result);
        }
        catch (IOException ex) {
            throw new IllegalArgumentException("Unable to load factories from location [" +
                    FACTORIES_RESOURCE_LOCATION + "]", ex);
        }
        return result;
    }
}
```
`META-INF/spring.factories` 文件中一部分自动配置的内容：
```java
# Auto Configure
org.springframework.boot.autoconfigure.EnableAutoConfiguration=\
org.springframework.boot.autoconfigure.admin.SpringApplicationAdminJmxAutoConfiguration,\
org.springframework.boot.autoconfigure.aop.AopAutoConfiguration,\
org.springframework.boot.autoconfigure.amqp.RabbitAutoConfiguration,\
org.springframework.boot.autoconfigure.batch.BatchAutoConfiguration,\
org.springframework.boot.autoconfigure.cache.CacheAutoConfiguration,\
org.springframework.boot.autoconfigure.cassandra.CassandraAutoConfiguration,\
org.springframework.boot.autoconfigure.context.ConfigurationPropertiesAutoConfiguration,\
org.springframework.boot.autoconfigure.context.LifecycleAutoConfiguration,\
org.springframework.boot.autoconfigure.context.MessageSourceAutoConfiguration,\
org.springframework.boot.autoconfigure.context.PropertyPlaceholderAutoConfiguration,\
org.springframework.boot.autoconfigure.couchbase.CouchbaseAutoConfiguration,\
org.springframework.boot.autoconfigure.dao.PersistenceExceptionTranslationAutoConfiguration,\
org.springframework.boot.autoconfigure.data.cassandra.CassandraDataAutoConfiguration,\
org.springframework.boot.autoconfigure.data.cassandra.CassandraReactiveDataAutoConfiguration,\
org.springframework.boot.autoconfigure.data.cassandra.CassandraReactiveRepositoriesAutoConfiguration,\
```
如果发现自动装配的 Bean 不是想要的，也可以 disable 它。比如说，不想要自动装配 Database 的那些Bean：
```java
@EnableAutoConfiguration(exclude  = {DataSourceAutoConfiguration.class})
```
这也是一种为 Spring Boot 项目瘦身的方法。可以看到网上一些为项目瘦身的方法都是通过这个注解来操作的。
<a name="aGiMP"></a>
### `@AutoConfigurationPackage`
`@EnableAutoConfiguration` 又继承了 `@AutoConfigurationPackage` ，`@AutoConfigurationPackage` 会引导类（`@SpringBootApplication` 标注的类）所在的包及下面所有子包里面的所有组件扫描到Spring容器。具体怎么实现的呢，来看代码，原来它 import 了 `AutoConfigurationPackages.Registrar.class`， 来看看它做了什么？
```java
@Import(AutoConfigurationPackages.Registrar.class)
public @interface AutoConfigurationPackage {
    String[] basePackages() default {};
    Class<?>[] basePackageClasses() default {};
}
static class Registrar implements ImportBeanDefinitionRegistrar, DeterminableImports {
    @Override
    public void registerBeanDefinitions(AnnotationMetadata metadata, BeanDefinitionRegistry registry) {
        register(registry, new PackageImports(metadata).getPackageNames().toArray(new String[0]));
    }
    @Override
    public Set<Object> determineImports(AnnotationMetadata metadata) {
        return Collections.singleton(new PackageImports(metadata));
    }
}
```
看代码就很容易理解，把注解扫描进来的 package 全部给注册到 spring bean中。这样 Spring Boot 的自动配置也就完成了。
