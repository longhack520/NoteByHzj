Java<br />SQL是说明性语言而非面向对象或是命令式编程语言。在SQL中要写个查询语句是很简单的。但在Java里类似的语句却不容易，因为程序员不仅要反复考虑编程范式，而且也要考虑算法的问题。<br />下面是Java程序员在写SQL时常犯的10个错误（没有特定的顺序）。
<a name="DD1RJ"></a>
## 1、忘掉NULL
Java程序员写SQL时对NULL的误解可能是最大的错误。也许是因为（并非唯一理由）NULL也称作UNKNOWN。如果被称作 UNKNOWN，这还好理解些。另一个原因是，当从数据库拿东西或是绑定变量时，JDBC将SQL NULL 和Java中的null对应了起来。这样导致了NULL = NULL（SQL）和null=null（Java）的误解。<br />对于NULL最大的误解是当NULL被用作行值表达式完整性约束条件时。另一个误解出现在对于NULL 在 NOT IN anti-joins的应用中。<br />解决方法：

- 当写SQL时要不停得想到NULL的用法：
- 这个NULL完整性约束条件是正确的？
- NULL是否影响到结果？
<a name="zbz2d"></a>
## 2、在Java内存中处理数据
很少有Java开发者能将SQL理解的很好。偶尔使用的JOIN，还有古怪的UNION，好吧，但是对于窗口函数呢？还有对集合进行分组呢？许多 的Java开发者将SQL数据加载到内存中，将这些数据转换成某些相近的集合类型，然后再那些集合上面使用边界循环控制结构（至少在Java8的集合升级 以前）执行令人生厌的数学运算。<br />但是一些SQL数据库支持先进的（而且是SQL标准支持的）OLAP特性，这一特性表现更好而且写起来也更加方便。一个（并不怎么标准的）例子就 是Oracle超棒的MODEL分句。只让数据库来做处理然后只把结果带到Java内存中吧。因为毕竟所有非常聪明的家伙已经对这些昂贵的产品进行了优 化。因此实际上，通过将OLAP移到数据库，将获得一下两项好处：

- 便利性。这比在Java中编写正确的SQL可能更加的容易。
- 性能表现。数据库应该比算法处理起来更加快。而且更加重要的是，不必再去传递数百万条记录了。

解决方法：<br />每次使用Java实现一个以数据为中心的算法时，问问自己：有没有一种方法可以让数据库做这种麻烦事。
<a name="hAuIN"></a>
## 3、使用`UNION` 代替`UNION ALL`

- UNION ALL（允许重复）
- UNION （去除了重复）

移除重复行不仅很少需要（有时甚至是错的），而且对于带很多行的大数据集合会相当慢，因为两个子select需要排序，而且每个元组也需要和它的子序列元组比较。<br />注意即使SQL标准规定了INTERSECT ALL和EXCEPT ALL，很少数据库会实现这些没用的集合操作符。<br />解决方法：<br />每次写UNION语句时，考虑实际上是否需要UNION ALL语句。
<a name="PzQ29"></a>
## 4、通过JDBC分页技术给大量的结果进行分页操作
大部分的数据库都会支持一些分页命令实现分页效果，譬如`LIMIT..OFFSET`，`TOP..START AT`，`OFFSET..FETCH`语句等。即使没有支持这些语句的数据库，仍有可能对ROWNUM（Oracle）或者是ROW NUMBER()、OVER()过滤（DB2、SQL Server2008等），这些比在内存中实现分页更快速。在处理大量数据中，效果尤其明显。<br />解决方法：<br />仅仅使用这些语句，那么一个工具（例如JOOQ）就可以模拟这些语句的操作。
<a name="dUVdB"></a>
## 5、在Java内存中加入数据
从SQL的初期开始，当在SQL中使用JOIN语句时，一些开发者仍旧有不安的感觉。这是源自对加入JOIN后会变慢的固有恐惧。<br />假如基于成本的 优化选择去实现嵌套循环，在创建一张连接表源前，可能加载所有的表在数据库内存中，这可能是真的。但是这事发生的概率太低了。通过合适的预测，约束和索 引，合并连接和哈希连接的操作都是相当的快。这完全是是关于正确元数据（在这里不能够引用Tom Kyte的太多）。而且，可能仍然有不少的Java开发人员加载两张表通过分开查询到一个映射中，并且在某种程度上把他们加到了内存当中。<br />解决方法：<br />假如在各个步骤中有从各种表的查询操作，好好想想是否可以表达查询操作在单条语句中。
<a name="Kx7al"></a>
## 6、在一个临时的笛卡尔积集合中使用 DISTINCT 或 UNION 消除重复项
通过复杂的连接，人们可能会对SQL语句中扮演关键角色的所有关系失去概念。特别的，如果这涉及到多列外键关系的话，很有可能会忘记在JOIN .. ON子句中增加相关的判断。这会导致重复的记录，但或许只是在特殊的情况下。有些开发者因此可能选择DISTINCT来消除这些重复记录。从三个方面来说 这是错误的：

