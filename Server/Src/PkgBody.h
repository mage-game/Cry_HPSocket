#pragma once
#pragma warning (disable:4200)
#include <stdint.h>

enum MsgType
{
	LOCAL_PLAYER_LOGIN = 1,
	LOCAL_PLAYER_LOGOUT,
	SELECT_ACTOR_INFO,
	NET_PLAYER_LOGIN = SELECT_ACTOR_INFO,
	NET_PLAYER_LOGOUT,
	ALL_NET_PLAYER_INFO,
	PLAYER_BIKE_TRANSFROM,
	PLAYER_BIKE_ANIMATION,
	PLAYER_TRANSFROM,
	PLAYER_ANIMATION
};

#pragma pack(push,2)

struct PkgBodyLocalPlayerLogin
{

};

struct ActorInfo
{
	uint64_t actorID = 0;
	uint8_t name[20] = { 0 };

	ActorInfo& operator = (ActorInfo& pkg)
	{
		actorID = pkg.actorID;
		memcpy(name, pkg.name, 20);
		return *this;
	}
};

struct PkgBodySelectActorInfo
{
	ActorInfo actor;

	PkgBodySelectActorInfo& operator = (PkgBodySelectActorInfo& pkg)
	{
		actor = pkg.actor;
		return *this;
	}
};

struct NetPlayerInfo
{
	uint64_t connID = 0;
	ActorInfo actor;

	NetPlayerInfo& operator = (NetPlayerInfo& pkg)
	{
		connID = pkg.connID;
		actor = pkg.actor;
		return *this;
	}
};
struct PkgBodyAllNetPlayerInfo
{
	NetPlayerInfo players[0];
};

struct PkgBodyNetPlayerLogin
{
	NetPlayerInfo player;

	PkgBodyNetPlayerLogin& operator = (PkgBodyNetPlayerLogin& pkg)
	{
		player = pkg.player;
		return *this;
	}
};

struct PkgBodyTransfrom
{
	float posX = 0.0f;
	float posY = 0.0f;
	float posZ = 0.0f;
	float rotX = 0.0f;
	float rotY = 0.0f;
	float rotZ = 0.0f;

	PkgBodyTransfrom& operator =(PkgBodyTransfrom& pkg)
	{
		posX = pkg.posX;
		posY = pkg.posY;
		posZ = pkg.posZ;
		rotX = pkg.rotX;
		rotY = pkg.rotY;
		rotZ = pkg.rotZ;

		return *this;
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

	PkgBodyBikeAnimation& operator = (PkgBodyBikeAnimation& pkg)
	{
		frontWheel = pkg.frontWheel;
		rearWheel = pkg.rearWheel;
		steer = pkg.steer;
		pedal = pkg.pedal;
		bodyRoll = pkg.bodyRoll;

		return *this;
	}
};

struct PkgBodyPlayerAnimation
{
	uint64_t animationIDs = 0;

	PkgBodyPlayerAnimation& operator = (PkgBodyPlayerAnimation& pkg)
	{
		animationIDs = pkg.animationIDs;

		return *this;
	}
};

#pragma pack(pop)

