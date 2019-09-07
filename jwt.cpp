#include <initializer_list>
#include <jwt-cpp/jwt.h>
#include "jwt.h"

using namespace std;

typedef initializer_list<string> strList;

template <typename B, typename L, typename S>
auto signJwt(B obj, L algo, const S &secret) -> string
{
    string token("");

    switch (algo)
    {
    case JWT_ALGO_HS256:
        token += obj.sign(jwt::algorithm::hs256{secret});
        break;

    case JWT_ALGO_HS384:
        token += obj.sign(jwt::algorithm::hs384{secret});
        break;

    case JWT_ALGO_HS512:
        token += obj.sign(jwt::algorithm::hs512{secret});
        break;

    default:
        break;
    }

    return token;
}

template <typename S, typename L, typename I>
auto jwtEncode(const S &secret, L algo, I claims) -> S
{
    auto tokenObj = jwt::create()
                        .set_type("JWS");

    for (auto &iter : claims)
    {
        tokenObj.set_payload_claim(iter.first, iter.second);
    }

    if (claims.count("iss"))
    {
        tokenObj.set_issuer(claims["iss"]);
    }

    return signJwt<jwt::builder, long, string>(tokenObj,
                                               algo,
                                               secret);
}

template <typename S, typename L>
auto jwtDecode(const S &token, L algo, const S &secret) -> strmap
{
    auto decoded = jwt::decode(token), verifier = jwt::verify();
    strmap claims;

    switch (algo)
    {
    case JWT_ALGO_HS256:
        verifier.allow_algorithm(jwt::algorithm::hs256{secret});
        break;

    case JWT_ALGO_HS512:
        verifier.allow_algorithm(jwt::algorithm::hs512{secret});
        break;

    default:
        break;
    }

    for (auto &iter : decoded.get_payload_claims())
    {
        claims[iter.first] = iter.second.to_json().to_str();
    }

    return claims;
}