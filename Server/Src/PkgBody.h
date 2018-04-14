#pragma once
#pragma warning (disable:4200)
#include <stdint.h>

enum MsgType
{
	NONE = 0,
	PLAYER_LOGIN,
	PLAYER_LOGOUT,
	SELECT_ACTOR_INFO,
	ALL_PLAYER_INFO,  	
	PLAYER_BIKE_TRANSFROM,
	PLAYER_BIKE_ANIMATION,
	PLAYER_TRANSFROM,
	PLAYER_ANIMATION,
	OTHERS,
};

#pragma pack(push,2)

struct PkgBodyPlayers
{
    struct Player
    {
        uint64_t connID;
        //uint8_t name[50];
    };
    //uint32_t playerCount = 0;
    Player players[0];
};

struct PkgBodyTransfrom
{
	float posX = 0.0f;
	float posY = 0.0f;
	float posZ = 0.0f;
	float rotX = 0.0f;
	float rotY = 0.0f;
	float rotZ = 0.0f;

	void operator =(PkgBodyTransfrom& pkg)
	{
		posX = pkg.posX;
		posY = pkg.posY;
		posZ = pkg.posZ;
		rotX = pkg.rotX;
		rotY = pkg.rotY;
		rotZ = pkg.rotZ;
	}
};

typedef PkgBodyTransfrom PkgBodyPlayerTransfrom;
typedef PkgBodyTransfrom PkgBodyBikeTransfrom;

struct PkgBodyBikeAnimation
{
	float frontWheel = 0.0f;
	float rearWheel = 0.0f;
	float steer = 0.5f;
	float pedal = 0.0f;
	float bodyRoll = 0.5f;

	void operator = (PkgBodyBikeAnimation& pkg)
	{
		frontWheel = pkg.frontWheel;
		rearWheel = pkg.rearWheel;
		steer = pkg.steer;
		pedal = pkg.pedal;
		bodyRoll = pkg.bodyRoll;
	}
};

#pragma pack(pop)
 
