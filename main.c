/*-------------------------------------------
Program 5: Find the Shortest Word Ladder
Course: CS 211, Fall 2023, UIC
System: Advanced zyLab
Author: Zeehan Sharif
------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

typedef struct WordNode_struct {
    char* myWord;//pointer to the word
    struct WordNode_struct* next; //pointer to the next word
} WordNode;

typedef struct LadderNode_struct {
    WordNode* topWord;//pointer to the word
    struct LadderNode_struct* next;//pointer to the next word
} LadderNode;

//This counts the number of words of a specific length (wordSize) in a file (filename)
int countWordsOfLength(char* filename, int wordSize) { 
    
    FILE *file = fopen(filename, "r");

    //check for file opening error
    if(file == NULL){
        return -1;
    }

    int count = 0;
    int ch = 0;
    int len = 0;

    while(EOF != (ch=getc(file))) {//loop through file until the end
        
        if(!(isspace(ch))){ //if character is not a space, increment len
            len++;
        }
        else {
            if(len == wordSize){//if the length of word equals wordSize, then increment count
                count++;
            }

            len = 0;
        }
    }

    if(len == wordSize){//repeat the check in case EOF is reached without a space
        count++;
    }

    fclose(file);
    return count;
}

// The buildWordArray function reads words from a file and stores them into a provided array
bool buildWordArray(char* filename, char** words, int numWords, int wordSize) { 
    
    char buffer[1024];//temp buffer to store buffers from file
    int index = 0;
    int wscheck = countWordsOfLength(filename, wordSize);//check ad count all the words in the file of wordSize

    if(wscheck < 0 || wscheck != numWords){//check if the num of words of correct length are expected
        return false;
    }
    
    FILE *file = fopen(filename, "r");
    if(file == NULL){//check for file opening error
        return false;
    }

    while(fgets(buffer, 1024, file)){//read each line into buffer

        int length = 0;
        for(int i = 0; (buffer[i] != '\0') && buffer[i] != '\n'; i++){
            length++;
        }

        buffer[length] = '\0';//null terminate the buffer after using it

        if(length == wordSize){// If the word in the buffer is of correct length, copy it to the words array
            strcpy(words[index++], buffer);
        }
    }

    fclose(file);
    return true;
}

/* This function returns the index of a word in the words array. If the word is not in the array, it returns -1 */
int findWord(char** words, char* aWord, int loInd, int hiInd) { 
    
    while(loInd <= hiInd){//loop until low index is greater than high index

        int mid = loInd + (hiInd - loInd) / 2;
        int check = strcmp(aWord, words[mid]);

        if(check == 0){//if the words match, return index
            return mid;
        }
        else if(check > 0){// If the word is lexographically larger, increment the low index
            loInd++;
        }
        else {// If the word is lexographically smaller, decrement the high index
            hiInd--;
        }
    }

    return -1;
}

//Free memory allocated for word Array
void freeWords(char** words, int numWords) {
    
    for(int i = 0; i < numWords; i++){//loop through array
        free(words[i]);
    }

    free(words);
}

//This function inserts a word at the front of the word ladder
void insertWordAtFront(WordNode** ladder, char* newWord) {
    
    WordNode* newWordNode = (WordNode*)malloc(sizeof(WordNode));//allocate mem for new word node

    newWordNode->myWord = newWord;//assign new word to element of new word node
    newWordNode->next = (*ladder);// Make the new word node's next point to the previous first word node
    *ladder = newWordNode;
}

/*This function returns the height of a ladder. The height of the ladder is the count of words in it*/
int getLadderHeight(WordNode* ladder) {
    
    int height = 0;

    while(ladder != NULL){
        height++;//increment height for each word
        ladder = ladder->next;
    }

    return height;
}

WordNode* copyLadder(WordNode* ladder) {
    
    if(ladder == NULL) return NULL;

    WordNode* newLadder = (WordNode*)malloc(sizeof(WordNode));//allocates new mem for new ladder
    WordNode* check = newLadder;

    newLadder->myWord = ladder->myWord;//assign word from old ladder to new one
    newLadder->next = NULL;

    while(ladder->next != NULL){//loop until end of old ladder

        WordNode* nextNode = (WordNode*)malloc(sizeof(WordNode));
        nextNode->myWord = ladder->next->myWord;
        nextNode->next = NULL;

        newLadder->next = nextNode;//assign newly created node

        ladder = ladder->next;
        newLadder = newLadder->next;
    }

    return check;
}

