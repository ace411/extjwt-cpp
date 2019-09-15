<?php

const TOKEN_SECRET          = 'extjwt';

const TOKEN_CLAIMS          = [
    'github'    => '@ace411',
    'twitter'   => '@agiroLoki'
];

const TOKEN_CLAIMS_MULTIDIM = [
    'social'    => TOKEN_CLAIMS,
    'location'  => 'kampala'
];