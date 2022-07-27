#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
 
int main( )
{
    //Read File to String dataToBeRead   
    //German Dictionary
    FILE *filePointer = fopen("res\\Groszes_Woerterbuch_Deutsch.txt", "rb");
    fseek(filePointer, 0, SEEK_END);
    unsigned long long fsize = ftell(filePointer);
    //Testerprint:
    //printf("Länge deutsches Wörterbuch: %llu\n", fsize);
    fseek(filePointer, 0, SEEK_SET);
    char *dataToBeRead = malloc (sizeof(char) * (fsize + 1));
    if (dataToBeRead == NULL){
        printf( "File (german) failed to open." ) ;
        return 0;
    }
    else
    {
        printf("The File for the german dictionary is opened.\n") ;
        fread(dataToBeRead, fsize, 1, filePointer);
        fclose(filePointer);
    }

    //English Dictionary
    FILE *filePointer2 = fopen("res\\Groszes_Woerterbuch_Englisch.txt", "rb");
    fseek(filePointer2, 0, SEEK_END);
    unsigned long long fsize2 = ftell(filePointer2);
    //Testerprint:
    //printf("Länge englisches Wörterbuch: %llu\n", fsize2);
    fseek(filePointer2, 0, SEEK_SET);
    char *dataToBeRead2 = malloc (sizeof(char) * (fsize2 + 1));
    if (dataToBeRead2 == NULL){
        printf( "File (english) failed to open." ) ;
        return 0;
    }
    else
    {
        printf("The File for the english dictionary is opened.\n") ;
        fread(dataToBeRead2, fsize2, 1, filePointer2);
        fclose(filePointer2);
    }

    char *tempword = malloc (sizeof(char) * 30);
    if (tempword == NULL){
        printf( "Oh oh, Memory Issues!\n" ) ;
        return 0;
    }
    char *tempword2 = malloc (sizeof(char) * 30);
    if (tempword2 == NULL){
        printf( "Oh oh, Memory Issues!\n" ) ;
        return 0;
    }
    char *tempsentence = malloc (sizeof(char) * 100);
    if (tempsentence == NULL){
        printf( "Oh oh, Memory Issues!\n" ) ;
        return 0;
    }
    char *tempsentence2 = malloc (sizeof(char) * 100);
    if (tempsentence2 == NULL){
        printf( "Oh oh, Memory Issues!\n" ) ;
        return 0;
    }
    strcpy(tempword, "");
    strcpy(tempword2, "");
    strcpy(tempsentence	, "");
    strcpy(tempsentence2	, "");
    bool foundword = false;
    long treffer = 0;
    long uniques = 0;
    //Parser der tatsaechlichen Daten, soll eine Zeile des englischen Wörterbuch nur erlauben wenn kein Wort darin im deutschen wörterbuch gefunden werden kann
    char *dataToBeWritten = malloc (sizeof(char) * (fsize2 + 1));
    if (dataToBeWritten == NULL){
        printf( "Oh oh, Memory Issues!\n" ) ;
        return 0;
    }
    strcpy(dataToBeWritten, "");
    char *dataToBeTrashed = malloc (sizeof(char) * (fsize2 + 1));
    if (dataToBeTrashed == NULL){
        printf( "Oh oh, Memory Issues!\n" ) ;
        return 0;
    }
    strcpy(dataToBeTrashed, "");
    for(unsigned long long i=0; i < fsize2 - 1; i++){
        //Sobald ein Wort im Eintrag im deutschen Wörterbuch gefunden wurde, wird bis zur nächsten Newline nichts mehr gemacht.
        if(foundword == true){
            if(dataToBeRead2[i] == '|'){
                //Testerprint:
                //printf("Eintrag: %swurde gelöscht.\n", tempsentence);
                strcat(dataToBeTrashed, tempsentence);
                strcpy(tempword, "");
                strcpy(tempword2, "");
                strcpy(tempsentence	, "");
                treffer++;
                foundword = false;
            }
            else{
                strncat(tempsentence, &dataToBeRead2[i], 1);
            }
        }
        //Testet ob Wort des Eintrags im deutschen Wörterbuch gefunden werden kann
        else if(dataToBeRead2[i] == ' ' || dataToBeRead2[i] == '-'){
            strcpy(tempword2, " ");
            strcat(tempword2, tempword);
            strcat(tempword2, " ");
            char* temppointer = strstr(dataToBeRead, tempword2);
            //Wenn nicht, dann wird Wort auf temporären Satz gepushed
            if(temppointer == NULL){
                strncat(tempword, &dataToBeRead2[i], 1);
                strcat(tempsentence, tempword);
                strcpy(tempword, "");
                strcpy(tempword2, "");
            }
            //Wenn doch, dann wird alles temporäres gelöscht und es wird festgehalten, dass Wort nicht einzigartig war
            else{
                strncat(tempword, &dataToBeRead2[i], 1);
                strcat(tempsentence, tempword);
                foundword = true;
            }
        }
        //Programm gelangt hier nur hin wenn kein Wort im Eintrag im deutschen Wörterbuch gefunden wurde. Daher wird Eintrag auf towrite-string gepushed.
        else if(dataToBeRead2[i] == '|'){
            //Testerprint:
            //printf("Eintrag: %swurde als sprachspezifisch eingetragen.\n", tempsentence);
            strcpy(tempsentence2, " ");
            strcat(tempsentence2, tempsentence);
            strcat(tempsentence2, "\n");
            strcat(dataToBeWritten, tempsentence2);
            strcpy(tempword, "");
            strcpy(tempword2, "");
            strcpy(tempsentence	, "");
            strcpy(tempsentence2	, "");
            uniques++;
        }
        //Liest Wort ein
        else{
            strncat(tempword, &dataToBeRead2[i], 1);
        }
    }
    printf("Es wurden insgesamt %ld Einträge gelöscht.\n", treffer);
    printf("Es wurden insgesamt %ld Einträge als einzigartig eingetragen.\n", uniques);

    //uebergibt den bearbeiteten String in ein neues Textdokument
    FILE *filePointer3 = fopen("out//Groszes_Woerterbuch_Einzigartig_fuer_Englisch.txt","w");
    fprintf(filePointer3, "%s", dataToBeWritten);
    fclose(filePointer3);
    free(dataToBeWritten);    
    //uebergibt den bearbeiteten String in ein neues Textdokument
    FILE *filePointer4 = fopen("out//Dopplungen_fuer_Woerterbuecher.txt","w");
    fprintf(filePointer4, "%s", dataToBeTrashed);
    fclose(filePointer4);
    free(dataToBeTrashed);

}