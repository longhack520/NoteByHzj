运维 Shell
<a name="S5f6Q"></a>
## 1、服务器系统配置初始化
```bash
#/bin/bash
# 安装系统性能分析工具及其他
yum install gcc make autoconf vim sysstat net-tools iostat iftop iotp wget lrzsz lsof unzip openssh-clients net-tool vim ntpdate -y
# 设置时区并同步时间
ln -s /usr/share/zoneinfo/Asia/Shanghai /etc/localtime
if ! crontab -l |grep ntpdate &>/dev/null ; then
    (echo "* 1 * * * ntpdate time.windows.com >/dev/null 2>&1";crontab -l) |crontab 
fi
 
# 禁用selinux
sed -i '/SELINUX/{s/permissive/disabled/}' /etc/selinux/config
 
# 关闭防火墙
if egrep "7.[0-9]" /etc/redhat-release &>/dev/null; then
    systemctl stop firewalld
    systemctl disable firewalld
elif egrep "6.[0-9]" /etc/redhat-release &>/dev/null; then
    service iptables stop
    chkconfig iptables off
fi
 
# 历史命令显示操作时间
if ! grep HISTTIMEFORMAT /etc/bashrc; then
    echo 'export HISTTIMEFORMAT="%Y-%m-%d %H:%M:%S  `whoami` "' >> /etc/bashrc
fi
 
# SSH超时时间
if ! grep "TMOUT=600" /etc/profile &>/dev/null; then
    echo "export TMOUT=600" >> /etc/profile
fi
 
# 禁止root远程登录 切记给系统添加普通用户，给su到root的权限
sed -i 's/#PermitRootLogin yes/PermitRootLogin no/' /etc/ssh/sshd_config
 
# 禁止定时任务向发送邮件
sed -i 's/^MAILTO=root/MAILTO=""/' /etc/crontab 
 
# 设置最大打开文件数
if ! grep "* soft nofile 65535" /etc/security/limits.conf &>/dev/null; then
cat >> /etc/security/limits.conf << EOF
    * soft nofile 65535
    * hard nofile 65535
EOF
fi
 
# 系统内核优化
cat >> /etc/sysctl.conf << EOF
net.ipv4.tcp_syncookies = 1
net.ipv4.tcp_max_tw_buckets = 20480
net.ipv4.tcp_max_syn_backlog = 20480
net.core.netdev_max_backlog = 262144
net.ipv4.tcp_fin_timeout = 20  
EOF
 
# 减少SWAP使用
echo "0" > /proc/sys/vm/swappiness
```
<a name="n8nqf"></a>
## 2、批量创建多个用户并设置密码
```bash
#!/bin/bash
USER_LIST=$@
USER_FILE=./user.info
for USER in $USER_LIST;do
 if ! id $USER &>/dev/null; then
  PASS=$(echo $RANDOM |md5sum |cut -c 1-8)
  useradd $USER
  echo $PASS | passwd --stdin $USER &>/dev/null
  echo "$USER   $PASS" >> $USER_FILE
  echo "$USER User create successful."
 else
  echo "$USER User already exists!"
 fi
done
```
<a name="Qm7qD"></a>
## 3、一键查看服务器利用率
```bash
#!/bin/bash
function cpu(){
 
 util=$(vmstat | awk '{if(NR==3)print $13+$14}')
 iowait=$(vmstat | awk '{if(NR==3)print $16}')
 echo "CPU -使用率：${util}% ,等待磁盘IO相应使用率：${iowait}:${iowait}%"
 
}
function memory (){
 
 total=`free -m |awk '{if(NR==2)printf "%.1f",$2/1024}'`
    used=`free -m |awk '{if(NR==2) printf "%.1f",($2-$NF)/1024}'`
    available=`free -m |awk '{if(NR==2) printf "%.1f",$NF/1024}'`
    echo "内存 - 总大小: ${total}G , 使用: ${used}G , 剩余: ${available}G"
}
disk(){
 
 fs=$(df -h |awk '/^\/dev/{print $1}')
    for p in $fs; do
        mounted=$(df -h |awk '$1=="'$p'"{print $NF}')
        size=$(df -h |awk '$1=="'$p'"{print $2}')
        used=$(df -h |awk '$1=="'$p'"{print $3}')
        used_percent=$(df -h |awk '$1=="'$p'"{print $5}')
        echo "硬盘 - 挂载点: $mounted , 总大小: $size , 使用: $used , 使用率: $used_percent"
    done
 
}
function tcp_status() {
    summary=$(ss -antp |awk '{status[$1]++}END{for(i in status) printf i":"status[i]" "}')
    echo "TCP连接状态 - $summary"
}
cpu
memory
disk
tcp_status
```
<a name="HAmaC"></a>
## 4、找出占用CPU 内存过高的进程
```bash
#!/bin/bash
echo "-------------------CUP占用前10排序--------------------------------"
ps -eo user,pid,pcpu,pmem,args --sort=-pcpu  |head -n 10
echo "-------------------内存占用前10排序--------------------------------"
ps -eo user,pid,pcpu,pmem,args --sort=-pmem  |head -n 10
```
<a name="nUf5E"></a>
## 5、查看网卡的实时流量
```bash
#!/bin/bash
eth0=$1
echo  -e    "流量进入--流量传出    "
while true; do
 old_in=$(cat /proc/net/dev |grep $eth0 |awk '{print $2}')
 old_out=$(cat /proc/net/dev |grep $eth0 |awk '{print $10}')
 sleep 1
 new_in=$(cat /proc/net/dev |grep $eth0 |awk '{print $2}')
 new_out=$(cat /proc/net/dev |grep $eth0 |awk '{print $10}')
 in=$(printf "%.1f%s" "$((($new_in-$old_in)/1024))" "KB/s")
 out=$(printf "%.1f%s" "$((($new_out-$old_out)/1024))" "KB/s")
 echo "$in $out"
done
```
<a name="amQ3L"></a>
## 6、监控多台服务器磁盘利用率脚本
```bash
#!/bin/bash
HOST_INFO=host.info
for IP in $(awk '/^[^#]/{print $1}' $HOST_INFO); do
 #取出用户名和端口
    USER=$(awk -v ip=$IP 'ip==$1{print $2}' $HOST_INFO)
    PORT=$(awk -v ip=$IP 'ip==$1{print $3}' $HOST_INFO)
 #创建临时文件，保存信息
    TMP_FILE=/tmp/disk.tmp
 #通过公钥登录获取主机磁盘信息
    ssh -p $PORT $USER@$IP 'df -h' > $TMP_FILE
 #分析磁盘占用空间
    USE_RATE_LIST=$(awk 'BEGIN{OFS="="}/^\/dev/{print $NF,int($5)}' $TMP_FILE)
 #循环磁盘列表，进行判断
    for USE_RATE in $USE_RATE_LIST; do
  #取出等号（=）右边的值 挂载点名称
        PART_NAME=${USE_RATE%=*}  
  #取出等号（=）左边的值  磁盘利用率
        USE_RATE=${USE_RATE#*=}
  #进行判断
        if [ $USE_RATE -ge 80 ]; then
            echo "Warning: $PART_NAME Partition usage $USE_RATE%!"
    echo "服务器$IP的磁盘空间占用过高，请及时处理" | mail -s "空间不足警告" 你的qq@qq.com
  else
   echo "服务器$IP的$PART_NAME目录空间良好"
        fi
    done
done
```
<a name="A0Z7O"></a>
## 7、批量检测网站是否异常并邮件通知
```bash
#!/bin/bash  
URL_LIST="www.baidu.com www.ctnrs.com www.der-matech.net.cn www.der-matech.com.cn www.der-matech.cn www.der-matech.top www.der-matech.org"
for URL in $URL_LIST; do
    FAIL_COUNT=0
    for ((i=1;i<=3;i++)); do
        HTTP_CODE=$(curl -o /dev/null --connect-timeout 3 -s -w "%{http_code}" $URL)
        if [ $HTTP_CODE -eq 200 ]; then
            echo "$URL OK"
            break
        else
            echo "$URL retry $FAIL_COUNT"
            let FAIL_COUNT++
        fi
    done
    if [ $FAIL_COUNT -eq 3 ]; then
        echo "Warning: $URL Access failure!"
  echo "网站$URL坏掉，请及时处理" | mail -s "$URL网站高危" 1794748404@qq.com
    fi
done
```
<a name="SUbx1"></a>
## 8、批量主机远程执行命令脚本
```bash
#!/bin/bash
COMMAND=$*
HOST_INFO=host.info
for IP in $(awk '/^[^#]/{print $1}' $HOST_INFO); do
    USER=$(awk -v ip=$IP 'ip==$1{print $2}' $HOST_INFO)
    PORT=$(awk -v ip=$IP 'ip==$1{print $3}' $HOST_INFO)
    PASS=$(awk -v ip=$IP 'ip==$1{print $4}' $HOST_INFO)
    expect -c "
       spawn ssh -p $PORT $USER@$IP
       expect {
          \"(yes/no)\" {send \"yes\r\"; exp_continue}
          \"password:\" {send \"$PASS\r\"; exp_continue}
          \"$USER@*\" {send \"$COMMAND\r exit\r\"; exp_continue}
       }
    "
    echo "-------------------"
done
```
<a name="IHVr8"></a>
## 9、一键部署LNMP网站平台脚本
```bash
#!/bin/bash
NGINX_V=1.15.6
PHP_V=5.6.36
TMP_DIR=/tmp
 
INSTALL_DIR=/usr/local
 
PWD_C=$PWD
 
echo
echo -e "\tMenu\n"
echo -e "1. Install Nginx"
echo -e "2. Install PHP"
echo -e "3. Install MySQL"
echo -e "4. Deploy LNMP"
echo -e "9. Quit"
 
function command_status_check() {
 if [ $? -ne 0 ]; then
  echo $1
  exit
 fi 
}
 
function install_nginx() {
    cd $TMP_DIR
    yum install -y gcc gcc-c++ make openssl-devel pcre-devel wget
    wget http://nginx.org/download/nginx-${NGINX_V}.tar.gz
    tar zxf nginx-${NGINX_V}.tar.gz
    cd nginx-${NGINX_V}
    ./configure --prefix=$INSTALL_DIR/nginx \
    --with-http_ssl_module \
    --with-http_stub_status_module \
    --with-stream
    command_status_check "Nginx - 平台环境检查失败！"
    make -j 4 
    command_status_check "Nginx - 编译失败！"
    make install
    command_status_check "Nginx - 安装失败！"
    mkdir -p $INSTALL_DIR/nginx/conf/vhost
    alias cp=cp ; cp -rf $PWD_C/nginx.conf $INSTALL_DIR/nginx/conf
    rm -rf $INSTALL_DIR/nginx/html/*
    echo "ok" > $INSTALL_DIR/nginx/html/status.html
    echo '<?php echo "ok"?>' > $INSTALL_DIR/nginx/html/status.php
    $INSTALL_DIR/nginx/sbin/nginx
    command_status_check "Nginx - 启动失败！"
}
 
function install_php() {
 cd $TMP_DIR
    yum install -y gcc gcc-c++ make gd-devel libxml2-devel \
        libcurl-devel libjpeg-devel libpng-devel openssl-devel \
        libmcrypt-devel libxslt-devel libtidy-devel
    wget http://docs.php.net/distributions/php-${PHP_V}.tar.gz
    tar zxf php-${PHP_V}.tar.gz
    cd php-${PHP_V}
    ./configure --prefix=$INSTALL_DIR/php \
    --with-config-file-path=$INSTALL_DIR/php/etc \
    --enable-fpm --enable-opcache \
    --with-mysql --with-mysqli --with-pdo-mysql \
    --with-openssl --with-zlib --with-curl --with-gd \
    --with-jpeg-dir --with-png-dir --with-freetype-dir \
    --enable-mbstring --enable-hash
    command_status_check "PHP - 平台环境检查失败！"
    make -j 4 
    command_status_check "PHP - 编译失败！"
    make install
    command_status_check "PHP - 安装失败！"
    cp php.ini-production $INSTALL_DIR/php/etc/php.ini
    cp sapi/fpm/php-fpm.conf $INSTALL_DIR/php/etc/php-fpm.conf
    cp sapi/fpm/init.d.php-fpm /etc/init.d/php-fpm
    chmod +x /etc/init.d/php-fpm
    /etc/init.d/php-fpm start
    command_status_check "PHP - 启动失败！"
}
 
read -p "请输入编号：" number
case $number in
    1)
        install_nginx;;
    2)
        install_php;;
    3)
        install_mysql;;
    4)
        install_nginx
        install_php
        ;;
    9)
        exit;;
esac
```
<a name="pF7Ib"></a>
## 10、监控MySQL主从同步状态是否异常脚本
```bash
#!/bin/bash  
HOST=localhost
USER=root
PASSWD=123.com
IO_SQL_STATUS=$(mysql -h$HOST -u$USER -p$PASSWD -e 'show slave status\G' 2>/dev/null |awk '/Slave_.*_Running:/{print $1$2}')
for i in $IO_SQL_STATUS; do
    THREAD_STATUS_NAME=${i%:*}
    THREAD_STATUS=${i#*:}
    if [ "$THREAD_STATUS" != "Yes" ]; then
        echo "Error: MySQL Master-Slave $THREAD_STATUS_NAME status is $THREAD_STATUS!" |mail -s "Master-Slave Staus" xxx@163.com
    fi
done
```
<a name="eftfU"></a>
## 11、MySql数据库备份脚本
<a name="os2mb"></a>
### 分库备份
mysqldump -uroot -pxxx -B A > A.sql
```bash
#!/bin/bash
DATE=$(date +%F_%H-%M-%S)
HOST=localhost
USER=backup
PASS=123.com
BACKUP_DIR=/data/db_backup
DB_LIST=$(mysql -h$HOST -u$USER -p$PASS -s -e "show databases;" 2>/dev/null |egrep -v "Database|information_schema|mysql|performance_schema|sys")
 
for DB in $DB_LIST; do
    BACKUP_NAME=$BACKUP_DIR/${DB}_${DATE}.sql
    if ! mysqldump -h$HOST -u$USER -p$PASS -B $DB > $BACKUP_NAME 2>/dev/null; then
        echo "$BACKUP_NAME 备份失败!"
    fi
done
```
<a name="qrOcn"></a>
### 分表备份
mysqldump -uroot -pxxx -A t > t.sql
```bash
#!/bin/bash
DATE=$(date +%F_%H-%M-%S)
HOST=localhost
USER=backup
PASS=123.com
BACKUP_DIR=/data/db_backup
DB_LIST=$(mysql -h$HOST -u$USER -p$PASS -s -e "show databases;" 2>/dev/null |egrep -v "Database|information_schema|mysql|performance_schema|sys")
 
for DB in $DB_LIST; do
    BACKUP_DB_DIR=$BACKUP_DIR/${DB}_${DATE}
    [ ! -d $BACKUP_DB_DIR ] && mkdir -p $BACKUP_DB_DIR &>/dev/null
    TABLE_LIST=$(mysql -h$HOST -u$USER -p$PASS -s -e "use $DB;show tables;" 2>/dev/null)
    for TABLE in $TABLE_LIST; do
        BACKUP_NAME=$BACKUP_DB_DIR/${TABLE}.sql 
        if ! mysqldump -h$HOST -u$USER -p$PASS $DB $TABLE > $BACKUP_NAME 2>/dev/null; then
            echo "$BACKUP_NAME 备份失败!"
        fi
    done
done
```
<a name="gBxjg"></a>
## 12、Nginx访问日志分析
```bash
#!/bin/bash
# 日志格式: $remote_addr - $remote_user [$time_local] "$request" $status $body_bytes_sent "$http_referer" "$http_user_agent" "$http_x_forwarded_for"
LOG_FILE=$1
echo "统计访问最多的10个IP"
awk '{a[$1]++}END{print "UV:",length(a);for(v in a)print v,a[v]}' $LOG_FILE |sort -k2 -nr |head -10
echo "----------------------"
 
echo "统计时间段访问最多的IP"
awk '$4>="[01/Dec/2018:13:20:25" && $4<="[27/Nov/2018:16:20:49"{a[$1]++}END{for(v in a)print v,a[v]}' $LOG_FILE |sort -k2 -nr|head -10
echo "----------------------"
 
echo "统计访问最多的10个页面"
awk '{a[$7]++}END{print "PV:",length(a);for(v in a){if(a[v]>10)print v,a[v]}}' $LOG_FILE |sort -k2 -nr
echo "----------------------"
 
echo "统计访问页面状态码数量"
awk '{a[$7" "$9]++}END{for(v in a){if(a[v]>5)print v,a[v]}}' $LOG_FILE |sort -k3 -nr
```
<a name="eVyaG"></a>
## 13、Nginx访问日志自动按天（周、月）切割
```bash
#!/bin/bash
#nginx日志目录
LOG_DIR=/www/server/nginx/logs
#获取到上一天的时间
YESTERDAY_TIME=$(date -d "yesterday" +%F)
#归档日志取时间
LOG_MONTH_DIR=$LOG_DIR/$(date +"%Y-%m")
#归档日志的名称
LOG_FILE_LIST="access.log"
 
for LOG_FILE in $LOG_FILE_LIST; do
    [ ! -d $LOG_MONTH_DIR ] && mkdir -p $LOG_MONTH_DIR
    mv $LOG_DIR/$LOG_FILE $LOG_MONTH_DIR/${LOG_FILE}_${YESTERDAY_TIME}
done
 
kill -USR1 $(cat $LOG_DIR/nginx.pid)
```
<a name="NiF1o"></a>
## 14、自动发布Java项目（Tomcat）
```bash
#!/bin/bash
DATE=$(date +%F_%T)
 
TOMCAT_NAME=$1
TOMCAT_DIR=/usr/local/$TOMCAT_NAME
ROOT=$TOMCAT_DIR/webapps/ROOT
 
BACKUP_DIR=/data/backup
WORK_DIR=/tmp
PROJECT_NAME=tomcat-java-demo
 
# 拉取代码
cd $WORK_DIR
if [ ! -d $PROJECT_NAME ]; then
   git clone https://github.com/lizhenliang/tomcat-java-demo
   cd $PROJECT_NAME
else
   cd $PROJECT_NAME
   git pull
fi
 
# 构建
mvn clean package -Dmaven.test.skip=true
if [ $? -ne 0 ]; then
   echo "maven build failure!"
   exit 1
fi
 
# 部署
TOMCAT_PID=$(ps -ef |grep "$TOMCAT_NAME" |egrep -v "grep|$$" |awk 'NR==1{print $2}')
[ -n "$TOMCAT_PID" ] && kill -9 $TOMCAT_PID
[ -d $ROOT ] && mv $ROOT $BACKUP_DIR/${TOMCAT_NAME}_ROOT$DATE
unzip $WORK_DIR/$PROJECT_NAME/target/*.war -d $ROOT
$TOMCAT_DIR/bin/startup.sh
```
<a name="UJ2WV"></a>
## 15、自动发布PHP项目
```bash
#!/bin/bash
 
DATE=$(date +%F_%T)
 
WWWROOT=/usr/local/nginx/html/$1
 
 
BACKUP_DIR=/data/backup
WORK_DIR=/tmp
PROJECT_NAME=php-demo
 
 
# 拉取代码
cd $WORK_DIR
if [ ! -d $PROJECT_NAME ]; then
   git clone https://github.com/lizhenliang/php-demo
   cd $PROJECT_NAME
else
   cd $PROJECT_NAME
   git pull
fi
 
 
# 部署
if [ ! -d $WWWROOT ]; then
   mkdir -p $WWWROOT
   rsync -avz --exclude=.git $WORK_DIR/$PROJECT_NAME/* $WWWROOT
else
   rsync -avz --exclude=.git $WORK_DIR/$PROJECT_NAME/* $WWWROOT
fi
```
<a name="XeTTx"></a>
## 16、DOS攻击防范（自动屏蔽攻击IP）
```bash
#!/bin/bash
DATE=$(date +%d/%b/%Y:%H:%M)
#nginx日志
LOG_FILE=/usr/local/nginx/logs/demo2.access.log
#分析ip的访问情况
ABNORMAL_IP=$(tail -n5000 $LOG_FILE |grep $DATE |awk '{a[$1]++}END{for(i in a)if(a[i]>10)print i}')
for IP in $ABNORMAL_IP; do
    if [ $(iptables -vnL |grep -c "$IP") -eq 0 ]; then
        iptables -I INPUT -s $IP -j DROP
        echo "$(date +'%F_%T') $IP" >> /tmp/drop_ip.log
    fi
done
```
<a name="XKzSf"></a>
## 17、目录入侵检测与告警
```bash
#!/bin/bash
 
MON_DIR=/opt
inotifywait -mqr --format %f -e create $MON_DIR |\
while read files; do
   #同步文件
   rsync -avz /opt /tmp/opt
  #检测文件是否被修改
   #echo "$(date +'%F %T') create $files" | mail -s "dir monitor" xxx@163.com
done
```