//free memory occupied by ladder node
void freeLadder(WordNode* ladder) {
    
    while(ladder != NULL){
        WordNode* temp = ladder->next;
        free(ladder);//frees current node
        ladder = temp;
    }
}

  // This function inserts a given ladder at the back of a list of ladders.
void insertLadderAtBack(LadderNode** list, WordNode* newLadder) {
    
    LadderNode* newLadderNode = (LadderNode*)malloc(sizeof(LadderNode));
    newLadderNode->topWord = newLadder;
    newLadderNode->next = NULL;

    if(*list == NULL){//if list is empty, insert as first element
        *list = newLadderNode;
        return;
    }

    LadderNode* current = *list;

    while(current->next){//find last node in list
        current = current->next;
    }

    current->next = newLadderNode;//add new ladder to the end
}

// This function pops a ladder from the front of a list of ladders.
WordNode* popLadderFromFront(LadderNode** list) {
    
    LadderNode* temp = (*list)->next;
    WordNode* returnNode = (*list)->topWord;
    free(*list);
    *list = temp;//second element becomes new front of list

    return returnNode;
}

//free up allocated mem for myList node
void freeLadderList(LadderNode* myList) {
    
    while(myList != NULL){
        freeLadder(myList->topWord);
        LadderNode* temp = myList->next;
        free(myList);//free mem from current ladder node
        myList = temp;
    }
}

//This function checks the difference between two strings
int checkBesides(char* str1, char* str2){

    char* p1 = str1;
    char* p2 = str2;
    int diff = 0;

    while((*p1 != '\0') && *p2 != '\0'){
        diff += (*p1 != *p2);//increment diff if characters of both strings are not equal
        p1++; p2++;
    }

    while(*p1 != '\0'){//loop to increment diff for the remainder of the longer string
        p1++;
        diff++;
    }

    while(*p2 != '\0'){//loop to increment diff for the remainder of the longer string
        p2++;
        diff++;
    }

    return diff;
}

/*This function finds the shortest ladder of words starting from a start word and aiming for a final word. A word ladder is a sequence of words made by changing one letter at a time*/
WordNode* findShortestWordLadder(   char** words, 
                                    bool* usedWord, 
                                    int numWords, 
                                    int wordSize, 
                                    char* startWord, 
                                    char* finalWord ) {
    
    LadderNode* myList = NULL;
    WordNode* myLadder = NULL;

    insertWordAtFront(&myLadder, startWord);
    insertLadderAtBack(&myList, myLadder);

    while(myList != NULL){//while there is still ladders to process

        myLadder = popLadderFromFront(&myList);

        for(int i = 0; i < numWords; i++){//loop through all words

            if(checkBesides(words[i], myLadder->myWord) != 1 || usedWord[i]) {
                continue;//skip if words are not close or if word has already been used
            }

            //if we found the final word, we construct the final ladder and free the others
            if(strcmp(words[i], finalWord) == 0){
                insertWordAtFront(&myLadder, words[i]);
                freeLadderList(myList);
                return myLadder;//return completed ladder
            }
            else{
                WordNode* anotherLadder = copyLadder(myLadder);
                insertWordAtFront(&anotherLadder, words[i]);
                insertLadderAtBack(&myList, anotherLadder);//else create a new ladder and add it to the list
                usedWord[i] = true;//mark current word as used
            }
        }

        freeLadder(myLadder);
    }

    return NULL;
}

// interactive user-input to set a word; 
// ensures sure the word is in the dictionary word array
void setWord(char** words, int numWords, int wordSize, char* aWord) {
    bool valid = false;
    printf("  Enter a %d-letter word: ", wordSize);
    int count = 0;
    while (!valid) {
        scanf("%s",aWord);
        count++;
        valid = (strlen(aWord) == wordSize);
        if (valid) {
            int wordInd = findWord(words, aWord, 0, numWords-1);
            if (wordInd < 0) {
                valid = false;
                printf("    Entered word %s is not in the dictionary.\n",aWord);
                printf("  Enter a %d-letter word: ", wordSize); 
            }
        } else {
            printf("    Entered word %s is not a valid %d-letter word.\n",aWord,wordSize);
            printf("  Enter a %d-letter word: ", wordSize);
        }
        if (!valid && count >= 5) { //too many tries, picking random word
            printf("\n");
            printf("  Picking a random word for you...\n");
            strcpy(aWord,words[rand()%numWords]);
            printf("  Your word is: %s\n",aWord);
            valid = true;
        }
    }
}

