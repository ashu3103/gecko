# Funtions

A function call expression looks the same as it does in C. A language isn’t very fun if you can’t define your own functions. In gecko, you do that with fun.

```
fun printSum(a, b) {
  print a + b;
}
```

From here on out:

- An argument is an actual value you pass to a function when you call it. So a function call has an argument list. Sometimes you hear actual parameter used for these.

- A parameter is a variable that holds the value of the argument inside the body of the function. Thus, a function declaration has a parameter list. Others call these formal parameters or simply formals.

The body of a function is always a block. Inside it, you can return a value using a return statement.

```
fun returnSum(a, b) {
  return a + b;
}
```

If execution reaches the end of the block without hitting a `return`, it implicitly returns `nil`.

### Closures

Functions are first class in gecko, which just means they are real values that you can get a reference to, store in variables, pass around, etc. This works:

```
fun addPair(a, b) {
  return a + b;
}

fun identity(a) {
  return a;
}

print identity(addPair)(1, 2); // Prints "3".
```

Since function declarations are statements, you can declare local functions inside another function.

```
fun outerFunction() {
  fun localFunction() {
    print "I'm local!";
  }

  localFunction();
}
```

If you combine local functions, first-class functions, and block scope, you run into this interesting situation:

```
fun returnFunction() {
  var outside = "outside";

  fun inner() {
    print outside;
  }

  return inner;
}

var fn = returnFunction();
fn();
```

Here, `inner()` accesses a local variable declared outside of its body in the surrounding function. Is this kosher? Now that lots of languages have borrowed this feature from Lisp, you probably know the answer is yes.

For that to work, `inner()` has to “hold on” to references to any surrounding variables that it uses so that they stay around even after the outer function has returned. We call functions that do this closures. These days, the term is often used for any first-class function, though it’s sort of a misnomer if the function doesn’t happen to close over any variables.