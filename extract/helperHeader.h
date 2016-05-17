//
//  helperHeader.h
//  extract
//
//  Created by Artur Artur on 28.04.16.
//  Copyright © 2016 Artur Artur. All rights reserved.
//
#include "dawgdic/completer.h"
#include "dawgdic/dawg-builder.h"
#include "dawgdic/dictionary.h"
#include "dawgdic/dictionary-builder.h"
#include "dawgdic/guide-builder.h"
#ifndef helperHeader_h
#define helperHeader_h

using namespace std;
struct RMUResult2 {
    int pcl;
    short S_cl;
    short Animate;
    short Gender;
    short Number;
    short Case;
    short Reflection;
    short Perfective;
    short Transitive;
    short Person;
    short Tense;
    short Voice;
    short Degree;
    bool Static;
    //short Stress;
    
    // phrase-specific
    
    //RMUResult *nextWord;
    //bool mainWord;
    //int Phrase_cl;
    //int innerPhrase_cl;
    
    RMUResult2 ()
    {
        pcl = 0;
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
        //Stress = 0;
    }
    
    std::string GetTranslatedAnswer() const;
};

struct GramInfo {
    short S_cl;
    short Animate;
    short Gender;
    short Number;
    short Case;
    short Reflection;
    short Perfective;
    short Transitive;
    short Person;
    short Tense;
    short Voice;
    short Degree;
    bool Static;
    //short Stress;
    
    // phrase-specific
    
    //RMUResult *nextWord;
    //bool mainWord;
    //int Phrase_cl;
    //int innerPhrase_cl;
    
    GramInfo ()
    {
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
        //Stress = 0;
    }
    
    std::string GetTranslatedAnswer() const;
};

struct par{
    string p_cl;
    int par_num;
    vector<string> suffixes;
    vector<int> suff_int;
    vector<int> gramm_int;
    vector<string> prefixes;
    vector<GramInfo> grammems;
};

struct par2{
    int par_num;
    int size_of_par;
    int* suf_int;
    int* gramm_int;
};
bool replace2(std::string& str, const std::string& from, const std::string& to);
bool check2(GramInfo res, GramInfo fromVec);
void Complete(const dawgdic::Dictionary &dic, const dawgdic::Guide &guide,
              const char *prefix, int top_n,std::vector<string>& result);
void convert(RMUResult result,GramInfo &output);
#endif /* helperHeader_h */
