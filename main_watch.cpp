#include <vector>
#include <map>
#include <string>
#include <future>
#include <chrono>
#include <thread>
#include <stdexcept>
#include <iostream>
#include <SFML\Graphics.hpp>
#include "aquarium.h"

using std::map;
using std::vector;
using std::string;
using std::pair;
using std::future;
using std::async;
using std::exception;
using std::cout;
using std::cin;
using std::endl;

using sf::RenderWindow;
using sf::VideoMode;
using sf::Event;
using sf::RectangleShape;
using sf::Color;
using sf::Vector2f;

// основные типы:
using aquarium::Aquarium;				// класс аквариума
using aquarium::FishType;				// класс типа рыб
using aquarium::Fish;					// класс рыбы

// для аквариума:
using aquarium::XMax;					// класс ширины аквариума
using aquarium::YMax;					// класс высоты аквариума
using aquarium::FishSize;				// класс размера рыб

// для типа рыб:
using aquarium::FishTypeName;			// класс имени типа рыб
using aquarium::SpeedWalk;				// класс скорости в спокойном состоянии
using aquarium::SpeedRun;				// класс скорости во время погони
using aquarium::Vision;					// класс радиуса зрения
using aquarium::BirthFrequency;			// класс частоты рождения новых рыб
using aquarium::Lifetime;				// класс продолжительности жизи
using aquarium::MaxCount;				// класс максимального кол-ва рыб определенного типа
using aquarium::Settled;				// класс, отвечающий за оседлость типа рыб

// для одной рыбы:
using aquarium::Location;				// класс местоположение
using aquarium::X;						// класс координата х
using aquarium::Y;						// класс координата у
using aquarium::A;						// класс угол (в градусах)

// для дипломатии:
using aquarium::DiplomaticStatus;		// класс отношений между типами рыб


// размеры аквариума
const double x_m = 1300, y_m = 900;
// размер рыбы
const double fish_size = 10;

