# Actividad 1.1 - Carga de memoria distribuida en MPI

## Integrantes

- Gomez Enriquez Maria Fernanda
- Lozano Perez Jose Manuel
- Martinez Arias Eduardo Caleb

## Descripción

En esta práctica se desarrolló un programa en C++ utilizando **MPI y OpenMP** para realizar la carga de un arreglo dinámico con valores aleatorios.

MPI se utiliza para trabajar con diferentes procesos, mientras que OpenMP permite paralelizar el procesamiento mediante hilos.

El programa muestra el avance del llenado del arreglo desde el **10% hasta el 100%**, indicando el proceso MPI, el equipo y el hilo OpenMP que participa.

## Objetivo

Implementar un programa de programación paralela utilizando MPI y OpenMP, trabajando con arreglos dinámicos y mostrando el porcentaje de avance durante la carga de datos.

## Características

- Uso de MPI.
- Uso de OpenMP.
- Uso de clases.
- Uso de arreglos dinámicos mediante `new`.
- Liberación de memoria mediante `delete[]`.
- Generación de valores aleatorios.
- Identificación del proceso MPI.
- Identificación del equipo.
- Identificación del hilo OpenMP.
- Avance del 10% al 100%.
- Trabajo con arreglos grandes.
- No se utiliza `vector`, `std::vector` ni `ArrayList`.

## Tecnologías utilizadas

- C++
- MS-MPI
- OpenMP
- Microsoft Visual Studio

## Estructura del proyecto

```text
Actividad1.1_MPI_OpenMP/
│
├── main.cpp
└── README.md
```

## Funcionamiento

El programa inicia MPI y obtiene el número de proceso y la cantidad total de procesos.

Después se crea un arreglo dinámico utilizando la clase `ArregloDinamico`.

El arreglo se llena utilizando OpenMP y se muestra el porcentaje de avance:

```text
10%
20%
30%
40%
50%
60%
70%
80%
90%
100%
```

Cada mensaje identifica el proceso MPI, el nombre del equipo y el hilo OpenMP.

## Arreglo dinámico

El almacenamiento se realiza mediante memoria dinámica:

```cpp
datos = new int[tamanio];
```

La memoria se libera al finalizar mediante:

```cpp
delete[] datos;
```

No se utilizan `vector`, `std::vector`, `ArrayList` ni estructuras similares.

## Compilación

Desde una consola de Visual Studio configurada para x64:

```bat
cl /EHsc /openmp main.cpp /I"C:\Program Files (x86)\Microsoft SDKs\MPI\Include" /link /LIBPATH:"C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64" msmpi.lib /OUT:main.exe
```

## Ejecución

### Primera prueba

Arreglo de tamaño aleatorio entre 20 y 50 elementos utilizando 4 procesos MPI:

```bat
mpiexec -n 4 .\main.exe
```

### Segunda prueba

Arreglo dinámico de 10,000,000 de elementos utilizando 4 procesos MPI:

```bat
mpiexec -n 4 .\main.exe 10000000
```

### Ejecución local

La actividad también puede realizarse en un esquema local utilizando varios procesos MPI:

```bat
mpiexec -n 8 .\main.exe 10000000
```

## Ejemplo de salida

```text
Equipo: DESKTOP-35AFNNO | Proceso MPI: 0 de 4 | Hilos OpenMP: 4

Avance: 10% | Proceso MPI: 0 | Equipo: DESKTOP-35AFNNO | Hilo OpenMP: 2
Avance: 20% | Proceso MPI: 0 | Equipo: DESKTOP-35AFNNO | Hilo OpenMP: 1
Avance: 30% | Proceso MPI: 0 | Equipo: DESKTOP-35AFNNO | Hilo OpenMP: 3
...
Avance: 100% | Proceso MPI: 0 | Equipo: DESKTOP-35AFNNO | Hilo OpenMP: 0
```

## Resultados

Las pruebas permiten comprobar el funcionamiento de MPI y OpenMP con diferentes tamaños de arreglos.

### Prueba 1

- 4 procesos MPI.
- Arreglo aleatorio de 20 a 50 elementos.
- Hilos OpenMP disponibles.
- Avance del 10% al 100%.
- Ejecución local.

### Prueba 2

- 4 procesos MPI.
- 10,000,000 de elementos.
- Hilos OpenMP disponibles.
- Avance del 10% al 100%.
- Ejecución local.

## Conclusión

Con esta práctica se comprobó el funcionamiento de MPI y OpenMP trabajando de manera conjunta. MPI permitió utilizar diferentes procesos y OpenMP permitió utilizar varios hilos para realizar el procesamiento. También se comprobó el uso de arreglos dinámicos para trabajar con grandes cantidades de datos sin utilizar `vector` ni estructuras similares.
