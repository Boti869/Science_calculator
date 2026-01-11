# Science_calculator
This is a science calculator specifically for electrical calculations (and possibly physics in later updates).  
Calculate RLC and variations of this circuit! Parallel or in series? No problem!  

## Compile and run
To do this ou'll need gcc on you computer
```
gcc main.c calcs/help.c calcs/Science-calc.c calcs/algebra.c electrics-c/electrical-laws.c electrics-c/initLaws.c electrics-c/AC-basics.c electrics-c/RLC_circuits/Series.c electrics-c/RLC_circuits/Parallel.c -o mycalc
mycalc
```
or you can just use 
```
make
make run
```
## Compatibility
- This project was written entirely on C.
- Written on Windows, but compatible with Linux and MacOS too! 
## License
This project is licensed under the GNU Affero General Public License v3.0 (AGPL-3.0). See [LICENSE](https://github.com/Boti869/Science_calculator/blob/main/LICENSE) for details.
