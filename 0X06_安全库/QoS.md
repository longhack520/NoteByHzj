Qos<br />QoS（Quality of Service）即服务质量。在有限的带宽资源下，QoS为各种业务分配带宽，为业务提供端到端的服务质量保证。例如，语音、视频和重要的数据应用在网络设备中可以通过配置QoS优先得到服务。<br />图1-1 什么是QoS？QoS是如何工作的？<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636289074966-175ba710-a0ec-4d37-b139-4394041442f5.webp#clientId=u8d5c0122-497c-4&from=paste&id=uce4ccf8a&originHeight=408&originWidth=543&originalType=url&ratio=1&status=done&style=none&taskId=u610258ad-0bb1-4ee5-a227-6a3dc4b0ab8)
<a name="cacfH"></a>
## QoS的重要性
在IP网络的业务可以分为实时业务和非实时业务。实时业务往往占据固定带宽，对网络质量变化感知明显，对网络质量的稳定性要求高，例如语音业务。非实时业务所占带宽难以预测，经常会出现突发流量。突发流量会导致网络质量下降，会引起网络拥塞，增加转发时延，严重时还会产生丢包，导致业务质量下降甚至不可用。<br />解决网络拥塞的最好的办法是增加网络的带宽，但从运营、维护的成本考虑，这是不现实的，最有效的解决方案就是应用一个“有保证”的策略对网络流量进行管理。<br />QoS一般针对网络中有突发流量时需要保障重要业务质量的场景。如果业务长时间达不到服务质量要求（例如业务流量长时间超过带宽限制），需要考虑对网络扩容或使用专用设备基于上层应用去控制业务。<br />近几年，视频的应用出现了爆炸式的增长，现在几乎每个人都拥有一部能够随时随地拍摄高分辨率视频的智能手机。同时随着社交网站的涌现，分享和发布视频成了每个人的日常行为，人们不论身在何处都可以将自己制作的视频发布出去与他人分享。对于企业来说，高清视频会议，高清视频监控等应用，也在网络中产生了大量的高清视频流量。与语音流量相比，视频流量占用的带宽更多，也更不稳定，特别是一些交互类视频，对实时性要求非常高。另外，随着无线网络的发展，越来越多的用户和企业都开始使用无线终端，而无线终端会随着用户的移动而不断变化位置，导致网络中的流量更加的不可预测。因此QoS方案设计也面临更多的挑战。
<a name="vRDwg"></a>
## QoS的度量指标
影响网络质量的因素包括传输链路的带宽、报文传送时延和抖动、以及丢包率等，它们也就成为了QoS的度量指标。
<a name="crbvh"></a>
### 带宽
带宽也称为吞吐量，是指在一个固定的时间内（1秒），从网络一端传输到另一端的最大数据位数，也可以理解为网络的两个节点之间特定数据流的平均速率。带宽的单位是比特/秒（bit/s）。在网络中，有两个常见的与带宽有关的概念：上行速率和下行速率。上行速率是指用户向网络发送信息时的数据传输速率，下行速率是指网络向用户发送信息时的传输速率。例如，用户通过FTP上传文件到网络，影响上传文件速度的就是上行速率；而从网络下载文件，影响下载文件速度的就是下行速率。
<a name="NhZFB"></a>
### 时延
时延是指一个报文或分组从网络的发送端到接收端所需要的延迟时间，一般由传输延迟及处理延迟组成。以语音传输为例，时延是指从说话者开始说话到对方听到所说内容的时间。一般人们察觉不到小于100毫秒的延迟。当延迟在100毫秒和300毫秒之间时，说话者可以察觉到对方回复的轻微停顿，这种停顿可能会使通话双方都感觉到不舒服。超过300毫秒，延迟就会很明显，用户开始互相等待对方的回复。当通话的一方不能及时接收到期望的回复时，说话者可能会重复所说的话，这样会与远端延迟的回复碰撞，导致重复。
<a name="iHuid"></a>
### 抖动
如果网络发生拥塞，导致通过同一连接传输的分组延迟各不相同。抖动用来描述延迟变化的程度，也就是最大延迟与最小延迟的时间差。抖动对于实时性的传输是一个重要参数，特别是语音和视频等实时业务是极不容忍抖动的，抖动会造成话音或视频的断续。抖动也会影响一些网络协议的处理。有些协议是按固定的时间间隔发送交互性报文，抖动过大会导致协议震荡。所有传输系统都有抖动，只要抖动在规定容差之内就不会影响服务质量。利用缓存可以克服过量的抖动，但这将增加时延。
<a name="x2KOq"></a>
### 丢包率
丢包率是指在网络传输过程中丢失报文的数量占传输报文总数的百分比。少量的丢包对业务的影响并不大，例如，在语音传输中，丢失一个比特或一个分组的信息，通话双方往往注意不到。在视频的传输中，丢失一个比特或一个分组可能造成在屏幕上瞬间的波形干扰，但能很快恢复正常。使用TCP传送数据可以处理少量的丢包，因为TCP允许丢失的信息重发。但大量的丢包会影响传输效率。在QoS中，关注的是丢包的统计数据，也就是丢包率。所以正常传输时，网络丢包率应该控制在一定范围内即可。
<a name="AHanW"></a>
## QoS的应用场景
以企业办公为例，除了基本的网页浏览、工作邮件外，在较集中的工作时间段内还需要保证Telnet登录设备、异地的视频会议、实时语音通话、FTP文件的上传和下载，以及视频播放等业务的网络质量。对于不同网络质量要求的业务，可以配置不同的QoS子功能，或者不部署QoS。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636289075247-38ced458-2408-4fdc-b2e0-119902c145f0.webp#clientId=u8d5c0122-497c-4&from=paste&id=u12fccb08&originHeight=179&originWidth=527&originalType=url&ratio=1&status=done&style=shadow&taskId=u9ed0c32b-a066-4e0c-8acb-c57f86aa5f9)

