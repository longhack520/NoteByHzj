Java
<a name="Wg2vY"></a>
## 一、MyBatis 不要为了多个查询条件而写 1 = 1
当遇到多个查询条件，使用where 1=1 可以很方便的解决问题，但是这样很可能会造成非常大的性能损失，因为添加了 “where 1=1 ”的过滤条件之后，数据库系统就无法使用索引等查询优化策略，数据库系统将会被迫对每行数据进行扫描（即全表扫描） 以比较此行是否满足过滤条件，当表中的数据量较大时查询速度会非常慢；此外，还会存在SQL 注入的风险。<br />反例：
```xml
<select id="queryBookInfo" parameterType="com.tjt.platform.entity.BookInfo" resultType="java.lang.Integer">
  select count(*) from t_rule_BookInfo t where 1=1
  <if test="title !=null and title !='' ">
    AND title = #{title}
  </if>
  <if test="author !=null and author !='' ">
    AND author = #{author}
  </if>
</select>
```
正例：
```xml
<select id="queryBookInfo" parameterType="com.tjt.platform.entity.BookInfo" resultType="java.lang.Integer">
  select count(*) from t_rule_BookInfo t
  <where>
    <if test="title !=null and title !='' ">
      title = #{title}
    </if>
    <if test="author !=null and author !='' ">
      AND author = #{author}
    </if>
  </where>
</select>
```
UPDATE 操作也一样，可以用标记代替 1=1。
<a name="f8311c74"></a>
## 二、迭代`entrySet()` 获取Map 的key 和value
当循环中只需要获取Map 的主键key时，迭代`keySet()` 是正确的；但是，当需要主键key 和取值value 时，迭代`entrySet()` 才是更高效的做法，其比先迭代`keySet()` 后再去通过get 取值性能更佳。<br />反例：
```java
//Map 获取value 反例:
HashMap<String, String> map = new HashMap<>();
for (String key : map.keySet()){
    String value = map.get(key);
}
```
正例：
```java
//Map 获取key & value 正例:
HashMap<String, String> map = new HashMap<>();
for (Map.Entry<String,String> entry : map.entrySet()){
    String key = entry.getKey();
    String value = entry.getValue();
}
```
<a name="fUZ8W"></a>
## 三、使用`Collection.isEmpty()` 检测空
使用`Collection.size()` 来检测是否为空在逻辑上没有问题，但是使用`Collection.isEmpty()` 使得代码更易读，并且可以获得更好的性能；除此之外，任何`Collection.isEmpty()` 实现的时间复杂度都是O(1) ，不需要多次循环遍历，但是某些通过`Collection.size()` 方法实现的时间复杂度可能是O(n)<br />反例：
```java
LinkedList<Object> collection = new LinkedList<>();
if (collection.size() == 0){
    System.out.println("collection is empty.");
}
```
正例：
```java
LinkedList<Object> collection = new LinkedList<>();
if (collection.isEmpty()){
    System.out.println("collection is empty.");
}
//检测是否为null 可以使用CollectionUtils.isEmpty()
if (CollectionUtils.isEmpty(collection)){
    System.out.println("collection is null.");
}
```
<a name="ifvqn"></a>
## 四、初始化集合时尽量指定其大小
尽量在初始化时指定集合的大小，能有效减少集合的扩容次数，因为集合每次扩容的时间复杂度很可能时O(n)，耗费时间和性能。<br />反例：
```java
//初始化list，往list 中添加元素反例：
int[] arr = new int[]{1,2,3,4};
List<Integer> list = new ArrayList<>();
for (int i : arr){
    list.add(i);
}
```
正例：
```java
//初始化list，往list 中添加元素正例：
int[] arr = new int[]{1,2,3,4};
//指定集合list 的容量大小
List<Integer> list = new ArrayList<>(arr.length);
for (int i : arr){
    list.add(i);
}
```
<a name="4UYdi"></a>
## 五、使用StringBuilder 拼接字符串
一般的字符串拼接在编译期Java 会对其进行优化，但是在循环中字符串的拼接Java 编译期无法执行优化，所以需要使用StringBuilder 进行替换。<br />反例：
```java
//在循环中拼接字符串反例
String str = "";
for (int i = 0; i < 10; i++){
    //在循环中字符串拼接Java 不会对其进行优化
    str += i;
}
```
正例：
```java
//在循环中拼接字符串正例
String str1 = "Love";
String str2 = "Courage";
String strConcat = str1 + str2;  //Java 编译器会对该普通模式的字符串拼接进行优化
StringBuilder sb = new StringBuilder();
for (int i = 0; i < 10; i++){
   //在循环中，Java 编译器无法进行优化，所以要手动使用StringBuilder
    sb.append(i);
}
```
<a name="lvGXd"></a>
## 六、若需频繁调用`Collection.contains()` 方法则使用Set
在Java 集合类库中，List的`contains` 方法普遍时间复杂度为O(n)，若代码中需要频繁调用`contains` 方法查找数据则先将集合list 转换成`HashSet` 实现，将O(n) 的时间复杂度将为O(1)。<br />反例：
```java
//频繁调用Collection.contains() 反例
List<Object> list = new ArrayList<>();
for (int i = 0; i <= Integer.MAX_VALUE; i++){
    //时间复杂度为O(n)
    if (list.contains(i))
    System.out.println("list contains "+ i);
}
```
正例:
```java
//频繁调用Collection.contains() 正例
List<Object> list = new ArrayList<>();
Set<Object> set = new HashSet<>();
for (int i = 0; i <= Integer.MAX_VALUE; i++){
    //时间复杂度为O(1)
    if (set.contains(i)){
        System.out.println("list contains "+ i);
    }
}
```
<a name="e081d413"></a>
## 七、使用静态代码块实现赋值静态成员变量
对于集合类型的静态成员变量，应该使用静态代码块赋值，而不是使用集合实现来赋值。<br />反例：
```java
//赋值静态成员变量反例
private static Map<String, Integer> map = new HashMap<String, Integer>(){
    {
        map.put("Leo",1);
        map.put("Family-loving",2);
        map.put("Cold on the out side passionate on the inside",3);
    }
};
private static List<String> list = new ArrayList<>(){
    {
        list.add("Sagittarius");
        list.add("Charming");
        list.add("Perfectionist");
    }
};
```
正例：
```java
//赋值静态成员变量正例
private static Map<String, Integer> map = new HashMap<String, Integer>();
static {
    map.put("Leo",1);
    map.put("Family-loving",2);
    map.put("Cold on the out side passionate on the inside",3);
}
private static List<String> list = new ArrayList<>();
static {
    list.add("Sagittarius");
    list.add("Charming");
    list.add("Perfectionist");
}
```
<a name="o5AAH"></a>
## 八、删除未使用的局部变量、方法参数、私有方法、字段和多余的括号。
<a name="roR8j"></a>
## 九、工具类中屏蔽构造函数
工具类是一堆静态字段和函数的集合，其不应该被实例化；但是，Java 为每个没有明确定义构造函数的类添加了一个隐式公有构造函数，为了避免不必要的实例化，应该显式定义私有构造函数来屏蔽这个隐式公有构造函数。<br />反例：
```java
public class PasswordUtils {
    //工具类构造函数反例
    private static final Logger LOG = LoggerFactory.getLogger(PasswordUtils.class);
    public static final String DEFAULT_CRYPT_ALGO = "PBEWithMD5AndDES";
    public static String encryptPassword(String aPassword) throws IOException {
        return new PasswordUtils(aPassword).encrypt();
    }
}
```
正例：
```java
public class PasswordUtils {
    //工具类构造函数正例
    private static final Logger LOG = LoggerFactory.getLogger(PasswordUtils.class);
    //定义私有构造函数来屏蔽这个隐式公有构造函数
    private PasswordUtils(){}
    public static final String DEFAULT_CRYPT_ALGO = "PBEWithMD5AndDES";
    public static String encryptPassword(String aPassword) throws IOException {
        return new PasswordUtils(aPassword).encrypt();
    }
}
```
<a name="MXBxT"></a>
## 十、删除多余的异常捕获并抛出
用`catch` 语句捕获异常后，若什么也不进行处理，就只是让异常重新抛出，这跟不捕获异常的效果一样，可以删除这块代码或添加别的处理。<br />反例：
```java
//多余异常反例
private static String fileReader(String fileName)throws IOException{
    try (BufferedReader reader = new BufferedReader(new FileReader(fileName))) {
        String line;
        StringBuilder builder = new StringBuilder();
        while ((line = reader.readLine()) != null) {
            builder.append(line);
        }
        return builder.toString();
    } catch (Exception e) {
        //仅仅是重复抛异常 未作任何处理
        throw e;
    }
}
```
正例：
```java
//多余异常正例
private static String fileReader(String fileName)throws IOException{
    try (BufferedReader reader = new BufferedReader(new FileReader(fileName))) {
        String line;
        StringBuilder builder = new StringBuilder();
        while ((line = reader.readLine()) != null) {
            builder.append(line);
        }
        return builder.toString();
        //删除多余的抛异常，或增加其他处理：
        /*catch (Exception e) {
            return "fileReader exception";
        }*/
    }
}
```
<a name="q2BH8"></a>
## 十一、字符串转化使用`String.valueOf(value)` 代替 `" " + value`
把其它对象或类型转化为字符串时，使用`String.valueOf(value)` 比 `""+value` 的效率更高。<br />反例：
```java
//把其它对象或类型转化为字符串反例：
int num = 520;
// "" + value
String strLove = "" + num;
```
正例：
```java
//把其它对象或类型转化为字符串正例：
int num = 520;
// String.valueOf() 效率更高
String strLove = String.valueOf(num);
```
<a name="aZgSo"></a>
## 十二、避免使用`BigDecimal(double)`
`BigDecimal(double)` 存在精度损失风险，在精确计算或值比较的场景中可能会导致业务逻辑异常。<br />反例：
```java
// BigDecimal 反例
BigDecimal bigDecimal = new BigDecimal(0.11D);
```
正例：
```java
// BigDecimal 正例
BigDecimal bigDecimal1 = bigDecimal.valueOf(0.11D);
```
<a name="Jf3bl"></a>
## 十三、返回空数组和集合而非 null
若程序运行返回null，需要调用方强制检测null，否则就会抛出空指针异常；返回空数组或空集合，有效地避免了调用方因为未检测null 而抛出空指针异常的情况，还可以删除调用方检测null 的语句使代码更简洁。<br />反例：
```java
//返回null 反例
public static Result[] getResults() {
    return null;
}
public static List<Result> getResultList() {
    return null;
}
public static Map<String, Result> getResultMap() {
    return null;
}
```
正例：
```java
//返回空数组和空集正例
public static Result[] getResults() {
    return new Result[0];
}
public static List<Result> getResultList() {
    return Collections.emptyList();
}
public static Map<String, Result> getResultMap() {
    return Collections.emptyMap();
}
```
<a name="PogQq"></a>
## 十四、优先使用常量或确定值调用`equals` 方法
对象的`equals` 方法容易抛空指针异常，应使用常量或确定有值的对象来调用`equals` 方法。<br />反例：
```java
//调用 equals 方法反例
private static boolean fileReader(String fileName)throws IOException{
    // 可能抛空指针异常
    return fileName.equals("Charming");
}
```
正例：
```java
//调用 equals 方法正例
private static boolean fileReader(String fileName)throws IOException{
    // 使用常量或确定有值的对象来调用 equals 方法
    return "Charming".equals(fileName);
    //或使用：java.util.Objects.equals() 方法
    return Objects.equals("Charming",fileName);
}
```
<a name="IQba3"></a>
## 十五、枚举的属性字段必须是私有且不可变
枚举通常被当做常量使用，如果枚举中存在公共属性字段或设置字段方法，那么这些枚举常量的属性很容易被修改；理想情况下，枚举中的属性字段是私有的，并在私有构造函数中赋值，没有对应的Setter 方法，最好加上final 修饰符。<br />反例：
```java
public enum SwitchStatus {
    // 枚举的属性字段反例
    DISABLED(0, "禁用"),
    ENABLED(1, "启用");
    public int value;
    private String description;
    private SwitchStatus(int value, String description) {
        this.value = value;
        this.description = description;
    }
    public String getDescription() {
        return description;
    }
    public void setDescription(String description) {
        this.description = description;
    }
}
```
正例：
```java
public enum SwitchStatus {
    // 枚举的属性字段正例
    DISABLED(0, "禁用"),
    ENABLED(1, "启用");
    // final 修饰
    private final int value;
    private final String description;
    private SwitchStatus(int value, String description) {
        this.value = value;
        this.description = description;
    }
    // 没有Setter 方法
    public int getValue() {
        return value;
    }
    public String getDescription() {
        return description;
    }
}
```
<a name="kgETj"></a>
## 十六、`tring.split(String regex)`部分关键字需要转译
使用字符串String 的`split` 方法时，传入的分隔字符串是正则表达式，则部分关键字（比如 `.|` 等）需要转义。<br />反例：
```java
// String.split(String regex) 反例
String[] split = "a.ab.abc".split(".");
System.out.println(Arrays.toString(split));   // 结果为[]
String[] split1 = "a|ab|abc".split("|");
System.out.println(Arrays.toString(split1));  // 结果为["a", "|", "a", "b", "|", "a", "b", "c"]
```
正例：
```java
// String.split(String regex) 正例
// . 需要转译
String[] split2 = "a.ab.abc".split("\\.");
System.out.println(Arrays.toString(split2));  // 结果为["a", "ab", "abc"]
// | 需要转译
String[] split3 = "a|ab|abc".split("\\|");
System.out.println(Arrays.toString(split3));  // 结果为["a", "ab", "abc"]
```
