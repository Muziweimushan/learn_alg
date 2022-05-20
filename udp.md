# UDP的使用场景
## 为什么使用udp?是带宽问题吗?
> 一般来说,带宽测试TCP优于UDP

## 使用场景
+ 要求实时性----UDP重传策略可以自己定制
    + 音视频通话.弱网环境下使用TCP会导致频繁的包重传,进而导致声音延迟,卡顿...UDP虽然也会卡,但是它能够降低延迟
    + 游戏,比如王者荣耀(实时战略游戏),TCP协议栈中会有应答,就会出现我一开始想走上路,下一秒想走下路,但是走上路这条指令都没发到服务器手机就一直等着服务器...(最坏的情况需要干掉连接才能取消命令...).  UDP则可以由程序员自己控制重传机制
    + webrtc的nack,比如客户端要求重发,服务端发现该数据没用了已经(超时)可以通过nack来不重发

+ 减少资源
DNS查询是使用UDP的,为什么呢?----DNS查询只有一个req和一个resp,如果用TCP就起码有三次握手四次挥手等等...
    + 手机厂商的监控.微信,美团什么的不定时监控位置,发送不频繁,如果都维持一个tcp,开销太大.

# sendto和recvfrom的使用
## sendto和recvfrom的问题



# udp可靠性设计
## 在UDP报文中添加一个应用层自定义的header, UDP报文 = header + body(实际传输的数据)
### header的构成
    同步头 + 数据总大小 + 分片数量 + 分片编号 + 当前分片长度 + padding/预留
    包含数据总大小域的原因是为了乱序或者丢包时能通过任意一个分片知道本次通信总大小,进而便于申请buffer

    这种私有协议在ip和端口不变的情况下是可以的,但是如果客户端所处的网络环境经常变化,就不行了

# udp服务器怎么识别客户端
socket通过ip+port构成,client在移动过程中ip/port都可能发生改变,服务端如何识别?
**在协议头中添加一个类似于序列号的信息**

## 对于序列号的分配有两种情况
+ 每个客户端都有一个固定的ID提前分配好
+ 客户端ID由服务端分配----在连接时,客户端先发送一个请求包REQ给服务端,REQ就是一个预先定义好的数据串,服务器可以识别. 会话ID,在webrtc ice里边就这么做的

因为UDP不像TCP那样一直保留一个fd socket,所以需要带上一个会话ID来标识客户端信息


# tcpip协议栈
## 网络通信
tcp socket type为 SOCK_STREAM
socket, 套接字---> 插座, 由两部分组成,一个是插,一个是座,两部分组成(文件描述符fd + 网络协议栈)
三次握手是哪个网络API时发生的? ---- connect(client) accept(server)


网络API
+ socket
+ listen
+ bind
+ connect
+ accept
+ send
+ recv
+ sendto
+ recvfrom
+ close
+ read
+ write

tcp client :
socket -> connect -> read/recv -> close

tcp server :
socket -> bind -> listen(fd, blog) -> accept -> send/write -> close

三次握手:从客户端发起连接到建立稳定连接需要三步
client ----  SYN同步头syn.seq -----> server
client <--- syn.ack 对同步头的应答(syn.seq + 1),同时携带一个新的seq --- server
client ---- ack(新的seq+1) ---> server














