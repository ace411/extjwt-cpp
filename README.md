# extjwt-cpp

A simple PHP extension for encoding and decoding JSON Web Tokens (JWTs). 

## Requirements

- OpenSSL
- [gtest](https://github.com/google/googletest) (for jwt-cpp)
- [jwt-cpp](https://github.com/Thalhammer/jwt-cpp) package
- PHP 7 or greater

**Note:** Only POSIX Operating Systems are currently supported.

## Supported Algorithms

- HS256
- HS384
- HS512
- RS256
- RS384
- RS512
- PS256
- PS384
- PS512

## Installation

extjwt-cpp requires tools specified in the [precursory text](#requirements). Once said tools are successfully installed on your system, type the following in a console to install extjwt-cpp on your machine.

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

> Do not forget to enable the extension by adding `extension=extjwt` to your `php.ini` file.

## Usage

Shown below is an example of how to encode and decode a JWT using the default HS256 signature, a payload (claims), and a secret.

```php
const SECRET = '@loki';
const CLAIMS = [
    'iss'       => 'https://github.com/ace411/extjwt-cpp',
    'aud'       => 'https://github.com/ace411/extjwt-cpp',
    'user'      => [
        'github'    => '@ace411',
        'twitter'   => '@agiroLoki'
    ],
    'cpp_ver'   => 11
];

$token = jwt_encode(SECRET, CLAIMS); // outputs a string token

$claims = jwt_decode($token, SECRET); // outputs a PHP hashtable
```

**The supported algorithm constants** are:

- ```JWT_ALGO_HS256```
- ```JWT_ALGO_HS384```
- ```JWT_ALGO_HS512```
- ```JWT_ALGO_RS256```
- ```JWT_ALGO_RS384```
- ```JWT_ALGO_RS512```
- ```JWT_ALGO_PS256```
- ```JWT_ALGO_PS384```
- ```JWT_ALGO_PS512```

## API

- **`jwt_encode`**

~~~
jwt_encode(string $secret, array $claims, int $algorithm)
~~~

**Description:** This function creates a JSON Web Token.

**Since:**

- v0.1.0

**Argument(s):**

- ***secret (string)*** - A discretionary secret key essential for JWT encipherment
> For algorithms prefixed with PS and RS, this is the private key

- ***claims (array)*** - The JWT payload; a combination of registered, public, and private claims

- ***algorithm (int)*** - The JWT signature algorithm

**Usage:**

~~~php
const RSA_PRIVATE_KEY = <<<'KEY'
-----BEGIN RSA PRIVATE KEY-----
MIICXAIBAAKBgQC8kGa1pSjbSYZVebtTRBLxBz5H4i2p/llLCrEeQhta5kaQu/Rn
vuER4W8oDH3+3iuIYW4VQAzyqFpwuzjkDI+17t5t0tyazyZ8JXw+KgXTxldMPEL9
5+qVhgXvwtihXC1c5oGbRlEDvDF6Sa53rcFVsYJ4ehde/zUxo6UvS7UrBQIDAQAB
AoGAb/MXV46XxCFRxNuB8LyAtmLDgi/xRnTAlMHjSACddwkyKem8//8eZtw9fzxz
bWZ/1/doQOuHBGYZU8aDzzj59FZ78dyzNFoF91hbvZKkg+6wGyd/LrGVEB+Xre0J
Nil0GReM2AHDNZUYRv+HYJPIOrB0CRczLQsgFJ8K6aAD6F0CQQDzbpjYdx10qgK1
cP59UHiHjPZYC0loEsk7s+hUmT3QHerAQJMZWC11Qrn2N+ybwwNblDKv+s5qgMQ5
5tNoQ9IfAkEAxkyffU6ythpg/H0Ixe1I2rd0GbF05biIzO/i77Det3n4YsJVlDck
ZkcvY3SK2iRIL4c9yY6hlIhs+K9wXTtGWwJBAO9Dskl48mO7woPR9uD22jDpNSwe
k90OMepTjzSvlhjbfuPN1IdhqvSJTDychRwn1kIJ7LQZgQ8fVz9OCFZ/6qMCQGOb
qaGwHmUK6xzpUbbacnYrIM6nLSkXgOAwv7XXCojvY614ILTK3iXiLBOxPu5Eu13k
eUz9sHyD6vkgZzjtxXECQAkp4Xerf5TGfQXGXhxIX52yH+N2LtujCdkQZjXAsGdm
B2zNzvrlgRmgBrklMTrMYgm1NPcW+bRLGcwgW2PTvNM=
-----END RSA PRIVATE KEY-----
KEY;

$token = jwt_encode([
    'user'      => 'ace411',
    'twitter'   => '@agiroLoki',
    'iss'       => 'example.org',
    'aud'       => 'example.org',
], RSA_PRIVATE_KEY, JWT_ALGO_RS256);

echo $token;
~~~

- **`jwt_decode`**

~~~
jwt_decode(string $token, string $secret, int $algorithm)
~~~

**Description:** This function decodes a JSON Web Token into a list of claims.

**Since:**

- v0.1.0

**Argument(s):**

- ***token (string)*** - The JWT to decode

- ***secret (string)*** - A discretionary secret key essential for JWT encipherment
> For algorithms prefixed with PS and RS, this is the public key

- ***algorithm (int)*** - The JWT signature algorithm

**Usage:**

~~~php
// combine with previous snippet
const RSA_PUBLIC_KEY = <<<'KEY'
-----BEGIN PUBLIC KEY-----
MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQC8kGa1pSjbSYZVebtTRBLxBz5H
4i2p/llLCrEeQhta5kaQu/RnvuER4W8oDH3+3iuIYW4VQAzyqFpwuzjkDI+17t5t
0tyazyZ8JXw+KgXTxldMPEL95+qVhgXvwtihXC1c5oGbRlEDvDF6Sa53rcFVsYJ4
ehde/zUxo6UvS7UrBQIDAQAB
-----END PUBLIC KEY-----
KEY;

$claims = jwt_decode($token, RSA_PUBLIC_KEY, JWT_ALGO_RS256);

print_r($claims);
~~~

## Why install extjwt-cpp?

PHP extensions, are by virtue of being C/C++ syntaxes, faster than regular PHP. extjwt is a PHP module which wraps around another C++ library and infuses robustness into the PHP userland. Although it is possible to craft high-quality idiomatic PHP solutions like [php-jwt](https://github.com/firebase/php-jwt), extjwt is more performant.

## Contributing

Consider buying me a coffee if you appreciate the offerings of project.

<a href="https://www.buymeacoffee.com/agiroLoki" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/lato-white.png" alt="Buy Me A Coffee" style="height: 51px !important;width: 217px !important;" ></a>
