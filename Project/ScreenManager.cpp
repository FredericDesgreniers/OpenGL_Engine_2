#include "ScreenManager.h"

ScreenManager::ScreenManager(int width, int height)
{
	this->width = width;
	this->height = height;
}
Screen* ScreenManager::getCurrentScreen()
{
	return currentScreen;
}

Screen* ScreenManager::setCurrentScreen(Screen* screen)
{
	Screen* oldScreen = currentScreen;
	currentScreen = screen;
	return oldScreen;
}
int ScreenManager::getHeight()
{
	return height;
}

int ScreenManager::getWidth()
{
	return width;
}

void ScreenManager::setDimensions(int width, int height)
{
	this->width = width;
	this->height = height;
}

void ScreenManager::setHeight(int height)
{
	this->height = height;
}

void ScreenManager::setWidth(int width)
{
	this->width = width;
}


