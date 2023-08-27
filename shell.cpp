#include <iostream>
#include <cstring>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ncurses.h>

const int MAX_INPUT_LENGTH = 1024;
int main() {
    initscr();
    cbreak(); 
    noecho(); 
    keypad(stdscr, TRUE);

    refresh();

    std::string entrada_texto;
    char* args[MAX_INPUT_LENGTH];
    int status;

    while (true) {
        printw("\nGon> "); 
        refresh();
        entrada_texto = "";

        int ch;
        while ((ch = getch()) != '\n') {
            if (ch == KEY_BACKSPACE || ch == 127) { 
                if (!entrada_texto.empty()) {
                    entrada_texto.pop_back();
                    addch('\b');
                    addch(' '); 
                    addch('\b'); 
                    refresh();
                }
            } else {
                entrada_texto += ch;
                addch(ch); 
                refresh();
            }
        }
        printw("\n");
        refresh();

        if (entrada_texto == "salir") {
            printw("\nSaliendo...\n");
            refresh();
            break;
        }

        std::istringstream iss(entrada_texto);
        int arg_count = 0;
        std::string arg;
        while (iss >> arg) {
            args[arg_count] = new char[arg.length() + 1];
            std::strcpy(args[arg_count], arg.c_str());
            arg_count++;
        }
        args[arg_count] = nullptr;

        pid_t pid = fork();
        if (pid == 0) {
            execvp(args[0], args);
            perror("Error al ejecutar el comando");
            exit(EXIT_FAILURE);
        } else if (pid > 0) {
            waitpid(pid, &status, 0);
        } else {
            perror("Error al crear el proceso hijo");
        }

        for (int i = 0; i < arg_count; i++) {
            delete[] args[i];
        }
    }

    endwin();

    return 0;
}
