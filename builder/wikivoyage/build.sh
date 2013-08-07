#!/bin/bash

set -e -u -x

MY_PATH=`dirname $0`
BIN=$MY_PATH make -f $MY_PATH/wikivoyage.mk
