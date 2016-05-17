//
//  main.cpp
//  extract
//
//  Created by Artur Artur on 27.02.16.
//  Copyright © 2016 Artur Artur. All rights reserved.
//

#include <iostream>
#include "RMUResult.h"
#include "RMUError.hpp"
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
#include "dawgdic/completer.h"
#include "dawgdic/dawg-builder.h"
#include "dawgdic/dictionary.h"
#include "dawgdic/dictionary-builder.h"
#include "dawgdic/guide-builder.h"
#include "helperHeader.h"


using namespace std;

std::ifstream infile("fullresult.txt");
std::ifstream stem_file("stems.dawg", std::ios::binary);


void parseXMLtoRMU(vector<RMUResult> &res){
    string s;
    vector<RMUResult> temp;
    while(getline(infile, s)){
        int lex = s.find("lexem=");
        //cout << s << endl;
        string an;
        if (lex==-1){
            an="error";
        } else {
            an = string(s,24,lex-26);
            
        }
        try {
            RMUResultGet(s,temp);
        } catch (RMUError er) {
            cout << "ERROR" << endl;
            continue;
        }
        for (int i=0;i<temp.size();i++){
            temp[i].an_w=an;
            res.push_back(temp[i]);
        }
        temp.clear();
    }

}

void extractPCL(vector<RMUResult> result, vector<string> &par){
    for (int i=0;i<result.size();i++){
        bool match = false;
        for (int j=0;j<par.size();j++){
            if (par[j] == result[i].P_cl){
                match=true;
                result[i].pcl = j;
                break;
            }
        }
        if (!match){
            par.push_back(result[i].P_cl);
            result[i].pcl = par.size()-1;
        }
    }
}

void generatePar(vector<string> parMatch,vector<RMUResult> res,vector<par> &paradigms, dawgdic::Dictionary *dic_stem, dawgdic::Guide *guide_stem){
    float parSize = parMatch.size(),it=0.0;
    for (int i=0; i<parMatch.size(); i++) {
        for (int j=0; j<res.size(); j++) {
            if (parMatch[i]==res[j].P_cl){
                vector<string> words;
                string s = string(res[j].uid,1,res[j].uid.size()-2);
                string wrd;
                s=s+":";
                Complete(*dic_stem, *guide_stem, s.c_str(), 1, words);
                for (int j=0;j<words.size();j++){
                    int l = words[j].find(":");
                    wrd = string(words[j],l+1,words[j].size());
                    if (wrd=="0"){
                        wrd="";
                    }
                }
                if (words.size() == 0){
                    replace2(s, "ё", "е");
                    Complete(*dic_stem, *guide_stem, s.c_str(), 1, words);
                    for (int j=0;j<words.size();j++){
                        int l = words[j].find(":");
                        wrd = string(words[j],l+1,words[j].size());
                        if (wrd=="0"){
                            wrd="";
                        }
                    }
                }
                bool match = false;
                string suf = "";
                string pref = "";
                int pr;
                try {
                    if (wrd.size()>0){
                        pr = res[j].an_w.find(wrd);
                        suf = string(res[j].an_w,pr+wrd.size(),res[j].an_w.size());
                        //cout << res[j].an_w+" "+suf+" "+wrd+" "<<pr << endl;
                        if (pr>=0){
                            pref = string(res[j].an_w,0,pr);
                        }
                    } else {
                        suf = res[j].an_w;
                    }
                } catch (std::out_of_range er) {
                    cout << "ERORR: "+res[j].an_w+"  "+wrd<< endl;
                }
                int number_in_par = -1;
                if (pr>=0){
                    for (int k=0;k<paradigms.size();k++){
                        if (paradigms[k].p_cl==res[j].P_cl){
                            match = true;
                            bool match2 = false;
                            for (int p=0; p<paradigms[k].suffixes.size(); p++) {
                                if (paradigms[k].suffixes[p]==suf){
                                    match2 = true;
                                    number_in_par = p;
                                    break;
                                }
                            }
                            if (!match2){
                                GramInfo tmp;
                                convert(res[j], tmp);
                                paradigms[k].grammems.push_back(tmp);
                                paradigms[k].gramm_int.push_back(-1);
                                paradigms[k].suff_int.push_back(-1);
                                paradigms[k].suffixes.push_back(suf);
                                paradigms[k].prefixes.push_back(pref);
                                number_in_par = paradigms[k].suffixes.size() -1;
                            }
                            break;
                        }
                    }
                    if (!match){
                        par tmp;
                        tmp.par_num = i;
                        tmp.p_cl = res[j].P_cl;
                        GramInfo tmp2;
                        convert(res[j], tmp2);
                        tmp.grammems.push_back(tmp2);
                        tmp.suffixes.push_back(suf);
                        tmp.prefixes.push_back(pref);
                        tmp.suff_int.push_back(-1);
                        tmp.gramm_int.push_back(-1);
                        paradigms.push_back(tmp);
                        number_in_par = 0;
                    }
                }
                res[j].num_inP = number_in_par;
            }
        }
        cout << it/parSize*100.0<<endl;
        it++;
    }
}

