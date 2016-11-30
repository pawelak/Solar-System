/*************************************************************************************/
// zadanie domowe - uk³ad s³oneczny
/*************************************************************************************/

#define _USE_MATH_DEFINES

#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <vector>
#include <iostream>
#include <conio.h>
#include <math.h>
#include <iomanip>
#include <time.h>



using namespace std;

/*************************************************************************************/

//do myszy-----------------------------------
typedef float point3[3];
static GLfloat viewer[] = { 0.0, 0.0, 50.0 };
static GLfloat pointOfObservtion[] = { 0.0, 0.0, 0.0 };
static GLfloat vectorOfUp[] = { 0.0, 1.0, 0.0 };

static GLfloat thetax = 0.0;   // k?t obrotu obiektu
static GLfloat thetay = 0.0;   // k?t obrotu obiektu
static GLfloat thetaz = 0.0;   // k?t obrotu obiektu

static GLfloat pix2anglex;     // przelicznik pikseli na stopnie
static GLfloat pix2angley;
static GLfloat pix2anglez;
static GLint status = 0;      // stan klawiszy myszy 
static int x_pos_old = 0, y_pos_old = 0, z_pos_old = 0;       // poprzednia pozycja kursora myszy

static float delta_x = 0, delta_y = 0, delta_z = 0;       // ró?nica pomi?dzy pozycj? bie??c?

float alfa = 0, beta = 0;
float alfaR = 0, betaR = 0;
double r = 0;
int test = 0;
float alfaP = 0, betaP = 0;
//--------------------------------------------



// Funkcaja okreœlaj¹ca, co ma byæ rysowane 
// (zawsze wywo³ywana, gdy trzeba przerysowaæ scenê)

int xs = 0, ys = 0;
float xP = 0, yP = 0, zP = 0;
//const int pointsNumber = 1000;
typedef float point3[3];
vector <int> pozycja = { 0,0,0,0,0,0,0,0,0 };

//vector <double> distanceFromSun = { 0,5.79,10.82,14.96,22.79,77.84,142.67,287.01,450 };		// odleg³oœci podzielone przez 10000000
//vector <int> orbitalPeriod = { 0,88,224,365,686,4333,10759,30707,60223 };
//vector <double> planetSize = { 0.07,0.00025,0.0006,0.00065,0.00035,0.00715,0.0060,0.0025,0.0025 };

vector <double> planetSize = { 0,1,1,1.4,1.2,3,2.5,2.0,1.8,1.7 };
vector <int> orbitalPeriod = { 0,400,1240,2650,3860,5330,7590,9070,12230 };
vector <double> distanceFromSun = { 0,5,8,12,16,30,40,60,80 };
int timeOrbit = 10;
point3 elips[9][100000];


void Axes(void)
{
	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };
	// pocz¹tek i koniec obrazu osi x

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0,  5.0, 0.0 };
	// pocz¹tek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0,  5.0 };
	//  pocz¹tek i koniec obrazu osi y
	glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania osi - czerwony
	glBegin(GL_LINES); // rysowanie osi x
	glVertex3fv(x_min);
	glVertex3fv(x_max);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);  // kolor rysowania - zielony
	glBegin(GL_LINES);  // rysowanie osi y
	glVertex3fv(y_min);
	glVertex3fv(y_max);
	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
	glBegin(GL_LINES); // rysowanie osi z
	glVertex3fv(z_min);
	glVertex3fv(z_max);
	glEnd();
}

void fillEllipse(int planeta, int a, int b, int ile)
{
	for (int i = 0; i < orbitalPeriod[planeta]; i++)
	{
		elips[planeta][i][0] = xs + a * cos(6.28318 * i / ile);
		elips[planeta][i][1] = ys + b * sin(6.28318 * i / ile);
		elips[planeta][i][2] = 0;
	}
}

void paint()
{
	glPointSize(0.1);
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < orbitalPeriod[i]; j++)
		{
			glBegin(GL_POINTS);
			glColor3d(1, 1, 1);
			glVertex3fv(elips[i][j]);
			glEnd();

		}
	}
}

void Sun()
{
	glColor3f(1.0, 1.0, 0);
	glPushMatrix();
	glutSolidSphere(3, 50, 50);
	glPopMatrix();
}

