#pragma once

#include <iostream>

class GuiEvent {
public:
	virtual ~GuiEvent() = default;
	virtual void excecute() {}
};

class ExitEvent: public GuiEvent {
	void excecute() override { exit(EXIT_SUCCESS); }
};

class UnpauseEvent: public GuiEvent {
	void excecute() override;
};


