#include <iostream>
#include <ctype.h>
#include <string.h>

#include "reconnect/producer.h"


int main() {
    std::cout << "Hello, Kafka!" << std::endl;

    char test_data[100];
    strcpy(test_data, "helloworld");


    IMsgProducer *producer = ZKafkaProducer::GetInstance();

    char *addr = (char *) "172.17.0.3:9092";
    char *topic = (char *) "mykafka";
    int ret = 0;
    ret = producer->init(topic, addr, 0);
    if (ret != 0) {
        printf("init error");
        return ret;
    }

    if (!producer->Connect()) {
        printf("Connect error");
        return ret;
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