void packPCL(vector<string> &suffixes,vector<GramInfo> &grammems, vector<par> &paradigms){
    for (int i=0;i<paradigms.size();i++){
        for (int j=0;j<paradigms[i].suffixes.size();j++){
            bool match = false;
            int matched = -1;
            for (int k=0;k<suffixes.size();k++){
                
                if (suffixes[k]==paradigms[i].suffixes[j]){
                    match=true;
                    matched = k;
                }
            }
            if (!match){
                suffixes.push_back(paradigms[i].suffixes[j]);
                int sInt = suffixes.size();
                paradigms[i].suff_int[j]=sInt-1;
            } else {
                paradigms[i].suff_int[j]=matched;
            }
        }
        for (int j=0;j<paradigms[i].grammems.size();j++){
            bool match = false;
            int matched = -1;
            for (int k=0;k<grammems.size();k++){
                
                if (check2(grammems[k], paradigms[i].grammems[j])){
                    match=true;
                    matched = k;
                }
            }
            if (!match){
                grammems.push_back(paradigms[i].grammems[j]);
                int sInt = grammems.size();
                paradigms[i].gramm_int[j]=sInt-1;
            } else {
                paradigms[i].gramm_int[j]=matched;
            }
        }
    }

}

void saveData(vector<RMUResult> res, vector<par> paradigms,vector<string> suffixes,vector<GramInfo> grammems){
    std::ofstream suf_file("suffixes.txt");
    std::ofstream gram_file("gram.data", std::ios::binary);
    std::ofstream par_file("par.data", std::ios::binary);
    std::ofstream words("words.txt");
    vector<par2> final_par;
    
    for (int i=0;i<res.size();i++){
        if (res[i].num_inP>=0)
            words << res[i].an_w +":"<<res[i].pcl<<","<<res[i].num_inP<<endl;
    }
    
    for (int i=0; i<paradigms.size(); i++) {
        int size = paradigms[i].gramm_int.size();
        par2 tmp;
        tmp.par_num = paradigms[i].par_num;
        tmp.size_of_par = size;
        tmp.gramm_int = new int [size];
        tmp.suf_int = new int [size];
        for (int j=0; j<size; j++) {
            tmp.gramm_int[j] = paradigms[i].gramm_int[j];
            tmp.suf_int[j] = paradigms[i].suff_int[j];
        }
        final_par.push_back(tmp);
    }
    
    for (int i=0; i<final_par.size(); i++) {
        par_file.write((char*)&final_par[i].par_num, sizeof(int));
        par_file.write((char*)&final_par[i].size_of_par, sizeof(int));
        par_file.write((char*)final_par[i].gramm_int, final_par[i].size_of_par*sizeof(int));
        par_file.write((char*)final_par[i].suf_int, final_par[i].size_of_par*sizeof(int));
    }
    
    for (int i=0;i<suffixes.size();i++){
        suf_file << suffixes[i] << endl;
    }
    for (int i=0;i<grammems.size();i++){
        gram_file.write((char*)&grammems[i], sizeof(GramInfo));
    }
    gram_file.close();
    par_file.close();
    suf_file.close();
    words.close();
}

