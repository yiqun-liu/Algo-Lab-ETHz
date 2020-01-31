# BGL

A brief summary of algolab-related API.



## General API:

**Algo-Lab doc**: search `adjacency list`

**sample code**: `tut4-init.cpp`



Vertices and edge are described by  `vertex_descriptor` and  `edge_descriptor`.

`vertex_iterator` and `edge_iterator` are their iterators.



We can simply use `int` as `vertex_descriptor` for convenience. (I will do so to make code simpler)



### Construct Graph

**Initialize Graph**

Use `boost::adjacency_list`.

```c++
#include <boost/graph/adjacency_list.hpp>
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> graph;

int nVertices = 13;
graph G(nVertices); 
// Initialize graph with 13 vertices, with vertex_descriptor being 0, 1, ... nVertices-1
```

The third class identifier of `boost::adjacency_list` (in our example `boost::directedS`) specify that the graph is directed. Other options are `undirectedS`, and `bidirectionalS`.



**Add edge**

Note this function returns a `std::pair`. The bool of it indicates whether the new edge is accepted (New edges might be rejected due to duplicate, but should not be our concerns).

```c++
// function declaration
std::pair<edge_descriptor, bool>
add_edge(vertex_descriptor from, vertex_descriptor to, adjacency_list& g);

// ...

// we call above function to add edge from 0 to 1
edge_descriptor e = boost::add_edge(0, 1, G).first; // Note the `first`
```



For graph with edge-property (e. g. weighted graph), property can also be added in a single function call

```c++
add_edge(vertex_descriptor from, vertex_descriptor to, edge_property_type property
         adjacency_list& g);
```



**Add vertex**

 `boost::add_vertex(G);`

But normally there is no need adding vertex to an initialized graph. 



This can also be done (mostly inadvertently) by calling `add_edge` with out-of-range `vertex_descriptor`.

> `graph G(4);`
>
> // ...
>
> `boost::add_edge(0, 7, G);` // this extends the vertex set of G to eight vertices!

But this operations are risky: it may cause core dump.



**Weighted graph**

Weighted graph is just a plain graph with a weight function. The standard programming interface to weight function is `boost::property_map`. 

To initialize a weighted graph, we need to provide additional class information:

```c++
#include <boost/graph/adjacency_list.hpp>
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
    boost::no_property, // no vertex property
    boost::property<boost::edge_weight_t, int> // edge property, used as weight
> weighted_graph;
```

Here we create a property for edge. `boost::edge_weight_t` is its property tag, while `int` is its data type



There're other predefined vertex and edge properties can be used. They serves as interface between the `adjacency_list` data structure and related BGL algorithms. However, they will not be automatically initialized nor updated when you manipulate the graph arbitrarily (which means you have to manually maintain it in some cases). `boost::property_map` is the programming interface to all these properties.

```c++
// a map. First arg is the adjacency list (typedef-ed to weighted_graph)
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
```



Usage of property map:

```c++
// Get the property map from an existing graph
weight_map weights = boost::get(boost::edge_weight, G);

edge_desc e = ... ; // any edge registered in G

// Access/modify the property
weights[e] = weight_to_assign; // use descriptor as subscript
```



**Number of vertices**

```c++
int nVertices = boost::num_vertices(G);
int nEdges    = boost::num_edges(G);
```



**Vertices adjacent to an edge** 

```c++
boost::graph_traits<graph>::edge_iterator e;

// e: (from -> to)
int from = boost::source(*e, G);
int to   = boost::target(*e, G);
```

To lookup edges adjacent to an edge, first lookup its end-points and lookup their incident edges (as following section shows).



**Edges adjacent to a vertex; vertices adjacent to a vertex**

Here we only consider `out_edges` in a directed graph. This is sufficient for most algo-lab problem

Note: `in_edges` and `in_edge_iterator` is not possible with standard adjacency list. It is only available for `bidirectionalS` graph. 

