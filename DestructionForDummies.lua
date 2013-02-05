--
--  Destruction for Dummies
--  An addon by Earendil of Elune
--
--
--  If you are planning on adding support modules for this (and want me
--  to incorporate them into the general release) please read and follow
--  these guidelines:
--
--  1.  Do not make any changes in my files.  I have written a generic
--      framework that should contain no class- or build- specific 
--      information.  If you think you need a new general-purpose function,
--      please talk to me before writing one.  If you need one to just
--      handle something for your class or build, it should go in your file.
--
--  2.  Your code should be lightweight.  The combat routines in particular
--      are going to be called fairly frequently in a situation where the
--      user isn't going to want to see a lot of lag.  Make the decision 
--      rapidly and get out.  Turn on DestroDummiesPCdb.profile to find out
--      how much time your addon is taking to figure out what to do in 
--      combat.  You should not need to exceed 0.07%.
--
--  3.  Do not require/permit stuff to be configured.  The name of this mod
--      was taken with some thought--the goal is to implement a "Dummies" type
--      addon, not one which allows the expert user to configure everything
--      the way they want.  You are the expert--implement the optimal rotation
--      (or at least a rotation as close to optimal as you can get).  The user
--      doesn't have to take your advice, but don't give choices.
--
--  4.  Do not recommend the use of long cooldown (like
--      once-per-boss-fight) casts.  Heroism for shamans, Berserk for
--      druids, and the like.  If such a cast is available, call
--      DestroDummies:altRecommend() on it.  (The problem is that in very
--      many boss fights you are going to want to save them for a specific
--      time (which the Raid Leader will probably call).  If you come up
--      with a rotation that will recommend them, the user will have no
--      way to see what you recommend if he can't cast that yet.)
--
--  5.  Most important of all: get your rotation right.  What you implement
--      had better agree with Elitist Jerks' take on optimal rotation.  This
--      mod will offer proven good advice, not just the rotation that you 
--      happen to like.  You don't need to optimize based on gear (it's going
--      to go out of date too quickly) but pick a reasonable rotation. 

local altFrame = {};
altFrame.nelem = 4;

local rangeSpells = {};
rangeSpells.count = 0;

local playerClass;		-- *not* class+build; eg. "MAGE" or "WARLOCK"
local curCasting = nil;		-- spell we are currently casting

function DestroDummies.events.ADDON_LOADED(addon)
    if addon ~= "DestructionForDummies" then 
	return 
    end

    print("Destruction for Dummies (v", GetAddOnMetadata("DestructionForDummies", "Version"), ") (configure with /dfd)");

    DestroDummies:LoadSavedVars();

    DestroDummies.updateInterval = DestroDummiesPCdb.noCombatUpdateInterval;
    DestroDummies.timeSinceLastUpdate = 0;
    DestroDummies.lastAltWarnTime = 0;

    -- Register for Slash Commands
    DestroDummies:CreateOptionFrame();
    SlashCmdList["DestroDummies"] = DestroDummies.Options
    SLASH_DestroDummies1 = "/DestroForDummies"
    SLASH_DestroDummies2 = "/dfd"

    SlashCmdList["DestroDummiesEnable"] = DestroDummies.CmdEnabled;
    SLASH_DestroDummiesEnable1 = "/DestroForDummies enable"
    SLASH_DestroDummiesEnable2 = "/DestroForDummies e"
    SLASH_DestroDummiesEnable3 = "/DestroForDummies en"
    SLASH_DestroDummiesEnable4 = "/dfd enable"
    SLASH_DestroDummiesEnable5 = "/dfd e"
    SLASH_DestroDummiesEnable6 = "/dfd en"

    DestroDummies:CreateGUI();

    if DestroDummiesPCdb.enabled then
	DestroDummies.turnOn();
    end

    DestroDummies:initRangeCheckSpells();
end

-- Begin running Destruction for Dummies
function DestroDummies:turnOn()
    DestroDummies.eventFrame:SetScript("OnUpdate", function(this, elapsed)
	DestroDummies:OnUpdate(elapsed)
    end)

    DestroDummies.eventFrame:RegisterEvent("PLAYER_TARGET_CHANGED")
    DestroDummies.eventFrame:RegisterEvent("PLAYER_REGEN_ENABLED")
    DestroDummies.eventFrame:RegisterEvent("PLAYER_REGEN_DISABLED")
    DestroDummies.eventFrame:RegisterEvent("PLAYER_TALENT_UPDATE")
    DestroDummies.eventFrame:RegisterEvent("UNIT_SPELLCAST_SENT")
    DestroDummies.eventFrame:RegisterEvent("UNIT_SPELLCAST_SUCCEEDED")
    DestroDummies.eventFrame:RegisterEvent("UNIT_SPELLCAST_FAILED")
    DestroDummies.eventFrame:RegisterEvent("UNIT_COMBAT")
end

-- Stop running Destruction for Dummies
function DestroDummies:turnOff()
    DestroDummies.eventFrame:SetScript("OnUpdate", nil);
    DestroDummies.eventFrame:UnregisterAllEvents();
    DestroDummies.suggestFrame:Hide();
end

function DestroDummies:LoadSavedVars()
    if not DestroDummiesPCdb then
	DestroDummiesPCdb = {} -- fresh start
    end

    if DestroDummiesPCdb.locked == nil then
	DestroDummiesPCdb.locked = false
    end
    if DestroDummiesPCdb.dummyWhenUnlocked == nil then
	DestroDummiesPCdb.dummyWhenUnlocked = false
    end
	
    if not DestroDummiesPCdb.combatUpdateInterval then 
	DestroDummiesPCdb.combatUpdateInterval = 0.5 
	DestroDummiesPCdb.noCombatUpdateInterval = 0.5
    end
    if DestroDummiesPCdb.enabled == nil then 
	DestroDummiesPCdb.enabled = true;
    end
    if not DestroDummiesPCdb.scale then 
	DestroDummiesPCdb.scale = 0.70 
    end
    if not DestroDummiesPCdb.suggestPoint then
	DestroDummiesPCdb.suggestPoint = "CENTER";
	DestroDummiesPCdb.suggestXoff = 0;
	DestroDummiesPCdb.suggestYoff = 0;
	DestroDummiesPCdb.suggestRelativeTo = nil;
	DestroDummiesPCdb.suggestRelativePoint = "CENTER";
    end
    if DestroDummiesPCdb.ferralMode == nil then
	DestroDummiesPCdb.ferralMode = false
    end
