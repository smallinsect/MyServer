package server

import (
	"fmt"
	"net"
)

type Server struct {
	Name string
	ln   net.Listener
	IP   string
	Port int
}

func CreateServer(name string) (r IServer) {
	r = &Server{
		Name: name,
		IP:   "0.0.0.0",
		Port: 8888,
	}
	return
}

func (s *Server) Start() {
	ln, err := net.Listen("tcp", fmt.Sprintf(":%d", s.Port))
	if err != nil {
		fmt.Println("开启服务器失败", err)
		return
	}
	s.ln = ln
	for {
		conn, err := s.ln.Accept()
		if err != nil {
			fmt.Println("接受客户端连接失败", err)
			continue
		}

		go handleConn(conn)
	}
}

func (s *Server) Stop() {

}

func handleConn(conn net.Conn) {
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
