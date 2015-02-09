#include "stdafx.h"
#include <iostream>
#include <ctime>
#include <windows.h>
#include <glut.h>
#include <vector>
#include <map>
#include "Aquarium.h"
using namespace std;

double x_m = 1300, y_m = 650;
double size = 10;
Aquarium a1{ XMax(x_m), YMax(y_m), FishSize(size) };

void circle(int x, int y, unsigned c)
{
	switch (c)
	{
	case 1: glColor3f(1, 0, 0); break;
	case 2: glColor3f(0, 1, 0); break;
	case 3: glColor3f(0, 0, 1); break;
	case 4: glColor3f(0, 1, 1); break;
	}
	glBegin(GL_QUADS);
	glVertex2f(x + size / 2, y + size / 2);
	glVertex2f(x + size / 2, y - size / 2);
	glVertex2f(x - size / 2, y - size / 2);
	glVertex2f(x - size / 2, y + size / 2);
	glEnd();
}

void Draw()
{

	// обновление изобржения
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);

	unsigned c = 1;
	auto koord = a1.locations();
	for (const auto &a : koord) {
		for (const auto &b : a.second) {
			circle(b.x, b.y, c);
		}
		++c;
	}

	glFlush();
}

void timer(int = 0)
{
	a1.step();
	cout << a1.current_time() << endl;

	Draw();
	glutTimerFunc(50, timer, 0);
}

int _tmain(int argc, char **argv)
{
	glutInit(&argc, argv);
	srand(time(0));

	a1.add_type(FishTypeName("hish"), SpeedWalk(6), SpeedRun(8), Vision(110), BirthFrequency(700), Lifetime(800), MaxCount(10), Settled(false));
	a1.add_type(FishTypeName("hish2"), SpeedWalk(6), SpeedRun(8), Vision(110), BirthFrequency(700), Lifetime(800), MaxCount(10), Settled(false));
	a1.add_type(FishTypeName("trav"), SpeedWalk(5), SpeedRun(7.5), Vision(85), BirthFrequency(100), Lifetime(600), MaxCount(10), Settled(true));
	a1.add_type(FishTypeName("trav2"), SpeedWalk(5), SpeedRun(7), Vision(70), BirthFrequency(100), Lifetime(500), MaxCount(10), Settled(false));
	
	a1.add_fish(FishTypeName("hish"), Location(X(300), Y(300), A(0)));
	a1.add_fish(FishTypeName("hish"), Location(X(500), Y(400), A(90)));
	a1.add_fish(FishTypeName("hish2"), Location(X(300), Y(400), A(180)));
	a1.add_fish(FishTypeName("hish2"), Location(X(100), Y(300), A(0)));
	a1.add_fish(FishTypeName("hish"), Location(X(100), Y(500), A(0)));
	a1.add_fish(FishTypeName("trav"), Location(X(200), Y(300), A(0)));
	a1.add_fish(FishTypeName("trav"), Location(X(400), Y(400), A(0)));
	a1.add_fish(FishTypeName("trav"), Location(X(300), Y(310), A(0)));
	a1.add_fish(FishTypeName("trav2"), Location(X(400), Y(300), A(0)));
	a1.add_fish(FishTypeName("trav2"), Location(X(500), Y(190), A(0)));

	a1.set_diplomatic_status(FishTypeName("hish2"), FishTypeName("trav"), DiplomaticStatus(1));
	a1.set_diplomatic_status(FishTypeName("hish2"), FishTypeName("trav2"), DiplomaticStatus(2));
	a1.set_diplomatic_status(FishTypeName("hish"), FishTypeName("trav"), DiplomaticStatus(2));
	a1.set_diplomatic_status(FishTypeName("hish"), FishTypeName("trav2"), DiplomaticStatus(1));
	a1.set_diplomatic_status(FishTypeName("trav"), FishTypeName("hish"), DiplomaticStatus(-2));
	a1.set_diplomatic_status(FishTypeName("trav"), FishTypeName("hish2"), DiplomaticStatus(-1));
	a1.set_diplomatic_status(FishTypeName("trav2"), FishTypeName("hish2"), DiplomaticStatus(-2));
	a1.set_diplomatic_status(FishTypeName("trav2"), FishTypeName("hish"), DiplomaticStatus(-1));
	

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(x_m, y_m);
	glutInitWindowPosition(20, 50);
	glutCreateWindow("Аквариум");

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, x_m, 0, y_m, 0, 1);

	glutDisplayFunc(Draw);
	timer();
	glutMainLoop();

	return 0;
}