end 


-- This is the workhorse routine.  It gets called on every update tick
function DestroDummies:OnUpdate(elapsed)
    if (DestroDummies.forceUpdate == 1) then
	DestroDummies.forceUpdate = 0;
    else
	self.timeSinceLastUpdate = self.timeSinceLastUpdate + elapsed;
	if (self.timeSinceLastUpdate < DestroDummies.updateInterval) then 
	    return 
	end
    end
    self.timeSinceLastUpdate = 0

    if not DestroDummiesPCdb.enabled then
	return
    end

    -- Because talent information is not available at addon load time (thank
    -- you so very much, Blizzard), we need to burn cycles here to check
    -- and init ourselves.  We are going to do a sin of commission and 
    -- trust that checking the combat routine is sufficient--that we will
    -- never have it set and the noCombat routine set to nil.  This ought
    -- to be safe, so it's worth it.  But it's still totally fucking
    -- stupid that I have to burn time every update doing the fucking 
    -- stupid test and I hate the idea of having to test two things.
    --
    if (DestroDummies.decisionCombat == nil) then
    	DestroDummies:populateRegistry();
	if (DestroDummies.decisionCombat == nil) then
	    return;
	end
    end

    spell = nil;		-- default
    local i;
    for i = 1,altFrame.nelem do
    	altFrame[i].spell = nil
    end

    -- don't add insult to injury by recommending anything when they're dead
    -- also, don't make a suggestion when they are in a vehicle.  they will
    -- have other abilities we don't know about
    if ((not UnitIsDeadOrGhost("player")) and 
        (not UnitInVehicle("player")) and
        (not UnitOnTaxi("player"))) then
	-- force a reload of target debuffs
	DestroDummies.needNewDebuffData = 1;

	if (UnitAffectingCombat("player")) then
	    if (UnitExists("target") and UnitCanAttack("player", "target") and
			not UnitIsDead("target")) then
		local startTime = GetTime();
		spell = DestroDummies.decisionCombat();
		DestroDummies.execTime = DestroDummies.execTime + 
							GetTime() - startTime;
	    end
	else
	    spell = DestroDummies.decisionNoCombat();
	end
    end

    if (DestroDummies.debug) then
	print("Decided on ", spell);
	DestroDummies.debug = false
    end

    if (spell == nil) then
	DestroDummies.cooldownFrame:SetAlpha(0);
	DestroDummies:displaySpellInFrame(DestroDummies.suggestFrame, 
					  DestroDummies.suggestTexture, nil);
    else
	DestroDummies:showCooldownAnimation(spell);
	DestroDummies:displaySpellInFrame(DestroDummies.suggestFrame, 
					  DestroDummies.suggestTexture, spell);
    end

    local i;
    for i = 1,altFrame.nelem do
	DestroDummies:displaySpellInFrame(altFrame[i].frame,
					  altFrame[i].texture,
					  altFrame[i].spell);
    end
end

-- Display the passed spell's texture in the passed spell's frame
function DestroDummies:displaySpellInFrame(frame, texture, spell)
    if (spell == nil) then
	if (not DestroDummies.inCombat and not DestroDummiesPCdb.locked and 
			    DestroDummiesPCdb.dummyWhenUnlocked) then
	    texture:SetTexture(GetItemIcon("item:42992"));
	    frame:Show();
	else
	    frame:Hide();
	end
    else
	texture:SetTexture(GetSpellTexture(spell));
	frame:Show();
    end
end


-- start the cooldown animation.  Time it for the max of the current
-- spell and the cooldown on the spell that is passed in
function DestroDummies:showCooldownAnimation(spell)
    -- Start by getting the information for the spell we are now casting
    local curspell,_,_,_,curStartTime,curEndTime,_ = UnitCastingInfo("player");
    if (curspell == nil) then
    	if (curCasting ~= nil) then
	    -- Sigh.  It takes Blizz about 15 calls to our add-on before they
	    -- are aware we are casting (depending on lag.  The delay is the
	    -- time between our knowing on our local machine that we are 
	    -- casting, and the server knowing that we are casting).  Fall
	    -- back to our information until Bliz gives us theirs
	    curStartTime = DestroDummies.castStartTime;
	    local _,_,_,_,_,_,castTime,_,_ = GetSpellInfo(curCasting);
	    -- castTime will be nil for things like Hearthstone, which count
	    -- as a spell, but are not one in the spell book
	    if (castTime == nil) then
		castTime = 0;
	    end
	    castTime = castTime / 1000;		-- convert to seconds

	    -- duration is max of GCD and cast time.  if the castTime is
	    -- greater than 1.5 seconds we don't need to bother computing
	    -- the GCD (which is a somewhat slow process)
	    local gcd = 0;
	    if (castTime < 1.5) then
		gcd = DestroDummies:computeGCD(playerClass, curCasting);
	    end
	    if (castTime > gcd) then
	    	curEndTime = curStartTime + castTime;
	    else
	    	curEndTime = curStartTime + gcd;
	    end
	else
	    curStartTime = 0;
	    curEndTime = 0;
	end
    else
	-- convert to seconds
	curStartTime = curStartTime / 1000;
	curEndTime = curEndTime / 1000;
    end
    local curDuration = curEndTime - curStartTime;

    -- Get the cooldown for the next spell to cast
    local cdStartTime, cdDuration, _ = GetSpellCooldown(spell);
    if cdStartTime == nil then
        cdStartTime = 0;
    end
    if cdDuration == nil then
    	cdDuration = 0;
    end

    -- figure out which is longest
    local startTime, duration;
    if (curEndTime > (cdStartTime + cdDuration)) then
	startTime = curStartTime;
	duration = curDuration;
    else
	startTime = cdStartTime;
	duration = cdDuration;
    end

    -- don't refresh the animation if it's unchanged
    if not ((startTime == DestroDummies.svCDStartTime) and 
	    (duration == DestroDummies.svCDDuration)) then

        DestroDummies.svCDStartTime = startTime;
	DestroDummies.svCDDuration = duration;

	-- Display the cooldown animation for the long pole
	if ((startTime ~= 0) and (startTime ~= nil)) then
	    DestroDummies.cooldownFrame:SetCooldown(startTime, duration)
	    DestroDummies.cooldownFrame:SetAlpha(1)
	else
	    DestroDummies.cooldownFrame:SetAlpha(0);
	end
    end
