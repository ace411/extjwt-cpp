/**
 * @file jwt.h
 * @author Lochemem Bruno Michael (lochbm@gmail.com)
 * @brief 
 * @version 0.1.0 
 */
#ifndef JWT_H
#define JWT_H

#include <map>
#include <string>
#include <chrono>
#include <tuple>

using namespace std;

#define JWT_ALGO_HS256 1
#define JWT_ALGO_HS384 2
#define JWT_ALGO_HS512 3
#define JWT_ALGO_RS256 4
#define JWT_ALGO_RS384 5
#define JWT_ALGO_RS512 6
#define JWT_ALGO_PS256 7
#define JWT_ALGO_PS384 8
#define JWT_ALGO_PS512 9

#define JWT_CLAIM_REPRESENT                  \
    map<string, long>                        \
    {                                        \
        {"iat", 1}, {"nbf", 2}, { "exp", 3 } \
    }

typedef map<string, string> strmap;

#endif /* JWT_H */