- 网络协议和管理协议（如OSPF、Telnet）这类业务要求低时延和低丢包率，但对带宽的要求不高。因此可以通过QoS的优先级映射功能，为此类报文标记较高的服务等级，使网络设备优先转发此类报文。
- 实时业务（如视频会议、VoIP）视频会议要求高带宽、低时延和低抖动。因此可以通过QoS的流量监管功能，为视频报文提供高带宽；通过QoS的优先级映射功能，适当调高视频报文的优先级。VoIP是指通过IP网络进行实时语音通话，它要求网络做到低丢包、低时延和低抖动，否则通话双方可以明显感知质量受损。因此一方面可以调整语音报文的优先级，使其高于视频报文；另一方面通过流量监管功能，为语音报文提供最大带宽，在网络产生拥塞时，可以保证语音报文优先通过。
- 大数据量业务（如FTP、数据库备份、文件转储）大数据量业务是指存在长时间大量数据传输行为的网络业务，这类业务需要尽可能低的网络丢包率。因此可以为这类报文配置流量整形功能，通过数据缓冲区缓存从接口发送的报文，减少由于突发流量导致拥塞而产生的丢包现象。
- 流媒体（如在线音频播放、视频点播）由于这些音视频节目都是提前制作好的，观看者的终端通常可以先进行缓存再进行播放，因此降低了对网络时延、丢包和抖动的要求。如果需要降低这类业务的丢包和时延，可以通过QoS的优先级映射功能，适当提高对应报文的优先级。
- 普通业务（如HTML网页浏览、邮件）这类业务对网络无特殊要求、重要性也不高。管理员可以对其保持默认设置，不需要额外部署QoS功能。
<a name="YrLCG"></a>
## QoS的服务模型
如何在网络中通过部署来保证QoS的度量指标在一定的合理范围内，从而提高网络的服务质量呢？这就涉及到QoS模型。需要说明的是，QoS模型不是一个具体功能，而是端到端QoS设计的一个方案。例如，网络中的两个主机通信时，中间可能会跨越各种各样的设备。只有当网络中所有设备都遵循统一的QoS服务模型时，才能实现端到端的质量保证。<br />下面介绍主流的三大QoS模型。其中，华为公司的交换机、路由器、防火墙、WLAN等产品均支持配置基于DiffServ服务模型的QoS业务。
<a name="keYlJ"></a>
### Best-Effort服务模型：尽力而为
Best-Effort是最简单的QoS服务模型，用户可以在任何时候，发出任意数量的报文，而且不需要通知网络。提供Best-Effort服务时，网络尽最大的可能来发送报文，但对时延、丢包率等性能不提供任何保证。Best-Effort服务模型适用于对时延、丢包率等性能要求不高的业务，是现在Internet的缺省服务模型，它适用于绝大多数网络应用，如FTP、E-Mail等。
<a name="SPZ90"></a>
### IntServ服务模型：预留资源
IntServ模型是指用户在发送报文前，需要通过信令（Signaling）向网络描述自己的流量参数，申请特定的QoS服务。网络根据流量参数，预留资源以承诺满足该请求。在收到确认信息，确定网络已经为这个应用程序的报文预留了资源后，用户才开始发送报文。用户发送的报文应该控制在流量参数描述的范围内。网络节点需要为每个流维护一个状态，并基于这个状态执行相应的QoS动作，来满足对用户的承诺。<br />IntServ模型使用了RSVP（Resource Reservation Protocol）协议作为信令，在一条已知路径的网络拓扑上预留带宽、优先级等资源，路径沿途的各网元必须为每个要求服务质量保证的数据流预留想要的资源，通过RSVP信息的预留，各网元可以判断是否有足够的资源可以使用。只有所有的网元都给RSVP提供了足够的资源，“路径”方可建立。
<a name="CyI2P"></a>
### DiffServ服务模型：差分服务
DiffServ模型的基本原理是将网络中的流量分成多个类，每个类享受不同的处理，尤其是网络出现拥塞时不同的类会享受不同级别的处理，从而得到不同的丢包率、时延以及时延抖动。同一类的业务在网络中会被聚合起来统一发送，保证相同的时延、抖动、丢包率等QoS指标。<br />Diffserv模型中，业务流的分类和汇聚工作在网络边缘由边界节点完成。边界节点可以通过多种条件（比如报文的源地址和目的地址、ToS域中的优先级、协议类型等）灵活地对报文进行分类，对不同的报文设置不同的标记字段，而其他节点只需要简单地识别报文中的这些标记，即可进行资源分配和流量控制。<br />与Intserv模型相比，DiffServ模型不需要信令。在DiffServ模型中，应用程序发出报文前，不需要预先向网络提出资源申请，而是通过设置报文的QoS参数信息，来告知网络节点它的QoS需求。网络不需要为每个流维护状态，而是根据每个报文流指定的QoS参数信息来提供差分服务，即对报文的服务等级划分，有差别地进行流量控制和转发，提供端到端的QoS保证。DiffServ模型充分考虑了IP网络本身灵活性、可扩展性强的特点，将复杂的服务质量保证通过报文自身携带的信息转换为单跳行为，从而大大减少了信令的工作，是当前网络中的主流服务模型。
<a name="SZT91"></a>
### 基于DiffServ模型的QoS组成
基于Diffserv模型的QoS业务主要分为以下几大类：

