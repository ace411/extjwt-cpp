/**
 * @file extjwt.cpp
 * @author Lochemem Bruno Michael (lochbm@gmail.com)
 * @brief 
 * @version 0.1.0
 */
#include "php_extjwt.h"
#include "jwt.cpp"

#ifdef HAVE_SPL
#include "ext/spl/spl_exceptions.h"
#endif

zend_class_entry *jwt_exception_ce;

/**
 * @brief 
 * 
 * @tparam L 
 * @param algo 
 * @return true 
 * @return false 
 */
template <typename L>
auto algoIsValid(L algo) -> bool
{
    switch (algo)
    {
    case JWT_ALGO_HS256:
    case JWT_ALGO_HS384:
    case JWT_ALGO_HS512:
    case JWT_ALGO_RS256:
    case JWT_ALGO_RS384:
    case JWT_ALGO_RS512:
    case JWT_ALGO_PS256:
    case JWT_ALGO_PS384:
    case JWT_ALGO_PS512:
        return true;
        break;

    default:
        return false;
        break;
    }
}

/**
 * @brief 
 * 
 * @tparam S 
 * @param str 
 * @return S 
 */
template <typename S>
auto ucFirst(const S &str) -> S
{
    string fst("");
    fst += toupper(str.at(0));
    fst += str.substr(1);

    return fst;
}

/* {{{ proto string jwt_encode(string secret, array claims, int algorithm)
   Returns a JSON Web Token */
PHP_FUNCTION(jwt_encode)
{
    zend_string *secret;
    long algo = JWT_ALGO_HS256; // default JWT encoding algorithm
    zval *claims;

    zend_string *key;
    zval *claim;
    strmap jwtClaims;

    ZEND_PARSE_PARAMETERS_START(2, 3)
    Z_PARAM_ARRAY(claims)
    Z_PARAM_STR(secret)
    Z_PARAM_OPTIONAL
    Z_PARAM_LONG(algo)
    ZEND_PARSE_PARAMETERS_END();

    // throw a PHP exception if one of either the payload or secret is empty
    if (zend_hash_num_elements(HASH_OF(claims)) == 0 ||
        ZSTR_LEN(secret) == 0)
    {
        zend_string_release(secret);
        zend_throw_exception(jwt_exception_ce,
                             "Argument(s) cannot be empty",
                             0 TSRMLS_CC);
        RETURN_NULL();
    }

    // throw a PHP exception if the algorithm is invalid
    if (algoIsValid<long>(algo) == false)
    {
        zend_string_release(secret);
        zend_throw_exception(jwt_exception_ce,
                             "Invalid algorithm detected",
                             0 TSRMLS_CC);
        RETURN_NULL();
    }

    // iterate through the list of claims
    ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(claims), key, claim)
    {
        // JSON encode all sub-array claims
        if (Z_TYPE_P(claim) == IS_ARRAY)
        {
            smart_str jsonData = {0};
            php_json_encode(&jsonData, claim, 0);
            smart_str_0(&jsonData);

            jwtClaims[ZSTR_VAL(key)] = ZSTR_VAL(jsonData.s);
            smart_str_free(&jsonData);
        }
        else
        {
            // coerce claim to string value
            convert_to_string(claim);

            jwtClaims[ZSTR_VAL(key)] = Z_STRVAL_P(claim);
        }
    }
    ZEND_HASH_FOREACH_END();

    auto retval = jwtEncode<std::string, long, strmap>(std::string(ZSTR_VAL(secret)),
                                                       algo,
                                                       jwtClaims);
    RETURN_STRING(retval.c_str());
    zend_string_release(key);
    zend_string_release(secret);
}
/* }}} */

/* {{{ proto array jwt_decode(string token, string secret, int algorithm) 
   Returns JSON Web Token payload */
