#!/bin/sh

. "./incl.sh"

yum -y install make &&\
yum -y install zip unzip &&\
yum -y install gcc-c++ &&\
yum -y install ncurses-devel &&\
yum -y install openssl-devel &&\
yum -y install curl-devel &&\
yum -y install cmake &&\
yum -y install epel-release && yum -y install cmake3
