/* Generated by re2c 0.9.10 on Mon Sep 19 23:21:26 2005 */
#line 1 "bytecode.re"
/*
 * bytecode.re
 *
 * $Author: shyouhei $
 * $Date: 2007-02-12 16:01:19 -0700 (Mon, 12 Feb 2007) $
 *
 * Copyright (C) 2003 why the lucky stiff
 */
#include "ruby.h"
#include "syck.h"
#include "gram.h"

#define QUOTELEN 128

/*
 * They do my bidding...
 */
#define YYCTYPE     char
#define YYCURSOR    parser->cursor
#define YYMARKER    parser->marker
#define YYLIMIT     parser->limit
#define YYTOKEN     parser->token
#define YYTOKTMP    parser->toktmp
#define YYLINEPTR   parser->lineptr
#define YYLINECTPTR parser->linectptr
#define YYLINE      parser->linect
#define YYFILL(n)   syck_parser_read(parser)

extern SyckParser *syck_parser_ptr;

char *get_inline( SyckParser *parser );

/*
 * Repositions the cursor at `n' offset from the token start.
 * Only works in `Header' and `Document' sections.
 */
#define YYPOS(n)    YYCURSOR = YYTOKEN + n

/*
 * Track line numbers
 */
#define CHK_NL(ptr)    if ( *( ptr - 1 ) == '\n' && ptr > YYLINECTPTR ) { YYLINEPTR = ptr; YYLINE++; YYLINECTPTR = YYLINEPTR; }

/*
 * I like seeing the level operations as macros...
 */
#define ADD_LEVEL(len, status)  syck_parser_add_level( parser, len, status )
#define POP_LEVEL()     syck_parser_pop_level( parser )
#define CURRENT_LEVEL() syck_parser_current_level( parser )

/*
 * Force a token next time around sycklex()
 */
#define FORCE_NEXT_TOKEN(tok)    parser->force_token = tok;

/*
 * Adding levels in bytecode requires us to make sure
 * we've got all our tokens worked out.
 */
#define ADD_BYTE_LEVEL(lvl, len, s ) \
        switch ( lvl->status ) \
        { \
            case syck_lvl_seq: \
                lvl->ncount++; \
                ADD_LEVEL(len, syck_lvl_open); \
                YYPOS(0); \
            return '-'; \
        \
            case syck_lvl_map: \
                lvl->ncount++; \
                ADD_LEVEL(len, s); \
            break; \
        \
            case syck_lvl_open: \
                lvl->status = s; \
            break; \
        \
            default: \
                ADD_LEVEL(len, s); \
            break; \
        }

/*
 * Nice little macro to ensure we're YAML_IOPENed to the current level.
 * * Only use this macro in the "Document" section *
 */
#define ENSURE_YAML_IOPEN(last_lvl, lvl_type, to_len, reset) \
        if ( last_lvl->spaces < to_len ) \
        { \
            if ( last_lvl->status == syck_lvl_iseq || last_lvl->status == syck_lvl_imap ) \
            { \
                goto Document; \
            } \
            else \
            { \
                ADD_LEVEL( to_len, lvl_type ); \
                if ( reset == 1 ) YYPOS(0); \
                return YAML_IOPEN; \
            } \
        } 

/*
 * Nice little macro to ensure closure of levels.
 * * Only use this macro in the "Document" section *
 */
#define ENSURE_YAML_IEND(last_lvl, to_len) \
        if ( last_lvl->spaces > to_len ) \
        { \
            syck_parser_pop_level( parser ); \
            YYPOS(0); \
            return YAML_IEND; \
        }

/*
 * Concatenates string items and manages allocation
 * to the string
 */
#define CAT(s, c, i, l) \
        { \
            if ( i + 1 >= c ) \
            { \
                c += QUOTELEN; \
                S_REALLOC_N( s, char, c ); \
            } \
            s[i++] = l; \
            s[i] = '\0'; \
        }

/*
 * Parser for standard YAML Bytecode [UTF-8]
 */
