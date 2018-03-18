#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <Archer.h>
#include <Tank.h>
#include <Melee.h>
#include <Healer.h>

using namespace std;
using namespace sf;

/*int GetRandomRol(vector<int> rols) {
	int num = rand() % rols.size;
	int temp = rols.at[num];
	rols.erase(rols.begin() + num);
	return temp;
}*/

Packet GetWorldUpdate(vector<Player> aPlayers, Boss boss) {
	Packet pck;
	string commandHead = "ACT";
	pck << commandHead;
	for (int i = 0; i < aPlayers.size(); i++) {
		pck << aPlayers[i].life;
	}
	pck << boss.life;
	return pck;
}

int main()
{

	/*vector<int> rols; 

	for (int i = 0; i < 4; i++) {
		rols.push_back(i);
	}*/

	std::cout << "Server online\n";
	std::vector<sf::TcpSocket*> aSocket;
	//std::string textoAEnviar = "";
	sf::SocketSelector ss;
	sf::TcpSocket socket;
	int clientN;
	std::string mensaje;
	Packet packet;
	//std::size_t bs;
	bool messageReceived = false;
	int maxPlayers = 2;

	bool game = false;
	bool command = false;
	bool message = false;
	vector<Player> aPlayers;
	Boss boss = Boss();
	int turn = 0;
	string recivedMessage;

	sf::TcpListener listener;
	sf::Socket::Status status = listener.listen(50000);
	if (status != sf::Socket::Status::Done) {
		std::cout << "End";
	}
	//mensaje = "Welcome, waiting for other players";
	//packet << mensaje;
	while (aSocket.size() < maxPlayers) {
		if (status == sf::Socket::Status::Done) {
			sf::TcpSocket* tempSocket = new sf::TcpSocket;
			sf::Socket::Status statusAccept = listener.accept(*tempSocket);
			if (statusAccept != sf::Socket::Status::Done) {
				std::cout << "Error accepting connection\n";
				delete tempSocket;
			}
			else {
				ss.add(*tempSocket);
				aSocket.push_back(tempSocket);
				//tempSocket->send(packet);
				//std::cout << "New client: " << tempSocket->getRemoteAddress().toString() << "\n";

				/*for (int i = 0; i < aSocket.size() - 1; i++) {
					aSocket[i]->send(packet);
				}*/
			}
		}
		game = true;
	}
	std::cout << "\nAll clients online\n";
	listener.close();
	for (int i = 0; i < 4; i++) {
		//packet.clear();
		mensaje = "START";
		packet << mensaje;
		int rol = i;
		packet << rol;
		aSocket[i]->send(packet);

		Player myRol;
		switch (rol)
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

	while (game) {
	
		if (ss.wait()) {
			for (int i = 0; i < aSocket.size(); i++)
			{
				if (ss.isReady(*aSocket[i]))
				{
					Packet packet;
					string typeMessage;

					sf::Socket::Status statusReceive = aSocket[i]->receive(packet);
					if (statusReceive == sf::Socket::NotReady) {
						break;
					}
					else if (statusReceive == sf::Socket::Done) {
						std::cout << "Packet recived from client " << i << endl;
						packet >> typeMessage;

						if (typeMessage == "C") {

							if (i != turn) {
								// Enviar que no es el teu torn, espera boi
								Packet pack;
								string notTurn = "NT";
								pack << notTurn;
								aSocket[i]->send(pack);
							}
							else {
								int receivedRol;
								int receivedCommand;
								packet >> receivedRol >> receivedCommand;
								aPlayers[receivedRol].GetAction(receivedCommand, boss);
								command = true;
							}
						}
						else if (typeMessage == "M") {
							// Receive a chat message
							// Chat
							packet >> recivedMessage;
							

							message = true;
						}

						clientN = i;
						messageReceived = true;
					}
					else if (statusReceive == sf::Socket::Disconnected) {
						ss.remove(*aSocket[i]);

						// Allibera memòria?
						//aSocket[i]->disconnect;

						//aSocket.erase[&i];


						mensaje = "S'ha desconectat el client: " + std::to_string(i);
						aSocket.erase(aSocket.begin() + (i));
						for (int j = 0; j < aSocket.size(); j++) {
							aSocket[j]->send(mensaje.c_str(), mensaje.length());
						}
						break;
					}
					break;
				}
			}

			if (turn == 4) {
				// Calcular damage boss
				// Enviar a tothom
				// Resetejar torns
				turn = 0;
			}

			else if (messageReceived) {
				// Calcules tot el joc
				if (command) {
					Packet packetCommand;
					packetCommand = GetWorldUpdate(aPlayers, boss);
					for (int i = 0; i < aSocket.size(); i++) {
						sf::Socket::Status statusSend = aSocket[i]->send(packetCommand);
						if (statusSend == sf::Socket::Status::Done)
							std::cout << "HP update sent\n";
					}
					command = false;
					messageReceived = false;
					turn++;
					mensaje = "";
				}

				if (message) {
					Packet messagePacket;
					string messageCommand = "M";
					messagePacket << messageCommand << recivedMessage;
					for (int j = 0; j < aSocket.size(); j++) {
						if (j != clientN)
						{
							std::cout << " L'envii al client " << j << "\n";
							sf::Socket::Status statusSend = aSocket[j]->send(messagePacket);
							if (statusSend == sf::Socket::Status::Done)
								std::cout << "L'he enviat correctament \n";
						}
					}
					message = false;
					messageReceived = false;
					mensaje = "";
				}

			}
		}

	}

	/*while (true) {
		if (ss.wait()) {
			for (int i = 0; i < aSocket.size(); i++)
			{
				if (ss.isReady(*aSocket[i]))
				{
					//char buffer[2000];
					//size_t bytesReceived;
					Packet packet;
					string typeMessage;

					sf::Socket::Status statusReceive = aSocket[i]->receive(packet);
					if (statusReceive == sf::Socket::NotReady) {
						break;
					}
					else if (statusReceive == sf::Socket::Done) {
						std::cout << "Packet recived\n";
						packet >> typeMessage;

						if (typeMessage == "C") {
							// Receive a command
							//
						}
						else if (typeMessage == "M") {
							// Receive a chat message
						}

						clientN = i;
						messageReceived = true;
					}
					else if (statusReceive == sf::Socket::Disconnected) {
						ss.remove(*aSocket[i]);

						// Allibera memòria?
						//aSocket[i]->disconnect;

						//aSocket.erase[&i];

						
						mensaje = "S'ha desconectat el client: " + std::to_string(i);
						aSocket.erase(aSocket.begin() + (i));
						for (int j = 0; j < aSocket.size(); j++) {
							aSocket[j]->send(mensaje.c_str(), mensaje.length());
						}
						break;
					}
					break;
				}
			}
			if (messageReceived) {
				for (int j = 0; j < aSocket.size(); j++) {
					if (j != clientN)
					{
						std::cout << " L'envii al client " << j << "\n";
						sf::Socket::Status statusSend = aSocket[j]->send(mensaje.c_str(), mensaje.length(), bs);
						while (statusSend == sf::Socket::Status::Partial)
						{
							mensaje = mensaje.substr(bs + 1, bs);
							statusSend = aSocket[j]->send(mensaje.c_str(), mensaje.length(), bs);
						}
						if (statusSend == sf::Socket::Status::Done)
							std::cout << "L'he enviat correctament \n";
					}
				}
				messageReceived = false;
				mensaje = "";
			}
		}
	}*/
}