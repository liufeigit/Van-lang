# Van
Embeddable scripting language in C.

A simple and efficient script language.

### Overview
---
**`Van`**  is a script language, implement in ANSI C.

**`Van`** as a script language, it can be used as glue to decoupling complex business logic.

**`Van`** language features include  A concise type system，Object-oriented，Function Closure，Coroutine，etc.

**`Van`** language design purpose is to facilitate embedding and extend, to facilitate interaction with C, and to decouple complex business.

##### Notice
Currently in the state of development and testing，documentation and functionality are still being refined and the release version will be released in the near future.

##### Test Use
If you are interested, you can now test and use,submit suggestions and bugs.

usage

```sh
$ cd Van
$ chmod +x ./build.sh 
$ ./build.sh
```
now genrate `van` and `libvan.so` file.

you can use `van` interpreter `.van` file in `vanTest` directory, for example

```sh
$ ./van ./vanTest/van_assign.van
$ ./van ./vanTest/van_class.van
$ ./van ./vanTest/van_exp.van
$ ./van ./vanTest/van_function.van
$ ./van ./vanTest/van_if.van
...
```

you can  use `libvan.so` write language extend, if you interpreter `van_extend.van`,for example

```sh
$ clang -g -fPIC -shared  -L. -lvan ./External/extendTest1.c -o ./External/libextendTest1.so
$ clang -g -fPIC -shared  -L. -lvan ./External/extendTest2.c -o ./External/libextendTest2.so
```
now you can use extend `sub` and `sub2` in `.van` file


## Welcome to join
I look forward to your participation in **`Van`** language design and development.

``` 
____    ____  ___      .__   __. 
\   \  /   / /   \     |  \ |  | 
 \   \/   / /  ^  \    |   \|  | 
  \      / /  /_\  \   |  . `  | 
   \    / /  _____  \  |  |\   | 
    \__/ /__/     \__\ |__| \__| 
    
   Welcome to join Van project                                                  
```

If you have good ideas and suggestions,contact me , welcome to improve and transform `Van` language.

email: [woaicplusplus@gmail.com](mailto:woaicplusplus@gmail.com) or [18510248500@163.com](mailto:18510248500@163.com)
                                                           
### License
---
MIT License
> Copyright (c) 2019 liufei and `Van` project contributors
>
>  Permission is hereby granted, free of charge, to any person obtaining a copy
>  of this software and associated documentation files (the "Software"), to deal
>  in the Software without restriction, including without limitation the rights
>  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
>  copies of the Software, and to permit persons to whom the Software is
>  furnished to do so, subject to the following conditions:
>
>  The above copyright notice and this permission notice shall be included in
>  all copies or substantial portions of the Software.
>
>  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
>  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
>  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
>  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
>  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
>  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
>  THE SOFTWARE.




