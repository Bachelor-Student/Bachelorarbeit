#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <windows.h> //Damit ist das Programm leider auf Windows angewiesen, aber eine Universelle Alternative gibt es nicht. 
#define pathname_korpus "res\\Songkorpus\\"
#define pathname_korpus_alt "res\\Songkorpus_Lemmatisiert\\"
#define pathname_woerterbuch_liste "res\\Anglizismenwoerterbuch.txt"
#define pathname_jugendwort_liste_tiny "res\\Jahreswoerter_tiny.txt"
#define pathname_jugendwort_liste_large "res\\Jahreswoerter_large.txt"
#define pathname_manuelle_Untersuchung "res\\Manuelle_Untersuchung.txt"

//Structs
typedef struct{
    char* title;
    char* primary_author;
    char* year;
    char* text;
} Textfile;

typedef struct{
    Textfile* textfiles;
    int amount_of_textfiles;
} All_Textfiles;

typedef struct{
    char* class; // Die Tatsaechlichen Listen
    small list; // 0 soll fuer Anglizismenliste aus Anglizismenwoerterbuch, 1 fuer aktuelle Jahreswoerter_tiny und 2 fuer Jahreswoerter_large
} Anglizismenklassen;

typedef struct{
    char* anglizism;
    char* list;
    char* title;
    char* author;
    char* year;
} detected_Anglizism;

typedef struct{
    detected_Anglizism* anglizismen;
    int amount_of_anglizisms;
} All_Anglizisms;

typedef struct{
    char* anglizism;
    int counter;
} distinct_anglis;

typedef struct{
    char* category;
    distinct_anglis *anglizism;
    int malloctracker;
    int stacktracker;
    int total_tracker;
} distinct_category;

typedef struct{
    distinct_category dist_cat;
    int amount_of_distinct_anglizisms;
} All_Distinct_Anglizisms;

typedef struct{
    char* year;
    int word_count;
} All_Words_Per_Year;

typedef struct{
    All_Words_Per_Year* set;
    int anzahl;
} All_Words_Per_Year_Set;

typedef struct{
    char* author;
    int word_count;
} All_Words_Per_Author;

typedef struct{
    All_Words_Per_Author* set;
    int anzahl;
} All_Words_Per_Author_Set;

typedef struct{
    char* lemma;
    char* token;
} duality_word; 

typedef struct{
    duality_word* text;
    int length;
} duality_text; 

typedef struct{
    char* title;
    char* primary_author;
    char* year;
    duality_text text;
} Textfile_lemma_alt;

typedef struct{
    Textfile_lemma_alt* textfiles;
    int amount_of_textfiles;
} All_Textfiles_lemma_alt;

typedef struct{
    char* word;
    int word_count;
} manuelle_list;

//Vordefinierte Funktionen, fuer main()
char* filereader (char* dateiname);
Textfile filetostruct (char* dateiname);
Textfile_lemma_alt filetostruct_lemma_alt (char* dateiname, char* dateiname_unlemmtised);
char* data_to_title (char* data);
char* data_to_author (char* data);
char* data_to_year (char* data);
char* data_to_text (char* data);
duality_text data_to_text_lemma_alt (char* data);
All_Textfiles scan_all_files_and_use_filetostruct(char* path);
All_Textfiles_lemma_alt scan_all_files_and_use_filetostruct_lemma_alt(char* path, char* path_unlemmatised);
Anglizismenklassen* scan_for_anglizismenlist (char* dateiname_woerterbuch, char* dateiname_jugendwort_tiny, char* dateiname_jugendwort_large);
All_Anglizisms detect_anglizismen (All_Textfiles, Anglizismenklassen*);
All_Anglizisms detect_anglizismen_lemma_alt (All_Textfiles_lemma_alt, Anglizismenklassen*);
All_Words_Per_Year_Set count_all_words_per_year(All_Textfiles);
All_Words_Per_Author_Set count_all_words_per_author(All_Textfiles textfiles);
distinct_anglis* sort_by_anglizism(All_Anglizisms);
void sort_by_author_and_anglizism(All_Anglizisms, char*, All_Words_Per_Author_Set);
void sort_by_year_and_anglizism(All_Anglizisms, char*, All_Words_Per_Year_Set);
void sort_by_anglizismlist_and_anglizism(All_Anglizisms, char*);
void sort_by_titel_and_anglizism(All_Anglizisms, char*);
void calculate_f_score(distinct_anglis*, char*, int, int);

//Fuehrt alle anderen Funktionen in richtiger Reihenfolge aus. Classic main.
int main( )
{
    All_Textfiles alle_files_als_structs_ohne_lemma = scan_all_files_and_use_filetostruct(pathname_korpus);
    
    //Alternative: Nutze zusätzlich zweiten Pipiline Schritt, mit Lemmatisierung
    All_Textfiles_lemma_alt alle_files_als_structs_mit_lemma = scan_all_files_and_use_filetostruct_lemma_alt(pathname_korpus_alt, pathname_korpus);
    
    /*
    //Bugfixing Tools:
    for(int i = 0; i<alle_files_als_structs.amount_of_textfiles; i++){
        printf("Autor: %s\n", alle_files_als_structs.textfiles[i].primary_author);
        printf("Titel: %s\n", alle_files_als_structs.textfiles[i].title);
        printf("Jahr: %s\n", alle_files_als_structs.textfiles[i].year);
        //printf("Text: %s\n", alle_files_als_structs.textfiles[i].text);
    }
    */
    Anglizismenklassen* anglizismenlist = scan_for_anglizismenlist (pathname_woerterbuch_liste, pathname_jugendwort_liste_tiny, pathname_jugendwort_liste_large);
    /*
    //Bugfixing Tools:
        printf("Anglizismen aus Woerterbuch: %s\n", anglizismenlist[0].class);
        printf("Anglizismen aus Jahreswoerter_tiny: %s\n", anglizismenlist[1].class);
        printf("Anglizismen aus Jahreswoerter_large: %s\n", anglizismenlist[2].class);
    */

    //Alternative 1: Nutze ersten Pipeline Schritt, ohne automatische Lemmatisierung
    //All_Anglizisms detected_list = detect_anglizismen(alle_files_als_structs_ohne_lemma, anglizismenlist);

    //Alternative 2: Nutze zweiten Pipiline Schritt, ohne automatische Lemmatisierung
    All_Anglizisms detected_list = detect_anglizismen_lemma_alt (alle_files_als_structs_mit_lemma, anglizismenlist); 
    
    All_Words_Per_Year_Set all_words_in_year_X = count_all_words_per_year (alle_files_als_structs_ohne_lemma);
    All_Words_Per_Author_Set all_words_in_author_X = count_all_words_per_author (alle_files_als_structs_ohne_lemma);

    //Prints all Anglizisms
    FILE *filePointer = fopen("out\\All_Anglizisms_List.txt","w");
    if(filePointer == NULL) {
        printf("File kann nicht geöffnet werden (um Anglizismenliste roh zu schreiben)\n");
        exit(EXIT_FAILURE);
    }
    for(int i=0; i<detected_list.amount_of_anglizisms; i++){
        fprintf(filePointer, "Anglizimuseintrag Nummer %d - Anglizismus: %s\n", i+1, detected_list.anglizismen[i].anglizism);
        fprintf(filePointer, "Anglizimuseintrag Nummer %d - Autor: %s\n", i+1, detected_list.anglizismen[i].author);
        fprintf(filePointer, "Anglizimuseintrag Nummer %d - Titel: %s\n", i+1, detected_list.anglizismen[i].title);
        fprintf(filePointer, "Anglizimuseintrag Nummer %d - Jahr: %s\n", i+1, detected_list.anglizismen[i].year);
        fprintf(filePointer, "Anglizimuseintrag Nummer %d - Gefunden in Anglizismenliste: %s\n\n", i+1, detected_list.anglizismen[i].list);
    }
    fclose(filePointer);
    //Sortiert alle Eintraege nach Anglizismen und schreibt diese Direkt in File. Da nach Anglizismus sortieren fuer weitere Verarbeitung relativ nutzlos ist, wird hier nichts returned.
    distinct_anglis* anglis = sort_by_anglizism(detected_list); //CALCULIERT AUCH F-MAß!!!
    //Sortiert die Listen nach Kriterium und danach noch nach Anglizismus. Die Ergebnisse werden noch waehrend der Funktion in einer File niedergeschrieben, werden jedoch da sie bereits in Anglizismen sortiert wurden danach verworfen.
    sort_by_author_and_anglizism(detected_list, "out\\Sorted_By_Authors.txt", all_words_in_author_X);
    sort_by_year_and_anglizism(detected_list, "out\\Sorted_By_Years.txt", all_words_in_year_X);
    sort_by_anglizismlist_and_anglizism(detected_list, "out\\Sorted_By_Lists.txt");
    sort_by_titel_and_anglizism(detected_list, "out\\Sorted_By_Titel.txt");
    printf("Every Function was successfully executed! Ending Programm now!");
    return 0;
}

//filereader liest eine File mit gegebenen Filepath (inkl. filename als Attribut) ein
char* filereader (char* dateiname){
    //Standard Filereader 
    FILE *filePointer = fopen(dateiname, "rb");
    fseek(filePointer, 0, SEEK_END);
    long fsize = ftell(filePointer);
    fseek(filePointer, 0, SEEK_SET);

    char* volltext = malloc (sizeof(char) * (fsize + 1));  // Erst mal nur in Text einlesen, da der Text erst in anderen Funktionen aufbereitet in einen struct uebergeben wird.
    if (volltext == NULL){
        printf("File failed to open.");
        exit(EXIT_FAILURE);
    }
    else{
        size_t chars_read = fread(volltext, sizeof(char), fsize, filePointer);
        volltext[chars_read] = '\0'; //fread does NOT terminate a string by itself!
        fclose(filePointer);
        return(volltext);
    }
}

//filetostruct ruft zuerst filereader() auf und teilt den Text dann in year, primary_author, title und text ein.
Textfile filetostruct (char* dateiname){
/* 
Zuerst werden die genauen Informationen mit weiteren Funktionen aus dem Text extrahiert und werden dann in ein struct Textfile gegeben
NOTE: Es koennte optimiert werden, indem nicht immer wieder von vorne gesucht werden musste, sondern linear immer von der letzten gefundenen Information, da die Informationen immer
      in gleicher Reihenfolge auftauchen: title -> author with primary role -> year -> text
      Es wuerde die Laufzeit durchaus optimieren, aber dadurch dass dies nur moeglich ist, wenn das xml auch wirklich immer gleich aufgebaut ist, ist es anfaellig fuer Fehler.
      Daher wird diese Umsetzung vorerst nicht implementiert.
*/
    Textfile data;
    char* dataToBeRead = filereader (dateiname);
    data.title = data_to_title (dataToBeRead);
    data.primary_author = data_to_author(dataToBeRead);
    data.year = data_to_year(dataToBeRead);
    data.text = data_to_text(dataToBeRead);
    free(dataToBeRead);
    /*
    //Bugfixing Tools:
    printf("Autor: %s\n", data.primary_author);
    printf("Titel: %s\n", data.title);
    printf("Jahr: %s\n", data.year);
    //printf("Text: %s\n", data.text);
    */
    return data;
}

//filetostruct ruft zuerst filereader() auf und teilt den Text dann in year, primary_author, title und text ein.
Textfile_lemma_alt filetostruct_lemma_alt (char* dateiname, char* dateiname_unlemmatised){
/* 
Zuerst werden die genauen Informationen mit weiteren Funktionen aus dem Text extrahiert und werden dann in ein struct Textfile gegeben
NOTE: Es koennte optimiert werden, indem nicht immer wieder von vorne gesucht werden musste, sondern linear immer von der letzten gefundenen Information, da die Informationen immer
      in gleicher Reihenfolge auftauchen: title -> author with primary role -> year -> text
      Es wuerde die Laufzeit durchaus optimieren, aber dadurch dass dies nur moeglich ist, wenn das xml auch wirklich immer gleich aufgebaut ist, ist es anfaellig fuer Fehler.
      Daher wird diese Umsetzung vorerst nicht implementiert.
*/
    //printf("dateiname: %s\ndateiname_unlemmatised: %s\n\n", dateiname, dateiname_unlemmatised);
    Textfile_lemma_alt data;
    char* dataToBeRead = filereader (dateiname_unlemmatised);
    data.title = data_to_title (dataToBeRead);
    data.primary_author = data_to_author(dataToBeRead);
    data.year = data_to_year(dataToBeRead);
    //printf("data.title: %s\ndata.primary_author: %s\ndata.year: %s\n\n", data.title, data.primary_author, data.year);
    free(dataToBeRead);
    char* dataToBeRead_lemma_alt = filereader (dateiname);
    data.text = data_to_text_lemma_alt (dataToBeRead_lemma_alt);
    free(dataToBeRead_lemma_alt);
    /*
    //Bugfixing Tools:
    printf("Autor: %s\n", data.primary_author);
    printf("Titel: %s\n", data.title);
    printf("Jahr: %s\n", data.year);
    //printf("Text: %s\n", data.text);
    */
    return data;
}

//Sucht im Text nach <title> und </title> und gibt einen Pointer mit dem Inhalt dazwischen zurueck
char* data_to_title (char* data){
    char* pointer_for_title_beginning;
    char* pointer_for_title_ending;
    pointer_for_title_beginning = strstr(data, "<title>");
    pointer_for_title_ending = strstr(data, "</title>");
    //Ausnahmenhandler, falls nichts gefunden wird, wird struct mit standard "unknown" gefuellt
    if(pointer_for_title_beginning == NULL || pointer_for_title_ending == NULL){   
        return "unknown";
    }
    pointer_for_title_beginning += 7; // Da strstr den pointer auf den anfang des gesuchten wortes setzt, er aber am ende sein soll, muss er manuell an das ende des gesuchten wortes geschoben werden
    int length_of_title= 0;
    for(int i=0; &pointer_for_title_beginning[i] != pointer_for_title_ending; i++){
        length_of_title++;
    }
    char* title = malloc (sizeof(char) * length_of_title + 1); //+1 damit auch noch fuer den terminierenden NullChar Platz ist.
    if (title == NULL){
        printf("Titlespace! There was too little memory left to find space for title or the title doesnt exist at all! Needed space was (%d) chars", length_of_title);
        exit(EXIT_FAILURE);
    }
    else{
        strncpy(title, pointer_for_title_beginning, length_of_title);
        strcpy(&title[length_of_title], "\0");
    }
    return title;
}

//Sucht im Text nach <author role="primary"> und </author> und gibt einen Pointer mit dem Inhalt dazwischen zurueck
char* data_to_author (char* data){
    char* pointer_for_author_beginning;
    char* pointer_for_author_ending;
    pointer_for_author_beginning = strstr(data, "<author role=\"primary\">");
    pointer_for_author_ending = strstr(data, "</author>");
    //Da leider nicht immer gleich dokumentiert ist, muss sowohl auf "author role=primary", "author role= music" als auch "author" getestet werden.
    if(pointer_for_author_ending == NULL){
        return "unknown";
    }
    if(pointer_for_author_beginning != NULL){   
        pointer_for_author_beginning += 23;
    }
    else{
        pointer_for_author_beginning = strstr(data, "<author role=\"music\">");
        if(pointer_for_author_beginning != NULL){   
            pointer_for_author_beginning += 21;
        }
        else{
            pointer_for_author_beginning = strstr(data, "<author>");
            //Ausnahmenhandler, falls nichts gefunden wird, wird struct mit standard "unknown" gefuellt
            if(pointer_for_author_beginning != NULL){   
                pointer_for_author_beginning += 8;
            }
            else{
                pointer_for_author_beginning = strstr(data, "<author role=\"text\">");
                if(pointer_for_author_beginning != NULL){   
                    pointer_for_author_beginning += 20;
                }
                else{
                    return "unknown";
                }
            }
        }
    }
    int length_of_author= 0;
    for(int i=0; &pointer_for_author_beginning[i] != pointer_for_author_ending; i++){
        length_of_author++;
    }
    char* author = malloc (sizeof(char) * length_of_author + 1);
    if (author == NULL){
        printf("Authorspace! There was too little memory left to find space for author or the author doesnt exist at all! Needed space was (%d) chars", length_of_author);
        exit(EXIT_FAILURE);
    }
    else{
        strncpy(author, pointer_for_author_beginning, length_of_author);
        strcpy(&author[length_of_author], "\0");
    }
    return author;
}

