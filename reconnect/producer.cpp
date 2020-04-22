//
// Created by domac on 2020/4/22.
//

#include "producer.h"


MsgProducer::MsgProducer() : m_partition(0), m_kafka_handle(), m_kafka_conf(), m_kafka_topic(), m_kafka_topic_conf(),
                             m_kafka_topic_partition_list() {
}

MsgProducer::~MsgProducer() {

}

int MsgProducer::init(char *topic, char *brokers, int partition, char *username, char *pwd) {
    int ret = 0;
    rd_kafka_conf_res_t ret_conf = RD_KAFKA_CONF_OK;

    if (topic == NULL)
        return -1;

    if (brokers == NULL)
        return -1;

    m_partition = partition;
    m_brokers = brokers;
    m_topic = topic;
    char errstr[512] = {0};
    m_kafka_conf = rd_kafka_conf_new();


    /*SASL 权限配置*/

    if (username != NULL) {
        rd_kafka_conf_set(m_kafka_conf, "security.protocol", "sasl_plaintext", errstr,
                          sizeof(errstr));
        rd_kafka_conf_set(m_kafka_conf, "sasl.mechanisms", "PLAIN", errstr, sizeof(errstr));
        rd_kafka_conf_set(m_kafka_conf, "sasl.username", username, errstr,
                          sizeof(errstr));
        rd_kafka_conf_set(m_kafka_conf, "sasl.password", pwd, errstr,
                          sizeof(errstr));
        rd_kafka_conf_set(m_kafka_conf, "api.version.request", "true", errstr,
                          sizeof(errstr));
    }
    /*配置处理*/
    rd_kafka_conf_set_error_cb(m_kafka_conf, err_cb);
    rd_kafka_conf_set_throttle_cb(m_kafka_conf, throttle_cb);
    rd_kafka_conf_set_offset_commit_cb(m_kafka_conf, offset_commit_cb);
    rd_kafka_conf_set_stats_cb(m_kafka_conf, stats_cb);

    /*生产者配置*/
    ret_conf = rd_kafka_conf_set(m_kafka_conf, "queue.buffering.max.messages", "500000", errstr, sizeof(errstr));
    if (ret_conf != RD_KAFKA_CONF_OK) {
        printf("Error: rd_kafka_conf_set() failed 1; ret_conf=%d; errstr:%s\n", ret_conf, errstr);
        return -1;
    }

    ret_conf = rd_kafka_conf_set(m_kafka_conf, "message.send.max.retries", "3", errstr, sizeof(errstr));
    if (ret_conf != RD_KAFKA_CONF_OK) {
        printf("Error: rd_kafka_conf_set() failed 2; ret_conf=%d; errstr:%s\n", ret_conf, errstr);
        return -1;
    }

    ret_conf = rd_kafka_conf_set(m_kafka_conf, "retry.backoff.ms", "500", errstr, sizeof(errstr));
    if (ret_conf != RD_KAFKA_CONF_OK) {
        printf("Error: rd_kafka_conf_set() failed 3; ret_conf=%d; errstr:%s\n", ret_conf, errstr);
        return -1;
    }


    /*Topic配置*/
    m_kafka_topic_conf = rd_kafka_topic_conf_new();
    ret_conf = rd_kafka_topic_conf_set(m_kafka_topic_conf, "auto.offset.reset", "earliest", errstr, sizeof(errstr));
    if (ret_conf != RD_KAFKA_CONF_OK) {
        printf("Error: rd_kafka_conf_set() failed 4; ret_conf=%d; errstr:%s\n", ret_conf, errstr);
        return -1;
    }
    return ret;
}

bool MsgProducer::Connect() {
    //Disconnect();
    char errstr[512] = {0};
    printf("Connect....\n");
    /*创建kafka生产者句柄*/
    m_kafka_handle = rd_kafka_new(RD_KAFKA_PRODUCER, m_kafka_conf, errstr, sizeof(errstr));

    if (m_kafka_handle == NULL) {
        printf(">>> Error: Failed to create Kafka producer: %s\n", errstr);
        return false;
    }

    //---------Add broker(s)-------------------
    if (m_brokers && rd_kafka_brokers_add(m_kafka_handle, m_brokers) < 1) {
        printf(">>> Error: No valid brokers specified\n");
        return false;
    }

    m_kafka_topic = rd_kafka_topic_new(m_kafka_handle, m_topic, m_kafka_topic_conf);
    return true;
}

bool MsgProducer::Disconnect() {
    printf("Disconnect....\n");
    rd_kafka_topic_destroy(m_kafka_topic);
    rd_kafka_destroy(m_kafka_handle);
    return false;
}

int MsgProducer::SendToMqDirect(const std::string &strKey, const char *buffer, size_t buf_len) {
    printf("SendToKafka....\n");
    int ret = 0;
    if (NULL == buffer)
        return 0;

    //ret = rd_kafka_produce(m_kafka_topic, m_partition, RD_KAFKA_MSG_F_COPY | RD_KAFKA_MSG_F_FREE, buffer, buf_len, NULL, 0, NULL);
    ret = rd_kafka_produce(m_kafka_topic, m_partition, RD_KAFKA_MSG_F_COPY | RD_KAFKA_MSG_F_FREE, (void *) buffer,
                           buf_len, NULL, 0, NULL);

    if (ret == -1) {
        rd_kafka_resp_err_t err = rd_kafka_last_error();

        if (err == RD_KAFKA_RESP_ERR__UNKNOWN_PARTITION) {
            printf("Error: No such partition: %d\n", m_partition);
        } else {
            printf("Error: produce error: %s%s\n", rd_kafka_err2str(err),
                   err == RD_KAFKA_RESP_ERR__QUEUE_FULL ? " (backpressure)" : "");
        }

        rd_kafka_poll(m_kafka_handle, 10); //Poll to handle delivery reports
        ret = -2;
        goto end;
    }
    ret = rd_kafka_poll(m_kafka_handle, 0);
    end:
    return ret;
}


void MsgProducer::err_cb(rd_kafka_t *rk, int err, const char *reason, void *opaque) {
    printf(">>> %% ERROR CALLBACK: %s: %s: %s\n", rd_kafka_name(rk), rd_kafka_err2str((rd_kafka_resp_err_t) err),
           reason);
}

void MsgProducer::throttle_cb(rd_kafka_t *rk, const char *broker_name, int32_t broker_id, int throttle_time_ms,
                              void *opaque) {
    printf(">>> %% THROTTLED %dms by %s (%d)\n", throttle_time_ms, broker_name, broker_id);
}

void MsgProducer::offset_commit_cb(rd_kafka_t *rk, rd_kafka_resp_err_t err, rd_kafka_topic_partition_list_t *offsets,
                                   void *opaque) {
    int i;
    int verbosity = 1;

    if (err || verbosity >= 2) {
        printf(">>> %% Offset commit of %d partition(s): %s\n", offsets->cnt, rd_kafka_err2str(err));
    }

    for (i = 0; i < offsets->cnt; i++) {
        rd_kafka_topic_partition_t *rktpar = &offsets->elems[i];

        if (rktpar->err || verbosity >= 2) {
            printf(">>> %%  %s [%d] @ %d: %s\n", rktpar->topic, rktpar->partition, rktpar->offset,
                   rd_kafka_err2str(err));
        }
    }
}

int MsgProducer::stats_cb(rd_kafka_t *rk, char *json, size_t json_len, void *opaque) {
    printf(">>> stats_cb ==> %s\n", json);
    return 0;
}