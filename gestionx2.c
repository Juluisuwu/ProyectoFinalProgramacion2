#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>
#include <stdbool.h>
#define STD_LEN 1024
#define MAX_LEN 60
typedef struct{
    char* isbn;
    char* title;
    char* author;
    int* stock;
    int* borrowed;
}book;
typedef struct{
    char* id;
    char* name;
    int* numLoans;
}person;
typedef struct{
    int usrs;
    int bks;
    int brwd;
    int avble;
}data;
typedef struct{
    int* cons;
    char* isbn;
    char* id;
}borrowings;
typedef enum{
    USERSDB,
    BIBLDB,
    LOANSDB,
    ISBN,
    STRING,
    TITLE,
    AUTHOR,
    NUMBER,
    ID
}dataType;

data numof = {0,0,0,0};
book* bib = NULL;
person* users = NULL;
borrowings* loans = NULL;

void* getInput(char* message,dataType type); void readDB(); void addInstance(dataType type); void updateDB(dataType tableUpdate);
int findLoan(char* usrID, char* bookID, dataType searchCrit); char* borrowBook(); int findInstance(char* key, dataType tableLookup);
char* returnBook(); void** consult(dataType, dataType,char*, size_t*); void printConsult();

int main(){
    char* opc;
    char* key;
    int aux;
    readDB();
    while(1){
        system("clear");
        printf("\nMenu Princial de Biblioteca");
        printf("a)Registro de usuario\nb)Registra un libro\nc)Prestamo de libro\nd)Devolucion de libro\ne)Busqueda de libro\nf)Buscar usuario\ng)Salir");
        opc = getInput("Registre una seleccion: ", STRING);
        switch(*opc){
            case 'a': 
                addInstance(USERSDB);
                break;
            case 'b':
                addInstance(BIBLDB);
                break;
            case 'c':
                printf("%s\n", borrowBook());
                sleep(2);
                break;  
            case 'd':
                printf("%s\n", returnBook());
                sleep(2);
                break;
            case 'e':
                key = getInput("Registra el ISBN del libro");
                if(aux = findInstance(key,BIBLDB) >= 0){
                    printf("ISBN: %s\tTITULO: %s\tAUTOR: %s", bib[aux].isbn, bib[aux].title, bib[aux].author);
                    free(getInput("Pulse cualquier tecla para volver al menu principal...",STRING));
                }
                else{ printf("No existe ese libro en nuestra base de datos\n");
                free(getInput("Pulse cualquier tecla para volver al menu principal...",STRING));
                }
                free(key);
                break;
            case 'f':
                key = getInput("Registre el ID de usuario buscado: ", ID);
                if(aux = findInstance(key,BIBLDB) >= 0){
                    printf("ID: %s\tNombre %s\t, users[aux].id, users[aux].name");
                    free(getInput("Pulse cualquier tecla para volver al menu principal...", STRING));
                }
                else{ printf("No existe ese usuario en nuestra base de datos\n");
                free(getInput("Pulse cualquier tecla para volver al menu principal...", STRING));
                }
                free(key);
                }
                break;
            case 'S':
                return 0;
            default:
                break;
            
        }
        
        
        
    }
    


    return 0;
}
void printConsult(){
    dataType tbleLookUp, criteria;
    size_t arrSize;
    char* key, *opc;
    opc = getInput("Seleccione el tipo de busqueda:\na)LIRO\tb)USUARIO, c)PRESTAMOS P/USUARIO\td)PRESTAMOS P/LIBRO\n",STRING);
    switch(*opc){
        case('A'): 
            tbleLookUp = BIBLDB;
            break;
        case('B'):
            tbleLookUp = USERSDB;
            break;
        case('C' | 'D'):
            tbleLookUp = LOANSDB;
            break; 
        default:
            printf("Seleccion no valida. Intente nuevamente.\n");
            free(opc);
            printConsult();
    }
    free(opc);
    key = getInput("Seleccione el criterio de busqueda:\na)ISBN\tb)TITULO\tc)AUTOR \td)ID DE USUARIO\n",STRING);
    switch(*opc){
        case('A'):
            criteria = ISBN;
            key = getInput("Registre ISBN: ",ISBN);
            
            break;
        case('B'):
            criteria = TITLE;
            break;
        case('C'):
            criteria = AUTHOR;
            break;
        case('D'):
            criteria = ID;
            break;
        default:
            printf("Seleccion no valida\n");
            free(opc);
            printConsult();
    }
    free(key);
    borrowings**  tmp = (borrowings**)consult(LOANSDB,ISBN,"768543846573X",&arrSize);
    printf("%zu\n",sizeof(borrowings)/sizeof(tmp));
    printf("Cons: %d\tID de usuario: %s\tISBN: %s\n",*tmp[1]->cons,tmp[1]->id,tmp[1]->isbn);
    free(key);
    return;
}
void** consult(dataType tableLookup, dataType criteria,char* key, size_t* arrSize){
    void** filter = NULL;
    int count = 0;
    char* pointer;
    size_t i;
    arrSize = NULL;
    switch(criteria){
        case(ISBN):
            for(i = 0; i < numof.bks; i++){
                    if(strcmp(key,bib[i].isbn) == 0){
                        *arrSize = *bib[i].borrowed;
                        break;
                    }
            }
        case(TITLE):

            if(arrSize == NULL) return filter;
            if(tableLookup == BIBLDB){
                *arrSize = 3;
                filter = malloc(sizeof(void*) * (*arrSize));
                filter[0] = (void*)&bib[i].isbn;
                filter[1] = (void*)&bib[i].title;
                filter[2] = (void*)&bib[i].author;
                return filter;
            }
            else if(tableLookup == LOANSDB){
                filter = malloc(sizeof(void*) * (*arrSize));
                for(i = 0; i < numof.brwd; i++){
                    if(strcmp(key,loans[i].isbn) == 0){
                        for(int j = 0; j < numof.usrs; j++){
                            if(strcmp(loans[i].id,users[j].id) == 0){
                                filter[count] = (void*)&users[j].name;
                            }
                        }
                        count ++;
                        if(count == *arrSize) break;
                    }
                }
                return filter;
            }
            else return filter;
            break;
        default:
            break;
        /*case(STRING):
            break;
        case(ID):
            break;
    };
    if(tableLookup == BIBLDB);

    if(tableLookup == USERSDB);

    if(tableLookup == LOANSDB)*/
    }
    return filter;
}
char* returnBook(){
    /*
    Author: 
    Date:
    Description:
    Param:
    Ver.:
    Name: returnBook
    Return: char*
    */
    char* idBook, *idUser;
    int tokenBook, tokenUser;
    int deleteDex;
    if(numof.brwd == 0) return "No hay registro de prestamos.";
    idBook = getInput("Registre el ISBN del libro a devolver: ",ISBN);
    if((tokenBook = findInstance(idBook,BIBLDB)) == -1) return "El libro no existe en la base de datos.";
    else if(bib[tokenBook].borrowed == 0) return "El libro no tiene prestamos actualmente.";
    idUser = getInput("Registre el ID del usuario: ",ID);
    if((tokenUser = findInstance(idUser,USERSDB)) == -1) return "El usuario no esta registrado en la base de datos.";
    else if((deleteDex =findLoan(idUser,idBook,ISBN)) == -1) return "EL usuario no cuenta con el titulo en prestamo.";
    free(loans[numof.brwd - 1].cons);free(loans[deleteDex].id); free(loans[deleteDex].isbn);
    for(size_t i = deleteDex + 1; i < numof.brwd; i++){
        loans[i - 1].id = loans[i].id; loans[i - 1].isbn = loans[i].isbn;
    }
    (numof.brwd)--;
    (numof.avble)++;
    (*bib[tokenBook].stock)++;
    (*bib[tokenBook].borrowed)--;
    (*users[tokenUser].numLoans)--;
    loans = realloc(loans,sizeof(borrowings) * numof.brwd);
    updateDB(LOANSDB);
    return "Devolucion exitosa.";
}
char* borrowBook(){
    char* isbn;
    char* id;
    int tokenBook, tokenUser;
    isbn = getInput("Registre el ISBN del libro a prestamo: ",ISBN);
    if((tokenBook = findInstance(isbn,BIBLDB)) == -1) return "El libro no existe en la base de datos.";
    else if(bib[tokenBook].stock == 0) return "El libro no tiene exisencia actualmente.";
    id = getInput("Registre el ID del usuario: ",ID);
    if((tokenUser = findInstance(id,USERSDB)) == -1) return "El usuario no esta registrado en la base de datos.";
    else if(findLoan(id,isbn,ISBN) >= 0) return "El usuario ya cuenta con el titulo en prestamo.";
    (*bib[tokenBook].stock)--;
    (*bib[tokenBook].borrowed)++;
    (*users[tokenUser].numLoans)++;
    loans = realloc(loans,sizeof(borrowings) * (numof.brwd + 1));
    loans[numof.brwd].cons = malloc(sizeof(int)); *loans[numof.brwd].cons = numof.brwd + 1;
    loans[numof.brwd].id = id;
    loans[numof.brwd].isbn = isbn;
    updateDB(LOANSDB);
    (numof.brwd)++;
    (numof.avble)--;
    return "Se realizo el prestamo de manera exitosa.";
}
void updateDB(dataType talbleUpdate){
    char* fname; int rows;
    FILE* dataBase = fopen("tmp.csv","w");
    if(talbleUpdate == BIBLDB){
        fname = "bibDB.csv";
        rows = numof.bks;
        for(int i = 0; i < rows; i++){
            fprintf(dataBase,"%s,%s,%s,%d,%d\n",bib[i].isbn,bib[i].title,bib[i].author,*bib[i].stock,*bib[i].borrowed);
        }
    }
    else if(talbleUpdate == USERSDB){
        fname = "usersDB.csv";
        rows = numof.usrs;
        for(int i = 0; i < rows; i++){
            fprintf(dataBase,"%s,%s,%d\n",users[i].id,users[i].name,*users[i].numLoans);
        }
    }
    else if(talbleUpdate == LOANSDB){
        fname = "loansDB.csv";
        rows = numof.brwd;
        //fprintf(dataBase,"CONS,USER ID,ISBN");
        //fputc('\n',dataBase);
        for(int i = 0; i < rows; i++){
            fprintf(dataBase,"%d,%s,%s\n",*loans[i].cons,loans[i].id,loans[i].isbn);
        }
    }
    else return;
    fclose(dataBase);
    remove(fname);
    rename("tmp.csv",fname);
    return;
}
int findLoan(char* usrID, char* bookID, dataType searchCrit){
    /*
    Name: findLoan
    Param: char* id
    Param: char* bookID
    Param: dataType searchCrit
    Return: int
    */
    if(numof.brwd == 0) return -1;
    for(size_t i = 0; i < numof.brwd; i++){
        if(strcmp(usrID,loans[i].id) == 0){
            if(searchCrit == ISBN){
                if(strcmp(bookID,loans[i].isbn) == 0) {
                    return i;
                }
            }
            else if(searchCrit == STRING){
                for(size_t j = 0; j < numof.bks; j++){
                    if(strcmp(bookID,bib[j].title) == 0){
                        bookID = bib[j].isbn;
                        break;
                    }
                }
                if(strcmp(bookID,loans[i].isbn) == 0) return i;
            }
        }
    }
    return -1;
}
int findInstance(char* key, dataType tableLookup){
    char** set;
    int numof_t;
    if(tableLookup == BIBLDB){
        set = malloc(sizeof(char*) * numof.bks);
        for(int i = 0; i < numof.bks; i++) set[i] = bib[i].isbn;
        numof_t = numof.bks;
    }
    else if(tableLookup == USERSDB){
        set = malloc(sizeof(char*) * numof.usrs);
        for(int i = 0; i < numof.usrs; i++) set[i] = users[i].id;
        numof_t = numof.usrs;
    }
    for(int i = 0; i < numof_t; i ++){
        if(strcmp(key, set[i]) == 0){
            free(set);
            return i;
        }
    }
    free(set);
    return -1;
}
void addInstance(dataType type){
    char* aux;
    if(type == BIBLDB){
        aux = getInput("Registre el ISBN del libro: ",ISBN);
        if(findInstance(aux,BIBLDB) >= 0){ printf("El libro ya esta registrado.\n"); return;}
        bib = realloc(bib,sizeof(book) * (numof.bks + 1));
        bib[numof.bks].isbn = aux;
        bib[numof.bks].title = getInput("Registre el titulo del libro: ",STRING);
        bib[numof.bks].author = getInput("Registre el nombre del autor del libro: ",STRING);
        bib[numof.bks].stock = getInput("Registre la existencia del libro: ",NUMBER);
        bib[numof.bks].borrowed = calloc(1,sizeof(int));
        (numof.avble) += *bib[numof.bks].stock;
        (numof.bks)++;
    }
    else if(type == USERSDB){
        aux = getInput("Registre el ID del usuario: ",ID);
        if(findInstance(aux,USERSDB) >= 0){ printf("El usuario ya esta registrado.\n"); return;}
        users = realloc(users,sizeof(person) * (numof.usrs + 1));
        users[numof.usrs].id = aux;
        users[numof.usrs].name = getInput("Registre el nombre del usuario: ",STRING);
        users[numof.usrs].numLoans = calloc(1,sizeof(int));
        (numof.usrs) ++;
    }
    else return;
}
void readDB(){
    FILE* dataBase = NULL;
    char aux[STD_LEN];
    char* word = NULL;
    int i;
    if((dataBase = fopen("bibDB.csv","r")) == NULL) system("touch bibDB.csv");
    else{
        while(fgets(aux,STD_LEN,dataBase)){
            bib = realloc(bib, sizeof(book)*(numof.bks + 1));
            bib[numof.bks].borrowed = malloc(sizeof(int)); bib[numof.bks].stock = malloc(sizeof(int));
            word = strtok(aux,","); bib[numof.bks].isbn = strdup(word);
            word = strtok(NULL,","); bib[numof.bks].title = strdup(word);
            word = strtok(NULL,","); bib[numof.bks].author = strdup(word);
            word = strtok(NULL,","); *bib[numof.bks].stock = atoi(word);
            word =  strtok(NULL,"\n"); *bib[numof.bks].borrowed = atoi(word);
            numof.avble += *bib[numof.bks].stock;
            (numof.bks)++;
        }
        fclose(dataBase);
    }
    if((dataBase = fopen("usersDB.csv","r")) == NULL) system("touch usersDB.csv");
    else{
        while(fgets(aux,STD_LEN,dataBase)){
            users = realloc(users, sizeof(person)*(numof.usrs + 1));
            users[numof.usrs].numLoans = malloc(sizeof(int));
            word = strtok(aux,","); users[numof.usrs].id = strdup(word);
            word = strtok(NULL,","); users[numof.usrs].name = strdup(word);
            word = strtok(NULL,"\n"); *users[numof.usrs].numLoans = atoi(word);
            (numof.usrs)++;
        }
        fclose(dataBase);
    }
    if((dataBase = fopen("loansDB.csv","r")) == NULL) system("touch loansDB.csv");
    else{
        while(fgets(aux,STD_LEN,dataBase)){
            loans = realloc(loans,sizeof(borrowings) * (numof.brwd + 1));
            loans[numof.brwd].cons = malloc(sizeof(int));
            word = strtok(aux,","); *loans[numof.brwd].cons = atoi(word);
            word = strtok(NULL,","); loans[numof.brwd].id = strdup(word);
            word = strtok(NULL,"\n"); loans[numof.brwd].isbn = strdup(word);
            size_t len = strlen(loans[numof.brwd].isbn);
            if(len == 11 || len == 14) loans[numof.brwd].isbn[len -1] = '\0';
            
            (numof.brwd) ++;
        }
        fclose(dataBase);
    }
    return;
}
void* getInput(char* message, dataType type){
    char aux[STD_LEN];
    char* input = NULL;
    size_t auxLen;
    while(1){
        printf("%s",message);
        scanf(" %[^\n]",aux);
        auxLen = strlen(aux);
        for(size_t i = 0; i < auxLen;i++) aux[i] = toupper(aux[i]);
        if(type == STRING){
            if(auxLen <= MAX_LEN) break;
        }
        else if(type == ISBN){
            if(auxLen == 13 && isdigit(aux[12]) || auxLen == 13 && aux[12] == 'X' || auxLen == 10){
                size_t i;
                for(i = 0; i < auxLen; i++){
                    if(!isdigit(aux[i])) break;
                }
                if((i == 10 || i > 11) && aux[0] != '0') break;
            }
        }
        else if(type == NUMBER){
            bool notdig = false;
            if(auxLen <= 3){
                for(size_t i = 0;i < auxLen;i++) if(!isdigit(aux[i])) notdig = true;
                if(!notdig){
                    int* num = malloc(sizeof(int));
                    *num = atoi(aux);
                    return num;
                }
            }
        }
        else if(type == ID){
            bool notdig = false;
            regex_t regex;
            int reti = regcomp(&regex,"^[A-Z][0-3][0-9][0-9][A-Z][0-9][A-Z]$",0);
            reti = regexec(&regex,aux,0,NULL,0);
            if(!reti) break;
        }
        printf("Los datos registrados son erroneos. Intente nuevamente.\n");
    }
    input = malloc(auxLen); strcpy(input,aux);
    return input;
}