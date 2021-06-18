# Generate input files

```bash
make gen_data && build/gen_data {seed} {number of sensors} {sensor max capacity} {mc max capacity} {default consuming rate}
```

# Run solutions

 - DPBM:
```bash
make brute_force && build/brute_force
```
 - LSA:
```bash
make main && build/main
```
 - MCSA:
```bash
make nguyen2020 && build/nguyen2020
```

# Multiple testcases

To run multiple testcases, change the parameters for gen_data inside `run.sh` and excecute it.
```bash
./run.sh
```

Results are shown in `OUT`
