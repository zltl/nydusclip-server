
URL=https://github.com/jtv/libpqxx/archive/refs/tags/7.7.4.zip
FILE_NAME=libpqxx-7.7.4.zip
SHA256=6a1aba33b08448795c621bcb36949158eb4a7c8db2946be4701700b6210ec985
FILE_PATH=${DOWNLOAD_DIR}/${FILE_NAME}
SOURCE_FOLDER=${R_TARGET_DIR}/libpqxx-7.7.4

d_check_rebuild() {
    if [ -f ${DEPS_INSTALL_DIR}/include/pqxx/pqxx ]; then
        echo -n "already build"
    else
        echo -n ""
    fi
}

d_configure() {
    OLD_DIR=$PWD
    cd ${SOURCE_FOLDER}

    ./configure --prefix=${DEPS_INSTALL_DIR}

    cd $OLD_DIR
}


