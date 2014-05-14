#!/bin/sh

/sbin/ifconfig | grep 'eth0' | tr -s ' ' | cut -d ' ' -f5 |  tr -d ':'
