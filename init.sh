#!/bin/bash
echo "ConnectionPool Init"

# 下面脚本参考了AI
# 旨在保证项目可以正确编译

# 检查CMake是否已安装
command -v cmake >/dev/null 2>&1 || { echo >&2 "未找到CMake，请确保已安装CMake并添加到系统路径。"; exit 1; }

# 检查Git是否已安装
command -v git >/dev/null 2>&1 || { echo >&2 "未找到Git，请确保已安装Git并添加到系统路径。"; exit 1; }

# 检查GCC是否已安装
command -v gcc >/dev/null 2>&1 || { echo >&2 "未找到GCC，请确保已安装GCC并添加到系统路径。"; exit 1; }

# 设置vcpkg安装目录为当前目录
VCPKG_DIR="$(pwd)/vcpkg"

# 检查vcpkg是否已经安装
if [ ! -d "$VCPKG_DIR" ]; then
    echo "正在下载vcpkg..."
    git clone https://github.com/microsoft/vcpkg.git "$VCPKG_DIR"
    cd "$VCPKG_DIR"
    ./bootstrap-vcpkg.sh
    ./vcpkg integrate install
else
    echo "vcpkg已经安装，跳过下载步骤。"
fi

# 安装libmysql
echo "正在安装libmysql..."
"$VCPKG_DIR/vcpkg" install libmysql