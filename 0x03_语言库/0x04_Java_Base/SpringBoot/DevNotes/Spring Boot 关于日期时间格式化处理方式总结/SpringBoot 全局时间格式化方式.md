SpringBoot 时间格式化<br />时间格式化在项目中使用频率是非常高的，当 `API` 接口返回结果，需要对其中某一个 `date` 字段属性进行特殊的格式化处理，通常会用到 `SimpleDateFormat` 工具处理。
```javascript
SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");
Date stationTime = dateFormat.parse(dateFormat.format(PayEndTime()));
```
可一旦处理的地方较多，不仅 `CV` 操作频繁，还产生很多重复臃肿的代码，而此时如果能将时间格式统一配置，就可以省下更多时间专注于业务开发了。<br />很多人觉得统一格式化时间很简单，像下边这样配置一下就可以了，但事实上这种方式只对 `date` 类型生效。
```java
spring.jackson.date-format=yyyy-MM-dd HH:mm:ss
spring.jackson.time-zone=GMT+8
```
而很多项目中用到的时间和日期`API` 比较混乱， `java.util.Date` 、 `java.util.Calendar` 和 `java.time LocalDateTime` 都存在，所以全局时间格式化必须要同时兼容性新旧 `API`。

---

```java
@Data
public class OrderDTO {
    private LocalDateTime createTime;
    private Date updateTime;
}
```
<a name="3RVfQ"></a>
## 一、`@JsonFormat` 注解
`@JsonFormat` 注解方式严格意义上不能叫全局时间格式化，应该叫部分格式化，因为`@JsonFormat` 注解需要用在实体类的时间字段上，而只有使用相应的实体类，对应的字段才能进行格式化。
```java
@Data
public class OrderDTO {
    @JsonFormat(locale = "zh", timezone = "GMT+8", pattern = "yyyy-MM-dd")
    private LocalDateTime createTime;
    @JsonFormat(locale = "zh", timezone = "GMT+8", pattern = "yyyy-MM-dd HH:mm:ss")
    private Date updateTime;
}
```
字段加上 `@JsonFormat` 注解后，`LocalDateTime` 和 `Date` 时间格式化成功。
<a name="0fBmF"></a>
## 二、`@JsonComponent` 注解（推荐）
使用 `@JsonFormat` 注解并不能完全做到全局时间格式化，所以接下来使用 `@JsonComponent` 注解自定义一个全局格式化类，分别对 `Date` 和 `LocalDate` 类型做格式化处理。
```java
@JsonComponent
public class DateFormatConfig {
    @Value("${spring.jackson.date-format:yyyy-MM-dd HH:mm:ss}")
    private String pattern;
    /**
     * @description date 类型全局时间格式化
     * @date 2020/8/31 18:22
     */
    @Bean
    public Jackson2ObjectMapperBuilderCustomizer jackson2ObjectMapperBuilder() {
        return builder -> {
            TimeZone tz = TimeZone.getTimeZone("UTC");
            DateFormat df = new SimpleDateFormat(pattern);
            df.setTimeZone(tz);
            builder.failOnEmptyBeans(false)
                .failOnUnknownProperties(false)
                .featuresToDisable(SerializationFeature.WRITE_DATES_AS_TIMESTAMPS)
                .dateFormat(df);
        };
    }
    /**
     * @description LocalDate 类型全局时间格式化
     * @date 2020/8/31 18:22
     */
    @Bean
    public LocalDateTimeSerializer localDateTimeDeserializer() {
        return new LocalDateTimeSerializer(DateTimeFormatter.ofPattern(pattern));
    }
    @Bean
    public Jackson2ObjectMapperBuilderCustomizer jackson2ObjectMapperBuilderCustomizer() {
        return builder -> builder.serializerByType(LocalDateTime.class, localDateTimeDeserializer());
    }
}
```
`Date` 和 `LocalDate` 两种时间类型格式化成功，此种方式有效。<br />实际开发中如果有个字段不想用全局格式化设置的时间样式，**想自定义格式怎么办？**<br />那就需要和 `@JsonFormat` 注解配合使用了。
```java
@Data
public class OrderDTO {
    @JsonFormat(locale = "zh", timezone = "GMT+8", pattern = "yyyy-MM-dd")
    private LocalDateTime createTime;
    @JsonFormat(locale = "zh", timezone = "GMT+8", pattern = "yyyy-MM-dd")
    private Date updateTime;
}
```
 `@JsonFormat` 注解的优先级比较高，会以 `@JsonFormat` 注解的时间格式为主。
<a name="VZZZc"></a>
## 三、`@Configuration` 注解
这种全局配置的实现方式与上边的效果是一样的。
> **注意**：在使用此种配置后，字段手动配置`@JsonFormat` 注解将不再生效。

```java
@Configuration
public class DateFormatConfig2 {
    @Value("${spring.jackson.date-format:yyyy-MM-dd HH:mm:ss}")
    private String pattern;
    public static DateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    @Bean
    @Primary
    public ObjectMapper serializingObjectMapper() {
        ObjectMapper objectMapper = new ObjectMapper();
        JavaTimeModule javaTimeModule = new JavaTimeModule();
        javaTimeModule.addSerializer(LocalDateTime.class, new LocalDateTimeSerializer());
        javaTimeModule.addDeserializer(LocalDateTime.class, new LocalDateTimeDeserializer());
        objectMapper.registerModule(javaTimeModule);
        return objectMapper;
    }
    /**
     * @author fcant
     * @description Date 时间类型装换
     * @date 2020/9/1 17:25
     */
    @Component
    public class DateSerializer extends JsonSerializer<Date> {
        @Override
        public void serialize(Date date, JsonGenerator gen, SerializerProvider provider) throws IOException {
            String formattedDate = dateFormat.format(date);
            gen.writeString(formattedDate);
        }
    }
    /**
     * @author fcant
     * @description Date 时间类型装换
     * @date 2020/9/1 17:25
     */
    @Component
    public class DateDeserializer extends JsonDeserializer<Date> {
        @Override
        public Date deserialize(JsonParser jsonParser, DeserializationContext deserializationContext) throws IOException {
            try {
                return dateFormat.parse(jsonParser.getValueAsString());
            } catch (ParseException e) {
                throw new RuntimeException("Could not parse date", e);
            }
        }
    }
    /**
     * @author fcant
     * @description LocalDate 时间类型装换
     * @date 2020/9/1 17:25
     */
    public class LocalDateTimeSerializer extends JsonSerializer<LocalDateTime> {
        @Override
        public void serialize(LocalDateTime value, JsonGenerator gen, SerializerProvider serializers) throws IOException {
            gen.writeString(value.format(DateTimeFormatter.ofPattern(pattern)));
        }
    }
    /**
     * @author fcant
     * @description LocalDate 时间类型装换
     * @date 2020/9/1 17:25
     */
    public class LocalDateTimeDeserializer extends JsonDeserializer<LocalDateTime> {
        @Override
        public LocalDateTime deserialize(JsonParser p, DeserializationContext deserializationContext) throws IOException {
            return LocalDateTime.parse(p.getValueAsString(), DateTimeFormatter.ofPattern(pattern));
        }
    }
}
```

