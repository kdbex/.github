#! /usr/bin/bash
build_dir="$(pwd)/build"
rm -rf $build_dir
mkdir $build_dir
mkdir "${build_dir}/extension"
mkdir "${build_dir}/server"
cp node.exe nssm.exe installer.bat "${build_dir}"
cd "../extension"
quasar build -m bex > /dev/null
cp -r "dist/bex/." ${build_dir}/extension
cd "../server"
tsc
cp -r "dist/server/." ${build_dir}/server
cd $build_dir
cd ..
x86_64-w64-mingw32-gcc -Wall -Werror kdbex.c -o ${build_dir}/kdbex.exe
tar -cf build.tar -C $build_dir .