end


function DestroDummies:CreateGUI()
    --
    --  Create the main suggest frame
    --
    DestroDummies.suggestFrame = CreateFrame("Frame", nil, UIParent);
    local f = DestroDummies.suggestFrame;

    local t = f:CreateTexture(nil, "BACKGROUND");
    t:SetTexture(nil)
    t:SetAllPoints(f)
    t:SetAlpha(0.5)
    f.texture = t
    DestroDummies.suggestTexture = t

    f:SetFrameStrata("BACKGROUND");
    f:SetClampedToScreen(true);
    f:SetWidth(128);
    f:SetHeight(128);
    f:SetScale(DestroDummiesPCdb.scale);
    f:SetPoint(DestroDummiesPCdb.suggestPoint, DestroDummiesPCdb.suggestRelativeFrame, DestroDummiesPCdb.suggestRelativePoint, DestroDummiesPCdb.suggestXoff, DestroDummiesPCdb.suggestYoff);
    f:Show();


    -- 
    --  Create the side frames
    --
    local i;
    for i = 1,altFrame.nelem do
        local part1, part2a, part2b;

    	if (i == 1 or i == 3) then
	    part1 = "BOTTOM";
	else
	    part1 = "TOP";
	end
	if (i == 1 or i == 2) then
	    part2a = "RIGHT";
	    part2b = "LEFT";
	else
	    part2a = "LEFT";
	    part2b = "RIGHT";
	end
	    
    	altFrame[i] = {};
	altFrame[i].frame, altFrame[i].texture = 
		DestroDummies:createFrame(64, part1..part2a, f, part1..part2b);
    end
    

    f:RegisterForDrag("LeftButton");
    f:SetScript("OnDragStart", f.StartMoving);
    f:SetScript("OnDragStop", DestroDummies.stopMoving);
    DestroDummies:FrameOptions(nil);


    -- cooldown animation
    DestroDummies.cooldownFrame = CreateFrame("Cooldown","$parent_cooldown", f);
    DestroDummies.cooldownFrame:SetHeight(128);
    DestroDummies.cooldownFrame:SetWidth(128);
    DestroDummies.cooldownFrame:ClearAllPoints();
    DestroDummies.cooldownFrame:SetPoint("CENTER", f, "CENTER", 0, 0);
end

function DestroDummies:stopMoving()
    local f = DestroDummies.suggestFrame;

    f:StopMovingOrSizing();
    local point, relativeTo, relativePoint, xOff, yOff = f:GetPoint();
    DestroDummiesPCdb.suggestPoint = point;
    DestroDummiesPCdb.suggestRelativeTo = relativeTo;
    DestroDummiesPCdb.suggestRelativePoint = relativePoint;
    DestroDummiesPCdb.suggestXoff = xOff;
    DestroDummiesPCdb.suggestYoff = yOff;
end

function DestroDummies:createFrame(size, myPoint, parent, parentPoint)
    local f = CreateFrame("Frame", nil, parent);
    local t = f:CreateTexture(nil, "BACKGROUND");
    t:SetTexture(nil);
    t:SetAllPoints(f);
    t:SetAlpha(0.5);
    f.texture = t;

    f:SetFrameStrata("BACKGROUND");
    f:SetClampedToScreen(true);
    f:SetWidth(size);
    f:SetHeight(size);
    f:SetPoint(myPoint, parent, parentPoint);

    return f,t;
end

-- Set various frame options for combat/no combat
function DestroDummies:FrameOptions()
    local f = DestroDummies.suggestFrame;
    if (DestroDummies.inCombat or DestroDummiesPCdb.locked) then
        f:SetMovable(0);
        f:EnableMouse(0);
    else
        f:SetMovable(1);
        f:EnableMouse(1);
    end
end

-- called for every attack or heal
function DestroDummies.events.UNIT_COMBAT(unit, action, descriptor, damage, 
					  damageType)
    if (unit ~= "player") then
        return;
    end

    if action == "WOUND" then
       if DestroDummies.takeDamage ~= nil then
	  DestroDummies.takeDamage(damage);
       end
    elseif action == "HEAL" then
       if DestroDummies.takeHeal ~= nil then
	  DestroDummies.takeHeal(damage);
       end
    end
end

function DestroDummies.events.UNIT_SPELLCAST_SENT(unit, spell)
    if (unit ~= "player") then
        return;
    end

    curCasting = spell;
    DestroDummies.forceUpdate = 1;
    if (spell ~= "Auto Shot") then	-- Don't let auto shot overwrite
       DestroDummies.lastSpell = spell;
    end
    DestroDummies.castStartTime = GetTime();

    if (DestroDummies.spellSent ~= nil) then
    	DestroDummies.spellSent(spell);
    end
end

function DestroDummies.events.UNIT_SPELLCAST_SUCCEEDED(unit, spell)
    if (unit ~= "player") then
        return;
    end
    curCasting = nil;
    DestroDummies.forceUpdate = 1;

    -- UNIT_SPELLCAST_START is sometimes not called, so need to set here too
    if (spell ~= "Auto Shot") then	-- Don't let auto shot overwrite
       DestroDummies.lastSpell = spell;
    end

    if (DestroDummies.postSpell ~= nil) then
    	DestroDummies.postSpell(spell, true);
    end
end

function DestroDummies.events.UNIT_SPELLCAST_FAILED(unit, spell)
    if (unit ~= "player") then
        return;
    end
    curCasting = nil;
    DestroDummies.forceUpdate = 1;
    DestroDummies.lastSpell = nil;

    if (DestroDummies.postSpell ~= nil) then
    	DestroDummies.postSpell(spell, false);
    end
end

function DestroDummies.events.PLAYER_TALENT_UPDATE()
    DestroDummies:populateRegistry();
    DestroDummies:initRangeCheckSpells();
end

function DestroDummies.events.PLAYER_TARGET_CHANGED()
    DestroDummies.lastSpell = nil;
end

