PHP_ARG_ENABLE(extjwt, whether to enable extjwt support,
dnl Make sure that the comment is aligned:
[  --enable-extjwt          Enable extjwt support], no)

if test "$PHP_EXTJWT" != "no"; then
    CXXFLAGS="-std=c++11 -lssl -lcrypto"
    PHP_REQUIRE_CXX()
    PHP_SUBST(EXTJWT_SHARED_LIBADD)
    PHP_ADD_LIBRARY(stdc++, 1, EXTJWT_SHARED_LIBADD)
    PHP_NEW_EXTENSION(extjwt, extjwt.cpp jwt.cpp, $ext_shared)
fi