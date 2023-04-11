# orpheus

Orpheus is a simple waveform generation engine. It is designed to support
generating aribtrary waveforms via a graph of interconnected nodes. Each node
is a transformation of given inputs and, as such, the total result is a
function of the generation graph.

Because `orpheus` is to be converted to `sysverilog` at one point, the graph
generation is **done at compile time**, avoiding dynamic memory and similar
shenanigans.

Originally, orpheus was designed to be written in `C`, but nobody likes `C`'s
macros so I decided to migrate to `C++`.