-- enter combat
function DestroDummies.events.PLAYER_REGEN_DISABLED()
    if (DestroDummies.preCombat ~= nil) then
    	DestroDummies.preCombat();
    end

    DestroDummies.combatStartTime = GetTime();
    DestroDummies.execTime = 0;

    DestroDummies.updateInterval = DestroDummiesPCdb.combatUpdateInterval;
    DestroDummies.forceUpdate = 1;
    DestroDummies:FrameOptions();

    DestroDummies.inCombat = true;
    DestroDummies:FrameOptions();
end

-- exit combat
function DestroDummies.events.PLAYER_REGEN_ENABLED()
    local endTime = GetTime();
    DestroDummies.inCombat = false;
    DestroDummies:FrameOptions();

    DestroDummies.updateInterval = DestroDummiesPCdb.noCombatUpdateInterval;
    DestroDummies.forceUpdate = 1;
    DestroDummies:FrameOptions();

    if DestroDummiesPCdb.profile then
        local combatTime = endTime - DestroDummies.combatStartTime;
        print("Destruction for Dummies: combat ended after ", combatTime, 
                "seconds.");
        print("  -- DfD took ", DestroDummies.execTime, " seconds (",
                (DestroDummies.execTime * 100) / combatTime, "%");
    end
    DestroDummies.combatStartTime = 0;
end

function DestroDummies:populateRegistry()
    DestroDummies.myClass = DestroDummies:whatAmI();
    if (DestroDummies.myClass == classUnknown) then
        DestroDummies.preCombat = nil;
	DestroDummies.spellSent = nil;
	DestroDummies.postSpell = nil;
        DestroDummies.takeHeal = nil;
        DestroDummies.takeDamage = nil;
        DestroDummies.decisionCombat = nil;
        DestroDummies.decisionNoCombat = nil;
    else
        DestroDummies.preCombat = DestroDummies.registry.preCombat[DestroDummies.myClass];
        DestroDummies.spellSent = DestroDummies.registry.spellSent[DestroDummies.myClass];
        DestroDummies.postSpell = DestroDummies.registry.postSpell[DestroDummies.myClass];
        DestroDummies.takeDamage = DestroDummies.registry.takeDamage[DestroDummies.myClass];
        DestroDummies.takeHeal = DestroDummies.registry.takeHeal[DestroDummies.myClass];
        DestroDummies.decisionCombat = DestroDummies.registry.decideCombat[DestroDummies.myClass];
        DestroDummies.decisionNoCombat = DestroDummies.registry.decideNoCombat[DestroDummies.myClass];
    end
end

function DestroDummies.Options()
    InterfaceOptionsFrame_OpenToCategory(getglobal("DestroDummiesOptions"));
end

function DestroDummies.CmdEnabled()
    if (DestroDummiesPCdb.enabled == true) then
       print("Disabling Destruction for Dummies");
    else 
       print("Enabling Destruction for Dummies");
    end
    DestroDummies:ToggleEnabled();   
end

function DestroDummies:CreateOptionFrame()
    local panel = CreateFrame("FRAME", "DestroDummiesOptions");
    panel.name = "Destruction for Dummies";
    
    local fstring1 = panel:CreateFontString("DestroDummiesOptions_1_string1","OVERLAY","GameFontNormal")
    fstring1:SetText(DestroDummies:xlateString("Enable (for this Character)"));
    fstring1:SetPoint("TOPLEFT", 10, -10)
    local checkbox1 = CreateFrame("CheckButton", "$parent_cb1", panel, "OptionsCheckButtonTemplate")
    checkbox1:SetWidth(30)
    checkbox1:SetHeight(30)
    checkbox1:SetScript("OnClick", function() DestroDummies:ToggleEnabled() end)
    checkbox1:SetPoint("TOPRIGHT", -10, -10)
    checkbox1:SetChecked(DestroDummiesPCdb.enabled);

    
    local fstring2 = panel:CreateFontString("DestroDummiesOptions_1_string2","OVERLAY","GameFontNormal")
    fstring2:SetText(DestroDummies:xlateString("Reposition Lock"));
    fstring2:SetPoint("TOPLEFT", 10, -50)
    local checkbox2 = CreateFrame("CheckButton", "$parent_cb1", panel, "OptionsCheckButtonTemplate")
    checkbox2:SetWidth(30)
    checkbox2:SetHeight(30)
    checkbox2:SetScript("OnClick", function() DestroDummies:ToggleLocked() end)
    checkbox2:SetPoint("TOPRIGHT", -10, -50)
    checkbox2:SetChecked(DestroDummiesPCdb.locked)

    
    local fstring3 = panel:CreateFontString("DestroDummiesOptions_1_string3","OVERLAY","GameFontNormal")
    fstring3:SetText(DestroDummies:xlateString("Display dummy suggestions when unlocked"))
    fstring3:SetPoint("TOPLEFT", 10, -90)
    local checkbox3 = CreateFrame("CheckButton", "$parent_cb1", panel, "OptionsCheckButtonTemplate")
    checkbox3:SetWidth(30)
    checkbox3:SetHeight(30)
    checkbox3:SetScript("OnClick", function() DestroDummies:ToggleDummySuggest() end)
    checkbox3:SetPoint("TOPRIGHT", -10, -90)
    checkbox3:SetChecked(DestroDummiesPCdb.dummyWhenUnlocked);


    local fstring8 = panel:CreateFontString("DestroDummiesOptions_1_string8","OVERLAY","GameFontNormal")
    fstring8:SetText(DestroDummies:xlateString("Minimal assistance mode"))
    fstring8:SetPoint("TOPLEFT", 10, -130)
    local checkbox8 = CreateFrame("CheckButton", "$parent_cb1", panel, "OptionsCheckButtonTemplate")
    checkbox8:SetWidth(30)
    checkbox8:SetHeight(30)
    checkbox8:SetScript("OnClick", function() DestroDummies:ToggleFerralMode() end)
    checkbox8:SetPoint("TOPRIGHT", -10, -130)
    checkbox8:SetChecked(DestroDummiesPCdb.ferralMode);
    
    local fstring6 = panel:CreateFontString("DestroDummiesOptions_1_string6","OVERLAY","GameFontNormal")
    fstring6:SetText("Scale")
    fstring6:SetPoint("TOPLEFT", 10, -170)
    local slider2 = CreateFrame("Slider", "$parent_sl2", panel, "OptionsSliderTemplate")
    slider2:SetMinMaxValues(.25, 8.0)
    slider2:SetValue(DestroDummies:GetScale())
    slider2:SetValueStep(.25)
    slider2:SetScript("OnValueChanged", function(self) DestroDummies:SetScale(self:GetValue()); getglobal(self:GetName() .. "Text"):SetText(self:GetValue())  end)
    getglobal(slider2:GetName() .. "Low"):SetText("0.25")
    getglobal(slider2:GetName() .. "High"):SetText("8.0")
    getglobal(slider2:GetName() .. "Text"):SetText(DestroDummies:GetScale())
    slider2:SetPoint("TOPRIGHT", -10, -170)

    local resetButton = CreateFrame("Button", "$parent_rb", panel, "OptionsButtonTemplate");
    resetButton:SetText(DestroDummies:xlateString("Reset"));
    resetButton:SetPoint("TOPRIGHT", -20, -210);
    resetButton:SetScript("OnClick", DestroDummies.Reset);
    
    InterfaceOptions_AddCategory(panel);
