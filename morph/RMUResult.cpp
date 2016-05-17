// RMUResult.cpp : Defines the exported functions for the DLL application.
//

//#include "stdafx.h"

#include "RMUResult.h"
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <assert.h>
#include "RMUError.hpp"

using namespace std;

static void ParseXMLToResult(std::vector<RMUResult>& MR, const string& xml_res);
static string GetTranslatedResult(const RMUResult * pResult);

void RMUResultGet(const std::string& answer, std::vector<RMUResult>& result)
{
	std::string a;
    transform(answer.begin(), answer.end(), back_inserter(a), towlower);
	ParseXMLToResult(result, a);
}

string RMUResult::GetTranslatedAnswer() const
{
	return GetTranslatedResult(this);
}

////////////////////////////////////////////////////////////
//
//		variables for translation
//
////////////////////////////////////////////////////////////
const char * S_clStr [] = {"Undefined", "noun", 
			"personalpronoun", "reflexivepronoun", "pronoun", 
			"name", 
			"adjective", "possesiveadjective", "pronounadjective", "numberordinal", "participle",
			"shortadjective", "shortparticiple", "comparative",
			"verb", "unpersonlverb", "frequentativeverb","gerund",
			"numberone", "numbertwo", "numberthree", "number", "numberbiform",
			"adverb", "preposition", "conjunction", "predicative", "particle", "interjection", "parenthesis", "acronym", 
			"comma", "quotation", "dash", "point", "any"};

const char * S_clStrRus [] = {"Не определен", "Существительное", 
			"Личное местоимение", "Возвратное местоимение", "Местоимение", 
			"Имя собственное", 
			"Прилагательное", "Притяжательное прилагательное", "Местоименное прилагательное", "Порядковое числительное", "Причастие",
			"Краткое прилагательное", "Краткое причастие", "Сравнительная степень прилагательного",
			"Глагол", "Безличный глагол", "Многократный глагол", "Деепричастие",
			"Числительное1", "Числительное2", "Числительное3", "Числительное", "Числительное с двумя формами", 
			"Наречие", "Предлог", "Союз", "Предикатив", "Частица", "Междометие", "Вводное", "Акроним",
			"Запятая", "Кавычки", "Тире", "Точка",
			"Произвольный"};

const char * S_clStrNic [] = {"Не определен", "Сущ", 
			"ЛичнМестоим", "ВозврМестоим", "Местоим", 
			"ИмяСобств", 
			"Прил", "ПритяжПрил", "МестПрил", "ПорядкЧисл", "Прич",
			"КраткПрил","КраткПрич", "СравнСтПрил",
			"Глагол", "БезлГлагол", "МногокрГлагол", "Дееприч",
			"Числ1", "Числ2,оба,полтора", "Числ3,4,собирательное", "Числ", "Числ2формы",
			"Наречие", "Предлог", "Союз", "Предикатив", "Частица", "Междометие", "Вводное", "Акроним",
			"Запятая", "Кавычки", "Тире", "Точка", "Произвольный"};

const char * AnimateStr [] = {"Undefined", "yes", "no", "any"};
const char * AnimateStrRus [] = {"Не определено", "Одушевленное", "Неодушевленное", "Произвольная"};
const char * GenderStr [] = {"Undefined", "male", "female", "neuter", "malefemale", "maleorfemale", "any"};
const char * GenderStrRus [] = {"Не определено", "Мужской", "Женский", "Средний", "Мужской/Женский", "Мужской или Женский", "Произвольный"};
const char * NumberStr [] = {"Undefined", "single", "plural", "any"};
const char * NumberStrRus [] = {"Не определено", "Единственное", "Множественное", "Произвольное"};
const char * CaseStr [] = {"Undefined", "nominative", "genitive", "dative", "accusative", "instrumental", "prepositional", "any"};
const char * CaseStrRus [] = {"Не определено", "И.п.", "Р.п.", "Д.п.", "В.п.", "Т.п.", "П.п.", "Произвольный"};
const char * ReflectionStr [] = {"Undefined", "reflexive", "unReflexive", "reflexiveForm", "any"};
const char * ReflectionStrRus [] = {"Не определено", "Возвратный", "Невозвратный", "Возвратная форма", "Произвольный"};
const char * PerfectiveStr [] = {"Undefined", "perfective", "unperfective", "any"};
const char * PerfectiveStrRus [] = {"Не определено", "Совершенный", "Несовершенный", "Произвольный"};
const char * TransitiveStr [] = {"Undefined", "transitive", "intransitive", "any"};
const char * TransitiveStrRus [] = {"Не определено", "Переходный", "Непереходный", "Произвольная"};
const char * PersonStr [] = {"Undefined", "1", "2", "3", "any"};
const char * PersonStrRus [] = {"Не определено", "1", "2", "3", "Произвольное"};
const char * TenseStr [] = {"Undefined", "infinitive", "present", "past", "future", "imperative", "any"};
const char * TenseStrRus [] = {"Не определено", "Инфинитив", "Настоящее", "Прошедшее", "Будущее", "Повелительное", "Произвольное"};
const char * VoiceStr [] = {"Undefined", "active", "passive", "any"};
const char * VoiceStrRus [] = {"Не определено", "Действительный", "Страдательный", "Произвольный"};
const char * DegreeStr [] = {"Undefined", "strong", "weak", "any"};
const char * DegreeStrRus [] = {"Не определено", "Сильная", "Слабая", "Произвольная"};


