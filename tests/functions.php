<?php

require_once 'constants.php';

/**
 * partial
 * creates a partially applied function
 * 
 * @param callable $function
 * @param mixed $args,...
 * 
 * @return callable
 */
function partial(callable $function, ...$args): callable
{
  return function (...$inner) use ($function, $args) {
    return $function(...array_merge($args, $inner));
  };
};
