#pragma once

struct SPacketHeader {
	unsigned short m_packetFormat;				// 2022
	unsigned char m_gameMajorVersion;			// Game major version - "X.00"
	unsigned char m_gameMinorVersion;			// Game minor version - "1.XX"
	unsigned char m_packetVersion;				// Version of this packet type, all start from 1
	unsigned char m_packetId;					// Identifier for the packet type, see below
	unsigned long m_sessionUID;					// Unique identifier for the session
	float m_sessionTime;						// Session timestamp
	unsigned int m_frameIdentifier;				// Identifier for the frame the data was retrieved on
	unsigned char m_playerCarIndex;				// Index of player's car in the array
	unsigned char m_secondaryPlayerCarIndex;	// Index of secondary player's car in the array (splitscreen), 255 if no second player
};
