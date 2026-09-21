#include <iostream>
#include <cstdlib>
#include <ctime>
#include <mpi.h>
#include <omp.h>

using namespace std;

class ArregloDinamico
{
private:
    int* datos;
    long long tamanio;

public:

    // Constructor
    ArregloDinamico(long long n)
    {
        tamanio = n;
        datos = new int[tamanio];
    }

    // Destructor
    ~ArregloDinamico()
    {
        delete[] datos;
    }

    // Llenar arreglo utilizando OpenMP
    void llenar(int proceso, const char* nombreEquipo)
    {
        // Semilla diferente para cada proceso MPI
        srand((unsigned int)time(NULL) + proceso * 1000);

        #pragma omp parallel
        {
            /*
                El arreglo se divide en 10 partes.

                Cada parte representa:
                10%, 20%, 30% ... 100%.

                OpenMP divide cada bloque entre los hilos.
            */

            for (int porcentaje = 10;
                 porcentaje <= 100;
                 porcentaje += 10)
            {
                long long inicio =
                    tamanio * (porcentaje - 10) / 100;

                long long fin =
                    tamanio * porcentaje / 100;

                #pragma omp for schedule(static)
                for (long long i = inicio; i < fin; i++)
                {
                    int numero;

                    /*
                        rand() comparte información interna.

                        Se protege para evitar que varios
                        hilos accedan a rand() al mismo tiempo.
                    */
                    #pragma omp critical(rand_generator)
                    {
                        numero = rand() % 1000;
                    }

                    datos[i] = numero;
                }

                /*
                    El omp for anterior tiene una barrera
                    implícita.

                    Cuando llegamos aquí, todo el bloque
                    correspondiente al porcentaje ya terminó.
                */

                #pragma omp single
                {
                    cout
                        << "Avance: "
                        << porcentaje
                        << "%"
                        << " | Proceso MPI: "
                        << proceso
                        << " | Equipo: "
                        << nombreEquipo
                        << " | Hilo OpenMP: "
                        << omp_get_thread_num()
                        << endl;
                }

                #pragma omp barrier
            }
        }
    }


    // Mostrar arreglo
    void mostrar(int proceso, const char* nombreEquipo)
    {
        cout << "\n==========================================" << endl;

        cout
            << "Equipo: "
            << nombreEquipo
            << endl;

        cout
            << "Proceso MPI: "
            << proceso
            << endl;

        cout
            << "Tamanio del arreglo: "
            << tamanio
            << endl;

        /*
            En arreglos pequeños mostramos todos
            los elementos.

            Con 10,000,000 mostramos una muestra
            para no saturar la consola.
        */

        if (tamanio <= 100)
        {
            cout << "Datos: ";

            for (long long i = 0; i < tamanio; i++)
            {
                cout << datos[i] << " ";
            }

            cout << endl;
        }
        else
        {
            cout << "\nPrimeros 20 elementos:" << endl;

            for (long long i = 0; i < 20; i++)
            {
                cout << datos[i] << " ";
            }

            cout << endl;

            cout << "\nUltimos 20 elementos:" << endl;

            for (long long i = tamanio - 20;
                 i < tamanio;
                 i++)
            {
                cout << datos[i] << " ";
            }

            cout << endl;
        }

        cout << "==========================================" << endl;
    }


    long long obtenerTamanio()
    {
        return tamanio;
    }
};


