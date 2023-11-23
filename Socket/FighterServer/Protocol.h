// ��Ŷ ������ ����
#include "Direction.h"

#define HEADER_SIZE 3

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
	unsigned char ByCode;	// ��Ŷ�ڵ� 0x89 ����.
	unsigned char BySize;	// ��Ŷ ������.
	unsigned char ByType;	// ��ŶŸ��.
};

#pragma pack(pop)