# A Fast Sequential Algorithm for the Matrix Chain Ordering Problem

This repository stores sequential algorithms and parallel algorithms (in the Coarse-Grained Multicomputer model) to solve the MCOP using the dynamic-programming technique. We have implemented many strategies :

1.  Sequential algorithms :
    - Godbole (dx.doi.org/10.1109/TC.1973.5009182)
    - Yao's speed-up (dx.doi.org/10.1137/0603055)
    - Myoupo and Kengne's enhancement based on the Yao's speed-up (dx.doi.org/10.4018/ijghpc.2014040105)
    - Lacmou et *al.* based on the Yao's speed-up (our own)
2.  CGM-based Parallel algorithms using the OpenMPI librairy:
    - Kechid and Myoupo (https://www.amazon.fr/Programmation-dynamique-mod%C3%A8les-calcul-parall%C3%A8le/dp/6131592527/)
    - Myoupo and Kengne (dx.doi.org/10.4018/ijghpc.2014040105)

## Build projects

Just execute in each root folder:

````
make
````

## Run projects

#### A. Configuration

The config file is located at the path :

````
{project}/resources/config/config.json
````

You can set the data set, the data size, the algorithm be used, etc.

````
...
...
"datasets": {
        "datasets-dir": "resources/datasets/",
        "datasets-type": "dimensions/",
        "datasets-dirname": "102401/",
        "datasets-filename": "102401",
        "datasets-extension": ".data",
        "datasets-max-number-matrix": "102401",
        "datasets-input": {
            "datasets-input-id": "_1",
            "datasets-input-max-number-matrix": "20480",
            "datasets-input-algorithm": "1",
            "datasets-input-partitioning-technique": "1"
        },
        "datasets-create": {
            "datasets-create-id": "_5"
        }
    }
...
...
````

The algorithms be used are identifyied by a number :

0.  Godbole
1.  Yao's speed-up
2.  Myoupo and Kengne's enhancement
3.  Lacmou et *al.*

The partitioning techniques (for the parallel solutions) are also identifyied by a number :

1.  the DAG is divided by *Tmax* (Myoupo and Kengne)
2.  the DAG is divided by *Tint* (Myoupo and Kengne)
3.  the DAG is divided by *Tcb* (Kechid and Myoupo)

#### B. Sequential algorithms

Run :

````
./bin/TCP-sequential.run {data size} {algorithm be used}
````

For instance :

````
./bin/TCP-sequential.run 10240 3
````

#### C. Parallel algorithms

Run :

````
mpirun -np {number of processors} -hostfile hosts ./bin/TCP-CGM.run {data size} {algorithm be used} {partitioning technique}
````

For instance :

````
mpirun -np 8 -hostfile hosts ./bin/TCP-CGM.run 10240 2 1
````

#### D. Remarks

1. If you execute either sequential algorithms, or parallel algorithms without parameters then default parameters will retrieve from the configuration file.
2. Only sequential algorithms 1 and 2 (Yao's speed-up and Myoupo and Kengne's enhancement) are used in CGM-based parallel algorithms.

## License
[MIT](LICENSE)