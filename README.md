# extjwt-cpp

A simple PHP extension for encoding and decoding JSON Web Tokens (JWTs). 

## Requirements

- OpenSSL
- [gtest](https://github.com/google/googletest) (for jwt-cpp)
- [jwt-cpp](https://github.com/Thalhammer/jwt-cpp) package
- PHP 7 or greater

## Supported Algorithms

- HS256
- HS384
- HS512

## Installation

Because ext-func is an extension built on top of PHP-CPP, installation of PHP-CPP is a mandatory prerequisite for using this tool. Also requirements are OpenSSL, and jwt-cpp. 

Once said tools are successfully installed on your system, type the following in a console to install extjwt-cpp on your machine.

```sh
git clone https://github.com/ace411/extjwt-cpp.git extjwt
cd extjwt
phpize
./configure --enable-extjwt --with-jwtcpp=/path/to/jwtcpp --with-openssl=/path/to/openssl
make && sudo make install
```

If you intend to run the tests in the tests directory, run the following command:

```sh
make test
```


## Usage

### Encoding a JWT

Creating a cryptographically signed token from arbitrary claims.

```php
const CLAIMS = [
    'iss' => 'https://github.com/ace411/extjwt-cpp',
    'aud' => 'https://github.com/ace411/extjwt-cpp',
    'user' => 'ace411'
];

$token = jwt_encode('@loki', JWT_ALGO_HS512, CLAIMS);
```

### Decoding a JWT

Decoding a JWT - print a hash table with encoded claims.

```php
...
$claims = jwt_decode($token, '@loki', JWT_ALGO_HS512);

assert($claims == CLAIMS, 'Claims are not the same');
```

**The supported algorithm constants** are:

- ```JWT_ALGO_HS256```
- ```JWT_ALGO_HS384```
- ```JWT_ALGO_HS512```

## Note

The extjwt-cpp extension is merely a wrapper around a C++ package. Because PHP has robust OpenSSL bindings - it is possible to use one of either a tailor-made or off-shelf PHP userland implementation of JWT encoding-decoding software (tools like [php-jwt](https://github.com/firebase/php-jwt) come to mind). 