/*
Aquarium.h by Alex Zhdanov. 2014-2015.
vk.com/qwert2603
Важно!!! Не храните объекты класса Aquarium в векторах или деках. Так как для нормальной работы необходимо,
чтобы указатели на объекты Аквариумов оставались допустимы.
Important!!! Do not store Aquarium objects in vector or deque, because it is needed pointers to Aquarium object to stay valid.
*/

#ifndef AQUARIUM
#define AQUARIUM

#include <iostream>
#include <ctime>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <random>

class Fish;
class FishType;
class Aquarium;

class FishTypeName {
	friend class FishType;
	friend class Aquarium;
public:
	explicit FishTypeName(const std::string &_n) : value(_n) {}
private:
	std::string value;
};

class X {
	friend class Location;
public:
	explicit X(double _x) : value(_x) { if (_x < 0) throw std::runtime_error("x < 0"); }
private:
	double value;
};

class Y {
	friend class Location;
public:
	explicit Y(double _y) : value(_y) { if (_y < 0) throw std::runtime_error("y < 0"); }
private:
	double value;
};

class A {
	friend class Location;
public:
	explicit A(double _a) : value(_a) {}
private:
	double value;
};

class DeathTime {
	friend class Fish;
public:
	explicit DeathTime(unsigned _dt) : value(_dt) {}
private:
	unsigned value;
};

class SpeedWalk {
	friend class FishType;
public:
	explicit SpeedWalk(double _sw) : value(_sw) { if (_sw < 0) throw std::runtime_error("SpeedWalk < 0"); }
private:
	double value;
};

class SpeedRun {
	friend class FishType;
public:
	explicit SpeedRun(double _sr) : value(_sr) { if (_sr < 0) throw std::runtime_error("SpeedRun < 0"); }
private:
	double value;
};

class Vision {
	friend class FishType;
public:
	explicit Vision(double _v) : value(_v) { if (_v < 0) throw std::runtime_error("Vision < 0"); }
private:
	double value;
};

class BirthFrequency {
	friend class FishType;
public:
	explicit BirthFrequency(unsigned _br) : value(_br) { if (_br < 0) throw std::runtime_error("BirthFrequency < 0"); }
private:
	unsigned value;
};

class Lifetime {
	friend class FishType;
public:
	explicit Lifetime(unsigned _lt) : value(_lt) { if (_lt < 0) throw std::runtime_error("Lifetime < 0"); }
private:
	unsigned value;
};

class Settled {
	friend class FishType;
public:
	explicit Settled(bool _st) : value(_st) {}
private:
	bool value;
};

class MaxCount {
	friend class FishType;
public:
	explicit MaxCount(unsigned _st) : value(_st) {}
private:
	unsigned value;
};

class XMax {
	friend class Aquarium;
public:
	explicit XMax(double _xm) : value(_xm) { if (_xm < 0) throw std::runtime_error("XMax < 0"); }
private:
	double value;
};

class YMax {
	friend class Aquarium;
public:
	explicit YMax(double _ym) : value(_ym) { if (_ym < 0) throw std::runtime_error("YMax < 0"); }
private:
	double value;
};

class FishSize {
	friend class Aquarium;
public:
	explicit FishSize(double _sz) : value(_sz) { if (_sz < 0) throw std::runtime_error("FishSize < 0"); }
private:
	double value;
};

class DiplomaticStatus {
	friend class Aquarium;
public:
	DiplomaticStatus();
	explicit DiplomaticStatus(int _ds) : value(_ds) {}
	bool operator == (const DiplomaticStatus &_f) const { return _f.value == value; }
	bool operator != (const DiplomaticStatus &_f) const { return !(_f.value == value); }
	bool operator < (const DiplomaticStatus & _ds) const { return value < _ds.value; }
	bool operator >(const DiplomaticStatus & _ds) const { return value > _ds.value; }
	DiplomaticStatus &operator ++ () { ++value; return *this; }
	DiplomaticStatus &operator -- () { --value; return *this; }
private:
	int value = 0;
};