end

function DestroDummies:Reset()
    DestroDummiesPCdb.scale = 0.70 
    DestroDummiesPCdb.suggestPoint = "CENTER";
    DestroDummiesPCdb.suggestXoff = 0;
    DestroDummiesPCdb.suggestYoff = 0;
    DestroDummiesPCdb.suggestRelativeTo = nil;
    DestroDummiesPCdb.suggestRelativePoint = "CENTER";

    local f = DestroDummies.suggestFrame;
    f:ClearAllPoints();
    f:SetPoint(DestroDummiesPCdb.suggestPoint, DestroDummiesPCdb.suggestRelativeFrame, DestroDummiesPCdb.suggestRelativePoint, DestroDummiesPCdb.suggestXoff, DestroDummiesPCdb.suggestYoff);
    f:SetScale(DestroDummiesPCdb.scale);
    f:Show();
end

function DestroDummies:ToggleLocked()
    DestroDummiesPCdb.locked = not DestroDummiesPCdb.locked;
    DestroDummies:FrameOptions();
    DestroDummies.forceUpdate = 1;
end

function DestroDummies:ToggleFerralMode()
    DestroDummiesPCdb.ferralMode = not DestroDummiesPCdb.ferralMode;
    DestroDummies:FrameOptions();
    DestroDummies.forceUpdate = 1;
end

function DestroDummies:ToggleDummySuggest()
    DestroDummiesPCdb.dummyWhenUnlocked = not DestroDummiesPCdb.dummyWhenUnlocked;
    DestroDummies.forceUpdate = 1;
end

function DestroDummies:ToggleEnabled()
    if (DestroDummiesPCdb.enabled == true) then
        DestroDummiesPCdb.enabled = false;
	DestroDummies.turnOff();
    else
        DestroDummiesPCdb.enabled = true;
	DestroDummies.turnOn();
    end
    DestroDummies:FrameOptions();
    DestroDummies.forceUpdate = 1;
end

-- API for people to use in macros
function DestroDummies:SetEnabled()
    DestroDummiesPCdb.enabled = true;
    DestroDummies.turnOn();
    DestroDummies:FrameOptions();
    DestroDummies.forceUpdate = 1;
end

-- API for people to use in macros
function DestroDummies:SetDisabled()
    DestroDummiesPCdb.enabled = false;
    DestroDummies.turnOff();
    DestroDummies:FrameOptions();
    DestroDummies.forceUpdate = 1;
end

function DestroDummies:GetScale()
    return DestroDummiesPCdb.scale;
end

function DestroDummies:SetScale(scale)
    DestroDummiesPCdb.scale = scale;
    DestroDummies.suggestFrame:SetScale(scale);
end

function DestroDummies:checkBeta(class)
    if (DestroDummies.beta) then
        return class;
    else
        return classUnknown;
    end
end

-- A debugging function.  Set a class to force, and call the talent update
-- handler to get us to reassign everything
-- 
function DestroDummies:forceClass(new)
    DestroDummies.forcedClass = new;
    DestroDummies.events.PLAYER_TALENT_UPDATE();
end

-- return the index for our class, or 0 if we don't know about it
function DestroDummies:whatAmI()
    -- For debugging
    if (DestroDummies.forcedClass ~= nil and DestroDummies.forcedClass ~= 0) then
        return DestroDummies.forcedClass;
    end

    local pc1
    pc1, playerClass = UnitClass("player");

    -- maxTab will hold the talent specialization number.  (This is a holdover
    -- name from pre Panda days when we had to count the number of points in
    -- each group to figure out what specialization this was)
    local maxTab;
    maxTab = GetSpecialization();

    if playerClass == "DEATHKNIGHT" then
        if (maxTab == 1) then
	    return DestroDummies.classDeathknightBlood;
        end
        if (maxTab == 2) then
	    return DestroDummies.classDeathknightFrost;
        end
        if (maxTab == 3) then
	    return DestroDummies.classDeathknightUnholy;
        end
        return 0;
    end

    if playerClass == "DRUID" then
        if (maxTab == 1) then
            return DestroDummies.classDruidBalance;
        end
        if (maxTab == 2 or maxTab == 3) then		-- Feral / Guardian
            return DestroDummies.classDruidFeral;
        end
        return 0;
    end

    if playerClass == "HUNTER" then
        if (maxTab == 1) then
            return DestroDummies.classHunterBeastmaster;
        end
        if (maxTab == 2) then
            return DestroDummies.classHunterMarksman;
        end
        if (maxTab == 3) then
            return DestroDummies.classHunterSurvival;
        end
        return 0;
    end

    if playerClass == "MAGE" then
        if (maxTab == 1) then
            return DestroDummies.classMageArcane;
        end
        if (maxTab == 2) then
            return DestroDummies.classMageFire;
        end
        if (maxTab == 3) then
            return DestroDummies.classMageFrost;
        end
        return 0;
    end

    if playerClass == "PALADIN" then
        if (maxTab == 3) then
            return DestroDummies.classPaladinRetribution;
        end
	return 0;
    end

    if playerClass == "PRIEST" then 
        if (maxTab == 3) then
            return DestroDummies.classPriestShadow;
        end
	return 0;
    end

    if playerClass == "ROGUE" then
        if (maxTab == 1) then
	    return DestroDummies.classRogueAssassination;
	end
        if (maxTab == 2) then
	    return DestroDummies.classRogueCombat;
	end
        if (maxTab == 3) then
	    return DestroDummies.classRogueSubtlety;
	end
    end

    if playerClass == "SHAMAN" then 
        if (maxTab == 1) then
            return DestroDummies.classShamanElemental;
        end
        if (maxTab == 2) then
            return DestroDummies.classShamanEnhancement;
        end
	return 0;
    end

    if playerClass == "WARLOCK" then 
        if (maxTab == 1) then
            return DestroDummies.classWarlockAffliction;
        end
        if (maxTab == 2) then
            return DestroDummies.classWarlockDemonology;
        end
        if (maxTab == 3) then
            return DestroDummies.classWarlockDestruction;
        end
        return 0;
    end

    if playerClass == "WARRIOR" then 
        if (maxTab == 2) then
            return DestroDummies.classWarriorFury;
        end
	if (maxTab == 3) then
            return DestroDummies.classWarriorProtection;
        end
	return 0;
    end

    if playerClass == "MONK" then 
        if (maxTab == 3) then
            return DestroDummies.classMonkWindwalker;
        end
	return 0;
    end

    return 0;