int main() {

	try {

		// сам аквариум
		Aquarium a1{ XMax(x_m), YMax(y_m), FishSize(fish_size) };

		// добавляем типы рыб
		a1.add_type(FishTypeName("main_hish"), SpeedWalk(7), SpeedRun(8.5), Vision(140), BirthFrequency(600), Lifetime(1200), MaxCount(3), Settled(false));
		a1.add_type(FishTypeName("hish"), SpeedWalk(6), SpeedRun(8), Vision(110), BirthFrequency(270), Lifetime(800), MaxCount(25), Settled(false));
		a1.add_type(FishTypeName("hish2"), SpeedWalk(6), SpeedRun(7.8), Vision(100), BirthFrequency(240), Lifetime(700), MaxCount(25), Settled(false));
		a1.add_type(FishTypeName("trav"), SpeedWalk(5), SpeedRun(7.5), Vision(85), BirthFrequency(150), Lifetime(600), MaxCount(50), Settled(true));
		a1.add_type(FishTypeName("trav2"), SpeedWalk(5), SpeedRun(7), Vision(70), BirthFrequency(110), Lifetime(500), MaxCount(90), Settled(false));

		// добавляем самих рыб
		a1.add_fish(FishTypeName("main_hish"), Location(X(1000), Y(300), A(0)));
		a1.add_fish(FishTypeName("main_hish"), Location(X(1000), Y(400), A(180)));
		for (unsigned u = 0; u != 6; ++u) {
			a1.add_fish(FishTypeName("hish"), Location(X(300), Y(400), A(111)));
			a1.add_fish(FishTypeName("hish"), Location(X(300), Y(400), A(140)));
			a1.add_fish(FishTypeName("hish2"), Location(X(700), Y(400), A(160)));
			a1.add_fish(FishTypeName("hish2"), Location(X(700), Y(300), A(0)));
			a1.add_fish(FishTypeName("hish2"), Location(X(700), Y(500), A(0)));
			a1.add_fish(FishTypeName("trav"), Location(X(900), Y(70), A(0)));
			a1.add_fish(FishTypeName("trav"), Location(X(900), Y(60), A(0)));
			a1.add_fish(FishTypeName("trav"), Location(X(900), Y(50), A(0)));
			a1.add_fish(FishTypeName("trav"), Location(X(900), Y(40), A(0)));
			a1.add_fish(FishTypeName("trav"), Location(X(900), Y(50), A(0)));
			a1.add_fish(FishTypeName("trav"), Location(X(900), Y(40), A(0)));
			a1.add_fish(FishTypeName("trav2"), Location(X(300), Y(200), A(0)));
			a1.add_fish(FishTypeName("trav2"), Location(X(300), Y(210), A(0)));
			a1.add_fish(FishTypeName("trav2"), Location(X(300), Y(220), A(0)));
			a1.add_fish(FishTypeName("trav2"), Location(X(300), Y(230), A(0)));
			a1.add_fish(FishTypeName("trav2"), Location(X(300), Y(220), A(0)));
			a1.add_fish(FishTypeName("trav2"), Location(X(300), Y(230), A(0)));
		}

		// устанавливаем отношения между типами рыб
		a1.set_diplomatic_status(FishTypeName("main_hish"), FishTypeName("hish"), DiplomaticStatus(2));
		a1.set_diplomatic_status(FishTypeName("main_hish"), FishTypeName("hish2"), DiplomaticStatus(1));
		a1.set_diplomatic_status(FishTypeName("hish"), FishTypeName("trav"), DiplomaticStatus(1));
		a1.set_diplomatic_status(FishTypeName("hish2"), FishTypeName("trav2"), DiplomaticStatus(1));
		a1.set_diplomatic_status(FishTypeName("hish"), FishTypeName("main_hish"), DiplomaticStatus(-1));
		a1.set_diplomatic_status(FishTypeName("hish2"), FishTypeName("main_hish"), DiplomaticStatus(-1));
		a1.set_diplomatic_status(FishTypeName("trav"), FishTypeName("hish"), DiplomaticStatus(-1));
		a1.set_diplomatic_status(FishTypeName("trav2"), FishTypeName("hish2"), DiplomaticStatus(-1));

		// получаем начальные местоположения рыб
		map<string, vector<Location>> fishes = a1.get_fish_locations();

		// для хранения результатов вычисления нового положения рыб
		future<map<string, vector<Location>>> step_result;

		// само окно 
		RenderWindow window(VideoMode(x_m, y_m), "_aquarium");

		while (window.isOpen()) {

			// обработка событий
			Event event;
			while (window.pollEvent(event)) {
				if (event.type == Event::Closed)
					window.close();
			}

			// рисуем картинку и параллельно создаем поток,
			// вычисляющий новое положение рыб, в результате следующего хода

			step_result = async([&a1] {
				// рыбы делают шаг
				a1.step();
				// получаем местоположения рыб,
				// а заодно и имена их типов, чтобы знать что рисовать
				return a1.get_fish_locations();
			});

			// очистка старого изображения
			window.clear(Color::White);

			// картинка рыбы (квадратик)
			RectangleShape fish_piture(Vector2f(fish_size, fish_size));
			// отображение рыб
			for (const pair<string, vector<Location>> &one_type : fishes) {
				// определяем с каким типом рыб мы имеем дело
				// и выбираем соответствующий цвет
				if (one_type.first == "main_hish") {
					fish_piture.setFillColor(Color::Black);
				}
				else if (one_type.first == "hish") {
					fish_piture.setFillColor(Color::Red);
				}
				else if (one_type.first == "hish2") {
					fish_piture.setFillColor(Color::Yellow);
				}
				else if (one_type.first == "trav") {
					fish_piture.setFillColor(Color::Green);
				}
				else if (one_type.first == "trav2") {
					fish_piture.setFillColor(Color::Blue);
				}
				// рисуем их
				for (const Location &one_fish : one_type.second) {
					fish_piture.setPosition(one_fish.x - fish_size / 2, y_m - (one_fish.y + fish_size / 2));
					window.draw(fish_piture);
				}
			}

			// отображение нового кадра
			window.display();

			// получаем результат выполения потока
			fishes = step_result.get();

			// задержка времени (чтобы рыбы плавали равномерно и чтобы не подвисало)
			std::this_thread::sleep_for(std::chrono::milliseconds(26));

		}

	}
	catch (const exception &_e) {
		cout << _e.what() << endl;
		cin.get();
	}

	return 0;

}