struct Location {
	Location(X _x, Y _y, A _a) : x(_x.value), y(_y.value), a(_a.value) {}
	Location(X _x, Y _y) : Location(_x, _y, A(0)) {}
	double x, y, a;
};

double _distance(const Location _l1, const Location _l2) {
	return sqrt((_l2.x - _l1.x)*(_l2.x - _l1.x) + (_l2.y - _l1.y)*(_l2.y - _l1.y));
}

class Fish {
public:
	Fish(Location _l, DeathTime _d, FishType &_ft) 
		: x(_l.x), y(_l.y), a(_l.a), death_time(_d.value), fish_type(&_ft) {}
	// для уворачивания от стен. когда рыба догоняет, она может ближе подплывать к стенам
	// dodge walls. when fish chases, it can be closer to walls
	void check_wall(bool _chase);
	void check_borders(); // чтобы случайно не вышла за границы. check boundary violation
	void stay(); // колеблется на месте. stay near its place
	void walk(); // прогуливается и хаотично поворачивает. walk and randomly turns
	// гнаться за указанной рыбой(_chase == true) или убегать от нее(_chase == false)
	// chase fish(_chase == true) or run away from it(_chase == false)
	void run(Fish &_f, bool _chase);
	void eat(Fish &_f);
	// указатель на ближайшую рыбу с таким дипломатический статусом
	// pointer to nearst fish with such diplomatic status
	Fish *nearest(const DiplomaticStatus _ds, bool _is_chase) const;
	void step();
	bool is_alive() const;
	void clear_chase() { chased = false; }
	bool is_chased() const { return chased; }
	Location location() const { return Location(X(x), Y(y), A(a)); }
	double get_vision() const;
private:
	double x, y, a;
	unsigned death_time;
	// догоняют ли эту рыбу в текущий момент (чтобы много рыб не гонялось за одной)
	// is this fish chased at the moment
	bool chased = false;
	// указатель на объект класса FishType, которому принадлежит рыба
	// pointer to FishType object that this fish belongs to
	FishType *fish_type;
};

class FishType {
	friend class Fish;
public:
	FishType(FishTypeName _ftn, SpeedWalk _sw, SpeedRun _sr, Vision _v, BirthFrequency _bf,
		Lifetime _lt, MaxCount _mc, Settled _st, Aquarium &_aq)
		: name(_ftn), speed_walk(_sw.value), speed_run(_sr.value), vision(_v.value), birth_frequency(_bf.value),
		lifetime(_lt.value), max_count(_mc.value), settled(_st.value), aquarium(&_aq) {}
	void add_fish(const Location _l);
	void delete_dead_fishes();
	void clear_chases();
	std::pair<const std::string, std::vector<Location>> locations() const;
	// вернет указатель и расстояние до ближайшей к этой локации рыбы этого типа
	// pointer and distance to nearst fish of this type
	std::pair<Fish *, double> nearest(const Fish &_f, bool _is_chase);
	void step(); // каждая рыба этого типа делает ход. every fish make step
	std::string get_name() const { return name.value; }
	void born(); // рождение новых рыб. borning of new fishes
private:
	FishTypeName name;
	std::vector<Fish> fishes; // все рыбы этого типа. all fishes of this type
	// скорость в спокойном состоянии и во время погони
	// speed during walking and running
	double speed_walk, speed_run;
	double vision; // радиус зрения. radius of vision
	unsigned birth_frequency;
	unsigned lifetime;
	// если нет врага, то сидит на месте(true) или гуляет
	// to stay ot to walk if there are no emenies in the neighbourhood
	bool settled;
	unsigned max_count; // максимальное кол-во рыб этого типа. maximum number of fish of this type
	// указатель на объект класса Aquarium, которому принадлежит тип рыб
	// pointer to Aquarium object that this fish belongs to
	Aquarium *aquarium;
};

