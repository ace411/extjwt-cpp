#include "php_extjwt.h"
#include "jwt.cpp"

#ifdef HAVE_SPL
#include "ext/spl/spl_exceptions.h"
#endif

zend_class_entry *jwt_exception_ce;

template <typename L>
auto algoIsValid(L algo) -> bool
{
    switch (algo)
    {
    case JWT_ALGO_HS256:
    case JWT_ALGO_HS384:
    case JWT_ALGO_HS512:
        return true;
        break;
    
    default:
        return false;
        break;
    }
}

PHP_FUNCTION(jwt_encode)
{
    zend_string *secret;
    long algo;
    zval *claims;

    zend_string *key;
    zval *claim;
    strmap jwtClaims;

    ZEND_PARSE_PARAMETERS_START(0, 3)
    Z_PARAM_STR(secret)
    Z_PARAM_LONG(algo)
    Z_PARAM_ARRAY(claims)
    ZEND_PARSE_PARAMETERS_END();

    if (zend_hash_num_elements(HASH_OF(claims)) == 0 || 
        ZSTR_LEN(secret) == 0)
    {
        zend_string_release(secret);
        zend_throw_exception(jwt_exception_ce,
                             "Argument(s) cannot be empty",
                             0 TSRMLS_CC);
        RETURN_NULL();
    }

    if (algoIsValid<long>(algo) == false)
    {
        zend_string_release(secret);
        zend_throw_exception(jwt_exception_ce,
                             "Invalid algorithm detected",
                             0 TSRMLS_CC);
        RETURN_NULL();
    }

    ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(claims), key, claim) {
        convert_to_string(claim);
        
        jwtClaims[ZSTR_VAL(key)] = Z_STRVAL_P(claim);
    } ZEND_HASH_FOREACH_END();

    auto retval = jwtEncode<std::string, long, strmap>(std::string(ZSTR_VAL(secret)),
                                                        algo,
                                                        jwtClaims);
    RETURN_STRING(retval.c_str());
    zend_string_release(key);
    zend_string_release(secret);
}

PHP_FUNCTION(jwt_decode)
{
    zend_string *secret;
    zend_string *token;
    long algo;

    zval *retval;

    ZEND_PARSE_PARAMETERS_START(0, 3)
    Z_PARAM_STR(token)
    Z_PARAM_STR(secret)
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

        array_init(retval);
        zend_string_release(secret);
        zend_string_release(token);

        for (auto &iter : claims)
        {
            add_assoc_string(retval,
                             iter.first.c_str(),
                             iter.second.c_str());
        }

        RETURN_ZVAL(retval, 1, 0);
    }
    catch (const std::exception &exp)
    {
        zend_string_release(secret);
        zend_string_release(token);
        zend_throw_exception(jwt_exception_ce,
                             exp.what(),
                             0 TSRMLS_CC);
        RETURN_NULL();
    }
}

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

    REGISTER_LONG_CONSTANT("JWT_ALGO_HS256", JWT_ALGO_HS256, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("JWT_ALGO_HS384", JWT_ALGO_HS384, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("JWT_ALGO_HS512", JWT_ALGO_HS512, CONST_CS | CONST_PERSISTENT);

    return SUCCESS;    
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_jwt_encode, 0, 0, 3)
ZEND_ARG_INFO(0, secret)
ZEND_ARG_INFO(0, algo)
ZEND_ARG_INFO(0, claims)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_jwt_decode, 0, 0, 3)
ZEND_ARG_INFO(0, token)
ZEND_ARG_INFO(0, secret)
ZEND_ARG_INFO(0, algo)
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