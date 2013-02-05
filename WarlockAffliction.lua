-- Autogenerated file.  Do not edit.  All changes will be lost
--
-- Definition of Affliction Warlock

DestroDummies.classWarlockAffliction = DestroDummies.lastClassVal + 1;
DestroDummies.lastClassVal = DestroDummies.lastClassVal + 1;

local mIndexBase = DestroDummies.lastMutexIndex;

DestroDummies.mutex[mIndexBase + 1] = {};
DestroDummies.mutex[mIndexBase + 1].clipCombat = 0;
DestroDummies.mutex[mIndexBase + 1].clipOOCombat = 0;
DestroDummies.mutex[mIndexBase + 1][1] = DestroDummies:xlateString("Bane of Agony")
DestroDummies.mutex[mIndexBase + 1][2] = DestroDummies:xlateString("Bane of Doom")
DestroDummies.mutex[mIndexBase + 1][3] = DestroDummies:xlateString("Bane of Havoc")
DestroDummies.mutex[mIndexBase + 1].count = 3;
DestroDummies.mutex[mIndexBase + 2] = {};
DestroDummies.mutex[mIndexBase + 2].clipCombat = 1;
DestroDummies.mutex[mIndexBase + 2].clipOOCombat = 1;
DestroDummies.mutex[mIndexBase + 2][1] = DestroDummies:xlateString("Curse of the Elements")
DestroDummies.mutex[mIndexBase + 2][2] = DestroDummies:xlateString("Curse of Tongues")
DestroDummies.mutex[mIndexBase + 2][3] = DestroDummies:xlateString("Curse of Weakness")
DestroDummies.mutex[mIndexBase + 2][4] = DestroDummies:xlateString("Curse of Exhaustion")
DestroDummies.mutex[mIndexBase + 2].count = 4;
DestroDummies.mutex[mIndexBase + 3] = {};
DestroDummies.mutex[mIndexBase + 3].clipCombat = 1;
DestroDummies.mutex[mIndexBase + 3].clipOOCombat = 1;
DestroDummies.mutex[mIndexBase + 3][1] = DestroDummies:xlateString("Unstable Affliction")
DestroDummies.mutex[mIndexBase + 3][2] = DestroDummies:xlateString("Immolate")
DestroDummies.mutex[mIndexBase + 3].count = 2;
DestroDummies.mutex[mIndexBase + 4] = {};
DestroDummies.mutex[mIndexBase + 4].clipCombat = 1;
DestroDummies.mutex[mIndexBase + 4].clipOOCombat = 1;
DestroDummies.mutex[mIndexBase + 4][1] = DestroDummies:xlateString("Corruption")
DestroDummies.mutex[mIndexBase + 4][2] = DestroDummies:xlateString("Seed of Corruption")
DestroDummies.mutex[mIndexBase + 4].count = 2;
DestroDummies.mutex[mIndexBase + 5] = {};
DestroDummies.mutex[mIndexBase + 5].clipOOCombat = 300;
DestroDummies.mutex[mIndexBase + 5].clipCombat = 5;
DestroDummies.mutex[mIndexBase + 5][1] = DestroDummies:xlateString("Fel Armor")
DestroDummies.mutex[mIndexBase + 5][2] = DestroDummies:xlateString("Demon Armor")
DestroDummies.mutex[mIndexBase + 5].count = 2;
DestroDummies.mutex[mIndexBase + 6] = {};
DestroDummies.mutex[mIndexBase + 6].clipCombat = 1;
DestroDummies.mutex[mIndexBase + 6].clipOOCombat = 1;
DestroDummies.mutex[mIndexBase + 6][1] = DestroDummies:xlateString("Shadow and Flame")
DestroDummies.mutex[mIndexBase + 6][2] = DestroDummies:xlateString("Critical Mass")
DestroDummies.mutex[mIndexBase + 6][3] = DestroDummies:xlateString("Winter's Chill")
DestroDummies.mutex[mIndexBase + 6].count = 3;
DestroDummies.mutex[mIndexBase + 7] = {};
DestroDummies.mutex[mIndexBase + 7].clipCombat = 1;
DestroDummies.mutex[mIndexBase + 7].clipOOCombat = 1;
DestroDummies.mutex[mIndexBase + 7][1] = DestroDummies:xlateString("Heroism")
DestroDummies.mutex[mIndexBase + 7][2] = DestroDummies:xlateString("Bloodlust")
DestroDummies.mutex[mIndexBase + 7][3] = DestroDummies:xlateString("Time Warp")
DestroDummies.mutex[mIndexBase + 7][4] = DestroDummies:xlateString("Ancient Hysteria")
DestroDummies.mutex[mIndexBase + 7][5] = DestroDummies:xlateString("Improved Soul Fire")
DestroDummies.mutex[mIndexBase + 7].count = 5;
local varHaveImpSF;
local varHaveSandF;
local varHaveShadowEmbrace;
local varbossFight;
local function preCombat()
    varHaveShadowEmbrace = ((DestroDummies:utilTalentCount(DestroDummies:xlateString("Shadow Embrace"))) > (1))
    varHaveSandF = (DestroDummies:utilTalentCount(DestroDummies:xlateString("Shadow and Flame"))) > (1)
    varHaveImpSF = (DestroDummies:utilTalentCount(DestroDummies:xlateString("Improved Soul Fire"))) > (1)
    varbossFight = nil
