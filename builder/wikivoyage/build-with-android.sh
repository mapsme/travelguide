#!/bin/bash

set -e -u -x

MY_PATH=`dirname $0`
MYSQL_BINARY=mysql MYSQL_USER=root MYSQL_DATABASE=wikivoyage_build BIN=$MY_PATH make -f $MY_PATH/wikivoyage.mk make_android
