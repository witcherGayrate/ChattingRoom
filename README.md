# ChattingRoom
The application supports multi-person online chatting and file transfer. 
The ultimate goal is to realize a chat software with the basic functions of WeChat.
#项目概述
（1）PC客户端client界面基于Qt6框架开发
（2）服务器端包括多个servers，主要语言为C/C++,验证码服务verify server使用node.js
（3）服务器Servers包括GateServer网关服务器、StatusServer状态服务器，VerifyServer验证码服务器，
     CommunicationServer通信服务器，各个服务之间通过grpc通信，client和server使用asio通信。
（4）redis数据库主要用于验证码的query、insert操作，mysql用于存储user information.
![聊天软件架构](https://github.com/user-attachments/assets/e4cc69d3-87bc-4dc9-be9a-63d938a4ad51)

#补充
项目尚未完成，一些功能待完善