#!/bin/sh

export BASE_DIR="`dirname $0`"
$CUTTER -s $BASE_DIR "$@" $BASE_DIR