int
sycklex_bytecode_utf8( YYSTYPE *sycklval, SyckParser *parser )
{
    SyckLevel *lvl;
    syck_parser_ptr = parser;
    if ( YYCURSOR == NULL ) 
    {
        syck_parser_read( parser );
    }

    if ( parser->force_token != 0 )
    {
        int t = parser->force_token;
        parser->force_token = 0;
        return t;
    }

#line 172 "bytecode.re"


    lvl = CURRENT_LEVEL();
    if ( lvl->status == syck_lvl_doc )
    {
        goto Document;
    }

/* Header: */

    YYTOKEN = YYCURSOR;


#line 165 "<stdout>"
{
	YYCTYPE yych;
	unsigned int yyaccept;
	goto yy0;
	++YYCURSOR;
yy0:
	if((YYLIMIT - YYCURSOR) < 3) YYFILL(3);
	yych = *YYCURSOR;
	switch(yych){
	case 0x00:	goto yy2;
	case 'D':	goto yy3;
	default:	goto yy5;
	}
yy2:	YYCURSOR = YYMARKER;
	switch(yyaccept){
	case 0:	goto yy4;
	}
yy3:	yyaccept = 0;
	yych = *(YYMARKER = ++YYCURSOR);
	switch(yych){
	case 0x0A:	goto yy6;
	case 0x0D:	goto yy8;
	default:	goto yy4;
	}
yy4:
#line 199 "bytecode.re"
{   YYPOS(0);
            goto Document;
        }
#line 195 "<stdout>"
yy5:	yych = *++YYCURSOR;
	goto yy4;
yy6:	++YYCURSOR;
	goto yy7;
yy7:
#line 186 "bytecode.re"
{   if ( lvl->status == syck_lvl_header )
            {
                CHK_NL(YYCURSOR);
                goto Directive;
            }
            else
            {
                ENSURE_YAML_IEND(lvl, -1);
                YYPOS(0);
                return 0;
            }
        }
#line 214 "<stdout>"
yy8:	++YYCURSOR;
	switch((yych = *YYCURSOR)) {
	case 0x0A:	goto yy6;
	default:	goto yy2;
	}
}
#line 203 "bytecode.re"


Document:
    {
        lvl = CURRENT_LEVEL();
        if ( lvl->status == syck_lvl_header )
        {
            lvl->status = syck_lvl_doc;
        }

        YYTOKEN = YYCURSOR;


#line 235 "<stdout>"
{
	YYCTYPE yych;
	goto yy9;
	++YYCURSOR;
yy9:
	if((YYLIMIT - YYCURSOR) < 3) YYFILL(3);
	yych = *YYCURSOR;
	switch(yych){
	case 0x00:	goto yy30;
	case 0x0A:	goto yy27;
	case 0x0D:	goto yy29;
	case 'A':	goto yy19;
	case 'D':	goto yy12;
	case 'E':	goto yy16;
	case 'M':	goto yy14;
	case 'P':	goto yy13;
	case 'Q':	goto yy15;
	case 'R':	goto yy21;
	case 'S':	goto yy17;
	case 'T':	goto yy23;
	case 'c':	goto yy25;
	default:	goto yy11;
	}
yy11:yy12:	yych = *++YYCURSOR;
	switch(yych){
	case 0x0A:	goto yy41;
	case 0x0D:	goto yy44;
	default:	goto yy11;
	}
yy13:	yych = *++YYCURSOR;
	switch(yych){
	case 0x0A:	goto yy41;
	case 0x0D:	goto yy43;
	default:	goto yy11;
	}
yy14:	yych = *++YYCURSOR;
	switch(yych){
	case 0x0A:	goto yy38;
	case 0x0D:	goto yy40;
	default:	goto yy11;
	}
yy15:	yych = *++YYCURSOR;
	switch(yych){
	case 0x0A:	goto yy35;
	case 0x0D:	goto yy37;
	default:	goto yy11;
	}
yy16:	yych = *++YYCURSOR;
	switch(yych){
	case 0x0A:	goto yy32;
	case 0x0D:	goto yy34;
	default:	goto yy11;
	}
yy17:	++YYCURSOR;
	goto yy18;
yy18:
#line 288 "bytecode.re"
{   ADD_BYTE_LEVEL(lvl, lvl->spaces + 1, syck_lvl_str); 
            goto Scalar;
        }
#line 296 "<stdout>"
yy19:	++YYCURSOR;
	goto yy20;
yy20:
#line 292 "bytecode.re"
{   ADD_BYTE_LEVEL(lvl, lvl->spaces + 1, syck_lvl_open);
            sycklval->name = get_inline( parser );
            syck_hdlr_remove_anchor( parser, sycklval->name );
            CHK_NL(YYCURSOR);
            return YAML_ANCHOR;
        }
#line 307 "<stdout>"
yy21:	++YYCURSOR;
	goto yy22;
yy22:
#line 299 "bytecode.re"
{   ADD_BYTE_LEVEL(lvl, lvl->spaces + 1, syck_lvl_str);
            sycklval->name = get_inline( parser );
            POP_LEVEL();
            if ( *( YYCURSOR - 1 ) == '\n' ) YYCURSOR--;
            return YAML_ALIAS;
        }
#line 318 "<stdout>"
yy23:	++YYCURSOR;
	goto yy24;
yy24:
#line 306 "bytecode.re"
{   char *qstr;
            ADD_BYTE_LEVEL(lvl, lvl->spaces + 1, syck_lvl_open);
            qstr = get_inline( parser );
            CHK_NL(YYCURSOR);
            if ( qstr[0] == '!' )
            {
                int qidx = strlen( qstr );
                if ( qstr[1] == '\0' )
                {
                    free( qstr );
                    return YAML_ITRANSFER;
                }

                lvl = CURRENT_LEVEL();

                /*
                 * URL Prefixing
                 */
                if ( qstr[1] == '^' )
                {
                    sycklval->name = S_ALLOC_N( char, qidx + strlen( lvl->domain ) );
                    sycklval->name[0] = '\0';
                    strcat( sycklval->name, lvl->domain );
                    strncat( sycklval->name, qstr + 2, qidx - 2 );
                    free( qstr );
                }
                else
                {
                    char *carat = qstr + 1;
                    char *qend = qstr + qidx;
                    while ( (++carat) < qend )
                    {
                        if ( *carat == '^' )
                            break;
                    }

                    if ( carat < qend )
                    {
                        free( lvl->domain );
                        lvl->domain = syck_strndup( qstr + 1, carat - ( qstr + 1 ) );
                        sycklval->name = S_ALLOC_N( char, ( qend - carat ) + strlen( lvl->domain ) );
                        sycklval->name[0] = '\0';
                        strcat( sycklval->name, lvl->domain );
                        strncat( sycklval->name, carat + 1, ( qend - carat ) - 1 );
                        free( qstr );
                    }
                    else
                    {
                        sycklval->name = S_ALLOC_N( char, strlen( qstr ) );
                        sycklval->name[0] = '\0';
                        S_MEMCPY( sycklval->name, qstr + 1, char, strlen( qstr ) );
                        free( qstr );
                    }
                }
                return YAML_TRANSFER;
            }
            sycklval->name = qstr;
            return YAML_TAGURI;
        }
#line 382 "<stdout>"
yy25:	++YYCURSOR;
	goto yy26;
yy26:
#line 366 "bytecode.re"
{   goto Comment; }
#line 388 "<stdout>"
yy27:	++YYCURSOR;
	goto yy28;
yy28:
#line 368 "bytecode.re"
{   CHK_NL(YYCURSOR);
            if ( lvl->status == syck_lvl_seq )
            {
                return YAML_INDENT; 
            }
            else if ( lvl->status == syck_lvl_map )
            {
                if ( lvl->ncount % 2 == 1 ) return ':';
                else                        return YAML_INDENT;
            }
            goto Document;
        }
#line 405 "<stdout>"
yy29:	yych = *++YYCURSOR;
	switch(yych){
	case 0x0A:	goto yy27;
	default:	goto yy11;
	}
yy30:	++YYCURSOR;
	goto yy31;
yy31:
#line 381 "bytecode.re"
{   ENSURE_YAML_IEND(lvl, -1);
            YYPOS(0);
            return 0;
        }
#line 419 "<stdout>"
yy32:	++YYCURSOR;
	goto yy33;
yy33:
#line 252 "bytecode.re"
{   if ( lvl->status == syck_lvl_seq && lvl->ncount == 0 )
            {
                lvl->ncount++;
                YYPOS(0);
                FORCE_NEXT_TOKEN( ']' );
                return '[';
            }
            else if ( lvl->status == syck_lvl_map && lvl->ncount == 0 )
            {
                lvl->ncount++;
                YYPOS(0);
                FORCE_NEXT_TOKEN( '}' );
                return '{';
            }
            
            POP_LEVEL();
            lvl = CURRENT_LEVEL();
            if ( lvl->status == syck_lvl_seq )
            {
                FORCE_NEXT_TOKEN(YAML_INDENT);   
            }
            else if ( lvl->status == syck_lvl_map )
            {
                if ( lvl->ncount % 2 == 1 )
                {
                    FORCE_NEXT_TOKEN(':');
                }
                else
                {
                    FORCE_NEXT_TOKEN(YAML_INDENT);
                }
            }
            CHK_NL(YYCURSOR);
            return YAML_IEND;
        }
#line 459 "<stdout>"
yy34:	yych = *++YYCURSOR;
	switch(yych){
	case 0x0A:	goto yy32;
	default:	goto yy11;
	}
yy35:	++YYCURSOR;
	goto yy36;
yy36:
#line 237 "bytecode.re"
{   int complex = 0;
            if ( lvl->ncount % 2 == 0 && ( lvl->status == syck_lvl_map || lvl->status == syck_lvl_seq ) )
            {
                complex = 1;
            }
            ADD_BYTE_LEVEL(lvl, lvl->spaces + 1, syck_lvl_seq);
            CHK_NL(YYCURSOR);
            if ( complex )
            {
                FORCE_NEXT_TOKEN( YAML_IOPEN );
                return '?';
            }
            return YAML_IOPEN;
        }
#line 483 "<stdout>"
yy37:	yych = *++YYCURSOR;
	switch(yych){
	case 0x0A:	goto yy35;
	default:	goto yy11;
	}
yy38:	++YYCURSOR;
	goto yy39;
yy39:
#line 222 "bytecode.re"
{   int complex = 0;
            if ( lvl->ncount % 2 == 0 && ( lvl->status == syck_lvl_map || lvl->status == syck_lvl_seq ) )
            {
                complex = 1;
            }
            ADD_BYTE_LEVEL(lvl, lvl->spaces + 1, syck_lvl_map); 
            CHK_NL(YYCURSOR);
            if ( complex )
            {
                FORCE_NEXT_TOKEN( YAML_IOPEN );
                return '?';
            }
            return YAML_IOPEN;
        }
#line 507 "<stdout>"
yy40:	yych = *++YYCURSOR;
	switch(yych){
	case 0x0A:	goto yy38;
	default:	goto yy11;
	}
yy41:	++YYCURSOR;
	goto yy42;
yy42:
#line 217 "bytecode.re"
{   ENSURE_YAML_IEND(lvl, -1);
                YYPOS(0);
                return 0;
            }
#line 521 "<stdout>"
yy43:	yych = *++YYCURSOR;
	switch(yych){
	case 0x0A:	goto yy41;
	default:	goto yy11;
	}
yy44:	++YYCURSOR;
	switch((yych = *YYCURSOR)) {
	case 0x0A:	goto yy41;
	default:	goto yy11;
	}
}
#line 386 "bytecode.re"


    }

Directive:
    {
        YYTOKEN = YYCURSOR;


#line 543 "<stdout>"
{
	YYCTYPE yych;
	unsigned int yyaccept;
	goto yy45;
	++YYCURSOR;
yy45:
	if((YYLIMIT - YYCURSOR) < 2) YYFILL(2);
	yych = *YYCURSOR;
	switch(yych){
	case 0x00:	goto yy47;
	case 'V':	goto yy48;
	default:	goto yy50;
	}
yy47:	YYCURSOR = YYMARKER;
	switch(yyaccept){
	case 0:	goto yy49;
	}
yy48:	yyaccept = 0;
	yych = *(YYMARKER = ++YYCURSOR);
	switch(yych){
	case '.':
	case '/':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case ':':
	case ';':
	case '<':
	case '=':
	case '>':
	case '?':
	case '@':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '[':
	case '\\':
	case ']':
	case '^':
	case '_':	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy51;
	default:	goto yy49;
	}
yy49:
#line 399 "bytecode.re"
{   YYCURSOR = YYTOKEN;
               return YAML_DOCSEP;
           }
#line 646 "<stdout>"
yy50:	yych = *++YYCURSOR;
	goto yy49;
yy51:	++YYCURSOR;
	if((YYLIMIT - YYCURSOR) < 2) YYFILL(2);
	yych = *YYCURSOR;
	goto yy52;
yy52:	switch(yych){
	case '.':
	case '/':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	case ';':
	case '<':
	case '=':
	case '>':
	case '?':
	case '@':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '[':
	case '\\':
	case ']':
	case '^':
	case '_':	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy51;
	case ':':	goto yy53;
	default:	goto yy47;
	}
yy53:	yych = *++YYCURSOR;
	switch(yych){
	case '.':
	case '/':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case ':':
	case ';':
	case '<':
	case '=':
	case '>':
	case '?':
	case '@':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '[':
	case '\\':
	case ']':
	case '^':
	case '_':	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy54;
	default:	goto yy47;
	}
yy54:	++YYCURSOR;
	if((YYLIMIT - YYCURSOR) < 2) YYFILL(2);
	yych = *YYCURSOR;
	goto yy55;
yy55:	switch(yych){
	case 0x0A:	goto yy56;
	case 0x0D:	goto yy58;
	case '.':
	case '/':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case ':':
	case ';':
	case '<':
	case '=':
	case '>':
	case '?':
	case '@':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '[':
	case '\\':
	case ']':
	case '^':
	case '_':	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy54;
	default:	goto yy47;
	}
yy56:	++YYCURSOR;
	goto yy57;
yy57:
#line 396 "bytecode.re"
{   CHK_NL(YYCURSOR);
               goto Directive; }
#line 899 "<stdout>"
yy58:	++YYCURSOR;
	switch((yych = *YYCURSOR)) {
	case 0x0A:	goto yy56;
	default:	goto yy47;
	}
}
#line 402 "bytecode.re"


    }

Comment:
    {
        YYTOKEN = YYCURSOR;


#line 916 "<stdout>"
{
	YYCTYPE yych;
	goto yy59;
	++YYCURSOR;
yy59:
	if((YYLIMIT - YYCURSOR) < 2) YYFILL(2);
	yych = *YYCURSOR;
	switch(yych){
	case 0x00:	goto yy61;
	case 0x0A:	goto yy62;
	case 0x0D:	goto yy64;
	default:	goto yy66;
	}
yy61:yy62:	++YYCURSOR;
	goto yy63;
yy63:
#line 412 "bytecode.re"
{   CHK_NL(YYCURSOR);
                goto Document; }
#line 936 "<stdout>"
yy64:	++YYCURSOR;
	switch((yych = *YYCURSOR)) {
	case 0x0A:	goto yy67;
	default:	goto yy65;
	}
yy65:
#line 415 "bytecode.re"
{   goto Comment; }
#line 945 "<stdout>"
yy66:	yych = *++YYCURSOR;
	goto yy65;
yy67:	++YYCURSOR;
	yych = *YYCURSOR;
	goto yy63;
}
#line 417 "bytecode.re"


    }

Scalar:
    {
    int idx = 0;
    int cap = 100;
    char *str = S_ALLOC_N( char, cap );
    char *tok;

    str[0] = '\0';

Scalar2:
    tok = YYCURSOR;


#line 970 "<stdout>"
{
	YYCTYPE yych;
	goto yy68;
	++YYCURSOR;
yy68:
	if((YYLIMIT - YYCURSOR) < 3) YYFILL(3);
	yych = *YYCURSOR;
	switch(yych){
	case 0x00:	goto yy74;
	case 0x0A:	goto yy70;
	case 0x0D:	goto yy72;
	default:	goto yy76;
	}
yy70:	++YYCURSOR;
	switch((yych = *YYCURSOR)) {
	case 'C':	goto yy78;
	case 'N':	goto yy80;
	case 'Z':	goto yy83;
	default:	goto yy71;
	}
yy71:
#line 461 "bytecode.re"
{   YYCURSOR = tok;
            goto ScalarEnd; 
        }
#line 996 "<stdout>"
yy72:	++YYCURSOR;
	switch((yych = *YYCURSOR)) {
	case 0x0A:	goto yy77;
	default:	goto yy73;
	}
yy73:
#line 469 "bytecode.re"
{   CAT(str, cap, idx, tok[0]);
            goto Scalar2; 
        }
#line 1007 "<stdout>"
yy74:	++YYCURSOR;
	goto yy75;
yy75:
#line 465 "bytecode.re"
{   YYCURSOR = tok;
            goto ScalarEnd;
        }
#line 1015 "<stdout>"
yy76:	yych = *++YYCURSOR;
	goto yy73;
yy77:	yych = *++YYCURSOR;
	switch(yych){
	case 'C':	goto yy78;
	case 'N':	goto yy80;
	case 'Z':	goto yy83;
	default:	goto yy71;
	}
yy78:	++YYCURSOR;
	goto yy79;
yy79:
#line 435 "bytecode.re"
{   CHK_NL(tok+1);
            goto Scalar2; }
#line 1031 "<stdout>"
yy80:	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	goto yy81;
yy81:	switch(yych){
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy80;
	default:	goto yy82;
	}
yy82:
#line 438 "bytecode.re"
{   CHK_NL(tok+1);
            if ( tok + 2 < YYCURSOR )
            {
                char *count = tok + 2;
                int total = strtod( count, NULL );
                int i;
                for ( i = 0; i < total; i++ )
                {
                    CAT(str, cap, idx, '\n');
                }
            }
            else
            {
                CAT(str, cap, idx, '\n');
            }
            goto Scalar2;
        }
#line 1068 "<stdout>"
yy83:	++YYCURSOR;
	goto yy84;
yy84:
#line 456 "bytecode.re"
{   CHK_NL(tok+1);
            CAT(str, cap, idx, '\0');
            goto Scalar2; 
        }
#line 1077 "<stdout>"
}
#line 473 "bytecode.re"


ScalarEnd:
        {
            SyckNode *n = syck_alloc_str();
            n->data.str->ptr = str;
            n->data.str->len = idx;
            sycklval->nodeData = n;
            POP_LEVEL();
            if ( parser->implicit_typing == 1 )
            {
                try_tag_implicit( sycklval->nodeData, parser->taguri_expansion );
            }
            return YAML_PLAIN;
        }
    }

}

