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
	"unsafe"
)

func checkError(err error) {
	if err != nil {
		fmt.Println("Error:%s", err.Error())
		os.Exit(1)
	}
}

func main() {
	udp_addr, err := net.ResolveUDPAddr("udp", ":8001")
	checkError(err)
	conn, err := net.ListenUDP("udp", udp_addr)
	defer conn.Close()
	checkError(err)

	totalPackets := 0
	var decodeBuffer []byte
	for {
		data := make([]byte, 1024)
		read, remoteAddr, err := conn.ReadFromUDP(data)
		if err != nil {
			fmt.Println("Read from UDP failed!", err)
			continue
		}
		totalPackets++
		fmt.Println(data[:read])
		decodeBuffer = append(decodeBuffer, data[:read]...)
		fmt.Println(len(decodeBuffer), cap(decodeBuffer))
		fmt.Println("Recv packet#:", totalPackets, "Length:", read, "From:", remoteAddr)
		//fmt.Println(data[:read])
		if ret := C.ecg_decode((*C.uchar)(unsafe.Pointer(&decodeBuffer[0])), C.uint(read)); ret > 0 {
			fmt.Println(ret)
			decodeBuffer = decodeBuffer[ret:]
			fmt.Println(len(decodeBuffer), cap(decodeBuffer))
		}
	}
}
