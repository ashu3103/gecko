# Control Flow

It’s hard to write useful programs if you can’t skip some code or execute some more than once. That means control flow. 

### If Statement

An if statement executes one of two statements based on some condition.

```
if (condition) {
  print "yes";
} else {
  print "no";
}
```

### Loop Statement

A while loop executes the body repeatedly as long as the condition expression evaluates to true.

```
var a = 1;
while (a < 10) {
  print a;
  a = a + 1;
}
```

Finally, we have `for` loops.

```
for (var a = 1; a < 10; a = a + 1) {
  print a;
}
```
