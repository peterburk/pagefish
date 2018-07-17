/*
cc -lm geonamesToIndex.c -o geonamesToIndex
cat geonames_relations.txt | ./geonamesToIndex

Benchmark for indexing:

> time cat geonames_relations.txt | ./geonamesToIndex

real	0m17.185s
user	0m2.395s
sys	0m13.544s

> wc -l geonames_relations.txt 
  576084 geonames_relations.txt


*/

#include <stdio.h>	 /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <stdlib.h>	 /* Standard library */
#include <wchar.h>	 /* UTF-8 library */

#define XML_LINE 10000	  /* Length of a string */


/*
 * textBetween: Gets text between two delimiters
 */
char* textBetween(char* thisText, char* startText, char* endText, char* returnText)
{
	//printf("textBetween\n");
 
    char* startPointer = NULL;
    int stringLength = 0;
 
    char* endPointer = NULL;
    int endLength = 0;
 
	if (strstr(startText, "start") != NULL)
	{
		// Set the beginning of the string
		startPointer = thisText;
	} else {
		startPointer = strstr(thisText, startText);
 
    	if (startPointer != NULL)
	    {
        	startPointer = startPointer + strlen(startText);
        }
	} // end if the start delimiter is "start"
 
    if (startPointer != NULL)
    {
 
		if (strstr(endText, "end") != NULL)
		{
			// Set the end of the string
			endPointer = thisText;
			endLength = 0;
		} else {
			endPointer = strstr(startPointer, endText);
			endLength = (int)strlen(endPointer);
		} // end if the end delimiter is "end"
 
        stringLength = strlen(startPointer) - endLength;
 
        if (stringLength == 0)
        {
		    returnText = "";
		    //startPointer = NULL;
        } else {
	        // Copy characters between the start and end delimiters
    	    strncpy(returnText,startPointer, stringLength);
	        returnText[stringLength++] = '\0';
		}
 
    } else {
	    //printf("Start pointer not found\n");
	    returnText = "";
 
    } // end if the start pointer is not found
	
	startPointer = startPointer + stringLength;
	
    return startPointer;
} // end textBetween method


