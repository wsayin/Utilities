/******************************************************************************
*** Name: c2html                                                            ***
*** Purpose: Format C files into a syntax suitable for a website.           ***
*** Copyright: None                                                         ***
*** Date: 20 SEPT 09                                                        ***
*** Author: William Sayin                                                   ***
*** Webpage: https://github.com/wsayin                                   ***
******************************************************************************/


#include <stdio.h>
#include <stdlib.h>

char usage[] = "Usage:\n"
               " c2html <flag> <argument> ...\n"
               " Flags:\n"
               " \t-i\tInput file.\n"
               " \t-o\tOutput file.\n"
               " \t-t\tTabspace, convert tabs to specified number of spaces.\n"
               " \t-n\tAdd line numbers to output file.\n";

int main( int numArgs, char *argList[] )
{
    FILE *ifp;       /*Input file--AKA the C file to be turned into an html file*/
    FILE *ofp;       /*Output file--AKA the HTML file that is soon to be created*/
    char *input;
    char *output;
    char c;
    char number = 0; /*Are we numbering the lines? 0 = NO 1 = YES*/
    int tab = 0;
    int i = 1;       /*The line number*/
    int j = 0;

    if ( numArgs < 3 )
    {
        fprintf( stderr, "%s\n", usage );
        return 1;
    }

    for( i = 0; i < numArgs; i++ )
    {
        if( !strcmp( argList[i], "-n" ) )
            number = 1;
        if( !strcmp( argList[i], "-t" ) && i != numArgs )
            tab = atoi( argList[i+1] );
        if( !strcmp( argList[i], "-i" ) && i != numArgs )
            input = argList[i+1];
        if( !strcmp( argList[i], "-o" ) && i != numArgs )
            output = argList[i+1];
    }

    i = 1;


    if ( ( ifp = fopen( input, "r" ) ) == NULL )
    {
        fprintf( stderr, "Error: Unable to open %s for reading. Aborting.\n", argList[1] );
        return 1;
    }

    if ( ( ofp = fopen( output, "w" ) ) == NULL )
    {
        fprintf( stderr, "Error: Unable to open %s for writing. Aborting.\n", argList[2] );
        return 1;
    }
    /*Add the needed opening stuff, and a little message from me.*/
    fprintf( ofp, "<!--Created using c2html programmed by William Sayin\nhttps://github.com/wsayin--!>\n<html>\n<head>\n<title>%s</title>\n</head>\n<body>\n<code>\n", input );

    /*If line numbering is on, then start printing line numbers!*/
    if ( number )
    {
        fprintf( ofp, "%i: ", i );
        i++;
    }

    while ( ( c = fgetc( ifp ) ) != EOF )
    {
        /*These are the only characters that need to be changed IAW HTML ISO-8859-1*/
        switch ( c )
        {
        case '\"':
            fprintf( ofp, "&quot;" );
            break;
        case '\'':
            fprintf( ofp, "&apos;" );
            break;
        case '&':
            fprintf( ofp, "&amp;" );
            break;
        case '<':
            fprintf( ofp, "&lt;" );
            break;
        case '>':
            fprintf( ofp, "&gt;" );
            break;
        case ' ':
            fprintf( ofp, "&nbsp;" );
            break;
        case '\t':
            if( tab != 0 )
            {
                for( j = 0; j < tab; j++ )
                    fprintf( ofp, "&nbsp;" );
            }
            else
            {
                fprintf( ofp, "&nbsp;&nbsp;&nbsp;&nbsp;" ); /*Default to 4 space tabs*/
            }
            break; 
        case '\n':
        {
            if ( number )
            {
                fprintf( ofp, "<br>\n%i: ", i++ ); /*If line numbering is on we need to
                                                     remember to number the lines!*/
            }
            else
            {
                fprintf( ofp, "<br>\n" );
            }
            break;
        }
        default:
            fprintf( ofp, "%c", c );
            break;
        }
    }

    /*Print the ending html codes*/
    fprintf( ofp, "</code>\n</body>\n</html>\n" );

    /*close all the streams*/
    fclose( ofp );
    fclose( ifp );

    return 0;
}
