-- Util functions, called by dddl code
--

-- Map auras to the spells that cast them.  In almost every case, the return
-- value is the same as the aura.
local function spell4aura (aura)
   if aura == "Improved Soul Fire" then
      return "Soul Fire";
   end 
   if aura == "Sunfire" then
      return "Moonfire";
   end 
   
   return aura;
end

-- return true if the passed buff is up on the doofus, and will extend past the
-- clip time (a clip of 0 is no clip) plus the cast time.  False otherwise
function DestroDummies:utilBuff(buff, clip, doofus, auratype, caster)
    local time = DestroDummies:buffTime(buff);

    local name, _,_,_,_,_,expiry,castBy,_ = UnitAura(doofus,buff,nil, auratype);

    if (name == nil) then
        return false;
    end
    if (caster ~= nil and castBy ~= caster) then
       return false;
    end

    if (expiry == nil or expiry == 0) then
        -- no expiry means that the buff just stays up (cat form, aspects, etc)
        -- note that a 0 expiry means time 0, which is the computer's boot time
        -- so it's not really ever a valid time
        return true;
    end

    -- Get the cast time.  This *cannot* be cached because it will change as
    -- the player's haste changes, which can be triggered by items
    local spell;
    spell = spell4aura(buff);
    local _,_,_,_,_,_,castTime,_,_ = GetSpellInfo(spell);  -- in milliseconds
    if (castTime == nil) then
    	-- Checks for proc'cing buffs will have a nil cast time
    	castTime = 0;
    end
    castTime = castTime / 1000;		-- now in seconds

    -- Worry about lag
    local _,_,lag = GetNetStats();
    lag = lag / 1000;			-- convert to seconds

    return (expiry - GetTime() - castTime - lag) > clip;
end


-- return the amount of time left on the buff, or 0 if it does not exist.
-- NB: permanent buffs (cat form, hunter aspects, etc) will return 0 when
-- they are up also.  Using this routine presupposes that you know that the
-- spell has a time associated with it.
--
-- Note also that this returns true, not clipped, time, even if you have a 
-- clip specified for this spell
function DestroDummies:utilBuffTime(buff)
    local name, _,_, _, _, _, expiry, _, _ = UnitAura("player", buff);

    if (name == nil or expiry == 0) then
        return 0;
    else
        return expiry - GetTime();
    end
end


-- Return the amount of time left on the debuff on the target, or 0 if it 
-- does not exist.
--
-- If the debuff is up multiple times, return the time for the one you cast.
-- If you did not cast any, return the time for the one falling off last.
--
-- Note also that this returns true, not clipped, time, even if you have a 
-- clip specified for this spell
function DestroDummies:utilDebuffTime(debuff)
    local i, name, expiry, caster, lastExpiry;

    lastExpiry = 0;
    for i = 1,40 do
	name,_,_,_,_,_, expiry, caster, _ = UnitAura("target", i, "HARMFUL");
	if name == debuff then
	    -- This is the one we were looking for
	    if caster == "player" then
		return expiry - GetTime();
	    end

	    -- someone else cast it.  remember if it's the last to fall off
	    if expiry > lastExpiry then
		lastExpiry = expiry
	    end
	end
    end

    if (lastExpiry == 0) then
        return 0;
    else
        return lastExpiry - GetTime();
    end
end


-- return true if the passed debuff is up by anyone (and will extend past the
-- clip plus cast time), false otherwise.  A clip of 0 means no clip
function DestroDummies:utilDebuff(debuff, clip)
    if (clip == nil) then
    	clip = 0;
    end

    local name, expiry = DestroDummies:debuffUpByAny(debuff);
    if (name == nil) then
        return false;
    end

    -- Get the cast time.  This *cannot* be cached because it will change as
    -- the player's haste changes, which can be triggered by items
    --
    -- Note that for rogue poison debuffs, the cast time will be nil
    local spell;
    spell = spell4aura(debuff);
    local _,_,_,_,_,_,castTime,_,_ = GetSpellInfo(spell);  -- in milliseconds
    if castTime == nil then
    	castTime = 0;
    end
    castTime = castTime / 1000;		-- now in seconds

    -- Worry about lag
    local _,_,lag = GetNetStats();
    lag = lag / 1000;			-- convert to seconds

    return (expiry - GetTime() - castTime - lag) > clip;