class Aquarium {
	friend class Fish;
	friend class FishType;
public:
	Aquarium(XMax _xm, YMax _ym, FishSize _sz)
		: x_max(_xm.value), y_max(_ym.value), fish_size(_sz.value), tempo(0) {}
	// все рыбы делают ход. all fishes make step
	void step();
	void add_type(FishTypeName _ftn, SpeedWalk _sw, SpeedRun _sr, Vision _v, 
		BirthFrequency _bf, Lifetime _lt, MaxCount _mc, Settled _st);
	void add_fish(FishTypeName _ftn, const Location _l);
	// получить координаты всех рыб. get locations of all fishes
	std::map<std::string, std::vector<Location>> locations() const;
	void set_diplomatic_status(const FishTypeName &_ftn1, const FishTypeName &_ftn2, const DiplomaticStatus _ds);
	unsigned current_time() const { return tempo; }
private:
	// все типы рыб в аквариуме. all fish types in aquarium.
	std::vector<FishType> types;
	// отношения между типами. чем больше модуль статуса, тем выше приоритет.
	// 0 - нейтральные. 1, 2, 3... - первая охотится на вторую. -1, -2, -3... - первая убегает от второй.
	// relations between types. more module of status, the higher the priority.
	// 0 - neutral. 1, 2, 3... - first type chases cecond one. -1, -2, -3... - first type runs away from second one.
	std::map<std::pair<const std::string, const std::string>, DiplomaticStatus> relations;
	double x_max, y_max;
	double fish_size;
	// можно было назвать просто time, но это может создать проблемы, если в основной программе тоже будет имя time,
	// к тому же есть функция time(), так что назовем ее по-итальянски
	// there is a variable and function called 'time', so it is better to name this variable in Itatian
	unsigned tempo = 0;
	std::default_random_engine rand_proc;	// для рандома. for random
};

double Fish::get_vision() const {
	return fish_type->vision;
}

bool Fish::is_alive() const {
	return death_time != fish_type->aquarium->tempo;
}

void FishType::add_fish(const Location _l) {
	fishes.emplace_back(_l, DeathTime(aquarium->tempo + lifetime), *this);
}

void Fish::check_borders() {
	const double &xm = fish_type->aquarium->x_max;
	const double &ym = fish_type->aquarium->y_max;
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (x > xm) x = xm;
	if (y > ym) y = ym;
}

void Fish::check_wall(bool _chase) {
	const double &vis = fish_type->vision / (_chase ? 8.2 : 1.1);
	const double &xm = fish_type->aquarium->x_max;
	const double &ym = fish_type->aquarium->y_max;
	// координаты единичного вектора с углом a
	// coordinates of the unit vector with the angle a
	double i = cos(a*3.1416 / 180.0);
	double j = sin(a*3.1416 / 180.0);
	if (x < vis)		i += ((vis - x) / vis);
	if (y < vis)		j += ((vis - y) / vis);
	if (x > xm - vis)	i -= ((vis + x - xm) / vis);
	if (y > ym - vis)	j -= ((vis + y - ym) / vis);
	a = atan(j / i)*180.0 / 3.1416;
	if (i < 0) a += 180.0;
}

void Fish::stay() {
	const double &xm = fish_type->aquarium->x_max;
	const double &ym = fish_type->aquarium->y_max;
	std::uniform_int_distribution<unsigned> unif_distr(0, 18);
	int d = unif_distr(fish_type->aquarium->rand_proc);
	switch (d) {
	case 1: x -= 0.9;	break;
	case 2: y -= 0.9;	break;
	case 3: x += 0.9;	break;
	case 4: y += 0.9;	break;
	case 5: x -= 1.8;	break;
	case 6: y -= 1.8;	break;
	case 7: x += 1.8;	break;
	case 8: y += 1.8;	break;
	}
	check_borders();
}

void Fish::walk() {
	check_wall(false);
	x += (fish_type->speed_walk*cos(a*3.1416 / 180.0));
	y += (fish_type->speed_walk*sin(a*3.1416 / 180.0));
	std::uniform_int_distribution<unsigned> unif_distr(0, 15);
	int d = unif_distr(fish_type->aquarium->rand_proc);
	switch (d) {
	case 1: a += 2.1;	break;
	case 2: a -= 2.1;	break;
	case 3: a += 4.3;	break;
	case 4: a -= 4.3;	break;
	case 5: a += 7.2;	break;
	case 6: a -= 7.2;	break;
	}
	check_borders();
}

