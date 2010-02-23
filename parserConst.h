#pragma once

#include <sstream>

#define _DEBUG
#define PARSER // leave here for SymTable.h/SymTable.cpp

static const int MAX_POSSIBLE_TOKENS		=	1000000;
static const int MAX_CHILD_NODES			=	100;

#define codeType int

// Arithmetik-Befehle ohne Argument
static const codeType CODE_ADD	=	0;
static const codeType CODE_SUB	=	CODE_ADD + 1;
static const codeType CODE_MUL	=	CODE_SUB + 1;
static const codeType CODE_DIV	=	CODE_MUL + 1;

// Vergleich ohne Argument
static const codeType CODE_LS	=	CODE_DIV + 1; // kleiner als
static const codeType CODE_EQ	=	CODE_LS + 1;

// Logische Operationen ohne Argument
static const codeType CODE_AND	=	CODE_EQ + 1;
static const codeType CODE_NOT	=	CODE_AND + 1;

// Laden, Speichern
static const codeType CODE_LA	=	CODE_NOT + 1; // Speicheradresse laden
static const codeType CODE_LCI	=	CODE_LA + 1; // Konstante laden
static const codeType CODE_LCF	=	CODE_LCI + 1; // Konstante laden
static const codeType CODE_LV	=	CODE_LCF + 1; // gespeicherten Wert laden
static const codeType CODE_STR	=	CODE_LV + 1; // Wert speichern

// Einlesen und Drucken ohne Argument
static const codeType CODE_PR	=	CODE_STR + 1; // drucken
static const codeType CODE_RD	=	CODE_PR + 1; // einlesen

// Sprünge mit Argument
static const codeType CODE_JMP	=	CODE_RD + 1; // unbedingter Sprung
static const codeType CODE_JIN	=	CODE_JMP + 1; // bedingter Sprung

// sonstige
static const codeType CODE_FLT	=	CODE_JIN + 1; // Konvertierung ohne Argument
static const codeType CODE_DS	=	CODE_FLT + 1; // Speicher reservieren mit Argument
static const codeType CODE_NOP	=	CODE_DS + 1; // nichts tun
static const codeType CODE_STP	=	CODE_NOP + 1; // stoppen





#define nodeType int

static const nodeType NODE_ROOT			=	0;

static const nodeType NODE_UNKNOWN		=	NODE_ROOT - 1;
static const nodeType NODE_ERROR		=	NODE_UNKNOWN - 1;
static const nodeType NODE_EMPTY		=	NODE_ERROR - 1;

static const nodeType NODE_PROG			=	NODE_ROOT + 1;
static const nodeType NODE_DECLS		=	NODE_PROG + 1;
static const nodeType NODE_DECL			=	NODE_DECLS + 1;
static const nodeType NODE_ARRAY		=	NODE_DECL + 1;
static const nodeType NODE_TYPE			=	NODE_ARRAY + 1;
static const nodeType NODE_STATEMENTS	=	NODE_TYPE + 1;
static const nodeType NODE_STATEMENT	=	NODE_STATEMENTS + 1;
static const nodeType NODE_EXP			=	NODE_STATEMENT + 1;
static const nodeType NODE_EXP2			=	NODE_EXP + 1;
static const nodeType NODE_INDEX		=	NODE_EXP2 + 1;
static const nodeType NODE_OP_EXP		=	NODE_INDEX + 1;
static const nodeType NODE_OP			=	NODE_OP_EXP + 1;

static const nodeType NODE_IDENTIFIER	=	NODE_OP + 1;
static const nodeType NODE_KEYWORD		=	NODE_IDENTIFIER + 1;


#define typification int

static const typification TYPIFICATION_NONE			= 0;

static const typification TYPIFICATION_ERROR		= TYPIFICATION_NONE - 1;

static const typification TYPIFICATION_INT			= 1;
static const typification TYPIFICATION_REAL			= TYPIFICATION_INT + 1;
static const typification TYPIFICATION_ARRAY		= TYPIFICATION_REAL + 1;
static const typification TYPIFICATION_INT_ARRAY	= TYPIFICATION_ARRAY + 1;
static const typification TYPIFICATION_REAL_ARRAY	= TYPIFICATION_INT_ARRAY + 1;
static const typification TYPIFICATION_OP_PLUS		= TYPIFICATION_REAL_ARRAY + 1;
static const typification TYPIFICATION_OP_MINUS		= TYPIFICATION_OP_PLUS + 1;
static const typification TYPIFICATION_OP_MULT		= TYPIFICATION_OP_MINUS + 1;
static const typification TYPIFICATION_OP_DIV		= TYPIFICATION_OP_MULT + 1;
static const typification TYPIFICATION_OP_LESS		= TYPIFICATION_OP_DIV + 1;
static const typification TYPIFICATION_OP_NEG		= TYPIFICATION_OP_LESS + 1;
static const typification TYPIFICATION_OP_AND		= TYPIFICATION_OP_NEG + 1;
static const typification TYPIFICATION_OP_EQUAL		= TYPIFICATION_OP_AND + 1;

class parserConst
{
	
public:
	
	parserConst();
	
	
	static std::string nodeTypeAsString(nodeType myNodeType);
	static std::string typificationAsString(typification myTypification);
	
};
