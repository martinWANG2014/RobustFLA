//
// Created by chenghaowang on 02/06/17.
//

#ifndef ROBUSTFLA_GLOBALDEFINITION_H
#define ROBUSTFLA_GLOBALDEFINITION_H

#include "predefine.h"

static IntVector LevelIFRA{10, 30, 50, 70, 90, 110, 130, 150, 170, 190, 210, 230, 250, 270, 290, 330, 370, 410, 450, 490};
static IntVector LevelIFRB{20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240, 260, 280, 310, 350, 390, 430, 470, 510};
static IntVector LevelVFRA{35, 55, 75, 95, 115, 135, 155, 175, 195};
static IntVector LevelVFRB{45, 65, 85, 105, 125, 145, 165, 185};

IntVector findFeasibleLevels(int iDefaultLevel){
    IntVector feasibleList;
    //auto position;
    if (iDefaultLevel < 290){
        auto position = LevelIFRA.end();
        switch (iDefaultLevel % 4){
            // in IFR B group
            case 0:
                position = std::find(LevelIFRB.begin(), LevelIFRB.end(), iDefaultLevel);
                if(position == LevelIFRB.begin()){
                    feasibleList.push_back(*(position + 1));
                    feasibleList.push_back(*(position + 2));
                }else{
                    feasibleList.push_back(*(position + 1));
                    feasibleList.push_back(*(position - 1));
                }
                break;
            // in VFR B group
            case 1:
                position = std::find(LevelVFRB.begin(), LevelVFRB.end(), iDefaultLevel);
                if(position == LevelVFRB.begin()){
                    feasibleList.push_back(*(position + 1));
                    feasibleList.push_back(*(position + 2));
                }else if(position == LevelVFRB.end() - 1){
                    feasibleList.push_back(*(position - 1));
                    feasibleList.push_back(*(position - 2));
                }else{
                    feasibleList.push_back(*(position + 1));
                    feasibleList.push_back(*(position - 1));
                }
                break;
            // in IFR A group
            case 2:
                position = std::find(LevelIFRA.begin(), LevelIFRA.end(), iDefaultLevel);
                if(position == LevelIFRA.begin()){
                    feasibleList.push_back(*(position + 1));
                    feasibleList.push_back(*(position + 2));
                }else{
                    feasibleList.push_back(*(position + 1));
                    feasibleList.push_back(*(position - 1));
                }
                break;
            // in VFR A group
            case 3:
                position = std::find(LevelVFRA.begin(), LevelVFRA.end(), iDefaultLevel);
                if(position == LevelVFRA.begin()){
                    feasibleList.push_back(*(position + 1));
                    feasibleList.push_back(*(position + 2));
                }else if(position == LevelVFRA.end() - 1){
                    feasibleList.push_back(*(position - 1));
                    feasibleList.push_back(*(position - 2));
                }else{
                    feasibleList.push_back(*(position + 1));
                    feasibleList.push_back(*(position - 1));
                }
                break;
        }
    }
    else if (iDefaultLevel % 40 == 10){
        // in IFR A group
        auto position = std::find(LevelIFRA.begin(), LevelIFRA.end(), iDefaultLevel);
        if(position == LevelIFRA.end() - 1){
            feasibleList.push_back(*(position - 1));
            feasibleList.push_back(*(position - 2));
        }else{
            feasibleList.push_back(*(position + 1));
            feasibleList.push_back(*(position - 1));
        }
    }else{
        // in IFR B group
        auto position = std::find(LevelIFRB.begin(), LevelIFRB.end(), iDefaultLevel);
        if(position == LevelIFRB.end() - 1){
            feasibleList.push_back(*(position - 1));
            feasibleList.push_back(*(position - 2));
        }else{
            feasibleList.push_back(*(position + 1));
            feasibleList.push_back(*(position - 1));
        }
    }
    return feasibleList;
}
#endif //ROBUSTFLA_GLOBALDEFINITION_H
