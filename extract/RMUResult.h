#ifndef __RMURESULT_H__
#define __RMURESULT_H__

#include <vector>
#include <string>

//#include "RMUDLL.h"

struct RMUResult;

void RMUResultGet(const std::string& answer, std::vector<RMUResult>& result); // throw(RMUError)


//	Return character representation (read-only) of the morphosyntax class of the result
const char * RMUGetSyntClassString(const RMUResult * pResult);


////////////////////////////////////////////////////////////////////////////////////////////
//
//		RMUFindSyntClassId
//	Helper function for grammar table parser
//	Return value - number of syntax class with given name (in Win-1251 encoding)
//	Currently English and Russian nclass names are supported
int RMUFindSyntClassId(const char * className);



const int Undefined = 0;

enum Es_cl {noun = 1,
			personalpronoun, reflexivepronoun, pronoun,
			name,
			adjective, possesiveadjective, pronounadjective, numberordinal, participle,
			shortadjective, shortparticiple, comparative,
			verb, unpersonalverb, frequentativeverb, gerund, 
			numberone, numbertwo, numberthree, number, numberbiform,
			adverb, preposition, conjunction, predicative, particle, interjection, parenthesis, acronym, 
			comma, quotation, dash, point, S_clAny
};

enum Eanimate {animate = 1, unanimate, animateAny};

enum Egender {male = 1, female, neuter, malefemale, maleorfemale, genderAny};

enum Enumber {single = 1, plural, numberAny};

enum Ecase { nominative = 1, genitive, dative, accusative, instrumental, prepositional, caseAny};

enum Ereflection {reflexive = 1, unreflexive, reflexiveForm, reflectionAny};


enum Eperfective {perfective = 1, unperfective, perfectiveAny};

enum Etransitive {transitive =1, untransitive, transitiveAny};

enum Eperson {personAny = 4};

enum Etense {infinitive = 1, present, past, future, imperative, tenseAny};

enum Evoice {active = 1, passive, voiceAny};

enum Edegree {strong = 1, weak, degreeAny};

struct RMUResult {
    std::string an_w;
    std::string lexem;
    std::string P_cl;
    std::string schema;
    std::string uid;
    
    int pcl;
    int num_inP;
    int S_cl;
    int Animate;
    int Gender;
    int Number;
    int Case;
    int Reflection;
    int Perfective;
    int Transitive;
    int Person;
    int Tense;
    int Voice;
    int Degree;
    bool Static;
    int Stress;
    
    // phrase-specific
    
    //RMUResult *nextWord;
    //bool mainWord;
    //int Phrase_cl;
    //int innerPhrase_cl;
    
    RMUResult ()
    {
        pcl = 0;
        num_inP = -1;
        S_cl = 0;
        Animate = 0;
        Gender = 0;
        Number =  0;
        Case =  0;
        Reflection =  0;
        Perfective =  0;
        Transitive = 0;
        Person = 0;
        Tense = 0 ;
        Voice = 0;
        Degree = 0;
        Static = false;
        Stress = 0;
        
        // phrase-specific
        
        //mainWord = false;
        //nextWord = NULL;
        //Phrase_cl = 0;
        //innerPhrase_cl = 0;
    }
    
    std::string GetTranslatedAnswer() const;
};

#endif