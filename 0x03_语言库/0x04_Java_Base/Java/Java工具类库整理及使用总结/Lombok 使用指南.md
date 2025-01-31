Java Lombok
<a name="XDyQK"></a>
## 1、Lombok简介
Lombok 是一款 Java开发插件，使得 Java 开发者可以通过其定义的一些注解来消除业务工程中冗长和繁琐的代码，尤其对于简单的 Java 模型对象（POJO）。在开发环境中使用 Lombok插件后，Java 开发人员可以节省出重复构建，诸如 `hashCode` 和 `equals` 这样的方法以及各种业务对象模型的 `accessor` 和 `toString` 等方法的大量时间。对于这些方法，Lombok 能够在编译源代码期间自动生成这些方法，但并不会像反射那样降低程序的性能。
<a name="EC6mE"></a>
## 2、Lombok安装
<a name="xVZfg"></a>
### 构建工具
<a name="kdcq8"></a>
#### Gradle
在 build.gradle 文件中添加 Lombok 依赖：
```groovy
dependencies {
    compileOnly 'org.projectlombok:lombok:1.18.10'
    annotationProcessor 'org.projectlombok:lombok:1.18.10'
}
```
<a name="DqF76"></a>
#### Maven
在 Maven 项目的 pom.xml 文件中添加 Lombok 依赖：
```xml
<dependency>
    <groupId>org.projectlombok</groupId>
    <artifactId>lombok</artifactId>
    <version>1.18.10</version>
    <scope>provided</scope>
</dependency>
```
<a name="LANv1"></a>
#### Ant
假设在 lib 目录中已经存在 lombok.jar，然后设置 javac 任务：
```xml
<javac srcdir="src" destdir="build" source="1.8">
    <classpath location="lib/lombok.jar" />
</javac>
```
<a name="sFicf"></a>
### IDE
由于 Lombok 仅在编译阶段生成代码，所以使用 Lombok 注解的源代码，在 IDE 中会被高亮显示错误，针对这个问题可以通过安装 IDE 对应的插件来解决。这里不详细展开，具体的安装方式可以参考:<br />[https://www.baeldung.com/lombok-ide](https://www.baeldung.com/lombok-ide)
<a name="mQjiN"></a>
## 3、Lombok 详解
注意：以下示例所使用的 Lombok 版本是 1.18.10
<a name="NWWp8"></a>
### 3.1 `@Getter`  and `@Setter` 
可以使用 `@Getter` 或 `@Setter` 注释任何类或字段，Lombok 会自动生成默认的 getter/setter 方法。
<a name="qSAbA"></a>
#### `@Getter`  注解 
```java
@Target({ElementType.FIELD, ElementType.TYPE})
@Retention(RetentionPolicy.SOURCE)
public @interface Getter {
    // 若getter方法非public的话，可以设置可访问级别
    lombok.AccessLevel value() default lombok.AccessLevel.PUBLIC;
    AnyAnnotation[] onMethod() default {};
    // 是否启用延迟初始化
    boolean lazy() default false;
}
```
<a name="HirdD"></a>
#### `@Setter `
```java
@Target({ElementType.FIELD, ElementType.TYPE})
@Retention(RetentionPolicy.SOURCE)
public @interface Setter {
    // 若setter方法非public的话，可以设置可访问级别
    lombok.AccessLevel value() default lombok.AccessLevel.PUBLIC;
    AnyAnnotation[] onMethod() default {};
    AnyAnnotation[] onParam() default {};
}
```
使用示例
```java
@Getter
@Setter
public class GetterAndSetterDemo {
    String firstName;
    String lastName;
    LocalDate dateOfBirth;
}
```
以上代码经过 Lombok 编译后，会生成如下代码：
```java
public class GetterAndSetterDemo {
    String firstName;
    String lastName;
    LocalDate dateOfBirth;
    public GetterAndSetterDemo() {
    }
    // 省略其它setter和getter方法
    public String getFirstName() {
        return this.firstName;
    }
    public void setFirstName(String firstName) {
        this.firstName = firstName;
    }
}
```
<a name="L2DEH"></a>
#### Lazy Getter
`@Getter`  注解支持一个 lazy 属性，该属性默认为 `false`。当设置为 `true` 时，会启用延迟初始化，即当首次调用 getter 方法时才进行初始化。 <br />示例
```java
public class LazyGetterDemo {
    public static void main(String[] args) {
        LazyGetterDemo m = new LazyGetterDemo();
        System.out.println("Main instance is created");
        m.getLazy();
    }
    @Getter
    private final String notLazy = createValue("not lazy");
    @Getter(lazy = true)
    private final String lazy = createValue("lazy");
    private String createValue(String name) {
        System.out.println("createValue(" + name + ")");
        return null;
    }
}
```
以上代码经过 Lombok 编译后，会生成如下代码：
```java
public class LazyGetterDemo {
    private final String notLazy = this.createValue("not lazy");
    private final AtomicReference<Object> lazy = new AtomicReference();
    // 已省略部分代码
    public String getNotLazy() {
        return this.notLazy;
    }
    public String getLazy() {
        Object value = this.lazy.get();
        if (value == null) {
            synchronized(this.lazy) {
                value = this.lazy.get();
                if (value == null) {
                    String actualValue = this.createValue("lazy");
                    value = actualValue == null ? this.lazy : actualValue;
                    this.lazy.set(value);
                }
            }
        }
        return (String)((String)(value == this.lazy ? null : value));
    }
}
```
通过以上代码可知，调用 getLazy 方法时，若发现 value 为 null，则会在同步代码块中执行初始化操作。
<a name="FBblG"></a>
### 3.2 Constructor Annotations
<a name="af5ar"></a>
#### `@NoArgsConstructor` 
使用 `@NoArgsConstructor`  注解可以为指定类，生成默认的构造函数，`@NoArgsConstructor`  注解的定义如下： 
```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.SOURCE)
public @interface NoArgsConstructor {
    // 若设置该属性，将会生成一个私有的构造函数且生成一个staticName指定的静态方法
    String staticName() default "";
    AnyAnnotation[] onConstructor() default {};
    // 设置生成构造函数的访问级别，默认是public
    AccessLevel access() default lombok.AccessLevel.PUBLIC;
    // 若设置为true，则初始化所有final的字段为0/null/false
    boolean force() default false;
}
```
示例
```java
@NoArgsConstructor(staticName = "getInstance")
public class NoArgsConstructorDemo {
    private long id;
    private String name;
    private int age;
}
```
以上代码经过 Lombok 编译后，会生成如下代码：
```java
public class NoArgsConstructorDemo {
    private long id;
    private String name;
    private int age;
    private NoArgsConstructorDemo() {
    }
    public static NoArgsConstructorDemo getInstance() {
        return new NoArgsConstructorDemo();
    }
}
```
<a name="nfdaY"></a>
#### `@AllArgsConstructor` 
使用 `@AllArgsConstructor`  注解可以为指定类，生成包含所有成员的构造函数，`@AllArgsConstructor`  注解的定义如下： 
```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.SOURCE)
public @interface AllArgsConstructor {
    // 若设置该属性，将会生成一个私有的构造函数且生成一个staticName指定的静态方法
    String staticName() default "";
    AnyAnnotation[] onConstructor() default {};
    // 设置生成构造函数的访问级别，默认是public
    AccessLevel access() default lombok.AccessLevel.PUBLIC;
}
```
示例
```java
@AllArgsConstructor
public class AllArgsConstructorDemo {
    private long id;
    private String name;
    private int age;
}
```
以上代码经过 Lombok 编译后，会生成如下代码：
```java
public class AllArgsConstructorDemo {
    private long id;
    private String name;
    private int age;
    public AllArgsConstructorDemo(long id, String name, int age) {
        this.id = id;
        this.name = name;
        this.age = age;
    }
}
```
<a name="kqX3n"></a>
#### `@RequiredArgsConstructor`
使用 `@RequiredArgsConstructor` 注解可以为指定类必需初始化的成员变量，如 `final` 成员变量，生成对应的构造函数，`@RequiredArgsConstructor` 注解的定义如下：
```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.SOURCE)
public @interface RequiredArgsConstructor {
    // 若设置该属性，将会生成一个私有的构造函数且生成一个staticName指定的静态方法
    String staticName() default "";
    AnyAnnotation[] onConstructor() default {};
    // 设置生成构造函数的访问级别，默认是public
    AccessLevel access() default lombok.AccessLevel.PUBLIC;
}
```
示例
```java
@RequiredArgsConstructor
public class RequiredArgsConstructorDemo {
    private final long id;
    private String name;
    private int age;
}
```
以上代码经过 Lombok 编译后，会生成如下代码：
```java
public class RequiredArgsConstructorDemo {
    private final long id;
    private String name;
    private int age;
    public RequiredArgsConstructorDemo(long id) {
        this.id = id;
    }
}
```
<a name="lcql8"></a>
### 3.3 `@EqualsAndHashCode` 
使用 `@EqualsAndHashCode`  注解可以为指定类生成 `equals` 和 `hashCode` 方法， `@EqualsAndHashCode`  注解的定义如下： 
```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.SOURCE)
public @interface EqualsAndHashCode {
    // 指定在生成的equals和hashCode方法中需要排除的字段列表
    String[] exclude() default {};

    // 显式列出用于identity的字段，一般情况下non-static,non-transient字段会被用于identity
    String[] of() default {};

    // 标识在执行字段计算前，是否调用父类的equals和hashCode方法
    boolean callSuper() default false;

    boolean doNotUseGetters() default false;

    AnyAnnotation[] onParam() default {};

    @Deprecated
    @Retention(RetentionPolicy.SOURCE)
    @Target({})
    @interface AnyAnnotation {}

    @Target(ElementType.FIELD)
    @Retention(RetentionPolicy.SOURCE)
    public @interface Exclude {}

    @Target({ElementType.FIELD, ElementType.METHOD})
    @Retention(RetentionPolicy.SOURCE)
    public @interface Include {
        String replaces() default "";
    }
}
```
示例
```java
@EqualsAndHashCode
public class EqualsAndHashCodeDemo {
    String firstName;
    String lastName;
    LocalDate dateOfBirth;
}
```
以上代码经过 Lombok 编译后，会生成如下代码：
```java
public class EqualsAndHashCodeDemo {
    String firstName;
    String lastName;
    LocalDate dateOfBirth;
    public EqualsAndHashCodeDemo() {
    }
    public boolean equals(Object o) {
        if (o == this) {
            return true;
        } else if (!(o instanceof EqualsAndHashCodeDemo)) {
            return false;
        } else {
            EqualsAndHashCodeDemo other = (EqualsAndHashCodeDemo)o;
            if (!other.canEqual(this)) {
                return false;
            } else {
              // 已省略大量代码
        }
    }
    public int hashCode() {
        int PRIME = true;
        int result = 1;
        Object $firstName = this.firstName;
        int result = result * 59 + ($firstName == null ? 43 : $firstName.hashCode());
        Object $lastName = this.lastName;
        result = result * 59 + ($lastName == null ? 43 : $lastName.hashCode());
        Object $dateOfBirth = this.dateOfBirth;
        result = result * 59 + ($dateOfBirth == null ? 43 : $dateOfBirth.hashCode());
        return result;
    }
}
```
<a name="WdePj"></a>
### 3.4 `@ToString` 
使用 `@ToString`  注解可以为指定类生成 `toString` 方法， `@ToString`  注解的定义如下： 
```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.SOURCE)
public @interface ToString {
    // 打印输出时是否包含字段的名称
    boolean includeFieldNames() default true;

    // 列出打印输出时，需要排除的字段列表
    String[] exclude() default {};

    // 显式的列出需要打印输出的字段列表
    String[] of() default {};

    // 打印输出的结果中是否包含父类的toString方法的返回结果
    boolean callSuper() default false;

    boolean doNotUseGetters() default false;

    boolean onlyExplicitlyIncluded() default false;

    @Target(ElementType.FIELD)
        @Retention(RetentionPolicy.SOURCE)
        public @interface Exclude {}

    @Target({ElementType.FIELD, ElementType.METHOD})
        @Retention(RetentionPolicy.SOURCE)
        public @interface Include {
            int rank() default 0;
            String name() default "";
        }
}
```
示例
```java
@ToString(exclude = {"dateOfBirth"})
public class ToStringDemo {
    String firstName;
    String lastName;
    LocalDate dateOfBirth;
}
```
以上代码经过 Lombok 编译后，会生成如下代码：
```java
public class ToStringDemo {
    String firstName;
    String lastName;
    LocalDate dateOfBirth;
    public ToStringDemo() {
    }
    public String toString() {
        return "ToStringDemo(firstName=" + this.firstName + ", lastName=" +
          this.lastName + ")";
    }
}
```
<a name="TZMC2"></a>
### 3.5 `@Data` 
`@Data` 注解与同时使用以下的注解的效果是一样的： 
```java
@ToString
@Getter
@Setter
@RequiredArgsConstructor
@EqualsAndHashCode
```
`@Data` 注解的定义如下： 
```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.SOURCE)
public @interface Data {
    String staticConstructor() default "";
}
```
示例
```java
@Data
public class DataDemo {
    private Long id;
    private String summary;
    private String description;
}
```
以上代码经过 Lombok 编译后，会生成如下代码：
```java
public class DataDemo {
    private Long id;
    private String summary;
    private String description;
    public DataDemo() {
    }
    // 省略summary和description成员属性的setter和getter方法
    public Long getId() {
        return this.id;
    }

    public void setId(Long id) {
        this.id = id;
    }
    public boolean equals(Object o) {
        if (o == this) {
            return true;
        } else if (!(o instanceof DataDemo)) {
            return false;
        } else {
            DataDemo other = (DataDemo)o;
            if (!other.canEqual(this)) {
                return false;
            } else {
               // 已省略大量代码
            }
        }
    }
    protected boolean canEqual(Object other) {
        return other instanceof DataDemo;
    }
    public int hashCode() {
        int PRIME = true;
        int result = 1;
        Object $id = this.getId();
        int result = result * 59 + ($id == null ? 43 : $id.hashCode());
        Object $summary = this.getSummary();
        result = result * 59 + ($summary == null ? 43 : $summary.hashCode());
        Object $description = this.getDescription();
        result = result * 59 + ($description == null ? 43 : $description.hashCode());
        return result;
    }
    public String toString() {
        return "DataDemo(id=" + this.getId() + ", summary=" + this.getSummary() + ", description=" + this.getDescription() + ")";
    }
}
```
<a name="VvAXx"></a>
### 3.6 `@Log` 
若将 `@Log`  的变体放在类上（适用于日志记录系统的任何一种），之后，将拥有一个静态的 `final log` 字段，然后就可以使用该字段来输出日志。 
```java
@Log
private static final java.util.logging.Logger log = java.util.logging.Logger.getLogger(LogExample.class.getName());
@Log4j
private static final org.apache.log4j.Logger log = org.apache.log4j.Logger.getLogger(LogExample.class);
@Log4j2
private static final org.apache.logging.log4j.Logger log = org.apache.logging.log4j.LogManager.getLogger(LogExample.class);
@Slf4j
private static final org.slf4j.Logger log = org.slf4j.LoggerFactory.getLogger(LogExample.class);
@XSlf4j
private static final org.slf4j.ext.XLogger log = org.slf4j.ext.XLoggerFactory.getXLogger(LogExample.class);
@CommonsLog
private static final org.apache.commons.logging.Log log = org.apache.commons.logging.LogFactory.getLog(LogExample.class);
```
<a name="vDXB4"></a>
### 3.7 `@Synchronized` 
`@Synchronized` 是同步方法修饰符的更安全的变体。与 `synchronized` 一样，该注解只能应用在静态和实例方法上。它的操作类似于 `synchronized` 关键字，但是它锁定在不同的对象上。`synchronized` 关键字应用在实例方法时，锁定的是 this 对象，而应用在静态方法上锁定的是类对象。对于 `@Synchronized`  注解声明的方法来说，它锁定的是 或lock。`@Synchronized`  注解的定义如下： 
```java
@Target(ElementType.METHOD)
@Retention(RetentionPolicy.SOURCE)
public @interface Synchronized {
    // 指定锁定的字段名称
    String value() default "";
}
```
示例
```java
public class SynchronizedDemo {
    private final Object readLock = new Object();
    @Synchronized
    public static void hello() {
        System.out.println("world");
    }
    @Synchronized
    public int answerToLife() {
        return 42;
    }
    @Synchronized("readLock")
    public void foo() {
        System.out.println("bar");
    }
}
```
以上代码经过 Lombok 编译后，会生成如下代码：
```java
public class SynchronizedDemo {
    private static final Object $LOCK = new Object[0];
    private final Object $lock = new Object[0];
    private final Object readLock = new Object();
    public SynchronizedDemo() {
    }
    public static void hello() {
        synchronized($LOCK) {
            System.out.println("world");
        }
    }
    public int answerToLife() {
        synchronized(this.$lock) {
            return 42;
        }
    }
    public void foo() {
        synchronized(this.readLock) {
            System.out.println("bar");
        }
    }
}
```
<a name="Ysd9W"></a>
### 3.8 `@Builder` 
使用 `@Builder`  注解可以为指定类实现建造者模式，该注解可以放在类、构造函数或方法上。`@Builder`  注解的定义如下： 
```java
@Target({TYPE, METHOD, CONSTRUCTOR})
@Retention(SOURCE)
public @interface Builder {
    @Target(FIELD)
    @Retention(SOURCE)
    public @interface Default {}
    // 创建新的builder实例的方法名称
    String builderMethodName() default "builder";
    // 创建Builder注解类对应实例的方法名称
    String buildMethodName() default "build";
    // builder类的名称
    String builderClassName() default "";

    boolean toBuilder() default false;

    AccessLevel access() default lombok.AccessLevel.PUBLIC;

    @Target({FIELD, PARAMETER})
    @Retention(SOURCE)
    public @interface ObtainVia {
        String field() default "";
        String method() default "";
        boolean isStatic() default false;
    }
}
```
示例
```java
@Builder
public class BuilderDemo {
    private final String firstname;
    private final String lastname;
    private final String email;
}
```
以上代码经过 Lombok 编译后，会生成如下代码：
```java
public class BuilderDemo {
    private final String firstname;
    private final String lastname;
    private final String email;
    BuilderDemo(String firstname, String lastname, String email) {
        this.firstname = firstname;
        this.lastname = lastname;
        this.email = email;
    }
    public static BuilderDemo.BuilderDemoBuilder builder() {
        return new BuilderDemo.BuilderDemoBuilder();
    }
    public static class BuilderDemoBuilder {
        private String firstname;
        private String lastname;
        private String email;
        BuilderDemoBuilder() {
        }
        public BuilderDemo.BuilderDemoBuilder firstname(String firstname) {
            this.firstname = firstname;
            return this;
        }
        public BuilderDemo.BuilderDemoBuilder lastname(String lastname) {
            this.lastname = lastname;
            return this;
        }
        public BuilderDemo.BuilderDemoBuilder email(String email) {
            this.email = email;
            return this;
        }
        public BuilderDemo build() {
            return new BuilderDemo(this.firstname, this.lastname, this.email);
        }
        public String toString() {
            return "BuilderDemo.BuilderDemoBuilder(firstname=" + this.firstname + ", lastname=" + this.lastname + ", email=" + this.email + ")";
        }
    }
}
```
<a name="dff6v"></a>
### 3.9 `@SneakyThrows` 
`@SneakyThrows`  注解用于自动抛出已检查的异常，而无需在方法中使用 `throw` 语句显式抛出。`@SneakyThrows`  注解的定义如下： 
```java
@Target({ElementType.METHOD, ElementType.CONSTRUCTOR})
@Retention(RetentionPolicy.SOURCE)
public @interface SneakyThrows {
    // 设置你希望向上抛的异常类
    Class<? extends Throwable>[] value() default java.lang.Throwable.class;
}
```
示例
```java
public class SneakyThrowsDemo {
    @SneakyThrows
    @Override
    protected Object clone() {
        return super.clone();
    }
}
```
以上代码经过 Lombok 编译后，会生成如下代码：
```java
public class SneakyThrowsDemo {
    public SneakyThrowsDemo() {
    }
    protected Object clone() {
        try {
            return super.clone();
        } catch (Throwable var2) {
            throw var2;
        }
    }
}
```
<a name="ylsTO"></a>
### 3.10 `@NonNull` 
可以在方法或构造函数的参数上使用 `@NonNull`  注解，它将会自动生成非空校验语句。`@NonNull`  注解的定义如下： 
```java
@Target({ElementType.FIELD, ElementType.METHOD, ElementType.PARAMETER, ElementType.LOCAL_VARIABLE, ElementType.TYPE_USE})
@Retention(RetentionPolicy.CLASS)
@Documented
public @interface NonNull {
}
```
示例
```java
public class NonNullDemo {
    @Getter
    @Setter
    @NonNull
    private String name;
}
```
以上代码经过 Lombok 编译后，会生成如下代码：
```java
public class NonNullDemo {
    @NonNull
    private String name;
    public NonNullDemo() {
    }
    @NonNull
    public String getName() {
        return this.name;
    }
    public void setName(@NonNull String name) {
        if (name == null) {
            throw new NullPointerException("name is marked non-null but is null");
        } else {
            this.name = name;
        }
    }
}
```
<a name="cdwT5"></a>
### 3.11 `@Clean` 
`@Clean`  注解用于自动管理资源，用在局部变量之前，在当前变量范围内即将执行完毕退出之前会自动清理资源，自动生成 `try-finally` 这样的代码来关闭流。 
```java
@Target(ElementType.LOCAL_VARIABLE)
@Retention(RetentionPolicy.SOURCE)
public @interface Cleanup {
    // 设置用于执行资源清理/回收的方法名称，对应方法不能包含任何参数，默认名称为close。
    String value() default "close";
}
```
示例
```java
public class CleanupDemo {
    public static void main(String[] args) throws IOException {
        @Cleanup InputStream in = new FileInputStream(args[0]);
        @Cleanup OutputStream out = new FileOutputStream(args[1]);
        byte[] b = new byte[10000];
        while (true) {
            int r = in.read(b);
            if (r == -1) break;
            out.write(b, 0, r);
        }
    }
}
```
以上代码经过 Lombok 编译后，会生成如下代码：
```java
public class CleanupDemo {
    public CleanupDemo() {
    }
    public static void main(String[] args) throws IOException {
        FileInputStream in = new FileInputStream(args[0]);
        try {
            FileOutputStream out = new FileOutputStream(args[1]);
            try {
                byte[] b = new byte[10000];
                while(true) {
                    int r = in.read(b);
                    if (r == -1) {
                        return;
                    }
                    out.write(b, 0, r);
                }
            } finally {
                if (Collections.singletonList(out).get(0) != null) {
                    out.close();
                }
            }
        } finally {
            if (Collections.singletonList(in).get(0) != null) {
                in.close();
            }
        }
    }
}
```
<a name="MkIzy"></a>
### 3.11 `@With` 
在类的字段上应用 `@With`  注解之后，将会自动生成一个 `withFieldName(newValue)` 的方法，该方法会基于 `newValue` 调用相应构造函数，创建一个当前类对应的实例。`@With`  注解的定义如下： 
```java
@Target({ElementType.FIELD, ElementType.TYPE})
@Retention(RetentionPolicy.SOURCE)
public @interface With {
    AccessLevel value() default AccessLevel.PUBLIC;
    With.AnyAnnotation[] onMethod() default {};
    With.AnyAnnotation[] onParam() default {};
    @Deprecated
    @Retention(RetentionPolicy.SOURCE)
    @Target({})
    public @interface AnyAnnotation {
    }
}
```
示例
```java
public class WithDemo {
    @With(AccessLevel.PROTECTED)
    @NonNull
    private final String name;
    @With
    private final int age;
    public WithDemo(String name, int age) {
        if (name == null) throw new NullPointerException();
        this.name = name;
        this.age = age;
    }
}
```
以上代码经过 Lombok 编译后，会生成如下代码：
```java
public class WithDemo {
    @NonNull
    private final String name;
    private final int age;
    public WithDemo(String name, int age) {
        if (name == null) {
            throw new NullPointerException();
        } else {
            this.name = name;
            this.age = age;
        }
    }
    protected WithDemo withName(@NonNull String name) {
        if (name == null) {
            throw new NullPointerException("name is marked non-null but is null");
        } else {
            return this.name == name ? this : new WithDemo(name, this.age);
        }
    }
    public WithDemo withAge(int age) {
        return this.age == age ? this : new WithDemo(this.name, age);
    }
}
```
<a name="qqsVJ"></a>
### 3.12 其它特性
<a name="iYq0s"></a>
#### `val`
`val` 用在局部变量前面，相当于将变量声明为 `final`，此外 Lombok 在编译时还会自动进行类型推断。`val` 的使用示例：
```java
public class ValExample {
    public String example() {
        val example = new ArrayList<String>();
        example.add("Hello, World!");
        val foo = example.get(0);
        return foo.toLowerCase();
    }

    public void example2() {
        val map = new HashMap<Integer, String>();
        map.put(0, "zero");
        map.put(5, "five");
        for (val entry : map.entrySet()) {
            System.out.printf("%d: %s\n", entry.getKey(), entry.getValue());
        }
    }
}
```
以上代码等价于：
```java
public class ValExample {
    public String example() {
        final ArrayList<String> example = new ArrayList<String>();
        example.add("Hello, World!");
        final String foo = example.get(0);
        return foo.toLowerCase();
    }

    public void example2() {
        final HashMap<Integer, String> map = new HashMap<Integer, String>();
        map.put(0, "zero");
        map.put(5, "five");
        for (final Map.Entry<Integer, String> entry : map.entrySet()) {
            System.out.printf("%d: %s\n", entry.getKey(), entry.getValue());
        }
    }
}
```
