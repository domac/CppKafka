#!/bin/sh
module=reconnect
g++  -g -O2 -fPIC  -I/root/tools/tools/lib/librdkafka/include main.cpp -o ${module}_producer ${module}/consumer.cpp ${module}/producer.cpp ${module}/consumer.h ${module}/producer.h base/lock.h base/lock.cpp base/singleton.h  /root/tools/tools/lib/librdkafka/lib/librdkafka.a -lm -lssl -lcrypto   -lcrypto   -lz   -ldl -lpthread -lrt -lstdc++