package main

/*
#include <stdio.h>
#include <string.h>
#include "ecg.h"
*/
import "C"

import (
	"fmt"
	"net"
	"os"
	"time"
	"unsafe"
)

func checkError(err error) {
	if err != nil {
		fmt.Println("Error:%s", err.Error())
		os.Exit(1)
	}
}

var bltPackets = [][]byte{
	//192.168.1.94:1024->192.168.1.255:8001, 12B
	{0xff, 0xd0, 0x01, 0xff, 0x00, 0x7a, 0x00, 0x01, 0x00, 0x22, 0x00, 0x00},
	//192.168.1.236:64666->192.168.1.255:8001, 12B
	{0xff, 0xd0, 0xf1, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00},
	//192.168.1.236:64667->255.255.255.255:8002, 9B
	{0xff, 0xd0, 0x7f, 0xf1, 0x00, 0xfe, 0x00, 0x01, 0x30},
	//192.168.1.94:1025->192.168.1.236:8001, 93B
	{0xff, 0xd4, 0x01, 0xf1, 0x00, 0x00, 0x56, 0xce, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x06, 0x0d, 0x02, 0x04, 0x78, 0x32, 0x00, 0xfa, 0x00, 0x03, 0x04, 0x64, 0x5a, 0xf9, 0x00, 0x04, 0x0c, 0x78, 0x32, 0x00, 0xf8, 0x00, 0x06, 0x21, 0x80, 0x14, 0x04, 0x1e, 0x08, 0xc1, 0x00, 0x04, 0x00, 0xc5, 0x86, 0x68, 0xfb, 0x00, 0x0c, 0x20, 0x00, 0x04, 0xa0, 0x5a, 0x00, 0x5a, 0x32, 0x00, 0x6e, 0x3c, 0x00},
	//192.168.1.236:64304->192.168.1.94:8001, 10B
	{0xff, 0xda, 0xf1, 0x01, 0x00, 0x02, 0x00, 0x01, 0xfc, 0x9e},
	//192.168.1.94:8001->192.168.1.236:8001, 9B
	{0xff, 0xda, 0x01, 0xf1, 0x00, 0x01, 0x00, 0x02, 0x01},
	//192.168.1.94:1024->192.168.1.236:64670, 297B
	{0xff, 0xd1, 0x01, 0xf1, 0x00, 0x01, 0x22, 0x00, 0xfd, 0x00, 0xba, 0x00, 0x80, 0x02, 0x00, 0x00, 0x2f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0xf8, 0x00, 0x27, 0x00, 0x83, 0x21, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0xfa, 0x00, 0x26, 0x81, 0x7f, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0xf9, 0x00, 0x03, 0xff, 0x98, 0x01, 0xfb, 0x00, 0x02, 0x80, 0x00, 0xc1, 0x00, 0x03, 0x84, 0xff, 0x81},
	//192.168.1.236:64671->192.168.1.94:8001, 12B
	{0xff, 0xd0, 0xf1, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00},
	//192.168.1.94:1024->192.168.1.236:64670,745B
	{0xff, 0xd1, 0x01, 0xf1, 0x00, 0x02, 0xe2, 0x01, 0xfd, 0x01, 0xfa, 0x00, 0x80, 0x02, 0x00, 0x00, 0x2f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0xf8, 0x00, 0x67, 0x00, 0x83, 0x21, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0xfa, 0x00, 0x66, 0x81, 0x7f, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0xf9, 0x00, 0x03, 0xff, 0x98, 0x01, 0xfb, 0x00, 0x02, 0x80, 0x00, 0xc1, 0x00, 0x03, 0x84, 0xff, 0x81},
	//192.168.1.236:64304->192.168.1.94:8001, 8B
	{0xff, 0xda, 0xf1, 0x01, 0x00, 0x02, 0x00, 0x07},
	//192.168.1.94:1025->192.168.1.236:8001, 93B
	//{0xff, 0xd4, 0x01, 0xf1, 0x00, 0x00, 0x56, 0xce, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc1, 0x00, 0x04, 0x00, 0xc5, 0x86, 0x68, 0xfb, 0x00, 0xfd, 0x00, 0x06, 0x0d, 0x02, 0x04, 0x78, 0x32, 0x00, 0xfa, 0x00, 0x03, 0x04, 0x64, 0x5a, 0xf9, 0x00, 0x04, 0x0c, 0x78, 0x32, 0x00, 0xf8, 0x00, 0x06, 0x21, 0x80, 0x14, 0x04, 0x1e, 0x08, 0xc1, 0x00, 0x04, 0x00, 0xc5, 0x86, 0x68, 0xfb, 0x00, 0x0c, 0x20, 0x00, 0x04, 0xa0, 0x5a, 0x00, 0x5a, 0x32, 0x00, 0x6e, 0x3c, 0x00},
}

func main() {
	udpAddr, err := net.ResolveUDPAddr("udp", ":8001")
	checkError(err)
	udpConn, err := net.ListenUDP("udp", udpAddr)
	defer udpConn.Close()
	checkError(err)

	for pktIndex, _ := range bltPackets {
		if ret := C.ecg_decode((*C.uchar)(unsafe.Pointer(&bltPackets[pktIndex][0])),
			C.int(len(bltPackets[pktIndex]))); ret > 0 {
			fmt.Println("Decode packet OK, size ", ret)
		}
		fmt.Println("")
		time.Sleep(time.Second)
	}

	for {
		time.Sleep(time.Second)
	}
	/*
		totalPackets := 0
		var decodeBuffer []byte
		for {
			fmt.Println("read from UDP 8001")
			data := make([]byte, 1024)
			readSize, remoteAddr, err := udpConn.ReadFromUDP(data)
			if err != nil {
				fmt.Println("Read from UDP failed!", err)
				continue
			}
			totalPackets++
			fmt.Println(data[:readSize])
			decodeBuffer = append(decodeBuffer, data[:readSize]...)
			fmt.Println(len(decodeBuffer), cap(decodeBuffer))
			fmt.Println("Recv packet#:", totalPackets, "Length:", readSize, "From:", remoteAddr)
			//fmt.Println(data[:readSize])
			if ret := C.ecg_decode((*C.uchar)(unsafe.Pointer(&decodeBuffer[0])), C.int(readSize)); ret > 0 {
				decodeBuffer = decodeBuffer[ret:]
				fmt.Println(len(decodeBuffer), cap(decodeBuffer))
			}
		}
	*/
}
