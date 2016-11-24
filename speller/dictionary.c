/**
 * Implements a dictionary's functionality.
 */

#include <cs50.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "dictionary.h"

int table_size = 700;

unsigned int size_counter = 0;

//Хеш функция.
int hash_gen(const char* string)
{
    if(string == '\0')
    {
        return 0;
    }
    //Конвертируем в верхний регистр.
    int hash = toupper(string[0]) - 'A';
    return hash % table_size;
}

//Опредиляем структуру
typedef struct node
   {
       char word[LENGTH + 1];
       struct node* next;
   }
   node;

//Опредиляем  hashtable
node* hashtable[700];
    
/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    
    //Опредиляем длину слова.
    int word_len = strlen(word);
    char check_word[LENGTH + 1];
    char check_word_l;
    
    //Проверяем и переводим в нижний регистр.
    for(int i = 0; i < word_len; i++)
    {
        check_word_l= tolower(word[i]);
        check_word[i] = check_word_l;
    }
    //Добавляем символ конец слова для перевода строки.
    check_word[word_len] = '\0';   
    
    //генерируем хеш значение.
    int word_index = hash_gen(check_word);
    
    node* cursor = hashtable[word_index];
    
    //Проверяем не равно ли  NULL хеш значение.
    if(hashtable[word_index] == NULL)
    {
        return false;
    }
   
    while(cursor != NULL)
    {   
        if(strcmp(cursor->word, check_word) == 0)
        {
            return true;
        }  
        else
        {   
            
            cursor = cursor->next;
        }             
         
    }
    return false;    
}


/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */

bool load(const char* dictionary)
{  
    for(int i = 0; i < table_size; i++)
    {
        hashtable[i] = NULL;
    }
    
    //Открываем словарь.
    FILE *d = fopen(dictionary,"r");
    //Возвращаем ошибку если словарь равен NULL. 
    if(d == NULL)
    {
        printf("Could not load %s\n",dictionary);
        return false;   
    }
    

    //Создаем буфер для fscanf     
    char buffer[LENGTH + 1];    
    
        while(fscanf(d,"%s", buffer) == 1)
        {
        node* new_node = malloc(sizeof(node));
        if(new_node == NULL)
        {
            return false;
        }
        strcpy(new_node->word, buffer);
        new_node->next = NULL;
        
        //Вичисляем хеш значение для добавленого слова.
        int index = hash_gen(new_node->word);
                
        if(hashtable[index] == NULL)
        {
            hashtable[index]= new_node;
        }
        
        else
        { 
            //Добавляем в начало связаного списка.
            new_node->next = hashtable[index];
            hashtable[index] = new_node; 
        }
        size_counter = size_counter + 1;
        }
    fclose(d);
    return true;    
}

    

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    //return the size_counter
    return size_counter;

}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    //Создаем временный указатель.
    node* temp = NULL;
    
    for(int i = 0; i <= (table_size - 1); i++)
    {
        node* curr = hashtable[i];
        
        //Проходим до конца списка.
        while(curr != NULL)
        {
            temp = curr->next;
            free(curr);
            curr = temp;
        }
    }
    return true;   
    
}