void Planets()
{
	//ustawienie cienia na obwodzie dla wszytkich planet 
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	//Merkury
	glPushMatrix();
	glColor3f(1.0, 0.2, 0.0);
	glTranslatef(elips[1][pozycja[1]][0], elips[1][pozycja[1]][1], elips[1][pozycja[1]][2]);
	glutSolidSphere(planetSize[1], 100, 100);
	glPopMatrix();
	//Wenus
	glPushMatrix();
	glColor3f(0.0, 0.1, 0.8);
	glTranslatef(elips[2][pozycja[2]][0], elips[2][pozycja[2]][1], elips[2][pozycja[2]][2]);
	glutSolidSphere(planetSize[2], 100, 100);
	glPopMatrix();
	//Ziemia
	glPushMatrix();
	glColor3f(0.0, 0.6, 0.4);
	glTranslatef(elips[3][pozycja[3]][0], elips[3][pozycja[3]][1], elips[3][pozycja[3]][2]);
	glutSolidSphere(planetSize[3], 100, 100);
	glPopMatrix();
	//mars
	glPushMatrix();
	glColor3f(1.0, 0.2, 0.0);
	glTranslatef(elips[4][pozycja[4]][0], elips[4][pozycja[4]][1], elips[4][pozycja[4]][2]);
	glutSolidSphere(planetSize[4], 100, 100);
	glPopMatrix();
	//Jowisz
	glPushMatrix();
	glColor3f(0.7, 0.2, 0.2);
	glTranslatef(elips[5][pozycja[5]][0], elips[5][pozycja[5]][1], elips[5][pozycja[5]][2]);
	glutSolidSphere(planetSize[5], 100, 100);
	glPopMatrix();
	//Saturn
	glPushMatrix();
	glColor3f(0.6, 0.1, 0.2);
	glTranslatef(elips[6][pozycja[6]][0], elips[6][pozycja[6]][1], elips[6][pozycja[6]][2]);
	glutSolidSphere(planetSize[6], 100, 100);
	glPopMatrix();
	//Uran
	glPushMatrix();
	glColor3f(0.2, 0.2, 0.8);
	glTranslatef(elips[7][pozycja[7]][0], elips[7][pozycja[7]][1], elips[7][pozycja[7]][2]);
	glutSolidSphere(planetSize[7], 100, 100);
	glPopMatrix();
	//Neptun
	glPushMatrix();
	glColor3f(0.1, 0.1, 0.9);
	glTranslatef(elips[8][pozycja[8]][0], elips[8][pozycja[8]][1], elips[8][pozycja[8]][2]);
	glutSolidSphere(planetSize[8], 100, 100);
	glPopMatrix();


}

void makeOrbits()
{
	for (int i = 0; i < 9; i++)
	{
		fillEllipse(i, distanceFromSun[i], distanceFromSun[i], orbitalPeriod[i]);
	}
}