//Sucht im Text nach <date> und </date> und gibt einen Pointer mit dem Inhalt dazwischen zurueck
char* data_to_year (char* data){
    char* pointer_for_year_beginning;
    char* pointer_for_year_ending;
    pointer_for_year_beginning = strstr(data, "<date>");
    pointer_for_year_ending = strstr(data, "</date>");
    //Ausnahmenhandler, falls nichts gefunden wird, wird struct mit standard "unknown" gefuellt
    if(pointer_for_year_beginning == NULL || pointer_for_year_ending == NULL){   
        return "unknown";
    }
    pointer_for_year_beginning += 6; 
    int length_of_year= 0;
    for(int i=0; &pointer_for_year_beginning[i] != pointer_for_year_ending; i++){
        length_of_year++;
    }
    char* year = malloc (sizeof(char) * length_of_year + 1);
    if (year == NULL){
        printf("Yearspace! There was too little memory left to find space for year!");
        exit(EXIT_FAILURE);
    }
    else{
        strncpy(year, pointer_for_year_beginning, length_of_year);
        strcpy(&year[length_of_year], "\0");
    }
    return year;
}

//Sucht im Text nach <text> und </text> und gibt einen Pointer mit dem Inhalt dazwischen zurueck.
char* data_to_text (char* data){
/*
Im Text sind jedoch viele Markierungen wie <body> / <\body> oder <l> / <\l> und so weiter. Waeherend diese Markierungenhilfreich sind um newlines oder Punktierungen anzuzeigen,
so sind sie fuer die Suche nach Anglizismen ziemlich nutzlos. Daher wird hier alles zwischen <...> und <\...> anschließend rausgefiltert.
*/
    char* pointer_for_text_beginning;
    char* pointer_for_text_ending;
    pointer_for_text_beginning = strstr(data, "<text>");
    pointer_for_text_ending = strstr(data, "</text>");
    //Ausnahmenhandler, falls nichts gefunden wird, wird struct mit standard "unknown" gefuellt
    if(pointer_for_text_beginning == NULL || pointer_for_text_ending == NULL){   
        return "unknown";
    }
    pointer_for_text_beginning += 6; 
    int length_of_text= 0;
    for(int i=0; &pointer_for_text_beginning[i] != pointer_for_text_ending; i++){
        length_of_text++;
    }
    char* text = malloc (sizeof(char) * length_of_text + 1);
    if (text == NULL){
        printf("Textspace! There was too little memory left to find space for text!");
        exit(EXIT_FAILURE);
    }
    else{
        bool needed = true; //Loescht Markierungen im XML, die fuer das Programm irrelevant sind
        strcpy(text, "");
        for(int i=0; &pointer_for_text_beginning[i] != pointer_for_text_ending; i++){
            if(needed && pointer_for_text_beginning[i] == '<'){
                needed = false;
            }
            else if(needed){
                    strncat(text, &pointer_for_text_beginning[i], 1);
            }
            else if(!needed && pointer_for_text_beginning[i] == '>'){
                needed = true;
            }
            
        }
    }
    strcat(text, "\0");
    return text;
}

//Sucht im Text nach <text> und </text> und gibt einen Pointer mit dem Inhalt dazwischen zurueck.
duality_text data_to_text_lemma_alt (char* data){
/*
Im Text sind jedoch viele Markierungen wie <body> / <\body> oder <l> / <\l> und so weiter. Waeherend diese Markierungenhilfreich sind um newlines oder Punktierungen anzuzeigen,
so sind sie fuer die Suche nach Anglizismen ziemlich nutzlos. Daher wird hier alles zwischen <...> und <\...> anschließend rausgefiltert.
*/
    char* pointer_for_lemma = strstr(data, "<tc:lemmas");    //Fettes Brot hat ein Url nach lemmas, daher kein generelles <tc:lemmas>
    char* pointer_for_token = strstr(data, "<tc:tokens");
    int lenght_lemma = 0;
    int lenght_token = 0;
    int stacktracker = 0;
    unsigned long long malloctracker = 10000;
    duality_word* text = malloc (sizeof(duality_word) * malloctracker);
    //Ausnahmenhandler, falls nichts gefunden wird oder speicherplatz nicht ausreicht, wird struct mit standard "unknown" gefuellt
    if(pointer_for_lemma == NULL|| text == NULL || pointer_for_token == NULL){   
        printf("Fehler in data_to_text_lemma_alt! Start hat schon nicht funktioniert.");
        exit(EXIT_FAILURE);
    }
    //Sucht nach jedem Lemma und Token mit Hilfe einheitlicher Markierung im XML
    while(strstr(pointer_for_lemma, "<tc:lemma ID=") != NULL && strstr(pointer_for_token, "<tc:token ID=") != NULL){
        pointer_for_lemma = strstr(pointer_for_lemma, "<tc:lemma ID=");
        pointer_for_lemma = strstr(pointer_for_lemma, ">");
        pointer_for_token = strstr(pointer_for_token, "<tc:token ID=");
        pointer_for_token = strstr(pointer_for_token, ">");
        if(pointer_for_lemma == NULL || pointer_for_token == NULL){
            printf("Fehler in data_to_text_lemma_alt! Token oder Lemma konnte nicht gefunden werden");
            exit(EXIT_FAILURE);
        }
        //Macht Platz für den nächsten structeintrag wenn benötigt
        stacktracker++;
        if(stacktracker >= malloctracker){
            malloctracker *= 2;
            text = realloc (text, sizeof(duality_word) * malloctracker);
            if (text == NULL){
                printf("Fehler beim Reallocaten von Speicherplatz in data_to_text_lemma_alt. Speicherplatz konnte nicht erweitert werden!");
                exit(EXIT_FAILURE);
            }
        }
        //Testet bevor das Lemma eingelesen werden kann, wie lang es ist um genügend speicherplatz zu mallozieren.
        for(int i = 1; pointer_for_lemma[i] != '<'; i++){
            lenght_lemma++;
        }
        text[stacktracker-1].lemma = malloc (sizeof(char) * (lenght_lemma + 1));
        strcpy(text[stacktracker-1].lemma, "");
        //Liest Lemma ein (beginnt bei 1, weil pointer noch auf dem > liegt)
        for(int i = 1; pointer_for_lemma[i] != '<'; i++){
            if(pointer_for_lemma[i] != ' '){
                strncat(text[stacktracker-1].lemma, &pointer_for_lemma[i], 1);
            }
        }
        strcat(text[stacktracker-1].lemma, "\0");
        //printf("text[stacktracker-1].lemma: %s\n", text[stacktracker-1].lemma);

        //Testet bevor das Token eingelesen werden kann, wie lang es ist um genügend speicherplatz zu mallozieren.
        for(int i = 1; pointer_for_token[i] != '<'; i++){
            lenght_token++;
        }
        text[stacktracker-1].token = malloc (sizeof(char) * (lenght_token + 1));
        strcpy(text[stacktracker-1].token, "");
        //Liest Token ein (beginnt bei 1, weil pointer noch auf dem > liegt)
        for(int i = 1; pointer_for_token[i] != '<'; i++){
            if(pointer_for_token[i] != ' '){
                strncat(text[stacktracker-1].token, &pointer_for_token[i], 1);
            }
        }
        //Wenn Lemma bei automatischer Bestimmung unbekannt war, wurde es mit "&lt;unknown>" markiert. In dem Falle soll das Standard Token benutzt werden.
        if(strcmp(text[stacktracker-1].lemma, "&lt;unknown>") == 0){
            text[stacktracker-1].lemma = realloc (text[stacktracker-1].lemma, sizeof(char) * (lenght_token + 1));
            strcpy(text[stacktracker-1].lemma, text[stacktracker-1].token);
        }
        //printf("An Stelle %d ist Token: %s und Lemma: %s\n", (stacktracker-1), text[stacktracker-1].token, text[stacktracker-1].lemma);
        lenght_lemma = 0;
        lenght_token = 0;

    }
    duality_text text_komplett;
    text_komplett.length = stacktracker;
    text_komplett.text = text;
    return (text_komplett);
}

//Scannt alle Dateien mit Hilfe von filetostruct einzeln und ueberfuehrt jedes in ein Struct. Die Gesammtheit aller structs werden als Textfile Pointer zusammengefasst und returned.
All_Textfiles scan_all_files_and_use_filetostruct(char* path){
/*
scan_all_files_and_use_filetostruct scant zuerst nach allen Dateien in Sognkorpus die er finden kann. Der Ordner "Songkorpus" muss dabei in der selben Directionary liegen wie die Detector.exe Datei.
Alle gefundenen Dateien werden Ordnerdateien sein, in denen die jeweiligen .xml Dateien zu finden sein werden. Ich entscheide mich dabei bewusst gegen einen Kompletten Scan nach 
subdirectionaries. Denn sich die Moeglichkeit offen zu halten fuer die Kategorien auch unterkategorien via neuer Ordner zu erstellen, ist nicht besonders sinnvoll, da die Ketegorie im struct
nach dem zu letzt gefundenen Ordner bennant wird. Eine Implementierung fuer structs auch Oberkategorien anzugeben zu koennen, koennte dies umegehen, da man sonst den kompletten Pathname speichern muesste,
aber da der Korpus keine Unterkategorien aufweist, wird dies nicht implementiert.
*/
    int malloctracker = 500; //Startwert (Da das Programm nicht wissen kann wie viel Platz fuer die Structs gemacht werden muss, muss mit realloc gearbeitet werden. Startwert sinnvoll waehlen!
    int stacktracker = 0;    //Um die Anzahl an Realloc so selten wie moeglich zu nutzen, wird malloctracker wenn noetig (indiziert durch Stacktracker) mal 2 gerechnet)
    Textfile* stack_of_Textfiles = malloc (sizeof(Textfile) * malloctracker);
    if (stack_of_Textfiles == NULL) exit(EXIT_FAILURE);;
    HANDLE fileHandle = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA fileData;
    char masterpath [MAX_PATH];
    strcpy(masterpath, path);
    strcat(masterpath, "*");
    fileHandle = FindFirstFile(masterpath, &fileData);    //Findet die erste File in der Directionary
    if (fileHandle == INVALID_HANDLE_VALUE){    
        printf("Fehler bei Suche nach Dateien (Ordnern). FileHandler hat Wert NULL");
        exit(EXIT_FAILURE);;
    }
    do {    //Da im Sognkorpus nur Ordner sein duerfen, wird erst in dieser Do{} nach den XML in den Ordnern gesucht
        if(strchr(fileData.cFileName, '.') == NULL){ //Es werden im Songkorpus aus unbekannten Gruenden "." und ".." gefunden, daher werden sie rausgefiltert, und da Dateienordner keine "." beinhalten duerfen ist dieser Filter sicher
            HANDLE fileHandle2 = INVALID_HANDLE_VALUE;
            WIN32_FIND_DATA fileData2;
            char subpath [MAX_PATH];
            strcpy(subpath, path);
            strcat(subpath, fileData.cFileName);
            strcat(subpath, "\\*.xml");
            fileHandle2 = FindFirstFile(subpath, &fileData2);   //Hier werden die XML dateien in den jeweiligen Ordnern gefunden
            if (fileHandle2 == INVALID_HANDLE_VALUE){    
                printf("Fehler bei Suche nach Dateien (XML-Dateien). FileHandler hat Wert NULL. Fehler wurde gefunden in Ordner %s", masterpath);
                exit(EXIT_FAILURE);
            }
            do { 
                //Hier wird zuerst gecheckt ob das gefundene XML ueberhaupt in ein struct umgewandelt werden kann, da evtl kein Platz im Textfile pointerarray ist. 
                stacktracker++;
                if(stacktracker > malloctracker){
                    malloctracker *= 2;
                    stack_of_Textfiles = realloc(stack_of_Textfiles, sizeof(Textfile) * malloctracker);
                    if (stack_of_Textfiles == NULL){
                        printf("Fehler beim Reallocaten von Speicherplatz in scan_all_files_and_use_filetostruct. Speicherplatz konnte nicht erweitert werden!");
                        exit(EXIT_FAILURE);
                    }
                }
                //Danach wird via filetostruct das xml in ein struct ueberfuehrt. Zuerst muss das aktuelle Subpath jedoch mit xml.Datei-Name gemerged werden.
                char fullpath [MAX_PATH];
                strcpy(fullpath, path);
                strcat(fullpath, fileData.cFileName);
                strcat(fullpath, "\\");
                strcat(fullpath, fileData2.cFileName);
                stack_of_Textfiles[(stacktracker-1)] = filetostruct (fullpath); //Stacktracker -1, da der Stacktracker zwar bei 0 beginnt aber zum realloc check vorher schon erhoeht werden muss.
            }
            while (FindNextFile(fileHandle2, &fileData2) != 0);
                FindClose(fileHandle2);
        }
    }
    while (FindNextFile(fileHandle, &fileData) != 0);
    FindClose(fileHandle);
    
    //Bevor der stack_of_textfiles returned wird, lohnt es sich unnoetigen Platz im Pointerarray loszuwerden durch realloc, da durch konstantes multiplizieren des Platzes sich evtl ein großer ungenutzter Speicherplatz ansammelt. Bei besonders großem Korpus definitv die Memoryumplatzierung wert.
    stack_of_Textfiles = realloc(stack_of_Textfiles, sizeof(Textfile) * stacktracker);
    if (stack_of_Textfiles == NULL){
        printf("Fehler beim Reallocaten von Speicherplatz in scan_all_files_and_use_filetostruct. Speicherplatz konnte nicht reduziert werden!");
        exit(EXIT_FAILURE);
    }
    //Ueberfuehrung in Struct, da die Laenge von Structs im Nachhinein sonst nicht garantiert ermittelt werden kann.
    else{
        All_Textfiles all_files;
        all_files.textfiles = stack_of_Textfiles;
        all_files.amount_of_textfiles = stacktracker;
        return all_files;
    }
}

