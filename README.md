# TcpMultipleChat
Background
Multi person chat and file transfer based on TCP protocol

Feature
1.multi-chat
2.login reminded, logout reminded
3.private-chat
4.transfer files to server
Server:
1.when server closed, all client will exit
2.using linked list to store userinfo

Usage
client:
1.How to run
./TCPClient <ip> <port>
2.private-chat
: 1 hello,1

server:
./TCPClient <ip> <port>

Example
client:
./TCPClient 127.0.0.1 50002
server:
./TCPClient 127.0.0.1 50002


Generator



有以下不规范做法，属于是个人练手项目，所以没有太多时间精力去追求细节，仅仅交流思路
1.服务器接收文件应该新建端口进行接收,或者标记消息流,把消息流和文件流分开。
2.在服务器的文件接收应该建立线程，并且进行连续接收的，但为了实验断点续传方法，所以用了函数封装+标志位识别的方法来写来接收文件。
3.由于上传文件功能是计划外加入的，所以加入该功能后整体代码可读性下降了很多，推荐了解多人聊天功能的话看，v1.1源码