PHP_FUNCTION(jwt_decode)
{
    zend_string *secret;
    zend_string *token;
    long algo(JWT_ALGO_HS256);

    zend_string *key;
    zval *claim;
    zval *retval;

    ZEND_PARSE_PARAMETERS_START(2, 3)
    Z_PARAM_STR(token)
    Z_PARAM_STR(secret)
    Z_PARAM_OPTIONAL
    Z_PARAM_LONG(algo)
    ZEND_PARSE_PARAMETERS_END();

    if (algoIsValid<long>(algo) == false)
    {
        zend_string_release(secret);
        zend_string_release(token);
        zend_throw_exception(jwt_exception_ce,
                             "Invalid algorithm detected",
                             0 TSRMLS_CC);
        RETURN_NULL();
    }

    try
    {
        auto claims = jwtDecode<std::string, long>(std::string(ZSTR_VAL(token)),
                                                   algo,
                                                   std::string(ZSTR_VAL(secret)));

        array_init(retval); // cast the return value as an array
        zend_string_release(secret);
        zend_string_release(token);

        for (auto &iter : claims)
        {
            // store each claim in the retval array as a string
            add_assoc_string(retval,
                             iter.first.c_str(),
                             iter.second.c_str());
        }

        // iterate through the retval array and resolve string claims to original uncoerced forms
        ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(retval), key, claim)
        {
            // copy claim zval to duplicate container
            zval dup;
            ZVAL_COPY_VALUE(&dup, claim);

            if (php_json_decode_ex(claim,
                                   Z_STRVAL_P(&dup),
                                   Z_STRLEN_P(&dup),
                                   PHP_JSON_OBJECT_AS_ARRAY,
                                   512) == FAILURE)
            {
                Z_STRVAL_P(&dup) == std::string("1").c_str() ? add_assoc_bool(retval,
                                                                              ZSTR_VAL(key),
                                                                              1)
                                                             : Z_STRLEN_P(&dup) == 0 ? add_assoc_bool(retval,
                                                                                                      ZSTR_VAL(key),
                                                                                                      0)
                                                                                     : add_assoc_string(retval,
                                                                                                        ZSTR_VAL(key),
                                                                                                        Z_STRVAL_P(&dup));
            }
        }
        ZEND_HASH_FOREACH_END();

        RETURN_ZVAL(retval, 1, 0);
    }
    catch (const std::exception &exp)
    {
        // convert C++ exception to PHP exception
        zend_string_release(secret);
        zend_string_release(token);
        zend_throw_exception(jwt_exception_ce,
                             ucFirst<std::string>(exp.what()).c_str(),
                             0 TSRMLS_CC);
        RETURN_NULL();
    }
}
/* }}} */

PHP_RINIT_FUNCTION(extjwt)
{
#if defined(ZTS) && defined(COMPILE_DL_EXTJWT)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif

    return SUCCESS;
}

PHP_MINFO_FUNCTION(extjwt)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "extjwt support", "enabled");
    php_info_print_table_header(2, "extjwt version", PHP_EXTJWT_EXTVER);
    php_info_print_table_header(2, "supported algorithms", "HS256, HS384, HS512");
    php_info_print_table_end();
}

PHP_MINIT_FUNCTION(extjwt)
{
    zend_class_entry ce;

    INIT_CLASS_ENTRY(ce, "ExtJwtException", NULL);

#ifdef HAVE_SPL
    jwt_exception_ce = zend_register_internal_class_ex(
        &ce, spl_ce_RuntimeException);
#else
    jwt_exception_ce = zend_register_internal_class_ex(
        &ce, zend_exception_get_default(TSRMLS_C));
#endif

    // register JWT algorithms as constants
    REGISTER_LONG_CONSTANT("JWT_ALGO_HS256", JWT_ALGO_HS256, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("JWT_ALGO_HS384", JWT_ALGO_HS384, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("JWT_ALGO_HS512", JWT_ALGO_HS512, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("JWT_ALGO_RS256", JWT_ALGO_RS256, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("JWT_ALGO_RS384", JWT_ALGO_RS384, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("JWT_ALGO_RS512", JWT_ALGO_RS512, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("JWT_ALGO_PS256", JWT_ALGO_PS256, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("JWT_ALGO_PS384", JWT_ALGO_PS384, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("JWT_ALGO_PS512", JWT_ALGO_PS512, CONST_CS | CONST_PERSISTENT);

    return SUCCESS;
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_jwt_encode, 0, 0, 3)
ZEND_ARG_ARRAY_INFO(0, claims, 0)
ZEND_ARG_INFO(0, secret)
ZEND_ARG_INFO(0, algorithm)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_jwt_decode, 0, 0, 3)
ZEND_ARG_INFO(0, token)
ZEND_ARG_INFO(0, secret)
ZEND_ARG_INFO(0, algorithm)
ZEND_END_ARG_INFO();

static const zend_function_entry extjwt_functions[] = {
    PHP_FE(jwt_encode, arginfo_jwt_encode)
        PHP_FE(jwt_decode, arginfo_jwt_decode)
            PHP_FE_END};

zend_module_entry extjwt_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_EXTJWT_EXTNAME,
    extjwt_functions,
    PHP_MINIT(extjwt),
    NULL,
    PHP_RINIT(extjwt),
    NULL,
    PHP_MINFO(extjwt),
    PHP_EXTJWT_EXTVER,
    STANDARD_MODULE_PROPERTIES};

#ifdef COMPILE_DL_EXTJWT
extern "C"
{
    ZEND_GET_MODULE(extjwt)
}
#endif
