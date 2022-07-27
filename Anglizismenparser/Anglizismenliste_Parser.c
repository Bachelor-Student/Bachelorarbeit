#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
 
int main( )
{
    //Read File to String dataToBeRead   
    FILE *filePointer = fopen("Liste.txt", "rb");
    fseek(filePointer, 0, SEEK_END);
    long fsize = ftell(filePointer);
    fseek(filePointer, 0, SEEK_SET);

    char *dataToBeRead = malloc (sizeof(char) * (fsize + 1));
    if (dataToBeRead == NULL){
        printf( "File failed to open." ) ;
        return 0;
    }
    else
    {
        printf("The File is opened.") ;
        fread(dataToBeRead, fsize, 1, filePointer);
        fclose(filePointer);
    }

    //Parser der tatsaechlichen Daten
    char *dataToBeWritten = malloc (sizeof(char) * (fsize + 1));
    strcpy(dataToBeWritten, "");
    strrev(dataToBeRead);   //Da Newline nicht immer neuer Eintrag ist aber ein ':' immer ein Eintrag anzeigt muss doppelt reversed werden um mit ':' suchen zu können
    bool isaftercolon = false;
    bool startdeswortes = false; // Soll Leerzeichen und Nummerierungen trimmen die zwischen Wort und ':' zu finden sind, da dies haeufig vorkommt. Es sollen jedoch keine Leerzeichen im Eintrag an sich verschwinden.
    for(long long i=0; dataToBeRead[i] != '\0'; i++){                              //wenn : erkannt wird, dann wird bei naechster iteration uebertragen
        if(dataToBeRead[i]==':'){
            isaftercolon = true;
            startdeswortes = true;
            }                                           
        else if(isaftercolon == true && dataToBeRead[i]=='\n'){             //beendet Uebertragung bei folgender naechsten Zeile und setzt trenner (In diesem Fall nextline)
            isaftercolon = false;
            strcat(dataToBeWritten, "\n");
            }                                                 
        else if(isaftercolon == true){                        //falls immernoch uebertragen werden soll, wird dies nun hier getan
            if (startdeswortes == true && dataToBeRead[i] != ' ' && dataToBeRead[i] != '1' && dataToBeRead[i] != '2' && dataToBeRead[i] != '3' && dataToBeRead[i] != '4' && dataToBeRead[i] != '5'){
                startdeswortes = false;
                strncat(dataToBeWritten, &dataToBeRead[i], 1);
                }
            else if(startdeswortes == false){
                strncat(dataToBeWritten, &dataToBeRead[i], 1);
            }
        }        
    }
    free(dataToBeRead);
    strrev(dataToBeWritten);

    //uebergibt den bearbeiteten String in ein neues Textdokument
    FILE *filePointer2 = fopen("Liste_Parsed.txt","w");
    fprintf(filePointer2, "%s", dataToBeWritten);
    fclose(filePointer2);
    free(dataToBeWritten);
}