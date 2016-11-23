#!/bin/zsh
[[ ! -d ramdisk ]] && mkdir -p ramdisk

USER_ID=`id -u`
GROUP_ID=`id -g`

sudo mount -t tmpfs -o rw,size=2G,uid=$USER_ID,gid=$GROUP_ID none ramdisk
sudo -k

mkdir -p ramdisk/build
