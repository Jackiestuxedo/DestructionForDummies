-- Autogenerated file.  Do not edit.  All changes will be lost
--
-- Definition of Marksman Hunter

DestroDummies.classHunterMarksman = DestroDummies.lastClassVal + 1;
DestroDummies.lastClassVal = DestroDummies.lastClassVal + 1;

local mIndexBase = DestroDummies.lastMutexIndex;

DestroDummies.mutex[mIndexBase + 1] = {};
DestroDummies.mutex[mIndexBase + 1].clipCombat = 0;
DestroDummies.mutex[mIndexBase + 1].clipOOCombat = 0;
DestroDummies.mutex[mIndexBase + 1][1] = DestroDummies:xlateString("Serpent Sting")
DestroDummies.mutex[mIndexBase + 1][2] = DestroDummies:xlateString("Wyvern Sting")
DestroDummies.mutex[mIndexBase + 1].count = 2;
DestroDummies.mutex[mIndexBase + 2] = {};
DestroDummies.mutex[mIndexBase + 2].clipCombat = 0;
DestroDummies.mutex[mIndexBase + 2].clipOOCombat = 0;
DestroDummies.mutex[mIndexBase + 2][1] = DestroDummies:xlateString("Marked for Death")
DestroDummies.mutex[mIndexBase + 2][2] = DestroDummies:xlateString("Hunter's Mark")
DestroDummies.mutex[mIndexBase + 2].count = 2;
local varsteadyShots;
local varhaveMFD;
local function preCombat()
    varsteadyShots = 0
    varhaveMFD = DestroDummies:utilHasGlyph(DestroDummies:xlateString("Marked for Death"))
end
DestroDummies.registry.preCombat[DestroDummies.classHunterMarksman] = preCombat;

local function spellSent(varspell)
    if (varspell) == (DestroDummies:xlateString("Steady Shot")) then
        varsteadyShots = (varsteadyShots) + (1)
    else
        varsteadyShots = 0
    end;
end
DestroDummies.registry.spellSent[DestroDummies.classHunterMarksman] = spellSent;

