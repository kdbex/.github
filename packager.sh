#! /usr/bin/bash
build_dir="$(pwd)/build"
rm -rf $build_dir
mkdir $build_dir
cp node.exe nssm.exe installer.bat server.bat "${build_dir}"
cd "../extension"
quasar build -m bex > /dev/null
cp -r "dist/bex/." ${build_dir}
cd "../server"
npm run build > /dev/null
cd $build_dir
rm *.zip
cd ..
gcc -Wall -Werror kdbex.c -o ${build_dir}/kdbex.exe -lshlwapi
tar -cf build.tar -C $build_dir .