end
DestroDummies.registry.preCombat[DestroDummies.classWarlockAffliction] = preCombat;

local function PickCombat()
    DestroDummies:analysisInit();
    local clipHaunt = DestroDummies:utilFlightTime();
    if (DestroDummies:spellReady(DestroDummies:xlateString("Summon Doomguard"))) then
        DestroDummies:altRecommend(DestroDummies:xlateString("Summon Doomguard"), 1);
    end;
    if (DestroDummies:spellReady(DestroDummies:xlateString("Demon Soul"))) then
        DestroDummies:altRecommend(DestroDummies:xlateString("Demon Soul"), 3);
    end;
    if ((DestroDummies:utilHealth("pet")) < (40)) then
        DestroDummies:altRecommend(DestroDummies:xlateString("Health Funnel"), 4);
    end;
    varbossFight = (DestroDummies:utilBoss()) or (varbossFight)
    if ((((varbossFight) and (not (DestroDummies:utilBoss()))) and ((DestroDummies:utilTimeToDie()) < (5))) and ((UnitPower("player", SPELL_POWER_SOUL_SHARDS)) < (2))) then
        DestroDummies:analysisAdd(DestroDummies:xlateString("Shadowburn"));
    end;
    if (not (DestroDummies:utilMxBuffByMe(mIndexBase + 5))) then
        DestroDummies:analysisAdd(DestroDummies:xlateString("Fel Armor"));
    end;
    if (not (DestroDummies:utilBuff(DestroDummies:xlateString("Dark Intent"), 0, "player", "HELPFUL", nil))) then
        DestroDummies:analysisAdd(DestroDummies:xlateString("Dark Intent"));
    end;
    if (((not (DestroDummies:utilMxDebuffByMe(mIndexBase + 2))) and (not (DestroDummies:utilDebuff(DestroDummies:xlateString("Curse of the Elements"), 0)))) and (DestroDummies:utilBoss())) then
        DestroDummies:analysisAdd(DestroDummies:xlateString("Curse of the Elements"));
    end;
    if ((not ((DestroDummies.lastSpell == DestroDummies:xlateString("Soul Fire")))) and (((((varHaveImpSF) and (not (DestroDummies:utilMxBuff(mIndexBase + 7))))) or (DestroDummies:utilBuff(DestroDummies:xlateString("Soulburn"), 0, "player", "HELPFUL", nil))))) then
        DestroDummies:analysisAdd(DestroDummies:xlateString("Soul Fire"));
    end;
    if (((DestroDummies:utilBuff(DestroDummies:xlateString("Nightfall"), 0, "player", "HELPFUL", nil)) or (DestroDummies:utilBuff(DestroDummies:xlateString("Shadow Trance"), 0, "player", "HELPFUL", nil))) or (((((IsInRaid()) and (not ((DestroDummies.lastSpell == DestroDummies:xlateString("Shadow Bolt"))))) and (varHaveSandF)) and (not (DestroDummies:utilMxDebuff(mIndexBase + 6)))))) then
        DestroDummies:analysisAdd(DestroDummies:xlateString("Shadow Bolt"));
    end;
    if (not (DestroDummies:utilMxDebuffByMe(mIndexBase + 4))) then
        DestroDummies:analysisAdd(DestroDummies:xlateString("Corruption"));
    end;
    if ((not (DestroDummies:utilMxDebuffByMe(mIndexBase + 3))) and (not ((DestroDummies.lastSpell == DestroDummies:xlateString("Unstable Affliction"))))) then
        DestroDummies:analysisAdd(DestroDummies:xlateString("Unstable Affliction"));
    end;
    if ((not (DestroDummies:utilMxDebuffByMe(mIndexBase + 1))) and ((DestroDummies:utilTimeToDie()) > (15))) then
        DestroDummies:analysisAdd(DestroDummies:xlateString("Bane of Doom"));
    end;
    if (not ((DestroDummies.lastSpell == DestroDummies:xlateString("Haunt")))) then
        DestroDummies:analysisAdd(DestroDummies:xlateString("Haunt"));
    end;
    if varHaveShadowEmbrace then
        local varSEtime;
        local varSEcount;
        varSEtime, varSEcount = DestroDummies:utilBuffInfo(DestroDummies:xlateString("Shadow Embrace"), true, "target")
        if ((varSEcount) < (3)) then
            DestroDummies:analysisAdd(DestroDummies:xlateString("Shadow Bolt"));
        end;
    end;
    if ((DestroDummies:utilHealth("target")) < (25)) then
        DestroDummies:analysisAdd(DestroDummies:xlateString("Drain Soul"));
    end;
    if ((DestroDummies:distanceToTarget()) <= (11.10)) then
        DestroDummies:analysisAdd(DestroDummies:xlateString("Shadowflame"));
    end;
    DestroDummies:analysisAdd(DestroDummies:xlateString("Shadow Bolt"));
    return DestroDummies:analysisPick("Life Tap", nil);