- 它（也许）解决了表面症状但并没有解决问题。它也有可能无法解决极端情况下的症状。
- 对具有很多列的庞大的结果集合来说它很慢。DISTINCT要执行ORDER BY操作来消除重复。
- 对庞大的笛卡尔积集合来说它很慢，还是需要加载很多的数据到内存中。

解决方法：<br />根据经验，如果获得了不需要的重复记录，还是检查JOIN判断吧。可能在某个地方有一个很难觉察的笛卡尔积集合。
<a name="RMYqY"></a>
## 7、不使用MERGE语句
这并不是一个过失，但是可能是缺少知识或者对于强悍的MERGE语句信心不足。一些数据库理解其它形式的更新插入（UPSERT）语句，如 MYSQL的重复主键更新语句，但是MERGE在数据库中确是很强大，很重要，以至于大肆扩展SQL标准，例如SQL SERVER。<br />解决方法：<br />如果使用像联合INSERT和UPDATE或者联合SELECT .. FOR UPDATE然后在INSERT或UPDATE等更新插入时，请三思。完全可以使用一个更简单的MERGE语句来远离冒险竞争条件。
<a name="V06Mg"></a>
## 8、使用聚合函数代替窗口函数（window functions）
在介绍窗口函数之前，在SQL中聚合数据意味着使用GROUP BY语句与聚合函数相映射。在很多情形下都工作得很好，如聚合数据需要浓缩常规数据，那么就在join子查询中使用group查询。<br />但是在SQL2003中定义了窗口函数，这个在很多主流数据库都实现了它。窗口函数能够在结果集上聚合数据，但是却没有分组。事实上，每个窗口函数都有自己的、独立的PARTITION BY语句，这个工具对于显示报告太好了。<br />使用窗口函数：

- 使SQL更易读（但在子查询中没有GROUP BY语句专业）
- 提升性能，像关系数据库管理系统能够更容易优化窗口函数

解决方法：<br />当在子查询中使用GROUP BY语句时，请再三考虑是否可以使用窗口函数完成。
<a name="tW4rC"></a>
## 9、使用内存间接排序
SQL的ORDER BY语句支持很多类型的表达式，包括CASE语句，对于间接排序十分有用。可能重来不会在Java内存中排序数据，因为你会想：

- SQL排序很慢
- SQL排序办不到

解决方法：<br />如果在内存中排序任何SQL数据，请再三考虑，是否不能在数据库中排序。这对于数据库分页数据十分有用。
<a name="CB7QH"></a>
## 10、一条一条地插入大量记录
JDBC“懂”批处理（batch），应该不会忘了它。不要使用INSERT语句来一条一条的出入成千上万的记录，（因为）每次都会创建一个新 的PreparedStatement对象。如果所有记录都插入到同一个表时，那么就创建一个带有一条SQL语句以及附带很多值集合的插入批处理语句。可能需要在达到一定量的插入记录后才提交来保证UNDO日志瘦小，这依赖于数据库和数据库设置。<br />解决方法：<br />总是使用批处理插入大量数据。
