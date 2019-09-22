#include <jwt-cpp/jwt.h>
#include "jwt.h"

using namespace std;
using namespace jwt;
using timepoint = chrono::time_point<chrono::system_clock>;

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

template <typename B, typename S>
auto addClaims(B obj, S claims) -> B
{
    auto strToTime = [](const string &timeval) {
        const chrono::system_clock::duration timeT = chrono::seconds{stoi(timeval)};
        const timepoint duration(timeT); 
        return duration; };

    for (auto &claim : claims)
    {
        switch (JWT_CLAIM_REPRESENT[claim.first]) 
        {
        case 1:
            obj.set_issued_at(strToTime(claim.second));
            break;

        case 2:
            obj.set_not_before(strToTime(claim.second));
            break;

        case 3:
            obj.set_expires_at(strToTime(claim.second));
            break;

        default:
            obj.set_payload_claim(claim.first, claim.second);
            break;
        }
    }

    return obj;
}

template <typename S, typename L, typename I>
auto jwtEncode(const S &secret, L algo, I claims) -> S
{
    auto tokenObj = addClaims<builder, I>(create().set_type("JWS"),
                                          claims);

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
    }

    verifier.verify(decoded);

    for (auto &iter : decoded.get_payload_claims())
    {
        claims[iter.first] = iter.second.to_json().to_str();
    }

    return claims;
}