//Scannt alle Dateien mit Hilfe von filetostruct einzeln und ueberfuehrt jedes in ein Struct. Die Gesammtheit aller structs werden als Textfile Pointer zusammengefasst und returned.
All_Textfiles_lemma_alt scan_all_files_and_use_filetostruct_lemma_alt(char* path, char* path_unlemmatised){
/*
scan_all_files_and_use_filetostruct scant zuerst nach allen Dateien in Sognkorpus die er finden kann. Der Ordner "Songkorpus" muss dabei in der selben Directionary liegen wie die Detector.exe Datei.
Alle gefundenen Dateien werden Ordnerdateien sein, in denen die jeweiligen .xml Dateien zu finden sein werden. Ich entscheide mich dabei bewusst gegen einen Kompletten Scan nach 
subdirectionaries. Denn sich die Moeglichkeit offen zu halten fuer die Kategorien auch unterkategorien via neuer Ordner zu erstellen, ist nicht besonders sinnvoll, da die Ketegorie im struct
nach dem zu letzt gefundenen Ordner bennant wird. Eine Implementierung fuer structs auch Oberkategorien anzugeben zu koennen, koennte dies umegehen, da man sonst den kompletten Pathname speichern muesste,
aber da der Korpus keine Unterkategorien aufweist, wird dies nicht implementiert.
*/
    int malloctracker = 500; //Startwert (Da das Programm nicht wissen kann wie viel Platz fuer die Structs gemacht werden muss, muss mit realloc gearbeitet werden. Startwert sinnvoll waehlen!
    int stacktracker = 0;    //Um die Anzahl an Realloc so selten wie moeglich zu nutzen, wird malloctracker wenn noetig (indiziert durch Stacktracker) mal 2 gerechnet)
    Textfile_lemma_alt* stack_of_Textfiles = malloc (sizeof(Textfile_lemma_alt) * malloctracker);
    if (stack_of_Textfiles == NULL) exit(EXIT_FAILURE);;
    HANDLE fileHandle = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA fileData;
    char masterpath [MAX_PATH];
    strcpy(masterpath, path);
    strcat(masterpath, "*");
    fileHandle = FindFirstFile(masterpath, &fileData);    //Findet die erste File in der Directionary
    if (fileHandle == INVALID_HANDLE_VALUE){    
        printf("Fehler bei Suche nach Dateien (Ordnern). FileHandler hat Wert NULL");
        exit(EXIT_FAILURE);;
    }
    do {    //Da im Sognkorpus nur Ordner sein duerfen, wird erst in dieser Do{} nach den XML in den Ordnern gesucht
        if(strchr(fileData.cFileName, '.') == NULL){ //Es werden im Songkorpus aus unbekannten Gruenden "." und ".." gefunden, daher werden sie rausgefiltert, und da Dateienordner keine "." beinhalten duerfen ist dieser Filter sicher
            HANDLE fileHandle2 = INVALID_HANDLE_VALUE;
            WIN32_FIND_DATA fileData2;
            char subpath [MAX_PATH];
            strcpy(subpath, path);
            strcat(subpath, fileData.cFileName);
            strcat(subpath, "\\*.xml");
            fileHandle2 = FindFirstFile(subpath, &fileData2);   //Hier werden die XML dateien in den jeweiligen Ordnern gefunden
            if (fileHandle2 == INVALID_HANDLE_VALUE){    
                printf("Fehler bei Suche nach Dateien (XML-Dateien). FileHandler hat Wert NULL. Fehler wurde gefunden in Ordner %s", masterpath);
                exit(EXIT_FAILURE);
            }
            do { 
                //Hier wird zuerst gecheckt ob das gefundene XML ueberhaupt in ein struct umgewandelt werden kann, da evtl kein Platz im Textfile pointerarray ist. 
                stacktracker++;
                if(stacktracker > malloctracker){
                    malloctracker *= 2;
                    stack_of_Textfiles = realloc(stack_of_Textfiles, sizeof(Textfile_lemma_alt) * malloctracker);
                    if (stack_of_Textfiles == NULL){
                        printf("Fehler beim Reallocaten von Speicherplatz in scan_all_files_and_use_filetostruct. Speicherplatz konnte nicht erweitert werden!");
                        exit(EXIT_FAILURE);
                    }
                }
                //Danach wird via filetostruct das xml in ein struct ueberfuehrt. Zuerst muss das aktuelle Subpath jedoch mit xml.Datei-Name gemerged werden.
                char fullpath [MAX_PATH];
                strcpy(fullpath, path);
                strcat(fullpath, fileData.cFileName);
                strcat(fullpath, "\\");
                strcat(fullpath, fileData2.cFileName);
                //Fullpath2 ist nötig für Metadaten
                char fullpath_unlemmatised [MAX_PATH];
                strcpy(fullpath_unlemmatised, path_unlemmatised);
                strcat(fullpath_unlemmatised, fileData.cFileName);
                strcat(fullpath_unlemmatised, "\\");
                strcat(fullpath_unlemmatised, fileData2.cFileName);
                stack_of_Textfiles[(stacktracker-1)] = filetostruct_lemma_alt (fullpath, fullpath_unlemmatised); //Stacktracker -1, da der Stacktracker zwar bei 0 beginnt aber zum realloc check vorher schon erhoeht werden muss.
            }
            while (FindNextFile(fileHandle2, &fileData2) != 0);
                FindClose(fileHandle2);
        }
    }
    while (FindNextFile(fileHandle, &fileData) != 0);
    FindClose(fileHandle);
    
    //Bevor der stack_of_textfiles returned wird, lohnt es sich unnoetigen Platz im Pointerarray loszuwerden durch realloc, da durch konstantes multiplizieren des Platzes sich evtl ein großer ungenutzter Speicherplatz ansammelt. Bei besonders großem Korpus definitv die Memoryumplatzierung wert.
    stack_of_Textfiles = realloc(stack_of_Textfiles, sizeof(Textfile_lemma_alt) * stacktracker);
    if (stack_of_Textfiles == NULL){
        printf("Fehler beim Reallocaten von Speicherplatz in scan_all_files_and_use_filetostruct. Speicherplatz konnte nicht reduziert werden!");
        exit(EXIT_FAILURE);
    }
    //Ueberfuehrung in Struct, da die Laenge von Structs im Nachhinein sonst nicht garantiert ermittelt werden kann.
    else{
        All_Textfiles_lemma_alt all_files;
        all_files.textfiles = stack_of_Textfiles;
        all_files.amount_of_textfiles = stacktracker;
        return all_files;
    }
}

//Liest mit filereader beide Anglizismenlisten ein und weißt ihnen zu, dass die eine Liste aus dem Woerterbuch kommt und die andere von Jugendwoertern stammt.
Anglizismenklassen* scan_for_anglizismenlist (char* dateiname_woerterbuch, char* dateiname_jugendwort_tiny, char* dateiname_jugendwort_large){
    /* 
    Die Anglizismenliste sollte urspruenglich in dieser Funktion in ihre Anfangsbuchstaben aufgeteilt werden. Somit sollte ein "char** liste" pointer
    auf die Ansammlungen von "char* anglizismen_liste" zeigen, wobei jede "anglizismen_liste" eine Gruppe seines eigenen Anfangsbuchstabens ist.
    Dies wurde jedoch verworfen, da strstr sehr optimiert vorangeht, und es keiner Listenkuerzung bedarf um strstr schneller zu machen.
    Beim Test wie schnell das letzte Wort in der Liste "Zoomen" gefunden werden kann, war strstr so schnell, dass keine Zeit groeßer 0 gemessen werden konnte.
    Beweis des Testcodes:
    //has to include: #include <time.h>
    char* raw_list = filereader(dateiname);
    clock_t start = clock();
    char* test = strstr (raw_list, "zoomen");
    clock_t end = clock();
    float seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf ("Your calculations took %lf seconds to run.\n", seconds );
    //Output: Your calculations took 0.000000 seconds to run.
    Daher kann davon ausgegangen werden, dass trotz immens haeufigen Benutzens von strstr dieses bereits genuegend optimiert ist. Es wird einfach mit char* gearbeitet.
    Strstr dauert natuerlich immer laenger, desto laenger die Anglizismenliste ist, in der gesucht wird. Wuerde die Liste bedeutend laenger werden, waere ein alphabetischer Ansatz durchaus nuetzlich.
    */
    Anglizismenklassen* anglizismen_liste = malloc (sizeof(Anglizismenklassen) * 3);
    anglizismen_liste[0].class = filereader(dateiname_woerterbuch);
    anglizismen_liste[0].list = 0;
    anglizismen_liste[1].class = filereader(dateiname_jugendwort_tiny);
    anglizismen_liste[1].list = 1;
    anglizismen_liste[2].class = filereader(dateiname_jugendwort_large);
    anglizismen_liste[2].list = 2;
    return anglizismen_liste;
}