const char * RMUGetSyntClassString(const RMUResult * pResult) 
{
	return S_clStrRus[pResult->S_cl];
}

string GetTranslatedResult(const RMUResult * pResult)
{
	assert(pResult);
	stringstream t;
	t << pResult->lexem << " - " << S_clStrRus[pResult->S_cl];
	if (!pResult->Static)
		t << '\t' << "изменяемое (P_cl = " << pResult->P_cl << " )\n\t ударная буква - " << pResult->Stress << " (схема " << pResult->schema << " )\n";
	else
		t << '\t' << "неизменяемое \n\t ударная буква - " << pResult->Stress << " (схема " << pResult->schema << " )\n";
	if (pResult->Animate)
		t << "\t\t Одушевленность - " << AnimateStrRus[pResult->Animate] << '\n';
	if (pResult->Gender)
		t << "\t\t Род - " << GenderStrRus[pResult->Gender] << '\n';
	if (pResult->Number)
		t << "\t\t Число - " << NumberStrRus[pResult->Number] << '\n';
	if (pResult->Case)
		t << "\t\t Падеж - " << CaseStrRus[pResult->Case] << '\n';
	if (pResult->Person)
		t << "\t\t Лицо - " << PersonStrRus[pResult->Person] << '\n';
	if (pResult->Reflection)
		t << "\t\t Возвратность - " << ReflectionStrRus[pResult->Reflection] << '\n';
	if (pResult->Perfective)
		t << "\t\t Вид - " << PerfectiveStrRus[pResult->Perfective] << '\n';
	if (pResult->Tense)
		t << "\t\t Время//Наклонение - " << TenseStrRus[pResult->Tense] << '\n';
	if (pResult->Transitive)
		t << "\t\t Переходность - " << TransitiveStrRus[pResult->Transitive] << '\n';
	if (pResult->Voice)
		t << "\t\t Залог - " << VoiceStrRus[pResult->Voice] << '\n';
	if (pResult->Degree)
		t << "\t\t Степень - " << DegreeStrRus[pResult->Degree] << '\n';

	return t.str();
}

/////////////////////////////////////////////////////
//
//	Helper functions for grammar table parser
//
/////////////////////////////////////////////////////

static int FindSyntClassIdEng(const char * className)
{
	const char ** pCur = S_clStr;
	while (*pCur)
	{
		if (strcmp(*pCur++, className) == 0)
			return pCur - S_clStr - 1;
	}
	return -1;
}

//	Win-1251 encoding
//  
static int FindSyntClassIdRus(const char * className)
{
	const char ** pCur = S_clStrNic;
	while (*pCur)
	{
		if (strcmp(*pCur++, className) == 0)
			return pCur - S_clStrNic - 1;
	}
	return -1;
}

int RMUFindSyntClassId(const char * className)
{
	if (className[0] & 0x80) // assume Russian
		return FindSyntClassIdRus(className);
	else
		return FindSyntClassIdEng(className);

}

/////////////////////////////////////////////////////////////////////////////
//
//		helper routines for converting RMU answer xml to RMUResult structure
//
/////////////////////////////////////////////////////////////////////////////

