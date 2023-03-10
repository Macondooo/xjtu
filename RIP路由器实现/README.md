# RIP路由器实现

该部分基于伯克利大学cs168课程(网址为https://github.com/NetSys/cs168_proj_routing_student) 所实现的RIP路由算法。 

运行环境：python3.8

编写代码文件：`./cs168_proj_routing_student-master/simulator/dv_router.py`

实验手册详见`./cs168_proj_routing_student-master/project1_writeup`

主要完成的内容如下：
- state1：为每个直接连接路由器的host建立一条静态路由，expiry_time = FOREVER
- state2：为每个路由器实现在接到数据包的时候，将数据包转发到其需要的端口。如果要去的目的地不存在，路由器会简单丢弃数据包；如果到目的地的延迟等于INFINITY，同样也丢弃数据包
- state3：实现路由器周期性的发送路由广播
- state4：实现路由器对其收到的路由广播包的处理，用更好的路由更新路由表，如果旧路由和新路由有同样的表现，同样也采用新路由
- state5：实现对于超时的路由，将其移出路由表
- state6：实现split horizon算法，一个路由器从一个端口学到的路由知识，不会再重新发给这个端口
- state7：实现poison reverse算法，一个路由器对于从该端口学习的知识，在广播路由时向该端口发送latency=INFINITY
- state8：对于有一个环路的无穷计数问题，当一条路径的延时不现实的过大时，认为发生了无穷计数，使用infinity作为界限，不再处理该数据包
- state9：对于已经失效的路由，发送毒性包，latency=infinity，路由器收到该数据包后检查如果自己表项中有该条，则修改其为无效。
- state10：对于路由器广播路由包，实现不广播全部的路由表项，对于每个端口，只是广播上一次没有从该端口发送的路由；同时实现只对一个端口发送路由表项，用于当一个路由器新连接上一个邻接路由器时。
