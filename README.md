# Simple Turing Machine Simulator

> it uses the same syntax used in the official turing competition in Italy.

# Important note:

The data it reads and writes is case-sensitive!

# Arguments and things

| Argument | Description      | Parameter                                                    | Default          | Optional |
| -------- | ---------------- | ------------------------------------------------------------ | ---------------- | -------- |
| `-t`     | verbose          | (Optional) `float`: seconds to wait between each instruction | `0.02`           | ✅       |
| `-f`     | read from a file | `file path`: the file path                                   | `istruzioni.txt` | ✅       |

## Stupid examples

```
./a.out -t
```

> Verbose with default wait time

#

```
./a.out -t -f a.txt
```

> Verbose with default wait time, reading from file

#

```
./a.out -f ./sadasd/fdksf/c.txt
```

> Reading from file

#

```
./a.out -f ./sadasd/fdksf/c.txt -t 0.1
```

> Reading from file with verbose and wait time 0.1s

# How instruction has to be formatted

`istruzioni.txt`

```
DJBPG
(0, [B..F][H..Z]G, 0, [A..E][G..Y]-, >)
```

**First line**: initial tape --> `DJBPG` \
**Everything below**: instructions --> `(0, [B..F][H..Z]G, 0, [A..E][G..Y]-, >)`

> You can find many examples in `problemi_risolti/`

# Compilation

It works on every OS (in theory) <br>
Just compile it with

```
g++ turing_simulator.cpp
```
