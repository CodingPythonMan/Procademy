// 패킷 데이터 정의
#pragma once
enum class Direction : unsigned char {
	LL = 0,
	RR
};

enum class MoveType : unsigned char {
	MOVE_DIR_LL = 0,
	MOVE_DIR_LU,
	MOVE_DIR_UU,
	MOVE_DIR_RU,
	MOVE_DIR_RR,
	MOVE_DIR_RD,
	MOVE_DIR_DD,
	MOVE_DIR_LD
};

enum class PacketType : unsigned char {
	FIGHTER_CMD_CREATE_MY_CHARACTER = 0,
	FIGHTER_CMD_CREATE_OTHER_CHARACTER,
	FIGHTER_CMD_DELETE_CHARACTER,

	FIGHTER_QRY_MOVE_START = 10,
	FIGHTER_REP_MOVE_START,
	FIGHTER_QRY_MOVE_STOP,
	FIGHTER_REP_MOVE_STOP,

	FIGHTER_QRY_ATTACK_001 = 20,
	FIGHTER_REP_ATTACK_001,
	FIGHTER_QRY_ATTACK_002,
	FIGHTER_REP_ATTACK_002,
	FIGHTER_QRY_ATTACK_003,
	FIGHTER_REP_ATTACK_003,

	FIGHTER_CMD_DAMAGE = 30,

	FIGHTER_QRY_SYNC = 250,
	FIGHTER_REP_SYNC,

};

#pragma pack(push, 1)
struct PACKET_HEADER {
	unsigned char ByCode;	// 패킷코드 0x89 고정.
	unsigned char BySize;	// 패킷 사이즈.
	unsigned char ByType;	// 패킷타입.
};

//---------------------------------------------------------------
// 클라이언트 자신의 캐릭터 할당		Server -> Client
//
// 서버에 접속시 최초로 받게되는 패킷으로 자신이 할당받은 ID 와
// 자신의 최초 위치, HP 를 받게 된다. (처음에 한번 받게 됨)
// 
// 이 패킷을 받으면 자신의 ID,X,Y,HP 를 저장하고 캐릭터를 생성시켜야 한다.
//
//	4	-	ID
//	1	-	Direction	(LL / RR)
//	2	-	X
//	2	-	Y
//	1	-	HP
//
//---------------------------------------------------------------
struct FIGHTER_CMD_CREATE_MY_CHARACTER : PACKET_HEADER {
	int ID;
	Direction Direct;
	short X;
	short Y;
	unsigned char HP;
};

//---------------------------------------------------------------
// 다른 클라이언트의 캐릭터 생성 패킷		Server -> Client
//
// 처음 서버에 접속시 이미 접속되어 있던 캐릭터들의 정보
// 또는 게임중에 접속된 클라이언트들의 생성용 정보.
//
//
//	4	-	ID
//	1	-	Direction	(LL / RR)
//	2	-	X
//	2	-	Y
//	1	-	HP
//
//---------------------------------------------------------------
struct FIGHTER_CMD_CREATE_OTHER_CHARACTER : PACKET_HEADER {
	int ID;
	Direction Direct;
	short X;
	short Y;
	unsigned char HP;
};

//---------------------------------------------------------------
// 캐릭터 삭제 패킷						Server -> Client
//
// 캐릭터의 접속해제 또는 캐릭터가 죽었을때 전송됨.
//
//	4	-	ID
//
//---------------------------------------------------------------
struct FIGHTER_CMD_DELETE_CHARACTER : PACKET_HEADER {
	int ID;
};

//---------------------------------------------------------------
// 캐릭터 이동시작 패킷						Client -> Server
//
// 자신의 캐릭터 이동시작시 이 패킷을 보낸다.
// 이동 중에는 본 패킷을 보내지 않으며, 키 입력이 변경되었을 경우에만
// 보내줘야 한다.
//
// (왼쪽 이동중 위로 이동 / 왼쪽 이동중 왼쪽 위로 이동... 등등)
//
//	1	-	Direction	( 방향 디파인 값 8방향 사용 )
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------
struct FIGHTER_QRY_MOVE_START : PACKET_HEADER {
	MoveType Move;
	short X;
	short Y;
};

//---------------------------------------------------------------
// 캐릭터 이동시작 패킷						Server -> Client
//
// 다른 유저의 캐릭터 이동시 본 패킷을 받는다.
// 패킷 수신시 해당 캐릭터를 찾아 이동처리를 해주도록 한다.
// 
// 패킷 수신 시 해당 키가 계속해서 눌린것으로 생각하고
// 해당 방향으로 계속 이동을 하고 있어야만 한다.
//
//	4	-	ID
//	1	-	Direction	( 방향 디파인 값 8방향 )
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------
struct FIGHTER_REP_MOVE_START : PACKET_HEADER {
	int ID;
	MoveType Move;
	short X;
	short Y;
};

//---------------------------------------------------------------
// 캐릭터 이동중지 패킷						Client -> Server
//
// 이동중 키보드 입력이 없어서 정지되었을 때, 이 패킷을 서버에 보내준다.
// 이동중 방향 전환시에는 스탑을 보내지 않는다.
//
//	1	-	Direction	( 방향 디파인 값 좌/우만 사용 )
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------
struct FIGHTER_QRY_MOVE_STOP : PACKET_HEADER {
	Direction Direct;
	short X;
	short Y;
};

