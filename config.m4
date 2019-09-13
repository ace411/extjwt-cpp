PHP_ARG_ENABLE(extjwt, whether to enable extjwt support,
dnl Make sure that the comment is aligned:
[  --enable-extjwt          Enable extjwt support], no)

if test "$PHP_EXTJWT" != "no"; then
    JWT_CPP_INSTALL_DIR="/usr/include/jwt-cpp"
    JWT_CPP_HEADERS="jwt.h picojson.h base.h"

    for iter in $JWT_CPP_HEADERS; do
        if test -s $JWT_CPP_INSTALL_DIR/$iter; then
            AC_MSG_RESULT($iter.h exists)
        else
            AC_MSG_ERROR($iter.h is missing. jwt-cpp is not properly installed)
        fi
    done

    CXXFLAGS="-std=c++11 -lssl -lcrypto"
    PHP_REQUIRE_CXX()
    PHP_SUBST(EXTJWT_SHARED_LIBADD)
    PHP_ADD_LIBRARY(stdc++, 1, EXTJWT_SHARED_LIBADD)
    PHP_NEW_EXTENSION(extjwt, extjwt.cpp jwt.cpp, $ext_shared)
fi