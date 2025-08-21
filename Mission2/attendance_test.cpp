
#include "gmock/gmock.h"
#include "attendance.h"

TEST(TS1, GRADE_FACTORY_TEST)
{
	GradeFactory::getInstance().addGradeFactory(GOLD_MEMBER_POINT, new GoldGradeFactory);
	GradeFactory::getInstance().addGradeFactory(SILVER_MEMBER_POINT, new SilverGradeFactory);
	GradeFactory::getInstance().addGradeFactory(NORMAL_MEMBER_POINT, new NormalGradeFactory);

	EXPECT_EQ(true, GradeFactory::getInstance().createGrade(63)->isPass());
	EXPECT_EQ(true, GradeFactory::getInstance().createGrade(33)->isPass());
	EXPECT_EQ(false, GradeFactory::getInstance().createGrade(25)->isPass());
}


TEST(TS1, BONUS_POINT_TEST)
{
	PointCalculator calc;

	Player player1;
	player1.name = string("James");
	player1.backNumber = 10;
	player1.attendance.attendanceDay[SATURDAY] = 10;

	EXPECT_EQ(10, calc.getBonusPoint(player1.attendance));

	Player player2;
	player2.name = string("John");
	player2.backNumber = 12;
	player2.attendance.attendanceDay[WEDNESDAY] = 8;

	EXPECT_EQ(0, calc.getBonusPoint(player2.attendance));

	player2.attendance.attendanceDay[WEDNESDAY] = 11;

	EXPECT_EQ(10, calc.getBonusPoint(player2.attendance));
}

TEST(TS1, POINT_TEST)
{
	PointCalculator calc;

	Player player1;
	player1.name = string("James");
	player1.backNumber = 10;
	player1.attendance.attendanceDay[MONDAY] = 10;
	player1.attendance.attendanceDay[SATURDAY] = 10;

	EXPECT_EQ(40, calc.getPoint(player1));

	Player player2;
	player2.name = string("John");
	player2.backNumber = 12;
	player2.attendance.attendanceDay[THURSDAY] = 10;
	player2.attendance.attendanceDay[WEDNESDAY] = 10;

	EXPECT_EQ(50, calc.getPoint(player2));
}

TEST(TS1, PLAYER_TEST)
{
	PlayerManager::getInstance().clear();
	PlayerManager::getInstance().getPlayer("John")->attendance.attendanceDay[MONDAY]++;

	EXPECT_EQ(PlayerManager::getInstance().getPlayer("John"), PlayerManager::getInstance().getBackNumberPlayer(1));
	EXPECT_EQ(1, PlayerManager::getInstance().getPlayerCount());
}

TEST(ts1, OUTPUT_MANAGER_TEST)
{
	Player player;
	player.name = "John";
	player.backNumber = 10;
	int point = 45;
	player.grade = GradeFactory::getInstance().createGrade(point);

	// std output을 text와 비교하는 방법으로 ASSERT 가능하나 시간 관계상 생략

	OutputManager outputManager;
	outputManager.printPlayerPointInfo(&player, point);

	DataProcessor dp;
	dp.processData();
}

TEST(TS1, PARSER_TEST)
{
	Parser parser;

	parser.parseAttendance("James", "monday");
	parser.parseAttendance("James", "tuesday");
	parser.parseAttendance("James", "tuesday");
	parser.parseAttendance("James", "wednesday");
	parser.parseAttendance("James", "thursday");
	parser.parseAttendance("James", "friday");
	parser.parseAttendance("James", "saturday");
	parser.parseAttendance("James", "sunday");

	EXPECT_EQ(1, PlayerManager::getInstance().getPlayer("James")->attendance.attendanceDay[MONDAY]);
	EXPECT_EQ(2, PlayerManager::getInstance().getPlayer("James")->attendance.attendanceDay[TUESDAY]);
	EXPECT_EQ(1, PlayerManager::getInstance().getPlayer("James")->attendance.attendanceDay[WEDNESDAY]);
	EXPECT_EQ(1, PlayerManager::getInstance().getPlayer("James")->attendance.attendanceDay[THURSDAY]);
	EXPECT_EQ(1, PlayerManager::getInstance().getPlayer("James")->attendance.attendanceDay[FRIDAY]);
	EXPECT_EQ(1, PlayerManager::getInstance().getPlayer("James")->attendance.attendanceDay[SATURDAY]);
	EXPECT_EQ(1, PlayerManager::getInstance().getPlayer("James")->attendance.attendanceDay[SUNDAY]);
}

TEST(TS1, GRADE_STRING_TEST)
{
	GradeFactory::getInstance().addGradeFactory(GOLD_MEMBER_POINT, new GoldGradeFactory);
	GradeFactory::getInstance().addGradeFactory(SILVER_MEMBER_POINT, new SilverGradeFactory);
	GradeFactory::getInstance().addGradeFactory(NORMAL_MEMBER_POINT, new NormalGradeFactory);

	EXPECT_THAT(string("GOLD"), GradeFactory::getInstance().createGrade(63)->getGradeString());
	EXPECT_THAT(string("SILVER"), GradeFactory::getInstance().createGrade(33)->getGradeString());
	EXPECT_THAT(string("NORMAL"), GradeFactory::getInstance().createGrade(25)->getGradeString());
}
