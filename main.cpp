#include <phpcpp.h>
#include <jwt-cpp/jwt.h>
#include <string>

using namespace std;

string jwths256 = "HS256";
string jwths384 = "HS384";
string jwths512 = "HS512";

//jwtEncode :: Secret -> Algo -> [Claims] -> Token
Php::Value jwtEncode(Php::Parameters &params)
{
    string secret = params[0];
    string algo = params[1];
    Php::Value claims = params[2];
    string _token;

    auto token = jwt::create()
        .set_type("JWS")
        .set_algorithm(algo);

    for (auto &iter : claims) 
    {
        string payloadClaim = iter.second;
        token.set_payload_claim(iter.first, payloadClaim);
    }

    if (algo == "HS256")
    {
        _token = token.sign(jwt::algorithm::hs256{secret});
    }
    else if (algo == "HS384")
    {
        _token = token.sign(jwt::algorithm::hs384{secret});
    }
    else if (algo == "HS512")
    {
        _token = token.sign(jwt::algorithm::hs512{secret});
    } else 
    {
        throw Php::Exception("Invalid Algorithm. Supported algorithms are HS256, HS384, and HS512.");
    }
    return _token;
}

//jwtDecode :: Token -> Secret -> Algo -> [Claims]
Php::Value jwtDecode(Php::Parameters &params)
{
    string _jwt = params[0];
    string secret = params[1];
    string algo = params[2];
    Php::Value claims;

    try {
        auto decoded = jwt::decode(_jwt);
        auto verifier = jwt::verify();

        if (algo == "HS256")
        {
            verifier.allow_algorithm(jwt::algorithm::hs256{secret});
        }
        else if (algo == "HS384")
        {
            verifier.allow_algorithm(jwt::algorithm::hs384{secret});
        }
        else if (algo == "HS512")
        {
            verifier.allow_algorithm(jwt::algorithm::hs512{secret});
        }
        else
        {
            throw Php::Exception("Invalid Algorithm. Supported algorithms are HS256, HS384, and HS512.");
        }

        for (auto &iter : decoded.get_payload_claims())
        {
            claims[iter.first] = iter.second.to_json().to_str();
        }
        return claims;
    }
    catch (const std::exception &e)
    {
        throw Php::Exception(e.what());
    }
}

extern "C" {
    PHPCPP_EXPORT void *get_module() 
    {
        static Php::Extension extension("extjwt_cpp", "1.0");

        extension.onStartup([=]() {
            Php::define("JWT_ALGO_HS256", jwths256);
            Php::define("JWT_ALGO_HS384", jwths384);
            Php::define("JWT_ALGO_HS512", jwths512);
        });

        extension.add<jwtEncode>("jwt_encode", {
            Php::ByVal("secret", Php::Type::String, true),
            Php::ByVal("algorithm", Php::Type::String, true),
            Php::ByVal("claims", Php::Type::Array, true)
        });

        extension.add<jwtDecode>("jwt_decode", {
            Php::ByVal("token", Php::Type::String, true),
            Php::ByVal("secret", Php::Type::String, true),
            Php::ByVal("algorithm", Php::Type::String, true)
        });

        return extension;
    }
}