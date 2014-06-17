# Excal

This is a C++ implementation of the [Shunting-yard Algorithm](http://en.wikipedia.org/wiki/Shunting_yard_algorithm) to parse mathematical expressions in infix notation.

A dirty secret of mine is that I like C++, so I consider this more of a playground app than production code. I've only tested it with trivial math problems, so I have no idea how well it works.

#### Example

```bash
./Excal 3.14159 * 8*8
201.06176

./Excal (4 * 3) / (8 - 3) * 4
9.6

./Excal 3 + 4 * 2 / ( 1 - 5 )
1
```
