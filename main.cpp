#include <iostream>
#include <ctype.h>
#include <string.h>

#include "simple/producer.h"


int main() {
    std::cout << "Hello, Kafka!" << std::endl;

    char test_data[100];
    strcpy(test_data, "helloworld");

    ProducerKafka *producer = new ProducerKafka;

    char *addr = (char *) "172.17.0.3:9092";
    char *topic = (char *) "mykafka";

    if (PRODUCER_INIT_SUCCESS == producer->init_kafka(0, addr, topic)) {
        printf("producer init success\n");
    } else {
        printf("producer init failed\n");
        return 0;
    }

    while (fgets(test_data, sizeof(test_data), stdin)) {

        printf("test_data >>> %s",test_data);

        size_t len = strlen(test_data);
        if (test_data[len - 1] == '\n')
            test_data[--len] = '\0';
        if (strcmp(test_data, "end") == 0)
            break;
        if (PUSH_DATA_SUCCESS == producer->push_data_to_kafka(test_data, strlen(test_data)))
            printf("push data success %s\n", test_data);
        else
            printf("push data failed %s\n", test_data);
    }

    producer->destroy();

    return 0;
}
