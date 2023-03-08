# Simple Turing Machine Simulator

> it uses the same syntax used in the official turing competition in Italy.

## How does it work

### Basic syntax

```
(currentState, readChar, finalState, writeChar, move)
```

example:

```
(0, -, 0, 0, -)
```

### Ranges and multicheck

**Example:**

```
(0, [0..8], 0, [1..9], -)
```

So this will replace 0 with 1, 1 with 2 and so on

#

**Example2:**

```
(0, 10, 0, 21, -)
```

This will replace 1 with 2, 0 with 1

#

**Example3**

```
(0, 01234, 0, -, -)
```

This will replace 01234 with - (nothing)

## How to compile

literally just

```
g++ turing_simulator.cpp
```

**it should work on every machine**

## How to use it

```
./a.out
```

runs as fast as possible

#

```
./a.out -v 0.03
```

with "verbose", shows you the value is working with, state, steps count and goes at the speed you pass (in seconds). <br>
Note: <br>
You can simply not pass the speed and it will go as fast as possible.

## How to use it

Write in the first line of `istruzioni.txt` the tape initial status, then write all the code below, then run.
