#ifndef PHP_EXTJWT_H
#define PHP_EXTJWT_H

#define PHP_EXTJWT_EXTNAME "extjwt_cpp"
#define PHP_EXTJWT_EXTVER "0.1.0"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

extern "C"
{
#include "php.h"
#include "zend_smart_str.h"
#include "zend_exceptions.h"
#include "ext/json/php_json.h"
#include "ext/standard/info.h"
#include "zend_exceptions.h"
}

extern zend_module_entry extjwt_module_entry;
#define phpext_extjwt_ptr &extjwt_module_entry

#endif /* PHP_EXTJWT_H */