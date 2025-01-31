JavaRedisJedis
<a name="AsET6"></a>
## 一、摘要
基于 redis 开放的通信协议，大神们纷纷开发了各种语言的 redis 客户端，有 c、c++、java、python、php、nodeJs 等等开发语言的客户端，准确来说其实这些客户端都是基于 redis 命令做了一层封装，然后打包成工具以便大家更佳方便的操作 redis，以 Java 项目为例，使用最广的就是以下三种客户端：

- Jedis
- Lettuce
- Redisson
<a name="AiRB8"></a>
## 二、Jedis
Jedis 是老牌的 Redis 的 Java 客户端，提供了比较全面的 Redis 命令的操作支持，也是目前使用最广泛的客户端。<br />官方网址如下：[https://github.com/redis/jedis](https://github.com/redis/jedis)<br />如何在项目中集成 Jedis 呢？请看下文！
<a name="bf6AI"></a>
### 2.1、基本使用
首先创建一个普通的 Maven 项目，然后添加Jedis依赖包！
```xml
<dependency>
  <groupId>redis.clients</groupId>
  <artifactId>jedis</artifactId>
  <version>3.9.0</version>
</dependency>
```
然后创建一个简单的测试，即可实现连接！
```java
public class JedisMain {

    public static void main(String[] args) {
        // 1.构造一个 Jedis 对象，因为这里使用的默认端口 6379，所以不用配置端口
        Jedis jedis = new Jedis("127.0.0.1", 6379);
        // 2.密码认证
        jedis.auth("111111");
        // 3.测试是否连接成功
        String ping = jedis.ping();
        // 4.返回 pong 表示连接成功
        System.out.println(ping);
    }
}
```
对于 Jedis 而言，一旦连接上了 Redis 服务器，剩下的操作就非常容易了，由于 Jedis 中的 API 和 Redis 的命令高度一致，所以，Jedis 中的方法见名知意，直接使用即可。
<a name="n2SP5"></a>
### 2.2、连接池
虽然 redis 服务端是单线程操作，但是在实际项目中，使用 Jedis 对象来操作 redis 时，每次操作都需要新建/关闭 TCP 连接，连接资源开销很高，同时 Jedis 对象的个数不受限制，在极端情况下可能会造成连接泄漏，同时 Jedis 存在多线程不安全的问题。<br />所以需要将 Jedis 交给线程池来管理，使用 Jedis 对象时，从连接池获取 Jedis，使用完成之后，再还给连接池。<br />在使用之前，需要添加common-pool线程池依赖包！
```xml
<dependency>
  <groupId>org.apache.commons</groupId>
  <artifactId>commons-pool2</artifactId>
  <version>2.11.1</version>
</dependency>
```
创建一个简单的使用线程池测试用例。
```java
public class JedisPoolMain {

    public static void main(String[] args) {
        // 1. 构造一个 Jedis 连接池
        JedisPool pool = new JedisPool("127.0.0.1", 6379);
        // 2. 从连接池中获取一个 Jedis 连接
        Jedis jedis = pool.getResource();
        jedis.auth("111111");
        // 3. Jedis 操作
        String ping = jedis.ping();
        System.out.println(ping);
        // 4. 归还连接
        jedis.close();
    }
}
```
<a name="pqaLj"></a>
### 2.3、连接池配置
在实际的使用过程中，常常会这样来初始化线程池JedisPool，详细代码如下：
```java
public class RedisPoolUtils {

    private static JedisPool jedisPool = null;

    /**
     * redis服务器地址
     */
    private static String addr = "127.0.0.1";

    /**
     * redis服务器端口
     */
    private static int port = 6379;

    /**
     * redis服务器密码
     */
    private static String auth = "111111";


    static{
        try {
            JedisPoolConfig config = new JedisPoolConfig();
            //连接耗尽时是否阻塞, false报异常,ture阻塞直到超时, 默认true
            config.setBlockWhenExhausted(true);
            //设置的逐出策略类名, 默认DefaultEvictionPolicy(当连接超过最大空闲时间,或连接数超过最大空闲连接数)
            config.setEvictionPolicyClassName("org.apache.commons.pool2.impl.DefaultEvictionPolicy");
            //是否启用pool的jmx管理功能, 默认true
            config.setJmxEnabled(true);
            //MBean ObjectName = new ObjectName("org.apache.commons.pool2:type=GenericObjectPool,name=" + "pool" + i); 默认为"pool", JMX不熟,具体不知道是干啥的...默认就好.
            config.setJmxNamePrefix("pool");
            //是否启用后进先出, 默认true
            config.setLifo(true);
            //最大空闲连接数, 默认8个
            config.setMaxIdle(8);
            //最大连接数, 默认8个
            config.setMaxTotal(8);
            //获取连接时的最大等待毫秒数(如果设置为阻塞时BlockWhenExhausted),如果超时就抛异常, 小于零:阻塞不确定的时间,  默认-1
            config.setMaxWaitMillis(-1);
            //逐出连接的最小空闲时间 默认1800000毫秒(30分钟)
            config.setMinEvictableIdleTimeMillis(1800000);
            //最小空闲连接数, 默认0
            config.setMinIdle(0);
            //每次逐出检查时 逐出的最大数目 如果为负数就是 : 1/abs(n), 默认3
            config.setNumTestsPerEvictionRun(3);
            //对象空闲多久后逐出, 当空闲时间>该值 且 空闲连接>最大空闲数 时直接逐出,不再根据MinEvictableIdleTimeMillis判断  (默认逐出策略)
            config.setSoftMinEvictableIdleTimeMillis(1800000);
            //在获取连接的时候检查有效性, 默认false
            config.setTestOnBorrow(false);
            //在空闲时检查有效性, 默认false
            config.setTestWhileIdle(false);
            //逐出扫描的时间间隔(毫秒) 如果为负数,则不运行逐出线程, 默认-1
            config.setTimeBetweenEvictionRunsMillis(-1);
            jedisPool = new JedisPool(config, addr, port, 3000, auth);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 获取 Jedis 资源
     * @return
     */
    public static Jedis getJedis() {
        if (jedisPool != null) {
            return jedisPool.getResource();
        }
        return null;
    }

    /**
     * 释放Jedis资源
     */
    public static void close(final Jedis jedis) {
        if (jedis != null) {
            jedis.close();
        }
    }

}
```
简单测试
```java
public static void main(String[] args) throws InterruptedException {
    //获取 jedis 客户端
    Jedis jedis = RedisPoolUtils.getJedis();

    System.out.println("清空数据："+jedis.flushDB());
    System.out.println("判断某个键是否存在："+jedis.exists("username"));
    System.out.println("新增<'username','xmr'>的键值对："+jedis.set("username", "xmr"));
    System.out.println(jedis.exists("username"));
    System.out.println("新增<'password','password'>的键值对："+jedis.set("password", "123"));
    System.out.print("系统中所有的键如下：");
    Set<String> keys = jedis.keys("*");
    System.out.println(keys);
    System.out.println("删除键password:"+jedis.del("password"));
    System.out.println("判断键password是否存在："+jedis.exists("password"));
    System.out.println("设置键username的过期时间为5s:"+jedis.expire("username", 8L));
    TimeUnit.SECONDS.sleep(1);
    System.out.println("查看键username的剩余生存时间："+jedis.ttl("username"));
    System.out.println("移除键username的生存时间："+jedis.persist("username"));
    System.out.println("查看键username的剩余生存时间："+jedis.ttl("username"));
    System.out.println("查看键username所存储的值的类型："+jedis.type("username"));
    RedisPoolUtils.close(jedis);
}
```
运行结果如下：
```
清空数据：OK
判断某个键是否存在：false
新增<'username','xmr'>的键值对：OK
true
新增<'password','password'>的键值对：OK
系统中所有的键如下：[password, username]
删除键password:1
判断键password是否存在：false
设置键username的过期时间为5s:1
查看键username的剩余生存时间：7
移除键username的生存时间：1
查看键username的剩余生存时间：-1
查看键username所存储的值的类型：string
```
<a name="VN8LQ"></a>
### 2.4、字符串常用 API 操作
```java
public class RedisClientUtil {

    private static final Logger log = LoggerFactory.getLogger(RedisClientUtil.class);


    /**
     * 获取指定key的值,如果key不存在返回null
     * 返回值：返回 key 的值，如果 key 不存在时，返回 nil
     * @param key
     * @return
     */
    public static String get(String key) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.get(key);
        } catch (Exception e){
            log.error("get命令操作失败，请求参数：{}", key,e);
        }
        return null;
    }


    /**
     * 设置key的值为value
     * 返回值：操作成功完成时返回 OK
     * @param key
     * @return
     */
    public static String set(String key, String value) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.set(key, value);
        } catch (Exception e){
            log.error("set命令操作失败，参数key：{}，参数value：{}", key, value,e);
        }
        return null;
    }


    /**
     * 删除指定的key，返回值：被删除 key 的数量
     * 返回值：被删除 key 的数量
     * @param key
     * @return
     */
    public static Long del(String key) {
        try (Jedis jedis = jedisPool.getResource()) {
            Long result = jedis.del(key);
            return jedis.del(key);
        } catch (Exception e){
            log.error("del命令操作失败，参数key：{}", key,e);
        }
        return 0L;
    }


    /**
     * 通过key向指定的value值追加值
     * 返回值：追加指定值之后， key中字符串的长度
     * @param key
     * @return
     */
    public static Long append(String key, String value) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.append(key, value);
        } catch (Exception e){
            log.error("append命令操作失败，参数key：{}，参数value：{}", key, value,e);
        }
        return 0L;
    }

    /**
     * 判断key是否存在
     * 返回值：true/false
     * @param key
     * @return
     */
    public static Boolean exists(String key) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.exists(key);
        } catch (Exception e){
            log.error("exists命令操作失败，参数key：{}", key,e);
        }
        return false;
    }


    /**
     * 设置key的超时时间为seconds
     * 返回值：若 key 存在返回 1 ，否则返回 0
     * @param key
     * @return
     */
    public static Long expire(String key, long seconds) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.expire(key, seconds);
        } catch (Exception e){
            log.error("expire命令操作失败，参数key：{}，参数seconds：{}", key, seconds,e);
        }
        return 0L;
    }

    /**
     * 返回 key 的剩余过期时间（单位秒）
     * 返回值：当 key 不存在时，返回 -2 。 当 key 存在但没有设置剩余生存时间时，返回 -1 。 否则，以秒为单位，返回 key 的剩余生存时间
     * @param key
     * @return
     */
    public static Long ttl(String key) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.ttl(key);
        } catch (Exception e){
            log.error("ttl命令操作失败，参数key：{}", key,e);
        }
        return 0L;
    }


    /**
     * 设置指定key的值为value,当key不存在时才设置
     * 返回值：设置成功返回 1，设置失败返回 0
     * @param key
     * @return
     */
    public static Long setnx(String key, String value) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.setnx(key, value);
        } catch (Exception e){
            log.error("setnx命令操作失败，参数key：{}，参数value：{}", key, value,e);
        }
        return 0L;
    }

    /**
     * 设置指定key的值为value,并设置过期时间
     * 返回值：设置成功时返回 OK
     * @param key
     * @return
     */
    public static String setex(String key, String value, long seconds) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.setex(key, seconds, value);
        } catch (Exception e){
            log.error("setex命令操作失败，参数key：{}，参数value：{}", key, value,e);
        }
        return null;
    }

    /**
     * 通过key 和offset 从指定的位置开始将原先value替换
     * 返回值：被修改后的字符串长度
     * @param key
     * @return
     */
    public static Long setrange(String key, int offset, String value) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.setrange(key, offset, value);
        } catch (Exception e){
            log.error("setrange命令操作失败，参数key：{}，参数value：{}，参数offset：{}", key, value, offset,e);
        }
        return null;
    }


    /**
     * 通过批量的key获取批量的value
     * 返回值：一个包含所有给定 key 的值的列表。
     * @param keys
     * @return
     */
    public static List<String> mget(String... keys) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.mget(keys);
        } catch (Exception e){
            log.error("mget命令操作失败，参数key：{}", keys.toString(),e);
        }
        return null;
    }

    /**
     * 批量的设置key:value,也可以一个
     * 返回值：总是返回 OK
     * @param keysValues
     * @return
     */
    public static String mset(String... keysValues) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.mset(keysValues);
        } catch (Exception e){
            log.error("mset命令操作失败，参数key：{}", keysValues.toString(),e);
        }
        return null;
    }


    /**
     * 设置key的值,并返回一个旧值
     * 返回值：返回给定 key 的旧值，当 key 没有旧值时，即 key 不存在时，返回 nil
     * @param key
     * @return
     */
    public static String getSet(String key, String value) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.getSet(key, value);
        } catch (Exception e){
            log.error("getSet命令操作失败，参数key：{}，参数value：{}", key, value,e);
        }
        return null;
    }

    /**
     * 通过下标和 key 获取指定下标位置的 value
     * 返回值：截取得到的子字符串
     * @param key
     * @return
     */
    public static String getrange(String key, int startOffset, int endOffset) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.getrange(key, startOffset, endOffset);
        } catch (Exception e){
            log.error("getrange命令操作失败，参数key：{}，参数startOffset：{}，参数offset：{}", key, startOffset, endOffset,e);
        }
        return null;
    }


    /**
     * 通过key 对value进行加值+1操作,当value不是int类型时会返回错误,当key不存在是则value为1
     * 返回值：执行INCR命令之后 key 的值
     * @param key
     * @return
     */
    public static Long incr(String key) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.incr(key);
        } catch (Exception e){
            log.error("incr命令操作失败，参数key：{}", key, e);
        }
        return 0L;
    }


    /**
     * 通过key给指定的value加值
     * 返回值：执行INCR命令之后 key 的值
     * @param key
     * @return
     */
    public static Long incrBy(String key, long increment) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.incrBy(key, increment);
        } catch (Exception e){
            log.error("incrBy命令操作失败，参数key：{}，参数increment：{}", key, increment,e);
        }
        return 0L;
    }

    /**
     * 对key的值做减减操作
     * 返回值：执行INCR命令之后 key 的值
     * @param key
     * @return
     */
    public static Long decr(String key) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.decr(key);
        } catch (Exception e){
            log.error("decr命令操作失败，参数key：{}", key, e);
        }
        return 0L;
    }

    /**
     * 对key的值做减减操作,减去指定的值
     * 返回值：执行INCR命令之后 key 的值
     * @param key
     * @return
     */
    public static Long decrBy(String key, long decrement) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.decrBy(key, decrement);
        } catch (Exception e){
            log.error("decrBy命令操作失败，参数key：{}，参数decrement：{}", key, decrement,e);
        }
        return 0L;
    }


    /**
     * 通过key获取value值的长度
     * 返回值：value值的长度
     * @param key
     * @return
     */
    public static Long strlen(String key) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.strlen(key);
        } catch (Exception e){
            log.error("strlen命令操作失败，参数key：{}", key, e);
        }
        return 0L;
    }
}
```
<a name="MrkjU"></a>
### 2.5、哈希常用 API 操作
```java
public class RedisClientUtil {

    private static final Logger log = LoggerFactory.getLogger(RedisClientUtil.class);


    /**
     * 通过key 和 field 获取指定的 value
     * 返回值：对应的value值
     * @param key
     * @return
     */
    public static String hget(String key, String field) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.hget(key, field);
        } catch (Exception e){
            log.error("hget命令操作失败，参数key：{}，参数field：{}", key, field,e);
        }
        return null;
    }

    /**
     * 通过key给field设置指定的值,如果key不存在,则先创建
     * 返回值：如果字段是哈希表中的一个新建字段，并且值设置成功，返回 1 ;如果哈希表中域字段已经存在且旧值已被新值覆盖，返回 0 。
     * @param key
     * @return
     */
    public static Long hset(String key, String field, String value) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.hset(key, field, value);
        } catch (Exception e){
            log.error("hset命令操作失败，参数key：{}，参数field：{}，参数value：{}", key, field, value,e);
        }
        return 0L;
    }


    /**
     * 通过key和field判断是否有指定的value存在
     * 返回值：true/false
     * @param key
     * @return
     */
    public static Boolean hexists(String key, String field) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.hexists(key, field);
        } catch (Exception e){
            log.error("hexists命令操作失败，参数key：{}，参数field：{}", key, field,e);
        }
        return false;
    }


    /**
     * 通过key返回field的数量
     * 返回值：field的数量
     * @param key
     * @return
     */
    public static Long hlen(String key) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.hlen(key);
        } catch (Exception e){
            log.error("hlen命令操作失败，参数key：{}", key,e);
        }
        return 0L;
    }


    /**
     * 通过key 删除指定的 field
     * 返回值：删除的数量
     * @param key
     * @return
     */
    public static Long hdel(String key, String... fields) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.hdel(key, fields);
        } catch (Exception e){
            log.error("hdel命令操作失败，参数key：{}，参数fields：{}", key, fields.toString(),e);
        }
        return 0L;
    }


    /**
     * 通过key返回所有的field
     * 返回值：field集合
     * @param key
     * @return
     */
    public static Set<String> hkeys(String key) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.hkeys(key);
        } catch (Exception e){
            log.error("hkeys命令操作失败，参数key：{}", key,e);
        }
        return null;
    }


    /**
     * 通过key获取所有的field和value
     * 返回值：map对象
     * @param key
     * @return
     */
    public static Map<String, String> hgetAll(String key) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.hgetAll(key);
        } catch (Exception e){
            log.error("hgetAll命令操作失败，参数key：{}", key,e);
        }
        return null;
    }
}
```
<a name="tUmm5"></a>
### 2.6、列表常用 API 操作
```java
public class RedisClientUtil {

    private static final Logger log = LoggerFactory.getLogger(RedisClientUtil.class);


    /**
     * 过key向list头部添加字符串
     * 返回值：执行 LPUSH 命令后，列表的长度
     * @param key
     * @return
     */
    public static Long lpush(String key, String... strs) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.lpush(key, strs);
        } catch (Exception e){
            log.error("lpush命令操作失败，参数key：{}，参数strs：{}", key, strs.toString(),e);
        }
        return null;
    }


    /**
     * 通过key向list尾部添加字符串
     * 返回值：执行 RPUSH 命令后，列表的长度
     * @param key
     * @return
     */
    public static Long rpush(String key, String... strs) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.rpush(key, strs);
        } catch (Exception e){
            log.error("rpush命令操作失败，参数key：{}，参数strs：{}", key, strs.toString(),e);
        }
        return null;
    }

    /**
     * 通过key设置list指定下标位置的value 如果下标超过list里面value的个数则报错
     * 返回值：操作成功返回 ok ，否则返回错误信息
     * @param key
     * @return
     */
    public static String lset(String key, Long index, String value) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.lset(key, index, value);
        } catch (Exception e){
            log.error("lset命令操作失败，参数key：{}，参数index：{}，参数value：{}", key, index, value,e);
        }
        return null;
    }


    /**
     * 通过key从对应的list中删除指定的count个 和 value相同的元素
     * 返回值：返回被删除的个数
     * @param key
     * @return
     */
    public static Long lrem(String key, long count, String value) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.lrem(key, count, value);
        } catch (Exception e){
            log.error("lrem命令操作失败，参数key：{}，参数count：{}，参数value：{}", key, count, value,e);
        }
        return null;
    }



    /**
     * 通过key保留list中从strat下标开始到end下标结束的value值
     * 返回值：操作成功返回 ok ，否则返回错误信息
     * @param key
     * @return
     */
    public static String ltrim(String key, long start, long end) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.ltrim(key, start, end);
        } catch (Exception e){
            log.error("ltrim命令操作失败，参数key：{}，参数start：{}，参数end：{}", key, start, end,e);
        }
        return null;
    }


    /**
     * 通过key从list的头部删除一个value,并返回该value
     * 返回值：value值
     * @param key
     * @return
     */
    public static String lpop(String key) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.lpop(key);
        } catch (Exception e){
            log.error("lpop命令操作失败，参数key：{}", key,e);
        }
        return null;
    }

    /**
     * 通过key从list尾部删除一个value,并返回该元素
     * 返回值：value值
     * @param key
     * @return
     */
    public static String rpop(String key) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.rpop(key);
        } catch (Exception e){
            log.error("rpop命令操作失败，参数key：{}", key,e);
        }
        return null;
    }


    /**
     * 通过key获取list中指定下标位置的value
     * 返回值：value值
     * @param key
     * @return
     */
    public static String lindex(String key, long index){
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.lindex(key, index);
        } catch (Exception e){
            log.error("lindex命令操作失败，参数key：{}，参数index：{}", key, index,e);
        }
        return null;
    }


    /**
     * 通过key返回list的长度
     * 返回值：value值
     * @param key
     * @return
     */
    public static Long llen(String key) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.llen(key);
        } catch (Exception e){
            log.error("llen命令操作失败，参数key：{}", key,e);
        }
        return null;
    }


    /**
     * 通过key获取list指定下标位置的value 如果start 为 0 end 为 -1 则返回全部的list中的value
     * 返回值：value值
     * @param key
     * @return
     */
    public static List<String> lrange(String key, long start, long end) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.lrange(key, start, end);
        } catch (Exception e){
            log.error("lrange命令操作失败，参数key：{}，参数start：{}，参数end：{}", key, start, end,e);
        }
        return null;
    }

}
```
<a name="aTLzq"></a>
### 2.7、集合常用 API 操作
```java
public class RedisClientUtil {

    private static final Logger log = LoggerFactory.getLogger(RedisClientUtil.class);


    /**
     * 通过key向指定的set中添加value
     * 返回值：添加成功的个数
     * @param key
     * @return
     */
    public static Long sadd(String key, String... members)  {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.sadd(key, members);
        } catch (Exception e){
            log.error("sadd命令操作失败，参数key：{}，参数members：{}", key, members.toString(),e);
        }
        return null;
    }

    /**
     * 通过key删除set中对应的value值
     * 返回值：删除成功的个数
     * @param key
     * @return
     */
    public static Long srem(String key, String... members)  {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.srem(key, members);
        } catch (Exception e){
            log.error("srem命令操作失败，参数key：{}，参数members：{}", key, members.toString(),e);
        }
        return null;
    }

    /**
     * 通过key获取set中value的个数
     * 返回值：value的个数
     * @param key
     * @return
     */
    public static Long scard(String key) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.scard(key);
        } catch (Exception e){
            log.error("scard命令操作失败，参数key：{}", key,e);
        }
        return 0L;
    }


    /**
     * 通过key判断value是否是set中的元素
     * 返回值：true/false
     * @param key
     * @return
     */
    public static Boolean sismember(String key, String member) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.sismember(key, member);
        } catch (Exception e){
            log.error("sismember命令操作失败，参数key：{}，参数member：{}", key, member,e);
        }
        return false;
    }


    /**
     * 通过key获取set中所有的value
     * 返回值：所有的value
     * @param key
     * @return
     */
    public static Set<String> smembers(String key) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.smembers(key);
        } catch (Exception e){
            log.error("smembers命令操作失败，参数key：{}", key,e);
        }
        return null;
    }

}
```
<a name="HgmDW"></a>
### 2.8、有序集合常用 API 操作
```java
public class RedisClientUtil {

    private static final Logger log = LoggerFactory.getLogger(RedisClientUtil.class);

    /**
     * 通过key向zset中添加value,score,其中score就是用来排序的 如果该value已经存在则根据score更新元素
     * 返回值：被成功添加的新成员的数量，不包括那些被更新的、已经存在的成员
     * @param key
     * @return
     */
    public static Long zadd(String key, double score, String member) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.zadd(key, score, member);
        } catch (Exception e){
            log.error("zadd命令操作失败，参数key：{}，参数score：{}，参数member：{}", key, score, member,e);
        }
        return null;
    }


    /**
     * 通过key删除在zset中指定的value
     * 返回值：删除个数
     * @param key
     * @return
     */
    public static Long zrem(String key, String... members) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.zrem(key, members);
        } catch (Exception e){
            log.error("zrem命令操作失败，参数key：{}，参数members：{}", key, members.toString(),e);
        }
        return null;
    }


    /**
     * 通过key增加该zset中value的score的值
     * 返回值：member 成员的新分数值
     * @param key
     * @return
     */
    public static Double zincrby(String key, double score, String member) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.zincrby(key, score, member);
        } catch (Exception e){
            log.error("zincrby命令操作失败，参数key：{}，参数score：{}，参数member：{}", key, score, member,e);
        }
        return null;
    }

    /**
     * 通过key返回zset中value的排名 下标从小到大排序
     * 返回值：返回 member 的排名
     * @param key
     * @return
     */
    public static Long zrank(String key, String member) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.zrank(key, member);
        } catch (Exception e){
            log.error("zrank命令操作失败，参数key：{}，参数member：{}", key, member,e);
        }
        return null;
    }


    /**
     * 通过key将获取score从start到end中zset的value socre从大到小排序 当start为0 end为-1时返回全部
     * 返回值：返回 member 集合
     * @param key
     * @return
     */
    public static Set<String> zrevrange(String key, long start, long end) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.zrevrange(key, start, end);
        } catch (Exception e){
            log.error("zrevrange命令操作失败，参数key：{}，参数start：{}，参数end：{}", key, start, end,e);
        }
        return null;
    }

    /**
     * 返回指定区间内zset中value的数量
     * 返回值：返回 member 集合
     * @param key
     * @return
     */
    public static Long zcount(String key, String min, String max)  {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.zcount(key, min, max);
        } catch (Exception e){
            log.error("zcount命令操作失败，参数key：{}，参数min：{}，参数max：{}", key, min, max,e);
        }
        return null;
    }


    /**
     * 通过key返回zset中的value个数
     * 返回值：返回 member 集合
     * @param key
     * @return
     */
    public static Long zcard(String key)  {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.zcard(key);
        } catch (Exception e){
            log.error("zcard命令操作失败，参数key：{}", key,e);
        }
        return null;
    }


    /**
     * 返回满足pattern表达式的所有key keys(*) 返回所有的key
     * 返回值：返回 key 集合
     * @param pattern
     * @return
     */
    public static Set<String> keys(String pattern) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.keys(pattern);
        } catch (Exception e){
            log.error("keys命令操作失败，参数pattern：{}", pattern,e);
        }
        return null;
    }

    /**
     * 通过key判断值得类型
     * 返回值：值的类型
     * @param key
     * @return
     */
    public static String type(String key) {
        try (Jedis jedis = jedisPool.getResource()) {
            return jedis.type(key);
        } catch (Exception e){
            log.error("type命令操作失败，参数key：{}", key,e);
        }
        return null;
    }

}
```
<a name="OL2pa"></a>
## 三、集群配置
在实际的项目生产环境中，redis 通常不是以单台服务实例来运行的，因为一旦服务器挂了，可能所有的下游服务都会受到影响，因此为了保障单台服务器即使出现故障也能运行，通常运维组会搭建集群环境，来保证服务高可用。<br />搭建的方式有两种，哨兵模式和 Cluster 模式。

- 哨兵模式：对redis服务器进行监控，如果有宕机的，就从备机里面选一个出来作为主机，实现自动切换
- Cluster 模式：将数据进行分片存储，避免全部节点数据都一样，浪费空间
<a name="FEKwD"></a>
### 3.1、哨兵模式
哨兵模式简单的说，就是一台主机，一台备机，外加一台监控服务，当监控服务观测到主机已经宕机，就会将备用机切换成主机，以便继续提供服务。
```java
public class RedisPoolUtils {

    private static Jedis jedis;

    private static JedisSentinelPool jedisSentinelPool;

    static{
        try {
            JedisPoolConfig config = new JedisPoolConfig();
            //最大空闲连接数, 默认8个
            config.setMaxIdle(8);
            //最大连接数, 默认8个
            config.setMaxTotal(8);
            //最小空闲连接数, 默认0
            config.setMinIdle(0);
            //获取连接时的最大等待毫秒数(如果设置为阻塞时BlockWhenExhausted),如果超时就抛异常, 小于零:阻塞不确定的时间,  默认-1
            config.setMaxWaitMillis(3000);
            //在获取连接的时候检查有效性,表示取出的redis对象可用, 默认false
            config.setTestOnBorrow(true);


            //redis服务器列表
            Set<String> sentinels = new HashSet<>();
            sentinels.add(new HostAndPort("192.168.43.212", 26379).toString());
            sentinels.add(new HostAndPort("192.168.43.213", 26379).toString());
            sentinels.add(new HostAndPort("192.168.43.214", 26379).toString());

            //初始化连接池
            jedisSentinelPool = new JedisSentinelPool("mymaster", sentinels, config, "111111");
            // 从池中获取一个Jedis对象
            jedis = jedisSentinelPool.getResource();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
}
```
<a name="PH4XL"></a>
### 3.2、集群模式
为了保证高可用，redis-cluster集群通常会引入主从复制模型，一个主节点对应一个或者多个从节点，当主节点宕机的时候，就会启用从节点。
```java
public class RedisPoolUtils {

    static{
        try {
            JedisPoolConfig config = new JedisPoolConfig();
            //最大空闲连接数, 默认8个
            config.setMaxIdle(8);
            //最大连接数, 默认8个
            config.setMaxTotal(8);
            //最小空闲连接数, 默认0
            config.setMinIdle(0);
            //获取连接时的最大等待毫秒数(如果设置为阻塞时BlockWhenExhausted),如果超时就抛异常, 小于零:阻塞不确定的时间,  默认-1
            config.setMaxWaitMillis(3000);
            //在获取连接的时候检查有效性,表示取出的redis对象可用, 默认false
            config.setTestOnBorrow(true);

            Set<HostAndPort> nodes = new HashSet<>();
            nodes.add(new HostAndPort("192.168.43.212", 26379));
            nodes.add(new HostAndPort("192.168.43.213", 26379));
            nodes.add(new HostAndPort("192.168.43.214", 26379));

            JedisCluster jedisCluster = new JedisCluster(nodes, config);
            jedisCluster.set("key", "hello world");

            jedisCluster.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
<a name="rtUjc"></a>
## 四、小结
jedis客户端是目前使用最广泛的一款 java 客户端，也是老牌的 Redis 的 Java 实现客户端。<br />优点很突出：

- 比较全面的提供了 Redis 的操作特性，也就是说能用 redis 命令操作的，Jedis 包都也都封装好了，直接使用即可
- 使用广泛，易上手

当然，缺点也有：

- Jedis 客户端实例不是线程安全的，需要借助连接池来管理和使用 Jedis
- 使用阻塞的I/O，且其方法调用都是同步的，程序流需要等到 sockets 处理完 I/O 才能执行，不支持异步
