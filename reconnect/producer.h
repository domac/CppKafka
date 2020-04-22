//
// Created by domac on 2020/4/22.
//

#ifndef CPPKAFKA_PRODUCER_H_RECONNECT
#define CPPKAFKA_PRODUCER_H_RECONNECT

#include "../base/singleton.h"
#include "rdkafka.h"
#include <string>

struct IMsgProducer
{
    virtual int init(char *topic, char *brokers, int partition) =0;
    virtual bool Connect()=0;
    virtual bool Disconnect()=0;
    virtual int SendToMqDirect(const std::string& strKey, const char* pData, size_t nDataLen)=0;
};

class MsgProducer : public IMsgProducer {

protected:
    MsgProducer();
    virtual ~MsgProducer();

public:
    virtual int init(char *topic, char *brokers, int partition);

    virtual bool Connect();
    virtual bool Disconnect();
    virtual int SendToMqDirect(const std::string& strKey, const char* pData, size_t nDataLen);

public:
    static void err_cb(rd_kafka_t *rk, int err, const char *reason, void *opaque);
    static void throttle_cb(rd_kafka_t *rk, const char *broker_name, int32_t broker_id, int throttle_time_ms, void *opaque);
    static void offset_commit_cb(rd_kafka_t *rk, rd_kafka_resp_err_t err, rd_kafka_topic_partition_list_t *offsets, void *opaque);
    static int stats_cb(rd_kafka_t *rk, char *json, size_t json_len, void *opaque);

private:
    int m_partition;
    char* m_brokers;
    char* m_topic;
    //rd
    rd_kafka_t *m_kafka_handle;
    rd_kafka_conf_t *m_kafka_conf;

    //topic
    rd_kafka_topic_t *m_kafka_topic;
    rd_kafka_topic_conf_t *m_kafka_topic_conf;
    rd_kafka_topic_partition_t *m_kafka_topic_partition_list;

};

class ZKafkaProducer: public MsgProducer, public base::Singleton<ZKafkaProducer>
{
    friend class base::Singleton<ZKafkaProducer>;

private:
    ZKafkaProducer(){};
    virtual ~ZKafkaProducer(){};
};

#endif //CPPKAFKA_PRODUCER_H
