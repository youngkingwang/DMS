#!/bin/bash
LC_ALL=C ifconfig ens32 | grep inet|grep -v inet6 | awk '{print $2}'
