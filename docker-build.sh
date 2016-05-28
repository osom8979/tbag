#!/bin/bash

WORKING=$PWD
NAME=`cat $WORKING/INFORMATION | grep NAME | sed -e 's/^NAME.*=\\(.*\\)$/\\1/g'`

FLAGS_UBUNTU_14_04=''
FLAGS_UBUNTU_14_04="$FLAGS_UBUNTU_14_04 --tag=$NAME-ubuntu-14.04"
FLAGS_UBUNTU_14_04="$FLAGS_UBUNTU_14_04 --file=docker/dockerfile-ubuntu-14.04"

docker build $FLAGS_UBUNTU_14_04 .

