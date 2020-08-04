package main

import (
	"fmt"
	"net"
)

func main() {
	// 开启服务器监听端口8888
	ln, err := net.Listen("tcp", ":8888")
	if err != nil {
		fmt.Println("开启监听失败", err)
		return
	}
	for {
		// 连接客户端
		conn, err := ln.Accept()
		if err != nil {
			fmt.Println("监听客户端失败", err)
			return
		}
		go handleConnection(conn)
	}
}

// 处理和客户端连接
func handleConnection(conn net.Conn) {
	for {
		b := make([]byte, 512)
		// 读取客户端数据
		cnt, err := conn.Read(b)
		if err != nil {
			fmt.Println("读取数据失败", err)
			return
		}
		fmt.Println("client", string(b[:cnt]))

		// 回写客户端消息
		_, err = conn.Write([]byte("我是小昆虫服务器，收到消息了。"))
		if err != nil {
			fmt.Println("回写数据失败", err)
			return
		}
	}
}
