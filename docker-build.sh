#!/bin/bash

WORKING=$PWD
NAME=`cmake -L -P config.cmake | grep MAIN_NAME | sed -e 's/^MAIN_NAME.*=\\(.*\\)$/\\1/g'`

FLAGS_UBUNTU_14_04=''
FLAGS_UBUNTU_14_04="$FLAGS_UBUNTU_14_04 --tag=$NAME-ubuntu-14.04"
FLAGS_UBUNTU_14_04="$FLAGS_UBUNTU_14_04 --file=docker/dockerfile-ubuntu-14.04"

docker build $FLAGS_UBUNTU_14_04 .

