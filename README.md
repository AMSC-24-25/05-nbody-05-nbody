# N-Body problem

Hands-on for **_Advanced Methods for Scientific Computing_** course @ **Politecnico di Milano**.

## 🧩 Authors

- [Angelo Notarnicola](https://github.com/MartinisFederico)
- [Daniele Piano](https://github.com/danielepiano)
- [Ettore Cirillo](https://github.com/EccirilloM)
- [Michele Milani](https://github.com/MicheleeMilani)
- [Stefano Pedretti](https://github.com/StePedre)
- Teaching assistant :: Federico gatti
- Professor :: [Luca Formaggia](https://github.com/lformaggia)

## ⚙️ Compilation

```shell
foo@bar:~/path/to/05-nbody-05-nbody$ mkdir -p build
foo@bar:~/path/to/05-nbody-05-nbody$ cd build
foo@bar:~/path/to/05-nbody-05-nbody/build$ cmake -DMODEL={SERIAL|MPI|OPENMP} ..
foo@bar:~/path/to/05-nbody-05-nbody/build$ make
```

(MPI and OpenMP implementations not available)