void compileDicData(){
    dawgdic::Dictionary dic_stem;
    dic_stem.Read(&stem_file);
    dawgdic::Guide guide_stem;
    guide_stem.Read(&stem_file);
    
    vector<RMUResult> res;
    parseXMLtoRMU(res);
    
    vector<string> parMatch;
    extractPCL(res,parMatch);
    
    vector<par> paradigms;
    generatePar(parMatch, res, paradigms, &dic_stem, &guide_stem);
    
    
    vector<string> suffixes;
    vector<GramInfo> grammems;
    packPCL(suffixes, grammems, paradigms);
    
    saveData(res, paradigms, suffixes, grammems);

}

int main(int argc, const char * argv[]) {
    // insert code here...
    compileDicData();
    
//    cout << ">";
//    cout <<paradigms.size() << endl;
//    cout << res.size() << endl;
//    string inLine;
//    while(true){
//        cin >> inLine;
//        cout << "Analized word: "+inLine << endl;
//        for (int i=0; i<res.size(); i++) {
//            if (res[i].an_w == inLine){
//                cout << "found " +res[i].P_cl<< endl;
//                cout << res[i].Animate <<" " << res[i].Gender <<" " << res[i].Number <<" " <<res[i].Case <<" " << res[i].Reflection <<" " << res[i].Perfective <<" " <<res[i].Transitive <<" " << res[i].Person <<" " << res[i].Tense <<" " <<res[i].Voice <<" " <<res[i].Degree<<" " << res[i].Static << endl;
//                string par = res[i].P_cl;
//                vector<string> words;
//                string s = string(res[i].lexem,1,res[i].lexem.size()-2);
//                string wrd;
//                s=s+":";
//                Complete(dic_stem, guide_stem, s.c_str(), 1, words);
//                for (int j=0;j<words.size();j++){
//                    int l = words[j].find(":");
//                    wrd = string(words[j],l+1,words[j].size());
//                    if (wrd=="0"){
//                        wrd="";
//                    }
//                }
//                if (words.size() == 0){
//                    replace2(s, "ё", "е");
//                    Complete(dic_stem, guide_stem, s.c_str(), 1, words);
//                    for (int j=0;j<words.size();j++){
//                        int l = words[j].find(":");
//                        wrd = string(words[j],l+1,words[j].size());
//                        if (wrd=="0"){
//                            wrd="";
//                        }
//                    }
//                }
//                string suf;
//                if (wrd.size()>0)
//                    suf = string(res[i].an_w,wrd.size(),res[i].an_w.size());
//                else
//                    suf = res[i].an_w;
//                for (int j=0; j<paradigms.size(); j++) {
//                    if (paradigms[j].p_cl == par){
//                        cout << "Paradigm size: "<<paradigms[j].suffixes.size() <<endl;
//                        for (int k=0; k<paradigms[j].suffixes.size(); k++) {
//                            string w = string(inLine,0,inLine.size()-suf.size())+paradigms[j].suffixes[k];
//                            int ch=0;
//                            for (int z=0;z<res.size();z++) {
//                                if (res[z].an_w == w){
//                                    if (check(res[z], paradigms[j].grammems[k])){
//                                        ch+=1;
//                                    } else {
//                                        ch-=1;
//                                    }
//                                    break;
//                                }
//                            }
//                            cout << paradigms[j].prefixes[k]+" + "+string(inLine,0,inLine.size()-suf.size())+paradigms[j].suffixes[k] +"  "<< paradigms[j].grammems[k].S_cl <<"  "<<paradigms[j].grammems[k].Animate <<" " << paradigms[j].grammems[k].Gender <<" " << paradigms[j].grammems[k].Number <<" " <<paradigms[j].grammems[k].Case <<" " << paradigms[j].grammems[k].Reflection <<" " << paradigms[j].grammems[k].Perfective <<" " <<paradigms[j].grammems[k].Transitive <<" " << paradigms[j].grammems[k].Person <<" " << paradigms[j].grammems[k].Tense <<" " <<paradigms[j].grammems[k].Voice <<" " <<paradigms[j].grammems[k].Degree<<" " << paradigms[j].grammems[k].Static <<" + "<< ch << endl;
//                        }
//                    }
//                }
//            }
//        }
//    }
    return 0;
}