end;

local function PickNoCombat()
    DestroDummies:analysisInit();
    if not (IsMounted()) then
        if (not (DestroDummies:utilMxBuffByMe(mIndexBase + 5))) then
            DestroDummies:analysisAdd(DestroDummies:xlateString("Fel Armor"));
        end;
        if (not (HasPetUI())) then
            DestroDummies:analysisAdd(DestroDummies:xlateString("Summon Felhunter"));
        end;
        if (not (HasPetUI())) then
            DestroDummies:analysisAdd(DestroDummies:xlateString("Summon Imp"));
        end;
        if (not (DestroDummies:utilBuff(DestroDummies:xlateString("Dark Intent"), 0, "player", "HELPFUL", nil))) then
            DestroDummies:analysisAdd(DestroDummies:xlateString("Dark Intent"));
        end;
        if ((not (DestroDummies:utilBuff(DestroDummies:xlateString("Soul Harvest"), 0, "player", "HELPFUL", nil))) and ((((UnitPower("player", SPELL_POWER_SOUL_SHARDS)) < (3)) or ((DestroDummies:utilHealth("player")) < (95))))) then
            DestroDummies:analysisAdd(DestroDummies:xlateString("Soul Harvest"));
        end;
    end;
    return DestroDummies:analysisPick("Life Tap", nil);
end;

-- populate our registries
DestroDummies.registry.decideCombat[DestroDummies.classWarlockAffliction] = PickCombat;
DestroDummies.registry.decideNoCombat[DestroDummies.classWarlockAffliction] = PickNoCombat;
DestroDummies.lastMutexIndex = mIndexBase + 7;
