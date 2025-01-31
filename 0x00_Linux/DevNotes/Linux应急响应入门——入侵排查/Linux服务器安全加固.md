Linux 安全
<a name="z36bJ"></a>
### 1. 账号和口令
<a name="TcOys"></a>
#### 1.1 禁用或删除无用账号
减少系统无用账号，降低安全风险。<br />**操作步骤**

- 使用命令 `userdel <用户名>` 删除不必要的账号。
- 使用命令 `passwd -l <用户名>` 锁定不必要的账号。
- 使用命令 `passwd -u <用户名>` 解锁必要的账号。
<a name="E5SX2"></a>
#### 1.2 检查特殊账号
检查是否存在空口令和root权限的账号。<br />**操作步骤**

1. 查看空口令和root权限账号，确认是否存在异常账号：
   - 使用命令 `awk -F: '($2=="")' /etc/shadow` 查看空口令账号。
   - 使用命令 `awk -F: '($3==0)' /etc/passwd` 查看UID为零的账号。
2. 加固空口令账号：
   - 使用命令 `passwd <用户名>` 为空口令账号设定密码。
   - 确认UID为零的账号只有root账号。
<a name="YSEgG"></a>
#### 1.3 添加口令策略
加强口令的复杂度等，降低被猜解的可能性。<br />**操作步骤**

1. 使用命令 `vi /etc/login.defs` 修改配置文件。
   - `PASS_MAX_DAYS 90 #新建用户的密码最长使用天数`
   - `PASS_MIN_DAYS 0 #新建用户的密码最短使用天数`
   - `PASS_WARN_AGE 7 #新建用户的密码到期提前提醒天数`
2. 使用chage命令修改用户设置。<br />例如，`chage -m 0 -M 30 -E 2000-01-01 -W 7 <用户名>`表示将此用户的密码最长使用天数设为30，最短使用天数设为0，密码2000年1月1日过期，过期前七天警告用户。
3. 设置连续输错三次密码，账号锁定五分钟。使用命令 `vi /etc/pam.d/common-auth`修改配置文件，在配置文件中添加 `auth required pam_tally.so onerr=fail deny=3 unlock_time=300`。
<a name="m9SRq"></a>
#### 1.4 限制用户su
限制能su到root的用户。<br />**操作步骤**<br />使用命令 `vi /etc/pam.d/su`修改配置文件，在配置文件中添加行。例如，只允许test组用户su到root，则添加 `auth required pam_wheel.so group=test`。
<a name="z4ovB"></a>
#### 1.4 禁止root用户直接登录
限制root用户直接登录。<br />**操作步骤**

1. 创建普通权限账号并配置密码,防止无法远程登录;
2. 使用命令 `vi /etc/ssh/sshd_config`修改配置文件将PermitRootLogin的值改成no，并保存，然后使用`service sshd restart`重启服务。
<a name="Ag8pb"></a>
### 2. 服务
<a name="QW5Ly"></a>
#### 2.1 关闭不必要的服务
关闭不必要的服务（如普通服务和xinetd服务），降低风险。<br />**操作步骤**<br />使用命令`systemctl disable <服务名>`设置服务在开机时不自动启动。<br />**说明**：对于部分老版本的Linux操作系统（如CentOS 6），可以使用命令`chkconfig \--level <init级别> <服务名> off`设置服务在指定init级别下开机时不自动启动。
<a name="hz0vP"></a>
#### 2.2 SSH服务安全
对SSH服务进行安全加固，防止暴力破解成功。<br />**操作步骤**<br />使用命令 `vim /etc/ssh/sshd_config` 编辑配置文件。

- 不允许root账号直接登录系统。<br />设置 PermitRootLogin 的值为 no。
- 修改SSH使用的协议版本。<br />设置 Protocol 的版本为 2。
- 修改允许密码错误次数（默认6次）。<br />设置 MaxAuthTries 的值为 3。

配置文件修改完成后，重启sshd服务生效。<br />
<a name="biLNI"></a>
### 3. 文件系统
<a name="XabHO"></a>
#### 3.1 设置umask值
设置默认的umask值，增强安全性。<br />**操作步骤**<br />使用命令 `vi /etc/profile` 修改配置文件，添加行 `umask 027`， 即新创建的文件属主拥有读写执行权限，同组用户拥有读和执行权限，其他用户无权限。
<a name="pQJwo"></a>
#### 3.2 设置登录超时
设置系统登录后，连接超时时间，增强安全性。<br />**操作步骤**<br />使用命令 `vi /etc/profile` 修改配置文件，将以 `TMOUT=` 开头的行注释，设置为`TMOUT=180`，即超时时间为三分钟。
<a name="8Qexu"></a>
### 4. 日志
<a name="3snuB"></a>
#### 4.1 syslogd日志
启用日志功能，并配置日志记录。<br />**操作步骤**<br />Linux系统默认启用以下类型日志：

- 系统日志（默认）/var/log/messages
- cron日志（默认）/var/log/cron
- 安全日志（默认）/var/log/secure

**注意**：部分系统可能使用syslog-ng日志，配置文件为：/etc/syslog-ng/syslog-ng.conf。<br />您可以根据需求配置详细日志。
<a name="uUIEQ"></a>
#### 4.2 记录所有用户的登录和操作日志
通过脚本代码实现记录所有用户的登录操作日志，防止出现安全事件后无据可查。<br />**操作步骤**<br />1.打开配置文件
```bash
[root@xxx /]# vim /etc/profile
```
2.在配置文件中输入以下内容：
```bash
history
USER=`whoami`
USER_IP=`who -u am i 2>/dev/null| awk '{print $NF}'|sed -e 's/[()]//g'`
if [ "$USER_IP" = "" ]; then
USER_IP=`hostname`
fi
if [ ! -d /var/log/history ]; then
mkdir /var/log/history
chmod 777 /var/log/history
fi
if [ ! -d /var/log/history/${LOGNAME} ]; then
mkdir /var/log/history/${LOGNAME}
chmod 300 /var/log/history/${LOGNAME}
fi
export HISTSIZE=4096
DT=`date +"%Y%m%d_%H:%M:%S"`
export HISTFILE="/var/log/history/${LOGNAME}/${USER}@${USER_IP}_$DT"
chmod 600 /var/log/history/${LOGNAME}/*history* 2>/dev/null
```

3. 运行加载配置生效。
```bash
[root@xxx /]# source /etc/profile
```
:::tips
**注意**：/var/log/history 是记录日志的存放位置，可以自定义。
:::
通过上述步骤，可以在 /var/log/history 目录下以每个用户为名新建一个文件夹，每次用户退出后都会产生以用户名、登录IP、时间的日志文件，包含此用户本次的所有操作（root用户除外）。
