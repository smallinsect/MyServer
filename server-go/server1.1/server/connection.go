package server

import (
	"fmt"
	"net"
)

type Connection struct {
	Conn      net.Conn
	ConnID    uint32
	handleAPI HandleFunc
	isClose   bool
	ExitChan  chan bool
}

func CreateConnection(conn net.Conn, connID uint32, callback_api HandleFunc) *Connection {
	c := &Connection{
		Conn:      conn,
		ConnID:    connID,
		handleAPI: callback_api,
		isClose:   false,
		ExitChan:  make(chan bool, 1),
	}
	return c
}

func (c *Connection) StartReader() {
	fmt.Println("StartReader Goroutine is running ...")
	defer fmt.Println("StartReader exit ... ConnID", c.ConnID)
	defer c.Stop()

	for {
		buf := make([]byte, 512)
		cnt, err := c.Conn.Read(buf)
		if err != nil {
			fmt.Println("StartReader", err)
			return
		}

		fmt.Println("reader cnt ...", cnt)

		if err := c.handleAPI(c.Conn, buf, cnt); err != nil {
			fmt.Println("StartReader err", err)
			return
		}
	}
}

func (c *Connection) StartWriter() {

}

func (c *Connection) Start() {
	fmt.Println("Conn start ... ConnID=", c.ConnID)

	// 启动读业务
	go c.StartReader()
}

func (c *Connection) Stop() {
	fmt.Println("Conn stop ... ConnID", c.ConnID)
	if c.isClose {
		return
	}
	c.isClose = true
	// c.Conn.Close()
	close(c.ExitChan)
}

func (c *Connection) GetConnection() net.Conn {
	return c.Conn
}

func (c *Connection) GetConnID() uint32 {
	return c.ConnID
}

func (c *Connection) RemoteAddr() net.Addr {
	return c.Conn.RemoteAddr()
}

func (c *Connection) Send(data []byte) error {
	return nil
}