void move()
{
	for (int i = 0; i < 9; i++)
	{
		pozycja[i]++;
		if (pozycja[i] > orbitalPeriod[i])pozycja[i] = 0;
		if (test == 1)
		{
			viewer[0] = elips[1][pozycja[1]][0];
			viewer[1] = elips[1][pozycja[1]][1];
			viewer[2] = elips[1][pozycja[1]][2]+planetSize[1];
			vectorOfUp[0] = 0.0;
			vectorOfUp[1] = 0.0;
			vectorOfUp[2] = 1.0;
		}
		if (test ==2)
		{
			viewer[0] = elips[2][pozycja[1]][0];
			viewer[1] = elips[2][pozycja[1]][1];
			viewer[2] = elips[2][pozycja[1]][2] + planetSize[2];
			vectorOfUp[0] = 0.0;
			vectorOfUp[1] = 0.0;
			vectorOfUp[2] = 1.0;
		}
		if (test == 3)
		{
			viewer[0] = elips[3][pozycja[1]][0];
			viewer[1] = elips[3][pozycja[1]][1];
			viewer[2] = elips[3][pozycja[1]][2] + planetSize[3];
			vectorOfUp[0] = 0.0;
			vectorOfUp[1] = 0.0;
			vectorOfUp[2] = 1.0;
		}
		if (test == 4)
		{
			viewer[0] = elips[4][pozycja[1]][0];
			viewer[1] = elips[4][pozycja[1]][1];
			viewer[2] = elips[4][pozycja[1]][2] + planetSize[4];
			vectorOfUp[0] = 0.0;
			vectorOfUp[1] = 0.0;
			vectorOfUp[2] = 1.0;
		}
		if (test == 5)
		{
			/*viewer[0] = elips[5][pozycja[1]][0];
			viewer[1] = elips[5][pozycja[1]][1];
			viewer[2] = elips[5][pozycja[1]][2] + planetSize[5];*/
			float a=viewer[0], c=viewer[1], b=viewer[2];
		
			viewer[0] = ((planetSize[5]+0.1) * cos(alfaP) * cos(betaP))+elips[5][pozycja[5]][0];
			viewer[1] = ((planetSize[5]+0.1) * sin(betaP)) +elips[5][pozycja[5]][1];
			viewer[2] = ((planetSize[5]+0.1) * sin(alfaP) * cos(betaP))+ elips[5][pozycja[5]][2];

			vectorOfUp[0] = viewer[0]-elips[5][pozycja[5]][0];
			vectorOfUp[1] = viewer[1]-elips[5][pozycja[5]][1];
			vectorOfUp[2] = viewer[2]-elips[5][pozycja[5]][2];

			pointOfObservtion[0] = (viewer[0] - a)*10;
			pointOfObservtion[1] = (viewer[1] - b)*10;
			pointOfObservtion[2] = (viewer[2] - c)*10;

			
		}
		if (test == 6)
		{
			viewer[0] = ((planetSize[6] + 0.1) * cos(alfaP) * cos(betaP)) + elips[6][pozycja[6]][0];
			viewer[1] = ((planetSize[6] + 0.1) * sin(betaP)) + elips[6][pozycja[6]][1];
			viewer[2] = ((planetSize[6] + 0.1) * sin(alfaP) * cos(betaP)) + elips[6][pozycja[6]][2];

			pointOfObservtion[0] = ((planetSize[6] + 10) * cos(alfaP) * cos(betaP)) + elips[6][pozycja[6]][0];
			pointOfObservtion[1] = ((planetSize[6] + 10) * sin(betaP)) + elips[6][pozycja[6]][1];
			pointOfObservtion[2] = ((planetSize[6] + 10) * sin(alfaP) * cos(betaP)) + elips[6][pozycja[6]][2];
			
			vectorOfUp[0] = 0.0;
			vectorOfUp[1] = 0.0;
			vectorOfUp[2] = 1.0;
		}
		if (test == 7)
		{
			viewer[0] = elips[7][pozycja[1]][0];
			viewer[1] = elips[7][pozycja[1]][1];
			viewer[2] = elips[7][pozycja[1]][2] + planetSize[7];
			vectorOfUp[0] = 0.0;
			vectorOfUp[1] = 0.0;
			vectorOfUp[2] = 1.0;
		}
		if (test == 8)
		{
			viewer[0] = elips[8][pozycja[1]][0];
			viewer[1] = elips[8][pozycja[1]][1];
			viewer[2] = elips[8][pozycja[1]][2] + planetSize[8];
		}
		if (test == 9)
		{
			viewer[0] = 0;
			viewer[1] = 0;
			viewer[2] = 50;
			vectorOfUp[0] = 0.0;
			vectorOfUp[1] = 1.0;
			vectorOfUp[2] = 0.0;

		}
		
		
		
	}
	Sleep(timeOrbit);
	glutPostRedisplay(); //odœwie¿enie zawartoœci aktualnego okna
}



void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz¹cym

	glLoadIdentity();

	gluLookAt(viewer[0], viewer[1], viewer[2], pointOfObservtion[0], pointOfObservtion[1], pointOfObservtion[3], vectorOfUp[0], vectorOfUp[1], vectorOfUp[2]);
	//glRotated(20.0, 1.0, 1.0, 0.0);  // Obrót o 60 stopni
	//Axes();
	makeOrbits();
	Sun();
	paint();
	Planets();
	move();
	
	if (status == 1)                    // je?li lewy klawisz myszy wci?ni?ty
	{
		r = sqrt(pow(viewer[0], 2) + pow(viewer[1], 2) + pow(viewer[2], 2));
		alfa += delta_x*pix2anglex / 50;    // modyfikacja k?ta obrotu o k?t proporcjonalny
		beta += delta_y*pix2angley / 50;
		alfaR = (alfa * M_PI) / 180.0f;
		betaR = (beta * M_PI) / 180.0f;
		if (alfaR > 2 * M_PI)alfaR -= 2 * M_PI;
		if (betaR > 2 * M_PI)betaR -= 2 * M_PI;

		pointOfObservtion[0] = (r * cos(alfa) * cos(beta));
		pointOfObservtion[1] = (r*sin(beta));
		pointOfObservtion[2] = (r * sin(alfa) * cos(beta));



	}                                 // do ró?nicy po?o?e? kursora myszy
	else if (status == 2)                    // je?li lewy klawisz myszy wci?ni?ty
	{
		viewer[2] += delta_z / 10;
	}






	glFlush();
	// Przekazanie poleceñ rysuj¹cych do wykonania

	glutSwapBuffers();
	//
}