void Fish::run(Fish &_f, bool _chase) {
	a = atan((_f.y - y) / (_f.x - x)) * 180.0 / 3.1416;
	if (!(_chase ^ (_f.x < x)))
		a += 180;
	check_wall(_chase);
	x += (fish_type->speed_run*cos(a*3.1416 / 180.0));
	y += (fish_type->speed_run*sin(a*3.1416 / 180.0));
	check_borders();
	if (_chase && _distance(this->location(), _f.location()) < fish_type->aquarium->fish_size / 2)
		eat(_f);
}

void Fish::eat(Fish &_f) {
	_f.death_time = fish_type->aquarium->tempo + 1;
	// еда продлевает жизнь. food makes life longer
	death_time += fish_type->lifetime / 8;
}

void FishType::clear_chases() {
	for (Fish &one_fish : fishes)
		one_fish.clear_chase();
}

std::pair<const std::string, std::vector<Location>> FishType::locations() const {
	std::vector<Location> locs;
	for (const Fish &one_fish : fishes)
		locs.push_back(one_fish.location());
	return std::make_pair(name.value, locs);
}

void FishType::delete_dead_fishes() {
	auto iter = partition(fishes.begin(), fishes.end(), [](const Fish &_f) { return _f.is_alive(); });
	fishes.erase(iter, fishes.end());
}

void Aquarium::add_type(FishTypeName _ftn, SpeedWalk _sw, SpeedRun _sr, Vision _v,
	BirthFrequency _bf, Lifetime _lt, MaxCount _mc, Settled _st) {
	types.emplace_back(_ftn, _sw, _sr, _v, _bf, _lt, _mc, _st, *this);
	// сделать тип взаимно нейтральным ко всем другим типам
	// to make this fish type neutral to all other ones
	for (const FishType &_ft : types) {
		set_diplomatic_status(FishTypeName(types.back().get_name()), FishTypeName(_ft.get_name()), DiplomaticStatus(0));
		set_diplomatic_status(FishTypeName(_ft.get_name()), FishTypeName(types.back().get_name()), DiplomaticStatus(0));
	}
}

void Aquarium::add_fish(FishTypeName _ftn, const Location _l) {
	auto type_iter = find_if(types.begin(), types.end(),
		[&](const FishType &_ft){ return _ft.get_name() == _ftn.value; });
	type_iter->add_fish(_l);
}

std::map<std::string, std::vector<Location>> Aquarium::locations() const {
	std::map<std::string, std::vector<Location>> locs;
	for (const FishType &one_type : types)
		locs.insert(one_type.locations());
	return locs;
}

void Aquarium::set_diplomatic_status(const FishTypeName &_ftn1, const FishTypeName &_ftn2, const DiplomaticStatus _ds) {
	if (relations.find(std::make_pair(_ftn1.value, _ftn2.value)) == relations.end()) {
		relations.insert(std::make_pair(std::make_pair(_ftn1.value, _ftn2.value), _ds));
	}
	else {
		relations.at(std::make_pair(_ftn1.value, _ftn2.value)) = _ds;
	}
}

std::pair<Fish *, double> FishType::nearest(const Fish &_f, bool _is_chase) {
	// текущая ближайшая рыба и расстояние до нее
	// current nearest fish and distance to it
	std::pair<Fish *, double> current_nearest = std::make_pair(nullptr, 1000000);
	for (Fish &one_fish : fishes) {
		double new_distance = _distance(_f.location(), one_fish.location());
		// если новая рыба попадает в поле зрения
		// если растояние до новой обрабатываемой рыбы меньше, чем расстояние до текущей ближайшей,
		// и если рыба, делающая ход, гонится за рыбой, которую еще никто не преследует
		// if the new fish enters the field of view,
		// if the distance to the new processed fish is less than the distance to the nearest current,
		// if the fish, make a move, chasing fish, which still has not been pursued
		if (new_distance < _f.get_vision() && new_distance < current_nearest.second && !(_is_chase && one_fish.is_chased())) {
			current_nearest.first = &one_fish;
			current_nearest.second = new_distance;
		}
	}
	return current_nearest;
}

