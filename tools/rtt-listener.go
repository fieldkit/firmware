package main

import (
	"bufio"
	"fmt"
	"net"
	"time"
)

const (
	AnnouncementString = "HERE"
)

type RTTListener struct {
	conn      net.Conn
	announced time.Time
	detected  bool
}

func NewRTTListener() RTTListener {
	return &RTTListener{}
}

func (l *RTTListener) handle(line string) {
	fmt.Println(line)

	if !l.detected {
		elapsed := time.Now().Sub(l.announced)
		if elapsed > 5*time.Second {
			l.Announce()
		}
	}
}

func (l *RTTListener) Announce() {
	l.conn.Write([]byte(AnnouncementString))
	l.announced = time.Now()
}

func (l *RTTListener) Open(addr string) {
	c, err := net.Dial("tcp", addr)
	if err != nil {
		panic(err)
	}

	l.conn = c

	scanner := bufio.NewScanner(l.conn)
	for {
		if ok := scanner.Scan(); !ok {
			break
		}

		l.handle(scanner.Text())
	}
}

func main() {
	l := NewRTTListener()
	l.Open("127.0.0.1:19021")

}
