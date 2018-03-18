#pragma once
#include <PlayerInfo.h>
#include <SFML\Network.hpp>
#include <iostream>
#include <Player.h>
#include <Archer.h>
#include <Healer.h>
#include <Tank.h>
#include <Melee.h>
#include <Boss.h>

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
	/*Packet pack;
	string msg = "Hello, I'm a new client";
	pack << msg;
	socket.send(pack);*/

	int myRol;
	vector<Player> aPlayers;
	Boss boss = Boss();
	vector<string> aMensajes;
	bool turnActive = false;

	for (int i = 0; i < 4; i++) {
		Player myRol;
		switch (i)
		{
		case 0:
			myRol = Tank();
			break;
		case 1:
			myRol = Archer();
			break;
		case 2:
			myRol = Healer();
			break;
		case 3:
			myRol = Melee();
			break;
		default:
			break;
		}
		aPlayers.push_back(myRol);
	}

	bool waiting = true;
	bool game = false;

	while (waiting) {
		Packet receivedPacket;
		sf::Socket::Status statusReceive = socket.receive(receivedPacket);
		/*if (statusReceive == sf::Socket::NotReady) {
			break;
		}*/
		if (statusReceive == sf::Socket::Done) {
			string msg;
			receivedPacket >> msg;
			cout << msg << endl;
			if (msg == "START") {
				receivedPacket >> myRol;
				cout << myRol << endl;
				waiting = false;
				game = true;
			}

		}
		else if (statusReceive == sf::Socket::Disconnected) {
			return 0;
		}

	}

	while (game && !waiting) {
			//packet << "C" << 1 << 2

		while (true) {
			Packet pack;
			sf::Socket::Status statusReceive = socket.receive(pack);
			if (statusReceive == sf::Socket::NotReady) {
				cout << "Error receiving" << endl;
				break;
			}
			else if (statusReceive == sf::Socket::Done) {
				string command;
				pack >> command;
				cout << "Packet received with command: " + command << endl;
				// No turn
				if (command == "NT") {
					// Block the send if it's an action, the player can't act
					turnActive = false;
				}
				// Action
				else if (command == "ACT") {
					// Recive the world update (each player's life and boss)
					string command;
					pack >> command;
					for (int i = 0; i < 4; i++) {
						int life;
						pack >> life;
						aPlayers[i].life = life;
					}
					int life;
					pack >> life;
					boss.life = life;
					cout << "Tank HP: " + aPlayers[0].life;
					cout << "   Archer HP: " + aPlayers[1].life;
					cout << "   Healer HP: " + aPlayers[2].life;
					cout << "   Melee HP: " + aPlayers[3].life;	
					cout << "   BOSS HP: " + boss.life << endl;

				}
				// Your turn
				else if (command == "TURN") {
					// The player can act (unlock turn)
					turnActive = true;
				}
				else if (command == "M") {
					// Chat message
					string messagereceived;
					pack >> messagereceived;
					messagereceived += "\0";
					aMensajes.push_back(messagereceived);
					cout << "Message received: " + messagereceived << endl;
				}
			}
			else if (statusReceive == sf::Socket::Disconnected) {
				return 0;
			}
		}

		sf::Vector2i screenDimensions(800, 600);
		sf::RenderWindow window;
		window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Game");
		sf::Font font;
		if (!font.loadFromFile("arial.ttf"))
		{
			std::cout << "Can't load the font file" << std::endl;
		}

		std::string mensaje = " >";

		sf::Text chattingText(mensaje, font, 14);
		chattingText.setFillColor(sf::Color(0, 160, 0));
		chattingText.setStyle(sf::Text::Bold);
		
		sf::Text text(mensaje, font, 14);
		text.setFillColor(sf::Color(0, 160, 0));
		text.setStyle(sf::Text::Bold);
		text.setPosition(0, 560);

		sf::RectangleShape separator(sf::Vector2f(800, 5));
		separator.setFillColor(sf::Color(200, 200, 200, 255));
		separator.setPosition(0, 550);
		while (window.isOpen())
		{
			sf::Event evento;
			while (window.pollEvent(evento))
			{
				switch (evento.type)
				{
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::KeyPressed:
					if (evento.key.code == sf::Keyboard::Escape)
						window.close();
					else if (evento.key.code == sf::Keyboard::Return)
					{
						aMensajes.push_back(mensaje);
						if (aMensajes.size() > 25)
						{
							aMensajes.erase(aMensajes.begin(), aMensajes.begin() + 1);
						}
						if (mensaje == ">exit" || mensaje == "exit")
						{
							std::size_t bs;
							mensaje = "Chat ended";
							std::cout << mensaje << "\n";
							/*sf::Socket::Status statusSend = socket.send(mensaje.c_str(), mensaje.length(), bs);
							while (statusSend == sf::Socket::Status::Partial)
							{
								mensaje = mensaje.substr(bs + 1, bs);
								statusSend = socket.send(mensaje.c_str(), mensaje.length(), bs);
							}*/
							mensaje = ">";
							window.display();
							window.clear();
							window.close();
							socket.disconnect();
						}
						
						else {//SEND
							std::size_t bs;
							std::cout << mensaje << "\n";
							string com;
							if (mensaje.find_first_of("#") == string::npos) {
								// Chat message
								Packet pack;
								string messageCommand = "M";
								pack << messageCommand << mensaje;
								sf::Socket::Status statusSend = socket.send(pack);
								while (socketStatus == Socket::Status::Done) {
									cout << "Message sent" << endl;
								}
							}
							else {
								// Command
								com = mensaje.substr(mensaje.find_first_of(">") + 1, mensaje.find_first_of("#"));
								if (com == "C") {
									// To int, action
									//if (intAction == 1 || 2...) 
									//else // No es correcte
								}
							}
							/*sf::Socket::Status statusSend = socket.send(mensaje.c_str(), mensaje.length(), bs);
							while (statusSend == sf::Socket::Status::Partial)
							{
								mensaje = mensaje.substr(bs + 1, bs);
								statusSend = socket.send(mensaje.c_str(), mensaje.length(), bs);
							}*/
							mensaje = ">";
							}
						}
						break;
					case sf::Event::TextEntered:
						if (evento.text.unicode >= 32 && evento.text.unicode <= 126)
							mensaje += (char)evento.text.unicode;
						else if (evento.text.unicode == 8 && mensaje.length() > 0)
							mensaje.erase(mensaje.length() - 1, mensaje.length());
						break;
					}

				}
				/*while (true) {

					char buffer[2000];
					size_t bytesReceived;

					sf::Socket::Status statusReceive = socket.receive(buffer, 2000, bytesReceived);
					if (statusReceive == sf::Socket::NotReady) {
						break;
					}
					else if (statusReceive == sf::Socket::Done) {
						buffer[bytesReceived] = '\0';
						aMensajes.push_back(buffer);
					}
					else if (statusReceive == sf::Socket::Disconnected) {
						return 0;
					}
				}*/

				window.draw(separator);
				for (size_t i = 0; i < aMensajes.size(); i++)
				{
					std::string chatting = aMensajes[i];
					chattingText.setPosition(sf::Vector2f(0, 20 * i));
					chattingText.setString(chatting);
					window.draw(chattingText);
				}
				std::string mensaje_ = mensaje + "_";
				text.setString(mensaje_);
				window.draw(text);


				window.display();
				window.clear();
			}
		}

}