end

-- return true if the passed debuff is up by me (and will extend past the
-- clip plus cast time), false otherwise.  A clip of 0 means no clip
function DestroDummies:utilDebuffByMe(debuff, clip)
    local name, expiry = DestroDummies:debuffUpByMe(debuff);
    
    if (name == nil) then
        return false;
    end

    -- Get the cast time.  This *cannot* be cached because it will change as
    -- the player's haste changes, which can be triggered by items
    local spell;
    spell = spell4aura(debuff);
    local _,_,_,_,_,_,castTime,_,_ = GetSpellInfo(spell);  -- in milliseconds
    if (castTime == nil) then
       -- This is odd, but can happen: this is a debuff we put up but cannot
       -- cast.  First seen with Sunfire for balance druids, which is what
       -- moonfire turns into while in solar eclipse.  We're just going to 
       -- treat this as an instant, which works for now.  But it's going to
       -- be wrong if we ever have an actual cast time in this situation.
       --
       -- The sunfire issue is now fixed with the call to spell4aura, but I
       -- am leaving this in here to catch future unhandled cases.
       castTime = 0;
    end 
    castTime = castTime / 1000;		-- now in seconds

    -- Worry about lag
    local _,_,lag = GetNetStats();
    lag = lag / 1000;			-- convert to seconds

    return (expiry - GetTime() - castTime - lag) > clip;
end