int main(int argc, char* argv[])
{
    // =====================================================
    // INICIALIZAR MPI
    // =====================================================

    MPI_Init(&argc, &argv);


    // =====================================================
    // INFORMACIÓN DE MPI
    // =====================================================

    int proceso;
    int totalProcesos;

    MPI_Comm_rank(
        MPI_COMM_WORLD,
        &proceso
    );

    MPI_Comm_size(
        MPI_COMM_WORLD,
        &totalProcesos
    );


    // =====================================================
    // OBTENER NOMBRE DE LA COMPUTADORA
    // =====================================================

    char nombreEquipo[MPI_MAX_PROCESSOR_NAME];

    int longitudNombre;

    MPI_Get_processor_name(
        nombreEquipo,
        &longitudNombre
    );


    // =====================================================
    // MOSTRAR INFORMACIÓN GENERAL
    // =====================================================

    if (proceso == 0)
    {
        cout << "\n";
        cout << "==================================================" << endl;
        cout << "             PRACTICA MPI + OPENMP" << endl;
        cout << "==================================================" << endl;
        cout << "Integrantes:" << endl;
        cout << "Gomez Enriquez Maria Fernanda" << endl;
        cout << "Lozano Perez Jose Manuel" << endl;
        cout << "Martinez Arias Eduardo Caleb" << endl;
        cout << "==================================================" << endl;
    }


    MPI_Barrier(MPI_COMM_WORLD);


    // =====================================================
    // DETERMINAR TAMAÑO DEL ARREGLO
    // =====================================================

    long long tamanio = 0;


    /*
        Si escribimos:

        main.exe 10000000

        utiliza 10 millones.

        Si NO escribimos tamaño:
        genera automáticamente entre 20 y 50.
    */

    if (argc >= 2)
    {
        tamanio = atoll(argv[1]);
    }
    else
    {
        if (proceso == 0)
        {
            srand((unsigned int)time(NULL));

            tamanio =
                20 + rand() % 31;
        }


        /*
            El proceso 0 manda el tamaño
            a los demás procesos MPI.
        */

        MPI_Bcast(
            &tamanio,
            1,
            MPI_LONG_LONG,
            0,
            MPI_COMM_WORLD
        );
    }


    // =====================================================
    // MOSTRAR HILOS OPENMP DISPONIBLES
    // =====================================================

    int cantidadHilos = 0;

    #pragma omp parallel
    {
        #pragma omp single
        {
            cantidadHilos =
                omp_get_num_threads();
        }
    }


    /*
        Imprimimos un proceso a la vez para evitar
        que esta información inicial se mezcle.
    */

    for (int p = 0; p < totalProcesos; p++)
    {
        MPI_Barrier(MPI_COMM_WORLD);

        if (proceso == p)
        {
            cout
                << "\nEquipo: "
                << nombreEquipo
                << " | Proceso MPI: "
                << proceso
                << " de "
                << totalProcesos
                << " | Hilos OpenMP: "
                << cantidadHilos
                << " | Tamanio: "
                << tamanio
                << endl;
        }
    }


    MPI_Barrier(MPI_COMM_WORLD);


    // =====================================================
    // CREAR ARREGLO DINÁMICO
    // =====================================================

    ArregloDinamico arreglo(tamanio);


    // =====================================================
    // MEDIR TIEMPO
    // =====================================================

    double inicio =
        MPI_Wtime();


    // =====================================================
    // LLENAR ARREGLO CON OPENMP
    // =====================================================

    arreglo.llenar(
        proceso,
        nombreEquipo
    );


    double fin =
        MPI_Wtime();


    // =====================================================
    // ESPERAR A TODOS LOS PROCESOS
    // =====================================================

    MPI_Barrier(MPI_COMM_WORLD);


    // =====================================================
    // MOSTRAR TIEMPO DE CADA PROCESO
    // =====================================================

    for (int p = 0; p < totalProcesos; p++)
    {
        MPI_Barrier(MPI_COMM_WORLD);

        if (proceso == p)
        {
            cout
                << "\nProceso MPI "
                << proceso
                << " | Equipo: "
                << nombreEquipo
                << " | Tiempo de llenado: "
                << (fin - inicio)
                << " segundos"
                << endl;
        }
    }


    // =====================================================
    // MOSTRAR ARREGLOS
    // =====================================================

    for (int p = 0; p < totalProcesos; p++)
    {
        MPI_Barrier(MPI_COMM_WORLD);

        if (proceso == p)
        {
            arreglo.mostrar(
                proceso,
                nombreEquipo
            );
        }
    }


    MPI_Barrier(MPI_COMM_WORLD);


    // =====================================================
    // MENSAJE FINAL
    // =====================================================

    if (proceso == 0)
    {
        cout << "\n";
        cout << "==================================================" << endl;
        cout << "        EJECUCION FINALIZADA CORRECTAMENTE" << endl;
        cout << "==================================================" << endl;
    }


    // =====================================================
    // FINALIZAR MPI
    // =====================================================

    MPI_Finalize();

    return 0;
}