//zapas
//void RenderScene(void)
//{
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	// Czyszczenie okna aktualnym kolorem czyszcz¹cym
//	glLoadIdentity();
//	Axes();
//	glRotated(60.0, 0.0, 1.0, 0.0);
//	//makeOrbits();
//	//paint();
//	//Sun();
//	//Planets();
//	//gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
//	//if (status == 1)                    // je?li lewy klawisz myszy wci?ni?ty
//	//{
//	//	r = sqrt(pow(viewer[0], 2) + pow(viewer[1], 2) + pow(viewer[2], 2));
//	//	alfa += delta_x*pix2anglex / 50;    // modyfikacja k?ta obrotu o k?t proporcjonalny
//	//	beta += delta_y*pix2angley / 50;
//	//	alfaR = (alfa * M_PI) / 180.0f;
//	//	betaR = (beta * M_PI) / 180.0f;
//	//	if (alfaR > 2 * M_PI)alfaR -= 2 * M_PI;
//	//	if (betaR > 2 * M_PI)betaR -= 2 * M_PI;
//	//	cout << delta_x << " " << delta_y << endl;
//	//	cout << alfa << " " << beta << endl;
//
//	//	viewer[0] = (r * cos(alfa) * cos(beta));
//	//	viewer[1] = (r*sin(beta));
//	//	viewer[2] = (r * sin(alfa) * cos(beta));
//
//	//}                                 // do ró?nicy po?o?e? kursora myszy
//	//else if (status == 2)                    // je?li lewy klawisz myszy wci?ni?ty
//	//{
//	//	viewer[2] += delta_z / 10;
//	//}
//	//dis();
//	
//	//glColor3f(1.0f, 1.0f, 0.0f);
//	//glutSolidSphere(10, 50, 50);
//	//glPushMatrix();
//
//	glFlush();
//	glutSwapBuffers();
//}
/*************************************************************************************/
// Funkcja ustalaj¹ca stan renderowania

void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// Kolor okna wnêtrza okna - ustawiono na czarnt
}
/*************************************************************************************/
// Funkcja s³u¿¹ca do kontroli zachowania proporcji rysowanych obiektów
// niezale¿nie od rozmiarów okna graficznego

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2anglex = 360.0 / (float)horizontal; // przeliczenie pikseli na stopnie
	pix2angley = 360.0 / (float)vertical; // przeliczenie pikseli na stopnie

	GLfloat AspectRatio;
	// Deklaracja zmiennej AspectRatio  okreœlaj¹cej proporcjê
	// wymiarów okna 
	if (vertical == 0)  // Zabezpieczenie przed dzieleniem przez 0
		vertical = 1;

	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielkoœciokna okna widoku (viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)  
	glMatrixMode(GL_PROJECTION);
	// Prze³¹czenie macierzy bie¿¹cej na macierz projekcji 
	glLoadIdentity();

	gluPerspective(120, 1.0, 000.1, 1000.0);
	// Ustawienie parametrów dla rzutu perspektywicznego

	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
	// Ustawienie wielko?ci okna okna widoku (viewport) w zale?no?ci
	// relacji pomi?dzy wysoko?ci? i szeroko?ci? okna

	glMatrixMode(GL_MODELVIEW);
	// Prze??czenie macierzy bie??cej na macierz widoku modelu  

	glLoadIdentity();
	// Czyszczenie macierzy bie??cej 
}
//zapas
//void ChangeSize(GLsizei horizontal, GLsizei vertical)
//// Parametry horizontal i vertical (szerokoœæ i wysokoœæ okna) s¹
//// przekazywane do funkcji za ka¿dym razem, gdy zmieni siê rozmiar okna
//{
//	pix2anglex = 360.0 / (float)horizontal; // przeliczenie pikseli na stopnie
//	pix2angley = 360.0 / (float)vertical; // przeliczenie pikseli na stopnie1
//
//	GLfloat AspectRatio;
//	// Deklaracja zmiennej AspectRatio okreœlaj¹cej proporcjê wymiarów okna
//
//	if (vertical == 0)
//		// Zabezpieczenie pzred dzieleniem przez 0
//		vertical = 1;
//
//	glViewport(0, 0, horizontal, vertical);
//	// Ustawienie wielkoœciokna okna urz¹dzenia (Viewport)
//	// W tym przypadku od (0,0) do (horizontal, vertical)
//
//	glMatrixMode(GL_PROJECTION);
//	// Okreœlenie uk³adu wspó³rzêdnych obserwatora
//	glLoadIdentity();
//
//	gluPerspective(1.0, 1, 1.0, 30.0);
//	// Ustawienie parametrów dla rzutu perspektywicznego
//
//	if (horizontal <= vertical)
//		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);
//
//	else
//		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
//	// Ustawienie wielko?ci okna okna widoku (viewport) w zale?no?ci
//	// relacji pomi?dzy wysoko?ci? i szeroko?ci? okna
//
//	glMatrixMode(GL_MODELVIEW);
//	// Prze??czenie macierzy bie??cej na macierz widoku modelu  
//
//	glLoadIdentity();
//	// Czyszczenie macierzy bie??cej 
//}



