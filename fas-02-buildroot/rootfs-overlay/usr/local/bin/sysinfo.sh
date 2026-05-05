#!/bin/sh
echo "=== System Info ==="
echo "Hostname:     $(hostname)"
echo "Kernel:       $(uname -r)"
echo "Architecture: $(uname -m)"
echo "Uptime:       $(uptime)"
echo ""
echo "Memory:"
free -h | head -2
echo ""
echo "Disk:"
df -h / | head -2
