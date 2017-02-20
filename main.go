package main

/*
#include <stdio.h>

int ecg_decode(unsigned char *buff, unsigned int buff_size)
{
	int i;

	printf(" decode ECG buffer: %p, size %d\n ", buff, buff_size);
#if 1
	for(i = 0; i < buff_size; i++) {
		printf("%02x ", buff[i]);
	}
#endif
	printf("\n");
	return 0;
}

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
	data := make([]byte, 4096)
	for {
		read, remoteAddr, err := conn.ReadFromUDP(data)
		if err != nil {
			fmt.Println("Read from UDP failed!", err)
			continue
		}
		totalPackets++
		fmt.Println("Recv packet#:", totalPackets, "Length:", read, "From:", remoteAddr)
		//fmt.Println(data[:read])
		C.ecg_decode((*C.uchar)(unsafe.Pointer(&data[0])), C.uint(read))
	}
}
