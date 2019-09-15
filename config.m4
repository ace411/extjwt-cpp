PHP_ARG_ENABLE(extjwt, whether to enable extjwt support,
[  --enable-extjwt          Enable extjwt support], no)

PHP_ARG_WITH(jwtcpp, whether to add jwtcpp library path,
[  --with-jwtcpp[=DIR]      Ignore jwtcpp library path check])

PHP_ARG_WITH(openssl, whether to add openssl library path,
[  --with-openssl=[=DIR]    Ignore openssl library path check])

if test "$PHP_EXTJWT" != "no"; then
    HEADER_INSTALL_DIRS="/usr/local /usr /usr/local/opt"
    JWT_CPP_INCLUDE_DIR="include/jwt-cpp"
    OPENSSL_INCLUDE_DIR="include/openssl"
    JWT_CPP_HEADER="jwt.h"
    OPENSSL_HEADER="ssl.h"

    dnl check if jwt-cpp extension is installed
    AC_MSG_CHECKING([for jwt-cpp library])
    if test -s $PHP_JWTCPP/$JWT_CPP_HEADER; then
        AC_MSG_RESULT(found $JWT_CPP_HEADER)
        JWTCPP_LIB=$PHP_JWTCPP/$JWT_CPP_HEADER
    else
        for iter in $HEADER_INSTALL_DIRS; do
            if test -s $iter/$JWT_CPP_INCLUDE_DIR/$JWT_CPP_HEADER; then
                AC_MSG_RESULT(found $JWT_CPP_HEADER)
                JWTCPP_LIB=$iter/$JWT_CPP_INCLUDE_DIR/$JWT_CPP_HEADER
            fi
        done
    fi        

    if test -z "$JWTCPP_LIB"; then
        AC_MSG_RESULT(jwt-cpp is not properly installed)
        AC_MSG_ERROR(Please install jwt-cpp)
    fi

    dnl check if openssl is installed; link ssl and crypto headers
    AC_MSG_CHECKING([for openssl])
    if test -s $PHP_OPENSSL/$OPENSSL_HEADER; then
        AC_MSG_RESULT(found $JWT_CPP_HEADER)
        OPENSSL_LIB=$PHP_OPENSSL/$OPENSSL_HEADER
        OPENSSL_DIR=$PHP_OPENSSL
    else
        for iter in $HEADER_INSTALL_DIRS; do
            if test -s $iter/$OPENSSL_INCLUDE_DIR/$OPENSSL_HEADER; then
                AC_MSG_RESULT(found $OPENSSL_HEADER)
                OPENSSL_LIB=$iter/$OPENSSL_INCLUDE_DIR/$OPENSSL_HEADER
                OPENSSL_DIR=$iter
            fi
        done
    fi

    if test -z "$OPENSSL_LIB"; then
        AC_MSG_RESULT(openssl is not properly installed)
        AC_MSG_ERROR(Please install openssl)
    fi

    PHP_ADD_INCLUDE($OPENSSL_DIR/include)
    AC_CHECK_HEADERS([openssl/$OPENSSL_HEADER openssl/crypto.h])

    PHP_CHECK_LIBRARY(ssl, SSL_get_options, 
    [
        PHP_ADD_INCLUDE($OPENSSL_DIR/include)
        PHP_ADD_LIBRARY_WITH_PATH(ssl, $OPENSSL_DIR/lib, EXTJWT_SHARED_LIBADD)
    ],[
        AC_MSG_ERROR(Invalid openssl version installed)
    ],[
        -L$OPENSSL_DIR/lib -lssl
    ])

    PHP_CHECK_LIBRARY(crypto, EVP_sha256, 
    [
        PHP_ADD_INCLUDE($OPENSSL_DIR/include)
        PHP_ADD_LIBRARY_WITH_PATH(crypto, $OPENSSL_DIR/lib, EXTJWT_SHARED_LIBADD)
    ],[
        AC_MSG_ERROR(Invalid openssl version installed)
    ],[
        -L$OPENSSL_DIR/lib -lcrypto
    ])

    CXXFLAGS="-std=c++11 -lssl -lcrypto"
    PHP_REQUIRE_CXX()
    PHP_SUBST(EXTJWT_SHARED_LIBADD)
    PHP_ADD_LIBRARY(stdc++, 1, EXTJWT_SHARED_LIBADD)
    PHP_NEW_EXTENSION(extjwt, extjwt.cpp jwt.cpp, $ext_shared)
fi