Fish *Fish::nearest(const DiplomaticStatus _ds, bool _is_chase) const {
	// текущая ближайшая рыба и расстояние до нее
	// current nearest fish and distance to it
	std::pair<Fish *, double> current_nearest = std::make_pair(nullptr, 1000000.0);
	for (FishType &one_type : fish_type->aquarium->types) {
		if (fish_type->aquarium->relations.at(std::make_pair(fish_type->get_name(), one_type.get_name())) == _ds) {
			std::pair<Fish *, double> new_nearest = one_type.nearest(*this, _is_chase);
			if (new_nearest.second < current_nearest.second) {
				current_nearest = new_nearest;
			}
		}
	}
	return current_nearest.first;
}

void Fish::step() {
	bool done = false; // сделала ли рыба ход. is fish made step
	DiplomaticStatus current_diplomatic_status(0);
	// поиск врагов. looking for emenies
	// получение минимального дипломатического статуса
	// getting minimal diplomatic status
	for (const FishType &one_type : fish_type->aquarium->types) {
		DiplomaticStatus new_diplomatic_status(fish_type->aquarium->relations.at(std::make_pair(fish_type->get_name(), one_type.get_name())));
		if (new_diplomatic_status < current_diplomatic_status) {
			current_diplomatic_status = new_diplomatic_status;
		}
	}
	while (current_diplomatic_status != DiplomaticStatus(0)) {
		Fish *nearest_fish = nearest(current_diplomatic_status, false);
		if (nearest_fish) {
			run(*nearest_fish, false); // убегать. run away
			current_diplomatic_status = DiplomaticStatus(0);
			done = true;
			break;
		}
		++current_diplomatic_status;
	}
	// если рыба еще не сделала ход, то она ищет еду
	// if fish haven't made step yet, it looks for food
	if (!done) {
		// получение максимального дипломатического статуса
		// getting maximal diplomatic status
		for (const FishType &one_type : fish_type->aquarium->types) {
			DiplomaticStatus new_diplomatic_status(fish_type->aquarium->relations.at(std::make_pair(fish_type->get_name(), one_type.get_name())));
			if (new_diplomatic_status > current_diplomatic_status) {
				current_diplomatic_status = new_diplomatic_status;
			}
		}
		while (current_diplomatic_status != DiplomaticStatus(0)) {
			Fish *nearest_fish = nearest(current_diplomatic_status, true);
			if (nearest_fish) {
				run(*nearest_fish, true); // догонять. chase
				nearest_fish->chased = true;
				current_diplomatic_status = DiplomaticStatus(0);
				done = true;
				break;
			}
			--current_diplomatic_status;
		}
	}
	// если она не нашла ни врагов, ни еды, то она просто гуляет, или колеблется на месте
	// if fish haven't found emenies or food, then it walk or stay near on it's place
	if (!done) {
		if (fish_type->settled) {
			stay();
		}
		else {
			walk();
		}
	}
}

void FishType::step() {
	for (Fish &one_fish : fishes) {
		one_fish.step();
	}
}

void Aquarium::step() {
	for (FishType &one_type : types) {
		one_type.delete_dead_fishes();
	}
	for (FishType &one_type : types) {
		one_type.step();
	}
	for (FishType &one_type : types) {
		one_type.clear_chases();
	}
	for (FishType &one_type : types) {
		one_type.born();
	}
	++tempo;
}

void FishType::born() {
	auto count = fishes.size();
	std::uniform_int_distribution<unsigned> unif_distr(0, birth_frequency - 1);
	while (count) {
		--count;
		if (fishes.size() != max_count && unif_distr(aquarium->rand_proc) == 1)
			add_fish(fishes[count].location());
	}
}

#endif
