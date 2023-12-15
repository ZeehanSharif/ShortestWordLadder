# ShortestWordLadder
This is a fun little game, where the program centers on an algorithm that finds the shortest word ladder that connects two English words. The algorithm involves organizing a linked list of partially complete word ladder, which are themselves linked lists of words.

A word ladder is a bridge between one word and another, formed by changing one letter at a time, with the constraint that at each step the sequence of letters still forms a valid word. For example, here is a word ladder starting from the word "data" and climbing up the ladder to the word "code". Each changed letter is bolded as an illustration:

|  code  |
|    ^   |
|  cove  |  
|   ^    | 
|  cave  |
|  ^     |
|  gave  |
|    ^   |
|  gate  |
|  ^     |
|  date  |
|     ^  |
|  data  |
This word ladder has height 7. There are many other word ladders that connect these two words. For example, here is another ladder of height 5 that uses some more obscure words:

|  code  |
|   ^    |
|  cade  |  
|    ^   | 
|  cate  |
|  ^     |
|  date  |
|     ^  |
|  data  |
In fact, this ladder has the shortest height that connects data to code. Note that there might be other ladders of the same height, but none with fewer rungs than this one. Notice that the shortest a word ladder can be is 2, e.g. cove -> code. There are also pairs of words for which a word ladder cannot be formed, e.g. there is no word ladder to connect stack -> queue.

In this project, you will write functions to support a program that prompts the user for two words and finds a minimum-height ladder linking the words, which uses linked lists of words to store ladders, linked lists of partially completed ladders to organize all possible solutions, and a prescribed algorithm to find the shortest word ladder.
