package main

import (
	"fmt"
	"os"
	"os/signal"
	"server-go/server1.1/server"
)

func main() {
	myserver := server.CreateServer("小昆虫")
	myserver.Start()

	c := make(chan os.Signal, 1)
	signal.Notify(c, os.Kill, os.Interrupt)
	si := <-c
	fmt.Println("程序停止", si)

	myserver.Stop()
}
