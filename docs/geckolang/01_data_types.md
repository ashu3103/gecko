# Data Types

In gecko's little universe, the atoms that make up all the matter are built-in data types. There are only a handful of built-in primary data types we use:

### Booleans

The boolean type represent the truth and falsehood logic, the only values it can take are:
- `true`
- `false`

Usage in code:

```
true;
false;
```

### Numbers

Gecko has only one kind of numbers - double-precision floating point. Since floating-point numbers can also represent a wide range of integers, that covers a lot of territory, while keeping things simple. We'll also skip different notation and bases like hexadecimal and only focus on decimal.

Usage in code:

```
1234;   // Integer
12.34;  // Float
```

### Strings

Like most languages, they are enclosed in double quotes

Usage in code:

```
"I am a string";
"";    // The empty string.
"123"; // This is a string, not a number.
```

### Nil

It represents “no value”. It’s called “null” in many other languages. In Gecko we spell it `nil`.

