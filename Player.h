#pragma once

struct SEAT
{
	SHORT SeatID;
	SHORT Top;
	SHORT Left;
	SHORT Width;
	SHORT Height;
	bool IsOccupied;
};

class Player
{
public:
	Player();
	~Player();

	LONG PlayerID;
	LONG LeaderID;
	LONG GroupID;

	CComBSTR Name;
	CComBSTR FullName;
	CComBSTR Location;
	CComBSTR Email;
	CComBSTR GroupName;

	SEAT* Seat;

	bool IsLeader;
	bool IsMyTurn;
	bool IsPlaying;

private:

};

class ConnectFourPlayer : public Player
{
public:
	ConnectFourPlayer();
	~ConnectFourPlayer();

	GAMESTATS GameStats;
	PLAYERTYPE PlayerType;
private:
};