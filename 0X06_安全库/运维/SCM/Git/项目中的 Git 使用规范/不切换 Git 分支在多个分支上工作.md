Git<br />一旦进入某个项目，从开发，到发布生产，到 hotfix，到后期维护，那基本都有开发者的份，正在开发某个 feature，老板突然跳出来说做生产上的 hotfix 更是家常便饭，面对这种情况，使用 Git 的通常有两种解决方案：

1. 草草提交未完成的 feature，然后切换分支到 hotfix
2. `git stash` | `git stash pop` 暂存工作内容，然后再切换到 hotfix

第二种方式较第一种还好很多，可是面对下面这些场景，stash 依旧不是很好的解决方案
<a name="gPiBp"></a>
## 场景

1. 正在 main 分支上跑长时间的测试，切换到 hotfix 或 feature, 测试就会中断
2. 项目非常大，频繁的切换索引，成本非常高
3. 有几年前 release 的旧版本，设置和当前不一样，IDE restructure 适配切换也会带来很大的开销
4. 切换分支，需要重新设置相应的环境变量，比如 dev/qa/prod
5. 需要切换到同事的代码，帮助调试代码复现问题

有的同学想到，`git clone` 多个 repo 不就可以了吗？这是解决上述问题的一个方法，但背后同样隐藏很多问题：

1. 多个 repo 的状态是不好同步的，比如没办法快速 cherry-pick，一个 repo checkout 的分支，另外一个 repo 需要重新 checkout
2. git history/log 是重复的，当项目历史非常长，.git 文件夹下的内容是非常占用磁盘空间的
3. 同一个项目，多个 repo，不易管理

那如何做才能满足这些特殊场景，又不出现这些上述这些问题呢？
<a name="VTzCa"></a>
## git-worktree
其实，这是 Git 2015 年就开始支持的功能，却很少有人知道它，git-worktree 的使用非常方便，在终端输入：
```bash
git worktree --help
```
就可以快速看到帮助文档说明：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642382290411-b6f33be1-95b8-4f96-bf2b-98a5269324a3.webp#clientId=u67216f48-94cb-4&from=paste&id=u0d0cb6de&originHeight=705&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2d3d9a51-9377-417e-8168-cc8197d10e8&title=)<br />用简单的话来解释 git-worktree 的作用就是：<br />仅需维护一个 repo，又可以同时在多个 branch 上工作，互不影响<br />上面红色框线命令有很多，常用的其实只有下面这四个：
```bash
 git worktree add [-f] [--detach] [--checkout] [--lock] [-b <new-branch>] <path> [<commit-ish>]
 git worktree list [--porcelain]
 git worktree remove [-f] <worktree>
 git worktree prune [-n] [-v] [--expire <expire>]
```
在展开说明之前，需要和大家普及两个可能忽视的 Git 知识点：

1. 默认情况下， `git init` 或 `git clone` 初始化的 repo，只有一个 worktree，叫做 main worktree
2. 在某一个目录下使用 Git 命令，当前目录下要么有 .git 文件夹；要么有 .git 文件，如果只有 .git 文件，里面的内容必须是指向 .git 文件夹的

