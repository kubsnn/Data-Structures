# Data-Structures

My own implementation of some containers and functions

Containers:
- vector
- hashtable
- hashset
- map   (rbtree)
- array   (stack allocated)
- dynarray   (heap)
- linkedlist   (bidirectional)
- forwardlist   (onedir)

Pipe functions:
- cycle   (infinity range)
- enumerate
- remove_if(function)
- transform(function)
- sorted
- unique
- take(count)
- zip(range, range)
- apply(container)   (moves data from range to container of fixed size)
- to_vector   (creates vector out of range)

Example:<br>
auto res = data | transform([](int x) { return x * 2; }
                | cycle | enumerate
                | take(12) | to_vector


Utilities:
- move(...)
- forward(...)
- swap(a, b)
- min,max(a,b)
- sort(begin, end)
- fill(begin, end)
- copy(begin, end, dst)
- distance(begin, end)
- find_if(begin, end, func)
- transform(begin, end, func)
- transform_if(begin, end, func, condition)
- replace(begin, end, old, new)
- replace_if(begin, end, func, new)


(pipe functions behaviour will be explained soon)