void keys(unsigned char key, int x, int y)
{
	if (key == 'w') viewer[1] += 1;
	if (key == 's') viewer[1] -= 1;
	if (key == 'a') viewer[0] -= 1;
	if (key == 'd') viewer[0] += 1;
	if (key == 'q') viewer[2] += 1;
	if (key == 'e') viewer[2] -= 1;
	if (key == '1') test = 1;
	if (key == '2') test = 2;
	if (key == '3') test = 3;
	if (key == '4') test = 4;
	if (key == '5') test = 5;
	if (key == '6') test = 6;
	if (key == '7') test = 7;
	if (key == '8') test = 8;
	if (key == '9') test = 9;
	if (key == 'i') alfaP += 0.2;
	if (key == 'k') alfaP -= 0.2;
	if (key == 'j') betaP -= 0.2;
	if (key == 'l') betaP += 0.2;
	



	RenderScene(); // przerysowanie obrazu sceny
	ChangeSize(800, 800);
}

void Mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;        // przypisanie aktualnie odczytanej pozycji kursora 
							  // jako pozycji poprzedniej
		status = 1;         // wci?ni?ty zosta? lewy klawisz myszy
	}
	else if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		y_pos_old = y;        // przypisanie aktualnie odczytanej pozycji kursora 
							  // jako pozycji poprzedniej
		status = 1;         // wci?ni?ty zosta? lewy klawisz myszy
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		z_pos_old = x;        // przypisanie aktualnie odczytanej pozycji kursora 
							  // jako pozycji poprzedniej
		status = 2;         // wci?ni?ty zosta? lewy klawisz myszy
	}
	else
		status = 0;         // nie zosta? wci?ni?ty ?aden klawisz 
}

void Motion(GLsizei x, GLsizei y)
{
	delta_x = x - x_pos_old;    // obliczenie ró?nicy po?o?enia kursora myszy
	x_pos_old = x;            // podstawienie bie?acego po?o?enia jako poprzednie
	delta_y = y - y_pos_old;    // obliczenie ró?nicy po?o?enia kursora myszy
	y_pos_old = y;            // podstawienie bie?acego po?o?enia jako poprzednie
	delta_z = x - z_pos_old;    // obliczenie ró?nicy po?o?enia kursora myszy
	z_pos_old = x;
	glutPostRedisplay();    // przerysowanie obrazu sceny
}

/*************************************************************************************/
// G³ówny punkt wejœcia programu. Program dzia³a w trybie konsoli

void main(void)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	// Ustawienie trybu wyœwietlania
	// GLUT_SINGLE - pojedynczy bufor wyœwietlania
	// GLUT_RGBA - model kolorów RGB

	glutInitWindowSize(800, 800);

	glutCreateWindow("Uk³ad s³oneczny");
	// Utworzenie okna i okreœlenie treœci napisu w nag³ówku okna

	glutKeyboardFunc(keys);
	glutIdleFunc(move);

	glutDisplayFunc(RenderScene);
	// Okreœlenie, ¿e funkcja RenderScene bêdzie funkcj¹ zwrotn¹ (callback)
	// Biblioteka GLUT bêdzie wywo³ywa³a t¹ funkcjê za ka¿dym razem, gdy
	// trzeba bêdzie przerysowaæ okno

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcjê zwrotn¹ odpowiedzialn¹ za
	// zmiany rozmiaru okna
	glutMouseFunc(Mouse);
	// Ustala funkcj? zwrotn? odpowiedzialn? za badanie stanu myszy

	glutMotionFunc(Motion);
	// Ustala funkcj? zwrotn? odpowiedzialn? za badanie ruchu myszy


	MyInit();
	// Funkcja MyInit (zdefiniowana powy¿ej) wykonuje wszelkie  
	// inicjalizacje konieczneprzed przyst¹pieniem do renderowania

	glEnable(GL_DEPTH_TEST);
	// W³¹czenie mechanizmu usuwania powierzchni niewidocznych
	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}

/*************************************************************************************/