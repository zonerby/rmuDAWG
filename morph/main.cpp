//
//  main.cpp
//  MorphAn
//
//  Created by Artur Artur on 27.04.16.
//  Copyright © 2016 Artur Artur. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "RMUResult.h"
#include "dawgdic/completer.h"
#include "dawgdic/dawg-builder.h"
#include "dawgdic/dictionary.h"
#include "dawgdic/dictionary-builder.h"
#include "dawgdic/guide-builder.h"
using namespace std;
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
void Complete(const dawgdic::Dictionary &dic, const dawgdic::Guide &guide,
              const char *prefix, int top_n,std::vector<string>& result) {
    
    // Checks if the dictionary contains keys starting with a given
    // string "prefix". At the same time, the start position of
    // key completion is acquired.
    dawgdic::BaseType index = dic.root();
    if (!dic.Follow(prefix, &index))
        return;
    
    // Initializes an object for key completion.
    dawgdic::Completer completer(dic, guide);
    
    // Prepares for key completion.
    completer.Start(index, prefix);
    
    for (int i = 0; i < top_n; ++i) {
        
        // Completes the next key.
        // Finishes the function if there are no more keys.
        if (!completer.Next())
            break;
        
        // Shows a completed key and its associated record.
        //std::cout << completer.key()<< std::endl;
        result.push_back(string(completer.key()));
    }
}

struct par2{
    int par_num;
    int size_of_par;
    int* suf_int;
    int* gramm_int;
};

void convert(GramInfo result,RMUResult &output){
    output.S_cl = result.S_cl;
    output.Animate = result.Animate;
    output.Gender = result.Gender;
    output.Number = result.Number;
    output.Case = result.Case;
    output.Reflection = result.Reflection;
    output.Perfective = result.Perfective;
    output.Transitive = result.Transitive;
    output.Person = result.Person;
    output.Tense = result.Tense;
    output.Voice = result.Voice;
    output.Degree = result.Degree;
    output.Static = result.Static;
    //output.Stress = result.Stress;
}

std::ifstream suf_file("suffixes.txt");
std::ifstream gram_file("gram.data", std::ios::binary);
std::ifstream par_file("par.data", std::ios::binary);
std::ifstream words_dawg("words.dawg", std::ios::binary);

vector<string> suffixes;
vector<par2> paradigms;
vector<GramInfo> grammems;
dawgdic::Dictionary dic;
dawgdic::Guide dic_guide;
vector<string> tst;

void makeRMUResult(int par_num, int num_in_par,RMUResult &res){
    int gram_num = paradigms[par_num].gramm_int[num_in_par];
    convert(grammems[gram_num], res);
}
int cn = 0;
void morph(string in){
    string word = in+":";
    vector<string> words;
    //cout << word << endl;
    Complete(dic, dic_guide, word.c_str(), 10, words);
    if (words.size()>0)
        tst.push_back(in);
    for (int i=0;i<words.size();i++){
        RMUResult tmp;
        tmp.an_w = in;
        int sep = words[i].find(":");
        string str = string(words[i],sep+1,words[i].size()-1);
        int hom =  str.find(",");
        string pN = string(str,0,hom);
        string iN = string(str,hom+1,str.size()-1);
        int par_num = stoi(pN);
        int num_in_par = stoi(iN);
        makeRMUResult(par_num, num_in_par,tmp);
        
        //cout << "done" <<endl;
    }
}
void morph2(string in){
    string word = in+":";
    vector<string> words;
    //cout << word << endl;
    Complete(dic, dic_guide, word.c_str(), 5, words);
    for (int i=0;i<words.size();i++){
        RMUResult tmp;
        size_t sep = words[i].find(":");
        string str = string(words[i],sep+1,words[i].size()-1);
        size_t hom =  str.find(",");
        string pN = string(str,0,hom);
        string iN = string(str,hom+1,str.size()-1);
        makeRMUResult(stoi(pN), stoi(iN),tmp);
        //cout << "done" <<endl;
    }
}

void loadDic(){
    dic.Read(&words_dawg);
    dic_guide.Read(&words_dawg);
    string s;
    cout << sizeof(GramInfo) <<endl;
    while (getline(suf_file,s)) {
        suffixes.push_back(s);
    }
    while (gram_file) {
        GramInfo tmp;
        gram_file.read((char*)&tmp, sizeof(GramInfo));
        grammems.push_back(tmp);
    }
    while (par_file) {
        par2 tmp;
        par_file.read((char*)&tmp.par_num, sizeof(int));
        par_file.read((char*)&tmp.size_of_par, sizeof(int));
        tmp.gramm_int = new int [tmp.size_of_par];
        par_file.read((char*)tmp.gramm_int, tmp.size_of_par*sizeof(int));
        tmp.suf_int = new int [tmp.size_of_par];
        par_file.read((char*)tmp.suf_int, tmp.size_of_par*sizeof(int));
        paradigms.push_back(tmp);
    }
    
    cout<< "Done loading suffix data - "<<suffixes.size() <<endl;
    cout<< "Done loading paradigms data - "<<paradigms.size() <<endl;
    cout<< "Done loading grammems data - "<<grammems.size() <<endl;
    cout<< "Done loading dic data" << endl;
}

std::ifstream full_file("full.txt");
int main(int argc, const char * argv[]) {
    vector<string> test;
    string wrd;
    while (getline(full_file,wrd)) {
        wrd.pop_back();
        test.push_back(wrd);
    }
    loadDic();
    string in;
    for (int i=0;i<test.size();i++){
        morph(test[i]);
    }
    clock_t start = clock();
    for (int i=0;i<tst.size();i++){
        morph2(tst[i]);
    }
    cout << double(clock() - start)/ (double)CLOCKS_PER_SEC <<endl;
    cout << tst.size() << endl;
    cout << cn << endl;
//    while(true){
//        cin >> in;
//        cout << "An word: "+in <<endl;
//        morph(in);
//    }
    
}