//Sucht nach Anglizismen im Korpus mit Hilfe von Anglizismenlisten
All_Anglizisms detect_anglizismen (All_Textfiles textfiles, Anglizismenklassen* angliszismen){
    /*
    Wichtig fuer diese Funktion ist es zu wissen, dass die Anglizismenliste keine Lemmata in sich haelt. Diese waren leider nicht digital vorliegend
    Da nach Woertern gesucht wird und nicht Lemmata ist klar, dass der Text Wort fuer Wort gescannt wird ob er Teil der Liste ist. 
    Da Die Anzahl an Woertern im Text oft kleiner ist als die Anzahl an Anglizismen in der Liste ist dies auch effizienter als jedes Wort der Liste im Text abzusuchen.
    Aber Eintraege der Anglizismenliste im Text zu suchen ist nicht sinnvolll, wenn es keine Lemmata sind.
    ---
    Zudem ist es wichtig, dass folgendes Problem bewusst ist: Wie soll man Anglizismen Kategorisieren die laenger als ein wort sind?
    Beispielsweise "Best of" waere eigentlich ein Eintrag, koennte allerdings auch als zwei Eintraege eingespeichert werden.
    Denn sowohl "best" als auch "of" sind auch Eintraege in der Liste, jedoch ergeben sie im Zusammenhang einen neuen Eintrag, daher wird
    das Programm den laengst moeglichen Eintrag als einzelnen Eintrag zaehlen.
    ---
    Zudem muss das Proggramm unterschieden zwischen Splittern und Trimmern. Denn waehrend das Programm Wort fuer Wort nach der laengesten Kette suchen soll (splitte je Wort am " " - Leerzeichen),
    so muss eine Kette gefolgt von Satzzeichen als zwangsweise beendet angesehen werden. Denn in "Er war einfach the Best! Of course, geb ich dir da recht, aber..."
    koennten neben "the Best" und "of course" auch "the Best of" ein potentieller Eintrag sein, der hier aber definitiv nicht gefunden werden soll, da es ja durch Satzzeichen getrennt ist.
    Zudem ist es wichtig Satzzeichen zu ignorieren beim Suchen. Denn in "Er ist the Best!" soll ja nicht nach "the Best!" sondern "the Best" gesucht werden.
    Dabei soll die Filterung so grob wie moeglich (also nur bei Satzzeichen) bleiben, da Apostrophe, Bindestriche, Zahlen (zb. "2cool4u"), etc. auch potentieller Bestandteil von Eintraegen sein koennen.
    Die Filterung hat bereits in der Korpusaufbereitung stattgefunden und hat verwerfbare Satzzeichen wie "." mit <add>.</add> erkannt und durch ein spaeter nutzbares eindeutiges Zeichen ersetzt
    Somit kann weiterhin nach bspw. "K.O." gesucht werden, da diese nicht durch <add/> gekennzeichnet werden wuerden. Eindeutiges Zeichen ist: "%", da dieses Zeichen augeschrieben werden wuerde in einem Songkorpus, daher ist es einzigartig fuer diesen Zweck.  
    */
    int malloctracker = 500; 
    int stacktracker = 0;
    bool gerade_schon_leertaste = false; //sorgt dafuer, dass viele hintereinander-folgende Leerzeichen im Text nicht immer wieder den Codepart "if(text[j] == '\n' || text[j] == ' ')" aufrufen. Es wuerde zwar auch ohne funktionieren, es muesste dann aber unnoetig oft die in der if-Condtion abgefragten Checks durchfuehren.
    detected_Anglizism* angliz = malloc (sizeof(detected_Anglizism) * malloctracker);
    char* temp_word = malloc (sizeof(char) * 100); //Dieser Zeiger wird immer wieder verwendet um Woerter zu finden, daher schadet es nicht ihn etwas groeßer zu machen als noetig.
    char* temp_word_chain = malloc (sizeof(char) * 300); //Wird auch immer wieder verwendet, soll jedoch fuer Eintraege, die laenger als ein Wort sind herhalten.
    char* temp_word_chain_tester = malloc (sizeof(char) * 300); //Wird auch immer wieder verwertet, kann aber beliebig zum Testen resettet werden
    strcpy(temp_word, "");
    strcpy(temp_word_chain, "");
    strcpy(temp_word_chain_tester, "");
    small which_list = 0; // 0 soll fuer Anglizismenliste aus Woerterbuch, 1 fuer aktuelle Anglizismen in tiy scope und 2 fuer Anglizismen in large scope
    for(int i=0; i < textfiles.amount_of_textfiles; i++){
        char* text = textfiles.textfiles[i].text;
        for(int j=0; text[j] != '\0';j++){
            //Klassische Wortsplitter mit " " und "\n". In diesem Modus kann Kette gebildet werden!
            // -> wenn ein neues Wort beginnt wird tempword erst einmal in die chain gegeben um zu sehen ob es noch einen Eintrag damit findet.
            //Fall 1 -> Wenn ja, dann wird Wort an Kette gehaengt.
            //Fall 2 -> Wenn nein, dann wird Kette auf eintrag gepushed und neues Wort wird in Kette geladen (wenn es ueberhaupt ein Eintrag ist) und neues Wort wird geleert
            //(Fall 3: das neue Wort ist kein Anglizismus und Kette wird ohne Wort gepushed)
            if((text[j] == '\n' || text[j] == ' ') && gerade_schon_leertaste == false){ 
                gerade_schon_leertaste = true;
                strcat(temp_word_chain_tester, " "); //Das inkludiert auch die Moeglichkeit, dass tester leer ist und mit " " startet. Das ist gewollt, da somit nur nach ganzen Woertern gesucht wird und nicht "im" in "immersive" gefunden werden kann
                strcat(temp_word_chain_tester, temp_word);
                strcat(temp_word_chain_tester, " ");
                strcat(temp_word_chain_tester, "\0");
                // Fall 1 fuer anglizismenliste_woerterbuch - Wort wird an Kette gehaengt und findet Eintrag im Woerterbuch
                if(strstr(angliszismen[0].class, temp_word_chain_tester) != NULL){
                    if(strcmp(temp_word_chain, "") != 0) strcat(temp_word_chain, " ");
                    strcat(temp_word_chain, temp_word);
                    strcat(temp_word_chain, "\0");
                    strcpy(temp_word_chain_tester, " ");
                    strcat(temp_word_chain_tester, temp_word_chain);
                    strcpy(temp_word, "");
                    which_list = 0;
                }
                else{
                    // Fall 1 fuer anglizismenliste_Jugendwoerter_tiny - Wort wird an Kette gehaengt und findet Eintrag in Jugendwoertern
                    if(strstr(angliszismen[1].class, temp_word_chain_tester) != NULL){
                        if(strcmp(temp_word_chain, "") != 0) strcat(temp_word_chain, " ");
                        strcat(temp_word_chain, temp_word);
                        strcat(temp_word_chain, "\0");
                        strcpy(temp_word_chain_tester, " ");
                        strcat(temp_word_chain_tester, temp_word_chain);
                        strcpy(temp_word, "");
                        which_list = 1;
                    }
                    else{
                        // Fall 1 fuer anglizismenliste_Jugendwoerter_large - Wort wird an Kette gehaengt und findet Eintrag in Jugendwoertern
                        if(strstr(angliszismen[2].class, temp_word_chain_tester) != NULL){
                            if(strcmp(temp_word_chain, "") != 0) strcat(temp_word_chain, " ");
                            strcat(temp_word_chain, temp_word);
                            strcat(temp_word_chain, "\0");
                            strcpy(temp_word_chain_tester, " ");
                            strcat(temp_word_chain_tester, temp_word_chain);
                            strcpy(temp_word, "");
                            which_list = 2;
                        }
                        // Fall 2 im Generellen (Kette auf Eintrag pushen) - Wort kann nicht an Kette angehaengt werden und muss einzeln verarbeitet werden
                        else{
                            if(strcmp(temp_word_chain, "") != 0){ //Eine leere kette entsteht sobald zwei Nicht anglizismen aufeinanderfolgen.
                                //Zuerst pruefen ob genuegend Platz fuer weiteren Eintrag ist und wenn nicht dann reallocaten!
                                stacktracker++;
                                if(stacktracker > malloctracker){
                                    malloctracker *= 2;
                                    angliz = realloc(angliz, sizeof(detected_Anglizism) * malloctracker);
                                    if (angliz == NULL){
                                        printf("Fehler beim Reallocaten von Speicherplatz in detect_anglizismen. Speicherplatz konnte nicht erhoeht werden!");
                                        exit(EXIT_FAILURE);
                                    }
                                }
                                //Kette auf Eintrag pushen
                                int length_of_entry = strlen(temp_word_chain) + 1;
                                angliz[(stacktracker-1)].anglizism = malloc (sizeof(char) * length_of_entry);
                                strncpy(angliz[(stacktracker-1)].anglizism, temp_word_chain, length_of_entry);
                                angliz[(stacktracker-1)].author = textfiles.textfiles[i].primary_author;
                                angliz[(stacktracker-1)].title = textfiles.textfiles[i].title;
                                angliz[(stacktracker-1)].year = textfiles.textfiles[i].year;
                                if ( which_list == 0){
                                    angliz[(stacktracker-1)].list = "Wörterbuch";
                                }
                                else if ( which_list == 1){
                                    angliz[(stacktracker-1)].list = "Jahreswörter_tiny";
                                }
                                else if ( which_list == 2){
                                    angliz[(stacktracker-1)].list = "Jahreswörter_large";
                                }
                                else{
                                    printf("FEHLER! which_list hat einen eigentlich unmöglichen Wert (Wert: %d) angenommen! Das Programm beendet sich nun!", which_list);
                                    exit(EXIT_FAILURE);
                                }
                            }
                            // Fall 2 fuer anglizismenliste_woerterbuch - Wort ist nicht Teil der Kette kann aber alleinstehend in Woerterbuch gefunden werden
                            strcpy(temp_word_chain_tester, " ");
                            strcat(temp_word_chain_tester, temp_word);
                            strcat(temp_word_chain_tester, " ");
                            strcat(temp_word_chain_tester, "\0");
                            if(strstr(angliszismen[0].class, temp_word_chain_tester) != NULL){
                                strcpy(temp_word_chain, temp_word);
                                strcat(temp_word_chain, "\0");
                                strcpy(temp_word_chain_tester, " ");
                                strcat(temp_word_chain_tester, temp_word);
                                strcpy(temp_word, "");
                                which_list = 0;
                            }
                            else{
                                // Fall 2 fuer anglizismenliste_Jugendwoerter_tiny - Wort ist nicht Teil der Kette kann aber alleinstehend in Jugendwoertern gefunden werden
                                if(strstr(angliszismen[1].class, temp_word_chain_tester) != NULL){
                                    strcpy(temp_word_chain, temp_word);
                                    strcat(temp_word_chain, "\0");
                                    strcpy(temp_word_chain_tester, " ");
                                    strcat(temp_word_chain_tester, temp_word);
                                    strcpy(temp_word, "");
                                    which_list = 1;
                                }
                                else{
                                    // Fall 2 fuer anglizismenliste_Jugendwoerter - Wort ist nicht Teil der Kette kann aber alleinstehend in Jugendwoertern gefunden werden
                                    if(strstr(angliszismen[1].class, temp_word_chain_tester) != NULL){
                                        strcpy(temp_word_chain, temp_word);
                                        strcat(temp_word_chain, "\0");
                                        strcpy(temp_word_chain_tester, " ");
                                        strcat(temp_word_chain_tester, temp_word);
                                        strcpy(temp_word, "");
                                        which_list = 2;
                                    }
                                    //Fall 3 - Wort kann weder mit Kette noch alleinstehend als Anglizismus gefunden werden. Wort wird verworfen (Kette ist bereits gepushed)
                                    else{
                                    strcpy(temp_word_chain, "");
                                    strcpy(temp_word_chain_tester, "");
                                    strcpy(temp_word, "");
                                    }
                                }
                            }
                        }
                    }
                }
            }
            //Eigener Fall fuer aufeinanderfolgende Leerzeichen, indem nichts geschehen soll. Nur fuer Else wichtig!
            else if((text[j] == '\n' || text[j] == ' '));
            //Worttrimmer von Satzzeichen. In diesem Modus kann Kette nicht weiter gebildet werden! Bisherige Kette wird entweder mit oder ohne Wort direkt gepushed, aber Wort wird nie in Kette geladen
            else if(text[j] == '.' || text[j] == ',' || text[j] == ':' || text[j] == ';' || text[j] == '!' || text[j] == '?' || text[j] == '\"'){
                gerade_schon_leertaste = false;
                strcat(temp_word_chain_tester, " ");
                strcat(temp_word_chain_tester, temp_word);
                strcat(temp_word_chain_tester, " ");
                strcat(temp_word_chain_tester, "\0");
                // Fall 1 fuer anglizismenliste_Jugendwoerter - Wort wird an Kette gehaengt und findet Eintrag in Jugendwoertern -> direkt gepushed
                if(strstr(angliszismen[0].class, temp_word_chain_tester) != NULL){
                    if(strcmp(temp_word_chain, "") != 0) strcat(temp_word_chain, " ");
                    strcat(temp_word_chain, temp_word);
                    strcat(temp_word_chain, "\0");
                    //Zuerst pruefen ob genuegend Platz fuer weiteren Eintrag ist und wenn nicht dann reallocaten!
                    stacktracker++;
                    if(stacktracker > malloctracker){
                        malloctracker *= 2;
                        angliz = realloc(angliz, sizeof(detected_Anglizism) * malloctracker);
                        if (angliz == NULL){
                            printf("Fehler beim Reallocaten von Speicherplatz in detect_anglizismen. Speicherplatz konnte nicht erhöht werden!");
                            exit(EXIT_FAILURE);
                        }
                    }
                    //Kette auf Eintrag pushen
                    int length_of_entry = strlen(temp_word_chain) + 1;
                    angliz[(stacktracker-1)].anglizism = malloc (sizeof(char) * length_of_entry);
                    strncpy(angliz[(stacktracker-1)].anglizism, temp_word_chain, length_of_entry);
                    angliz[(stacktracker-1)].author = textfiles.textfiles[i].primary_author;
                    angliz[(stacktracker-1)].title = textfiles.textfiles[i].title;
                    angliz[(stacktracker-1)].year = textfiles.textfiles[i].year;
                    angliz[(stacktracker-1)].list = "Wörterbuch"; //gleichbedeutend mit which_list == 0
                }
                else{
                // Fall 1 fuer anglizismenliste_Jugendwoerter - Wort wird an Kette gehaengt und findet Eintrag in Jugendwoertern -> direkt gepushed
                    if(strstr(angliszismen[1].class, temp_word_chain_tester) != NULL){
                        if(strcmp(temp_word_chain, "") != 0) strcat(temp_word_chain, " ");
                        strcat(temp_word_chain, temp_word);
                        strcat(temp_word_chain, "\0");
                        //Zuerst pruefen ob genuegend Platz fuer weiteren Eintrag ist und wenn nicht dann reallocaten!
                        stacktracker++;
                        if(stacktracker > malloctracker){
                            malloctracker *= 2;
                            angliz = realloc(angliz, sizeof(detected_Anglizism) * malloctracker);
                            if (angliz == NULL){
                                printf("Fehler beim Reallocaten von Speicherplatz in detect_anglizismen. Speicherplatz konnte nicht erhöht werden!");
                                exit(EXIT_FAILURE);
                            }
                        }
                        //Kette auf Eintrag pushen
                        int length_of_entry = strlen(temp_word_chain) + 1;
                        angliz[(stacktracker-1)].anglizism = malloc (sizeof(char) * length_of_entry);
                        strncpy(angliz[(stacktracker-1)].anglizism, temp_word_chain, length_of_entry);
                        angliz[(stacktracker-1)].author = textfiles.textfiles[i].primary_author;
                        angliz[(stacktracker-1)].title = textfiles.textfiles[i].title;
                        angliz[(stacktracker-1)].year = textfiles.textfiles[i].year;
                        angliz[(stacktracker-1)].list = "Jahreswörter_tiny"; //gleichbedeutend mit which_list == 1
                    }
                    else{
                    // Fall 1 fuer anglizismenliste_Jugendwoerter - Wort wird an Kette gehaengt und findet Eintrag in Jugendwoertern -> direkt gepushed
                        if(strstr(angliszismen[2].class, temp_word_chain_tester) != NULL){
                            if(strcmp(temp_word_chain, "") != 0) strcat(temp_word_chain, " ");
                            strcat(temp_word_chain, temp_word);
                            strcat(temp_word_chain, "\0");
                            //Zuerst pruefen ob genuegend Platz fuer weiteren Eintrag ist und wenn nicht dann reallocaten!
                            stacktracker++;
                            if(stacktracker > malloctracker){
                                malloctracker *= 2;
                                angliz = realloc(angliz, sizeof(detected_Anglizism) * malloctracker);
                                if (angliz == NULL){
                                    printf("Fehler beim Reallocaten von Speicherplatz in detect_anglizismen. Speicherplatz konnte nicht erhöht werden!");
                                    exit(EXIT_FAILURE);
                                }
                            }
                            //Kette auf Eintrag pushen
                            int length_of_entry = strlen(temp_word_chain) + 1;
                            angliz[(stacktracker-1)].anglizism = malloc (sizeof(char) * length_of_entry);
                            strncpy(angliz[(stacktracker-1)].anglizism, temp_word_chain, length_of_entry);
                            angliz[(stacktracker-1)].author = textfiles.textfiles[i].primary_author;
                            angliz[(stacktracker-1)].title = textfiles.textfiles[i].title;
                            angliz[(stacktracker-1)].year = textfiles.textfiles[i].year;
                            angliz[(stacktracker-1)].list = "Jahreswörter_large"; //gleichbedeutend mit which_list == 2
                        }
                        // Fall 2 im Generellen (Kette auf Eintrag pushen) - Wort kann nicht an Kette angehaengt werden und muss einzeln verarbeitet werden
                        else{
                            if(strcmp(temp_word_chain, "") != 0){ 
                                strcat(temp_word_chain, "\0");
                                //Zuerst pruefen ob genuegend Platz fuer weiteren Eintrag ist und wenn nicht dann reallocaten!
                                stacktracker++;
                                if(stacktracker > malloctracker){
                                    malloctracker *= 2;
                                    angliz = realloc(angliz, sizeof(detected_Anglizism) * malloctracker);
                                    if (angliz == NULL){
                                        printf("Fehler beim Reallocaten von Speicherplatz in detect_anglizismen. Speicherplatz konnte nicht erhöht werden!");
                                        exit(EXIT_FAILURE);
                                    }
                                }
                                //Kette auf Eintrag pushen
                                int length_of_entry = strlen(temp_word_chain)  + 1;
                                angliz[(stacktracker-1)].anglizism = malloc (sizeof(char) * length_of_entry);
                                strncpy(angliz[(stacktracker-1)].anglizism, temp_word_chain, length_of_entry);
                                angliz[(stacktracker-1)].author = textfiles.textfiles[i].primary_author;
                                angliz[(stacktracker-1)].title = textfiles.textfiles[i].title;
                                angliz[(stacktracker-1)].year = textfiles.textfiles[i].year;
                                if ( which_list == 0){
                                    angliz[(stacktracker-1)].list = "Wörterbuch";
                                }
                                else if ( which_list == 1){
                                    angliz[(stacktracker-1)].list = "Jahreswörter_tiny";
                                }
                                else if ( which_list == 2){
                                    angliz[(stacktracker-1)].list = "Jahreswörter_large";
                                }
                                else{
                                    printf("FEHLER! which_list hat einen eigentlich unmöglichen Wert (Wert: %d) angenommen! Das Programm beendet sich nun!", which_list);
                                    exit(EXIT_FAILURE);
                                }
                            }
                            strcpy(temp_word_chain_tester, " ");
                            strcat(temp_word_chain_tester, temp_word);
                            strcat(temp_word_chain_tester, " ");
                            strcat(temp_word_chain_tester, "\0");
                            // Fall 2 fuer anglizismenliste_woerterbuch - Wort ist nicht Teil der Kette kann aber alleinstehend in Woerterbuch gefunden werden
                            if(strstr(angliszismen[0].class, temp_word_chain_tester) != NULL){
                                strcpy(temp_word_chain, temp_word);
                                strcat(temp_word_chain, "\0");
                                //Zuerst pruefen ob genuegend Platz fuer weiteren Eintrag ist und wenn nicht dann reallocaten!
                                stacktracker++;
                                if(stacktracker > malloctracker){
                                    malloctracker *= 2;
                                    angliz = realloc(angliz, sizeof(detected_Anglizism) * malloctracker);
                                    if (angliz == NULL){
                                        printf("Fehler beim Reallocaten von Speicherplatz in detect_anglizismen. Speicherplatz konnte nicht erhöht werden!");
                                        exit(EXIT_FAILURE);
                                    }
                                }
                                //Kette auf Eintrag pushen
                                int length_of_entry = strlen(temp_word_chain) + 1;
                                angliz[(stacktracker-1)].anglizism = malloc (sizeof(char) * length_of_entry);
                                strncpy(angliz[(stacktracker-1)].anglizism, temp_word_chain, length_of_entry);
                                angliz[(stacktracker-1)].author = textfiles.textfiles[i].primary_author;
                                angliz[(stacktracker-1)].title = textfiles.textfiles[i].title;
                                angliz[(stacktracker-1)].year = textfiles.textfiles[i].year;
                                angliz[(stacktracker-1)].list = "Wörterbuch"; //gleichbedeutend mit which_list == 0
                            }
                            else{
                                // Fall 2 fuer anglizismenliste_Jugendwoerter_tiny - Wort ist nicht Teil der Kette kann aber alleinstehend in Jugendwoertern gefunden werden
                                if(strstr(angliszismen[1].class, temp_word_chain_tester) != NULL){
                                    strcpy(temp_word_chain, temp_word);
                                    strcat(temp_word_chain, "\0");
                                    //Zuerst pruefen ob genuegend Platz fuer weiteren Eintrag ist und wenn nicht dann reallocaten!
                                    stacktracker++;
                                    if(stacktracker > malloctracker){
                                        malloctracker *= 2;
                                        angliz = realloc(angliz, sizeof(detected_Anglizism) * malloctracker);
                                        if (angliz == NULL){
                                            printf("Fehler beim Reallocaten von Speicherplatz in detect_anglizismen. Speicherplatz konnte nicht erhöht werden!");
                                            exit(EXIT_FAILURE);
                                        }
                                    }
                                    //Kette auf Eintrag pushen
                                    int length_of_entry = strlen(temp_word_chain) + 1;
                                    angliz[(stacktracker-1)].anglizism = malloc (sizeof(char) * length_of_entry);
                                    strncpy(angliz[(stacktracker-1)].anglizism, temp_word_chain, length_of_entry);
                                    angliz[(stacktracker-1)].author = textfiles.textfiles[i].primary_author;
                                    angliz[(stacktracker-1)].title = textfiles.textfiles[i].title;
                                    angliz[(stacktracker-1)].year = textfiles.textfiles[i].year;
                                    angliz[(stacktracker-1)].list = "Jahreswörter_tiny"; //gleichbedeutend mit which_list == 1
                                }
                                else{
                                    // Fall 2 fuer anglizismenliste_Jugendwoerter_large - Wort ist nicht Teil der Kette kann aber alleinstehend in Jugendwoertern gefunden werden
                                    if(strstr(angliszismen[2].class, temp_word_chain_tester) != NULL){
                                        strcpy(temp_word_chain, temp_word);
                                        strcat(temp_word_chain, "\0");
                                        //Zuerst pruefen ob genuegend Platz fuer weiteren Eintrag ist und wenn nicht dann reallocaten!
                                        stacktracker++;
                                        if(stacktracker > malloctracker){
                                            malloctracker *= 2;
                                            angliz = realloc(angliz, sizeof(detected_Anglizism) * malloctracker);
                                            if (angliz == NULL){
                                                printf("Fehler beim Reallocaten von Speicherplatz in detect_anglizismen. Speicherplatz konnte nicht erhöht werden!");
                                                exit(EXIT_FAILURE);
                                            }
                                        }
                                        //Kette auf Eintrag pushen
                                        int length_of_entry = strlen(temp_word_chain) + 1;
                                        angliz[(stacktracker-1)].anglizism = malloc (sizeof(char) * length_of_entry);
                                        strncpy(angliz[(stacktracker-1)].anglizism, temp_word_chain, length_of_entry);
                                        angliz[(stacktracker-1)].author = textfiles.textfiles[i].primary_author;
                                        angliz[(stacktracker-1)].title = textfiles.textfiles[i].title;
                                        angliz[(stacktracker-1)].year = textfiles.textfiles[i].year;
                                        angliz[(stacktracker-1)].list = "Jahreswörter_large"; //gleichbedeutend mit which_list == 2
                                    }
                                    //Fall 3 - Wort kann weder mit Kette noch alleinstehend als Anglizismus gefunden werden. Nix muss geschehen, da am ende des Satzzeichens eh alles resettet wird.
                                }
                            }
                        }
                    }
                }
                //Auf Grund von Satzzeichen wird alles resettet!
                    strcpy(temp_word_chain, "");
                    strcpy(temp_word_chain_tester, "");
                    strcpy(temp_word, "");
            }
            //Hier wird alles mid-word gescannt und auf temp_word geschrieben.
            else{ //liest die normalen/gewollten Zeichen ein.
                strncat(temp_word, &text[j], 1);
                gerade_schon_leertaste = false;
            }
        }
        //Da die Schleife mit dem Ende des Textes Endet kann das letzte Wort nicht mehr durch den \n Splitter kommen und die Kette wird nie eingetragen, daher hier noch eine letzte Eintragsueberpruefung
        strcat(temp_word_chain_tester, " ");
        strcat(temp_word_chain_tester, temp_word);
        strcat(temp_word_chain_tester, " ");
        strcat(temp_word_chain_tester, "\0");
        // Fall 1 fuer anglizismenliste_Jugendwoerter - Wort wird an Kette gehaengt und findet Eintrag in Jugendwoertern -> direkt gepushed
        if(strstr(angliszismen[0].class, temp_word_chain_tester) != NULL){
            if(strcmp(temp_word_chain, "") != 0) strcat(temp_word_chain, " ");
            strcat(temp_word_chain, temp_word);
            strcat(temp_word_chain, "\0");
            //Zuerst pruefen ob genuegend Platz fuer weiteren Eintrag ist und wenn nicht dann reallocaten!
            stacktracker++;
            if(stacktracker > malloctracker){
                malloctracker *= 2;
                angliz = realloc(angliz, sizeof(detected_Anglizism) * malloctracker);
                if (angliz == NULL){
                    printf("Fehler beim Reallocaten von Speicherplatz in detect_anglizismen. Speicherplatz konnte nicht erhöht werden!");
                    exit(EXIT_FAILURE);
                }
            }
            //Kette auf Eintrag pushen
            int length_of_entry = strlen(temp_word_chain) + 1;
            angliz[(stacktracker-1)].anglizism = malloc (sizeof(char) * length_of_entry);
            strncpy(angliz[(stacktracker-1)].anglizism, temp_word_chain, length_of_entry);
            angliz[(stacktracker-1)].author = textfiles.textfiles[i].primary_author;
            angliz[(stacktracker-1)].title = textfiles.textfiles[i].title;
            angliz[(stacktracker-1)].year = textfiles.textfiles[i].year;
            angliz[(stacktracker-1)].list = "Wörterbuch"; //gleichbedeutend mit which_list == 0
        }
        else{
            // Fall 1 fuer anglizismenliste_Jugendwoerter - Wort wird an Kette gehaengt und findet Eintrag in Jugendwoertern -> direkt gepushed
            if(strstr(angliszismen[1].class, temp_word_chain_tester) != NULL){
                if(strcmp(temp_word_chain, "") != 0) strcat(temp_word_chain, " ");
                strcat(temp_word_chain, temp_word);
                strcat(temp_word_chain, "\0");
                //Zuerst pruefen ob genuegend Platz fuer weiteren Eintrag ist und wenn nicht dann reallocaten!
                stacktracker++;
                if(stacktracker > malloctracker){
                    malloctracker *= 2;
                    angliz = realloc(angliz, sizeof(detected_Anglizism) * malloctracker);
                    if (angliz == NULL){
                        printf("Fehler beim Reallocaten von Speicherplatz in detect_anglizismen. Speicherplatz konnte nicht erhöht werden!");
                        exit(EXIT_FAILURE);
                    }
                }
                //Kette auf Eintrag pushen
                int length_of_entry = strlen(temp_word_chain) + 1;
                angliz[(stacktracker-1)].anglizism = malloc (sizeof(char) * length_of_entry);
                strncpy(angliz[(stacktracker-1)].anglizism, temp_word_chain, length_of_entry);
                angliz[(stacktracker-1)].author = textfiles.textfiles[i].primary_author;
                angliz[(stacktracker-1)].title = textfiles.textfiles[i].title;
                angliz[(stacktracker-1)].year = textfiles.textfiles[i].year;
                angliz[(stacktracker-1)].list = "Jahreswörter_tiny"; //gleichbedeutend mit which_list == 1
            }
            else{
                // Fall 1 fuer anglizismenliste_Jugendwoerter - Wort wird an Kette gehaengt und findet Eintrag in Jugendwoertern -> direkt gepushed
                if(strstr(angliszismen[2].class, temp_word_chain_tester) != NULL){
                    if(strcmp(temp_word_chain, "") != 0) strcat(temp_word_chain, " ");
                    strcat(temp_word_chain, temp_word);
                    strcat(temp_word_chain, "\0");
                    //Zuerst pruefen ob genuegend Platz fuer weiteren Eintrag ist und wenn nicht dann reallocaten!
                    stacktracker++;
                    if(stacktracker > malloctracker){
                        malloctracker *= 2;
                        angliz = realloc(angliz, sizeof(detected_Anglizism) * malloctracker);
                        if (angliz == NULL){
                            printf("Fehler beim Reallocaten von Speicherplatz in detect_anglizismen. Speicherplatz konnte nicht erhöht werden!");
                            exit(EXIT_FAILURE);
                        }
                    }
                    //Kette auf Eintrag pushen
                    int length_of_entry = strlen(temp_word_chain) + 1;
                    angliz[(stacktracker-1)].anglizism = malloc (sizeof(char) * length_of_entry);
                    strncpy(angliz[(stacktracker-1)].anglizism, temp_word_chain, length_of_entry);
                    angliz[(stacktracker-1)].author = textfiles.textfiles[i].primary_author;
                    angliz[(stacktracker-1)].title = textfiles.textfiles[i].title;
                    angliz[(stacktracker-1)].year = textfiles.textfiles[i].year;
                    angliz[(stacktracker-1)].list = "Jahreswörter_large"; //gleichbedeutend mit which_list == 2
                }
                // Fall 2 im Generellen (Kette auf Eintrag pushen) - Wort kann nicht an Kette angehaengt werden und muss einzeln verarbeitet werden
                else{
                    if(strcmp(temp_word_chain, "") != 0){ 
                        strcat(temp_word_chain, "\0");
                        //Zuerst pruefen ob genuegend Platz fuer weiteren Eintrag ist und wenn nicht dann reallocaten!
                        stacktracker++;
                        if(stacktracker > malloctracker){
                            malloctracker *= 2;
                            angliz = realloc(angliz, sizeof(detected_Anglizism) * malloctracker);
                            if (angliz == NULL){
                                printf("Fehler beim Reallocaten von Speicherplatz in detect_anglizismen. Speicherplatz konnte nicht erhöht werden!");
                                exit(EXIT_FAILURE);
                            }
                        }
                        //Kette auf Eintrag pushen
                        int length_of_entry = strlen(temp_word_chain)  + 1;
                        angliz[(stacktracker-1)].anglizism = malloc (sizeof(char) * length_of_entry);
                        strncpy(angliz[(stacktracker-1)].anglizism, temp_word_chain, length_of_entry);
                        angliz[(stacktracker-1)].author = textfiles.textfiles[i].primary_author;
                        angliz[(stacktracker-1)].title = textfiles.textfiles[i].title;
                        angliz[(stacktracker-1)].year = textfiles.textfiles[i].year;
                        if ( which_list == 0){
                            angliz[(stacktracker-1)].list = "Wörterbuch";
                        }
                        else if ( which_list == 1){
                            angliz[(stacktracker-1)].list = "Jahreswörter_tiny";
                        }
                        else if ( which_list == 2){
                            angliz[(stacktracker-1)].list = "Jahreswörter_large";
                        }
                        else{
                            printf("FEHLER! which_list hat einen eigentlich unmöglichen Wert (Wert: %d) angenommen! Das Programm beendet sich nun!", which_list);
                            exit(EXIT_FAILURE);
                        }
                    }
                    strcpy(temp_word_chain_tester, " ");
                    strcat(temp_word_chain_tester, temp_word);
                    strcat(temp_word_chain_tester, " ");
                    strcat(temp_word_chain_tester, "\0");
                    // Fall 2 fuer anglizismenliste_woerterbuch - Wort ist nicht Teil der Kette kann aber alleinstehend in Woerterbuch gefunden werden
                    if(strstr(angliszismen[0].class, temp_word_chain_tester) != NULL){
                        strcpy(temp_word_chain, temp_word);
                        strcat(temp_word_chain, "\0");
                        //Zuerst pruefen ob genuegend Platz fuer weiteren Eintrag ist und wenn nicht dann reallocaten!
                        stacktracker++;
                        if(stacktracker > malloctracker){
                            malloctracker *= 2;
                            angliz = realloc(angliz, sizeof(detected_Anglizism) * malloctracker);
                            if (angliz == NULL){
                                printf("Fehler beim Reallocaten von Speicherplatz in detect_anglizismen. Speicherplatz konnte nicht erhöht werden!");
                                exit(EXIT_FAILURE);
                            }
                        }
                        //Kette auf Eintrag pushen
                        int length_of_entry = strlen(temp_word_chain) + 1;
                        angliz[(stacktracker-1)].anglizism = malloc (sizeof(char) * length_of_entry);
                        strncpy(angliz[(stacktracker-1)].anglizism, temp_word_chain, length_of_entry);
                        angliz[(stacktracker-1)].author = textfiles.textfiles[i].primary_author;
                        angliz[(stacktracker-1)].title = textfiles.textfiles[i].title;
                        angliz[(stacktracker-1)].year = textfiles.textfiles[i].year;
                        angliz[(stacktracker-1)].list = "Wörterbuch"; //gleichbedeutend mit which_list == 0
                    }
                    else{
                        // Fall 2 fuer anglizismenliste_Jugendwoerter_tiny - Wort ist nicht Teil der Kette kann aber alleinstehend in Jugendwoertern gefunden werden
                        if(strstr(angliszismen[1].class, temp_word_chain_tester) != NULL){
                            strcpy(temp_word_chain, temp_word);
                            strcat(temp_word_chain, "\0");
                            //Zuerst pruefen ob genuegend Platz fuer weiteren Eintrag ist und wenn nicht dann reallocaten!
                            stacktracker++;
                            if(stacktracker > malloctracker){
                                malloctracker *= 2;
                                angliz = realloc(angliz, sizeof(detected_Anglizism) * malloctracker);
                                if (angliz == NULL){
                                    printf("Fehler beim Reallocaten von Speicherplatz in detect_anglizismen. Speicherplatz konnte nicht erhöht werden!");
                                    exit(EXIT_FAILURE);
                                }
                            }
                            //Kette auf Eintrag pushen
                            int length_of_entry = strlen(temp_word_chain) + 1;
                            angliz[(stacktracker-1)].anglizism = malloc (sizeof(char) * length_of_entry);
                            strncpy(angliz[(stacktracker-1)].anglizism, temp_word_chain, length_of_entry);
                            angliz[(stacktracker-1)].author = textfiles.textfiles[i].primary_author;
                            angliz[(stacktracker-1)].title = textfiles.textfiles[i].title;
                            angliz[(stacktracker-1)].year = textfiles.textfiles[i].year;
                            angliz[(stacktracker-1)].list = "Jahreswörter_tiny"; //gleichbedeutend mit which_list == 1
                        }
                        else{
                            // Fall 2 fuer anglizismenliste_Jugendwoerter_large - Wort ist nicht Teil der Kette kann aber alleinstehend in Jugendwoertern gefunden werden
                            if(strstr(angliszismen[2].class, temp_word_chain_tester) != NULL){
                                strcpy(temp_word_chain, temp_word);
                                strcat(temp_word_chain, "\0");
                                //Zuerst pruefen ob genuegend Platz fuer weiteren Eintrag ist und wenn nicht dann reallocaten!
                                stacktracker++;
                                if(stacktracker > malloctracker){
                                    malloctracker *= 2;
                                    angliz = realloc(angliz, sizeof(detected_Anglizism) * malloctracker);
                                    if (angliz == NULL){
                                        printf("Fehler beim Reallocaten von Speicherplatz in detect_anglizismen. Speicherplatz konnte nicht erhöht werden!");
                                        exit(EXIT_FAILURE);
                                    }
                                }
                                //Kette auf Eintrag pushen
                                int length_of_entry = strlen(temp_word_chain) + 1;
                                angliz[(stacktracker-1)].anglizism = malloc (sizeof(char) * length_of_entry);
                                strncpy(angliz[(stacktracker-1)].anglizism, temp_word_chain, length_of_entry);
                                angliz[(stacktracker-1)].author = textfiles.textfiles[i].primary_author;
                                angliz[(stacktracker-1)].title = textfiles.textfiles[i].title;
                                angliz[(stacktracker-1)].year = textfiles.textfiles[i].year;
                                angliz[(stacktracker-1)].list = "Jahreswörter_large"; //gleichbedeutend mit which_list == 1
                            }
                            //Fall 3 - Wort kann weder mit Kette noch alleinstehend als Anglizismus gefunden werden. Nix muss geschehen, da am ende der for Schleife eh alles resettet wird.
                        }
                    }
                }
            }
        }
        //Ende des Textdokuments erreicht, resettet alles!
        strcpy(temp_word_chain, "");
        strcpy(temp_word_chain_tester, "");
        strcpy(temp_word, "");
    }
    //Bevor angliz returned wird, lohnt es sich unnoetigen Platz im Pointerarray loszuwerden durch realloc, da durch konstantes multiplizieren des Platzes sich evtl ein großer ungenutzter Speicherplatz ansammelt. Bei besonders großem Korpus definitv die Memoryumplatzierung wert.
    if(stacktracker == 0){
        printf("Es wurden keine Anglizismen in den Dokumenten gefunden! Das Sortieren erübrigt sich somit und das Programm beendet sich!\n");
        exit(EXIT_SUCCESS);
    }
    angliz = realloc(angliz, sizeof(detected_Anglizism) * stacktracker);
    if (angliz == NULL){
        printf("Fehler beim Reallocaten von Speicherplatz in detect_anglizismen. Speicherplatz konnte nicht reduziert werden!");
        exit(EXIT_FAILURE);
    }
    //Ueberfuehrung in Struct, da die Laenge von Structs im Nachhinein sonst nicht garantiert ermittelt werden kann.
    else{
        All_Anglizisms all_angliz;
        all_angliz.anglizismen = angliz;
        all_angliz.amount_of_anglizisms = stacktracker;
        return all_angliz;
    } 
}

