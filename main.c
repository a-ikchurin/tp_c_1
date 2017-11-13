#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char** StringArray;
typedef const char* const* ConstStringArray;

const size_t BUFF_SIZE = 10;
const size_t N_STRINGS = 10;
const size_t REALLOC_COEFFICIENT = 2;

char* readString();
size_t readStrings(StringArray* dest);
size_t lowerCaseStrings(ConstStringArray strings, size_t size, StringArray* dest);
char lowerCaseChar(char c);

int main()
{
    //input and init
    StringArray in_strings;
    size_t in_n = readStrings(&in_strings);
    if (in_n == 0) {
        printf("[error]");
        return 0;
    }
    //work
    StringArray out_strings;
    size_t out_n = lowerCaseStrings((ConstStringArray)in_strings, in_n, &out_strings);
    if (out_n == 0) {
        printf("[error]");
        return 0;
    }
    //output
    size_t i;
    for (i = 0; i < out_n; ++i) {
        puts(out_strings[i]);
    }
    //free
    for (i = 0; i < in_n; ++i) {
        free(in_strings[i]);
    }
    free(in_strings);
    for (i = 0; i < out_n; ++i) {
        free(out_strings[i]);
    }
    free(out_strings);
    return 0;
}

char* readString() {
    size_t cur_size = BUFF_SIZE;
    char* str = (char*)malloc(sizeof(char) * cur_size);
    if (str == NULL) {
        return NULL;
    }
    char cur_char = getchar();
    size_t i;
    for (i = 0; cur_char != '\n' && cur_char != EOF; ++i, cur_char = getchar()) {
        //grow if necessary
        if (i >= cur_size) {
            cur_size *= REALLOC_COEFFICIENT;
            char* res = realloc(str, sizeof(char) * cur_size);
            if (res == NULL) {
                free(str);
                return NULL;
            }
            str = res;
        }
        //add
        str[i] = cur_char;
    }
    //free and return if EOF is first read
    if (i == 0 && cur_char == EOF) {
        free(str);
        return NULL;
    }
    //grow if necessary
    if (i >= cur_size) {
        ++cur_size;
        void * res = realloc(str, sizeof(char) * cur_size);
        if (res == NULL) {
            free(str);
            return NULL;
        }
        str = res;
    }
    //add a terminating null character
    str[i] = '\0';
    return str;
}

size_t readStrings(StringArray* dest) {
    //check parameters
    if (dest == NULL) return 0;
    size_t cur_size = N_STRINGS;
    StringArray strings = (char**)malloc(sizeof(char*) * cur_size);
    if (strings == NULL) return 0;
    size_t i;
    char * cur_str = readString();
    for (i = 0; !feof(stdin) && !ferror(stdin); ++i, cur_str = readString()) {
        //free if an error has occured or if read empty string
        if (cur_str == NULL || cur_str[0] == '\0') {
            size_t j;
            for (j = 0; j < i; ++j) {
                free(strings[j]);
            }
            free(strings);
            if (cur_str != NULL) free(cur_str);
            return 0;
        }
        //grow if necessary
        if (i >= cur_size) {
            cur_size *= REALLOC_COEFFICIENT;
            StringArray res = realloc(strings, sizeof(char*) * cur_size);
            if (res == NULL) {
                free(strings);
                return 0;
            }
            strings = res;
        }
        //add
        strings[i] = cur_str;
    }
    //free if an error has occured or nothing read
    if (ferror(stdin) || (i == 0 && cur_str == NULL) ) {
        size_t j;
        for (j = 0; j < i; ++j) {
            free(strings[j]);
        }
        free(strings);
        return 0;
    }
    if (cur_str != NULL) {
        //grow if necessary
        if (i >= cur_size) {
            cur_size += 1;
            void * res = realloc(strings, sizeof(char*) * cur_size);
            if (res == NULL) {
                free(strings);
                return 0;
            }
            strings = res;
        }
        //add last read string
        strings[i++] = cur_str;
    }
    *dest = strings;
    return i;
}

char lowerCaseChar(char c) {
       if (c >= 'A' && c <= 'Z'){
           return c + ('a' - 'A');
       }
       return c;
}

size_t lowerCaseStrings(ConstStringArray strings, size_t size, StringArray* dest) {
    //check parameters
    if (size == 0 || strings == NULL || dest == NULL) {
        return 0;
    }
    size_t i;
    char** out_array = (char**)malloc(sizeof(char*) * size);
    if (out_array == NULL) {
        return 0;
    }
    for (i = 0; i < size; ++i) {
        size_t cur_size = strlen(strings[i]);
        out_array[i] = (char*)malloc(sizeof(char) * cur_size + 1);
        if (out_array[i] == NULL) {
            //free if an error has occured
            size_t j;
            for (j = 0; j < i; ++j) {
                free(out_array[j]);
            }
            free(out_array);
        }
        size_t k;
        for (k = 0; k < cur_size + 1; ++k) {
            out_array[i][k] = lowerCaseChar(strings[i][k]);
;
        }
    }
    *dest = out_array;
    return size;
}
