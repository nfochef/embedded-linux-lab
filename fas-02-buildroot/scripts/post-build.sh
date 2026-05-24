#!/bin/sh
#
# post-build.sh — runs after all packages installed but before image is created
#
# This script removes the /var/log symlink (which points to tmpfs /tmp) and
# replaces it with a real directory on the persistent rootfs. This makes
# log files survive reboots.
#
# Argument: $1 = TARGET_DIR (the path to the rootfs being built)
#

TARGET_DIR="$1"

if [ -z "$TARGET_DIR" ]; then
    echo "ERROR: TARGET_DIR not provided"
    exit 1
fi

# Remove symlink, create real directory
if [ -L "$TARGET_DIR/var/log" ]; then
    echo "post-build: removing /var/log symlink, creating real directory"
    rm "$TARGET_DIR/var/log"
    mkdir -p "$TARGET_DIR/var/log"
    chmod 0755 "$TARGET_DIR/var/log"
fi

exit 0