int main(int argc, char** argv) 
{
    wchar_t unicodeStringArray[16];
    wchar_t* unicodeString = unicodeStringArray;

    char* tsvBuffer;
	char tsvBufferArray[XML_LINE]; /* Line buffer for reading OSM file */	
	tsvBuffer = tsvBufferArray;
	
    char* startPointer = NULL;

    char* startText;
    char startTextArray[XML_LINE];

    char* endText;
    char endTextArray[XML_LINE];

    char* thisName;
    char thisNameArray[XML_LINE];
	
	int thisNameLength = 0;
	int thisNameHash = 0;
	
    char* alternativeNames;
    char alternativeNamesArray[XML_LINE];

    char* osmId;
    char osmIdArray[XML_LINE];

    char* latText;
    char latTextArray[XML_LINE];

    char* lonText;
    char lonTextArray[XML_LINE];

    int nodeId = 0;
    double latitude = 0.0;
    double longitude = 0.0;

    char* outputLine = NULL;
    char outputLineArray[XML_LINE];
	outputLine = outputLineArray;

	int characterTableLength = 65536;
	int characterTableFileSize = 260000;
	int unicodeCharacterLength = 3;

	// Read the character table
	char characterTable[characterTableLength][unicodeCharacterLength + 1];
	char oneCharacter;
	
	FILE *thisIndexFile;

    char* thisIndexFileName;
    char thisIndexFileNameArray[XML_LINE];
    thisIndexFileName = thisIndexFileNameArray;

    char* characterTableBuffer;
	char characterTableBufferArray[characterTableFileSize];
	characterTableBuffer = characterTableBufferArray;

    char* characterTableString;
	char characterTableStringArray[characterTableFileSize];
	characterTableString = characterTableStringArray;
	
	
    char* thisCharacter;
    char thisCharacterArray[XML_LINE];
    thisCharacter = thisCharacterArray;
    
    char* thisCharacterTableLine;

	FILE *characterFile;

	if (!(characterFile = fopen("characterTable.txt", "r")))
	{
		fprintf(stderr, "could not open %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	
	int currentCharacterLine = 0;
	int currentCharacterInLine = 0;
	
	char* thisCharacterLine = characterTable[currentCharacterLine];
	
	// If the file is not already read
	if (strlen(characterTableString) == 0)
	{
		// Read a line from the file.
		while (fgets(characterTableBuffer, XML_LINE, characterFile) != NULL)
		{
			// Append it to the library string
			strcat(characterTableString, characterTableBuffer);
		}
	}

	fclose(characterFile);
	
	// Tokenise the characters
	thisCharacter = strtok (characterTableString,"\n");
	 
	while (thisCharacter != NULL)
	{
		//sprintf(outputLine, "%s %lu\n", thisCharacter, strlen(thisCharacter));
    	//fprintf( stdout, "%s", outputLine); fflush(stdout);
	 	
	 	thisCharacterTableLine = characterTable[currentCharacterLine];
	 	
		strncpy(thisCharacterTableLine, thisCharacter, strlen(thisCharacter));
		
		currentCharacterLine = currentCharacterLine + 1;
	 	
		// Get the next token
		thisCharacter = strtok (NULL,"\n");
	}
	
    //unsigned char iobuf[255] = {0};
    //while( fgets((char*)iobuf, sizeof(iobuf), characterFile) )
    //{
    //        size_t len = strlen((char *)iobuf);
    //        if(len > 1 &&  iobuf[len-1] == '\n')
    //        {
    //            iobuf[len-1] = 0;
    //        }
            
    //        len = strlen((char *)iobuf);
            
            //printf("(%d) \"%s\"  ", (int)len, iobuf);
            
            
    //        thisCharacterLine = characterTable[currentCharacterLine];
	//	    sprintf(thisCharacterLine, "%s", iobuf);

    //        currentCharacterLine = currentCharacterLine + 1;
            
            //if( iobuf[0] == '\n' )
            //    printf("Yes\n");
            //else
            //    printf("No\n");
    //}
	 
	//thisCharacterTableLine = characterTable[2244];
	//sprintf(outputLine, "%s\n", thisCharacterTableLine);
	//fprintf( stdout, "%s", outputLine); fflush(stdout);

	
	//while (fread(&oneCharacter, sizeof(char), 1, characterFile))
	//{
	//	if (oneCharacter != '\n')
	//	{
	//		characterTable[currentCharacterLine][currentCharacterInLine] = oneCharacter;
	//		currentCharacterInLine = currentCharacterInLine + 1;
	//		
	//	} else {
	//		characterTable[currentCharacterLine][currentCharacterInLine] = '\0';
	//		
	//		currentCharacterLine = currentCharacterLine + 1;
	//		currentCharacterInLine = 0;
	//	}
	//}

	
    // Read a line from stdin.
    while (fgets(tsvBuffer, XML_LINE, stdin) != NULL)
    {
		// Reset the variables
	    startText = startTextArray;
	    endText = endTextArray;
	    thisName = thisNameArray;
	    alternativeNames = alternativeNamesArray;
		osmId = osmIdArray;
		latText = latTextArray;
		lonText = lonTextArray;
		thisNameLength = 0;
		thisNameHash = 0;
		unicodeString = unicodeStringArray;
	
	    thisIndexFileName = thisIndexFileNameArray;
        
        // Step 1: Read fields from the TSV line
        
        
        // Name (first field)
        startText = "start";
        endText = "	";

        startPointer = textBetween(tsvBuffer, startText, endText, thisName);
		
        if (startPointer != NULL)
        {
	        //sprintf(outputLine, "%s\n", thisName);
    	    //fprintf( stdout, "%s", outputLine); fflush(stdout);
        }
        
        // Alternative names (second field)
        startText = "start";
        endText = "	";

        startPointer = textBetween(startPointer, startText, endText, alternativeNames);
		
        if (startPointer != NULL)
        {
	        //sprintf(outputLine, "%s\n", alternativeNames);
    	    //fprintf( stdout, "%s", outputLine); fflush(stdout);
        }

		// Skip the third field (osm_type)
		startPointer = strstr(startPointer, "	") + 1;

        // osm_id (fourth field)
        startText = "start";
        endText = "	";

        startPointer = textBetween(startPointer, startText, endText, osmId);
		
        if (startPointer != NULL)
        {
	        //sprintf(outputLine, "%s\n", osmId);
    	    //fprintf( stdout, "%s", outputLine); fflush(stdout);
        }

		// Skip the fifth field (class)
		startPointer = strstr(startPointer, "	") + 1;

		// Skip the sixth field (type)
		startPointer = strstr(startPointer, "	") + 1;

        // lat (seventh field)
        startText = "start";
        endText = "	";

        startPointer = textBetween(startPointer, startText, endText, latText);
		
        if (startPointer != NULL)
        {
	        //sprintf(outputLine, "%s\n", latText);
    	    //fprintf( stdout, "%s", outputLine); fflush(stdout);
        }

        // lon (eighth field)
        startText = "start";
        endText = "	";

        startPointer = textBetween(startPointer, startText, endText, lonText);
		
        if (startPointer != NULL)
        {
	        //sprintf(outputLine, "%s\n", lonText);
    	    //fprintf( stdout, "%s", outputLine); fflush(stdout);
        }
        
        
        // Step 2: Hash the names
        // This algorithm sums the integer value of every character in the string to get a hash,
        // then looks up that character in the Unicode table (only the first 65536). 
        
        thisNameLength = strlen(thisName);
        
	    //sprintf(outputLine, "%d\n", thisNameLength);
    	//fprintf( stdout, "%s", outputLine); fflush(stdout);
        
        thisNameHash = 0;
        
		for (int currentCharacter = 0; currentCharacter < thisNameLength; currentCharacter++)
		{
			thisNameHash = thisNameHash + thisName[currentCharacter];
		} // end for every character in the name

		thisNameHash = thisNameHash % characterTableLength;
		
	    //sprintf(outputLine, "%s hashes to characterTable[%d] = %s\n", thisName, thisNameHash, characterTable[thisNameHash]);
    	//fprintf( stdout, "%s", outputLine); fflush(stdout);
    	
	    //sprintf(outputLine, "%lu\n", strlen(characterTable[2048]));
    	//fprintf( stdout, "%s", outputLine); fflush(stdout);
    	
    	
	    sprintf(thisIndexFileName, "GeoNamesIndex/%s.txt", characterTable[thisNameHash]);

	    //sprintf(outputLine, "%d\n", thisNameHash);
    	//fprintf( stdout, "%s", outputLine); fflush(stdout);
		

    	
	    sprintf(outputLine, "%s	%s	%s	%s\n", thisName, osmId, latText, lonText);
    	//fprintf( stdout, "%s", outputLine); fflush(stdout);
    	
    	thisIndexFile=fopen(thisIndexFileName, "a");

	    if (thisIndexFile != NULL)
    	{
        	fwrite(outputLine, sizeof(char), strlen(outputLine), thisIndexFile);
	    }

	    fclose(thisIndexFile);
		

    } // end while read lines from stdin
    
} // end main