```c++
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;

int from = 0; // vertex id
out_edge_it oe_beg, oe_end;
for (boost::tie(oe_beg, oe_end) = boost::out_edges(from, G); oe_beg != oe_end; ++oe_beg) {
    assert(boost::source(*oe_beg, G) == from);
    int to = boost::target(*oe_beg, G); // neighbors vertex descriptor
    // ANYTHING
}
```



**Graph-wide edge/vertex iteration**

<u>Vertex</u>: Usually, the vertex id (`vertex_descriptor`) is assigned by programmer from `0` to `n-1`, thus a `for` loop suffices to iterate all vertices. BGL API is not necessary.



<u>Edge</u>: To traverse all edges, you can traverse all vertices, and for each of them, traverse its out-going edges (See the use of `boost::out_edges(from, G)` in previous example).

There is also a dedicated API `boost::edges` for this. We should use `edge_iterator`  to iterate:

```c++
typedef boost::graph_traits<graph>::edge_iterator edge_it;

edge_it e_beg, e_end;
for (boost::tie(e_beg, e_end) = boost::edges(G); e_beg != e_end; ++e_beg) {
	// ANYTHING about e_beg
    ;
}
```

Alternatively:

```c++
edge_it e_beg, e_end;
boost::tie(e_beg, e_end) = boost::edges(G);
for (edge_it e; e != e_end; ++e) {
	// ANYTHING about e
    ;
}
```



## Dijkstra

**Algo-Lab doc**: search `dijkstra`

**Sample code:** `tut4-dijkstra.cpp`



All the library asks for are:

- graph
- source
- a `distance_map` (usually `std::vector<int>`)
- optional `predecessor map` (usually `std::vector<vertex_desc>` or `std::vector<int>`)



1. Shortest path **distance only**: provide a `std::vector<int> dist_map(n);` as distance map. After running the function, access the results by `dist_map[to]`:

```c++
int n = boost::num_vertices(G);
std::vector<int> dist_map(n);

// library function
boost::dijkstra_shortest_paths(G, s,
	boost::distance_map(boost::make_iterator_property_map(
        dist_map.begin(), // 
        boost::get(boost::vertex_index, G))));

return dist_map[t];
```

2. Need distance and **path information**. Provide an additional`std::vector<vertex_desc> pred_map(n);` to record predecessor of each vertex, then manually construct the path.

```c++
int n = boost::num_vertices(G);
std::vector<int> dist_map(n);
std::vector<vertex_desc> pred_map(n);

// Run library function
boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
    dist_map.begin(), boost::get(boost::vertex_index, G)))
    /* dot! */ .predecessor_map(boost::make_iterator_property_map(
    pred_map.begin(), boost::get(boost::vertex_index, G)))
);

// Manually Construct the path
int cur = t;
path.clear(); path.push_back(cur);
while (s != cur) {
    cur = pred_map[cur];
    path.push_back(cur);
}
std::reverse(path.begin(), path.end());
```



If an vertex is **unreachable**, its predecessor is itself (Note: source's predecessor is also itself), and the corresponding element in dist_map is the biggest possible number of that type (for `int` it is usually 2147483747)



## Minimum Spanning Tree

**Sample code**: `tut4-kruskal.cpp`



Kruskal API requires **undirected** graph, and a container (usually ``std::vector<edge_desc>`). 

```c++
std::vector<edge_desc> mst; // vector to store MST edges (not a property map!)
boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
```



After the function finished, all edges of the MST will be stored in the container (by `edge_descriptor`).

```c++
for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it)
	std::cout << boost::source(*it, G) << " " << boost::target(*it, G) << "\n";
```



## Maximum Matching

**Sample code**: `tut4-edmonds.cpp`



Edmonds maximum matching requires an **undirected** graph and a container (usually ``std::vector<vertex_desc>`) to store matching results.

`O(mn * alpha(m,n))`



```c++
int n = boost::num_vertices(G);

std::vector<vertex_desc> mate_map(n); // exterior property map

boost::edmonds_maximum_cardinality_matching(G,
	boost::make_iterator_property_map(mate_map.begin(),
		boost::get(boost::vertex_index, G))
);

int matching_size = boost::matching_size(G,
	boost::make_iterator_property_map(mate_map.begin(), 
		boost::get(boost::vertex_index, G))
);
```



