//
//  helper.cpp
//  extract
//
//  Created by Artur Artur on 28.04.16.
//  Copyright © 2016 Artur Artur. All rights reserved.
//

#include <stdio.h>
#include "RMUResult.h"
#include <string>
#include "helperHeader.h"
#include "dawgdic/completer.h"
#include "dawgdic/dawg-builder.h"
#include "dawgdic/dictionary.h"
#include "dawgdic/dictionary-builder.h"
#include "dawgdic/guide-builder.h"

using namespace std;

bool replace2(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

bool check2(GramInfo res, GramInfo fromVec){
    if (res.S_cl == fromVec.S_cl)
        if (res.Animate == fromVec.Animate )
            if (res.Gender == fromVec.Gender )
                if (res.Number == fromVec.Number )
                    if (res.Case == fromVec.Case )
                        if (res.Reflection == fromVec.Reflection )
                            if (res.Perfective == fromVec.Perfective )
                                if (res.Transitive == fromVec.Transitive )
                                    if (res.Person == fromVec.Person )
                                        if (res.Tense == fromVec.Tense )
                                            if (res.Voice == fromVec.Voice )
                                                if (res.Degree == fromVec.Degree )
                                                    if (res.Static == fromVec.Static )
                                                        return true;
    
    return false;
}



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

void convert(RMUResult result,GramInfo &output){
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