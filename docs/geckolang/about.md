# About

A compact, high level scripting language with C-like syntax. Since its a scripting language, we can execute it from source. Two major aspects that gecko share with other C-like high level languages like Java-Script are:

- **Dynamic Typing:** Variables can store values of any type, and a single variable can even store values of different types at different times. If you try to perform an operation on values of the wrong type—say, dividing a number by a string—then the error is detected and reported at runtime. Statically typed implementation has a lot to learn and implement, so to keep stuff simple, we are moving ahead with dynamically typing.

- **Automatic memory management:** There are two main techniques for managing memory: reference counting and tracing garbage collection (usually just called garbage collection or GC). Ref counters are much simpler to implement.