local function PickCombat()
    DestroDummies:analysisInit();
    local varpHealth;
    varpHealth = DestroDummies:utilHealth("pet")
    if (((varpHealth) == (nil)) or ((varpHealth) == (0))) then
        DestroDummies:altRecommend(DestroDummies:xlateString("Revive Pet"), 0);
    end;
    if (DestroDummies:spellReady(DestroDummies:xlateString("Silencing Shot"))) then
        DestroDummies:altRecommend(DestroDummies:xlateString("Silencing Shot"), 2);
    end;
    if ((DestroDummies:spellReady(DestroDummies:xlateString("Kill Command"))) and (not (DestroDummies:utilBuff(DestroDummies:xlateString("Kill Command"), 0, "pet", "HELPFUL", nil)))) then
        DestroDummies:altRecommend(DestroDummies:xlateString("Kill Command"), 1);
    end;
    if (DestroDummies:spellReady(DestroDummies:xlateString("Stampede"))) then
        DestroDummies:altRecommend(DestroDummies:xlateString("Stampede"), 3);
    end;
    if (DestroDummies:spellReady(DestroDummies:xlateString("Readiness"))) then
        DestroDummies:altRecommend(DestroDummies:xlateString("Readiness"), 4);
    end;
    if (varpHealth) == (nil) then
        varpHealth = 0
    end;
    if (((not (DestroDummies:utilBuff(DestroDummies:xlateString("Mend Pet"), 0, "pet", "HELPFUL", nil))) and ((varpHealth) > (0))) and ((varpHealth) < (50))) then
        DestroDummies:analysisAdd(DestroDummies:xlateString("Mend Pet"));
    end;
    DestroDummies:genfunc_DoAspect(1, varranged);
    if DestroDummiesPCdb.ferralMode then
        if (((not (DestroDummies:utilMxDebuffByMe(mIndexBase + 1))) and (not ((DestroDummies.lastSpell == DestroDummies:xlateString("Serpent Sting"))))) and ((DestroDummies:utilTimeToDie()) > (12))) then
            DestroDummies:analysisAdd(DestroDummies:xlateString("Serpent Sting"));
        end;
    else
        local varphase;
        local varCA;
        local varST;
        local varKS;
        varCA = 0
        varST = 1
        varKS = 2
        if (DestroDummies:utilHealth("target")) > (90) then
            varphase = varCA
        else
            if (DestroDummies:utilHealth("target")) > (20) then
                varphase = varST
            else
                varphase = varKS
            end;
        end;
        if (((varsteadyShots) == (1)) and ((DestroDummies:utilBuffTime(DestroDummies:xlateString("Improved Steady Shot"))) < (6))) then
            DestroDummies:analysisAdd(DestroDummies:xlateString("Steady Shot"));
        end;
        if ((((not (DestroDummies:utilMxDebuffByMe(mIndexBase + 1))) and (not ((DestroDummies.lastSpell == DestroDummies:xlateString("Serpent Sting"))))) and ((DestroDummies:utilTimeToDie()) > (15)))) then
            DestroDummies:analysisAdd(DestroDummies:xlateString("Serpent Sting"));
        end;
        if ((varphase) == (varKS)) then
            DestroDummies:analysisAdd(DestroDummies:xlateString("Kill Shot"));
        end;
        DestroDummies:analysisAdd(DestroDummies:xlateString("Rapid Fire"));
        if (not (DestroDummies:utilDebuffByMe(DestroDummies:xlateString("Murder of Crows"), 0))) then
            DestroDummies:analysisAdd(DestroDummies:xlateString("Murder of Crows"));
        end;
        DestroDummies:analysisAdd(DestroDummies:xlateString("Dire Beast"));
        if not (DestroDummies:utilMxDebuff(mIndexBase + 2)) then
            if varhaveMFD then
                DestroDummies:analysisAdd(DestroDummies:xlateString("Chimera Shot"));
                DestroDummies:analysisAdd(DestroDummies:xlateString("Arcane Shot"));
            else
                DestroDummies:analysisAdd(DestroDummies:xlateString("Hunter's Mark"));
            end;
        end;
        DestroDummies:analysisAdd(DestroDummies:xlateString("Chimera Shot"));
        if (((varphase) == (varST)) or ((varphase) == (varKS))) then
            DestroDummies:analysisAdd(DestroDummies:xlateString("Glaive Toss"));
        end;
        if (DestroDummies:utilBuff(DestroDummies:xlateString("Fire!"), 0, "player", "HELPFUL", nil)) then
            DestroDummies:analysisAdd(DestroDummies:xlateString("Aimed Shot"));
        end;
        if (((varphase) == (varST)) or ((varphase) == (varKS))) then
            DestroDummies:analysisAdd(DestroDummies:xlateString("Arcane Shot"));
        end;
        local varssTime;
        varssTime = DestroDummies:utilDebuffByMeInfo(DestroDummies:xlateString("Serpent Sting"))
        if (((varssTime) > (0)) and ((varssTime) < (2.50))) then
            DestroDummies:analysisAdd(DestroDummies:xlateString("Chimera Shot"));
        end;
        if (DestroDummies:utilRequiredPower(DestroDummies:xlateString("Aimed Shot"))) <= (DestroDummies:unitPowerAfterCast()) then
            if (((varssTime) > (0)) and ((varssTime) < (3.00))) then
                DestroDummies:analysisAdd(DestroDummies:xlateString("Chimera Shot"));
            end;
            DestroDummies:analysisAdd(DestroDummies:xlateString("Aimed Shot"));
        end;
        if (((not (DestroDummies:utilBuff(DestroDummies:xlateString("Mend Pet"), 0, "pet", "HELPFUL", nil))) and ((varpHealth) > (0))) and ((varpHealth) < (70))) then
            DestroDummies:analysisAdd(DestroDummies:xlateString("Mend Pet"));
        end;
        DestroDummies:analysisAdd(DestroDummies:xlateString("Steady Shot"));
    end;
    return DestroDummies:analysisPick("Steady Shot", nil);
end;

local function PickNoCombat()
    DestroDummies:analysisInit();
    if not (IsMounted()) then
        DestroDummies:genfunc_DoAspect(nil, nil);
        local varpHealth = DestroDummies:utilHealth("pet");
        local varwithPet = HasPetUI();
        if (varpHealth) == (nil) then
            DestroDummies:analysisAdd(DestroDummies:xlateString("Revive Pet"));
        else
            if (not (UnitExists("pet"))) then
                DestroDummies:analysisAdd(DestroDummies:xlateString("Call Pet"));
            end;
            if ((varwithPet) and (((((varpHealth) > (0)) and ((varpHealth) < (90))) and (not (DestroDummies:utilBuff(DestroDummies:xlateString("Mend Pet"), 0, "pet", "HELPFUL", nil)))))) then
                DestroDummies:analysisAdd(DestroDummies:xlateString("Mend Pet"));
            end;
        end;
    end;
    return DestroDummies:analysisPick(nil, nil);
end;

-- populate our registries
DestroDummies.registry.decideCombat[DestroDummies.classHunterMarksman] = PickCombat;
DestroDummies.registry.decideNoCombat[DestroDummies.classHunterMarksman] = PickNoCombat;
DestroDummies.lastMutexIndex = mIndexBase + 2;
