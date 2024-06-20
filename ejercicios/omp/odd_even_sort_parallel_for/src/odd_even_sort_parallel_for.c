#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void swap(double *a, double *b)
{
    double temp = *a;
    *a = *b;
    *b = temp;
}

void parallel_odd_even_sort(size_t n, double arr[n], int num_threads)
{
    omp_set_num_threads(num_threads);
    for (size_t phase = 0; phase < n; ++phase)
    {
        if (phase % 2 == 0)
        {
            #pragma omp parallel for
            for (size_t i = 1; i < n; i += 2)
            {
                if (arr[i - 1] > arr[i])
                {
                    swap(&arr[i - 1], &arr[i]);
                }
            }
        }
        else
        {
            #pragma omp parallel for
            for (size_t i = 1; i < n - 1; i += 2)
            {
                if (arr[i] > arr[i + 1])
                {
                    swap(&arr[i], &arr[i + 1]);
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s size of array number_of_threads\n", argv[0]);
        return EXIT_FAILURE;
    }

    size_t n = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    double *arr = (double *)malloc(n * sizeof(double));

    if (arr == NULL)
    {
        fprintf(stderr, "Memory allocation failed\\n");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < n; ++i)
    {
        arr[i] = (double)rand() / RAND_MAX * 100.0;
    }

    // #pragma omp single
    // {
    //   printf("Before sorting:\n");
    //   for (size_t i = 0; i < n; ++i)
    //   {
    //       printf("%f ", arr[i]);
    //   }
    //   printf("\n");
    // }

    parallel_odd_even_sort(n, arr, num_threads);

    // #pragma omp single
    // {
    //   printf("After sorting:\n");
    //   for (size_t i = 0; i < n; ++i)
    //   {
    //       printf("%f ", arr[i]);
    //   }
    //   printf("\n");
    // }

    free(arr);
    return EXIT_SUCCESS;
}
