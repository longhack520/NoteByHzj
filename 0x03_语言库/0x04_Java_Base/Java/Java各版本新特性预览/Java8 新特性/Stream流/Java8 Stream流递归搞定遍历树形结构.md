Java8 Stream<br />可能平常会遇到一些需求，比如构建菜单，构建树形结构，数据库一般就使用父id来表示，为了降低数据库的查询压力，我们可以使用Java8中的Stream流一次性把数据查出来，然后通过流式处理，一起来看看，代码实现为了实现简单，就模拟查看数据库所有数据到List里面。
<a name="Bf6HD"></a>
### 实体类：Menu.java
```java
/**
 * Menu
 */
@Data
@Builder
public class Menu {
    /**
     * id
     */
    public Integer id;
    /**
     * 名称
     */
    public String name;
    /**
     * 父id ，根节点为0
     */
    public Integer parentId;
    /**
     * 子节点信息
     */
    public List<Menu> childList;


    public Menu(Integer id, String name, Integer parentId) {
        this.id = id;
        this.name = name;
        this.parentId = parentId;
    }

    public Menu(Integer id, String name, Integer parentId, List<Menu> childList) {
        this.id = id;
        this.name = name;
        this.parentId = parentId;
        this.childList = childList;
    }
    
}
```
<a name="ko7pJ"></a>
### 递归组装树形结构
```java
@Test
public void testtree(){
    //模拟从数据库查询出来
    List<Menu> menus = Arrays.asList(
        new Menu(1,"根节点",0),
        new Menu(2,"子节点1",1),
        new Menu(3,"子节点1.1",2),
        new Menu(4,"子节点1.2",2),
        new Menu(5,"根节点1.3",2),
        new Menu(6,"根节点2",1),
        new Menu(7,"根节点2.1",6),
        new Menu(8,"根节点2.2",6),
        new Menu(9,"根节点2.2.1",7),
        new Menu(10,"根节点2.2.2",7),
        new Menu(11,"根节点3",1),
        new Menu(12,"根节点3.1",11)
    );

    //获取父节点
    List<Menu> collect = menus.stream().filter(m -> m.getParentId() == 0).map(
        (m) -> {
            m.setChildList(getChildrens(m, menus));
            return m;
        }
    ).collect(Collectors.toList());
    System.out.println("-------转json输出结果-------");
    System.out.println(JSON.toJSON(collect));
}

	/**
     * 递归查询子节点
     * @param root  根节点
     * @param all   所有节点
     * @return 根节点信息
     */
private List<Menu> getChildrens(Menu root, List<Menu> all) {
    List<Menu> children = all.stream().filter(m -> {
        return Objects.equals(m.getParentId(), root.getId());
    }).map(
        (m) -> {
            m.setChildList(getChildrens(m, all));
            return m;
        }
    ).collect(Collectors.toList());
    return children;
}
```
<a name="IYspu"></a>
### 格式化打印结果
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636173264361-74a7534c-a897-4d05-9132-4da040fb798d.webp#clientId=u3d7d3207-9a19-4&from=paste&id=uf7ac8c3f&originHeight=838&originWidth=537&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8b4f8ff9-f21b-48c5-8ad2-3ec453d70f8&title=)
