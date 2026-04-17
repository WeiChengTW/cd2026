#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEXEME_LEN 256

static int isSpace(int c)
{
    return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

static int isDigit(int c)
{
    return c >= '0' && c <= '9';
}

static int isAlpha(int c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static int isAlphaNum(int c)
{
    return isAlpha(c) || isDigit(c);
}

static void emitToken(const char *lexeme, const char *token)
{
    printf("%s: %s\n", lexeme, token);
}

static const char *keywordToken(const char *lexeme)
{
    if (strcmp(lexeme, "int") == 0)
        return "TYPE_TOKEN";
    if (strcmp(lexeme, "main") == 0)
        return "MAIN_TOKEN";
    if (strcmp(lexeme, "if") == 0)
        return "IF_TOKEN";
    if (strcmp(lexeme, "else") == 0)
        return "ELSE_TOKEN";
    if (strcmp(lexeme, "while") == 0)
        return "WHILE_TOKEN";

    return "ID_TOKEN";
}

int main(int argc, char *argv[])
{

    // int cd2026 = 2026;
    // int cd2026_ = 115;
    // if (cd2026 == 2026)
    // {
    //     cd2026_ = 0;
    // }
    // else
    // {
    //     cd2026_ = 1 + 2 + 3 + (4 + 5);
    // }
    // while (cd2026 >= 0)
    // {

    //     cd2026_ = cd2026 + 1;
    // }

    const char *fileName = "input.c";
    FILE *file;
    int ch;

    if (argc > 1)
        fileName = argv[1];

    file = fopen(fileName, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Cannot open file: %s\n", fileName);
        return 1;
    }

    while ((ch = fgetc(file)) != EOF)
    {
        if (isSpace(ch))
        {
            continue;
        }

        if (isAlpha(ch))
        {
            char lexeme[MAX_LEXEME_LEN];
            int len = 0;

            do
            {
                if (len < MAX_LEXEME_LEN - 1)
                    lexeme[len++] = (char)ch;
                ch = fgetc(file);
            } while (ch != EOF && isAlphaNum(ch));

            lexeme[len] = '\0';
            emitToken(lexeme, keywordToken(lexeme));

            if (ch != EOF)
                ungetc(ch, file);

            continue;
        }

        if (isDigit(ch))
        {
            char lexeme[MAX_LEXEME_LEN];
            int len = 0;

            do
            {
                if (len < MAX_LEXEME_LEN - 1)
                    lexeme[len++] = (char)ch;
                ch = fgetc(file);
            } while (ch != EOF && isDigit(ch));

            lexeme[len] = '\0';
            emitToken(lexeme, "LITERAL_TOKEN");

            if (ch != EOF)
                ungetc(ch, file);

            continue;
        }

        if (ch == '/')
        {
            int next = fgetc(file);

            if (next == '/')
            {
                while ((ch = fgetc(file)) != EOF && ch != '\n')
                {
                }
                continue;
            }

            if (next == '*')
            {
                int prev = 0;
                while ((ch = fgetc(file)) != EOF)
                {
                    if (prev == '*' && ch == '/')
                        break;
                    prev = ch;
                }
                continue;
            }

            if (next != EOF)
                ungetc(next, file);
            continue;
        }

        switch (ch)
        {
        case '(':
            emitToken("(", "LEFTPAREN_TOKEN");
            break;
        case ')':
            emitToken(")", "REFTPAREN_TOKEN");
            break;
        case '{':
            emitToken("{", "LEFTBRACE_TOKEN");
            break;
        case '}':
            emitToken("}", "REFTBRACE_TOKEN");
            break;
        case ';':
            emitToken(";", "SEMICOLON_TOKEN");
            break;
        case '+':
            emitToken("+", "PLUS_TOKEN");
            break;
        case '-':
            emitToken("-", "MINUS_TOKEN");
            break;
        case '=':
        {
            int next = fgetc(file);
            if (next == '=')
                emitToken("==", "EQUAL_TOKEN");
            else
            {
                emitToken("=", "ASSIGN_TOKEN");
                if (next != EOF)
                    ungetc(next, file);
            }
            break;
        }
        case '>':
        {
            int next = fgetc(file);
            if (next == '=')
                emitToken(">=", "GREATEREQUAL_TOKEN");
            else
            {
                emitToken(">", "GREATER_TOKEN");
                if (next != EOF)
                    ungetc(next, file);
            }
            break;
        }
        case '<':
        {
            int next = fgetc(file);
            if (next == '=')
                emitToken("<=", "LESSEQUAL_TOKEN");
            else
            {
                emitToken("<", "LESS_TOKEN");
                if (next != EOF)
                    ungetc(next, file);
            }
            break;
        }
        default:
            break;
        }
    }

    fclose(file);

    return 0;
}