//Sucht nach Anglizismen im Korpus mit Hilfe von Anglizismenlisten
All_Anglizisms detect_anglizismen_lemma_alt (All_Textfiles_lemma_alt textfiles, Anglizismenklassen* angliszismen){
    long long malloctracker = 500; 
    long long  stacktracker = 0;
    detected_Anglizism* angliz = malloc (sizeof(detected_Anglizism) * malloctracker);
    char* word_chain = malloc (sizeof(char) * 300); //Wird auch immer wieder verwendet, soll jedoch fuer Eintraege, die laenger als ein Wort sind herhalten.
    char* temp_word_chain = malloc (sizeof(char) * 300); //Wird auch immer wieder verwendet, soll jedoch fuer Eintraege, die laenger als ein Wort sind herhalten.
    char* temp_word_chain_tester = malloc (sizeof(char) * 300); //Wird auch immer wieder verwertet, kann aber beliebig zum Testen resettet werden
    strcpy(temp_word_chain, "");
    strcpy(word_chain, "");
    char* which_list = malloc (sizeof(char) * 30);
    for(int i=0; i < textfiles.amount_of_textfiles; i++){
        duality_text wordset = textfiles.textfiles[i].text;
        if(wordset.length > 10000) {
            i++;
            wordset = textfiles.textfiles[i].text;
        }
        for(int j=0; j < wordset.length; j++){
            //Fall 1: Derzeit liegt kein Anglizismus in der Kette vor und es wird danach gesucht
            if(strcmp(temp_word_chain, "") == 0){
                //bereitet wordchain zum testen vor 
                strcpy(temp_word_chain_tester, " ");
                strcat(temp_word_chain_tester, wordset.text[j].lemma);
                strcat(temp_word_chain_tester, " ");
                //testet wordchain auf anglizismus. Wenn ja dann wird eintrag auf chain gepushed
                if(strstr(angliszismen[0].class, temp_word_chain_tester) != NULL || strstr(angliszismen[1].class, temp_word_chain_tester) != NULL || strstr(angliszismen[2].class, temp_word_chain_tester) != NULL){
                    strcpy(temp_word_chain, " ");
                    strcat(temp_word_chain, wordset.text[j].lemma);
                    strcat(word_chain, wordset.text[j].token);                
                    if(strstr(angliszismen[0].class, temp_word_chain_tester) != NULL ){
                        strcpy(which_list, "Wörterbuch");
                    }
                    else if(strstr(angliszismen[1].class, temp_word_chain_tester) != NULL ){
                        strcpy(which_list, "Jahreswörter_tiny");
                    }
                    else{
                        strcpy(which_list, "Jahreswörter_large");
                    }
                    //printf("Anglizimus gefunden: \"%s\" (temp_word_chain_tester = \"%s\") Stacktracker ist %lld und malloctracker ist: %lld i und j sind %d und %d\n", word_chain, temp_word_chain_tester, stacktracker, malloctracker, i, j);         

                }
            }
            //Fall 2: Es wurde bereits ein Anglizismus gefunden, es wird auf Kette überprüft.
            else{
                //bereitet wordchain zum testen vor 
                strcpy(temp_word_chain_tester, "");
                strcat(temp_word_chain_tester, temp_word_chain);
                strcat(temp_word_chain_tester, " ");
                //printf("wordset.text[j].lemma: %s i ist: %d und j ist: %d\n", wordset.text[j].lemma, i, j);
                strcat(temp_word_chain_tester, wordset.text[j].lemma);
                strcat(temp_word_chain_tester, " ");
                //testet wordchain auf anglizismus. Wenn ja dann wird eintrag auf chain gepushed
                if(strstr(angliszismen[0].class, temp_word_chain_tester) != NULL || strstr(angliszismen[1].class, temp_word_chain_tester) != NULL || strstr(angliszismen[2].class, temp_word_chain_tester) != NULL){
                    strcat(temp_word_chain, wordset.text[j].lemma);
                    strcat(word_chain, " ");
                    strcat(word_chain, wordset.text[j].token);                
                    if(strstr(angliszismen[0].class, temp_word_chain_tester) != NULL ){
                        strcpy(which_list, "Wörterbuch");
                    }
                    else if(strstr(angliszismen[1].class, temp_word_chain_tester) != NULL ){
                        strcpy(which_list, "Jahreswörter_tiny");
                    }
                    else{
                        strcpy(which_list, "Jahreswörter_large");
                    }
                }
                // Wenn nicht dann wird eintrag auf angliz gepushed
                else{
                    //Platzerweiterung wenn nötig
                    stacktracker++;
                    if(stacktracker > malloctracker){
                        malloctracker *= 2;
                        angliz = realloc(angliz, sizeof(detected_Anglizism) * malloctracker);
                        if (angliz == NULL){
                            printf("Fehler beim Reallocaten von Speicherplatz in detect_anglizismen_lemma_alt. Speicherplatz konnte nicht erhoeht werden!");
                            exit(EXIT_FAILURE);
                        }
                    }
                    //Eintrag push
                    angliz[(stacktracker-1)].anglizism = malloc (sizeof(char) * (strlen(word_chain) + 1));
                    strcpy(angliz[(stacktracker-1)].anglizism, word_chain);
                    angliz[(stacktracker-1)].author = textfiles.textfiles[i].primary_author;
                    angliz[(stacktracker-1)].title = textfiles.textfiles[i].title;
                    angliz[(stacktracker-1)].year = textfiles.textfiles[i].year;
                    angliz[(stacktracker-1)].list = malloc (sizeof(char) * (strlen(which_list) + 1));
                    strcpy(angliz[(stacktracker-1)].list, which_list);
                    //printf("Kette gepushed: \"%s\" (temp_word_chain_tester = \"%s\") Stacktracker ist %lld und malloctracker ist: %lld i und j sind %d und %d\n", word_chain, temp_word_chain_tester, stacktracker, malloctracker, i, j);         
                    //Das neue Wort war nun zwar nicht teil der vorhergehenden Kette, kann jedoch potentiell selbst eine neue bilden
                    strcpy(temp_word_chain_tester, " ");
                    strcat(temp_word_chain_tester, wordset.text[j].lemma);
                    strcat(temp_word_chain_tester, " ");   
                    if(strstr(angliszismen[0].class, temp_word_chain_tester) != NULL || strstr(angliszismen[1].class, temp_word_chain_tester) != NULL || strstr(angliszismen[2].class, temp_word_chain_tester) != NULL){
                        strcpy(temp_word_chain, " ");
                        strcat(temp_word_chain, wordset.text[j].lemma);
                        strcpy(word_chain, wordset.text[j].token);
                    }
                    else{
                        strcpy(temp_word_chain, "");
                        strcpy(word_chain, "");
                    }
                }
            }
        }
    }
    All_Anglizisms all_angliz;
    all_angliz.anglizismen = angliz;
    all_angliz.amount_of_anglizisms = stacktracker;
    return all_angliz;
}

