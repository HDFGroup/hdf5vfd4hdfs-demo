#!/usr/bin/bash
export CLASSPATH=`$HADOOP_HOME/bin/hadoop classpath --glob`
ARG0="$0"
EXEC_PATH="$( dirname "$ARG0" )"
"${EXEC_PATH}/hdfs-vfd-mapper" $@