end


-- return true if player knows the spell (of any rank), false otherwise
function DestroDummies:knowsSpell(target)
    local i;
    for i = 1, MAX_SKILLLINE_TABS do
        local name, _, offset, numSpells = GetSpellTabInfo(i);
        
        if not name then
            break
        end

        for s = offset + 1, offset + numSpells do
            local spell, rank = GetSpellBookItemName(s, BOOKTYPE_SPELL);
            if spell == target then
                return true;
            end
        end
    end
    return false;
end
        

-- return true if we can cast the passed spell
function DestroDummies:spellReady(spell)
    local castable, mana = IsUsableSpell(spell);
    if (not castable or mana == 1) then
    	return false;
    end

    -- it is ready if it's cooldown is less than or equal to the GCD.
    local i;
    i = DestroDummies:cooldownTime(spell) <= DestroDummies:computeGCD(playerClass, spell);
    if DestroDummies.debug then
	print("spellReady(", spell, "): ",
    	      DestroDummies:cooldownTime(spell), ", ",
	      DestroDummies:computeGCD(playerClass, spell), " = ", i);
    end
    return i;
end


-- refresh our knowledge of what debuffs are up on target
function DestroDummies:refreshDebuffData()
    DestroDummies.needNewDebuffData = false;
    DestroDummies.numDebuffsByAny = 0;
    DestroDummies.numDebuffsByMe = 0;
    for i = 1,40 do
        local name,_,_,count,_,_,expiry,caster,_,_,spid = UnitDebuff("target", i);

        if (name ~= nil) then 
	    local j = DestroDummies.numDebuffsByAny;
	    DestroDummies.numDebuffsByAny = j + 1;
	    j = DestroDummies.numDebuffsByAny;

	    DestroDummies.debuffByAny[j].name = name;
	    DestroDummies.debuffByAny[j].expiry = expiry;
	    DestroDummies.debuffByAny[j].count = count;

	    if (caster == "player") then
		j = DestroDummies.numDebuffsByMe;
		DestroDummies.numDebuffsByMe = j + 1;
		j = DestroDummies.numDebuffsByMe;

		DestroDummies.debuffByMe[j].name = name;
		DestroDummies.debuffByMe[j].expiry = expiry;
		DestroDummies.debuffByMe[j].count = count;
	    end
	end
    end
end

