# Expressions

If built-in data types and their literals are atoms, then expressions must be the molecules. Most of these will be familiar.

### Arithmetic

The subexpressions on either side of the operator are operands. Because there are two of them, these are called binary operators

```
add + me;
subtract - me;
multiply * me;
divide / me;
```

### Comparision and Equality

we have a few more operators that always return a Boolean result

```
less < than;
lessThan <= orEqual;
greater > than;
greaterThan >= orEqual;
```

We can test two values of any kind for equality or inequality.

```
1 == 2;         // false.
"cat" != "dog"; // true.
```

Even different types.

```
314 == "pi"; // false.
```

### Logical operators

The not operator, a prefix !, returns false if its operand is true, and vice versa

```
!true;  // false.
!false; // true.
```

The other two logical operators really are control flow constructs in the guise of expressions. An and expression determines if two values are both true. It returns the left operand if it’s false, or the right operand otherwise.

```
true and false; // false.
true and true;  // true.
```

And an or expression determines if either of two values (or both) are true. It returns the left operand if it is true and the right operand otherwise.

```
false or false; // false.
true or false;  // true.
```

### Precedence and grouping

All of these operators have the same precedence and associativity that you’d expect coming from C. In cases where the precedence isn’t what you want, you can use () to group stuff.

```
var average = (min + max) / 2;
```


