# MySQL Explain详解

日常工作中，会有时会开慢查询去记录一些执行时间比较久的SQL语句，找出这些SQL语句并不意味着完事了，些时我们常常用到explain这个命令来查看一个这些SQL语句的执行计划，查看该SQL语句有没有使用上了索引，有没有做全表扫描，这都可以通过explain命令来查看。所以我们深入了解MySQL的基于开销的优化器，还可以获得很多可能被优化器考虑到的访问策略的细节，以及当运行SQL语句时哪种策略预计会被优化器采用。（QEP：sql生成一个执行计划query Execution plan）

~~~ sql
mysql> explain select * from servers;
+----+-------------+---------+------+---------------+------+---------+------+------+-------+
| id | select_type | table   | type | possible_keys | key  | key_len | ref  | rows | Extra |
+----+-------------+---------+------+---------------+------+---------+------+------+-------+
|  1 | SIMPLE      | servers | ALL  | NULL          | NULL | NULL    | NULL |    1 | NULL  |
+----+-------------+---------+------+---------------+------+---------+------+------+-------+
row in set (0.03 sec)
~~~

expain出来的信息有10列，分别是id、select_type、table、type、possible_keys、key、key_len、ref、rows、Extra,



## 一、 id

 理解是SQL执行顺序标识，SQL从大到小执行：

1. id相同时执行顺序由上至下；

2. 若是子查询id的序号会递增，id值越大优先级越高，越先被执行；

3. id如果相同，可以一组，从上往下顺序执行；所有组中id值越大，优先级越高，越先执行



## 二、select_type

**展示查询中每个select子句的类型**

> (1) SIMPLE(简单SELECT,不使用UNION或子查询等)
>
> (2) PRIMARY(查询中若包含任何复杂的子部分,最外层的select被标记为PRIMARY)
>
> (3) UNION(UNION中的第二个或后面的SELECT语句)
>
> (4) DEPENDENT UNION(UNION中的第二个或后面的SELECT语句，取决于外面的查询)
>
> (5) UNION RESULT(UNION的结果)
>
> (6) SUBQUERY(子查询中的第一个SELECT)
>
> (7) DEPENDENT SUBQUERY(子查询中的第一个SELECT，取决于外面的查询)
>
> (8) DERIVED(派生表的SELECT, FROM子句的子查询)
>
> (9) UNCACHEABLE SUBQUERY(一个子查询的结果不能被缓存，必须重新评估外链接的第一行)
>



## 三、table

显示这一行的数据是关于哪张表的，有时不是真实的表名字,看到的是derivedx(x是个数字,我的理解是第几步执行的结果)

~~~ sql
mysql> explain select * from (select * from ( select * from t1 where id=2602) a) b;
+----+-------------+------------+--------+-------------------+---------+---------+------+------+-------+
| id | select_type | table      | type   | possible_keys     | key     | key_len | ref  | rows | Extra |
+----+-------------+------------+--------+-------------------+---------+---------+------+------+-------+
|  1 | PRIMARY     | <derived2> | system | NULL              | NULL    | NULL    | NULL |    1 |       |
|  2 | DERIVED     | <derived3> | system | NULL              | NULL    | NULL    | NULL |    1 |       |
|  3 | DERIVED     | t1         | const  | PRIMARY,idx_t1_id | PRIMARY | 4       |      |    1 |       |
+----+-------------+------------+--------+-------------------+---------+---------+------+------+-------+
~~~



## 四、type

表示MySQL在表中找到所需行的方式，又称“访问类型”。

常用的类型有： **ALL, index, range, ref, eq_ref, const, system, NULL（从左到右，性能从差到好）**

**Select_type 说明查询中使用到的索引类型，如果没有用有用到索引则为all**

ALL：Full Table Scan， MySQL将遍历全表以找到匹配的行

index: Full Index Scan，index与ALL区别为index类型只遍历索引树

range:只检索给定范围的行，使用一个索引来选择行

ref: 表示上述表的连接匹配条件，即哪些列或常量被用于查找索引列上的值

eq_ref: 类似ref，区别就在使用的索引是唯一索引，对于每个索引键值，表中只有一条记录匹配，简单来说，就是多表连接中使用primary key或者 unique key作为关联条件

const、system: 当MySQL对查询某部分进行优化，并转换为一个常量时，使用这些类型访问。如将主键置于where列表中，MySQL就能将该查询转换为一个常量,system是const类型的特例，当查询的表只有一行的情况下，使用system

NULL: MySQL在优化过程中分解语句，执行时甚至不用访问表或索引，例如从一个索引列里选取最小值可以通过单独索引查找完成。



## 五、possible_keys

**指出MySQL能使用哪个索引在表中找到记录，查询涉及到的字段上若存在索引，则该索引将被列出，但不一定被查询使用**

该列完全独立于EXPLAIN输出所示的表的次序。这意味着在possible_keys中的某些键实际上不能按生成的表次序使用。
如果该列是NULL，则没有相关的索引。在这种情况下，可以通过检查WHERE子句看是否它引用某些列或适合索引的列来提高你的查询性能。如果是这样，创造一个适当的索引并且再次用EXPLAIN检查查询



## 六、Key

**key列显示MySQL实际决定使用的键（索引）**

如果没有选择索引，键是NULL。要想强制MySQL使用或忽视possible_keys列中的索引，在查询中使用FORCE INDEX、USE INDEX或者IGNORE INDEX。



## 七、key_len

表示索引中使用的字节数，可通过该列计算查询中使用的索引的长度（key_len显示的值为索引字段的最大可能长度，并非实际使用长度，即key_len是根据表定义计算而得，不是通过表内检索出的）

不损失精确性的情况下，长度越短越好 



## 八、ref

表示上述表的连接匹配条件，即哪些列或常量被用于查找索引列上的值



## 九、rows

表示MySQL根据表统计信息及索引选用情况，估算的找到所需的记录所需要读取的行数



## 十、Extra

该列包含MySQL解决查询的详细信息,有以下几种情况：

> Using where:列数据是从仅仅使用了索引中的信息而没有读取实际的行动的表返回的，这发生在对表的全部的请求列都是同一个索引的部分的时候，表示mysql服务器将在存储引擎检索行后再进行过滤
>
> Using temporary：表示MySQL需要使用临时表来存储结果集，常见于排序和分组查询
>
> Using filesort：MySQL中无法利用索引完成的排序操作称为“文件排序”
>
> (如果出现以上的两种的红色的Using temporary和Using filesort说明效率低)
>
> Using join buffer：改值强调了在获取连接条件时没有使用索引，并且需要连接缓冲区来存储中间结果。如果出现了这个值，那应该注意，根据查询的具体情况可能需要添加索引来改进能。
>
> Impossible where：这个值强调了where语句会导致没有符合条件的行。
>
> Select tables optimized away：这个值意味着仅通过使用索引，优化器可能仅从聚合函数结果中返回一行
>
>  (复合索引再使用时，尽量的考虑查询时，常用的排序方向和字段组合顺序)
>



**总结：**
**• EXPLAIN不会告诉你关于触发器、存储过程的信息或用户自定义函数对查询的影响情况**
**• EXPLAIN不考虑各种Cache**
**• EXPLAIN不能显示MySQL在执行查询时所作的优化工作**
**• 部分统计信息是估算的，并非精确值**
**• EXPALIN只能解释SELECT操作，其他操作要重写为SELECT后查看执行计划。**