static void r_P_cl(string & var, RMUResult & res){
	int p,k;
	if ((p = var.find("paradigmid")) != string::npos){
			k = var.find(" ", p);
			res.P_cl = strdup(string(var, p+11, k-p-11).c_str());
	}
}

static void r_schema(string & var, RMUResult & res){
	int p,k;
	if ((p = var.find("schemaid")) != string::npos){
			k = var.find(" ", p);
			res.schema = strdup(string(var, p+9, k-p-9).c_str());
	}
}

static void r_uid(string & var, RMUResult & res){
	int p,k;
	if ((p = var.find("uid=")) != string::npos){
			k = var.find(" ", p);
			res.uid = strdup(string(var, p+4, k-p-4).c_str());
	}
}

static void r_static(string & var, RMUResult & res){
	if (var.find("static") != string::npos){
			res.Number = numberAny;
			res.Case = caseAny;
			res.Static = true;
	}
}

static void r_stress(string & var, RMUResult & res){
	int n;
	if ((n = var.find("stress")) != string::npos) { 
			if ('0' <= var[n+8] && '9' >= var[n+8])
				res.Stress = (var[n+7]-'0') * 10 + (var[n+8] - '0');
			else
				res.Stress = var[n+7]-'0';
		}
	else
		;//RAISE_RMU_ERR("Stress is absent!\n");
}

static void r_case(string & var, RMUResult & res){
	int n,p;
	if ( (n = var.find("case")) != string::npos) { 
			for (p=1; p < 8; p++) {
				if (var.find(CaseStr[p], n+1) != string::npos){
					res.Case = p;
					break;
				} 
			}
		}
	else
		if (!res.Static)  
			RAISE_RMU_ERR("Case is absent!\n");
}

static void r_animate(string & var, RMUResult & res){
	int n;
	if ((n = var.find("animate")) != string::npos) { 
				if (var.find("animate=yes ", n) != string::npos){
					res.Animate = animate;
				} 
				else
				if (var.find("no ", n+1) != string::npos){
					res.Animate = unanimate;
				} 
				else {
					res.Animate = animateAny;
				}		
	}
	else
		RAISE_RMU_ERR("Animate is absent!\n");
}

static void r_gender(string & var, RMUResult & res){
	int n;
	if ((n = var.find("gender")) != string::npos) { 
			if (var.find("malefemale", n+1) != string::npos){
				res.Gender = malefemale;
			} 
			else
				if (var.find("maleorfemale", n+1) != string::npos){
				res.Gender = maleorfemale;
			} 
			else
			if (var.find("female", n+1) != string::npos){
				res.Gender = female;
			} 
			else
			if (var.find("male", n+1) != string::npos){
				res.Gender = male;
			}
			else
			if (var.find("neuter", n+1) != string::npos){
				res.Gender = neuter;
			} 
			else {
				res.Gender = genderAny;
			}
		}
		else
			RAISE_RMU_ERR("Gender is absent!\n");
}

static void r_number(string & var, RMUResult & res){
	int n;
	if ((n = var.find("number")) != string::npos) { 
			if (var.find("single ", n+1) != string::npos){
				res.Number = single;
			} 
			else
			if (var.find("plural ", n+1) != string::npos){
				res.Number = plural;
			} 
			else {
				res.Number = numberAny;
			}
	}
	else
		if (!res.Static) 
			RAISE_RMU_ERR( "Number is absent!\n");
}

static void r_degree(string & var, RMUResult & res){
	int n;
	if ((n = var.find("degree")) != string::npos) { 
				if (var.find("strong", n) != string::npos){
					res.Degree = strong;
				} 
				else
				if (var.find("weak", n+1) != string::npos){
					res.Degree = weak;
				} 
				else {
					res.Animate = animateAny;
				}		
	}
	else
		RAISE_RMU_ERR( "Degree is absent!\n");
}

static void r_person(string & var, RMUResult & res){
	int n;
	if ((n = var.find("person=")) != string::npos) { 
				if (var[n+7] == '1'){
					res.Person = 1;
				} 
				else
				if (var[n+7] =='2'){
					res.Person = 2;
				}
				else
				if (var[n+7] =='3'){
					res.Person = 3;
				} 
				else {
					res.Person = personAny;
				}		
	}
	else
		RAISE_RMU_ERR( "Person is absent!\n");
}

