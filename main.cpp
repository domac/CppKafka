#include <iostream>
#include <ctype.h>
#include <string.h>

#include "reconnect/producer.h"
#include "reconnect/consumer.h"


int makeProducer() {
    std::cout << "Hello, Producer!" << std::endl;

    char test_data[100];
    strcpy(test_data, "helloworld");


    IMsgProducer *producer = ZKafkaProducer::GetInstance();

    //172.17.0.3
//    char *addr = (char *) "172.20.0.3:9092";
//    char *user = (char *) "admin";

    char *addr = (char *) "172.17.0.3:9092";
    char *user = NULL;

    char *topic = (char *) "fast_msg_06";
    char *pwd = (char *) "admin123";

    if (!producer->Connect(topic, addr, 0, user, pwd)) {
        printf("Connect error");
        return -1;
    }

    while (fgets(test_data, sizeof(test_data), stdin)) {

        printf("push data >>> %s", test_data);

        size_t len = strlen(test_data);
        if (test_data[len - 1] == '\n')
            test_data[--len] = '\0';
        if (strcmp(test_data, "end") == 0)
            break;
        if (0 == producer->SendToMqDirect("", test_data, strlen(test_data)))
            printf("push data success %s\n", test_data);
        else
            printf("push data failed %s\n", test_data);
    }
    producer->Disconnect();

    return 0;
}


static void msg_consume(rd_kafka_message_t *rkmessage, void *opaque)
{
    printf("[MSG] %d: %s\n", (int)rkmessage->len, (char *)rkmessage->payload);
}

int makeConsumer() {
    std::cout << "Hello, Consumer!" << std::endl;

    IMsgConsumer* msgConsumer = ZKafkaConsumer::GetInstance();

    char* groupId = (char *)"my_group1";
    char *addr = (char *) "172.20.0.3:9092";
    char *topic = (char *) "mykafka";
    char *user = (char *) "admin";
    char *pwd = (char *) "admin123";
    char *partitions = (char *) "0";

    std::cout << "----------------" << std::endl;

    consumer_callback consumer_cb = msg_consume; //注册消息回调函数，用户可以自定义此函数
    void * param_cb = NULL; //param_cb=this;
    int ret = 0;

    ret = msgConsumer->init(topic, addr, partitions,user,pwd, groupId, consumer_cb, param_cb);
    if(ret != 0){printf("Error: kc.init(): ret=%d;\n", ret); return 0;}

    ret = msgConsumer->getMessage(); //从kafka服务器接收消息
    if(ret != 0){printf("Error: kc.getMessage(): ret=%d;\n", ret); return 0;}

    return 0;
}

int main() {
    //makeConsumer();
    makeProducer();
    return 0;
}
