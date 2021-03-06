#include <windows.h>
#include <vector>
#include <map>
#include <string>
#include <SFML\Graphics.hpp>
#include "aquarium.h"

using std::map;
using std::vector;
using std::string;
using std::pair;

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

// для управления рыбой
using aquarium::FishNumber;				// класс номера рыбы

// для преобразований между градусами и радианами
using aquarium::degree_to_radian;
using aquarium::radian_to_degree;

// размеры аквариума
const double x_m = 1300, y_m = 650;
// размер рыбы
const double fish_size = 10;


namespace n1 {
	std::vector<int> v;
}


int main() {
	// сам аквариум
	Aquarium a1{ XMax(x_m), YMax(y_m), FishSize(fish_size) };

	// добавляем типы рыб
	a1.add_type(FishTypeName("main_hish"), SpeedWalk(7), SpeedRun(8.5), Vision(140), BirthFrequency(600), Lifetime(1200), MaxCount(15), Settled(false));
	a1.add_type(FishTypeName("hish"), SpeedWalk(6), SpeedRun(8), Vision(110), BirthFrequency(270), Lifetime(800), MaxCount(20), Settled(false));
	a1.add_type(FishTypeName("hish2"), SpeedWalk(6), SpeedRun(7.8), Vision(100), BirthFrequency(240), Lifetime(700), MaxCount(30), Settled(false));
	a1.add_type(FishTypeName("trav"), SpeedWalk(5), SpeedRun(7.5), Vision(85), BirthFrequency(150), Lifetime(600), MaxCount(40), Settled(true));
	a1.add_type(FishTypeName("trav2"), SpeedWalk(5), SpeedRun(7), Vision(70), BirthFrequency(120), Lifetime(500), MaxCount(60), Settled(false));
	a1.add_type(FishTypeName("player"), SpeedWalk(5), SpeedRun(9), Vision(70), BirthFrequency(UINT_MAX), Lifetime(UINT_MAX), MaxCount(1), Settled(false));
	
	// добавляем самих рыб
	a1.add_fish(FishTypeName("main_hish"), Location(X(1000), Y(400), A(180)));
	a1.add_fish(FishTypeName("player"), Location(X(500), Y(530), A(0)));
	for (unsigned u = 0; u != 2; ++u) {
		a1.add_fish(FishTypeName("hish"), Location(X(300), Y(400), A(90)));
		a1.add_fish(FishTypeName("hish"), Location(X(300), Y(400), A(111)));
		a1.add_fish(FishTypeName("hish2"), Location(X(700), Y(400), A(180)));
		a1.add_fish(FishTypeName("hish2"), Location(X(700), Y(400), A(160)));
		a1.add_fish(FishTypeName("trav"), Location(X(900), Y(70), A(0)));
		a1.add_fish(FishTypeName("trav"), Location(X(900), Y(60), A(0)));
		a1.add_fish(FishTypeName("trav2"), Location(X(300), Y(220), A(0)));
		a1.add_fish(FishTypeName("trav2"), Location(X(300), Y(230), A(0)));
	}

	// устанавливаем отношения между типами рыб
	// за игроком охотится FishTypeName("hish")
	a1.set_diplomatic_status(FishTypeName("main_hish"), FishTypeName("hish"), DiplomaticStatus(2));
	a1.set_diplomatic_status(FishTypeName("main_hish"), FishTypeName("hish2"), DiplomaticStatus(1));
	a1.set_diplomatic_status(FishTypeName("hish"), FishTypeName("trav"), DiplomaticStatus(1));
	a1.set_diplomatic_status(FishTypeName("hish"), FishTypeName("player"), DiplomaticStatus(1));
	a1.set_diplomatic_status(FishTypeName("hish2"), FishTypeName("trav2"), DiplomaticStatus(1));
	a1.set_diplomatic_status(FishTypeName("hish"), FishTypeName("main_hish"), DiplomaticStatus(-1));
	a1.set_diplomatic_status(FishTypeName("hish2"), FishTypeName("main_hish"), DiplomaticStatus(-1));
	a1.set_diplomatic_status(FishTypeName("trav"), FishTypeName("hish"), DiplomaticStatus(-1));
	a1.set_diplomatic_status(FishTypeName("player"), FishTypeName("hish"), DiplomaticStatus(-1));
	a1.set_diplomatic_status(FishTypeName("trav2"), FishTypeName("hish2"), DiplomaticStatus(-1));
	


	//a1.add_type(FishTypeName("main_hish"), SpeedWalk(7), SpeedRun(8.5), Vision(240), BirthFrequency(600), Lifetime(1200), MaxCount(2), Settled(false));
	//a1.add_type(FishTypeName("hish"), SpeedWalk(6), SpeedRun(8), Vision(210), BirthFrequency(320), Lifetime(800), MaxCount(5), Settled(false));
	//a1.add_type(FishTypeName("trav"), SpeedWalk(5), SpeedRun(7.5), Vision(185), BirthFrequency(120), Lifetime(600), MaxCount(9), Settled(true));
	//a1.add_fish(FishTypeName("main_hish"), Location(X(400), Y(300), A(0)));
	//a1.add_fish(FishTypeName("hish"), Location(X(500), Y(300), A(0)));
	//a1.add_fish(FishTypeName("trav"), Location(X(600), Y(300), A(0)));
	//a1.set_diplomatic_status(FishTypeName("main_hish"), FishTypeName("hish"), DiplomaticStatus(2));
	//a1.set_diplomatic_status(FishTypeName("hish"), FishTypeName("trav"), DiplomaticStatus(1));
	//a1.set_diplomatic_status(FishTypeName("hish"), FishTypeName("main_hish"), DiplomaticStatus(-1));
	//a1.set_diplomatic_status(FishTypeName("trav"), FishTypeName("hish"), DiplomaticStatus(-1));


	// само окно 
	sf::RenderWindow window(sf::VideoMode(x_m, y_m), "_aquarium");

	// текущее положение управляемой рыбы
	Location fish_location = a1.get_fish_locations()["player"][0];

	while (window.isOpen()) {
		// обработка закрытия окна
		sf::Event Event;
		while (window.pollEvent(Event)) {
			if (Event.type == sf::Event::Closed)
				window.close();
		}

		// рыба игрок плавает за мышкой, если нажата левая кнопка мышки
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			// текущее положение мышки
			sf::Vector2i mouse_position = sf::Mouse::getPosition();
			// подкорректируем, учитывая положение окна
			mouse_position -= window.getPosition();
			mouse_position.y -= 32;
			fish_location.y = y_m - fish_location.y;
			// угол в градусах, куда плыть рыбе (направление к стелочке мышки)
			double direction = atan((mouse_position.y - fish_location.y) / (mouse_position.x - fish_location.x));
			direction = radian_to_degree(direction);
			if (mouse_position.x > fish_location.x) {
				direction += 180.0;
			}
			direction = 180.0 - direction;
			a1.fish_control(FishTypeName("player"), FishNumber(0), A(direction));
		}

		// остальные рыбы делают шаг
		a1.step();

		// получаем местоположения рыб,
		// а заодно и имена их типов, чтобы знать что рисовать
		map<string, vector<Location>> fishes = a1.get_fish_locations();
		// запоминаем текущее положение управляемой рыбы, если она жива
		if (!fishes["player"].empty()) {
			fish_location = fishes["player"][0];
		}

		// очистка старого изображения
		window.clear(sf::Color::White);


		// картинка рыбы (квадратик)
		sf::RectangleShape fish_piture(sf::Vector2f(fish_size, fish_size));
		// отображение рыб
		for (const pair<string, vector<Location>> &one_type : fishes) {
			// определяем с каким типом рыб мы имеем дело
			// и выбираем соответствующий цвет
			if (one_type.first == "main_hish") {
				fish_piture.setFillColor(sf::Color::Red);
			}
			else if (one_type.first == "hish") {
				fish_piture.setFillColor(sf::Color(255, 128, 0, 255));
			}
			else if (one_type.first == "hish2") {
				fish_piture.setFillColor(sf::Color::Yellow);
			}
			else if (one_type.first == "trav") {
				fish_piture.setFillColor(sf::Color::Green);
			}
			else if (one_type.first == "trav2") {
				fish_piture.setFillColor(sf::Color::Blue);
			}
			else if (one_type.first == "player") {
				fish_piture.setFillColor(sf::Color::Black);
			}
			// рисуем их
			for (const Location &one_fish : one_type.second) {
				fish_piture.setPosition(one_fish.x - fish_size / 2, y_m - (one_fish.y + fish_size / 2));
				window.draw(fish_piture);
			}
		}

		// отображение нового кадра
		window.display();

		// задержка времени
		Sleep(26);
	}

	return 0;
}
