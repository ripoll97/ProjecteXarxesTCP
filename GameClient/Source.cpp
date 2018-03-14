#pragma once
#include <PlayerInfo.h>
#include <SFML\Network.hpp>
#include <iostream>

using namespace std;
using namespace sf;

int main()
{
	std::cout << "Client online\n";

	sf::TcpSocket socket;
	socket.setBlocking(false);
	sf::Socket::Status socketStatus = socket.connect("localhost", 50000, sf::milliseconds(15.f));
	if (socketStatus == sf::Socket::Status::Done) {
		std::cout << "Connected to server\n";
	}


}