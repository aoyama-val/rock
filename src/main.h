#ifndef MAIN_H
#define MAIN_H

int main(int argc, char* argv[]);
void gameStart();
void action();
void init();
void initGL();
void opening();
void loadTextures();
void deleteTextures();
void createDisplayLists();
void deleteDisplayLists();
void loadModels();
void clearDeadObjs();
void parseArgs(int argc, char* argv[]);

#endif
