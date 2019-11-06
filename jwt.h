#ifndef JWT_H
#define JWT_H

#include <map>
#include <string>
#include <chrono>

using namespace std;

#define JWT_ALGO_HS256 1
#define JWT_ALGO_HS384 2
#define JWT_ALGO_HS512 3
#define JWT_CLAIM_REPRESENT                  \
    map<string, long>                        \
    {                                        \
        {"iat", 1}, {"nbf", 2}, { "exp", 3 } \
    }

typedef map<string, string> strmap;

#endif /* JWT_H */