#include <stdio.h>
#include <stdlib.h>

void invert(double **matrix, int n);
void input(double **matrix, int n);
void output(double **matrix, int n);

int main()
{
    int n;
    printf("Введите размер квадратной матрицы: ");
    scanf("%d", &n);

    double **matrix = (double **)malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        matrix[i] = (double *)malloc(n * sizeof(double));
    }

    input(matrix, n);
    invert(matrix, n);
    output(matrix, n);

    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}

void invert(double **matrix, int n)
{
    // Создаем расширенную матрицу, добавляя единичную матрицу справа
    double **augmented = (double **)malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        augmented[i] = (double *)malloc(2 * n * sizeof(double));
        for (int j = 0; j < n; j++) {
            augmented[i][j] = matrix[i][j];
        }
        for (int j = n; j < 2 * n; j++) {
            augmented[i][j] = (i == (j - n)) ? 1.0 : 0.0;
        }
    }

    // Применяем элементарные преобразования для приведения исходной матрицы к единичной
    for (int i = 0; i < n; i++) {
        // Если главный элемент равен нулю, меняем строку с нижней строкой, где есть ненулевой элемент в текущем столбце
        if (augmented[i][i] == 0.0) {
            int swap_row = i + 1;
            while (swap_row < n && augmented[swap_row][i] == 0.0) {
                swap_row++;
            }
            if (swap_row == n) {
                // Обратная матрица не существует, освобождаем память и выходим
                for (int j = 0; j < n; j++) {
                    free(augmented[j]);
                }
                free(augmented);
                printf("n/a\n");
                return;
            }
            // Меняем строки местами
            double *temp = augmented[i];
            augmented[i] = augmented[swap_row];
            augmented[swap_row] = temp;
        }

        // Делим текущую строку на главный элемент, чтобы получить единицу на главной диагонали
        double pivot = augmented[i][i];
        for (int j = 0; j < 2 * n; j++) {
            augmented[i][j] /= pivot;
        }

        // Вычитаем текущую строку из остальных строк с умножением на соответствующий элемент, чтобы получить нули под главной диагональю
        for (int j = 0; j < n; j++) {
            if (j != i) {
                double factor = augmented[j][i];
                for (int k = 0; k < 2 * n; k++) {
                    augmented[j][k] -= factor * augmented[i][k];
                }
            }
        }
    }

    // Копируем обратную матрицу из расширенной матрицы
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = augmented[i][j + n];
        }
    }

    // Освобождаем память, выделенную для расширенной матрицы
    for (int i = 0; i < n; i++) {
        free(augmented[i]);
    }
    free(augmented);
}

void input(double **matrix, int n)
{
    printf("Введите элементы матрицы:\n");
    for (int i = 0; i < n; i++) {
        printf("Строка %d: ", i + 1);
        for (int j = 0; j < n; j++) {
            scanf("%lf", &matrix[i][j]);
        }
    }
}

void output(double **matrix, int n)
{
    printf("Обратная матрица:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%.6f ", matrix[i][j]);
        }
        printf("\n");
    }
}