// helpful debugging function to print a single Ladder
void printLadder(WordNode* ladder) {
    WordNode* currNode = ladder;
    while (currNode != NULL) {
        printf("\t\t\t%s\n",currNode->myWord);
        currNode = currNode->next;
    }
}

// helpful debugging function to print the entire list of Ladders
void printList(LadderNode* list) {
    printf("\n");
    printf("Printing the full list of ladders:\n");
    LadderNode* currList = list;
    while (currList != NULL) {
        printf("  Printing a ladder:\n");
        printLadder(currList->topWord);
        currList = currList->next;
    }
    printf("\n");
}

//-----------------------------------------------------
// The primary application is fully provided in main(); 
//  no changes should be made to main()
//-----------------------------------------------------
int main() {
    srand((int)time(0));
    
    printf("\nWelcome to the CS 211 Word Ladder Generator!\n\n");

    // set word length using interactive user-input
    int wordSize;
    printf("Enter the word size for the ladder: ");
    scanf("%d",&wordSize);
    printf("\n");

    printf("This program will make the shortest possible\n"); 
    printf("word ladder between two %d-letter words.\n\n",wordSize);
    
    // interactive user-input sets the dictionary file;
    //  check that file exists; if not, user enters another file
    //  if file exists, count #words of desired length [wordSize]
    char dict[100];
    printf("Enter filename for dictionary: ");
    scanf("%s", dict);
    printf("\n");
    int numWords = countWordsOfLength(dict,wordSize);
    while (numWords < 0) {
        printf("  Dictionary %s not found...\n",dict);
        printf("Enter filename for dictionary: ");
        scanf("%s", dict);
        printf("\n");
        numWords = countWordsOfLength(dict,wordSize);
    }
    
    // allocate heap memory for the word array; only words with desired length
    char** words = (char**)malloc(numWords*sizeof(char*));
    for (int i = 0; i < numWords; ++i) {
        words[i] = (char*)malloc((wordSize+1)*sizeof(char));
    }

    // end program if file does not have at least two words of desired length
    if (numWords < 2) {
        printf("  Dictionary %s contains insufficient %d-letter words...\n",dict,wordSize);
        printf("Terminating program...\n");
        return -1;
    }
    
    // [usedWord] bool array has same size as word array [words];
    //  all elements initialized to [false];
    //  later, usedWord[i] will be set to [true] whenever 
    //      words[i] is added to ANY partial word ladder;
    //      before adding words[i] to another word ladder,
    //      check for previous usage with usedWord[i] 
    bool* usedWord = (bool*)malloc(numWords*sizeof(bool));
    for (int i = 0; i < numWords; ++i) {
        usedWord[i] = false;
    }
    
    // build word array (only words with desired length) from dictionary file
    printf("Building array of %d-letter words... ", wordSize); 
    bool status = buildWordArray(dict,words,numWords,wordSize);
    if (!status) {
        printf("  ERROR in building word array.\n");
        printf("  File not found or incorrect number of %d-letter words.\n",wordSize);
        printf("Terminating program...\n");
        return -1;
    }
    printf("Done!\n"); 

    // set the two ends of the word ladder using interactive user-input
    //  make sure start and final words are in the word array, 
    //  have the correct length (implicit by checking word array), AND
    //  that the two words are not the same
    char startWord[30];
    char finalWord[30];
    printf("Setting the start %d-letter word... \n", wordSize);
    setWord(words, numWords, wordSize, startWord);
    printf("\n");
    printf("Setting the final %d-letter word... \n", wordSize);
    setWord(words, numWords, wordSize, finalWord);
    while (strcmp(finalWord,startWord) == 0) {
        printf("  The final word cannot be the same as the start word (%s).\n",startWord);
        printf("Setting the final %d-letter word... \n", wordSize);
        setWord(words, numWords, wordSize, finalWord);
    }
    printf("\n");

    // run the algorithm to find the shortest word ladder
    WordNode* myLadder = findShortestWordLadder(words, usedWord, numWords, wordSize, startWord, finalWord);

    // display word ladder and its height if one was found
    if (myLadder == NULL) {
        printf("There is no possible word ladder from %s to %s\n",startWord,finalWord);    
    } else {
        printf("Shortest Word Ladder found!\n");
        printLadder(myLadder);
    }
    printf("Word Ladder height = %d\n",getLadderHeight(myLadder));
    
    // free the heap-allocated memory for the shortest ladder
    freeLadder(myLadder);
    // free the heap-allocated memory for the words array
    freeWords(words,numWords);
    free(usedWord);
    
    return 0;
}
