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
    int tamanio;

public:
    ArregloDinamico(int n)
    {
        tamanio = n;
        datos = new int[tamanio];
    }

    ~ArregloDinamico()
    {
        delete[] datos;
    }

    void llenar(int proceso, const char* nombreEquipo)
    {
        srand((unsigned int)time(NULL) + proceso * 1000);

        #pragma omp parallel
        {
            for (int porcentaje = 10; porcentaje <= 100; porcentaje += 10)
            {
                int inicio = tamanio * (porcentaje - 10) / 100;
                int fin = tamanio * porcentaje / 100;

                #pragma omp for schedule(static)
                for (int i = inicio; i < fin; i++)
                {
                    int numero;

                    #pragma omp critical
                    {
                        numero = rand() % 100;
                    }

                    datos[i] = numero;
                }

                #pragma omp single
                {
                    cout << "Avance: "
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

    void mostrar(int proceso, const char* nombreEquipo)
    {
        cout << "\n---------------------------------" << endl;
        cout << "Equipo: " << nombreEquipo << endl;
        cout << "Proceso MPI: " << proceso << endl;
        cout << "Tamanio del arreglo: " << tamanio << endl;
        cout << "Datos: ";

        for (int i = 0; i < tamanio; i++)
        {
            cout << datos[i] << " ";
        }

        cout << endl;
    }
};

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int proceso;
    int totalProcesos;

    MPI_Comm_rank(MPI_COMM_WORLD, &proceso);
    MPI_Comm_size(MPI_COMM_WORLD, &totalProcesos);

    char nombreEquipo[MPI_MAX_PROCESSOR_NAME];
    int longitud;

    MPI_Get_processor_name(nombreEquipo, &longitud);

    int tamanio = 0;

    if (proceso == 0)
    {
        srand((unsigned int)time(NULL));
        tamanio = 20 + rand() % 31;
    }

    MPI_Bcast(
        &tamanio,
        1,
        MPI_INT,
        0,
        MPI_COMM_WORLD
    );

    cout << "\nEquipo: "
         << nombreEquipo
         << " | Proceso MPI: "
         << proceso
         << " de "
         << totalProcesos
         << " | Tamanio: "
         << tamanio
         << endl;

    ArregloDinamico arreglo(tamanio);

    arreglo.llenar(
        proceso,
        nombreEquipo
    );

    MPI_Barrier(MPI_COMM_WORLD);

    for (int p = 0; p < totalProcesos; p++)
    {
        MPI_Barrier(MPI_COMM_WORLD);

        if (p == proceso)
        {
            arreglo.mostrar(
                proceso,
                nombreEquipo
            );
        }
    }

    MPI_Finalize();

    return 0;
}