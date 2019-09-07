#ifndef JWT_H
#define JWT_H

#include <map>
#include <string>

using namespace std;

#define JWT_ALGO_HS256 1
#define JWT_ALGO_HS384 2
#define JWT_ALGO_HS512 3

typedef map<string, string> strmap;

#endif /* JWT_H */