-- Class-specific information

-- array of spells that don't trigger the GCD
local noGCD = {};

local function noGCDaryNewClass(class)
    noGCD[class] = {};
    noGCD[class].count = 0;
    noGCD[class].gcd = 1.5;	-- all but rogues, kitties, and DKs
end

-- add a spell that doesn't trigger the GCD
local function noGCDaryAddSpell(class, spell)
    local count = noGCD[class].count + 1;

    noGCD[class].count = count;
    noGCD[class][count] = spell;
end

-- List taken from wowwiki.com/Cooldown
--
noGCDaryNewClass("MONK");

noGCDaryNewClass("DEATHKNIGHT");
noGCD["DEATHKNIGHT"].gcd = 1;	-- DKs have a 1 second GCD
noGCDaryAddSpell("DEATHKNIGHT", "Anti-Magic Shell");
noGCDaryAddSpell("DEATHKNIGHT", "Blood Tap");
noGCDaryAddSpell("DEATHKNIGHT", "Dark Command");
noGCDaryAddSpell("DEATHKNIGHT", "Empower Rune Weapon");
noGCDaryAddSpell("DEATHKNIGHT", "Hysteria");
noGCDaryAddSpell("DEATHKNIGHT", "Icebound Fortitude");
noGCDaryAddSpell("DEATHKNIGHT", "Lichborne");
noGCDaryAddSpell("DEATHKNIGHT", "Mind Freeze");
noGCDaryAddSpell("DEATHKNIGHT", "Rune Strike");
noGCDaryAddSpell("DEATHKNIGHT", "Rune Tap");
noGCDaryAddSpell("DEATHKNIGHT", "Vampiric Blood");

noGCDaryNewClass("DRUID");
noGCDaryAddSpell("DRUID", "Maul");
noGCDaryAddSpell("DRUID", "Nature's Swiftness");
noGCDaryAddSpell("DRUID", "Barkskin");

noGCDaryNewClass("HUNTER");
noGCDaryAddSpell("HUNTER", "Deterrence");
noGCDaryAddSpell("HUNTER", "Kill Command");
noGCDaryAddSpell("HUNTER", "Silencing Shot");

noGCDaryNewClass("MAGE");
noGCDaryAddSpell("MAGE", "Counterspell");
noGCDaryAddSpell("MAGE", "Cold Snap");
noGCDaryAddSpell("MAGE", "Combustion");
noGCDaryAddSpell("MAGE", "Arcane Power");
noGCDaryAddSpell("MAGE", "Presence of Mind");
noGCDaryAddSpell("MAGE", "Icy Veins");

noGCDaryNewClass("PALADIN");
noGCDaryAddSpell("PALADIN", "Divine Favor");

noGCDaryNewClass("PRIEST");
noGCDaryAddSpell("PRIEST", "Inner Focus");
noGCDaryAddSpell("PRIEST", "Guardian Spirit");

noGCDaryNewClass("ROGUE");
noGCD["ROGUE"].gcd = 1;		-- rogues have a 1 second GCD
noGCDaryAddSpell("ROGUE", "Cold Blood");
noGCDaryAddSpell("ROGUE", "Preparation");
noGCDaryAddSpell("ROGUE", "Evasion");
noGCDaryAddSpell("ROGUE", "Sprint");
noGCDaryAddSpell("ROGUE", "Vanish");

noGCDaryNewClass("SHAMAN");
noGCDaryAddSpell("SHAMAN", "Nature's Swiftness");
noGCDaryAddSpell("SHAMAN", "Elemental Mastery");
noGCDaryAddSpell("SHAMAN", "Wind Shock");

noGCDaryNewClass("WARLOCK");
noGCDaryAddSpell("WARLOCK", "Fel Domination");

noGCDaryNewClass("WARRIOR");
noGCDaryAddSpell("WARRIOR", "Charge");
noGCDaryAddSpell("WARRIOR", "Cleave");
noGCDaryAddSpell("WARRIOR", "Heroic Strike");
noGCDaryAddSpell("WARRIOR", "Intercept");
noGCDaryAddSpell("WARRIOR", "Shield Bash");
noGCDaryAddSpell("WARRIOR", "Shield Block");
noGCDaryAddSpell("WARRIOR", "Sweeping Strikes");

function DestroDummies:computeGCD(class, spell)
    local i, gcd;

    -- If this spell is on the no GCD list, return 0
    for i = 1,noGCD[class].count do
	if noGCD[class][i] == spell then
	    return 0;
	end
    end

    -- Get the class's gcd
    gcd = noGCD[class].gcd;

    -- the only hiccup is that kitty druids have a diff GCD than other druids
    if (class == DRUID) then
	local _,_,isKitty = GetShapeshiftFormInfo(3);
	if (isKitty) then
	    gcd = 1;
	end
    end

    -- Reduce GCD by spell haste.  Or is all haste the same now?  
    -- not yet implemented
    --local spellHaste, meleeHaste, rangedHaste;
    --spellHaste = GetCombatRating(20);
    --meleeHaste = GetCombatRating(18);
    --rangedHaste = GetCombatRating(19);

    -- Return the GCD
    return gcd;
end
