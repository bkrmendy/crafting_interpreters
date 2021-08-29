# Crafting Interpreters
### _But this time in C++!_

[Crafting Interpreters](craftinginterpreters.com/)

```
Work in progress!
```

## TODOS
- [ ] typechecking instead of runtime casting
- [ ] make operators built in functions
- [ ] implement mixfix parsing (see above)


- [ ] Check `std::shared_ptr` usage 

## Places where I didn't follow the site:

### No code gen for AST classes
I personally don't like code generation, as it's pretty hard to debug, and involves mushing together strings
in an extremely error-prone way. Since there aren't that many classes involved (yet! still to be seen if this holds up),
I just typed them out manually.

### Visitor function instead of visitor class
Since expressions are represented by `std::shared_ptr`s, it's pretty easy to "pattern match" by "std::dynamic_pointer_cast"-ing
in a cascade of `if`s. Another possibility would have been to use `std::variant` and `std::visit`.