第二句话感觉挺绕的，下面用例子说明，就很容易明白了
<a name="hLurY"></a>
### `git worktree add`
当前项目目录结构是这样的(amend-crash-demo 是 repo 的 root)：
```
.
└── amend-crash-demo

1 directory
```
`cd amend-crash-demo` 运行命令 `git worktree add ../feature/feature2`
```
➜  amend-crash-demo git:(main) git worktree add ../feature/feature2
Preparing worktree (new branch 'feature2')
HEAD is now at 82b8711 add main file
```
重新看目录结构
```
.
├── amend-crash-demo
└── feature
    └── feature2

3 directories
```
该命令默认会根据 HEAD 所在的 commit-ish (当然也可以指定 git log 中的任意一个 commit-ish) 创建一个名为 feature2 的分支，分支磁盘位置如上面结构所示<br />cd ../feature/feature2/ 会发现，这个分支下并不存在 .git 文件夹，却存在一个 .git 文件，打开文件，内容如下：
```
gitdir: /Users/rgyb/Documents/projects/amend-crash-demo/.git/worktrees/feature2
```
到这里，再理解一下上面的知识点2，是不是就清晰许多了呢？<br />接下来，就可以在 feature2 分支上做一切想做的内容了(add/commit/pull/push)，和 main worktree 互不干扰<br />一般情况下，项目组都有一定的分支命名规范，比如 feature/JIRAID-Title, hotfix/JIRAID-Title, 如果仅仅按照上面命令新建 worktree，分支名称中的 / 会被当成文件目录来处理
```bash
git worktree add ../hotfix/hotfix/JIRA234-fix-naming
```
运行完该命令，文件目录结构是这样的
```
.
├── amend-crash-demo
├── feature
│   └── feature2
└── hotfix
    └── hotfix
        └── JIRA234-fix-naming

6 directories
```
很显然这不是想要的，这时就需要 `-b` 参数的支持了，就像 `git checkout -b` 一样<br />执行命令：
```bash
git worktree add -b "hotfix/JIRA234-fix-naming" ../hotfix/JIRA234-fix-naming
```
再来看一下目录结构
```
.
├── amend-crash-demo
├── feature
│   └── feature2
└── hotfix
    ├── JIRA234-fix-naming
    └── hotfix
        └── JIRA234-fix-naming

7 directories
```
进入 JIRA234-fix-naming 目录，默认是在 hotfix/JIRA234-fix-naming 分支上<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642382290111-6f875878-ec71-4b5f-81c2-82f3d1af3fb1.webp#clientId=u67216f48-94cb-4&from=paste&id=u5fba7534&originHeight=168&originWidth=940&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf0aba8b5-c2fc-4068-9eb9-e79e2c0ed84&title=)<br />worktree 建立起来很容易，不加管理，项目目录结构肯定乱糟糟，这是不想看到的，所以需要清晰的知道某个 repo 都建立了哪些 worktree
<a name="hCatV"></a>
### `git worktree list`
所有的worktree 都在共用一个 repo，所以在任意一个 worktree 目录下，都可以执行如下命令来查看 worktree 列表
```bash
git worktree list
```
执行完命令后，可以查看到上面创建的所有 worktree 信息，`main worktree` 也会显示在此处
```bash
/Users/rgyb/Documents/projects/amend-crash-demo                        82b8711 [main]
/Users/rgyb/Documents/projects/chore/chore                                   8782898 (detached HEAD)
/Users/rgyb/Documents/projects/feature/feature2                             82b8711 [feature2]
/Users/rgyb/Documents/projects/hotfix/hotfix/JIRA234-fix-naming     82b8711 [JIRA234-fix-naming]
/Users/rgyb/Documents/projects/hotfix/JIRA234-fix-naming              82b8711 [hotfix/JIRA234-fix-naming]
```
worktree 的工作做完了，也是要及时删除的，否则也会浪费很多磁盘空间
<a name="IivQh"></a>
### `git worktree remove`
这个命令很简单了，worktree 的名字叫什么，直接就 remove 什么就好了
```bash
git worktree remove hotfix/hotfix/JIRA234-fix-naming
```
此时，分支名弄错的那个 hotfix 就被删掉了
```bash
/Users/rgyb/Documents/projects/amend-crash-demo                        82b8711 [main]
/Users/rgyb/Documents/projects/chore/chore                                   8782898 (detached HEAD)
/Users/rgyb/Documents/projects/feature/feature2                             82b8711 [feature2]
/Users/rgyb/Documents/projects/hotfix/JIRA234-fix-naming              82b8711 [hotfix/JIRA234-fix-naming]
```
假设创建一个 worktree，并在里面有改动，突然间这个worktree 又不需要了，此刻按照上述命令是不能删掉了，此时就需要 `-f` 参数来帮忙了
```bash
git worktree remove -f hotfix/JIRA234-fix-naming
```
删除了 worktree，其实在 Git 的文件中，还有很多 administrative 文件是没有用的，为了保持清洁，还需要进一步清理
<a name="TXdta"></a>
### `git worktree prune`
这个命令就是清洁的兜底操作，可以让工作始终保持整洁
<a name="n0Ata"></a>
## 总结
到这里，应该理解，整个 git-worktree 的使用流程就是下面这四个命令：
```bash
git worktree add
git worktree list
git worktree remove
git worktree prune
```
也应该明白 `git worktree` 和 `git clone` 多个 repo 的区别了。只维护一个 repo，创建多个 worktree，操作间行云流水<br />实践：通常使用 git worktree，会统一目录结构，比如 feature 目录下存放所有 feature 的worktree，hotfix 目录下存放所有 hotfix 的 worktree，这样整个磁盘目录结构不至于因为创建多个 worktree 而变得混乱<br />在磁盘管理上有些强迫症，理想情况下，某个 repo 的 worktree 最好放在这个 repo 的文件目录里面，但这就会导致 Git track 新创建的 worktree 下的所有文件，为了避免 Git track worktree 的内容，来来回回修改 gitignore 文件肯定是不合适的
