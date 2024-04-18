#include <iostream>
#include <fstream>
#include <cmath>

// y(x)-1/2 int_0_1{xty(t)dt} = 5/6 x

float integralFunction(float x, float t)
{
	return -0.5 * x * t;
}
float Fun(float x)
{
	return 5 * x / 6;
}

float ** createArr(int n)
{
	float** slu = new float* [n + 1]; // тк количество переменных на 1 больше чем разбиения на узлы

	for (int i = 0; i < n + 1; i++)
	{
		slu[i] = new float[n + 2]; // n+2 тк +1 стлб свободных членов
	}

	return slu;
}
float* createArrSol(int k)
{
	float* slu = new float [k]; // 1 строка - название решения; 2ая - численное значение решения
	
	return slu;
}
void printDDArr(float** Arr, int rows, int cols)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (Arr[i][j] == 0)
			{
				std::cout << abs(Arr[i][j]) << '\t';
			}
			else
			{
				std::cout << Arr[i][j] << '\t';
			}
		}
		std::cout << '\n';
	}
	std::cout << std::endl;
}
void deleteArr(float** Arr, int n)
{
	for (int i = 0; i < n + 1; i++)
	{
		delete[] Arr[i];
	}
	delete[] Arr;
}

// РЕШЕНИЕ СЛУ
float fill_slu (float** slu, float a, float b, int n)
{
	slu[0][0] = 1;
	for (int i = 1; i < n + 1; i++)
	{
		slu[i][0] = 0;
	}

	float h = (b - a) / n; // b и a - пределы интегрирования
	float x = a;
	float t = a+=h;
	for (int i = 0; i < n+1; i++)
	{
		for (int j = 1; j < n + 2; j++)
		{
			if (x == t && x!=0) // для повторяющихся неизвестных
			{
				slu[i][j] = 1 + h * integralFunction(x, t);
			}
			else if (j == n + 1) // столбец свободных членов
			{
				slu[i][j] = Fun(x);
			}
			else
			{
				slu[i][j] = h * integralFunction(x, t);
			}

			t += h;
		}

		x += h;
		t = a;
	}
	return h;
}
void swapRows(float** Arr, int index1, int index2, int cols)
{
	for (int i = 0; i < cols; i++)
	{
		std::swap(Arr[index1][i], Arr[index2][i]);
	}
}
void methodGaussa(float** Array, int rows, int cols)
{
	for (int i = 0; i < rows - 1; i++)
	{

		int iMax = i;

		for (int j = i + 1; j < rows; j++)
		{
			if (abs(Array[j][i]) > abs(Array[iMax][i]))
			{
				iMax = j;
			}
		}

		if (i != iMax)
		{
			swapRows(Array, i, iMax, cols);
		}

		for (int k = i + 1; k < rows; k++)
		{
			float tmp = Array[k][i];

			for (int s = i; s < cols; s++)
			{
				float koef = Array[i][s] / Array[i][i];
				Array[k][s] -= koef * tmp;
				Array[k][s] = Array[k][s];
			}
		}
	}

	for (int i = 0; i < rows; i++) // нормализация 
	{
		float temp = Array[i][i];

		if (temp!=0)
		{
			for (int j = 0; j < cols; j++)
			{
				Array[i][j] /= temp;
			}
		}
	}
}
void solveSLU(float** slu, int k, float h) // k - количество неизвестных в слу
{
	float* solution = createArrSol(k);
	
	float sum{0};

	for (int i = k - 1; i >= 0; i--)
	{
		for (int j = i; j < k-1; j++)
		{
			sum += solution[j+1] * slu[i][j+1];
		}

		solution[i] = slu[i][k] - sum;
		sum = 0;
	}
	

	std::cout << "\n";
	float a = 0;

	std::ofstream f;
	f.open("./solve.txt",std::ios_base::app);

	for (float i = 0; i < k; i ++)
	{

		f << a << "\t\t";
		a += h;
	}

	f << "\n";

	for (int i = 0; i < k; i++)
	{
		f << std::round(solution[i]*10000)/10000 << "\t";
	}

	f << "\n\n";

	f.close();
	delete[] solution;
}

// ИТОГОВАЯ ФУНКЦИЯ
void solveFredgolmSecond(int kol) // сколько раз увеличивать количество узлов
{
	int s = 0;
	int n = 4; 
	while (s < kol)
	{
		// количество узлов
		float a(0), b(1); // начало и конец интегрирования
		float** eq = createArr(n);
		float h = fill_slu(eq, a, b, n);
		methodGaussa(eq, n + 1, n + 2);
		solveSLU(eq, n + 1, h);
		n *= 2; // увеличиваем количество узлов в 2 раза
		s++;
		std::cout << "\n\n";
		
	}

}

int main()
{
	int k{ 2 }; // сколько раз решить уравнение, увеличивая количество узлов

	solveFredgolmSecond(k);

	std::cout << "\n";
	system("pause");
	return 0;
}