-- return the amount of time left on the buff (or nil if it's not up), along
-- with the count of how many are up.
--
-- Note well that a buff that does not expire will return 0, so a test like
-- if (DestroDummies:buffTime("Aspect of the Hawk")) will never return true.
-- the only possible return values for Aspect of the Hawk (which does not 
-- expire) are nil and 0.
--
function DestroDummies:buffTime(buff, cretin)
    if cretin == nil then
       cretin = "player";
    end
    local name,_,_,count,_,_, expiry = UnitAura(cretin, buff, nil, "HELPFUL");
    if (name == nil) then
        return nil, 0;
    else
        return expiry - GetTime(), count;
    end
end

function DestroDummies:debuffTime(debuff, cretin)
    if cretin == nil then
       cretin = "player";
    end
   local name,_,_,count,_,_, expiry = UnitAura(cretin, debuff, nil, "HARMFUL");
   if (name == nil) then
       return nil, 0;
   else
       return expiry - GetTime(), count;
   end
end


-- return the spell name and expiration time for my cast of the debuff
function DestroDummies:debuffUpByMe(debuff)
    if (DestroDummies.needNewDebuffData) then
        DestroDummies:refreshDebuffData();
    end
    
    for i = 1,DestroDummies.numDebuffsByMe do
        if (DestroDummies.debuffByMe[i].name == debuff) then
            return debuff, DestroDummies.debuffByMe[i].expiry;
        end
    end
    return nil, nil;
end

-- A debugging function
function DestroDummies:showBuffs(unit)
    if unit == nil then
    	unit = "player";
    end
    local i;
    for i = 1,40 do
	local name, _,_, _, _, _, expiry, _, _ = UnitAura(unit, i);
	if (name ~= nil) then
	    print(name, ": ", expiry);
	end
    end
end

-- A debugging function
function DestroDummies:showDebuffs(cretin, force)
   if cretin == "player" then
      local i;
      for i = 1,40 do
	 local name, _,_, _, _, _,expiry,_,_ = UnitDebuff(cretin, i);
	 if (name ~= nil) then
	    print(name, ": ", expiry);
	 end
      end
   else
      if (force == 1 or DestroDummies.needNewDebuffData) then
	 DestroDummies:refreshDebuffData();
      end
      local i;
      for i = 1,DestroDummies.numDebuffsByAny do
	 print (DestroDummies.debuffByAny[i].name, ": ",
		DestroDummies.debuffByAny[i].expiry);
      end
   end
end

-- return the spell name, expiration time, and count for any cast of the debuff
-- if the same debuff is up from multiple people, return the most distant
-- expiry
function DestroDummies:debuffUpByAny(debuff)
    if (DestroDummies.needNewDebuffData) then
        DestroDummies:refreshDebuffData();
    end
    
    local i, sv_entry;
    sv_entry = 0;
    for i = 1,DestroDummies.numDebuffsByAny do
        if (DestroDummies.debuffByAny[i].name == debuff) then
	    -- if we never expire, or if this is the first we've seen, or if 
	    -- this entry expires after the previous one, save off this entry
	    if ((DestroDummies.debuffByAny[i].expiry == nil) or
	        (sv_entry == 0) or 
                (DestroDummies.debuffByAny[sv_entry].expiry < 
                                DestroDummies.debuffByAny[i].expiry)) then
                sv_entry = i;
            end
        end
    end
    if (sv_entry == 0) then
        return nil, nil, 0;
    else
        return debuff, DestroDummies.debuffByAny[sv_entry].expiry,
	       DestroDummies.debuffByAny[sv_entry].count;
    end
end

-- recommend an alternate spell
local altTestOrder = {};
altTestOrder[1] = 1;
altTestOrder[2] = 3;
altTestOrder[3] = 2;
altTestOrder[4] = 4;
function DestroDummies:altRecommend(spell, num)
   if (num >= 1 and num <= altFrame.nelem) then
      if (altFrame[num].spell == nil) then
	 altFrame[num].spell = spell;
	 return
      end
   end

   -- You can't always get what you want...
   local i;
   for i = 1,altFrame.nelem do
      if (altFrame[altTestOrder[i]].spell == nil) then
	 -- But if you try, sometimes, you get what you need
	 altFrame[altTestOrder[i]].spell = spell;
	 return;
      end
   end
end

-- reset our analysis engine
local analTable = {};
local analCount = 0;
analTable[0] = {};
analTable[0].spell = nil;
analTable[0].cooldown = 0;

function DestroDummies:analysisInit()
    analCount = 0;
end

-- add a spell to analyze.  returns the cooldown for debugging purposes only
--
-- if dependSpell is non-nil, the cooldown cannot be earlier than the cooldown
-- for the dependSpell.  If the dependSpell is not castable then do not add
-- the spell at all
function DestroDummies:analysisAdd(spell, dependSpell)
    local cooldown = DestroDummies:cooldownTime(spell);

    if dependSpell then
        if not DestroDummies:spellReady(dependSpell) then
	    return nil;
	end
	
	local depCool = DestroDummies:cooldownTime(dependSpell);
	if (depCool > cooldown) then
	    cooldown = depCool;
	end
    end

    analCount = analCount + 1;
    analTable[analCount] = {};
    analTable[analCount].spell = spell;
    analTable[analCount].cooldown = cooldown;
    analTable[analCount].killed = false;
    if DestroDummies.debug then
       print("analysisAdd(", spell, "): ", cooldown);
    end 
    return cooldown;
end

-- pick the spell to cast.  return the spell and how long it will be before
-- you can cast it.  
function DestroDummies:analysisPick(powerAlt, freeSpell)
    local i;
    local min;

    -- find the shortest cooldown and return it.  in case of ties, favor 
    -- the one with the lowest index.  If we cannot cast the spell in
    -- question because of shortage of power, return the powerAlt spell
    -- if they gave one.  If they can't cast it because they don't know
    -- it, kill it in the table and find the next best
    while true do
	-- pick the best of the remaining spells
	min = 0;
        for i = 1,analCount do
	    if (not analTable[i].killed) then
	    	if (min == 0) then
		    min = i;
	    	elseif ((analTable[i].cooldown + 0.010) <
	                                  analTable[min].cooldown) then
		    -- the 10 ms slop is here to catch the case where a
		    -- timer tick occurs while this routine is running, which
		    -- will cause us to flip between spells like we can't
		    -- make up our minds.  The tick appears to be about 1ms
		    -- (a little over), so 10ms should be more than plenty,
		    -- and still be fast enough not to matter to the user.
		    min = i;
		end
	    end
        end

	-- if min is 0, there's nothing they can cast 
	if (min == 0) then
	    if DestroDummies.debug then
		print("Nothing is castable");
	    end
	    return nil;
	end

	-- Can they cast this spell?
        local name,_,_,cost,_,ptype,_,_,_ = GetSpellInfo(analTable[min].spell);
	if (name == nil) then
	    -- they don't know this spell, kill it and pick the next
	    analTable[min].killed = true;
	    if DestroDummies.debug then
		print("Want to cast ", analTable[min].spell, 
		      ", but don't know it.");
	    end
	 else
	    -- look for spells that eat health instead of normal power
	    if (ptype == -2) then
	       if (UnitHealth("player") > cost) then
		  -- they can cast this.  return it
		  return analTable[min].spell, analTable[min].cooldown;
	       else
		  -- they don't have enough health.  kill it and try again
		  analTable[min].killed = true;
		  if DestroDummies.debug then
		     print("Want to cast ", analTable[min].spell, 
			   ", but don't have the health.");
		  end
	       end 
	    elseif (DestroDummies:unitPowerAfterCast() < cost) then
	       -- they don't have the mana.  return the alternate
	       if (powerAlt ~= nil) then
		  -- Is the powerAlt on cooldown and we have a free spell?
		  if ((freeSpell ~= nil) and 
		      (DestroDummies:cooldownTime(powerAlt) > 
		               DestroDummies:cooldownTime(freeSpell))) then
		     return freeSpell;
		  else
		     return powerAlt;
		  end
	       else
		  if DestroDummies.debug then
		     print("Want to cast ", analTable[min].spell, 
			   ", but we dinna have enough power (have ", 
			   DestroDummies:unitPowerAfterCast(), ", need ", 
			   cost, ")");
		  end
		  return nil;
	       end
	    else
	       -- return the spell they can cast
	       return analTable[min].spell, analTable[min].cooldown;
	    end
	 end 
    end
end


-- return the amount of time before you can cast the passed spell (in seconds)
function DestroDummies:cooldownTime(spell)
    local start, duration, enabled = GetSpellCooldown(spell);

    if (start == 0 or start == nil) then
        return 0;
    else
        return start + duration - GetTime();
    end
end


-- return the amount of power we will have at the end of the current cast
function DestroDummies:unitPowerAfterCast ()
   local power = UnitPower("player");

   -- What are we currently casting
   local name,_,_,_,_,endTime,isTrade = UnitCastingInfo("player");
   if (name and not isTrade) then
      -- How much more power will we have when we are done casting?
      local delta = (endTime / 1000) - GetTime();
      if (delta > 0) then 
	 local regenNorm, regenCombat = GetPowerRegen();
	 local regen;
	 if (UnitAffectingCombat("player")) then
	    regen = regenCombat;
	 else
	    regen = regenNorm;
	 end
	 power = power + (regen * delta);
      end
      if power > UnitPowerMax("player") then
	 power = UnitPowerMax("player");
      end 

      -- How much power will this take?
      local newName,_,_,cost = GetSpellInfo(name);
      if newName then	-- this should always be true
	 power = power - cost;
      end
   end
   return power;
end


--  Return a guess at the distance to the target, along with the min and max
--  possible distances
--
--  This is made considerably harder than it needs to be by Bliz preventing
--  GetPlayerMapPosition from working with the "target" unit.  We have to
--  rely on rough guesses
--
--  CheckInteractDistance
--	1: 28 yds
-- 	2: 11.1 yds (actually, 11.11, but we are *so* not that accurate)
--	3: 9.9 yds
--
--  We also have some spells with known ranges we can use to test
--
--  We will end up with a (hopefully narrow) range of distances the mob can
--  be at.  If the distance between us and the mob's target is in that range,
--  assume the mob is at exactly that distance (ie., it's on the tank).  
--  Otherwise return the average.
--
function DestroDummies:distanceToTarget()
    local minrange = 0;
    local maxrange = 100;

    if (CheckInteractDistance("target",3)) then
	maxrange = 9.9;
    else
	minrange = 10;
	if (CheckInteractDistance("target",2)) then
	    maxrange = 11.1;
	else
	    minrange = 11.2;
	    if (CheckInteractDistance("target",1)) then
		maxrange = 28;
	    else
		minrange = 28.1;
	    end
	end
    end

    -- Now walk our Ranged spells to see which can help fix the range
    local sn;
    for sn = 1,rangeSpells.count do
	-- for easier-to-read code
	local smin = rangeSpells[sn].min;
	local smax = rangeSpells[sn].max;

	if (smin > minrange or (smax > 0 and smax < maxrange)) then
	    local rng = IsSpellInRange(rangeSpells[sn].spell, "target");
	    -- range of nil is invalid range
	    if (rng ~= nil) then
		if (rng == 0) then
		    -- out of range.  If our window overlaps just one end
		    -- of the range, we can shift that end.  If it overlaps
		    -- both, then we don't actually know which side is off.
		    local smin_inrange, smax_inrange;
		    smin_inrange = ((smin >= minrange) and (smin <= maxrange));
		    smax_inrange = ((smax >= minrange) and (smax <= maxrange));

		    if (not smin_inrange and not smax_inrange) then
			-- we already knew this spell wouldn't work.  we
			-- shouldn't have bothered trying
		    elseif (smin_inrange and smax_inrange) then
			-- they could either be too close or too far.  maybe
			-- we should come up with a way to come back and try
			-- this again when the possible range has shrunk?
		    elseif (smin_inrange) then
			-- We know that the spell minimum was within our 
			-- range, but the spell max was not.  That means that
			-- we could not be too far away (the maximum is beyond
			-- our possible distance).  So the problem must be
			-- that we are too close).  That means that the max
			-- distance we can be is less than the current spells'
			-- minimum.  This sounds wrong but it is actually true.
			--
			maxrange = smin - 0.1;
		    else
			-- the inverse of the above
			minrange = smax + 0.1;
		    end
		else
		    -- Spell is in range
		    if (smin > minrange) then
			minrange = smin;
		    end
		    if (smax < maxrange and smax > 0) then
			maxrange = smax;
		    end
		end
	    end
	end
    end

    return (minrange + maxrange) / 2, minrange, maxrange;
end


-- Some spells (like Fishing) have very useful ranges but can't be used
-- for range checking, so they must be specifically excluded to keep them
-- from excluding other spells that would actually be useful
--
local ExcludeRangeSpells = {};
ExcludeRangeSpells[1] = "Fishing";
ExcludeRangeSpells.count = 0;

-- build our list of range-checking spells
function DestroDummies:initRangeCheckSpells()
    rangeSpells.count = 0;

    local i;
    for i = 1, MAX_SKILLLINE_TABS do
        local name, _, offset, numSpells = GetSpellTabInfo(i);
        
        if not name then
            break
        end

	-- for every spell on the tab
        for s = offset + 1, offset + numSpells do
            local spell, _ = GetSpellBookItemName(s, BOOKTYPE_SPELL);
	    if (spell == nil) then
		-- should never happen
		break;
	    end

	    local sname,_,_,_,_,_,_,min,max = GetSpellInfo(spell);
	    if (spell ~= sname) then
		-- should never happen
		break;
	    end

	    local xno;
	    local xclude_this = false;
	    for xno = 1,ExcludeRangeSpells.count do
		if (ExcludeRangeSpells[xno] == spell) then
		    xclude_this = true;
		end
	    end

	    if not xclude_this and min ~= nil and max ~= nil then
--[[
	    	local useful_min = true;
	    	local useful_max = true;
	    	local spellNo;
	    	for spellNo = 1,rangeSpells.count do
		    if (min == rangeSpells[spellNo].min) then
		    	useful_min = false;
		    end
		    if (max == rangeSpells[spellNo].max) then
		    	useful_max = false;
		    end
		    if (not useful_min) and (not useful_max) then
		    	break;
		    end
	        end
	        if useful_min or useful_max then
--]]
		local useful = true;
	    	local spellNo;
	    	for spellNo = 1,rangeSpells.count do
		    if (rangeSpells[spellNo].spell == spell) then
			useful = false;
			break;
		    end
		end
		if useful then
		    rangeSpells.count = rangeSpells.count + 1;
		    rangeSpells[rangeSpells.count] = {};
		    rangeSpells[rangeSpells.count].spell = spell;
		    rangeSpells[rangeSpells.count].min = min;
		    rangeSpells[rangeSpells.count].max = max;
		end
	    end
        end
    end
end
