========================================================================================
# Implementation of CosySEL a SAT solver combining Symmetric Explanation Learning (SEL) and Effective Symmetry Breaking Predicates (ESBP).
========================================================================================

## Directory overview:
-------------------

`core/` A core version of the solver  
`mtl/` MiniSat Template Library  
`sat_symmetry/` A library for ESBP  
`simp/` An extended solver with simplification capabilities  
`utils/` MiniSat util files  
`Changelog`    
`LICENCE`  
`README`    

## Building
-------
```bash
cd sat_symmetry
source .env
make third_party
make
cd ../simp
make rs
```
## Running
------
**BLISS**: 
```bash 
    cd simp/
   ./runner/cosysel.sh bliss CNF_FILE 
```

**SAUCY**: 
```bash
    cd simp/
    ./runner/cosysel.sh saucy CNF_FILE 
```

## Information:
```bash
    ./glucose --help 
```