-- return the seconds left on the buff/debuff (or 0 if it's not up), and the
-- count of the buff/debuff stack (or 0 if it's not up)
function DestroDummies:utilBuffInfo(buff, isDebuff, cretin)
   local name, expiry, count;
   if (isDebuff and cretin == "target") then
      name, expiry, count = DestroDummies:debuffUpByAny(buff);
      if (expiry ~= nil) then
	 expiry = expiry - GetTime();
      end
   else
      if isDebuff then
	 expiry, count = DestroDummies:debuffTime(buff, cretin);
      else
	 expiry, count = DestroDummies:buffTime(buff, cretin);
      end
      name = buff;
   end

   if (name == nil or expiry == nil or expiry == 0) then
      return 0, 0;
   else
      if count == nil then
	 count = 0;
      end
      return expiry, count;
   end
end
    
-- return the seconds left on the debuff (or 0 if it's not up), and the
-- count of the buff/debuff stack (or 0 if it's not up)
function DestroDummies:utilDebuffByMeInfo(debuff)
   local name, expiry, count;
   name, expiry, count = DestroDummies:debuffUpByMe(debuff);
   if (expiry ~= nil) then
      expiry = expiry - GetTime();
   end

   if (name == nil or expiry == nil or expiry == 0) then
      return 0, 0;
   else
      if count == nil then
	 count = 0;
      end
      return expiry, count;
   end
end


-- check a mutex to see if any of the buffs are up on the player.  if a buff
-- is castable by the player, consider it not up if it is within the clip
-- time.  Return the name of the buff that is up, or nil if none
function DestroDummies:utilMxBuff(mNum)
    for i = 1,DestroDummies.mutex[mNum].count do
    	local name, _,_, _, _, _, expiry, caster, _ = 
		    UnitAura("player", DestroDummies.mutex[mNum][i]);

        if (name ~= nil) then
	    -- this really ought to be cached in the mutex[]
	    local spell;
	    spell = spell4aura(name);

	    local _,_,_,_,_,_,castTime,_,_ = GetSpellInfo(spell);
	    -- We get nil if the buff is not the one we can cast (for example,
	    -- Abominable Might having overwritten Trueshot Aura on a Hunter)
	    if (castTime == nil) then
		return name;
	    end
	    castTime = castTime / 1000;		-- milliseconds to seconds

	    -- Worry about lag
	    local _,_,lag = GetNetStats();
	    lag = lag / 1000;			-- convert to seconds

	    -- pick the right clip to use
	    local clip;
	    if (UnitAffectingCombat("player")) then
	    	clip = DestroDummies.mutex[mNum].clipCombat;
	    else
	    	clip = DestroDummies.mutex[mNum].clipOOCombat;
	    end

	    if (expiry == 0) or 
	        ((expiry - GetTime() - clip - castTime - lag) > 0) then
		return name;
	    else
	    	return nil;
	    end
        end
    end
    return nil;
end

-- check a mutex for which of the buffs is up on the player (cast by him, 
-- to catch the case where another hunter has Aspect of the Pack up)
function DestroDummies:utilMxBuffByMe(mNum)
    for i = 1,DestroDummies.mutex[mNum].count do
    	local name, _,_, _, _, _, expiry, caster, _ = 
		    UnitAura("player", DestroDummies.mutex[mNum][i]);

        if (name ~= nil and caster == "player") then
	    -- this really should be cached in the mutex[] +++
	    local spell;
	    spell = spell4aura(name);

	    local _,_,_,_,_,_,castTime,_,_ = GetSpellInfo(spell);
	    -- This should never happen
	    if (castTime == nil) then
		return nil;
	    end
	    castTime = castTime / 1000;		-- milliseconds to seconds

	    -- Worry about lag
	    local _,_,lag = GetNetStats();
	    lag = lag / 1000;			-- convert to seconds

	    -- pick the right clip to use
	    local clip;
	    if (UnitAffectingCombat("player")) then
	    	clip = DestroDummies.mutex[mNum].clipCombat;
	    else
	    	clip = DestroDummies.mutex[mNum].clipOOCombat;
	    end

	    if (expiry == 0) or 
	       ((expiry - GetTime() - clip - castTime - lag) > 0) then
		return name;
	    else
	    	return nil;
	    end
        end
    end
    return nil;
end

-- check a mutex for which of the debuffs is up on the target 
function DestroDummies:utilMxDebuff(mNum)
    for i = 1,DestroDummies.mutex[mNum].count do
        local name, expiry = DestroDummies:debuffUpByAny(DestroDummies.mutex[mNum][i]);
        if (name ~= nil) then
	    -- this really should be cached in the mutex  +++
	    local spell;
	    spell = spell4aura(name);

	    local _,_,_,_,_,_,castTime,_,_ = GetSpellInfo(spell);
	    if castTime == nil then
	    	return name;	-- must be someone else's
	    end
	    castTime = castTime / 1000;		-- milliseconds to seconds

	    -- Worry about lag
	    local _,_,lag = GetNetStats();
	    lag = lag / 1000;			-- convert to seconds

	    expiry = expiry - GetTime();

	    -- pick the right clip to use
	    local clip;
	    if (UnitAffectingCombat("player")) then
	    	clip = DestroDummies.mutex[mNum].clipCombat;
	    else
	    	clip = DestroDummies.mutex[mNum].clipOOCombat;
	    end

	    if (expiry > (clip + castTime + lag)) then
	    	return name;
	    else
	    	return nil;
	    end
        end
    end
    return nil;
end

-- check a mutex for which of the debuffs is up on the target (cast by 
-- the player)
function DestroDummies:utilMxDebuffByMe(mNum)
    for i = 1,DestroDummies.mutex[mNum].count do
        local name, expiry = DestroDummies:debuffUpByMe(DestroDummies.mutex[mNum][i]);
        if (name ~= nil) then
	    local spell;
	    spell = spell4aura(name);
	    local _,_,_,_,_,_,castTime,_,_ = GetSpellInfo(spell);
	    castTime = castTime / 1000;		-- milliseconds to seconds

	    -- Worry about lag
	    local _,_,lag = GetNetStats();
	    lag = lag / 1000;			-- convert to seconds

	    expiry = expiry - GetTime();

	    -- pick the right clip to use
	    local clip;
	    if (UnitAffectingCombat("player")) then
	    	clip = DestroDummies.mutex[mNum].clipCombat;
	    else
	    	clip = DestroDummies.mutex[mNum].clipOOCombat;
	    end

	    if (expiry > (clip + castTime + lag)) then
	    	return name;
	    else
	    	return nil;
	    end
        end
     end
     return nil;
end

-- Check a mutex to see if we should apply the passed-in debuff to the 
-- target.  We should if there is no higher-level debuff (ie., declared 
-- earlier) already on, or if expiry is less than the clip time.  Return
-- true if we should cast, false if not
function DestroDummies:utilMxNeedDebuff(mNum, debuff)
    -- loop on each spell in the mutex, in decreasing priority
    for i = 1,DestroDummies.mutex[mNum].count do
        local name, expiry, count = DestroDummies:debuffUpByAny(DestroDummies.mutex[mNum][i]);
        if (name == nil) then
	    -- the debuff is not up.  If it is us, we should apply it
	    if (debuff == DestroDummies.mutex[mNum][i]) then
	    	return true;
	    end
	else
	    -- We have a debuff.  If it's not the debuff we are looking
	    -- for, there is a higher priority debuff up, so we we should
	    -- not cast
	    if (name == debuff) then
	    	return false;
	    end

	    -- It's us.  Check the expiry
	    local spell;
	    spell = spell4aura(debuff);
	    local _,_,_,_,_,_,castTime,_,_ = GetSpellInfo(spell);
	    castTime = castTime / 1000;		-- milliseconds to seconds

	    -- Worry about lag
	    local _,_,lag = GetNetStats();
	    lag = lag / 1000;			-- convert to seconds

	    expiry = expiry - GetTime();

	    -- pick the right clip to use
	    local clip;
	    if (UnitAffectingCombat("player")) then
	    	clip = DestroDummies.mutex[mNum].clipCombat;
	    else
	    	clip = DestroDummies.mutex[mNum].clipOOCombat;
	    end

	    return (expiry > (clip + castTime + lag));
        end
    end
    return true;
end

-- Only weapons may be chanted (not shields or other offhand frills).
local function itemCanBeChanted (itemId)
   local _,_,_,_,_, itemType, itemSubtype = GetItemInfo(itemId);

   if (itemType ~= DestroDummies:xlateString("Weapon")) then
      return false;
   end 
   return (itemSubtype == DestroDummies:xlateString("Daggers") or
	   itemSubtype == DestroDummies:xlateString("Fist Weapons") or
	   itemSubtype == DestroDummies:xlateString("One-Handed Axes") or
           itemSubtype == DestroDummies:xlateString("Two-Handed Axes") or
           itemSubtype == DestroDummies:xlateString("One-Handed Swords") or
           itemSubtype == DestroDummies:xlateString("Two-Handed Swords") or
           itemSubtype == DestroDummies:xlateString("One-Handed Maces") or
           itemSubtype == DestroDummies:xlateString("Two-Handed Maces") or
	   itemSubtype == DestroDummies:xlateString("Staves"));
end

-- return true if the item in the passed slot number can be chanted
local function canBeChanted (slotnum)
   local item = GetInventoryItemID("player", slotnum);
   return item ~= nil and itemCanBeChanted(item);
end

-- return true all weapons have enhancements, false otherwise.  
--
-- Also returns the main, offhand, and thrown enchants (true if it's
-- chanted or not available to be chanted, or nil if it's missing a
-- chant)
function DestroDummies:utilWeaponEnhance(clip)
    local mainChant, mainTime, _, offChant, offTime, _, rangedChant, rangedTime, _ = GetWeaponEnchantInfo();
    local allChanted = true;

    clip = clip * 1000;		-- convert to milliseconds

    -- Worry about lag
    local _,_,lag = GetNetStats();

    if canBeChanted(DestroDummies.mainhandSlotNum) then
       if (mainChant == nil) or (mainTime < (clip + lag)) then
	  mainChant = nil;
	  allChanted = nil;
       end 
    else 
       mainChant = true;
    end

    if canBeChanted(DestroDummies.offhandSlotNum) then
       if (offChant == nil) or (offTime < (clip + lag)) then
	 offChant = nil;
	  allChanted = nil;
       end 
    else 
       offChant = true;
    end

    return allChanted, mainChant, offChant, true;
end

-- return the passed things health, as a percent of max.  if the passed thing
-- does not exist, return that it's at full health (so tests on pets that 
-- aren't out don't result in constant calls to heal the pet
function DestroDummies:utilHealth(thang)
    if not UnitExists(thang) then
    	return 100;
    end

    local cur = UnitHealth(thang);
    local max = UnitHealthMax(thang);

    if (cur == nil or max == nil or max == 0) then
    	return 0;
    end
    return (cur * 100) / max;
end

-- Extrapolate how long it will take to kill the target
function DestroDummies:utilTimeToDie()
    local perc = DestroDummies:utilHealth("target");
    local elapsed = GetTime() - DestroDummies.combatStartTime;
    
    return (elapsed * 100) / (100 - perc);
end

-- UnitDetailedThreatSituation() tends to return nil a lot, especially
-- when practicing on the dummies in Ironforge.  So turn them all into 0's
-- for our nice users
function DestroDummies:utilThreat()
    local isTanking, status, threatpct, rawthreatpct, threatvalue = 
		UnitDetailedThreatSituation("player", "target");
    if isTanking == nil then
	isTanking = 0;
    end
    if status == nil then
	status = 0;
    end
    if threatpct == nil then
	threatpct = 0;
    end
    if rawthreatpct == nil then
	rawthreatpct = 0;
    end
    if threatvalue == nil then
	threatvalue = 0;
    end
    return isTanking, status, threatpct, rawthreatpct, threatvalue;
end

function DestroDummies:utilHasGlyph(name)
    for i = 1, NUM_GLYPH_SLOTS do
        local _, _, _, glyphSpell, _ = GetGlyphSocketInfo(i);
	if (glyphSpell ~= nil) then
	    local gname,_,_,_,_,_,_,_,_ = GetSpellInfo(glyphSpell);
	    if (gname == name) then
	    	return true;
	    end
	end
    end
    return false;
end

-- Return the amount of time it will take this spell to fly the distance,
-- *assuming* it actually does have to fly and is not instant.  Knowledge
-- of which spells need to call this rests with you, dear programmer, not
-- with me
--
-- Assume a flight time of 20yds/sec, which is what I timed on a stopwatch
--
function DestroDummies:utilFlightTime()
    return DestroDummies:distanceToTarget() / 20.0;
end


-- Something is a boss if it is elite and 3 or more levels above you
function DestroDummies:utilBoss()
   -- a target level of -1 is a boss.  If the level is not either -1 or at
   -- least 3 levels above us, then it's not a boss to us
   local tLev = UnitLevel("target");
   if not (tLev == -1 or (tLev >= UnitLevel("player") + 3)) then
      return false;
   end 

   -- it also needs to be some sort of elite
   local uc = UnitClassification("target");
   return uc == "worldboss" or uc == "rareelite" or uc == "elite";
end


-- Return the amount of time the totem has left, nil if it is not deployed
function DestroDummies:utilTotem (totem)
   local tno;
   for tno = 1,4 do
      local _, totemName, startTime, duration = GetTotemInfo(tno);
      if (totemName == totem) then
	 if startTime == 0 then
	    return nil;	-- can this ever happen?
	 end 
	 return duration;
      end 
   end 
   return nil;
end

-- Return information about the passed-in totem type (Fire = 1, Earth = 2, 
-- Water = 3, Air = 4).  
--
-- Returned values:
--	haveTotem:	true if a totem of this type is down, nil otherwise
--	totemName:	which totem of this type is down (nil if none)
--	remainingTime:	time remaining on this totem (or 0 if none)
function DestroDummies:utilTotemType (typenum)
   local _, totemName, startTime, duration = GetTotemInfo(typenum);

   if totemName == "" or startTime == 0 or duration == 0 then
      return nil, nil, 0;
   end

   return true, totemName, duration - (GetTime() - startTime);
end


-- True if we are in the passed-in stance, false otherwise
function DestroDummies:utilStance (stance)
    if (stance <= 0 or stance > GetNumShapeshiftForms()) then
	return false;
    end

    local _,_,active,_ = GetShapeshiftFormInfo(stance);
    if (active == 1) then
	return true; 
    else
	return false;
    end
end


-- Return amount of power required to cast this spell
function DestroDummies:utilRequiredPower (spell)
    local name,_,_,cost = GetSpellInfo(spell);
    if (name == nil) then
	return 0;	-- they don't know this spell
    end
    return cost;
end

-- Returns the number of points we have in the passed-in talent
function DestroDummies:utilTalentCount (spell)
    local numTabs = GetNumTalentTabs();
    for t = 1,numTabs do
        local numTalents = GetNumTalents(t);
        for i = 1,numTalents do
            local name, _, _, _, points, _ = GetTalentInfo(t,i);
            if (name == spell) then
	       return points;
            end
        end
    end
    return 0;
end

-- Return the number of runes of the passed-in type (including Death Runes in
-- place of one of the expected runes).  Mainly useful for tests to see 
-- whether both of a pair of runes are off cooldown.
--
-- Called with rtype == 1 for blood, 2 for unholy, 3 for frost
function DestroDummies:utilRuneCount (rType)
   local ready, count, r1, r2;

   count = 0;
   r1 = ((rType - 1) * 2) + 1;
   r2 = r1 + 1;

   _,_,ready = GetRuneCooldown(r1);
   if ready then 
      count = count + 1;
   end

   _,_,ready = GetRuneCooldown(r2);
   if ready then 
      count = count + 1;
   end

   return count;
end

-- True if we have at least the passed-in number of each rune type (including
-- death runes.
function DestroDummies:utilHaveRunes (Nblood, Nfrost, Nunholy)
    local i;
    local Hblood, Hfrost, Hunholy, Hdeath;

    Hblood = 0;
    Hfrost = 0;
    Hunholy = 0;
    Hdeath = 0;

    for i = 1,6 do
	local _,_,ready = GetRuneCooldown(i);
	if (ready) then
	    local runeType = GetRuneType(i);
	    if (runeType == 1) then
		Hblood = Hblood + 1;
	    elseif (runeType == 2) then
		Hunholy = Hunholy + 1;
	    elseif (runeType == 3) then
		Hfrost = Hfrost + 1;
	    elseif (runeType == 4) then
		Hdeath = Hdeath + 1;
	    end
	end
    end

    -- we only bother decrementing Hdeath, as those are the only ones we
    -- need to look at more than once
    if (Nblood > Hblood) then 
	Hdeath = Hdeath - (Nblood - Hblood);
	if (Hdeath < 0) then
	    return nil;
	end
    end

    if (Nunholy > Hunholy) then 
	Hdeath = Hdeath - (Nunholy - Hunholy);
	if (Hdeath < 0) then
	    return nil;
	end
    end

    if (Nfrost > Hfrost) then 
	Hdeath = Hdeath - (Nfrost - Hfrost);
	if (Hdeath < 0) then
	    return nil;
	end
    end

    return true;
end

-- return true if the current target is currently aggro'd on a tank
function DestroDummies:utilOnTank ()
   local class, role;

   -- Did they assign it a role?
   role = UnitGroupRolesAssigned("targettarget");
   if role == "TANK" then
      return true;
   end
   if role ~= "NONE" then
      return false;
   end 

   -- Is it a tanking class?
   _,class = UnitClass("targettarget");
   if class == nil then
      return nil;	-- no target, or target is not targetting anyone
   end 
   if class ~= "WARRIOR" and class ~= "DEATHKNIGHT" and 
      class ~= "PALADIN" and class ~= "DRUID" then
      return nil
   end

   -- Sigh.  It would be nice to look at the talents and figure out if it's a
   -- tank, but I can't find an API for this.  I guess we'll just assume it's
   -- on a tank.
   return true;
end