Unmatched vertex are "paired" with flag`null_vertex()`. 

Also, the matching is represented in bidirectional manner, thus we should filter results by `i < mate_map[i]` so that we traverse the matching only once.

```c++
const vertex_desc NULL_VERTEX = boost::graph_traits<graph>::null_vertex();

for (int i = 0; i < n; ++i)
	if (mate_map[i] != NULL_VERTEX && i < mate_map[i])
        std::cout << i << " " << mate_map[i] << "\n";
```



## BFS - coloring

**Sample Code: **`tut4-universal_slow.cpp`



Not significant simpler than writing a BFS by hand.

`boost::breadth_first_search` colors visited black. Can be used to check reachability

```c++
int n = boost::num_vertices(G);

typedef boost::default_color_type color;
std::vector<color> vertex_color(n); // exterior property map

boost::breadth_first_search(G, u,
	boost::color_map(boost::make_iterator_property_map(
		vertex_color.begin(), boost::get(boost::vertex_index, G))));
```



```c++
const color black = boost::color_traits<color>::black(); // visited by BFS
const color white = boost::color_traits<color>::white(); // not visited by BFS
```



## Strong Connected Component

**Sample Code: **`tut4-universal_fast.cpp`



We pass in a `std::vector<int>`,  `boost::strong_components` will assign the 'strong-component id' to each vertex, and return the number of strong connected components.

Very fast (`O(|V| + |E|)`, probably Tarjan)

```c++
int n = boost::num_vertices(G);

// scc_map[i]: index of SCC containing i-th vertex
std::vector<int> scc_map(n); // exterior property map

// nscc: total number of SCCs
int nscc = boost::strong_components(G, boost::make_iterator_property_map(
    scc_map.begin(), boost::get(boost::vertex_index, G)));
```



## Maximum Flows

**Sample Code: ** `tut6-bgl_push_relabel_max_flow.cpp`



Similar to weighted graph, new edge properties, including capacity and residual capacity (as `long` type), must be added to adjacency list:

```c++
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
    boost::property<boost::edge_residual_capacity_t, long,
    boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
```



As in weighted graph, we intialize and access these properties through property map

```c++
typedef boost::property_map<graph, boost::edge_capacity_t>::type capacity_map;
typedef boost::property_map<graph, boost::edge_residual_capacity_t>::type r_capacity_map;
typedef boost::property_map<graph, boost::edge_reverse_t>::type rev_edge_map;

// Get property map from graph
capacity_map c_map = boost::get(boost::edge_capacity, G);
r_capacity_map rc_map = boost::get(boost::edge_residual_capacity, G);
rev_edge_map r_map = boost::get(boost::edge_reverse, G);

edge_desc e = ...;

c_map[e] = ...;
r_map[e] = ...;
```

We can also avoid those `typedef`s with C++11's `auto` keyword (which is used in sample code) 

```c++
auto c_map = boost::get(boost::edge_capacity, G);
auto r_map = boost::get(boost::edge_reverse, G);
auto rc_map = boost::get(boost::edge_residual_capacity, G);
```



Before running `push_relabel` algorithm, we have to make sure all edges in residual graph are added, and are connected to the corresponding edges. However, there's no need to initalize residual capacity map.

```c++
c_map[e] = capacity;
c_map[rev_e] = 0; // we treat this edge as absent by assign 0 to its capacity
r_map[e] = rev_e;
r_map[rev_e] = e;
```



Run maximum flow algorithm:

```c++
vertex_desc source = ..., target = ...;
long flow = boost::push_relabel_max_flow(G, source, target);
```



If curious in the flow of single edge, (after running statements above) we can calculate it using residual graph information:

```c++
long flow_through_edge = c_map[e] - rc_map[e];
```



### Minimum Cut

1. Run maximum flow algorithm
2. Run DFS/BFS to judge reachability on residual graph

**Sample code:** `tut9-residual_bfs.cpp`



TODO minIS check