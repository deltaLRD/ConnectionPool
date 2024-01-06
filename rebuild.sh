# 运行cmake编译
echo "正在编译项目..."
rm -r ./build
mkdir build
cmake -B ./build -S . -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build ./build

echo "编译完成！"