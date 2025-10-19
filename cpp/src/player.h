#pragma once

#include <string>
#include <unistd.h>

class Player {

	bool running = false;
	int terminalPid = 0;

	int makeRequest();

  public:
	static std::string WHITE;
	static std::string BLACK;

	std::string Name;
	std::string Cmd; //"nc localhost 8080"
	int Socket;
	std::string Type;

	Player(std::string name, std::string type, std::string cmd)
		: Name(name), Type(type), Cmd(cmd) {};
	Player() {};
	~Player() {

	};

	void CleanUp();
	bool Run();
	int Prompt();
	void Msg(std::string);
};