static void r_reflection(string & var, RMUResult & res){
	int n;
	if ((n = var.find("reflexive")) != string::npos){
		if (var.find("reflexive=yes ", n) != string::npos){
					res.Reflection = reflexive;
				} 
				else
				if (var.find("no ", n+1) != string::npos){
					if (var.find("vform") != string::npos)
						res.Reflection = reflexiveForm;
					else
						res.Reflection = unreflexive;
				} 
				else {
					res.Reflection = reflectionAny;
				}			
	}
	else
		RAISE_RMU_ERR( "Reflective is absent!\n");
}

static void r_perfective(string & var, RMUResult & res){
	int n;
	if ((n = var.find("perfective")) != string::npos){
		if (var.find("perfective=yes ", n) != string::npos){
					res.Perfective = perfective;
				} 
				else
				if (var.find("no ", n+1) != string::npos){
					res.Perfective = unperfective;
				} 
				else {
					res.Perfective = perfectiveAny;
				}			
	}
	else
		RAISE_RMU_ERR( "Perfective is absent!\n");
}

static void r_transitive(string & var, RMUResult & res){
	int n;
	if ((n = var.find("transitive")) != string::npos){
		if (var.find("transitive=yes ", n) != string::npos){
					res.Transitive = transitive;
				} 
				else
				if (var.find("no ", n+1) != string::npos){
					res.Transitive = untransitive;
				} 
				else {
					res.Transitive = transitiveAny;
				}			
	}
	else
		RAISE_RMU_ERR( "Transitive is absent!\n");
}

static void r_tense(string & var, RMUResult & res){
	int n;
	if (var.find("infinitive") != string::npos){
					res.Tense = infinitive;
					return;
	}
	if (var.find("imperative") != string::npos){
					res.Tense = imperative;
					return;
	}
	if ((n = var.find("tense")) != string::npos) { 
				if (var.find("present", n) != string::npos){
					res.Tense = present;
				} 
				else
				if (var.find("past", n+1) != string::npos){
					res.Tense = past;
				} 
				else
				if (var.find("future", n+1) != string::npos){
					res.Tense = future;
				} 
				else {
					res.Tense = tenseAny;
				}		
	}
	else
		RAISE_RMU_ERR( "Tense is absent!\n");
}

static void r_voice(string & var, RMUResult & res){
	int n;
	if ((n = var.find("voice")) != string::npos) { 
				if (var.find("active", n) != string::npos){
					res.Voice = active;
				} 
				else
				if (var.find("passive", n+1) != string::npos){
					res.Voice = passive;
				} 
				else {
					res.Voice = voiceAny;
				}		
	}
	else
		RAISE_RMU_ERR( "Voice is absent!\n");
}

static void noun_res (vector <RMUResult> & MR, const string & lex_var, const string & lex) {
	int i = -1, k;
	while ((i = lex_var.find("<noun", i+1)) != string::npos) {
		RMUResult res;
		res.S_cl = noun;
		k = lex_var.find("/>", i+1);
		string var(lex_var, i, k-i);

		res.lexem = strdup(lex.c_str());

		r_P_cl(var, res);
		r_schema(var, res);
		r_uid(var, res);
		r_static(var, res);
		r_stress(var, res);
		r_case(var, res);
		r_animate(var, res);
		r_gender(var, res);
		r_number(var, res);
				
		MR.push_back(res);
	}
}

static void personalpronoun_res (vector <RMUResult> & MR, const string & lex_var, const string & lex) {
	int i = -1, k;
	while ((i = lex_var.find("<personalpronoun", i+1)) != string::npos) {
		RMUResult res;
		res.S_cl = personalpronoun;
		k = lex_var.find("/>", i+1);
		string var(lex_var, i, k-i);

		res.lexem = strdup(lex.c_str());

		r_P_cl(var, res);
		r_schema(var, res);
		r_uid(var, res);
		r_static(var, res);
		r_stress(var, res);
		r_person(var, res);
		r_case(var, res);
		r_animate(var, res);
		r_gender(var, res);
		r_number(var, res);
				
		MR.push_back(res);
	}
}

