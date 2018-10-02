# Tipe

[![CircleCI](https://circleci.com/gh/FatihBAKIR/tipe.svg?style=svg)](https://circleci.com/gh/FatihBAKIR/tipe)

_Tipe_ (pronounced type) is a strongly typed data flow graph processing library.
It's advantage lies at its compile time nature. The graphs are defined at compile
time, and are heavily optimized with C++ compilers and type checking is also
performed by the C++ compiler.

## Usage

Define a graph:

```cpp
void create_graph()
{
    auto nodes = tip::make_nodes(
        tip::nodes::constant<1>{},
        [](int x) { return x * 2; },
        tip::nodes::print{ std::cout, "\n" }
    );

    auto ids = tip::get_ids(nodes);
    auto [entry, mul, print] = ids;

    auto edges = make_edges(ids)
        .connect(entry | mul | print) // short hand for .connect(entry | mul).connect(mul | print)
        .connect(entry | print);

    return make_graph(std::move(nodes), edges);
}
```

Execute it:

```cpp
int main()
{
    auto g = create_graph();
    tip::execute(g, tip::node_id_t<1>{});
}
```

This will print the following:

```bash
2
1
```

The above code compiles down directly to printing 2 and 1, with all of the `Tipe`
code being eliminated by the compiler.

## `constexpr`

Everything in the core library is `constexpr`, thus, all `constexpr` graphs can execute
during compile time. The limitation is simply the nodes themselves. For example, the
`print` node provided in common nodes has `std::ostream` as a member, and thus, a graph
containing a `print` node cannot be `constexpr`.

## Strong Typing

Nodes in a `Tipe` graph is simply a C++ function object. These could be free functions,
lambdas, or custom function object types. As with all C++ functions, these have argument
types and return types. `Tipe` builds directly on the C++ type system. Every edge in the
graph must satisfy C++ type requirements. For example, the following graph will not
compile due to the mismatch of types between `root` and `doubl` nodes:

```cpp
auto nodes = make_nodes(
    []{ return "hello"s; },
    [](std::vector<float> values) {
        for (auto& v : values) v *= 2;
        return values;
    },
    [](auto v) { std::cout << v << '\n'; }
);

auto [root, doubl, print] = get_ids(nodes);

auto edges = make_edges(get_ids(nodes))
    .connect(root | doubl | print);

auto g = make_graph(std::move(nodes), edges);
```

This increases the correctness of programs produced using `Tipe` and eliminates the
majority of runtime type checks. For user or network provided inputs, obviously, type
checking must be done at runtime.

## Nodes

Nodes are simply function objects, that take a parameter and emit 0 or more messages
in response. Even a regular function can be a node, such as `std::sin`.

Right now, there's a requirement of immutability of function objects. This means the
functions passed as nodes must be callable on const objects. Execution model of `Tipe`
graphs may involve multiple threads in the future and immutable functions are a good
precaution for now. The constness requirement is mostly about thread safety. Through
proper synchronisation and use of `mutable` members, stateful nodes can be implemented.
