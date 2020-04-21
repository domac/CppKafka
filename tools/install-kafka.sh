#!/bin/sh
CUR_DIR=`pwd`
MQ_DIR=librdkafka-1.4.0
MQ_PACKAGE=${MQ_DIR}.tar.gz

. "$CUR_DIR/incl.sh"

build_kafka() {
  mkdir -p ${CUR_DIR}/lib
  rm -rf ${MQ_DIR}
  tar xvf ${MQ_PACKAGE}
  if [ $? != 0 ]; then
    echo "tar:${MQ_PACKAGE} get error"
    exit 1
  fi

  cd ${MQ_DIR} && ./configure --prefix=${CUR_DIR}/lib/librdkafka  && make -j${CPU_COUNT} && make install

  mv ${CUR_DIR}/lib/librdkafka/include/librdkafka/* ${CUR_DIR}/lib/librdkafka/include
  rm -rf ${CUR_DIR}/lib/librdkafka/include/librdkafka
}
build_kafka