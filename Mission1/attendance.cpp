#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

map<string, int> mapNameID;
int idCount = 0;

const int WEEKDAYNUM = 7;

enum Grade {
	NORMAL = 0,
	GOLD = 1,
	SILVER = 2
};

const int MAX_PLAYERS = 100;

int attendanceData[MAX_PLAYERS][WEEKDAYNUM];
int points[MAX_PLAYERS];
int grade[MAX_PLAYERS];
string names[MAX_PLAYERS];
int wednesday[MAX_PLAYERS];
int weekend[MAX_PLAYERS];

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

void getWeekdayAndPoint(int idNum, std::string& weekday, WeekDay& weekdayIndex, int& attendancePoint)
{
	if (weekday == "monday") {
		weekdayIndex = MONDAY;
		attendancePoint++;
	}
	if (weekday == "tuesday") {
		weekdayIndex = TUESDAY;
		attendancePoint++;
	}
	if (weekday == "wednesday") {
		weekdayIndex = WEDNESDAY;
		attendancePoint += 3;
		wednesday[idNum] += 1;
	}
	if (weekday == "thursday") {
		weekdayIndex = THURSDAY;
		attendancePoint++;
	}
	if (weekday == "friday") {
		weekdayIndex = FRIDAY;
		attendancePoint++;
	}
	if (weekday == "saturday") {
		weekdayIndex = SATURDAY;
		attendancePoint += 2;
		weekend[idNum] += 1;
	}
	if (weekday == "sunday") {
		weekdayIndex = SUNDAY;
		attendancePoint += 2;
		weekend[idNum] += 1;
	}
}

void calcWeekdayPoints(string name, string weekday) {
	//ID 부여
	if (mapNameID.count(name) == 0) {
		mapNameID.insert({ name, ++idCount });
		names[idCount] = name;
	}
	int idNum = mapNameID[name];

	int attendancePoint = 0;
	WeekDay weekdayIndex = MONDAY;

	getWeekdayAndPoint(idNum, weekday, weekdayIndex, attendancePoint);

	attendanceData[idNum][weekdayIndex] += 1;
	points[idNum] += attendancePoint;
}

const int BONUS_ATTENDANCE_COUNT = 9;
const int BONUS_ATTENDANCE_POINT = 10;

void applyBonusPoints(int idNum)
{
	if (attendanceData[idNum][WEDNESDAY] > BONUS_ATTENDANCE_COUNT) {
		points[idNum] += BONUS_ATTENDANCE_POINT;
	}

	if (attendanceData[idNum][SATURDAY] + attendanceData[idNum][SUNDAY] > BONUS_ATTENDANCE_COUNT) {
		points[idNum] += BONUS_ATTENDANCE_POINT;
	}
}

const int GOLD_MEMBER_POINT = 50;
const int SILVER_MEMBER_POINT = 30;

void applyGrade(int idNum)
{
	if (points[idNum] >= GOLD_MEMBER_POINT) {
		grade[idNum] = GOLD;
	}
	else if (points[idNum] >= SILVER_MEMBER_POINT) {
		grade[idNum] = SILVER;
	}
	else {
		grade[idNum] = NORMAL;
	}
}

void printPlayerInfo(int idNum)
{
	cout << "NAME : " << names[idNum] << ", ";
	cout << "POINT : " << points[idNum] << ", ";
	cout << "GRADE : ";

	if (grade[idNum] == GOLD) {
		cout << "GOLD" << "\n";
	}
	else if (grade[idNum] == SILVER) {
		cout << "SILVER" << "\n";
	}
	else {
		cout << "NORMAL" << "\n";
	}
}

void printRemovePlayers()
{
	std::cout << "Removed player\n";
	std::cout << "==============\n";
	for (int i = 1; i <= idCount; i++) {
		if (grade[i] != GOLD && grade[i] != SILVER && wednesday[i] == 0 && weekend[i] == 0) {
			std::cout << names[i] << "\n";
		}
	}
}

void processData() {
	ifstream fin{ "attendance_weekday_500.txt" }; //500개 데이터 입력
	for (int i = 0; i < 500; i++) {
		string name, weekday;
		fin >> name >> weekday;
		calcWeekdayPoints(name, weekday);
	}

	for (int idNum = 1; idNum <= idCount; idNum++) {
		applyBonusPoints(idNum);
		applyGrade(idNum);
		printPlayerInfo(idNum);
	}

	std::cout << "\n";
	printRemovePlayers();
}

int main(int argc, char* argv[]) {
	processData();
	return 0;
}
