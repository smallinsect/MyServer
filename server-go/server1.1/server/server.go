package server

import (
	"errors"
	"fmt"
	"net"
)

type Server struct {
	Name string
	ln   net.Listener
	IP   string
	Port int
}

func CallBackToClient(conn net.Conn, data []byte, cnt int) error {

	if _, err := conn.Write(data[:cnt]); err != nil {
		fmt.Println("write buf err", err)
		return errors.New("CallBackToClient err")
	}

	return nil
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
	go func() {
		cid := uint32(0)
		for {
			conn, err := s.ln.Accept()
			if err != nil {
				fmt.Println("接受客户端连接失败", err)
				continue
			}

			dealConn := CreateConnection(conn, cid, CallBackToClient)
			cid++
			go dealConn.Start()
		}
	}()
}

func (s *Server) Stop() {
	s.ln.Close()
}
