// Copyright 2018  Author. 高继勇.
//
// channel 和 goroutine 配合并发的demo
package main

import (
	"fmt"
	"time"
	"strconv"
)

func main() {

	//runtime.GOMAXPROCS(4)//开启多核并行

	sliceList := []int{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20}

	//定义一个channel
	channelMy := make(chan int, 20)

	for _, item := range sliceList {
		//channel 满5个会阻塞
		channelMy <- 1

		//开启一个goroutine
		go task(channelMy, item)

	}

	close(channelMy)

	time.Sleep(100 * time.Second)

	fmt.Println("程序结束")

}

func task(channelMy chan int, num int) {

	fmt.Println("任务：" + strconv.Itoa(num) + "开始...")

	//模拟任务执行时间
	time.Sleep(10 * time.Second)

	fmt.Println("任务：" + strconv.Itoa(num) + "结束!!!")

	<-channelMy
}
