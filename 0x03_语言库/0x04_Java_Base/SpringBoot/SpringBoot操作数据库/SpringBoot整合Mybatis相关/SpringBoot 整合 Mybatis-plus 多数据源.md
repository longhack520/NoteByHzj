JavaSpringBootMybatis-plus
<a name="Cfzm0"></a>
### 简介
Mybatis-puls 多数据源的使用，采用的是官方提供的dynamic-datasource-spring-boot-starter包的 `@DS` 注解，具体可以参考官网：[https://gitee.com/baomidou/dynamic-datasource-spring-boot-starter](https://gitee.com/baomidou/dynamic-datasource-spring-boot-starter)
<a name="IkC7U"></a>
### pom.xml文件引入如下依赖
主要引入dynamic-datasource-spring-boot-starter包
```xml
<project xmlns="http://maven.apache.org/POM/4.0.0">http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>com.olive</groupId>
  <artifactId>mybatis-plus-multip-datasource</artifactId>
  <version>0.0.1-SNAPSHOT</version>
  <packaging>jar</packaging>

  <name>mybatis-multip-datasource</name>
  <url>http://maven.apache.org</url>

  <parent>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-parent</artifactId>
    <version>2.5.14</version>
    <relativePath /> <!-- lookup parent from repository -->
    </parent>

  <properties>
    <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
    <maven.compiler.source>8</maven.compiler.source>
    <maven.compiler.target>8</maven.compiler.target>
  </properties>

  <dependencies>
    <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-starter-test</artifactId>
      <scope>test</scope>
    </dependency>


    <dependency>
      <groupId>mysql</groupId>
      <artifactId>mysql-connector-java</artifactId>
    </dependency>

    <dependency>
      <groupId>org.projectlombok</groupId>
      <artifactId>lombok</artifactId>
    </dependency>

    <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-starter-web</artifactId>
    </dependency>

    <dependency>
      <groupId>com.baomidou</groupId>
      <artifactId>dynamic-datasource-spring-boot-starter</artifactId>
      <version>3.5.2</version>
    </dependency>
    <dependency>
      <groupId>com.baomidou</groupId>
      <artifactId>mybatis-plus-boot-starter</artifactId>
      <version>3.5.2</version>
    </dependency>
  </dependencies>
</project>
```
<a name="BLVqc"></a>
### 配置两个数据源
分别为第一个主数据源(primary)，第二数据源(slave_1)，具体配置如下：
```yaml
# 基本配置
server:
  port: 8080

# 数据库
spring:
  datasource:
    dynamic:
      primary: master #设置默认的数据源或者数据源组,默认值即为master
      strict: false #严格匹配数据源,默认false. true未匹配到指定数据源时抛异常,false使用默认数据源
      datasource:
        master:
          url: jdbc:mysql://127.0.0.1:3306/db01?characterEncoding=utf-8&allowMultiQueries=true&autoReconnect=true
          username: root
          password: root
          driver-class-name: com.mysql.cj.jdbc.Driver # 3.2.0开始支持SPI可省略此配置
        slave_1:
          url: jdbc:mysql://127.0.0.1:3306/crm72?characterEncoding=utf-8&allowMultiQueries=true&autoReconnect=true
          username: root
          password: root
          driver-class-name: com.mysql.cj.jdbc.Driver

mybatis-plus:
  mapper-locations: mapper/*.xml
  type-aliases-package: com.olive.entity
  configuration:
    map-underscore-to-camel-case: true
    log-impl: org.apache.ibatis.logging.stdout.StdOutImpl

  jackson:
    serialization:
      indent-output: true
```
<a name="rZkJE"></a>
### 创建学生与老师实体类
学生实体类
```java
package com.olive.entity;

import java.io.Serializable;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

@TableName("t_student")
@Data
public class StudentDO implements Serializable{

    @TableId(value="id", type = IdType.AUTO)
    private Long id;

    @TableField("user_name")
    private String name;

    @TableField("sex")
    private int sex;

    @TableField("grade")
    private String grade;
}
```
老师实体类
```java
package com.olive.entity;

import java.io.Serializable;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

@Data
@TableName("t_teacher")
public class TeacherDO implements Serializable {

    @TableId(value = "id", type = IdType.AUTO)

    private Long id;

    @TableField("user_name")
    private String name;

    @TableField("sex")
    private int sex;

    @TableField("office")
    private String office;
}
```
<a name="tEpQi"></a>
### 数据库持久类
StudentMapper持久类
```java
package com.olive.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.olive.entity.StudentDO;
import org.apache.ibatis.annotations.Mapper;

@Mapper
public interface StudentMapper extends BaseMapper<StudentDO> {

}
```
TeacherMapper持久类
```java
package com.olive.mapper;

import com.baomidou.dynamic.datasource.annotation.DS;
import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.olive.entity.TeacherDO;
import org.apache.ibatis.annotations.Mapper;

@DS("slave_1")
@Mapper
public interface TeacherMapper extends BaseMapper<TeacherDO> {

}
```
以上，两个持久类的实现都是继承`BaseMapper`基类，该类提供了基本的增删改查的方法；要注意的是，TeacherMapper 类使用了`@DS("slave_1")`指定了数据源；而`StudentMapper`没有指定，使用的是默认的数据源。
<a name="JiDW0"></a>
### 服务类的实现
<a name="IrJ4L"></a>
#### 先定义操作学生和老师的两个接口
StudentService接口
```java
package com.olive.service;

import com.baomidou.mybatisplus.extension.service.IService;
import com.olive.entity.StudentDO;

public interface StudentService extends IService<StudentDO> {
}
```
TeacherService接口
```java
package com.olive.service;

import com.baomidou.mybatisplus.extension.service.IService;
import com.olive.entity.TeacherDO;

public interface TeacherService extends IService<TeacherDO> {
}
```
<a name="l6S3P"></a>
#### 再定义接口的实现类
StudentServiceImpl类
```java
package com.olive.service.impl;

import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.olive.entity.StudentDO;
import com.olive.mapper.StudentMapper;
import com.olive.service.StudentService;
import org.springframework.stereotype.Service;

@Service
public class StudentServiceImpl extends ServiceImpl<StudentMapper, StudentDO>
implements StudentService {
}
```
TeacherServiceImpl类
```java
package com.olive.service.impl;

import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.olive.entity.TeacherDO;
import com.olive.mapper.TeacherMapper;
import com.olive.service.TeacherService;
import org.springframework.stereotype.Service;

@Service
public class TeacherServiceImpl extends ServiceImpl<TeacherMapper, TeacherDO>
implements TeacherService {

}
```
观察接口定义和接口实现，套路非常标准。都是实现和继承Mybatis-Plus提供的标准接口和基类
<a name="zVz2a"></a>
### 创建SpringBoot引导类
```java
package com.olive;

import org.mybatis.spring.annotation.MapperScan;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@MapperScan("com.olive.mapper")
@SpringBootApplication
public class Application {

    public static void main(String[] args) {
        SpringApplication.run(Application.class);
    }

}
```
<a name="DjfyE"></a>
### 测试
```java
package com.olive;

import com.olive.service.StudentService;
import com.olive.service.TeacherService;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;

import com.olive.entity.StudentDO;
import com.olive.entity.TeacherDO;

@SpringBootTest
public class MybatisPlusTest {

    @Autowired
    StudentService studentService;

    @Autowired
    TeacherService teacherService;

    @Test
    public void userSave() {
        StudentDO studentDO = new StudentDO();
        studentDO.setName("小明");
        studentDO.setSex(1);
        studentDO.setGrade("一年级");
        studentService.save(studentDO);

        TeacherDO teacherDO = new TeacherDO();
        teacherDO.setName("小黑");
        teacherDO.setSex(2);
        teacherDO.setOffice("语文");
        teacherService.save(teacherDO);
    }
}
```