static void reflexivepronoun_res (vector <RMUResult> & MR, const string & lex_var, const string & lex) {
	int i = -1, k;
	while ((i = lex_var.find("<reflexivepronoun", i+1)) != string::npos) {
		RMUResult res;
		res.S_cl = reflexivepronoun;
		k = lex_var.find("/>", i+1);
		string var(lex_var, i, k-i);

		res.lexem = strdup(lex.c_str());

		r_P_cl(var, res);
		r_schema(var, res);
		r_uid(var, res);
		r_static(var, res);
		r_stress(var, res);
		r_case(var, res);
		r_animate(var, res);
		r_gender(var, res);
		r_number(var, res);
				
		MR.push_back(res);
	}
}

static void pronoun_res (vector <RMUResult> & MR, const string & lex_var, const string & lex) {
	int i = -1, k;
	while ((i = lex_var.find("<pronoun ", i+1)) != string::npos) {
		RMUResult res;
		res.S_cl = pronoun;
		k = lex_var.find("/>", i+1);
		string var(lex_var, i, k-i);

		res.lexem = strdup(lex.c_str());

		r_P_cl(var, res);
		r_schema(var, res);
		r_uid(var, res);
		r_static(var, res);
		r_stress(var, res);
		r_case(var, res);
		r_animate(var, res);
		r_gender(var, res);
		r_number(var, res);
				
		MR.push_back(res);
	}
}

static void possesiveadjective_res (vector <RMUResult> & MR, const string & lex_var, const string & lex) {
	int i = -1, k;
	while ((i = lex_var.find("<possesiveadjective ", i+1)) != string::npos) {
		RMUResult res;
		res.S_cl = possesiveadjective;
		k = lex_var.find("/>", i+1);
		string var(lex_var, i, k-i);

		res.lexem = strdup(lex.c_str());

		r_P_cl(var, res);
		r_schema(var, res);
		r_uid(var, res);
		r_static(var, res);
		r_stress(var, res);
		r_case(var, res);
		r_animate(var, res);
		r_gender(var, res);
		r_number(var, res);
				
		MR.push_back(res);
	}
}

static void pronounadjective_res (vector <RMUResult> & MR, const string & lex_var, const string & lex) {
	int i = -1, k;
	while ((i = lex_var.find("<pronounadjective ", i+1)) != string::npos) {
		RMUResult res;
		res.S_cl = pronounadjective;
		k = lex_var.find("/>", i+1);
		string var(lex_var, i, k-i);

		res.lexem = strdup(lex.c_str());

		r_P_cl(var, res);
		r_schema(var, res);
		r_uid(var, res);
		r_static(var, res);
		r_stress(var, res);
		r_case(var, res);
		r_animate(var, res);
		r_gender(var, res);
		r_number(var, res);
				
		MR.push_back(res);
	}
}

static void numberordinal_res (vector <RMUResult> & MR, const string & lex_var, const string & lex) {
	int i = -1, k;
	while ((i = lex_var.find("<numberordinal ", i+1)) != string::npos) {
		RMUResult res;
		res.S_cl = numberordinal;
		k = lex_var.find("/>", i+1);
		string var(lex_var, i, k-i);

		res.lexem = strdup(lex.c_str());

		r_P_cl(var, res);
		r_schema(var, res);
		r_uid(var, res);
		r_static(var, res);
		r_stress(var, res);
		r_case(var, res);
		r_animate(var, res);
		r_gender(var, res);
		r_number(var, res);
				
		MR.push_back(res);
	}
}

static void adj_res (vector <RMUResult> & MR, const string & lex_var, const string & lex) {
	int i = -1, k, n;
	while ((i = lex_var.find("<adjective", i+1)) != string::npos) {
		RMUResult res;

		res.S_cl = adjective;

		k = lex_var.find("/>", i+1);
		string var(lex_var, i, k-i);

		res.lexem = strdup(lex.c_str());

		r_P_cl(var, res);
		r_schema(var, res);
		r_uid(var, res);
		r_static(var, res);
		if(res.Static){
			res.Animate = animateAny;
			res.Gender = genderAny;
		}
		r_stress(var, res);
			
		if ((n = var.find(" form")) != string::npos) { 
		
			if (var.find("short", n+1) != string::npos) {
				
				r_gender(var, res);
				r_number(var, res);
				res.S_cl = shortadjective;
			}
			else
			if (var.find("comparative", n+1) != string::npos){

				r_degree(var, res);	
				res.S_cl = comparative;
			}

			MR.push_back(res);
			return;
		}
		
		r_case(var, res);
		r_animate(var, res);
		r_gender(var, res);
		r_number(var, res);
		
		MR.push_back(res);
	}
}

