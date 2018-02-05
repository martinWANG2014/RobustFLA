########################################################################
# this script to update the makefile on the server.
########################################################################
#!/usr/bin/env bash
cmake_dir='../cmake-build-debug'
if [[ -e  ${cmake_dir}/CMakeFiles ]]; then
    rm -rf ${cmake_dir}/CMakeFiles
fi
if [[ -e ${cmake_dir}/CMakeCache.txt ]]; then
    rm ${cmake_dir}/CMakeCache.txt
fi
if [[ -e ${cmake_dir}/RobustFLA.cbp ]]; then
    rm ${cmake_dir}/RobustFLA.cbp
fi
# cmake_path_old=/etc/opt/clion-2017.2.3/bin/cmake/bin/
# cmake_path_new=cmake
# source_path_old=/home/chenghaowang/CLionProjects/RobustFLA
# source_path_new=~/RobustFLA
sed -i -e 's|/etc/opt/clion-2017.2.3/bin/cmake/bin/||g' ${cmake_dir}/Makefile
sed -i -e 's|/home/chenghaowang/CLionProjects|~|g' ${cmake_dir}/Makefile
sed -i -e 's|/home/chenghaowang/CLionProjects|~|g' ${cmake_dir}/cmake_install.cmake

cd ../cmake-build-debug
make
