#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define RL_BUFF_SIZE 1024
#define FAILURE_CODE 1
#define TOK_BUFSIZE 64
#define TOK_ESCAPE "\t\r\n\a"

using namespace std;

int SHell_cd(char **args);
int SHell_help(char **args);
int SHell_exit(char **args);
int SHell_launch(char **args);

char const *builtin_str[] = {
    "cd",
    "help",
    "exit"};

int (*builtin_func[])(char **) = {
    &SHell_cd,
    &SHell_help,
    &SHell_exit};

int SHell_num_builtins()
{
    return sizeof(builtin_str) / sizeof(char *);
}

int SHell_cd(char **args)
{
    if (args[1] == NULL)
    {
        cout << "SHell: expected argument to \"cd\"" << endl;
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            perror("SHell");
        }
    }
    return 1;
}

int SHell_help(char **args)
{
    int i;
    cout << "Wojciech Binkowski's SHell" << endl;
    cout << "Type program names and arguments, and hit enter. " << endl;
    cout << "The following are built in: " << endl;

    for (i = 0; i < SHell_num_builtins(); i++)
    {
        cout << builtin_str[i] << endl;
    }

    cout << "Use the man command for information on other programs. " << endl;
    return 1;
}

int SHell_exit(char **args)
{
    return 0;
}

int execute(char **args)
{
    int i;

    if (args[0] == NULL)
    {
        return 1;
    }

    for (i = 0; i < SHell_num_builtins(); i++)
    {
        if (strcmp(args[0], builtin_str[i]) == 0)
        {
            return (*builtin_func[i])(args);
        }
    }

    return SHell_launch(args);
}

char *read_line(void)
{
    int buffSize = RL_BUFF_SIZE;
    int position = 0;
    char *buffer = (char *)malloc(sizeof(char) * buffSize);
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

    if (position >= buffSize)
    {
        buffSize += RL_BUFF_SIZE;
        buffer = (char *)realloc(buffer, buffSize);
        if (!buffer)
        {
            cout << "SHell: allocation error";
            exit(FAILURE_CODE);
        }
    }
}

char **split_line(char *line)
{
    int buffer_size = TOK_BUFSIZE, position = 0;
    char **tokens = (char **)malloc(buffer_size * sizeof(char *));
    char *token;

    if (!tokens)
    {
        cout << "SHell: allocation error";
        exit(EXIT_FAILURE);
    }

    token = strtok(line, TOK_ESCAPE);
    while (token != NULL)
    {
        tokens[position] = token;
        position++;

        if (position >= buffer_size)
        {
            buffer_size += TOK_BUFSIZE;
            tokens = (char **)realloc(tokens, buffer_size * sizeof(char *));
            if (!tokens)
            {
                cout << "Allocation Error";
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

int SHell_launch(char **args)
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        if (execvp(args[0], args) == -1)
        {
            perror("SHell");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        perror("lsh");
    }
    else
    {
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int main()
{

    shell_loop();

    return 0;
}