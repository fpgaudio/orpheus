# orpheus

Orpheus is a simple waveform generation engine. It is designed to support
generating aribtrary waveforms via a graph of interconnected nodes. Each node
is a transformation of given inputs and, as such, the total result is a
function of the generation graph.

Because `orpheus` is to be converted to `sysverilog` at one point, the graph
generation is **done at compile time**, avoiding dynamic memory and similar
shenanigans.

## Prerequesites

* `-std=c++20`
* [meson](https://mesonbuild.com/)
* POSIX Shell + Bash
* [catch2](https://github.com/catchorg/Catch2)

## Building

After you collected all of these, clone and build:

```bash
git clone git@github.com:fpgaudio/orpheus.git
cd orpheus
git submodule update
meson setup build
cd build
meson compile
meson test -v
```

Tests should pass and you should see cool graphs:

```text
 32767 ┤                 ╭──────────────╮                                                                   
 26214 ┤            ╭────╯              ╰────╮                                                              
 19660 ┤        ╭───╯                        ╰───╮                                                          
 13107 ┤    ╭───╯                                ╰───╮                                                      
  6553 ┤ ╭──╯                                        ╰──╮                                                   
     0 ┼─╯                                              ╰──╮                                              ╭ 
 -6553 ┤                                                   ╰──╮                                        ╭──╯ 
-13107 ┤                                                      ╰───╮                                ╭───╯    
-19660 ┤                                                          ╰───╮                        ╭───╯        
-26214 ┤                                                              ╰────╮              ╭────╯            
-32767 ┤                                                                   ╰──────────────╯                 

===============================================================================
All tests passed (5 assertions in 2 test cases)
```
