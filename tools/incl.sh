#!/bin/sh

CPU_COUNT=`cat /proc/cpuinfo | grep 'processor' | wc -l`
echo "num of cpu:${CPU_COUNT}"
expr $CPU_COUNT "+" 0 > /dev/null 2>&1
if [ $? != 0 ]; then
  CPU_COUNT=4
fi
