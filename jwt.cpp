#include <jwt-cpp/jwt.h>
#include "jwt.h"

using namespace std;
using namespace jwt;

template <typename B, typename L, typename S>
auto signJwt(B obj, L algo, const S &secret) -> S
{
    string token("");

    switch (algo)
    {
    case JWT_ALGO_HS256:
        token += obj.sign(algorithm::hs256{secret});
        break;

    case JWT_ALGO_HS384:
        token += obj.sign(algorithm::hs384{secret});
        break;

    case JWT_ALGO_HS512:
        token += obj.sign(algorithm::hs512{secret});
        break;
    }

    return token;
}

template <typename S, typename L, typename I>
auto jwtEncode(const S &secret, L algo, I claims) -> S
{
    auto tokenObj = create()
                        .set_type("JWS");

    for (auto &iter : claims)
    {
        tokenObj.set_payload_claim(iter.first, iter.second);
    }

    return signJwt<builder, long, string>(tokenObj,
                                               algo,
                                               secret);
}

template <typename S, typename L>
auto jwtDecode(const S &token, L algo, const S &secret) -> strmap
{
    auto decoded = decode(token), verifier = verify();
    strmap claims;

    switch (algo)
    {
    case JWT_ALGO_HS256:
        verifier.allow_algorithm(algorithm::hs256{secret});
        break;

    case JWT_ALGO_HS384:
        verifier.allow_algorithm(algorithm::hs384{secret});
        break;

    case JWT_ALGO_HS512:
        verifier.allow_algorithm(algorithm::hs512{secret});
        break;

    default:
        break;
    }

    verifier.verify(decoded);

    for (auto &iter : decoded.get_payload_claims())
    {
        claims[iter.first] = iter.second.to_json().to_str();
    }

    return claims;
}