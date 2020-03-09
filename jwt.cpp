/**
 * @file jwt.cpp
 * @author Lochemem Bruno Michael (lochbm@gmail.com)
 * @brief 
 * @version 0.1.0
 */
#include <jwt-cpp/jwt.h>
#include "jwt.h"

using namespace std;
using namespace jwt;

using timepoint = chrono::time_point<chrono::system_clock>;

/**
 * @brief 
 * 
 * @tparam B
 * @tparam L
 * @tparam S  
 * @param obj   jwt::builder object
 * @param algo  jwt algorithm
 * @param S     jwt secret key 
 * @return string  
 */
template <typename B, typename L, typename S>
auto signJwt(B obj, L algo, const S &secret) -> S
{
    string token("");

    switch (algo)
    {
    case JWT_ALGO_HS256:
    default:
        token += obj.sign(algorithm::hs256{secret});
        break;

    case JWT_ALGO_HS384:
        token += obj.sign(algorithm::hs384{secret});
        break;

    case JWT_ALGO_HS512:
        token += obj.sign(algorithm::hs512{secret});
        break;

    case JWT_ALGO_PS256:
        token += obj.sign(algorithm::ps256("", secret, "", ""));
        break;

    case JWT_ALGO_PS384:
        token += obj.sign(algorithm::ps384("", secret, "", ""));
        break;

    case JWT_ALGO_PS512:
        token += obj.sign(algorithm::ps512("", secret, "", ""));
        break;

    case JWT_ALGO_RS256:
        token += obj.sign(algorithm::rs256("", secret, "", ""));
        break;

    case JWT_ALGO_RS384:
        token += obj.sign(algorithm::rs384("", secret, "", ""));
        break;

    case JWT_ALGO_RS512:
        token += obj.sign(algorithm::rs512("", secret, "", ""));
        break;
    }

    return token;
}

/**
 * @brief 
 * 
 * @tparam B 
 * @tparam S 
 * @param obj 
 * @param claims 
 * @return B 
 */
template <typename B, typename S>
auto addClaims(B obj, S claims) -> B
{
    // convert string to C++ time
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

/**
 * @brief 
 * 
 * @tparam S 
 * @tparam L 
 * @tparam I 
 * @param secret 
 * @param algo 
 * @param claims 
 * @return S 
 */
template <typename S, typename L, typename I>
auto jwtEncode(const S &secret, L algo, I claims) -> S
{
    auto tokenObj = addClaims<builder, I>(create().set_type("JWS"),
                                          claims);

    return signJwt<builder, long, string>(tokenObj,
                                          algo,
                                          secret);
}

/**
 * @brief 
 * 
 * @tparam S 
 * @tparam L 
 * @param token 
 * @param algo 
 * @param secret 
 * @return strmap 
 */
template <typename S, typename L>
auto jwtDecode(const S &token, L algo, const S &secret) -> strmap
{
    auto decoded = decode(token);
    auto verifier = verify();
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

    case JWT_ALGO_RS256:
        verifier.allow_algorithm(algorithm::rs256(secret, "", "", ""));
        break;

    case JWT_ALGO_RS384:
        verifier.allow_algorithm(algorithm::rs384(secret, "", "", ""));
        break;

    case JWT_ALGO_RS512:
        verifier.allow_algorithm(algorithm::rs512(secret, "", "", ""));
        break;

    case JWT_ALGO_PS256:
        verifier.allow_algorithm(algorithm::ps256(secret, "", "", ""));
        break;

    case JWT_ALGO_PS384:
        verifier.allow_algorithm(algorithm::ps384(secret, "", "", ""));
        break;
    
    case JWT_ALGO_PS512:
        verifier.allow_algorithm(algorithm::ps512(secret, "", "", ""));
        break;
    }

    verifier.verify(decoded);

    for (auto &iter : decoded.get_payload_claims())
    {
        claims[iter.first] = iter.second.to_json().to_str();
    }

    return claims;
}
