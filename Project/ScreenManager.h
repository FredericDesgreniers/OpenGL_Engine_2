#pragma once
#include "Screen.h"
class Screen;
class ScreenManager
{
private:
	Screen* currentScreen;

	int width;
	int height;
public:
	ScreenManager(int width, int height);

	Screen* getCurrentScreen();
	Screen* setCurrentScreen(Screen* screen);

	int getWidth();
	int getHeight();

	void setWidth(int width);
	void setHeight(int height);

	void setDimensions(int width, int height);
};
