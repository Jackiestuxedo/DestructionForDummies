-- Autogenerated file.  Do not edit.  All changes will be lost
--
-- Definition of Arcane Mage

DestroDummies.classMageArcane = DestroDummies.lastClassVal + 1;
DestroDummies.lastClassVal = DestroDummies.lastClassVal + 1;

local mIndexBase = DestroDummies.lastMutexIndex;

local varrotSpot = 0;
DestroDummies.mutex[mIndexBase + 1] = {};
DestroDummies.mutex[mIndexBase + 1].clipOOCombat = 300;
DestroDummies.mutex[mIndexBase + 1].clipCombat = 5;
DestroDummies.mutex[mIndexBase + 1][1] = DestroDummies:xlateString("Molten Armor")
DestroDummies.mutex[mIndexBase + 1][2] = DestroDummies:xlateString("Mage Armor")
DestroDummies.mutex[mIndexBase + 1][3] = DestroDummies:xlateString("Frost Armor")
DestroDummies.mutex[mIndexBase + 1].count = 3;
DestroDummies.mutex[mIndexBase + 2] = {};
DestroDummies.mutex[mIndexBase + 2].clipOOCombat = 300;
DestroDummies.mutex[mIndexBase + 2].clipCombat = 5;
DestroDummies.mutex[mIndexBase + 2][1] = DestroDummies:xlateString("Arcane Brilliance")
DestroDummies.mutex[mIndexBase + 2][2] = DestroDummies:xlateString("Dalaran Brilliance")
DestroDummies.mutex[mIndexBase + 2].count = 2;
local varlastSpell = nil;
local function spellSent(varspell)
    varlastSpell = varspell
end
DestroDummies.registry.spellSent[DestroDummies.classMageArcane] = spellSent;

local function postSpell(varspell, varsuccess)
    varlastSpell = nil
end
DestroDummies.registry.postSpell[DestroDummies.classMageArcane] = postSpell;

local function PickCombat()
    DestroDummies:analysisInit();
    local clipArcaneBrilliance = 5;
    if (not (DestroDummies:utilMxBuffByMe(mIndexBase + 1))) then
        DestroDummies:analysisAdd(DestroDummies:xlateString("Mage Armor"));
    end;
    if (not (DestroDummies:utilMxBuff(mIndexBase + 2))) then
        DestroDummies:analysisAdd(DestroDummies:xlateString("Arcane Brilliance"));
    end;
    if (DestroDummies:spellReady(DestroDummies:xlateString("Icy Veins"))) then
        DestroDummies:altRecommend(DestroDummies:xlateString("Icy Veins"), 0);
    end;
    if (DestroDummies:spellReady(DestroDummies:xlateString("Presence of Mind"))) then
        DestroDummies:altRecommend(DestroDummies:xlateString("Presence of Mind"), 0);
    end;
    if (DestroDummies:spellReady(DestroDummies:xlateString("Arcane Power"))) then
        DestroDummies:altRecommend(DestroDummies:xlateString("Arcane Power"), 0);
    end;
    if (((DestroDummies:utilBuff(DestroDummies:xlateString("Icy Veins"), 0, "player", "HELPFUL", nil)) and ((DestroDummies:utilBuffTime(DestroDummies:xlateString("Icy Veins"))) < (2))) and ((((DestroDummies:unitPowerAfterCast() * 100) / UnitPowerMax("player"))) < (45))) then
        DestroDummies:analysisAdd(DestroDummies:xlateString("Evocation"));
    end;
    local varabBuffTime;
    local varabBuffCount;
    varabBuffTime, varabBuffCount = DestroDummies:utilBuffInfo(DestroDummies:xlateString("Arcane Blast"), true, "player")
    if (varlastSpell) == (DestroDummies:xlateString("Arcane Blast")) then
        varabBuffCount = (varabBuffCount) + (1)
    end;
    if (((varabBuffCount) >= (1)) and ((varabBuffCount) < (3))) then
        DestroDummies:analysisAdd(DestroDummies:xlateString("Arcane Blast"));
    end;
    if (DestroDummies:spellReady(DestroDummies:xlateString("Arcane Barrage"))) then
        DestroDummies:analysisAdd(DestroDummies:xlateString("Arcane Barrage"));
    end;
    if (DestroDummies:spellReady(DestroDummies:xlateString("Arcane Missiles"))) then
        DestroDummies:analysisAdd(DestroDummies:xlateString("Arcane Missiles"));
    end;
    DestroDummies:analysisAdd(DestroDummies:xlateString("Arcane Blast"));
    DestroDummies:analysisAdd(DestroDummies:xlateString("Fire Blast"));
    DestroDummies:analysisAdd(DestroDummies:xlateString("Fireball"));
    return DestroDummies:analysisPick("Evocation", nil);
end;

local function PickNoCombat()
    DestroDummies:analysisInit();
    if (not (DestroDummies:utilMxBuffByMe(mIndexBase + 1))) then
        DestroDummies:analysisAdd(DestroDummies:xlateString("Mage Armor"));
    end;
    if (not (DestroDummies:utilMxBuff(mIndexBase + 2))) then
        DestroDummies:analysisAdd(DestroDummies:xlateString("Arcane Brilliance"));
    end;
    return DestroDummies:analysisPick(nil, nil);
end;

-- populate our registries
DestroDummies.registry.decideCombat[DestroDummies.classMageArcane] = PickCombat;
DestroDummies.registry.decideNoCombat[DestroDummies.classMageArcane] = PickNoCombat;
DestroDummies.lastMutexIndex = mIndexBase + 2;