char *
get_inline( SyckParser *parser )
{
    int idx = 0;
    int cap = 100;
    char *str = S_ALLOC_N( char, cap );
    char *tok;

    str[0] = '\0';

Inline:
    {
        tok = YYCURSOR;


#line 1114 "<stdout>"
{
	YYCTYPE yych;
	goto yy85;
	++YYCURSOR;
yy85:
	if((YYLIMIT - YYCURSOR) < 2) YYFILL(2);
	yych = *YYCURSOR;
	switch(yych){
	case 0x00:	goto yy91;
	case 0x0A:	goto yy87;
	case 0x0D:	goto yy89;
	default:	goto yy93;
	}
yy87:	++YYCURSOR;
	goto yy88;
yy88:
#line 508 "bytecode.re"
{   CHK_NL(YYCURSOR);
                return str; }
#line 1134 "<stdout>"
yy89:	++YYCURSOR;
	switch((yych = *YYCURSOR)) {
	case 0x0A:	goto yy94;
	default:	goto yy90;
	}
yy90:
#line 515 "bytecode.re"
{   CAT(str, cap, idx, tok[0]);
                goto Inline; 
            }
#line 1145 "<stdout>"
yy91:	++YYCURSOR;
	goto yy92;
yy92:
#line 511 "bytecode.re"
{   YYCURSOR = tok;
                return str;
            }
#line 1153 "<stdout>"
yy93:	yych = *++YYCURSOR;
	goto yy90;
yy94:	++YYCURSOR;
	yych = *YYCURSOR;
	goto yy88;
}
#line 519 "bytecode.re"


    }

}

