# Statements

Where an expression’s main job is to produce a value, a statement’s job is to produce an effect. Since, by definition, statements don’t evaluate to a value, to be useful they have to otherwise change the world in some way—usually modifying some state, reading input, or producing output.

You’ve seen a couple of kinds of statements already. The first one was:

```
print "Hello, world!";
```

An expression followed by a semicolon (;) promotes the expression to statement-hood. This is called (imaginatively enough), an expression statement.

If you want to pack a series of statements where a single one is expected, you can wrap them up in a block.

```
{
  print "One statement.";
  print "Two statements.";
}
```
