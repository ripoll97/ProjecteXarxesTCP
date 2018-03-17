#include <SFML\Network.hpp>
#include <SFML\Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>

using namespace std;
using namespace sf;

vector<TcpSocket*>aSock;

struct Direction
{
	string ip;
	unsigned short port;
	Direction(string _ip, unsigned short _port) : ip(_ip), port(_port) {}
	Direction() {}
};

void main() 
{
	int maxPeers;
	int conectedPeers;
	vector<Direction>aPeers;

	TcpSocket sock;
	sock.connect("localhost", 50000);
	Packet packet;
	string str;
	Socket::Status statusReceive = sock.receive(packet);
	if (statusReceive == Socket::Status::Done) 
	{
		packet >> str;
		cout << str << endl;
		maxPeers = stoi(str.substr(0, str.find_first_of('+')));
		conectedPeers = stoi(str.substr(str.find_first_of('+') + 1, str.find_first_of('-'))); 
		//cout << "Maxpeers: " << maxPeers << "\nConectedPeers: " << conectedPeers << "\n";
		str = str.substr(str.find_first_of('-') + 1, str.at(str.size() - 1));
		for (int i = 0; i < conectedPeers; i++) {
			cout << "Tallada " << str << endl;

			Direction dir;
			TcpSocket* sock = new TcpSocket;

			dir.ip = str.substr(0, str.find_first_of('_'));

			dir.port = (unsigned short) stoi(str.substr(str.find_first_of('_') + 1, str.find_first_of('H') - 1).c_str());

			sock->connect(dir.ip, dir.port);
			sock->setBlocking(false);
			aSock.push_back(sock);

			//cout << "El seu port �s: " << dir.port << " i en String: " << str.substr(str.find_first_of('_') + 1, str.find_first_of('#') - 1) << "\n";

			aPeers.push_back(dir);

			str = str.substr(str.find_first_of('H') + 1, str.size() - 1); 
			//cout << "Client afegit, mida: " << aPeers.size() << "\n";
		}
	}
	// The client connects with the others who have already connected

	int port2 = sock.getLocalPort();
	sock.disconnect();
	TcpListener listener;
	listener.listen(port2);
	// The client stays listening for new clients
	for (int i = conectedPeers; i < maxPeers - 1; i++) 
	{
		TcpSocket* sockAux = new TcpSocket;
		listener.accept(*sockAux);
		sockAux->setBlocking(false);
		aSock.push_back(sockAux);
		cout << "Client nou trobat\n";
	}
	cout << "Tots conectats: \n";
	for (int i = 0; i < aSock.size(); i++) {
		cout << aSock.at(i)->getRemotePort() << "\n";
	}

	listener.close();
	//cout << "Ja estan tots!" << aSock.size();

	// CHAT
	std::vector<std::string> aMensajes;

	sf::Vector2i screenDimensions(800, 600);

	sf::RenderWindow window;
	window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Chat");

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
						for (int i = 0; i < aSock.size(); i++) {
							sf::Socket::Status statusSend = aSock.at(i)->send(mensaje.c_str(), mensaje.length(), bs);
							while (statusSend == sf::Socket::Status::Partial)
							{
								mensaje = mensaje.substr(bs + 1, bs);
								statusSend = aSock.at(i)->send(mensaje.c_str(), mensaje.length(), bs);
							}
							aSock.at(i)->disconnect();
							aSock.erase(aSock.begin() + i);
						}
						mensaje = ">";
						window.display();
						window.clear();
						window.close();
					}

					else {//SEND
						std::size_t bs;
						std::cout << mensaje << "\n";
						for each (TcpSocket* sock in aSock) {
							sf::Socket::Status statusSend = sock->send(mensaje.c_str(), mensaje.length(), bs);
							while (statusSend == sf::Socket::Status::Partial)
							{
								mensaje = mensaje.substr(bs + 1, bs);
								statusSend = sock->send(mensaje.c_str(), mensaje.length(), bs);
							}
							mensaje = ">";
						}
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

			
				sf::Socket::Status statusReceive = aSock.at(0)->receive(buffer, 2000, bytesReceived);
				if (statusReceive == sf::Socket::NotReady) {
					break;
				}
				else if (statusReceive == sf::Socket::Done) {
					buffer[bytesReceived] = '\0';
					aMensajes.push_back(buffer);
				}
				else if (statusReceive == sf::Socket::Disconnected) {
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
	//socket.disconnect();
}