Linux
<a name="L2qEh"></a>
## 接口信息
当需要了解有关登录的设备的网络接口的更多信息时使用接口信息命令。
<a name="HVi49"></a>
### IP 命令
查看所有 IP 地址:
```bash
ip a
```
可以使用-4或-6选项添加过滤器以仅显示 IPv4 或 IPv6 地址。
```bash
ip -4 a
ip -6 a
```
查看网络信息的另一种方法是使用ifconfig，它比ip命令更容易阅读，显示的信息量没有太大差异，一个显着的区别是ifconfig显示一些基本的传输/接收统计信息，这些统计信息可能很有用，一目了然。
```bash
ifconfig
```
<a name="OoFJB"></a>
### 网络状态命令
netstat命令对于发现机器上各种服务正在侦听的端口很有用，`-t`可以选择 TCP 和`-u` UDP，使用-l侦听端口的选项，如果更喜欢查看 IP 地址而不是主机名，请使用该`-n`选项。<br />添加多个选项时，可以将它们组合在一起，如下所示。
```bash
netstat -tul
```
如果要查看侦听端口的服务的进程 ID (pid)，请使用该`-p`选项，需要使用sudo此命令来查看pid，否则，它不会显示。
```bash
sudo netstat -tulp
```
<a name="ReiSR"></a>
## 网络可用性
网络可用性命令可用于快速检查是否可以访问网络上的主机或确定主机是否已打开电源（假设它已连接到网络）。
<a name="DNGyM"></a>
### Ping 命令
最著名的网络命令是ping，可以快速查看是否可以通过网络访问设备，前提是它没有被防火墙阻止，由于流量不需要由路由器路由（流量保持本地），因此始终允许同一网络内的 Ping。<br />可以使用ping带有 IP 地址或主机名的命令。
```bash
ping 192.168.1.10
ping thehostname
```
ping 将继续 ping 直到停止命令（取决于使用的终端，可以使用键盘快捷键（例如“Ctrl + Shift + C”）关闭命令）<br />要指定 ping 数，请使用该-c选项以及要执行的 ping 数。
```bash
ping -c 3 192.168.1.10
```
如果启用了 IPv6，则该ping命令可能默认使用 IPv6 IP 地址，可以使用-4或-6选项指定使用 IPv4 或 IPv6 地址。
```bash
ping -4 192.168.1.10
ping -6 192.168.1.10
```
可以同时使用多个选项，下面的命令使用 IPv4 地址并 ping 3 次。
```bash
ping -4 -c 3 192.168.1.10
```
某些防火墙配置为禁用 ping，但如果有多个内部网络，建议在本地网络上允许 ping，即使ping阻止了 WAN 地址，因为这对于故障排除非常有用。如果担心如果内部网络或 DMZ 之一受到威胁，会更容易发现网络上的设备，则不允许从内部网络向 DMZ 发出 ping 或从 DMZ 发出 ping，阻止进出 DMZ 的 ping 有助于进一步隔离该网络。
<a name="HEPYa"></a>
### 跟踪路由命令
跟踪路由对于查看到达 Internet 上另一个网络所采用的路径很有用，除非网络中有多个路由器，否则它在本地网络上用处不大。<br />对于大多数家庭用户来说，情况并非如此，但一些家庭实验室可能会使用路由器来运行单独的内部网络，某些traceroute输出中有星号或问号，这是因为跟踪的整个路径可能并不总是显示，特别是如果防火墙配置为出于安全考虑丢弃 ICMP 流量。<br />基本traceroute命令如下所示，可以使用 IP 地址或主机名。
```bash
traceroute 8.8.8.8
traceroute google.com
```
与 一样ping，可以指定 IPv4 和 IPv6。traceroute默认为 IPv4，不像ping需要指定该-4选项。
```bash
traceroute -4 google.com
traceroute -6 google.com
```
traceroute 还有许多其他选项，但基本命令可能对许多用户来说已经足够了。
<a name="NUuoc"></a>
## 域名系统
DNS 命令在分配主机名和/或覆盖路由器中的 DNS 值时尤其有用，可以确定主机名是否指向正确的 IP 地址。
<a name="BUSGi"></a>
### 主机命令
要在本地网络或 Internet 上查找与域名关联的 IP 地址，请使用该host命令。
```bash
host google.com
```
如果要指定用于 DNS 查找的 DNS 服务器。
```bash
host google.com 1.1.1.1
```
<a name="pmxby"></a>
### 挖掘命令
如果希望获得更详细的 DNS 信息，该dig命令很有用，基本用法与host命令类似。
```bash
dig google.com
```
还可以指定用于查找的 DNS 服务器。
```bash
dig @8.8.8.8 google.com
```
如果想要一个域的所有 DNS 记录，可以使用该any选项。
```bash
dig google.com any
```
还可以指定其他 DNS 记录，例如 MX 记录。
```bash
dig google.com mx
```
当想知道给定 IP 地址的主机名/域名时，反向 DNS 查找很有用，结合使用-x选项和 IP 地址即可。
```bash
dig -x 8.8.8.8
```
<a name="bNUno"></a>
### 系统解析命令
systemd-resolve命令可用于检查当前的 DNS 服务器设置为什么，当在设置新的内部网络或弄乱路由器上的 DNS 设置时对 DNS 问题进行故障排除时，这很有帮助。如果运行以下命令，将看到一行Current DNS Server，DNS Servers其中列出了通过 DHCP 或手动设置的 DNS。
```bash
systemd-resolve --status
```
当更改某些 DNS 设置以使用拆分 DNS（也称为水平拆分、拆分域等）时，可以发现能够刷新 DNS 解析器缓存很有用，以便系统更快地开始解析新的 IP 地址。要刷新缓存，只需使用`--flush-caches`选项。
```bash
sudo systemd-resolve --flush-caches
```
该systemd-resolve命令仅对使用systemd.
<a name="akJkB"></a>
## DHCP
DHCP 命令非常有用，尤其是当由于对路由器或交换机所做的更改而想要手动释放和更新 DHCP 租约时。
<a name="Xb9Wa"></a>
### Dhclient 命令
当通过 DHCP 为路由器静态分配 IP 地址、更改路由器/交换机上的 DHCP 地址范围或其他与 DHCP 相关的网络更改，可以使用dhclient命令更轻松地更新设备 DHCP租期。<br />如果不使用该命令，要么必须以物理方式或通过操作系统控制面板断开并重新连接到网络，要么等待 DHCP 租约到期。<br />要在设备上释放和续订 DHCP 租约，必须按所示顺序输入以下两个命令，还需要sudo此命令的权限。
```bash
sudo dhclient -r
sudo dhclient
```
