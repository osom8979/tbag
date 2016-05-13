#!/bin/bash

SUFFIX=$1
TAG=$2
USE_VOLUME=false

if [[ -z $SUFFIX ]]; then
    SUFFIX=ubuntu-14.04
    TAG=
    #echo "Usage: $0 {suffix} {optional:tag}"
    #exit 1
fi

WORKING=$PWD
HOST_VOLUME=$WORKING/docker/vol
GUEST_VOLUME=/vol
NAME=`cmake -L -P config.cmake | grep MAIN_NAME | sed -e 's/^MAIN_NAME.*=\\(.*\\)$/\\1/g'`

if [[ -z $TAG ]]; then
MACHINE_NAME=$NAME-$SUFFIX
else
MACHINE_NAME=$NAME-$SUFFIX:$TAG
fi

FLAGS=''
FLAGS="$FLAGS --interactive=true"    # == -i
FLAGS="$FLAGS --tty=true"            # == -t
FLAGS="$FLAGS --rm"

# ENV settings.
#FLAGS="$FLAGS --memory=1024m"       # Memory limit
#FLAGS="$FLAGS --cpu-shares=1024"    # CPU shares (relative weight)
#FLAGS="$FLAGS --cpu-period=1000000" # Limit CPU CFS (Completely Fair Scheduler) period
#FLAGS="$FLAGS --cpu-quota=1000000"  # Limit CPU CFS (Completely Fair Scheduler) quota
#FLAGS="$FLAGS --cpuset-cpus=0"      # CPUs in which to allow execution (0-3, 0,1)

echo  "Machine name: $MACHINE_NAME"

# Volume settings.
case "$USE_VOLUME" in
[Tt]rue)
    FLAGS="$FLAGS --volume=$HOST_VOLUME:$GUEST_VOLUME"
    if [[ ! -d "$HOST_VOLUME" ]]; then
        mkdir "$HOST_VOLUME"
    fi
    echo "Host volume: $HOST_VOLUME"
    echo "Guest volume: $GUEST_VOLUME"
    ;;
*)
    echo "disable volume."
    ;;
esac

docker run $FLAGS $MACHINE_NAME