//---------------------------------------------------------------
// 캐릭터 이동중지 패킷						Server -> Client
//
// ID 에 해당하는 캐릭터가 이동을 멈춘것이므로 
// 캐릭터를 찾아서 방향과, 좌표를 입력해주고 멈추도록 처리한다.
//
//	4	-	ID
//	1	-	Direction	( 방향 디파인 값. 좌/우만 사용 )
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------
struct FIGHTER_REP_MOVE_STOP : PACKET_HEADER {
	int ID;
	Direction Direct;
	short X;
	short Y;
};

//---------------------------------------------------------------
// 캐릭터 공격 패킷							Client -> Server
//
// 공격 키 입력시 본 패킷을 서버에게 보낸다.
// 충돌 및 데미지에 대한 결과는 서버에서 알려 줄 것이다.
//
// 공격 동작 시작시 한번만 서버에게 보내줘야 한다.
//
//	1	-	Direction	( 방향 디파인 값. 좌/우만 사용 )
//	2	-	X
//	2	-	Y	
//
//---------------------------------------------------------------
struct FIGHTER_QRY_ATTACK_001 : PACKET_HEADER {
	Direction Direct;
	short X;
	short Y;
};

//---------------------------------------------------------------
// 캐릭터 공격 패킷							Server -> Client
//
// 패킷 수신시 해당 캐릭터를 찾아서 공격1번 동작으로 액션을 취해준다.
// 방향이 다를 경우에는 해당 방향으로 바꾼 후 해준다.
//
//	4	-	ID
//	1	-	Direction	( 방향 디파인 값. 좌/우만 사용 )
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------
struct FIGHTER_REP_ATTACK_001 : PACKET_HEADER {
	int ID;
	Direction Direct;
	short X;
	short Y;
};

//---------------------------------------------------------------
// 캐릭터 공격 패킷							Client -> Server
//
// 공격 키 입력시 본 패킷을 서버에게 보낸다.
// 충돌 및 데미지에 대한 결과는 서버에서 알려 줄 것이다.
//
// 공격 동작 시작시 한번만 서버에게 보내줘야 한다.
//
//	1	-	Direction	( 방향 디파인 값. 좌/우만 사용 )
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------
struct FIGHTER_QRY_ATTACK_002 : PACKET_HEADER {
	Direction Direct;
	short X;
	short Y;
};

//---------------------------------------------------------------
// 캐릭터 공격 패킷							Server -> Client
//
// 패킷 수신시 해당 캐릭터를 찾아서 공격2번 동작으로 액션을 취해준다.
// 방향이 다를 경우에는 해당 방향으로 바꾼 후 해준다.
//
//	4	-	ID
//	1	-	Direction	( 방향 디파인 값. 좌/우만 사용 )
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------
struct FIGHTER_REP_ATTACK_002 : PACKET_HEADER {
	int ID;
	Direction Direct;
	short X;
	short Y;
};

//---------------------------------------------------------------
// 캐릭터 공격 패킷							Client -> Server
//
// 공격 키 입력시 본 패킷을 서버에게 보낸다.
// 충돌 및 데미지에 대한 결과는 서버에서 알려 줄 것이다.
//
// 공격 동작 시작시 한번만 서버에게 보내줘야 한다.
//
//	1	-	Direction	( 방향 디파인 값. 좌/우만 사용 )
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------
struct FIGHTER_QRY_ATTACK_003 : PACKET_HEADER {
	Direction Direct;
	short X;
	short Y;
};

//---------------------------------------------------------------
// 캐릭터 공격 패킷							Server -> Client
//
// 패킷 수신시 해당 캐릭터를 찾아서 공격3번 동작으로 액션을 취해준다.
// 방향이 다를 경우에는 해당 방향으로 바꾼 후 해준다.
//
//	4	-	ID
//	1	-	Direction	( 방향 디파인 값. 좌/우만 사용 )
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------
struct FIGHTER_REP_ATTACK_003 : PACKET_HEADER {
	int ID;
	Direction Direct;
	short X;
	short Y;
};

//---------------------------------------------------------------
// 캐릭터 데미지 패킷							Server -> Client
//
// 공격에 맞은 캐릭터의 정보를 보냄.
//
//	4	-	AttackID	( 공격자 ID )
//	4	-	DamageID	( 피해자 ID )
//	1	-	DamageHP	( 피해자 HP )
//
//---------------------------------------------------------------
struct FIGHTER_CMD_DAMAGE : PACKET_HEADER {
	int AttackID;
	int DamageID;
	unsigned char DamageHP;
};


// 사용안함...
//---------------------------------------------------------------
// 동기화를 위한 패킷					Client -> Server
//
//
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------
struct FIGHTER_QRY_SYNC : PACKET_HEADER {
	short X;
	short Y;
};

//---------------------------------------------------------------
// 동기화를 위한 패킷					Server -> Client
//
// 서버로부터 동기화 패킷을 받으면 해당 캐릭터를 찾아서
// 캐릭터 좌표를 보정해준다.
//
//	4	-	ID
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------
struct FIGHTER_REP_SYNC : PACKET_HEADER {
	int ID;
	short X;
	short Y;
};
#pragma pack(pop)