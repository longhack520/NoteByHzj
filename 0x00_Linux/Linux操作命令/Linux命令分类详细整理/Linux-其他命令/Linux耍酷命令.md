Linux
<a name="yFOHN"></a>
## asciiview 
自从有了你，黑与白的世界也可以五彩缤纷<br />安装指令如下：
```bash
sudo apt-get install aview imagemagick  
asciiview kobe.jpg  
```
执行结果如下；（致敬蜗壳）<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1638281238138-00d64423-74c5-4976-8516-fe796f25aa03.gif#clientId=u8c404496-2367-4&from=paste&id=ucd364286&originHeight=497&originWidth=814&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u12730cd2-dcd7-49b7-aa52-df53346f2c3&title=)<br />PS：可更换妹子的照片，ohoho
<a name="A9hqo"></a>
## oneko 
我的系统里养了一只小猫，它的名字叫 “爱你”<br />你就是鼠标，我便是“爱你”，一直追着鼠标跑<br />安装指令如下；
```bash
sudo apt-get install ninvaders
```
执行结果如下；<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1638281238521-11ee29dc-1c5d-46f0-9d3e-e727e3d7eb18.gif#clientId=u8c404496-2367-4&from=paste&id=ua84e56c4&originHeight=558&originWidth=747&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud8f83857-ef8c-47f7-8693-cd28fde5bca&title=)
<a name="l7exz"></a>
## cmatrix 
今天没见你，我的系统为你下起了 “大雨”<br />安装指令如下；
```bash
sudo apt-get install cmatrix
```
执行结果如下；<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1638281238220-7651064d-f13b-49f1-8d5c-cba5286bdcee.gif#clientId=u8c404496-2367-4&from=paste&id=u951641ea&originHeight=519&originWidth=819&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u817abd70-3b5b-4dbe-9bd1-c679a7416ed&title=)<br />图片
<a name="dEzsG"></a>
## pipe 
手撸 pipe<br />做我女朋友，我将为你筑起无数爱的城墙<br />脚本如下所示；
```bash
#!/bin/bash  
# The author of the original script is unknown to me. The first entry I can  
# find was posted at 2010-03-21 09:50:09 on Arch Linux Forums (doesn't mean the  
# poster is the author at all):  
#  
#   Post your handy self made command line utilities (Page 37) / Programming & Scripting / Arch Linux Forums  
#  
# I, Yu-Jie Lin, made a few changes and additions:  
#  
#   -p, -t, -R, and -C  
#  
#   Screenshot: http://flic.kr/p/dRnLVj  
#   Screencast: http://youtu.be/5XnGSFg_gTk  
#  
# And push the commits to Gist:  
#  
#   https://gist.github.com/4689307  
#  
# I, Devin Samarin, made a few changes and additions:  
#  
#   -r can be 0 to mean "no limit".  
#   Reset cursor visibility after done.  
#   Cleanup for those people who want to quit with ^C  
#  
#   Pushed the changes to https://gist.github.com/4725048  
#   hole1: https://gist.githubusercontent.com/livibetter/4689307/raw/949e43fe2962c2c97c8b1d974ff93dd053d9bd37/pipes.sh  
#   hole2: Fun On The Terminal Part 2  
p=1  
f=75 s=13 r=2000 t=0  
w=$(tput cols) h=$(tput lines)  
# ab -> idx = a*4 + b  
# 0: up, 1: right, 2: down, 3: left  
# 00 means going up   , then going up   -> ┃  
# 12 means going right, then going down -> ┓  
sets=(  
    "┃┏ ┓┛━┓  ┗┃┛┗ ┏━"  
    "│╭ ╮╯─╮  ╰│╯╰ ╭─"  
    "│┌ ┐┘─┐  └│┘└ ┌─"  
    "║╔ ╗╝═╗  ╚║╝╚ ╔═"  
)  
v="${sets[0]}"  
RNDSTART=0  
NOCOLOR=0  
OPTIND=1  
while getopts "p:t:f:s:r:RCh" arg; do  
case $arg in  
    p) ((p=(OPTARG>0)?OPTARG:p));;  
    t) ((OPTARG>=0 && OPTARG<${#sets[@]})) && v="${sets[OPTARG]}";;  
    f) ((f=(OPTARG>19 && OPTARG<101)?OPTARG:f));;  
    s) ((s=(OPTARG>4 && OPTARG<16 )?OPTARG:s));;  
    r) ((r=(OPTARG>=0)?OPTARG:r));;  
    R) RNDSTART=1;;  
    C) NOCOLOR=1;;  
    h) echo -e "Usage: $(basename $0) [OPTION]..."  
        echo -e "Animated pipes terminal screensaver.\n"  
        echo -e " -p [1-]\tnumber of pipes (D=1)."  
        echo -e " -t [0-$((${#sets[@]} - 1))]\ttype of pipes (D=0)."  
        echo -e " -f [20-100]\tframerate (D=75)."  
        echo -e " -s [5-15]\tprobability of a straight fitting (D=13)."  
        echo -e " -r LIMIT\treset after x characters, 0 if no limit (D=2000)."  
        echo -e " -R \t\trandom starting point."  
        echo -e " -C \t\tno color."  
        echo -e " -h\t\thelp (this screen).\n"  
        exit 0;;  
    esac  
done  
cleanup() {  
    tput rmcup  
    tput cnorm  
    exit 0  
}  
trap cleanup SIGHUP SIGINT SIGTERM  
for (( i=1; i<=p; i++ )); do  
    c[i]=$((i%8)) n[i]=0 l[i]=0  
    ((x[i]=RNDSTART==1?RANDOM*w/32768:w/2))  
    ((y[i]=RNDSTART==1?RANDOM*h/32768:h/2))  
done  
tput smcup  
tput reset  
tput civis  
while ! read -t0.0$((1000/f)) -n1; do  
    for (( i=1; i<=p; i++ )); do  
        # New position:  
        ((${l[i]}%2)) && ((x[i]+=-${l[i]}+2,1)) || ((y[i]+=${l[i]}-1))  
        # Loop on edges (change color on loop):  
        ((${x[i]}>w||${x[i]}<0||${y[i]}>h||${y[i]}<0)) && ((c[i]=RANDOM%8))  
        ((x[i]=(x[i]+w)%w))  
        ((y[i]=(y[i]+h)%h))  
        # New random direction:  
        ((n[i]=RANDOM%s-1))  
        ((n[i]=(${n[i]}>1||${n[i]}==0)?${l[i]}:${l[i]}+${n[i]}))  
        ((n[i]=(${n[i]}<0)?3:${n[i]}%4))  
        # Print:  
        tput cup ${y[i]} ${x[i]}  
        [[ $NOCOLOR == 0 ]] && echo -ne "\033[1;3${c[i]}m"  
        echo -n "${v:l[i]*4+n[i]:1}"  
        l[i]=${n[i]}  
    done  
    ((r>0 && t*p>=r)) && tput reset && tput civis && t=0 || ((t++))  
done  
cleanup
```
执行结果如下；<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1638281238350-00b64fab-3457-44e1-a154-337790b99e2c.gif#clientId=u8c404496-2367-4&from=paste&id=uc7a16029&originHeight=648&originWidth=759&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u305d3540-1627-4b2b-94fb-a48385de944&title=)
<a name="ruak1"></a>
## sl 
我的“火车”，拉满爱意，只为奔你而来
```bash
sudo apt-get install sl  
```
执行结果如下：<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1638281238145-12ff3fdf-bd27-4a61-a12c-a6c8789b0568.gif#clientId=u8c404496-2367-4&from=paste&id=ua50a22bc&originHeight=553&originWidth=820&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5ba931d6-0d50-491d-a35f-ad6f6714840&title=)
<a name="r7uMc"></a>
## ninvaders 
带妹子展示，最“远古版”太空侵略者<br />安装指令如下；
```bash
sudo apt-get install ninvaders
```
执行结果如下；<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1638281238643-87055878-5da2-49ae-87b7-44153701faa1.gif#clientId=u8c404496-2367-4&from=paste&id=u6da4baac&originHeight=497&originWidth=814&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1d482810-8ddd-460f-9e35-00192dddd25&title=)
