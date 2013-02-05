-- Autogenerated file.  Do not edit.  All changes will be lost
--
-- Definition of Assassination Rogue

DestroDummies.classRogueAssassination = DestroDummies.lastClassVal + 1;
DestroDummies.lastClassVal = DestroDummies.lastClassVal + 1;

local mIndexBase = DestroDummies.lastMutexIndex;

local vartalentChase;
local vartalentMurderous;
local function preCombat()
    vartalentChase = DestroDummies:utilTalentCount(DestroDummies:xlateString("Cut to the Chase"))
    vartalentMurderous = DestroDummies:utilTalentCount(DestroDummies:xlateString("Murderous Intent"))
end
DestroDummies.registry.preCombat[DestroDummies.classRogueAssassination] = preCombat;

local function PickCombat()
    DestroDummies:analysisInit();
    if ((not (DestroDummies:utilBuff(DestroDummies:xlateString("Overkill"), 0, "player", "HELPFUL", nil))) and (DestroDummies:spellReady(DestroDummies:xlateString("Vanish")))) then
        DestroDummies:altRecommend(DestroDummies:xlateString("Vanish"), 0);
    end;
    if (DestroDummies:spellReady(DestroDummies:xlateString("Cold Blood"))) then
        DestroDummies:altRecommend(DestroDummies:xlateString("Cold Blood"), 0);
    end;
    if (DestroDummies:spellReady(DestroDummies:xlateString("Vendetta"))) then
        DestroDummies:altRecommend(DestroDummies:xlateString("Vendetta"), 0);
    end;
    local clipSliceAndDice = 1.50;
    local varcomboPts = GetComboPoints("player", "target");
    local varsndTime = DestroDummies:utilBuffTime(DestroDummies:xlateString("Slice and Dice"));
    if (DestroDummies:spellReady(DestroDummies:xlateString("Garrote"))) then
        DestroDummies:analysisAdd(DestroDummies:xlateString("Garrote"));
    end;
    if (varcomboPts) > (0) then
        if (varsndTime) == (0) then
            DestroDummies:analysisAdd(DestroDummies:xlateString("Slice and Dice"));
        else
            if not (DestroDummies:utilBuff(DestroDummies:xlateString("Slice and Dice"), clipSliceAndDice, "player", "HELPFUL", nil)) then
                if (((vartalentChase) > (0)) and (DestroDummies:utilDebuff(DestroDummies:xlateString("Deadly Poison"), 0))) then
                    DestroDummies:analysisAdd(DestroDummies:xlateString("Envenom"));
                end;
                DestroDummies:analysisAdd(DestroDummies:xlateString("Slice and Dice"));
            end;
        end;
    end;
    local varfinish;
    local vardoBackstab = ((DestroDummies:utilHealth("target")) < (35)) and ((vartalentMurderous) > (0));
    if vardoBackstab then
        varfinish = (varcomboPts) == (5)
    else
        varfinish = (varcomboPts) >= (4)
    end;
    if ((not (DestroDummies:utilDebuff(DestroDummies:xlateString("Rupture"), 0))) and (varfinish)) then
        DestroDummies:analysisAdd(DestroDummies:xlateString("Rupture"));
    end;
    if ((varfinish) and (DestroDummies:utilDebuff(DestroDummies:xlateString("Deadly Poison"), 0))) then
        DestroDummies:analysisAdd(DestroDummies:xlateString("Envenom"));
    end;
    if (varfinish) then
        DestroDummies:analysisAdd(DestroDummies:xlateString("Eviscerate"));
    end;
    if (vardoBackstab) then
        DestroDummies:analysisAdd(DestroDummies:xlateString("Backstab"));
    end;
    DestroDummies:analysisAdd(DestroDummies:xlateString("Mutilate"));
    return DestroDummies:analysisPick(nil, nil);
end;

local function PickNoCombat()
    DestroDummies:analysisInit();
    local clipPoisons = (5) * (60);
    if not (IsMounted()) then
        if (not (DestroDummies:utilWeaponEnhance(clipPoisons))) then
            DestroDummies:analysisAdd(DestroDummies:xlateString("Poisons"));
        end;
    end;
    return DestroDummies:analysisPick(nil, nil);
end;

-- populate our registries
DestroDummies.registry.decideCombat[DestroDummies.classRogueAssassination] = PickCombat;
DestroDummies.registry.decideNoCombat[DestroDummies.classRogueAssassination] = PickNoCombat;
DestroDummies.lastMutexIndex = mIndexBase + 0;