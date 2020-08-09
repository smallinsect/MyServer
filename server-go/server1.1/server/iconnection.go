package server

import "net"

type IConnection interface {
	Start()
	Stop()
	GetConnection() net.Conn
	GetConnID() uint32
	RemoteAddr() net.Addr
	Send(data []byte) error
}

type HandleFunc func(net.Conn, []byte, int) error
