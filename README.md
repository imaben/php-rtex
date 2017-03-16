# PHP Runtime Extended

## Build

```shell
$ git clone https://github.com/imaben/php-rtex.git
$ cd php-rtex
$ phpize
$ ./configure
$ make 
$ sudo make install
```

## php.ini
```ini
[rtex]
extension=rtex.so
```

## Functions

### rtex_class_rename

Rename the defined class

```
bool rtex_class_rename(string $old, string $new)
```

**Params**

- `$old`：The old class name
- `$new`：The new class name

```php
<?php
class A
{
    public function foo()
    {
        echo 'hello ' . get_class();
    }
}
$a = new A;
$a->foo();			// 'hello A'

rtex_class_rename('A', 'B');

class_exists('A');		//false

echo get_class($a);		// B
$a->foo();			// 'hello B'

```
