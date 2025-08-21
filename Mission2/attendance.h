#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cassert>

using namespace std;

const int GOLD_MEMBER_POINT = 50;
const int SILVER_MEMBER_POINT = 30;
const int NORMAL_MEMBER_POINT = 0;

#define interface struct


enum WeekDay
{
	MONDAY = 0,
	TUESDAY,
	WEDNESDAY,
	THURSDAY,
	FRIDAY,
	SATURDAY,
	SUNDAY
};

const int WEEKDAYNUM = 7;

interface Grade{
public:
	virtual bool isPass() = 0;
	virtual string getGradeString() = 0;
};

interface GradeFactoryInterface{
public:
	virtual Grade * getGrade() = 0;
};

class GoldGrade : public Grade {
public:
	bool isPass() override { return true; }
	string getGradeString() override { return string("GOLD"); }
};

class GoldGradeFactory : public GradeFactoryInterface {
public:
	Grade* getGrade() override {
		return new GoldGrade();
	}
};

class SilverGrade : public Grade {
public:
	bool isPass() override { return true; }
	string getGradeString() override { return string("SILVER"); }
};

class SilverGradeFactory : public GradeFactoryInterface {
public:
	Grade* getGrade() override {
		return new SilverGrade();
	}
};

class NormalGrade : public Grade {
public:
	bool isPass() override { return false; }
	string getGradeString() override { return string("NORMAL"); }
};

class NormalGradeFactory : public GradeFactoryInterface {
public:
	Grade* getGrade() override {
		return new NormalGrade();
	}
};

class GradeFactory {
public:
	static GradeFactory& getInstance() {
		static GradeFactory instance;
		return instance;
	}

	Grade* createGrade(int point) {
		GradeFactoryInterface* factory = nullptr;
		int prevFactoryPoint = 0;
		for (auto it = mapPointGradeFactory.begin(); it != mapPointGradeFactory.end(); ++it) {
			if (point >= it->first) {
				if (point > prevFactoryPoint)
				{
					factory = it->second;
					prevFactoryPoint = it->first;
				}
			}
		}

		assert(factory != nullptr);

		return factory->getGrade();
	}

	void addGradeFactory(int point, GradeFactoryInterface* factory) {
		mapPointGradeFactory.insert({ point, factory });
	}

private:
	GradeFactory() {}
	GradeFactory& operator=(const GradeFactory& other) = delete;
	GradeFactory(const GradeFactory& other) = delete;

	map<int, GradeFactoryInterface*> mapPointGradeFactory;
};




class Attendance {
public:
	Attendance() : wednesdayAttendance(0), weekendAttendance(0)
	{
		for (int i = 0; i < WEEKDAYNUM; ++i) attendanceDay[i] = 0;
	}
	int attendanceDay[WEEKDAYNUM];
	int wednesdayAttendance;
	int weekendAttendance;
};

class Player {
public:
	Player() : name("no name"), backNumber(0), grade(nullptr) {}
	string name;
	int backNumber;

	Grade* grade;
	Attendance attendance;
};

class PlayerManager {
public:
	static PlayerManager& getInstance() {
		static PlayerManager instance;
		return instance;
	}

	Player* getPlayer(string name) {
		if (mapPlayer.count(name) == 0) {
			mapPlayer.insert({ name, new Player() });
			mapPlayer[name]->name = name;
			mapPlayer[name]->backNumber = ++idCount;
			mapBackNumberPlayer.insert({ idCount, mapPlayer[name] });
		}
		return mapPlayer[name];
	}

	Player* getBackNumberPlayer(int backNumber) {
		return mapBackNumberPlayer[backNumber];
	}

	int getPlayerCount() { return idCount; }

	void clear() { mapPlayer.clear(); mapBackNumberPlayer.clear(); }

private:
	int idCount;
	PlayerManager() : idCount(0) {}
	PlayerManager& operator=(const PlayerManager& other) = delete;
	PlayerManager(const PlayerManager& other) = delete;

	map<string, Player*> mapPlayer;
	map<int, Player*> mapBackNumberPlayer;
};

class PointCalculator {
public:
	int getPoint(Player& player) {
		int point = 0;

		for (int day = MONDAY; day <= SUNDAY; ++day)
		{
			if (day == WEDNESDAY) {
				point += player.attendance.attendanceDay[day] * 3;
			}
			else if ((day == SATURDAY) || (day == SUNDAY)) {
				point += player.attendance.attendanceDay[day] * 2;
			}
			else {
				point += player.attendance.attendanceDay[day];
			}
		}

		point += getBonusPoint(player.attendance);
		return point;
	}

