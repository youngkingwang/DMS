#!/bin/bash
#remark

if cp $1 $2 >&/dev/null
then
	exit -1
fi