static void prep_res (vector <RMUResult> & MR, const string & lex_var, const string & lex) {
	int i = -1, k;
	while ((i = lex_var.find("preposition ", i+1)) != string::npos) {
		RMUResult res;
		res.S_cl = preposition;
		k = lex_var.find("/>", i+1);
		string var(lex_var, i, k-i);

		res.lexem = strdup(lex.c_str());

		r_schema(var, res);
		r_uid(var, res);
		r_stress(var, res);
		res.Static = true;

		MR.push_back(res);
	}
}


static void interj_res (vector <RMUResult> & MR, const string & lex_var, const string & lex) {
	int i = -1, k;
	while ((i = lex_var.find("interjection", i+1)) != string::npos) {
		RMUResult res;
		res.S_cl = interjection;
		k = lex_var.find("/>", i+1);
		string var(lex_var, i, k-i);

		res.lexem = strdup(lex.c_str());

		r_schema(var, res);
		r_uid(var, res);
		r_stress(var, res);
		res.Static = true;

		MR.push_back(res);
	}
}

static void particle_res (vector <RMUResult> & MR, const string & lex_var, const string & lex) {
	int i = -1, k;
	while ((i = lex_var.find("particle", i+1)) != string::npos) {
		RMUResult res;
		res.S_cl = particle;
		k = lex_var.find("/>", i+1);
		string var(lex_var, i, k-i);

		res.lexem = strdup(lex.c_str());

		r_schema(var, res);
		r_uid(var, res);
		r_stress(var, res);
		res.Static = true;

		MR.push_back(res);
	}
}

static void adverb_res (vector <RMUResult> & MR, const string & lex_var, const string & lex) {
	int i = -1, k;
	while ((i = lex_var.find("adverb", i+1)) != string::npos) {
		RMUResult res;
		res.S_cl = adverb;
		k = lex_var.find("/>", i+1);
		string var(lex_var, i, k-i);

		res.lexem = strdup(lex.c_str());

		r_schema(var, res);
		r_uid(var, res);
		r_stress(var, res);
		res.Static = true;

		MR.push_back(res);
	}
}

static void conj_res (vector <RMUResult> & MR, const string & lex_var, const string & lex) {
	int i = -1, k;
	while ((i = lex_var.find("conjunction", i+1)) != string::npos) {
		RMUResult res;
		res.S_cl = conjunction;
		k = lex_var.find("/>", i+1);
		string var(lex_var, i, k-i);

		res.lexem = strdup(lex.c_str());

		r_schema(var, res);
		r_uid(var, res);
		r_stress(var, res);
		res.Static = true;

		MR.push_back(res);
	}
}

static void predicative_res (vector <RMUResult> & MR, const string & lex_var, const string & lex) {
	int i = -1, k;
	while ((i = lex_var.find("predicative", i+1)) != string::npos) {
		RMUResult res;
		res.S_cl = predicative;
		k = lex_var.find("/>", i+1);
		string var(lex_var, i, k-i);

		res.lexem = strdup(lex.c_str());

		r_schema(var, res);
		r_uid(var, res);
		r_stress(var, res);
		res.Static = true;

		MR.push_back(res);
	}
}

static void parenthesis_res (vector <RMUResult> & MR, const string & lex_var, const string & lex) {
	int i = -1, k;
	while ((i = lex_var.find("parenthesis", i+1)) != string::npos) {
		RMUResult res;
		res.S_cl = parenthesis;
		k = lex_var.find("/>", i+1);
		string var(lex_var, i, k-i);

		res.lexem = strdup(lex.c_str());

		r_schema(var, res);
		r_uid(var, res);
		r_stress(var, res);
		res.Static = true;

		MR.push_back(res);
	}
}

static void acronym_res (vector <RMUResult> & MR, const string & lex_var, const string & lex) {
	int i = -1, k;
	while ((i = lex_var.find("acronym", i+1)) != string::npos) {
		RMUResult res;
		res.S_cl = acronym;
		k = lex_var.find("/>", i+1);
		string var(lex_var, i, k-i);

		res.lexem = strdup(lex.c_str());

		r_schema(var, res);
		r_uid(var, res);
		r_stress(var, res);
		res.Static = true;

		MR.push_back(res);
	}
}

