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

## 🌀 Visualization

Install the required dependencies mentioned in `requirements.txt`.

```shell
foo@bar:~/path/to/05-nbody-05-nbody$ pip3 install -r script/requirements.txt
```

Run the `visualize.py` script to visualize the generated 2D / 3D output data.

```shell
foo@bar:~/path/to/05-nbody-05-nbody$ python3 script/visualize.py
```

Should they be needed, the following options can be configured:

* `-i` `--input` `INPUT` - The directory containing the input files (_default_: `output`)
* `-p` `--input-prefix` `INPUT_PREFIX` - The prefix of the input files (_default_: `nbody-`)
* `-a` `--animation-interval` `ANIMATION_INTERVAL` - The interval between subsequent animation steps (milliseconds)
  (_default_: `50`)

todo :: gif of a result in readme