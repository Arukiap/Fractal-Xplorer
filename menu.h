#include <iostream>
#include <cstdlib>

#define SDL_MAIN_HANDLED //https://stackoverflow.com/questions/32342285/undefined-reference-to-winmain16-c-sdl-2/32343111#32343111
#include <SDL2/SDL.h>

#ifndef MENU_H
#define MENU_H

class Menu{

    enum MenuState {MainMenu, SelectFractal, Options};

    public:
        Menu();
        void OpenMenu();
        bool IsOpen();
        bool IsExit();
        int ListenInput();
        int GetSelectedFractal();
    private:
        void printMainMenu();
        void printOptionsMenu();
        void printSelectionMenu();
        void clearConsole();
        void Navigate(int option);
        void NavigateMainMenu(int option);
        void NavigateOptionsMenu(int option);
        void NavigateSelectionMenu(int option);
        void ExitMenu();
        MenuState state;
        bool isOpen,isExit;
        int selectedFractal;
};


#endif // MENU_H