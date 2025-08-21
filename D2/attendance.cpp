#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

struct Node {
	string w;
	string wk;
};

map<string, int> mapNameID;
int idCount = 0;

const int WEEKDAYNUM = 7;

enum Grade {
	NORMAL = 0,
	GOLD = 1,
	SILVER = 2
};

struct PlayerAttendance {
	string name;
	int attendance[WEEKDAYNUM];
	int points;
	Grade grade;
	int wednesdayAttendance;
	int weekendAttendance;
};

const int MAX_PLAYERS = 100;

PlayerAttendance playerAttendance[MAX_PLAYERS];

//attendanceData[사용자ID][요일]
int attendanceData[100][100];
int points[100];
int grade[100];
string names[100];
int wednesday[100];
int weekend[100];

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

void getWeekdayAndPoint(std::string& weekday, WeekDay& weekdayIndex, int& add_point, int idNum);

void printRemovePlayers();

void printPlayerInfo(int idNum);

void applyGrade(int idNum);

void applyBonusPoints(int idNum);

void calcWeekdayPoints(string name, string weekday) {
	//ID 부여
	if (mapNameID.count(name) == 0) {
		mapNameID.insert({ name, ++idCount });
		names[idCount] = name;
	}
	int idNum = mapNameID[name];

	int add_point = 0;
	WeekDay weekdayIndex = MONDAY;

	getWeekdayAndPoint(weekday, weekdayIndex, add_point, idNum);

	//사용자ID별 요일 데이터에 1씩 증가
	attendanceData[idNum][weekdayIndex] += 1;
	points[idNum] += add_point;
}

void getWeekdayAndPoint(std::string& weekday, WeekDay& weekdayIndex, int& add_point, int idNum)
{
	if (weekday == "monday") {
		weekdayIndex = MONDAY;
		add_point++;
	}
	if (weekday == "tuesday") {
		weekdayIndex = TUESDAY;
		add_point++;
	}
	if (weekday == "wednesday") {
		weekdayIndex = WEDNESDAY;
		add_point += 3;
		wednesday[idNum] += 1;
	}
	if (weekday == "thursday") {
		weekdayIndex = THURSDAY;
		add_point++;
	}
	if (weekday == "friday") {
		weekdayIndex = FRIDAY;
		add_point++;
	}
	if (weekday == "saturday") {
		weekdayIndex = SATURDAY;
		add_point += 2;
		weekend[idNum] += 1;
	}
	if (weekday == "sunday") {
		weekdayIndex = SUNDAY;
		add_point += 2;
		weekend[idNum] += 1;
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




int main() {
	processData();
}
