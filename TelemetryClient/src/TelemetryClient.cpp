#include "TelemetryClient.h"
#include <string>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include "types\struct\SPacketHeader.h"
#include "types\enum\EPacket.h"
#pragma comment(lib,"WS2_32")


#define BUFFER_LEN 2048

std::string GetPackedID(unsigned char PacketType) {
	EPacket type = static_cast<EPacket>(PacketType);
	switch (type) {
		case EPacket::Motion:
			return "Motion";
		case EPacket::Session:
			return "Session";
		case EPacket::LapData:
			return "LapData";
		case EPacket::Event:
			return "Event";
		case EPacket::Participants:
			return "Participants";
		case EPacket::CarSetups:
			return "CarSetups";
		case EPacket::CarTelemetry:
			return "CarTelemetry";
		case EPacket::CarStatus:
			return "CarStatus";
		case EPacket::FinalClassification:
			return "FinalClassification";
		case EPacket::LobbyInfo:
			return "LobbyInfo";
		case EPacket::CarDamage:
			return "CarDamage";
		case EPacket::SessionHistory:
			return "SessionHistory";
		default:
			return "None";
	}
}


void TelemetryClient::CreateSocket() {
	int iResult;
	WSADATA wsaData;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		return;
	}

	int slen, recv_len;
	char buffer[BUFFER_LEN];
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct sockaddr_in server, si_other;

	slen = sizeof(si_other);

	if ((ConnectSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
		WSACleanup();
		return;
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(20777);

	if (bind(ConnectSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
		WSACleanup();
		return;
	}

	puts("Bind done");

	SPacketHeader header;


	while (1) {
		fflush(stdout);

		//clear the buffer by filling null, it might have previously received data
		memset(buffer, '\0', BUFFER_LEN);

		//try to receive some data, this is a blocking call
		if ((recv_len = recvfrom(ConnectSocket, buffer, BUFFER_LEN, 0, (struct sockaddr*)&si_other, &slen)) == SOCKET_ERROR) {
			printf("recvfrom() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		memcpy(&header, buffer, sizeof(header));

		//print details of the client/peer and the data received
		//printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
		printf("%d %d.%d Packet=%s Frame=%d\n", header.m_packetFormat, header.m_gameMajorVersion, header.m_gameMinorVersion, GetPackedID(header.m_packetId).c_str(), header.m_frameIdentifier);
	}

	/*
	SOCKET ConnectSocket = INVALID_SOCKET;
	if ((ConnectSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		WSACleanup();
		return;
	}

	char buffer[BUFFER_LEN];
	char address[INET_ADDRSTRLEN];
	struct sockaddr_in si_other;
	memset((char*)&si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(20777);
	si_other.sin_addr.S_un.S_addr = inet_pton(AF_INET, "127.0.0.1", address);
	int slen = sizeof(si_other);
	while (1)
	{
		if (recvfrom(ConnectSocket, buffer, BUFFER_LEN, 0, (struct sockaddr*)&si_other, &slen) == SOCKET_ERROR)
		{
			WSACleanup();
			return;
		}

		puts(buffer);
	}
	*/
}
