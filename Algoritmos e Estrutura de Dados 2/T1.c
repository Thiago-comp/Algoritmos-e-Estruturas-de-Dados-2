#include "gfx.h"
#include <stdio.h>	/* printf */
#include <unistd.h> /* sleep  */


#define pi 3.1415926535

void apagaQuadrado(int x1, int y1, int x2, int y2) // funcao para apagar os quadrados internos
{
	gfx_set_color(0, 0, 0); // pinta o interno do quadrado de preto

	gfx_filled_rectangle(x1 + 1, y1 + 1, x2 - 1, y2 - 1); // apaga os quadrados internos
}

void FazerQuad(int x1, int y1, int x2, int y2, int recursionlevel) // funcao para criar quadrados
{

	gfx_rectangle(x1, y1, x2, y2); // desenha o primeiro nivel de recursao

	if (recursionlevel == 0) // encerra as recursoes
	{
		return;
	}

	// calculo para desenhar o quadrado da esquerda
	int Ex2 = x1;
	int Ey1 = y1 + (y2 - y1) / 4;
	int Ex1 = x1 - (x2 - x1) / 2;
	int Ey2 = y2 - (y2 - y1) / 4;

	// calculo para desenhar o quadrado de cima
	int Cy2 = y1;
	int Cy1 = y1 - (y2 - y1) / 2;
	int Cx1 = x1 + (x2 - x1) / 4;
	int Cx2 = x2 - (x2 - x1) / 4;

	// calculo para desenhar o quadrado da direita
	int Dx1 = x2;
	int Dy1 = y1 + (y2 - y1) / 4;
	int Dx2 = x2 + (x2 - x1) / 2;
	int Dy2 = y2 - (y2 - y1) / 4;

	// calculo para desenhar o quadrado de baixo
	int By1 = y2;
	int Bx2 = x2 - (x2 - x1) / 4;
	int Bx1 = x1 + (x2 - x1) / 4;
	int By2 = y2 + (y2 - y1) / 2;

	FazerQuad(Ex1, Ey1, Ex2, Ey2, recursionlevel - 1); // quadrado esquerda
	FazerQuad(Cx1, Cy1, Cx2, Cy2, recursionlevel - 1); // quadrado cima
	FazerQuad(Dx1, Dy1, Dx2, Dy2, recursionlevel - 1); // quadrado direita
	FazerQuad(Bx1, By1, Bx2, By2, recursionlevel - 1); // quadrado baixo

	apagaQuadrado(x1, y1, x2, y2); // apaga os quadrados internos

	gfx_set_color(255, 255, 255); // coloca como branco a cor dos desenhos

	gfx_rectangle(Ex1, Ey1, Ex2, Ey2); // desenha o quadrado da esquerda
	gfx_rectangle(Cx1, Cy1, Cx2, Cy2); // desenha o quadrado de cima
	gfx_rectangle(Dx1, Dy1, Dx2, Dy2); // desenha o quadrado da direita
	gfx_rectangle(Bx1, By1, Bx2, By2); // desenha o quadrado de baixo
}

void FazerCirc(float x, float y, float raio, unsigned n) // funcao para criar circulos
{
	gfx_ellipse(x, y, raio, raio); // desenha o primeiro nivel de recursao

	if (n == 0) // encerra as recursoes
	{
		return;
	}
	
	float raioN = raio / 2; // Cálculo para dividir o raio original pela metade

	float x1 = raioN * cos(pi/4) + x; // Calcula a coordenada x do primeiro ponto
	float y1 = raioN * sin(pi/4) + y; // Calcula a coordenada y do primeiro ponto
	float x2 = raioN * cos(5*pi/4) + x; // Calcula a coordenada x do segundo ponto
	float y2 = raioN * sin(5*pi/4) + y; // Calcula a coordenada y do segundo ponto

	// Chamada Recursiva
	FazerCirc(x1, y1, raioN, n - 1);
	FazerCirc(x1, y2, raioN, n - 1);
	FazerCirc(x2, y1, raioN, n - 1);
	FazerCirc(x2, y2, raioN, n - 1);

	gfx_ellipse(x, y, raio, raio); // desenha os circulos
}

int main()
{
	int p, escolha;

	printf("Informe a figura desejada \n");
	printf("1- Circulo\n2- Quadrado\n");
	scanf("%d", &escolha);

	if (escolha == 1) // roda a funcao do circulo
	{

		printf("Informe o nivel de recursao \n");
		scanf("%d", &p);
		if (p >= 0 && p <= 10) // nivel de recursao tem que estar entre 0 e 10
		{
			gfx_init(600, 600, "Circulo");
			FazerCirc(300, 300, 270, p); // coordenadas do circulo

			gfx_paint();

			sleep(5);

			gfx_quit();
		}
		else
		{

			printf("Numero de recursoes invalidos\n");
		}
	}
	if (escolha == 2) // roda a funcao do quadrado
	{
		printf("informe o nivel de recursao \n");
		scanf("%d", &p);
		if (p >= 0 && p <= 10)
		{
			gfx_init(600, 600, "quadrado");
			FazerQuad(200, 200, 400, 400, p); // coordenadas do quadrado

			gfx_paint();

			sleep(5);
			gfx_quit();
		}
		
	
		else
		{
			printf("numero de recursoes invalidas");
		}
	}

	return 0;
}
