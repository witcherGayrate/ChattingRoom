# ChattingRoom
该应用程序支持多人在线聊天和文件传输。

目标是实现一个具备微信基本功能的聊天软件。

#项目概述

（1）PC客户端client界面基于Qt6框架开发，实现气泡聊天对话框，支持添加好友，好友通信，聊天记录展示等功能。仿照微信实现聊天布局和qss优化

（2）后端采用分布式设计，主要语言为C/C++,验证码服务verify server使用node.js

（3）服务器Servers包括GateServer网关服务器、StatusServer状态服务器，VerifyServer验证码服务器，CommunicationServer通信服务器，各个服务之间通过grpc通信，网关对外采用HTTP服务，聊天服务基于asio实现tcp通信和转发。
     
（4）用户数据通过mysql存储并且基于mysqlconnector手动封装连接池，包括redis连接池，以及grpc连接池等。

（5）基于boost asio实现异步通信，通过连接池管理连接。单服务器支持8000连接，多服务器分布部署可支持1W~2W活跃用户。

#项目架构概要说明图

![聊天软件架构](https://github.com/user-attachments/assets/a245f991-4796-4981-84eb-61f85bb6501a)

#登录界面

![聊天软件登录界面](https://github.com/user-attachments/assets/5a47d669-75e7-4407-a4d9-731b6fc975bd)

#用户注册界面

![用户注册界面](https://github.com/user-attachments/assets/72d2d53b-25a4-4a6b-8d15-be788a006cd2)

#密码重置界面

![密码重置界面](https://github.com/user-attachments/assets/5e92545c-5d3d-4a30-9228-544c9c3943e6)


#客户端界面图

![聊天界面](https://github.com/user-attachments/assets/0b65f382-cc1d-4c4b-9cee-f7c798274b18)

#联系人界面

![联系人界面](https://github.com/user-attachments/assets/3ba836ee-0954-4cc1-9012-671aa721d30c)

#补充说明

若客户端出现网络错误是因为后端服务器群没有在运行

