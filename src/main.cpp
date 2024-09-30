#include <iostream>
#include <stdlib.h>

#define RL_BUFF_SIZE = 1024
#define FAILURE_CODE = 1
#define TOK_BUFSIZE =  64
#define TOK_ESCAPE = "\t\r\n\a"

using namespace std;

char *read_line(void)
{
    int buffSize = RL_BUFF_SIZE;
    int postiion = 0;
    char *buffer = malloc(sizeof(char) * buffSize);
    int c;

    if (!buffer)
    {
        cout << "SHell: allocation error";
        exit(FAILURE_CODE);
    }

    while (1)
    {
        c = getchar();

        if (c == EOF || c == '\n')
        {
            buffer[position] = '\0';
            return buffer;
        }
        else
        {
            buffer[position] = c;
        }
        position++;
    }

    if(position >= buffSize){
        buffSize ++RL_BUFF_SIZE;
        buffer = realloc(buffer, buffSize);
        if(!buffer){
            cout<<"SHell: allocation error";
            exit(FAILURE_CODE);
        }
    }
}

char **split_line(char *line){
    int buffer_size = TOK_BUFSIZE, position = 0;
    char **tokens = malloc(buffer_size * sizeof(char*));
    char *token;


    if(!tokens){
        cout<<"SHell: allocation error";
        exit(EXIT_FAILURE);
    }

    token = strtok(line, TOK_ESCAPE);
    while(token != NULL){
        tokens[position] = token;
        position ++;

        if(position >= buffer_size){
            buffer_size += TOK_BUFSIZE;
            tokens = realloc(tokens, buffer_size * sizeof(char*) );
            if(!tokens){
                cout<<"Allocation Error";
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, TOK_ESCAPE);
    }
    tokens[position] = NULL;
    return tokens;
}

void shell_loop()
{
    char *line;
    char **args;
    int status;

    do
    {
        cout << ">";
        line = read_line();
        args = split_line(line);
        status = execute(args);

        free(line);
        free(args);
    } while (status);
}

void terminal_init()
{
    system("cls");
    cout << '>';
    cin <<
}

int main()
{

    shell_loop();

    return 0;
}