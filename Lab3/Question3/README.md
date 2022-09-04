# Question3

## Create 10mb file

```bash
make genfile
```

## Compile the code

```bash
make all
```

## Run the code

### terminal 1

```bash
./server_A 8888
```

### terminal 2

```bash
./server_B 8889
```

### terminal 3
Port numbers are for `server_A` and `server_B` respectively

```bash
./client localhost 8888 8889
```

## Remove binaries

```bash
make clean
```
