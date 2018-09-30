# Tipe

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
    auto& [entry, mul, print] = ids;

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