- 报文分类和标记要实现差分服务，需要首先将数据包分为不同的类别或者设置为不同的优先级。报文分类即把数据包分为不同的类别，可以通过MQC配置中的流分类实现。报文标记即为数据包设置不同的优先级，可以通过优先级映射和重标记优先级实现。不同的报文使用不同的QoS优先级，例如VLAN报文使用802.1p，IP报文使用DSCP，MPLS报文使用EXP。
- 流量监管、流量整形和接口限速流量监管和流量整形可以将业务流量限制在特定的带宽内，当业务流量超过额定带宽时，超过的流量将被丢弃或缓存。其中，将超过的流量丢弃的技术称为流量监管，将超过的流量缓存的技术称为流量整形。接口限速分为基于接口的流量监管和基于接口的流量整形。
- 拥塞管理和拥塞避免拥塞管理在网络发生拥塞时，将报文放入队列中缓存，并采取某种调度算法安排报文的转发次序。而拥塞避免可以监督网络资源的使用情况，当发现拥塞有加剧的趋势时采取主动丢弃报文的策略，通过调整流量来解除网络的过载。

其中，报文分类和标记是实现差分服务的前提和基础；流量监管、流量整形、接口限速、拥塞管理和拥塞避免从不同方面对网络流量及其分配的资源实施控制，是提供差分服务的具体体现。<br />各种QoS技术在网络设备上的处理顺序如下图所示。<br />图1-2 QoS技术处理流程<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636289074858-cfb96e5c-62e6-4c3e-86e9-0c064c6233dc.webp#clientId=u8d5c0122-497c-4&from=paste&id=u0b1dfb9c&originHeight=194&originWidth=512&originalType=url&ratio=1&status=done&style=shadow&taskId=u6ab638e4-6622-499a-a75d-224ce087e3b)
<a name="x5YNY"></a>
## 对比QoS和HQoS
传统QoS技术可以满足语音、视频以及数据等业务的不同服务需求，可以针对不同的业务提供不同的服务。但是随着网络设备的高速发展，接入用户数量和每个用户的业务量不断增多，传统的QoS在应用中遇到了新问题：

- 传统QoS是基于端口带宽进行调度的，因此流量管理可以基于服务等级进行业务区分，却很难基于用户进行区分，因此比较适合部署在网络核心侧，但不适合部署在业务接入侧。
- 传统QoS无法做到同时对多个用户的多个业务进行流量管理和调度。

图1-3 传统QoS vs HQoS调度模型<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636289074933-63db7a5e-45ac-4fcc-a3b0-155414c43fbd.webp#clientId=u8d5c0122-497c-4&from=paste&id=u11609646&originHeight=257&originWidth=690&originalType=url&ratio=1&status=done&style=shadow&taskId=ua4bfc39e-cdaa-474d-8cdc-c16f7203e9b)<br />为了解决上述问题，人们需要一种既能区分用户流量又能根据用户业务的优先级进行调度的技术，HQoS（Hierarchical Quality of Service，层次化QoS）应运而生。HQoS通过多级队列进一步细化区分业务流量，对多个用户、多种业务等传输对象进行统一管理和分层调度，在现有的硬件环境下使设备具备内部资源的控制策略，既能够为高级用户提供质量保证，又能够从整体上节约网络建设成本。