//Zählt generell alle Wörter und weißt sie ihrer entsprechenden Jahreszahl zu. Dies wird mit einem Tupel an Jahreszahl, Wortanzahl returned.
All_Words_Per_Year_Set count_all_words_per_year(All_Textfiles textfiles){
    int malloctracker = 50; //Sinnvollerweise auf die erwartete Jahresspanne zu setzen, aber erweitert sich selbstständig
    int stacktracker = 0;
    All_Words_Per_Year* year_angl_tuple = malloc (sizeof(All_Words_Per_Year) * malloctracker);
    bool gerade_schon_buchstabe;
    bool jahr_bereits_vorhanden;
    int jahr_index;
    for(int i=0; i < textfiles.amount_of_textfiles; i++){
        char* text = textfiles.textfiles[i].text;
        //Setzt zuerst für den ganzen Text das Jahr und fängt entweder neuen Slot an wenn noch nicht vorhanden und benutzt alten wenn bereits vorhanden
        char* year = textfiles.textfiles[i].year;
        gerade_schon_buchstabe = false;
        jahr_bereits_vorhanden = false;
        for(int x = 0; x < stacktracker; x++){
            //Wenn Jahr bereits existiert
            if(strcmp(year_angl_tuple[x].year, year) == 0){
                jahr_bereits_vorhanden = true;
                jahr_index=x;
                break;
            }
        }
        //Wenn Jahr neuen Eintrag braucht
        if(jahr_bereits_vorhanden == false){
            /*
            Testprinter
            printf("Ein neues Jahr wurde gefunden! %s war noch nicht registriert!\n", year);
            */
            jahr_index=stacktracker;
            stacktracker++;
            if(stacktracker > malloctracker){
                malloctracker *= 2;
                year_angl_tuple = realloc(year_angl_tuple, sizeof(All_Words_Per_Year) * malloctracker);
                if (year_angl_tuple == NULL){
                    printf("Fehler beim Reallocaten von Speicherplatz in count_all_words_per_year. Speicherplatz konnte nicht erhöht werden!");
                    exit(EXIT_FAILURE);
                }
            }
            year_angl_tuple[jahr_index].year=year;
            year_angl_tuple[jahr_index].word_count=0;
        }
        //Momentanes Jahr ist nun im Index jahr_index zu finden
        for(int j=0; text[j] != '\0';j++){
            if(text[j] == '\n' || text[j] == ' ' || text[j] == '.' || text[j] == ',' || text[j] == ':' || text[j] == ';' || text[j] == '!' || text[j] == '?' || text[j] == '\"'){ 
                gerade_schon_buchstabe = false;
            }
            else if(gerade_schon_buchstabe == true);
            else{
                year_angl_tuple[jahr_index].word_count++;
                gerade_schon_buchstabe = true;
            }
        }
    }
    All_Words_Per_Year_Set all_words_set;
    all_words_set.set=year_angl_tuple;
    all_words_set.anzahl=stacktracker;
    return all_words_set;
}

//Zählt generell alle Wörter und weißt sie ihrem entsprechenden Autor zu. Dies wird mit einem Tupel an Autor, Wortanzahl returned.
All_Words_Per_Author_Set count_all_words_per_author(All_Textfiles textfiles){
    int malloctracker = 50; //Sinnvollerweise auf die erwartete Autoranzahl zu setzen, aber erweitert sich selbstständig
    int stacktracker = 0;
    All_Words_Per_Author* author_angl_tuple = malloc (sizeof(All_Words_Per_Author) * malloctracker);
    bool gerade_schon_buchstabe;
    bool author_bereits_vorhanden;
    int author_index;
    for(int i=0; i < textfiles.amount_of_textfiles; i++){
        char* text = textfiles.textfiles[i].text;
        //Setzt zuerst für den ganzen Text den Autor und fängt entweder neuen Slot an wenn noch nicht vorhanden und benutzt alten wenn bereits vorhanden
        char* author = textfiles.textfiles[i].primary_author;
        gerade_schon_buchstabe = false;
        author_bereits_vorhanden = false;
        for(int x = 0; x < stacktracker; x++){
            //Wenn Autor bereits existiert
            if(strcmp(author_angl_tuple[x].author, author) == 0){
                author_bereits_vorhanden = true;
                author_index=x;
                break;
            }
        }
        //Wenn Autor neuen Eintrag braucht
        if(author_bereits_vorhanden == false){
            /*
            Testprinter
            printf("Ein neuer Autor wurde gefunden! %s war noch nicht registriert!\n", author);
            */
            author_index=stacktracker;
            stacktracker++;
            if(stacktracker > malloctracker){
                malloctracker *= 2;
                author_angl_tuple = realloc(author_angl_tuple, sizeof(All_Words_Per_Author) * malloctracker);
                if (author_angl_tuple == NULL){
                    printf("Fehler beim Reallocaten von Speicherplatz in count_all_words_per_author. Speicherplatz konnte nicht erhöht werden!");
                    exit(EXIT_FAILURE);
                }
            }
            author_angl_tuple[author_index].author=author;
            author_angl_tuple[author_index].word_count=0;
        }
        //Momentaner Autor ist nun im Index author_index zu finden
        for(int j=0; text[j] != '\0';j++){
            if(text[j] == '\n' || text[j] == ' ' || text[j] == '.' || text[j] == ',' || text[j] == ':' || text[j] == ';' || text[j] == '!' || text[j] == '?' || text[j] == '\"'){ 
                gerade_schon_buchstabe = false;
            }
            else if(gerade_schon_buchstabe == true);
            else{
                author_angl_tuple[author_index].word_count++;
                gerade_schon_buchstabe = true;
            }
        }
    }
    All_Words_Per_Author_Set all_words_set;
    all_words_set.set=author_angl_tuple;
    all_words_set.anzahl=stacktracker;
    return all_words_set;
}

//Sortiert die Eintraege nach den Anglizismen und schreibt Anglizismen und ihre Anzahl in eine File
distinct_anglis* sort_by_anglizism(All_Anglizisms all_anglis){
    int malloctracker = 500; 
    int stacktracker = 0;
    int position_in_struct=0; //steht nicht fuer position im array, sondern fuer laenge (also pos+1), da 0 fuer "nicht gefunden" reserviert bleibt
    distinct_anglis* dist_angliz = malloc (sizeof(distinct_anglis) * malloctracker);
    if (dist_angliz == NULL){
        printf("Distinct_Anglizism Space! There was too little memory left to find space for a Distinct_Anglizism or the Distinct_Anglizism doesnt exist at all! Needed space was (%d) chars", malloctracker);
        exit(EXIT_FAILURE);
    }
    //Zaehlt die Anglizismen indem er sie zu distict anglizisms (struct) ueberfuehrt und anzahl darin festhaelt
    for(int i=0; i<all_anglis.amount_of_anglizisms; i++){
        //Checkt ob das Wort bereits in struct uebergeben wurde und wenn ja wo es gefunden werden kann
        for(int j=0; j < stacktracker ;j++){ 
            if(strcmp(dist_angliz[j].anglizism, all_anglis.anglizismen[i].anglizism) == 0){
                position_in_struct = j + 1;
                break;
            }
        }
        //Wenn das Wort noch nicht im tracker ist, wird es dem tracker hinzugefuegt und ein neuer Eintrag im structarray wird erstellt
        if (position_in_struct == 0){
            //Zuerst pruefen ob genuegend Platz fuer weiteren Eintrag ist und wenn nicht dann reallocaten!
            stacktracker++;
            if(stacktracker > malloctracker){
                malloctracker *= 2;
                dist_angliz = realloc(dist_angliz, sizeof(distinct_anglis) * malloctracker);
                if (dist_angliz == NULL){
                    printf("Fehler beim Reallocaten von Speicherplatz in sort_by_anglizism. Speicherplatz konnte nicht erhöht werden!");
                    exit(EXIT_FAILURE);
                }
            }
            //Uebergebe den Pointer zum Anglizismus und setze Counter auf 1.
            dist_angliz[stacktracker-1].anglizism = all_anglis.anglizismen[i].anglizism;
            dist_angliz[stacktracker-1].counter = 1;
        }
        //Wenn das Wort bereits im tracker ist, dann wird dessen count erhoeht und position_in_struct wird resettet.
        else{
            dist_angliz[position_in_struct-1].counter++;
            position_in_struct=0;
        }
    }

    //uebergibt den bearbeiteten String in ein neues Textdokument via filewriter
    FILE *filePointer = fopen("out\\Sorted_By_Anglizism.txt","w");
    fprintf(filePointer, "Es wurden insgesamt %d unterschiedliche Anglizismen gefunden!\nDiese Anglizismen sind:\n\n", stacktracker);
    for(int i= 0; i < stacktracker ; i++){
        fprintf(filePointer, "[%s] kommt %d mal vor!\n", dist_angliz[i].anglizism, dist_angliz[i].counter);
    }
    fclose(filePointer);
    //calculiert das F-maß
    //calculate_f_score(dist_angliz, pathname_manuelle_Untersuchung, stacktracker, all_anglis.amount_of_anglizisms);
    return(dist_angliz);
}

//Sortiert die Eintraege nach den Autoren und schreibt Autoren mit ihren genutzten Anglizismen (zusammen mit der Gesamtanzahl der Anglizismen) in eine File
void sort_by_author_and_anglizism(All_Anglizisms all_anglis, char* path, All_Words_Per_Author_Set all_words_in_author_X){
    int malloctracker_aut = 500; 
    int stacktracker_aut = 0; 
    int position_in_struct_aut=0; //steht nicht fuer position im array, sondern fuer laenge (also pos+1), da 0 fuer "nicht gefunden" reserviert bleibt
    int position_in_struct_angl=0; //siehe oben
    int all_angl = 0;
    distinct_category* dist_aut = malloc (sizeof(distinct_category) * malloctracker_aut);
    for(int i=0; i<all_anglis.amount_of_anglizisms; i++){
        //Checkt ob der Autor bereits in struct uebergeben wurde und wenn ja wo es gefunden werden kann
        for(int j=0; j < stacktracker_aut ;j++){ 
            if(strcmp(dist_aut[j].category, all_anglis.anglizismen[i].author) == 0){
                position_in_struct_aut = j + 1;
                break;
            }
        }
        //Wenn der Autor noch nicht im tracker ist, wird es dem tracker hinzugefuegt und da der neue Eintrag definitiv noch keine Anglizismen hat kann sofort ein Anglizismus eingetragen werden auf counter 1
        if (position_in_struct_aut == 0){
            //Zuerst pruefen ob genuegend Platz fuer weiteren Eintrag ist und wenn nicht dann reallocaten!
            stacktracker_aut++;
            if(stacktracker_aut > malloctracker_aut){
                malloctracker_aut *= 2;
                dist_aut = realloc(dist_aut, sizeof(distinct_category) * malloctracker_aut);
                if (dist_aut == NULL){
                    printf("Fehler beim Reallocaten von Speicherplatz in sort_by_author für Autoreintrag. Speicherplatz konnte nicht erhöht werden!");
                    exit(EXIT_FAILURE);
                }
            }
            //Initialisiere Autor mit Startwerten und dem jeweiligen Anglizismus
            dist_aut[stacktracker_aut-1].malloctracker=100;
            dist_aut[stacktracker_aut-1].stacktracker=1;
            dist_aut[stacktracker_aut-1].anglizism = malloc (sizeof(distinct_anglis) * dist_aut[stacktracker_aut-1].malloctracker);
            if(dist_aut[stacktracker_aut-1].anglizism == NULL){
                //printf("Distinct_Aut Space! There was too little memory left to find space for a Distinct_Aut or the Distinct_Aut doesnt exist at all! Needed space was (%d) chars", malloctracker_aut);
                exit(EXIT_FAILURE);
            }
            dist_aut[stacktracker_aut-1].category = all_anglis.anglizismen[i].author;
            dist_aut[stacktracker_aut-1].anglizism[0].anglizism = all_anglis.anglizismen[i].anglizism;
            dist_aut[stacktracker_aut-1].anglizism[0].counter = 1;
            dist_aut[stacktracker_aut-1].total_tracker = 1;
            all_angl++;
            //printf("Der Autor %s ist neu! Der Anglizismus %s wurde mit dem Counter %d direkt eingefuegt\n", dist_aut[stacktracker_aut-1].category, dist_aut[stacktracker_aut-1].anglizism[0].anglizism, dist_aut[stacktracker_aut-1].anglizism[0].counter);
        }
        //Wenn der Autor bereits im tracker ist, dann muss an dessen Stelle nach dem jeweiligen Anglizismus gesucht werden, und gegebenefalls eingefuegt oder der counter erhoeht werden.
        else{
            //Checkt ob der Anglizismus bereits in struct uebergeben wurde und wenn ja wo er gefunden werden kann
            for(int j=0; j < dist_aut[position_in_struct_aut - 1].stacktracker ;j++){ 
                if(strcmp(dist_aut[position_in_struct_aut - 1].anglizism[j].anglizism, all_anglis.anglizismen[i].anglizism) == 0){
                    position_in_struct_angl = j + 1;
                }
            }
            //Wenn der Anglizimus noch nicht im tracker ist, wird er dem tracker hinzugefuegt mit counter 1
            if (position_in_struct_angl == 0){
                //Zuerst pruefen ob genuegend Platz fuer weiteren Eintrag ist und wenn nicht dann reallocaten!
                dist_aut[position_in_struct_aut - 1].stacktracker++;
                if(dist_aut[position_in_struct_aut - 1].stacktracker > dist_aut[position_in_struct_aut - 1].malloctracker){
                    dist_aut[position_in_struct_aut - 1].malloctracker *= 2;
                    dist_aut[position_in_struct_aut - 1].anglizism = realloc(dist_aut[position_in_struct_aut - 1].anglizism, sizeof(distinct_anglis) * dist_aut[position_in_struct_aut - 1].malloctracker);
                    if (dist_aut[position_in_struct_aut - 1].anglizism == NULL){
                        printf("Fehler beim Reallocaten von Speicherplatz in sort_by_author für Anglizismuseintrag. Speicherplatz konnte nicht erhöht werden!");
                        exit(EXIT_FAILURE);
                    }
                }
                //Uebergebe den Pointer zum Anglizismus und setzte Anglizismus-Counter auf 1.
                dist_aut[position_in_struct_aut-1].anglizism[dist_aut[position_in_struct_aut - 1].stacktracker - 1].anglizism = all_anglis.anglizismen[i].anglizism;
                dist_aut[position_in_struct_aut-1].anglizism[dist_aut[position_in_struct_aut - 1].stacktracker - 1].counter = 1;
                dist_aut[position_in_struct_aut-1].total_tracker++;
                all_angl++;
                //printf("Das Autor %s ist bereits bekannt! Der Anglizismus %s ist jedoch neu und wird deshalb erst mal auf %d gesetzt\n", dist_aut[position_in_struct_aut-1].category, dist_aut[position_in_struct_aut-1].anglizism[dist_aut[position_in_struct_aut - 1].stacktracker - 1].anglizism, dist_aut[position_in_struct_aut-1].anglizism[dist_aut[position_in_struct_aut - 1].stacktracker - 1].counter);
                position_in_struct_aut=0;
            }
            //Wenn der Anglizimus bereits im tracker ist, wird der counter um 1 erhoeht.
            else{
                dist_aut[position_in_struct_aut-1].anglizism[position_in_struct_angl - 1].counter++;
                dist_aut[position_in_struct_aut-1].total_tracker++;
                all_angl++;
                //printf("Der Autor %s ist bereits bekannt! Der Anglizismus %s auch, dieser ist hiermit schon %d mal vorgekommen\n", dist_aut[position_in_struct_aut-1].category, dist_aut[position_in_struct_aut-1].anglizism[position_in_struct_angl - 1].anglizism, dist_aut[position_in_struct_aut-1].anglizism[position_in_struct_angl - 1].counter);
                position_in_struct_aut=0;
                position_in_struct_angl=0;
            }
        }
    }
    //uebergibt die Daten in ein Textdokument via filewriter
    FILE *filePointer = fopen(path, "w");
    fprintf (filePointer, "Es wurden insgesamt %d unterschiedliche Autoren festgestellt!\nDie Gesamtmenge aller %d Anglizismen teilt sich wie folgt auf die folgenden Autoren auf:\n\n", stacktracker_aut, all_angl);
    int word_count=0;
    double anteil=0.0;
    double anteil_divers=0.0;
    double mittelwert=0.0;
    for(int i= 0; i < stacktracker_aut ; i++){
        //Schaut zum passenden Autor nach wie viele Wörter dazu insgesamt gefunden werden konnten (Hoffentlich sprengt das den int max wert nicht)
        for(int x = 0; x < all_words_in_author_X.anzahl; x++){
            if(strcmp(all_words_in_author_X.set[x].author, dist_aut[i].category) == 0){
                word_count = all_words_in_author_X.set[x].word_count;
            }
        }
        if(word_count==0){
            printf("Fehler beim Suchen von Autor in sort_by_author_and_anglizism für Count_word. %dter gefundener Autor \"%s\" kann nicht keine Wörter enthalten, jedoch ist word_count == 0", i, dist_aut[i].category);
            exit(EXIT_FAILURE);
        }
        //prozentualer Anteil aller anglizismen
        anteil = ((double) dist_aut[i].total_tracker / (double) word_count) * 100;
        //prozentualer Anteil der unterschiedlichen Angilzismen
        anteil_divers = ((double) dist_aut[i].stacktracker / (double) word_count) * 100;
        //Mittelwert aus beiden
        mittelwert = (anteil + anteil_divers)/2.0;
        //prints info
        fprintf(filePointer, "\nDer Autor [%s] nutzt in allen vorhandenen Texten %d verschiedene Anglizismen und im Gesammten nutzt er %d Anglizismen!\nInsgesamt wurden in allen Texten von diesem Autor %d Wörter gefunden.\nSomit ergibt sich ein prozentualer Anteil von %.2f%% an Anglizismen und ein Anteil von %.2f%% unterschiedlichen Anglizismen im Korpus für diesen Autor.\nDer Mittelwert der beiden prozentualen Werte ist dabei: %.2f%%.\nDiese Anglizismen sind:\n", dist_aut[i].category, dist_aut[i].stacktracker, dist_aut[i].total_tracker, word_count, anteil, anteil_divers, mittelwert);
        for(int j=0; j < dist_aut[i].stacktracker ;j++){
            fprintf(filePointer, "[%s] kommt %d mal vor!\n", dist_aut[i].anglizism[j].anglizism, dist_aut[i].anglizism[j].counter);
        }
        free(dist_aut[i].anglizism);
    }
    fclose(filePointer);
    free(dist_aut);
}

