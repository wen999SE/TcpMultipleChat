# TcpMultipleChat
## Table of Contents

- [Background](#background)
- [Exhibition](#exhibition)
- [Feature](#feature)
- [Usage](#usage)
	- [Generator](#generator)
- [Notice](#notice)
- [License](#license)
# Background
Multi person chat and file transfer based on TCP protocol

# Exhibition
[![show](https://github.com/wen99se/TcpMultipleChat/blob/master/readme_img/show.png)]


# Feature
## Client:
1. multi-chat
2. login reminded, logout reminded
3. private-chat
4. transfer files to server
## Server:
1. when server closed, all client will exit
2. using linked list to store userinfo

# Usage
## client:
1.How to run
```sh
$ ./TCPClient <ip> <port>
```
2.private-chat
 ```sh
$ : <ID> <message>
 ```
3.transfer files to server
 ```sh
 $ + filename
 ```

## server:
```sh
$ ./TCPServer <ip> <port>
```

# Example
## client:
1.How to run
```sh
$ ./TCPClient 127.0.0.1 50002
```
2.private-chat
```sh
$ : 1 hello,1
```
3.transfer files to server
```sh
$ + ncurses.tar.gz
```

## server:
```sh
$ ./TCPServer 127.0.0.1 50002
```

# Generator
Using QT to build the project,you can go to directory "TcpMultipleChat/.../build-TCPServer-Desktop-Debug" to make to generate TCPServer and TCPClient.


# Notice
There are the following non-standard practices，whicht is a personal practice，so I don't have too much time and energy to pursue details,just to exchange ideas
1. The server should create a new port for receiving files,or mark the message stream to separate the message stream from the translate file stream.
2. In order to test the breakpoint continuation method, I use function encapsulation and flag bit recognition method to recerve files.
3. Since the upload file function was added unplanned, the overall code readability after the addition of this function decreased a lot.If you want to know the multi-chat func, Just recommended to read the v1.1 source code.

# License

[MIT](LICENSE) © wen99se
