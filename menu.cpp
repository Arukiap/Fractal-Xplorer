#include "menu.h"


const char *selectionMenuText = "       << Select your Fractal >>\n\n"
                                "1: Mandelbulb\n"
                                "2: Sierpinski Tetrahedron\n"
                                "3: Julia Quaternion\n"
                                "4: Mandelbox\n";

const char *title = "      :::::::::: :::::::::      :::      :::::::: ::::::::::: :::     :::           :::    ::: :::::::::  :::        ::::::::  :::::::::  :::::::::: ::::::::: \n"
                    "     :+:        :+:    :+:   :+: :+:   :+:    :+:    :+:   :+: :+:   :+:           :+:    :+: :+:    :+: :+:       :+:    :+: :+:    :+: :+:        :+:    :+: \n"
                    "    +:+        +:+    +:+  +:+   +:+  +:+           +:+  +:+   +:+  +:+            +:+  +:+  +:+    +:+ +:+       +:+    +:+ +:+    +:+ +:+        +:+    +:+  \n"
                    "   :#::+::#   +#++:++#:  +#++:++#++: +#+           +#+ +#++:++#++: +#+             +#++:+   +#++:++#+  +#+       +#+    +:+ +#++:++#:  +#++:++#   +#++:++#:    \n"
                    "  +#+        +#+    +#+ +#+     +#+ +#+           +#+ +#+     +#+ +#+            +#+  +#+  +#+        +#+       +#+    +#+ +#+    +#+ +#+        +#+    +#+    \n"
                    " #+#        #+#    #+# #+#     #+# #+#    #+#    #+# #+#     #+# #+#           #+#    #+# #+#        #+#       #+#    #+# #+#    #+# #+#        #+#    #+#     \n"
                    "###        ###    ### ###     ###  ########     ### ###     ### ##########    ###    ### ###        ########## ########  ###    ### ########## ###    ###      \n"
                    "                                                                                    \n\n"
                    "   - An interactive generator that uses ray marching to render 3D fractals in realtime\n\n\n\n";

const char *mainMenuText = "<< Main menu (type number and press enter to choose option)>>\n\n"
                           "1: Launch\n"
                           "2: Exit\n";

Menu::Menu(){}

void Menu::OpenMenu(){
    this->state = MenuState::MainMenu;
    this->isOpen = true;
    this->isExit = false;
    this->selectedFractal = 1;

    clearConsole();
    printMainMenu();

    while(IsOpen()){
        int keyPressed = ListenInput();
        if(keyPressed!=0){
            Navigate(keyPressed);
        }
    }
}

void Menu::Navigate(int option){
    clearConsole();
    switch(this->state){
        case MenuState::MainMenu:
            NavigateMainMenu(option);
        break;
        case MenuState::Options:
            //NavigateOptionsMenu(option);
        break;
        case MenuState::SelectFractal:
            NavigateSelectionMenu(option);
        break;
        default:
        break;
    }
}

void Menu::NavigateMainMenu(int option){
    switch(option){
        case 1:
            this->state = MenuState::SelectFractal;
            printSelectionMenu();
        break;
        //case 2:
        //    printOptionsMenu();
        //break;
        case 2:
            ExitMenu();
        default:
        break;
    }
}

void Menu::NavigateSelectionMenu(int option){
    this->isOpen = false;
    this->selectedFractal = option;
}

void Menu::printMainMenu(){
    std::cout << title << mainMenuText;
}

void Menu::printSelectionMenu(){
    std::cout << selectionMenuText;
}

void Menu::clearConsole(){
    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif
}

bool Menu::IsOpen(){
    return this->isOpen;
}

bool Menu::IsExit(){
    return this->isExit;
}

int Menu::GetSelectedFractal(){
    return this->selectedFractal;
}

int Menu::ListenInput(){
    int number;
    std::cin >> number;
    if(number>0 && number<9){
        return number;
    } else {
        return 0;
    }
}

void Menu::ExitMenu(){
    this->isOpen = false;
    this->isExit = true;
}

