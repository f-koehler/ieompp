#!/bin/bash

if [ ! -d ramdisk/build ]; then
    echo "ERROR: ramdisk does not seem to exist; run create-ramdisk.sh"
    exit 1
fi

rsync \
    --recursive \
    --relative \
    --perms \
    --executability \
    --acls \
    --xattrs \
    --owner \
    --group \
    --times \
    --prune-empty-dirs \
    --exclude=".git" \
    --filter="dir-merge,-n /.gitignore" \
    --progress \
    . ramdisk