	int getBonusPoint(const Attendance& attedance) {
		int bonusPoint = 0;
		if (attedance.attendanceDay[WEDNESDAY] > BONUS_ATTENDANCE_COUNT) {
			bonusPoint += BONUS_ATTENDANCE_POINT;
		}

		if (attedance.attendanceDay[SATURDAY] + attedance.attendanceDay[SUNDAY] > BONUS_ATTENDANCE_COUNT) {
			bonusPoint += BONUS_ATTENDANCE_POINT;
		}
		return bonusPoint;
	}

private:
	const int BONUS_ATTENDANCE_COUNT = 9;
	const int BONUS_ATTENDANCE_POINT = 10;
};

class Parser {
public:
	void parseAttendance(string name, string weekday) {
		if (weekday == "monday") {
			PlayerManager::getInstance().getPlayer(name)->attendance.attendanceDay[MONDAY]++;
		}
		if (weekday == "tuesday") {
			PlayerManager::getInstance().getPlayer(name)->attendance.attendanceDay[TUESDAY]++;
		}
		if (weekday == "wednesday") {
			PlayerManager::getInstance().getPlayer(name)->attendance.attendanceDay[WEDNESDAY]++;
			PlayerManager::getInstance().getPlayer(name)->attendance.wednesdayAttendance++;
		}
		if (weekday == "thursday") {
			PlayerManager::getInstance().getPlayer(name)->attendance.attendanceDay[THURSDAY]++;
		}
		if (weekday == "friday") {
			PlayerManager::getInstance().getPlayer(name)->attendance.attendanceDay[FRIDAY]++;
		}
		if (weekday == "saturday") {
			PlayerManager::getInstance().getPlayer(name)->attendance.attendanceDay[SATURDAY]++;
			PlayerManager::getInstance().getPlayer(name)->attendance.weekendAttendance++;
		}
		if (weekday == "sunday") {
			PlayerManager::getInstance().getPlayer(name)->attendance.attendanceDay[SUNDAY]++;
			PlayerManager::getInstance().getPlayer(name)->attendance.weekendAttendance++;
		}
	}
};

class OutputManager {
public:
	void printPlayerPointInfo(Player* player, int point) {
		cout << "NAME : " << player->name << ", ";
		cout << "POINT : " << point << ", ";
		cout << "GRADE : " << player->grade->getGradeString() << "\n";
	}

	void printRemovePlayerHeader() {
		std::cout << "\n";

		std::cout << "Removed player\n";
		std::cout << "==============\n";
	}

	void printPlayerNameOnly(Player* player) {
		std::cout << player->name << "\n";
	}
};

class DataProcessor {
public:
	void processData() {
		ifstream fin{ "attendance_weekday_500.txt" }; //500개 데이터 입력

		Parser parser;

		for (int i = 0; i < 500; i++) {
			string name, weekday;
			fin >> name >> weekday;

			parser.parseAttendance(name, weekday);
		}

		GradeFactory::getInstance().addGradeFactory(GOLD_MEMBER_POINT, new GoldGradeFactory);
		GradeFactory::getInstance().addGradeFactory(SILVER_MEMBER_POINT, new SilverGradeFactory);
		GradeFactory::getInstance().addGradeFactory(NORMAL_MEMBER_POINT, new NormalGradeFactory);

		PointCalculator calc;
		OutputManager outputManager;

		for (int idNum = 1; idNum <= PlayerManager::getInstance().getPlayerCount(); idNum++) {
			PointCalculator calc;
			Player* player = PlayerManager::getInstance().getBackNumberPlayer(idNum);
			int point = calc.getPoint(*player);
			player->grade = GradeFactory::getInstance().createGrade(point);

			outputManager.printPlayerPointInfo(player, point);
		}

		outputManager.printRemovePlayerHeader();

		for (int idNum = 1; idNum <= PlayerManager::getInstance().getPlayerCount(); idNum++) {
			Player* player = PlayerManager::getInstance().getBackNumberPlayer(idNum);
			if ((player->grade->isPass() == false) && (player->attendance.wednesdayAttendance == 0) && (player->attendance.weekendAttendance == 0)) {
				outputManager.printPlayerNameOnly(player);
			}
		}
	}
};