/*
std::vector<std::string> aMensajes;
sf::Vector2i screenDimensions(800, 600);

sf::RenderWindow window;
window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Game");


sf::Font font;
if (!font.loadFromFile("arial.ttf"))
{
	std::cout << "Can't load the font file" << std::endl;
}

//std::string mensaje = " >";

sf::Text chattingText(mensaje, font, 14);
chattingText.setFillColor(sf::Color(0, 160, 0));
chattingText.setStyle(sf::Text::Bold);


sf::Text text(mensaje, font, 14);
text.setFillColor(sf::Color(0, 160, 0));
text.setStyle(sf::Text::Bold);
text.setPosition(0, 560);

sf::RectangleShape separator(sf::Vector2f(800, 5));
separator.setFillColor(sf::Color(200, 200, 200, 255));
separator.setPosition(0, 550);

while (window.isOpen())
{
	sf::Event evento;
	while (window.pollEvent(evento))
	{
		switch (evento.type)
		{
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::KeyPressed:
			if (evento.key.code == sf::Keyboard::Escape)
				window.close();
			else if (evento.key.code == sf::Keyboard::Return)
			{
				aMensajes.push_back(mensaje);
				if (aMensajes.size() > 25)
				{
					aMensajes.erase(aMensajes.begin(), aMensajes.begin() + 1);
				}
				if (mensaje == ">exit" || mensaje == "exit")
				{
					std::size_t bs;
					mensaje = "Chat ended";
					std::cout << mensaje << "\n";
					sf::Socket::Status statusSend = socket.send(mensaje.c_str(), mensaje.length(), bs);
					while (statusSend == sf::Socket::Status::Partial)
					{
						mensaje = mensaje.substr(bs + 1, bs);
						statusSend = socket.send(mensaje.c_str(), mensaje.length(), bs);
					}
					mensaje = ">";
					window.display();
					window.clear();
					window.close();
					socket.disconnect();
				}

				else {//SEND
					std::size_t bs;
					std::cout << mensaje << "\n";
					sf::Socket::Status statusSend = socket.send(mensaje.c_str(), mensaje.length(), bs);
					while (statusSend == sf::Socket::Status::Partial)
					{
						mensaje = mensaje.substr(bs + 1, bs);
						statusSend = socket.send(mensaje.c_str(), mensaje.length(), bs);
					}
					mensaje = ">";
				}
			}
			break;
		case sf::Event::TextEntered:
			if (evento.text.unicode >= 32 && evento.text.unicode <= 126)
				mensaje += (char)evento.text.unicode;
			else if (evento.text.unicode == 8 && mensaje.length() > 0)
				mensaje.erase(mensaje.length() - 1, mensaje.length());
			break;
		}

	}
	while (true) {

		char buffer[2000];
		size_t bytesReceived;

		sf::Socket::Status statusReceive = socket.receive(buffer, 2000, bytesReceived);
		if (statusReceive == sf::Socket::NotReady) {
			break;
		}
		else if (statusReceive == sf::Socket::Done) {
			buffer[bytesReceived] = '\0';
			aMensajes.push_back(buffer);
		}
		else if (statusReceive == sf::Socket::Disconnected) {
			return 0;
		}
	}

	window.draw(separator);
	for (size_t i = 0; i < aMensajes.size(); i++)
	{
		std::string chatting = aMensajes[i];
		chattingText.setPosition(sf::Vector2f(0, 20 * i));
		chattingText.setString(chatting);
		window.draw(chattingText);
	}
	std::string mensaje_ = mensaje + "_";
	text.setString(mensaje_);
	window.draw(text);


	window.display();
	window.clear();
}
socket.disconnect();*/