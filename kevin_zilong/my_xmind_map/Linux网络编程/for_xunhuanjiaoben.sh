#!/bin/sh
#-*- coding: utf-8 -*-
#author:肖子龙


simulate_key 28
sleep 15
simulate_key 28
sleep 15
simulate_key 108
sleep 5
simulate_key 108
sleep 5

#循环
for i in $(seq 1 50)
#while true
do
	#跟踪命令：将代码打印出来
	set -x
	simulate_key 106
	free
	top -n 1
	echo "run $i times"
	#确保正确
	sleep 5
	set +x
done
	