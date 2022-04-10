#include <stdio.h>


int main()
{
	float salario;
	int i = 0;
	for (i = 0; i < 5; ++i)
	{
		printf("entre com o salario: ");
		scanf("%f", &salario);
		printf("salario = %f\n", salario);
	}
	return 0;
}
		