//Sortiert die Eintraege nach den Jahren und schreibt Jahre mit ihren genutzten Anglizismen (zusammen mit der Gesamtanzahl der Anglizismen) in eine File
void sort_by_year_and_anglizism(All_Anglizisms all_anglis, char* path, All_Words_Per_Year_Set all_words_in_year_X){
    int malloctracker_year = 500; 
    int stacktracker_year = 0; 
    int position_in_struct_year=0; //steht nicht fuer position im array, sondern fuer laenge (also pos+1), da 0 fuer "nicht gefunden" reserviert bleibt
    int position_in_struct_angl=0; //siehe oben
    int all_angl = 0;
    distinct_category* dist_year = malloc (sizeof(distinct_category) * malloctracker_year);
    for(int i=0; i<all_anglis.amount_of_anglizisms; i++){
        //Checkt ob das Jahr bereits in struct uebergeben wurde und wenn ja wo es gefunden werden kann
        for(int j=0; j < stacktracker_year ;j++){ 
            if(strcmp(dist_year[j].category, all_anglis.anglizismen[i].year) == 0){
                position_in_struct_year = j + 1;
                break;
            }
        }
        //Wenn das Jahr noch nicht im tracker ist, wird es dem tracker hinzugefuegt und da der neue Eintrag definitiv noch keine Anglizismen hat kann sofort ein Anglizismus eingetragen werden auf counter 1
        if (position_in_struct_year == 0){
            //Zuerst pruefen ob genuegend Platz fuer weiteren Eintrag ist und wenn nicht dann reallocaten!
            stacktracker_year++;
            if(stacktracker_year > malloctracker_year){
                malloctracker_year *= 2;
                dist_year = realloc(dist_year, sizeof(distinct_category) * malloctracker_year);
                if (dist_year == NULL){
                    printf("Fehler beim Reallocaten von Speicherplatz in sort_by_year für Jahreseintrag. Speicherplatz konnte nicht erhöht werden!");
                    exit(EXIT_FAILURE);
                }
            }
            //Initialisiere Jahr mit Startwerten und dem jeweiligen Anglizismus
            dist_year[stacktracker_year-1].malloctracker=100;
            dist_year[stacktracker_year-1].stacktracker=1;
            dist_year[stacktracker_year-1].anglizism = malloc (sizeof(distinct_anglis) * dist_year[stacktracker_year-1].malloctracker);
            if(dist_year[stacktracker_year-1].anglizism == NULL){
                //printf("Distinct_Year Space! There was too little memory left to find space for a Distinct_Year or the Distinct_Year doesnt exist at all! Needed space was (%d) chars", malloctracker_year);
                exit(EXIT_FAILURE);
            }
            dist_year[stacktracker_year-1].category = all_anglis.anglizismen[i].year;
            dist_year[stacktracker_year-1].anglizism[0].anglizism = all_anglis.anglizismen[i].anglizism;
            dist_year[stacktracker_year-1].anglizism[0].counter = 1;
            dist_year[stacktracker_year-1].total_tracker = 1;
            all_angl++;
            //printf("Das Jahr %s ist neu! Der Anglizismus %s wurde mit dem Counter %d direkt eingefuegt\n", dist_year[stacktracker_year-1].category, dist_year[stacktracker_year-1].anglizism[0].anglizism, dist_year[stacktracker_year-1].anglizism[0].counter);
        }
        //Wenn das Jahr bereits im tracker ist, dann muss an dessen Stelle nach dem jeweiligen Anglizismus gesucht werden, und gegebenefalls eingefuegt oder der counter erhoeht werden.
        else{
            //Checkt ob der Anglizismus bereits in struct uebergeben wurde und wenn ja wo er gefunden werden kann
            for(int j=0; j < dist_year[position_in_struct_year - 1].stacktracker ;j++){ 
                if(strcmp(dist_year[position_in_struct_year - 1].anglizism[j].anglizism, all_anglis.anglizismen[i].anglizism) == 0){
                    position_in_struct_angl = j + 1;
                }
            }
            //Wenn der Anglizimus noch nicht im tracker ist, wird er dem tracker hinzugefuegt mit counter 1
            if (position_in_struct_angl == 0){
                //Zuerst pruefen ob genuegend Platz fuer weiteren Eintrag ist und wenn nicht dann reallocaten!
                dist_year[position_in_struct_year - 1].stacktracker++;
                if(dist_year[position_in_struct_year - 1].stacktracker > dist_year[position_in_struct_year - 1].malloctracker){
                    dist_year[position_in_struct_year - 1].malloctracker *= 2;
                    dist_year[position_in_struct_year - 1].anglizism = realloc(dist_year[position_in_struct_year - 1].anglizism, sizeof(distinct_anglis) * dist_year[position_in_struct_year - 1].malloctracker);
                    if (dist_year[position_in_struct_year - 1].anglizism == NULL){
                        printf("Fehler beim Reallocaten von Speicherplatz in sort_by_year für Anglizismuseintrag. Speicherplatz konnte nicht erhöht werden!");
                        exit(EXIT_FAILURE);
                    }
                }
                //Uebergebe den Pointer zum Anglizismus und setzte Anglizismus-Counter auf 1.
                dist_year[position_in_struct_year-1].anglizism[dist_year[position_in_struct_year - 1].stacktracker - 1].anglizism = all_anglis.anglizismen[i].anglizism;
                dist_year[position_in_struct_year-1].anglizism[dist_year[position_in_struct_year - 1].stacktracker - 1].counter = 1;
                dist_year[position_in_struct_year-1].total_tracker++;
                all_angl++;
                //printf("Das Jahr %s ist bereits bekannt! Der Anglizismus %s ist jedoch neu und wird deshalb erst mal auf %d gesetzt\n", dist_year[position_in_struct_year-1].category, dist_year[position_in_struct_year-1].anglizism[dist_year[position_in_struct_year - 1].stacktracker - 1].anglizism, dist_year[position_in_struct_year-1].anglizism[dist_year[position_in_struct_year - 1].stacktracker - 1].counter);
                position_in_struct_year=0;
            }
            //Wenn der Anglizimus bereits im tracker ist, wird der counter um 1 erhoeht.
            else{
                dist_year[position_in_struct_year-1].anglizism[position_in_struct_angl - 1].counter++;
                dist_year[position_in_struct_year-1].total_tracker++;
                all_angl++;
                //printf("Das Jahr %s ist bereits bekannt! Der Anglizismus %s auch, dieser ist hiermit schon %d mal vorgekommen\n", dist_year[position_in_struct_year-1].category, dist_year[position_in_struct_year-1].anglizism[position_in_struct_angl - 1].anglizism, dist_year[position_in_struct_year-1].anglizism[position_in_struct_angl - 1].counter);
                position_in_struct_year=0;
                position_in_struct_angl=0;
            }
        }
    }
    //uebergibt die Daten in ein Textdokument via filewriter
    FILE *filePointer = fopen(path, "w");
    fprintf (filePointer, "Es wurden insgesamt %d unterschiedliche Jahreszahlen festgestellt!\nDie Gesamtmenge aller %d Anglizismen teilt sich wie folgt auf die folgenden Jahre auf:\n\n", stacktracker_year, all_angl);
    int word_count=0;
    double anteil=0.0;
    double anteil_divers=0.0;
    double mittelwert=0.0;
    for(int i= 0; i < stacktracker_year ; i++){
        //Schaut zum passenden Jahr nach wie viele Wörter dazu insgesamt gefunden werden konnten (Hoffentlich sprengt das den int max wert nicht)
        for(int x = 0; x < all_words_in_year_X.anzahl; x++){
            if(strcmp(all_words_in_year_X.set[x].year, dist_year[i].category) == 0){
                word_count = all_words_in_year_X.set[x].word_count;
            }
        }
        if(word_count==0){
            printf("Fehler beim Suchen von Jahr in sort_by_year_and_anglizism für Count_word. Gefundenes Jahr kann nicht keine Wörter enthalten, jedoch ist word_count == 0");
            exit(EXIT_FAILURE);
        }
        //prozentualer Anteil aller anglizismen
        anteil = ((double) dist_year[i].total_tracker / (double) word_count) * 100;
        //prozentualer Anteil der unterschiedlichen Angilzismen
        anteil_divers = ((double) dist_year[i].stacktracker / (double) word_count) * 100;
        //Mittelwert aus beiden
        mittelwert = (anteil + anteil_divers)/2.0;
        //prints info
        fprintf(filePointer, "\nDas Jahr [%s] nutzt in allen vorhandenen Texten %d verschiedene Anglizismen und im Gesammten nutzt es %d Anglizismen!\nInsgesamt wurden in allen Texten dieses Jahres %d Wörter gefunden.\nSomit ergibt sich ein prozentualer Anteil von %.2f%% an Anglizismen und ein Anteil von %.2f%% unterschiedlichen Anglizismen im Korpus für dieses Jahr.\nDer Mittelwert der beiden prozentualen Werte ist dabei: %.2f%%.\nDiese Anglizismen sind:\n", dist_year[i].category, dist_year[i].stacktracker, dist_year[i].total_tracker, word_count, anteil, anteil_divers, mittelwert);
        for(int j=0; j < dist_year[i].stacktracker ;j++){
            fprintf(filePointer, "[%s] kommt %d mal vor!\n", dist_year[i].anglizism[j].anglizism, dist_year[i].anglizism[j].counter);
        }
        free(dist_year[i].anglizism);
    }
    fclose(filePointer);
    free(dist_year);
}