static void verb_res (vector <RMUResult> & MR, const string & lex_var, const string & lex) {
	int i = -1, k, n;
	while ((i = lex_var.find("<verb ", i+1)) != string::npos) {
		RMUResult res;

		k = lex_var.find("/>", i+1);
		string var(lex_var, i, k-i);

		res.lexem = strdup(lex.c_str());

		if (var.find("frequentative=yes ") != string::npos)
			res.S_cl = frequentativeverb;
		else
		if (var.find("personal=no ") != string::npos)
			res.S_cl = unpersonalverb;
		else
		if (var.find("=gerund ") != string::npos)
			res.S_cl = gerund;
		else
		if (var.find("participle ") != string::npos){
			res.S_cl = participle;
			r_voice(var, res);
			r_gender(var, res);
			r_number(var, res);
			if ((n = var.find(" form")) != string::npos){  		
				if (var.find("short", n+1) != string::npos) {
					res.S_cl = shortparticiple;
				}
			}
			else {
				r_case(var, res);
				r_animate(var, res);
			}		
		}
		else
			res.S_cl = verb;

		r_P_cl(var, res);
		r_schema(var, res);
		r_uid(var, res);
		r_stress(var, res);
		r_reflection(var, res);	
		r_transitive(var, res);	
		r_perfective(var, res);
		r_tense(var, res);
		
		if(res.Tense != infinitive && res.S_cl != gerund && res.S_cl != participle && res.S_cl != shortparticiple) {
				r_person(var, res);
				r_gender(var, res);
				r_number(var, res);
		}
		MR.push_back(res);
	}
}

static void number_res (vector <RMUResult> & MR, const string & lex_var, const string & lex) {
	int i = -1, k;
	while ((i = lex_var.find("<number", i+1)) != string::npos) {
		RMUResult res;

		k = lex_var.find("/>", i+1);
		string var(lex_var, i, k-i);

		res.lexem = strdup(lex.c_str());

		r_P_cl(var, res);
		r_schema(var, res);
		r_uid(var, res);
		r_stress(var, res);
		r_case(var, res);

		string pclass = res.P_cl;

		if ( pclass.find("Adj3") != string::npos) { 
			res.S_cl = numberone;
			r_animate(var, res);
			r_gender(var, res);
			r_number(var, res);
		}
		else
		if ( pclass.find("Num2") != string::npos) { 
			res.S_cl = numbertwo;
			r_animate(var, res);
			r_gender(var, res);
		}
		else
		if ( pclass.find("Num3") != string::npos) { 
			res.S_cl = numberthree;
			r_animate(var, res);
		}
		else
		if ( pclass.find("Num4") != string::npos) { 
			res.S_cl = number;
		}
		else
			res.S_cl = numberbiform;
			
		MR.push_back(res);
	}
}

void ParseXMLToResult(std::vector<RMUResult>& MR, const string& xml_res){
	int i = 0, j = 0, k, r = 0;

	if (xml_res.find("unrecognized") != string::npos){
		return;
	}
	while ((r = xml_res.find("lexem", r+1)) != string::npos) {
		j++;
		k = xml_res.find("/>", r);
		string lex (xml_res, r+6, k-r-7);

		if ((i = xml_res.find("analyzed", r+1)) == string::npos)
			i = xml_res.find("</answer>", r+1);

		string lex_var (xml_res, k+2, i-k-2);

		noun_res(MR, lex_var, lex);
		adj_res(MR, lex_var, lex);
		verb_res(MR, lex_var, lex);
		personalpronoun_res(MR, lex_var, lex);
		reflexivepronoun_res(MR, lex_var, lex);
		pronoun_res(MR, lex_var, lex);
		possesiveadjective_res(MR, lex_var, lex);
		pronounadjective_res(MR, lex_var, lex);
		numberordinal_res(MR, lex_var, lex);
		prep_res(MR, lex_var, lex);
		particle_res(MR, lex_var, lex);
		interj_res(MR, lex_var, lex);
		adverb_res(MR, lex_var, lex);
		conj_res(MR, lex_var, lex);
		parenthesis_res(MR, lex_var, lex);
		predicative_res(MR, lex_var, lex);
		acronym_res(MR, lex_var, lex);
		number_res(MR, lex_var, lex);
	}
}