//Sortiert die Eintraege nach den Listen und schreibt Listen mit ihren genutzten Anglizismen (zusammen mit der Gesamtanzahl der Anglizismen) in eine File
void sort_by_anglizismlist_and_anglizism(All_Anglizisms all_anglis, char* path){
    int malloctracker_list = 500; 
    int stacktracker_list = 0; 
    int position_in_struct_list=0; //steht nicht fuer position im array, sondern fuer laenge (also pos+1), da 0 fuer "nicht gefunden" reserviert bleibt
    int position_in_struct_angl=0; //siehe oben
    int all_angl = 0;
    distinct_category* dist_list = malloc (sizeof(distinct_category) * malloctracker_list);
    for(int i=0; i<all_anglis.amount_of_anglizisms; i++){
        //Checkt ob die Liste bereits in struct uebergeben wurde und wenn ja wo sie gefunden werden kann
        for(int j=0; j < stacktracker_list ;j++){ 
            if(strcmp(dist_list[j].category, all_anglis.anglizismen[i].list) == 0){
                position_in_struct_list = j + 1;
                break;
            }
        }
        //Wenn die Liste noch nicht im tracker ist, wird sie dem tracker hinzugefuegt und da der neue Eintrag definitiv noch keine Anglizismen hat kann sofort ein Anglizismus eingetragen werden auf counter 1
        if (position_in_struct_list == 0){
            //Zuerst pruefen ob genuegend Platz fuer weiteren Eintrag ist und wenn nicht dann reallocaten!
            stacktracker_list++;
            if(stacktracker_list > malloctracker_list){
                malloctracker_list *= 2;
                dist_list = realloc(dist_list, sizeof(distinct_category) * malloctracker_list);
                if (dist_list == NULL){
                    printf("Fehler beim Reallocaten von Speicherplatz in sort_by_anglizismlist für Listeneintrag. Speicherplatz konnte nicht erhöht werden!");
                    exit(EXIT_FAILURE);
                }
            }
            //Initialisiere Jahr mit Startwerten und dem jeweiligen Anglizismus
            dist_list[stacktracker_list-1].malloctracker=100;
            dist_list[stacktracker_list-1].stacktracker=1;
            dist_list[stacktracker_list-1].anglizism = malloc (sizeof(distinct_anglis) * dist_list[stacktracker_list-1].malloctracker);
            if(dist_list[stacktracker_list-1].anglizism == NULL){
                //printf("Distinct_List Space! There was too little memory left to find space for a Distinct_List or the Distinct_List doesnt exist at all! Needed space was (%d) chars", malloctracker_list);
                exit(EXIT_FAILURE);
            }
            dist_list[stacktracker_list-1].category = all_anglis.anglizismen[i].list;
            dist_list[stacktracker_list-1].anglizism[0].anglizism = all_anglis.anglizismen[i].anglizism;
            dist_list[stacktracker_list-1].anglizism[0].counter = 1;
            dist_list[stacktracker_list-1].total_tracker = 1;
            all_angl++;
            //printf("die Liste %s ist neu! Der Anglizismus %s wurde mit dem Counter %d direkt eingefuegt\n", dist_list[stacktracker_list-1].category, dist_list[stacktracker_list-1].anglizism[0].anglizism, dist_list[stacktracker_list-1].anglizism[0].counter);
        }
        //Wenn die Liste bereits im tracker ist, dann muss an dessen Stelle nach dem jeweiligen Anglizismus gesucht werden, und gegebenefalls eingefuegt oder der counter erhoeht werden.
        else{
            //Checkt ob der Anglizismus bereits in struct uebergeben wurde und wenn ja wo er gefunden werden kann
            for(int j=0; j < dist_list[position_in_struct_list - 1].stacktracker ;j++){ 
                if(strcmp(dist_list[position_in_struct_list - 1].anglizism[j].anglizism, all_anglis.anglizismen[i].anglizism) == 0){
                    position_in_struct_angl = j + 1;
                }
            }
            //Wenn der Anglizimus noch nicht im tracker ist, wird er dem tracker hinzugefuegt mit counter 1
            if (position_in_struct_angl == 0){
                //Zuerst pruefen ob genuegend Platz fuer weiteren Eintrag ist und wenn nicht dann reallocaten!
                dist_list[position_in_struct_list - 1].stacktracker++;
                if(dist_list[position_in_struct_list - 1].stacktracker > dist_list[position_in_struct_list - 1].malloctracker){
                    dist_list[position_in_struct_list - 1].malloctracker *= 2;
                    dist_list[position_in_struct_list - 1].anglizism = realloc(dist_list[position_in_struct_list - 1].anglizism, sizeof(distinct_anglis) * dist_list[position_in_struct_list - 1].malloctracker);
                    if (dist_list[position_in_struct_list - 1].anglizism == NULL){
                        printf("Fehler beim Reallocaten von Speicherplatz in sort_by_anglizismlist für Anglizismuseintrag. Speicherplatz konnte nicht erhöht werden!");
                        exit(EXIT_FAILURE);
                    }
                }
                //Uebergebe den Pointer zum Anglizismus und setzte Anglizismus-Counter auf 1.
                dist_list[position_in_struct_list-1].anglizism[dist_list[position_in_struct_list - 1].stacktracker - 1].anglizism = all_anglis.anglizismen[i].anglizism;
                dist_list[position_in_struct_list-1].anglizism[dist_list[position_in_struct_list - 1].stacktracker - 1].counter = 1;
                dist_list[position_in_struct_list-1].total_tracker++;
                all_angl++;
                //printf("die Liste %s ist bereits bekannt! Der Anglizismus %s ist jedoch neu und wird deshalb erst mal auf %d gesetzt\n", dist_list[position_in_struct_list-1].category, dist_list[position_in_struct_list-1].anglizism[dist_list[position_in_struct_list - 1].stacktracker - 1].anglizism, dist_list[position_in_struct_list-1].anglizism[dist_list[position_in_struct_list - 1].stacktracker - 1].counter);
                position_in_struct_list=0;
            }
            //Wenn der Anglizimus bereits im tracker ist, wird der counter um 1 erhoeht.
            else{
                dist_list[position_in_struct_list-1].anglizism[position_in_struct_angl - 1].counter++;
                dist_list[position_in_struct_list-1].total_tracker++;
                all_angl++;
                //printf("die Liste %s ist bereits bekannt! Der Anglizismus %s auch, dieser ist hiermit schon %d mal vorgekommen\n", dist_list[position_in_struct_list-1].category, dist_list[position_in_struct_list-1].anglizism[position_in_struct_angl - 1].anglizism, dist_list[position_in_struct_list-1].anglizism[position_in_struct_angl - 1].counter);
                position_in_struct_list=0;
                position_in_struct_angl=0;
            }
        }
    }
    //uebergibt die Daten in ein Textdokument via filewriter
    FILE *filePointer = fopen(path, "w");
    fprintf (filePointer, "Es wurden insgesamt %d unterschiedliche Anglizismenlisten festgestellt!\nDie Gesamtmenge aller %d Anglizismen teilt sich wie folgt auf die folgenden Listen auf:\n\n", stacktracker_list, all_angl);
    for(int i= 0; i < stacktracker_list ; i++){
        fprintf(filePointer, "\nDie Liste [%s] findet in allen vorhandenen Texten %d verschiedene Anglizismen und im Gesammten findet sie %d Anglizismen!\nDiese Anglizismen sind:\n", dist_list[i].category, dist_list[i].stacktracker, dist_list[i].total_tracker);
        for(int j=0; j < dist_list[i].stacktracker ;j++){
            fprintf(filePointer, "[%s] kommt %d mal vor!\n", dist_list[i].anglizism[j].anglizism, dist_list[i].anglizism[j].counter);
        }
        free(dist_list[i].anglizism);
    }
    fclose(filePointer);
    free(dist_list);
}

//Sortiert die Eintraege nach den Titeln und schreibt Titel mit ihren genutzten Anglizismen (zusammen mit der Gesamtanzahl der Anglizismen) in eine File
void sort_by_titel_and_anglizism(All_Anglizisms all_anglis, char* path){
    int malloctracker_list = 500; 
    int stacktracker_list = 0; 
    int position_in_struct_list=0; //steht nicht fuer position im array, sondern fuer laenge (also pos+1), da 0 fuer "nicht gefunden" reserviert bleibt
    int position_in_struct_angl=0; //siehe oben
    int all_angl = 0;
    distinct_category* dist_list = malloc (sizeof(distinct_category) * malloctracker_list);
    for(int i=0; i<all_anglis.amount_of_anglizisms; i++){
        //Checkt ob die Liste bereits in struct uebergeben wurde und wenn ja wo sie gefunden werden kann
        for(int j=0; j < stacktracker_list ;j++){ 
            if(strcmp(dist_list[j].category, all_anglis.anglizismen[i].title) == 0){
                position_in_struct_list = j + 1;
                break;
            }
        }
        //Wenn der Titel noch nicht im tracker ist, wird er dem tracker hinzugefuegt und da der neue Eintrag definitiv noch keine Anglizismen hat kann sofort ein Anglizismus eingetragen werden auf counter 1
        if (position_in_struct_list == 0){
            //Zuerst pruefen ob genuegend Platz fuer weiteren Eintrag ist und wenn nicht dann reallocaten!
            stacktracker_list++;
            if(stacktracker_list > malloctracker_list){
                malloctracker_list *= 2;
                dist_list = realloc(dist_list, sizeof(distinct_category) * malloctracker_list);
                if (dist_list == NULL){
                    printf("Fehler beim Reallocaten von Speicherplatz in sort_by_anglizismlist für Listeneintrag. Speicherplatz konnte nicht erhöht werden!");
                    exit(EXIT_FAILURE);
                }
            }
            //Initialisiere Titel mit Startwerten und dem jeweiligen Anglizismus
            dist_list[stacktracker_list-1].malloctracker=100;
            dist_list[stacktracker_list-1].stacktracker=1;
            dist_list[stacktracker_list-1].anglizism = malloc (sizeof(distinct_anglis) * dist_list[stacktracker_list-1].malloctracker);
            if(dist_list[stacktracker_list-1].anglizism == NULL){
                //printf("Distinct_Titel Space! There was too little memory left to find space for a Distinct_Titel or the Distinct_Titel doesnt exist at all! Needed space was (%d) chars", malloctracker_list);
                exit(EXIT_FAILURE);
            }
            dist_list[stacktracker_list-1].category = all_anglis.anglizismen[i].title;
            dist_list[stacktracker_list-1].anglizism[0].anglizism = all_anglis.anglizismen[i].anglizism;
            dist_list[stacktracker_list-1].anglizism[0].counter = 1;
            dist_list[stacktracker_list-1].total_tracker = 1;
            all_angl++;
            //printf("Der Titel %s ist neu! Der Anglizismus %s wurde mit dem Counter %d direkt eingefuegt\n", dist_list[stacktracker_list-1].category, dist_list[stacktracker_list-1].anglizism[0].anglizism, dist_list[stacktracker_list-1].anglizism[0].counter);
        }
        //Wenn der Titel bereits im tracker ist, dann muss an dessen Stelle nach dem jeweiligen Anglizismus gesucht werden, und gegebenefalls eingefuegt oder der counter erhoeht werden.
        else{
            //Checkt ob der Anglizismus bereits in struct uebergeben wurde und wenn ja wo er gefunden werden kann
            for(int j=0; j < dist_list[position_in_struct_list - 1].stacktracker ;j++){ 
                if(strcmp(dist_list[position_in_struct_list - 1].anglizism[j].anglizism, all_anglis.anglizismen[i].anglizism) == 0){
                    position_in_struct_angl = j + 1;
                }
            }
            //Wenn der Anglizimus noch nicht im tracker ist, wird er dem tracker hinzugefuegt mit counter 1
            if (position_in_struct_angl == 0){
                //Zuerst pruefen ob genuegend Platz fuer weiteren Eintrag ist und wenn nicht dann reallocaten!
                dist_list[position_in_struct_list - 1].stacktracker++;
                if(dist_list[position_in_struct_list - 1].stacktracker > dist_list[position_in_struct_list - 1].malloctracker){
                    dist_list[position_in_struct_list - 1].malloctracker *= 2;
                    dist_list[position_in_struct_list - 1].anglizism = realloc(dist_list[position_in_struct_list - 1].anglizism, sizeof(distinct_anglis) * dist_list[position_in_struct_list - 1].malloctracker);
                    if (dist_list[position_in_struct_list - 1].anglizism == NULL){
                        printf("Fehler beim Reallocaten von Speicherplatz in sort_by_anglizismlist für Anglizismuseintrag. Speicherplatz konnte nicht erhöht werden!");
                        exit(EXIT_FAILURE);
                    }
                }
                //Uebergebe den Pointer zum Anglizismus und setzte Anglizismus-Counter auf 1.
                dist_list[position_in_struct_list-1].anglizism[dist_list[position_in_struct_list - 1].stacktracker - 1].anglizism = all_anglis.anglizismen[i].anglizism;
                dist_list[position_in_struct_list-1].anglizism[dist_list[position_in_struct_list - 1].stacktracker - 1].counter = 1;
                dist_list[position_in_struct_list-1].total_tracker++;
                all_angl++;
                //printf("Der Titel %s ist bereits bekannt! Der Anglizismus %s ist jedoch neu und wird deshalb erst mal auf %d gesetzt\n", dist_list[position_in_struct_list-1].category, dist_list[position_in_struct_list-1].anglizism[dist_list[position_in_struct_list - 1].stacktracker - 1].anglizism, dist_list[position_in_struct_list-1].anglizism[dist_list[position_in_struct_list - 1].stacktracker - 1].counter);
                position_in_struct_list=0;
            }
            //Wenn der Anglizimus bereits im tracker ist, wird der counter um 1 erhoeht.
            else{
                dist_list[position_in_struct_list-1].anglizism[position_in_struct_angl - 1].counter++;
                dist_list[position_in_struct_list-1].total_tracker++;
                all_angl++;
                //printf("Der Titel %s ist bereits bekannt! Der Anglizismus %s auch, dieser ist hiermit schon %d mal vorgekommen\n", dist_list[position_in_struct_list-1].category, dist_list[position_in_struct_list-1].anglizism[position_in_struct_angl - 1].anglizism, dist_list[position_in_struct_list-1].anglizism[position_in_struct_angl - 1].counter);
                position_in_struct_list=0;
                position_in_struct_angl=0;
            }
        }
    }
    //uebergibt die Daten in ein Textdokument via filewriter
    FILE *filePointer = fopen(path, "w");
    fprintf (filePointer, "Es wurden insgesamt %d unterschiedliche Titel festgestellt!\nDie Gesamtmenge aller %d Anglizismen teilt sich wie folgt auf die folgenden Titel auf:\n\n", stacktracker_list, all_angl);
    for(int i= 0; i < stacktracker_list ; i++){
        fprintf(filePointer, "\nDer Titel [%s] findet in allen vorhandenen Texten %d verschiedene Anglizismen und im Gesammten findet sie %d Anglizismen!\nDiese Anglizismen sind:\n", dist_list[i].category, dist_list[i].stacktracker, dist_list[i].total_tracker);
        for(int j=0; j < dist_list[i].stacktracker ;j++){
            fprintf(filePointer, "[%s] kommt %d mal vor!\n", dist_list[i].anglizism[j].anglizism, dist_list[i].anglizism[j].counter);
        }
        free(dist_list[i].anglizism);
    }
    fclose(filePointer);
    free(dist_list);
}

//Ermittelt f-Maß
void calculate_f_score(distinct_anglis* ermittelt, char* liste_manuell_path, int length_ermittelt, int gefundene_angl){
    //Reads in selfmade List and puts values in structs
    char* liste_manuell = filereader(liste_manuell_path);
    int stacktracker = 0;
    int malloctracker = 300;
    char* word_temp = malloc (sizeof(char) * 100); 
    manuelle_list* richtwert = malloc (sizeof(manuelle_list) * malloctracker);
    int pos = 0;
    int zu_findene_angl = 0;
    while(strstr(liste_manuell, "Anglizismus:") != NULL){
        stacktracker++;
        if(stacktracker > malloctracker){
            malloctracker *= 2;
            richtwert = realloc (richtwert, sizeof(manuelle_list) * malloctracker);
            if(richtwert == NULL){
                printf("Fehler in calculate_f_score. Nicht genügend Speicherplatz!");
                exit(EXIT_FAILURE);
            }
        }
        //sets Word
        liste_manuell = strstr(liste_manuell, "Anglizismus:");
        liste_manuell = liste_manuell + 12;
        strcpy(word_temp, "");
        for(int i = 0; liste_manuell[i] != '|'; i++){
            strncat(word_temp, &liste_manuell[i], 1);
        }
        richtwert[pos].word = malloc (sizeof(char) * strlen(word_temp));
        strcpy(richtwert[pos].word, word_temp);
        //sets counter
        liste_manuell = strstr(liste_manuell, "|Counter:");
        liste_manuell = liste_manuell + 9;
        strcpy(word_temp, "");
        for(int i = 0; liste_manuell[i] != '|'; i++){
            strncat(word_temp, &liste_manuell[i], 1);
        }
        //printf("%s\n", word_temp);
        int count;
        char* useless;
        count = strtol(word_temp, &useless, 10);
        richtwert[pos].word_count = count;
        zu_findene_angl = zu_findene_angl + count;
        pos++;
    }
    /*
    for(int i=0; i< stacktracker; i++){
    printf("%s kommt %d oft vor\n", richtwert[i].word, richtwert[i].word_count);  
    }
    */
    //Ermittelt true positives
    int true_positives = 0;
    for(int i=0; i< stacktracker; i++){
        for(int j=0; j < length_ermittelt; j++){
            if(strcmp(richtwert[i].word, ermittelt[j].anglizism) == 0){
                //Nimmt sich min wert, da nicht mehr als gewollt ein true positiv sein kann.
                if(richtwert[i].word_count < ermittelt[j].counter){
                    true_positives = true_positives + richtwert[i].word_count;
                }
                else{
                    true_positives = true_positives + ermittelt[j].counter; 
                }
            }
        }
    }
    //printf("Es wurden %d true positives gefunden!\n", true_positives);
    //Ermittelt Listenpräsision mit truepositives / alle automatisch gefundenen Anglizismen
    double Listenpraezision = ((double) true_positives / (double) gefundene_angl) * 100;
    //Ermittelt Listenabdeckung mit truepositives / alle zu findenen Anglizismen
    double Listenabdeckung = ((double) true_positives / (double) zu_findene_angl) * 100;
    //Ermittelt F-Maß mit 2*((Listenpraezision*Listenabdeckung)/(Listenpraezision+Listenabdeckung))
    double F_mas = 2 * ((Listenpraezision*Listenabdeckung)/(Listenpraezision+Listenabdeckung));
    //prints it in a file
    FILE *filePointer = fopen("out\\Trefferquote.txt","w");
    if(filePointer == NULL) {
        printf("File kann nicht geöffnet werden (um Anglizismenliste roh zu schreiben)\n");
        exit(EXIT_FAILURE);
    }
    fprintf(filePointer, "Es wurden insgesamt %d Anglizismen automatisch entdeckt. Es sollten optimalerweise %d Anglizismen gefunden werden\nDabei waren von den %d automatisch entdeckten Anglizismen %d True-Positives!\nDamit gelangen wir zu einer Listenpräzision von %.2f%% und einer Listenabdeckung von %.2f%%!\nSomit hat dieser Durchlauf ein F-Maß von %.2f%% erzielt!\n", gefundene_angl, zu_findene_angl, gefundene_angl, true_positives, Listenpraezision, Listenabdeckung, F_mas);
    fclose(filePointer);
}
